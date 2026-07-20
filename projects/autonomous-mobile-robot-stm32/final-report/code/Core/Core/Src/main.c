#include "main.h"
#include "ultrasonico.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "mpu9250.h"
#include "kalmanfilter.h"
#include "fuzzylogic.h"
#include "calibration.h"

/* Periféricos --------------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* Prototipos HAL -----------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);

/* Prototipos de funciones de usuario --------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

static void checar_boton_calibracion(void);
static void actualizar_imu(void);
static void mover_distancia_controlada(void);

/* Prototipos para rutinas de giro y cuadrados */
static void reset_yaw_ref(void);
static float get_yaw_rel(void);
static void girar_90_derecha(void);
static void iniciar_trayectoria(float metros);
static void rutina_cuadrado(float lado_m);


/* Definiciones de movimiento ----------------------------------------------*/
#define ADELANTE 1
#define ATRAS    2
#define ALTO     3
#define giroizq  4
#define giroder  5

/* Geometría / encoders -----------------------------------------------------*/
#define DIAMETRO_RUEDA         0.06612f
#define PULSOS_POR_VUELTA      485.0f
#define PI_F                   3.1416f
#define CIRCUNFERENCIA_RUEDA  (DIAMETRO_RUEDA * PI_F)
#define METROS_POR_PULSO      (CIRCUNFERENCIA_RUEDA / PULSOS_POR_VUELTA)
//#define PULSOS_POR_METRO       2335
#define DISTANCIA_OBSTACULO_MM 200
#define ULTRA_MAX_MM            3000  // arriba de 3 m lo consideramos ruido
#define OBST_CONSEC_MIN         3     // nº de lecturas seguidas para aceptar obstáculo


#define RX_LINE_LEN  32
#define E_GIRO_OFFSET_DEG   1.5f   // offset inicial para compensar deriva a la derecha




/* Variables globales -------------------------------------------------------*/
volatile uint16_t distancia = 0;
char buffer[128];

volatile uint16_t pwm_last_L = 0;
volatile uint16_t pwm_last_R = 0;


volatile unsigned int trayectoria = 0;

volatile float distancia_objetivo      = 0.0f;
volatile float distancia_recorrida_izq = 0.0f;
volatile float distancia_recorrida_der = 0.0f;

volatile int32_t pulsos_encoder_izq = 0;
volatile int32_t pulsos_encoder_der = 0;
volatile int32_t pulsos_objetivo    = 0;

uint8_t rxData;
volatile char  rx_line[RX_LINE_LEN];
volatile uint8_t rx_index = 0;

/* IMU / Filtro / Fuzzy -----------------------------------------------------*/
MPU9250_Data imu;
CalibrationData imuCal;
uint8_t imu_calibrado = 0;
float yaw_deg   = 0.0f;
float pitch_deg = 0.0f;
float roll_deg  = 0.0f;
uint32_t last_imu_ms = 0;

FuzzySystem fuzzy;
float velocidad_deseada_mps = 0.4f;   // <--- AQUI CAMBIAS LA VELOCIDAD DESEADA
float velocidad_actual_mps  = 0.0f;
uint32_t last_vel_ms       = 0;
int32_t  last_encoder_prom = 0;


/* Control de yaw relativo y rutinas de cuadrado */
float base_yaw = 0.0f;                 // referencia para giros de 90°
volatile uint8_t rutina_pendiente = 0; // 0 = ninguna, 1=3x3, 2=5x5, 3=7x7


/* Prototipo porque no está en kalmanfilter.h */
void Madgwick_Reset(void);

/* ========================= FUNCIONES BASE (DEL CODE QUE JALABA) ========================= */

float absf32(float x)
{
    return (x < 0.0f) ? -x : x;
}

void muevete(int direccion)
{
    switch(direccion)
    {
        case ADELANTE:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
            break;

        case ATRAS:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
            break;

        case ALTO:
        default:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
            break;
    }
}

void muevete1(int direccion)
{
    switch(direccion)
    {
        case ADELANTE:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
            break;

        case ATRAS:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
            break;

        case ALTO:
        default:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
            break;
    }
}

