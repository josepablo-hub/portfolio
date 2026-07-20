/*
 * calibracion.h
 */

#ifndef CALIBRACION_H
#define CALIBRACION_H

#include "stm32f1xx_hal.h"
#include "mpu9250.h"

typedef struct {
    float gx_offset;
    float gy_offset;
    float gz_offset;
    float ax_offset;
    float ay_offset;
    float az_offset;
} CalibrationData;

void Calibrate_Gyro(I2C_HandleTypeDef *hi2c, CalibrationData *cal);
void Calibrate_Accel(I2C_HandleTypeDef *hi2c, CalibrationData *cal);

#endif
