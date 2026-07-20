/*
 * mpu9250.c (adaptado para MPU6050)
 */
#include "mpu9250.h"
#include <math.h>

#define PWR_MGMT_1   0x6B
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_XOUT_H 0x3B
#define WHO_AM_I     0x75

static void read_bytes(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t reg, uint8_t *buf, uint8_t len) {
    HAL_I2C_Mem_Read(hi2c, addr, reg, 1, buf, len, HAL_MAX_DELAY);
}

static void write_byte(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t reg, uint8_t data) {
    HAL_I2C_Mem_Write(hi2c, addr, reg, 1, &data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU9250_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t check;
    HAL_StatusTypeDef status;

    // Leer WHO_AM_I
    status = HAL_I2C_Mem_Read(hi2c, MPU9250_ADDR, WHO_AM_I, 1, &check, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) return HAL_ERROR;

    // MPU6050 = 0x68
    if (check != 0x68) return HAL_ERROR;

    // Despertar el sensor
    write_byte(hi2c, MPU9250_ADDR, PWR_MGMT_1, 0x00);
    HAL_Delay(100);

    // Configuración
    write_byte(hi2c, MPU9250_ADDR, CONFIG, 0x01);        // Filtro pasa-bajos 184Hz
    write_byte(hi2c, MPU9250_ADDR, GYRO_CONFIG, 0x00);   // ±250 °/s
    write_byte(hi2c, MPU9250_ADDR, ACCEL_CONFIG, 0x00);  // ±2g

    return HAL_OK;
}

void MPU9250_Read_All(I2C_HandleTypeDef *hi2c, MPU9250_Data *data) {
    uint8_t buf[14];
    read_bytes(hi2c, MPU9250_ADDR, ACCEL_XOUT_H, buf, 14);

    int16_t ax = (buf[0] << 8) | buf[1];
    int16_t ay = (buf[2] << 8) | buf[3];
    int16_t az = (buf[4] << 8) | buf[5];
    int16_t gx = (buf[8] << 8) | buf[9];
    int16_t gy = (buf[10] << 8) | buf[11];
    int16_t gz = (buf[12] << 8) | buf[13];

    // Convertir a unidades físicas
    data->ax = ax / 16384.0f;  // g
    data->ay = ay / 16384.0f;
    data->az = az / 16384.0f;
    data->gx = gx / 131.0f * M_PI / 180.0f;  // rad/s
    data->gy = gy / 131.0f * M_PI / 180.0f;
    data->gz = gz / 131.0f * M_PI / 180.0f;

    // MPU6050 no tiene magnetómetro
    data->mx = 0.0f;
    data->my = 0.0f;
    data->mz = 0.0f;
}