void giro(int direccion)
{
    switch(direccion)
    {
        case giroizq:
            muevete(ADELANTE);
            muevete1(ATRAS);
            break;

        case giroder:
            muevete(ATRAS);
            muevete1(ADELANTE);
            break;

        default:
            muevete(ALTO);
            muevete1(ALTO);
            break;
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_6)
    {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8))
            pulsos_encoder_izq--;
        else
            pulsos_encoder_izq++;
    }
    else if (GPIO_Pin == GPIO_PIN_7)
    {
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
            pulsos_encoder_der--;
        else
            pulsos_encoder_der++;
    }
}

void calcular_distancias_ruedas(void)
{
    distancia_recorrida_izq = absf32((float)pulsos_encoder_izq) * METROS_POR_PULSO;
    distancia_recorrida_der = absf32((float)pulsos_encoder_der) * METROS_POR_PULSO;
}

float calcular_distancia_promedio(void)
{
    return (distancia_recorrida_izq + distancia_recorrida_der) / 2.0f;
}

float obtener_diferencia_ruedas(void)
{
    return distancia_recorrida_izq - distancia_recorrida_der;
}

void resetear_encoders(void)
{
    pulsos_encoder_izq      = 0;
    pulsos_encoder_der      = 0;
    distancia_recorrida_izq = 0.0f;
    distancia_recorrida_der = 0.0f;
}

/* ========================= INICIAR TRAYECTORIA LINEAL ========================= */

static void iniciar_trayectoria(float metros)
{
    distancia_objetivo = metros;
    pulsos_objetivo = (int32_t)((metros / METROS_POR_PULSO) + 0.5f);

    resetear_encoders();

    // *** NUEVO: referencia de yaw = rumbo actual ***
    reset_yaw_ref();

    // RESET de la parte de velocidad antes de esta nueva trayectoria
    last_vel_ms          = 0;
    last_encoder_prom    = 0;
    velocidad_actual_mps = 0.0f;

    trayectoria = 1;

    muevete(ADELANTE);
    muevete1(ADELANTE);

    sprintf(buffer, "Moviendo %.2f m (%ld pulsos objetivo)\r\n",
            distancia_objetivo, (long)pulsos_objetivo);
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}

//void procesar_linea_recibida(void)
//{
//    char linea[RX_LINE_LEN];
//    uint8_t i;
//
//    for (i = 0; i < rx_index && i < RX_LINE_LEN-1; i++)
//        linea[i] = rx_line[i];
//    linea[i] = '\0';
//
//    rx_index = 0;
//
//    if ((linea[0] == 'X' || linea[0] == 'x') && linea[1] == '\0')
//    {
//        trayectoria = 0;
//        muevete(ALTO);
//        muevete1(ALTO);
//        sprintf(buffer, "Movimiento cancelado\r\n");
//        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
//        return;
//    }
//
//    float metros = atof(linea);
//
//    if (metros < 1.0f || metros > 10.0f)
//    {
//        sprintf(buffer, "Distancia invalida: %.2f\r\n", metros);
//        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
//        return;
//    }
//
//    distancia_objetivo = metros;
//    pulsos_objetivo = (int32_t)(metros * (float)PULSOS_POR_METRO + 0.5f);
//
//    resetear_encoders();
//    trayectoria = 1;
//
//    muevete(ADELANTE);
//    muevete1(ADELANTE);
//
//    sprintf(buffer, "Moviendo %.2f m (%ld pulsos objetivo)\r\n",
//            distancia_objetivo, (long)pulsos_objetivo);
//    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
//}

void procesar_linea_recibida(void)
{
    char linea[RX_LINE_LEN];
    uint8_t i;

    for (i = 0; i < rx_index && i < RX_LINE_LEN-1; i++)
        linea[i] = rx_line[i];
    linea[i] = '\0';

    rx_index = 0;

    // Cancelar movimiento actual
    if ((linea[0] == 'X' || linea[0] == 'x') && linea[1] == '\0')
    {
        trayectoria = 0;
        muevete(ALTO);
        muevete1(ALTO);
        sprintf(buffer, "Movimiento cancelado\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
        return;
    }

    // Rutinas de cuadrado por letras: A=3x3, B=5x5, C=7x7
    if (linea[1] == '\0')
    {
        if (linea[0] == 'A' || linea[0] == 'a')
        {
            rutina_pendiente = 1;
            sprintf(buffer, "Rutina cuadrado 3x3 pendiente\r\n");
            HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
            return;
        }
        if (linea[0] == 'B' || linea[0] == 'b')
        {
            rutina_pendiente = 2;
            sprintf(buffer, "Rutina cuadrado 5x5 pendiente\r\n");
            HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
            return;
        }
        if (linea[0] == 'C' || linea[0] == 'c')
        {
            rutina_pendiente = 3;
            sprintf(buffer, "Rutina cuadrado 7x7 pendiente\r\n");
            HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
            return;
        }
    }

    // Si no es X ni A/B/C, asumimos que es una distancia en metros
    float metros = atof(linea);

    if (metros < 1.0f || metros > 10.0f)
    {
        sprintf(buffer, "Distancia invalida: %.2f\r\n", metros);
        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
        return;
    }

    iniciar_trayectoria(metros);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        char c = (char)rxData;

        if (c == '\r' || c == '\n')
        {
            if (rx_index > 0)
            {
                procesar_linea_recibida();
            }
        }
        else
        {
            if (rx_index < RX_LINE_LEN-1)
            {
                rx_line[rx_index++] = c;
            }
            else
            {
                rx_index = 0;
            }
        }

        HAL_UART_Receive_IT(&huart1, &rxData, 1);
    }
}

