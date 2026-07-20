/*
 * calibracion.c
 */

#include "calibration.h"
#include "string.h"

#define CALIBRATION_SAMPLES 1000  // Más muestras = mejor calibración

// Calibración del giroscopio (sensor DEBE estar COMPLETAMENTE QUIETO)
void Calibrate_Gyro(I2C_HandleTypeDef *hi2c, CalibrationData *cal) {
    MPU9250_Data imu;
    float gx_sum = 0, gy_sum = 0, gz_sum = 0;

    HAL_Delay(500);

    for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
        MPU9250_Read_All(hi2c, &imu);
        gx_sum += imu.gx;
        gy_sum += imu.gy;
        gz_sum += imu.gz;
        HAL_Delay(2);
    }

    cal->gx_offset = gx_sum / CALIBRATION_SAMPLES;
    cal->gy_offset = gy_sum / CALIBRATION_SAMPLES;
    cal->gz_offset = gz_sum / CALIBRATION_SAMPLES;
}

// Calibración del acelerómetro (sensor en superficie HORIZONTAL)
void Calibrate_Accel(I2C_HandleTypeDef *hi2c, CalibrationData *cal) {
    MPU9250_Data imu;
    float ax_sum = 0, ay_sum = 0, az_sum = 0;

    HAL_Delay(500);

    for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
        MPU9250_Read_All(hi2c, &imu);
        ax_sum += imu.ax;
        ay_sum += imu.ay;
        az_sum += imu.az;
        HAL_Delay(2);
    }

    cal->ax_offset = ax_sum / CALIBRATION_SAMPLES;
    cal->ay_offset = ay_sum / CALIBRATION_SAMPLES;
    cal->az_offset = (az_sum / CALIBRATION_SAMPLES) - 1.0f;  // -1g de gravedad
}
