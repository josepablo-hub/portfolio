/*
 * ultrasonico.h
 */
#ifndef INC_ULTRASONICO_H_
#define INC_ULTRASONICO_H_

#include "stm32f1xx_hal.h" // sitienes otra familia de micro reemplazas el f3 por f4 ejemplo.
// aqui cambian ppor los pines que utilizen
//*************************************************
#define TRIG_PIN GPIO_PIN_11 // PA9 = D8
#define TRIG_PORT GPIOB
#define ECHO_PIN GPIO_PIN_14 // PA8 = D7
#define ECHO_PORT GPIOB
//*************************************************
//funcion que inicia el timmer.
void ultrasonic_init(void);
//logica de medicion distancias.
uint16_t ultrasonic_measure_distance(void);

#endif /* INC_ULTRASONICO_H_ */