/* ========================= IMU: CALIBRACION + ACTUALIZACION ========================= */

void checar_boton_calibracion(void)
{
    // En la Nucleo, el botón azul normalmente está en HIGH y al presionarlo va a LOW
    static uint8_t last_state = GPIO_PIN_SET;
    uint8_t state = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);

    // Flanco de bajada (soltando -> presionando)
    if (last_state == GPIO_PIN_SET && state == GPIO_PIN_RESET)
    {
        trayectoria = 0;
        muevete(ALTO);
        muevete1(ALTO);

        // --- Mensaje GIROSCOPIO ---
        // Tus funciones de calibración tardan ~0.5 s + 1000*2 ms ≈ 2.5 s
        // Redondeamos a 3 s para que el mensaje tenga sentido.
        snprintf(buffer, sizeof(buffer),
                 "\r\n=== CALIBRACION IMU ===\r\n"
                 "1) GIROSCOPIO: deja el carrito COMPLETAMENTE QUIETO ~3 s...\r\n");
        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

        Calibrate_Gyro(&hi2c1, &imuCal);

        // --- Mensaje ACELEROMETRO ---
        snprintf(buffer, sizeof(buffer),
                 "2) ACELEROMETRO: coloca el carrito en superficie HORIZONTAL y QUIETA ~3 s...\r\n");
        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

        Calibrate_Accel(&hi2c1, &imuCal);

        // Reiniciar filtro para que el yaw arranque desde cero después de calibrar
        Madgwick_Reset();
        imu_calibrado = 1;
        last_imu_ms = 0;   // para que el siguiente actualizar_imu recalcule bien dt

        snprintf(buffer, sizeof(buffer),
                 ">>> Calibracion completa. Reanudando medicion de YAW.\r\n\r\n");
        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
    }

    last_state = state;
}

/* ========================= YAW RELATIVO PARA GIROS ========================= */

static void reset_yaw_ref(void)
{
    base_yaw = yaw_deg;
}

static float get_yaw_rel(void)
{
    // Diferencia bruta
    float delta = yaw_deg - base_yaw;

    // Normalizar a rango [-180, 180] para evitar saltos grandes por el wrap
    while (delta > 180.0f)
        delta -= 360.0f;
    while (delta < -180.0f)
        delta += 360.0f;

    return delta;   // este es el yaw relativo "de verdad"
}




