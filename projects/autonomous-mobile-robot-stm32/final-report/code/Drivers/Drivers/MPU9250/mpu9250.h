/*
 * mpu9250.h (compatible con MPU6050)
 */

#ifndef MPU9250_H
#define MPU9250_H

#include "stm32f1xx_hal.h"

#define MPU9250_ADDR     (0x68 << 1)  // Dirección I2C del MPU6050

typedef struct {
    float ax, ay, az;  // Acelerómetro (g)
    float gx, gy, gz;  // Giroscopio (rad/s)
    float mx, my, mz;  // Magnetómetro (no disponible en MPU6050)
} MPU9250_Data;

HAL_StatusTypeDef MPU9250_Init(I2C_HandleTypeDef *hi2c);
void MPU9250_Read_All(I2C_HandleTypeDef *hi2c, MPU9250_Data *data);

#endif