void actualizar_imu(void)
{
    uint32_t now = HAL_GetTick();
    if (last_imu_ms == 0)
    {
        last_imu_ms = now;
        return;
    }

    uint32_t dt_ms = now - last_imu_ms;
    if (dt_ms < 200)   // ~20 Hz
        return;

    float dt = dt_ms / 1000.0f;
    last_imu_ms = now;

    // Leer datos crudos del MPU
    MPU9250_Read_All(&hi2c1, &imu);

    float ax = imu.ax;
    float ay = imu.ay;
    float az = imu.az;
    float gx = imu.gx;
    float gy = imu.gy;
    float gz = imu.gz;

    if (imu_calibrado)
    {
        gx -= imuCal.gx_offset;
        gy -= imuCal.gy_offset;
        gz -= imuCal.gz_offset;
        ax -= imuCal.ax_offset;
        ay -= imuCal.ay_offset;
        az -= imuCal.az_offset;
    }

    MadgwickAHRSupdate(gx, gy, gz, ax, ay, az,
                       0.0f, 0.0f, 0.0f,
                       dt);

    Madgwick_GetAngles(&yaw_deg, &pitch_deg, &roll_deg);

    // Escalar a entero x100 para imprimir sin %f
    int16_t yaw100  = (int16_t)(yaw_deg * 100.0f);
    int16_t vel100  = (int16_t)(velocidad_actual_mps * 100.0f);

    // Distancia promedio en metros -> también x100
    calcular_distancias_ruedas();
    float   dist_m   = calcular_distancia_promedio();
    int16_t dist100  = (int16_t)(dist_m * 100.0f);

    snprintf(buffer, sizeof(buffer),
    		"%.2f %.2f %.3f %u %u\r\n",
             yaw100  / 100,  abs(yaw100  % 100),
             vel100  / 100,  abs(vel100  % 100),
             dist100 / 100,  abs(dist100 % 100),
             (unsigned int)pwm_last_L,
             (unsigned int)pwm_last_R);

    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY); // BT
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY); // TeraTerm

//             "Yaw=%d.%02d deg | V=%d.%02d m/s | Dist=%d.%02d m | PWM_L:%u | PWM_R:%u\r\n""%.2f %.2f %.3f %u %u\r\n",
}

/* ========================= GIRO 90° A LA DERECHA ========================= */

static void girar_90_derecha(void)
{
    // Referencia de yaw para este giro (0° en la orientación actual)
    reset_yaw_ref();

    // Dirección de giro a la DERECHA (motores contrarrotando)
    giro(giroder);

    // PWM fuerte para el giro (80%)
    uint8_t duty_giro = 80;
    TIM3->CCR1 = ((TIM3->ARR) + 1) * duty_giro / 100;
    TIM3->CCR3 = ((TIM3->ARR) + 1) * duty_giro / 100;

    // Tiempo mínimo de giro (en ms) – evita que un pico raro de yaw corte el giro demasiado pronto
    uint32_t t0 = HAL_GetTick();
    const uint32_t MIN_TIEMPO_GIRO_MS = 500;  // 0.5 s mínimo girando

    while (1)
    {
        actualizar_imu();

        float yaw_rel = get_yaw_rel();  // cuánto hemos girado desde que empezamos este giro

        // Condición de salida:
        // - haber girado ~90° (usamos 80° como margen)
        // - Y además haber estado girando al menos MIN_TIEMPO_GIRO_MS
        if ((HAL_GetTick() - t0) >= MIN_TIEMPO_GIRO_MS &&
            fabsf(yaw_rel) >= 84.0f)
        {
            break;
        }

        HAL_Delay(10);
    }

    // Detener motores al terminar el giro
    muevete(ALTO);
    muevete1(ALTO);
    TIM3->CCR1 = 0;
    TIM3->CCR3 = 0;
}


/* ========================= TELEMETRIA CONTINUA ========================= */

void enviar_telemetria(void)
{
    static uint32_t last_tx = 0;

    if (HAL_GetTick() - last_tx >= 200)  // cada 200 ms (ajusta si quieres)
    {
        snprintf(buffer, sizeof(buffer),
                 "Yaw=%.2f deg | V=%.2f m/s | PWM_L:%u | PWM_R:%u\r\n",
                 yaw_deg,
                 velocidad_actual_mps,
				 (unsigned int)pwm_last_L,
				 (unsigned int)pwm_last_R);


        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

        last_tx = HAL_GetTick();
    }
}




/* ========================= MOVER DISTANCIA + CONTROL DIFUSO ========================= */

void mover_distancia_controlada(void)
{
    // Distancias por rueda (en metros)
    calcular_distancias_ruedas();
    float distancia_promedio_m = calcular_distancia_promedio();
    float diferencia_ruedas    = obtener_diferencia_ruedas();
    (void)diferencia_ruedas;   // por ahora no lo usamos

    // Pulsos promedio (para condición de fin y velocidad)
    int32_t enc_izq_abs  = abs(pulsos_encoder_izq);
    int32_t enc_der_abs  = abs(pulsos_encoder_der);
    int32_t encoder_prom = (enc_izq_abs + enc_der_abs) / 2;

    // -------------------- FIN DE RECORRIDO (POR PULSOS) --------------------
    if (encoder_prom >= pulsos_objetivo)
    {
        muevete(ALTO);
        muevete1(ALTO);
        trayectoria = 0;

        snprintf(buffer, sizeof(buffer),
                 "Movimiento completado: dist=%.3f m (objetivo=%.3f) | enc=%ld / %ld\r\n",
                 distancia_promedio_m,
                 distancia_objetivo,
                 (long)encoder_prom,
                 (long)pulsos_objetivo);
        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
        return;
    }

//     -------------------- ULTRASONICO --------------------
    distancia = ultrasonic_measure_distance();
    float distancia_obstaculo_m = distancia / 1000.0f;

    if (distancia < DISTANCIA_OBSTACULO_MM)
    {
        muevete(ALTO);
        muevete1(ALTO);

        snprintf(buffer, sizeof(buffer),
                 "Obstaculo: %.3f m\r\n",
                 distancia_obstaculo_m);
        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
        return;
    }

    // -------------------- SIN OBSTACULO: AVANZAR --------------------
    muevete(ADELANTE);
    muevete1(ADELANTE);

    // ----- CÁLCULO DE VELOCIDAD REAL (m/s) -----
    uint32_t now = HAL_GetTick();

    if (last_vel_ms == 0)
    {
        last_vel_ms          = now;
        last_encoder_prom    = encoder_prom;
        velocidad_actual_mps = 0.0f;
    }
    else
    {
        uint32_t dt_ms = now - last_vel_ms;
        if (dt_ms >= 50)
        {
            int32_t delta_pulsos = encoder_prom - last_encoder_prom;
            float   dist         = (float)delta_pulsos * METROS_POR_PULSO;
            float   dt_s         = dt_ms / 1000.0f;

            if (dt_s > 0.0f)
                velocidad_actual_mps = dist / dt_s;
            else
                velocidad_actual_mps = 0.0f;

            last_vel_ms       = now;
            last_encoder_prom = encoder_prom;
        }
    }


    // ----- ENTRADAS A LA LOGICA DIFUSA -----
    float e_vel = velocidad_deseada_mps - velocidad_actual_mps;
    if (e_vel > 0.3f)  e_vel = 0.3f;
    if (e_vel < -0.3f) e_vel = -0.3f;

    float e_giro;
    if (!imu_calibrado)
    {
        e_giro = 0.0f;
    }
    else
    {
        float yaw_rel = get_yaw_rel();   // Yaw relativo al inicio de la recta
        // AQUÍ PONES TU AJUSTE MANUAL (por ejemplo +0.8 o lo que te funcionó):
        e_giro = yaw_rel + 0.15f;        // <-- ajusta este número a lo que tú ya viste que endereza

        if (e_giro >  90.0f) e_giro =  90.0f;
        if (e_giro < -90.0f) e_giro = -90.0f;
    }

    // ----- SALIDAS DIFUSAS: PWM DERECHO / IZQUIERDO -----
    uint16_t pwmR = fuzzy_motor_derecho(&fuzzy, e_vel, e_giro);
    uint16_t pwmL = fuzzy_motor_izquierdo(&fuzzy, e_vel, e_giro);

    // Limitar a rango útil físico
    if (pwmR > 100) pwmR = 100;
    if (pwmR < 35)  pwmR = 35;
    if (pwmL > 100) pwmL = 100;
    if (pwmL < 35)  pwmL = 35;

    TIM3->CCR1 = ((TIM3->ARR) + 1) * pwmL / 100;  // motor izquierdo
    TIM3->CCR3 = ((TIM3->ARR) + 1) * pwmR / 100;  // motor derecho

    // Guardar últimos PWM para telemetría
    pwm_last_L = pwmL;
    pwm_last_R = pwmR;

    // ----- FEEDBACK OPCIONAL POR BT (USART1) -----
    static uint32_t last_feedback = 0;
    if (HAL_GetTick() - last_feedback > 1000)
    {
        snprintf(buffer, sizeof(buffer),
                 "Prog: %.3f/%.2f m, YAW=%.2f, V=%.2f m/s, pwmL=%u, pwmR=%u\r\n",
                 distancia_promedio_m,
                 distancia_objetivo,
                 yaw_deg,              // aquí sigues viendo yaw global en TeraTerm
                 velocidad_actual_mps,
                 pwmL, pwmR);
        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
        last_feedback = HAL_GetTick();
    }
}



/* ========================= RUTINA DE CUADRADO ========================= */

static void rutina_cuadrado(float lado_m)
{
    for (int k = 0; k < 4; k++)
    {
        // Iniciar un lado
        iniciar_trayectoria(lado_m);

        // Avanzar hasta completar la trayectoria (usa tu lógica difusa normal)
        while (trayectoria == 1)
        {
            checar_boton_calibracion();
            actualizar_imu();
            mover_distancia_controlada();
            HAL_Delay(10);
        }

        // Llegó a la esquina: alto y esperar 5 s
        muevete(ALTO);
        muevete1(ALTO);
        HAL_Delay(5000);

        // Giro 90° a la DERECHA
        girar_90_derecha();
        HAL_Delay(500);  // pequeña pausa extra
    }

    sprintf(buffer, "Rutina de cuadrado (%.2f m) completada\r\n", lado_m);
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}


/* ========================= MAIN ========================= */

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_TIM2_Init();
    MX_USART1_UART_Init();
    MX_I2C1_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();

    /* PWM base (se ajusta luego por fuzzy) */
    uint8_t duty = 80;
    TIM3->CCR1 = ((TIM3->ARR) + 1) * duty / 100;
    TIM3->CCR3 = ((TIM3->ARR) + 1) * duty / 100;
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

    /* Ultrasonico */
    ultrasonic_init();

    /* UART BT */
    HAL_UART_Receive_IT(&huart1, &rxData, 1);
    sprintf(buffer, "BT listo\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);


    /* IMU */
    if (MPU9250_Init(&hi2c1) == HAL_OK)
        sprintf(buffer, "IMU inicializada\r\n");
    else
        sprintf(buffer, "ERROR IMU\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

    /* Fuzzy */
    fuzzy_init(&fuzzy);

    sprintf(buffer,
            "Sistema listo. Envia distancia 1-10 m por BT.\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

    while (1)
    {
        checar_boton_calibracion();
        actualizar_imu();
//        enviar_telemetria();   // <-- FUNCION NUEVA que manda telemetría SIEMPRE

        // Si hay una rutina pendiente (A/B/C), ejecutarla
        if (rutina_pendiente != 0)
        {
            uint8_t r = rutina_pendiente;
            rutina_pendiente = 0;   // limpiar bandera

            if (r == 1)
                rutina_cuadrado(3.0f);
            else if (r == 2)
                rutina_cuadrado(5.0f);
            else if (r == 3)
                rutina_cuadrado(7.0f);
        }

        // Modo normal: avanzar la distancia que se mandó como número
        if (trayectoria == 1)
        {
            mover_distancia_controlada();
        }

        HAL_Delay(10);
    }

}

/* ========================= CONFIGURACIONES CUBEMX ========================= */

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 63;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim1);
}

static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 63;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 199;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_TIM3_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 63;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 199;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim3);
}

static void MX_TIM4_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 63;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* LED2 + motor izq */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|output__motores_lado_izq_Pin, GPIO_PIN_RESET);

  /* Motores der + trigger */
  HAL_GPIO_WritePin(GPIOB, output__motores_lado_izqB10_Pin|Trigger_Pin|
                          output__motores_lado_der_Pin|output__motores_lado_derB5_Pin,
                          GPIO_PIN_RESET);

  /* Botón azul + Canal A encoder derecho (PC) */
  GPIO_InitStruct.Pin = B1_Pin|Canal_A_Encoder_Derecho_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* LED2 + salida motores lado izq (PA) */
  GPIO_InitStruct.Pin = LD2_Pin|output__motores_lado_izq_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Canal A encoder izquierdo */
  GPIO_InitStruct.Pin = Canal_A_del_Encoder_Izquierdo_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Canal_A_del_Encoder_Izquierdo_GPIO_Port, &GPIO_InitStruct);

  /* Salidas motores + trigger ultrasonico (PB) */
  GPIO_InitStruct.Pin = output__motores_lado_izqB10_Pin|Trigger_Pin|
                        output__motores_lado_der_Pin|output__motores_lado_derB5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Echo ultrasonico + canal B encoder derecho (PB) */
  GPIO_InitStruct.Pin = Echo_Pin|Canal_B_Encoder_Derecho_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Canal B encoder izquierdo */
  GPIO_InitStruct.Pin = Canal_B_Enconder_Izquierdo_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Canal_B_Enconder_Izquierdo_GPIO_Port, &GPIO_InitStruct);

  /* EXTI init */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  (void)file;
  (void)line;
}
#endif
