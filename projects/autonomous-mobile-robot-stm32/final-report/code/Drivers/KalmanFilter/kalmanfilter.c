/*
 * kalmanfilter.c
 */
#include "kalmanfilter.h"

static float beta = 0.1f;  // Ganancia del filtro
static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;

void MadgwickAHRSupdate(float gx, float gy, float gz,
                        float ax, float ay, float az,
                        float mx, float my, float mz,
                        float dt)
{
    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float _2q0 = 2.0f * q0;
    float _2q1 = 2.0f * q1;
    float _2q2 = 2.0f * q2;
    float _2q3 = 2.0f * q3;
    float _2q0q2 = 2.0f * q0 * q2;
    float _2q2q3 = 2.0f * q2 * q3;
    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    if ((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))
        return;

    recipNorm = 1.0f / sqrtf(ax * ax + ay * ay + az * az);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;

    s0 = 4.0f * q0 * q2q2 + 2.0f * q2 * ax + 4.0f * q0 * q1q1 - 2.0f * q1 * ay;
    s1 = 4.0f * q1 * q3q3 - 2.0f * q3 * ax + 4.0f * q0q0 * q1 - 2.0f * q0 * ay - 4.0f * q1 + 8.0f * q1 * q1q1 + 8.0f * q1 * q2q2 + 4.0f * q1 * az;
    s2 = 4.0f * q0q0 * q2 + 2.0f * q0 * ax + 4.0f * q2 * q3q3 - 2.0f * q3 * ay - 4.0f * q2 + 8.0f * q2 * q1q1 + 8.0f * q2 * q2q2 + 4.0f * q2 * az;
    s3 = 4.0f * q1q1 * q3 - 2.0f * q1 * ax + 4.0f * q2q2 * q3 - 2.0f * q2 * ay;
    recipNorm = 1.0f / sqrtf(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
    s0 *= recipNorm;
    s1 *= recipNorm;
    s2 *= recipNorm;
    s3 *= recipNorm;

    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz) - beta * s0;
    qDot2 = 0.5f * ( q0 * gx + q2 * gz - q3 * gy) - beta * s1;
    qDot3 = 0.5f * ( q0 * gy - q1 * gz + q3 * gx) - beta * s2;
    qDot4 = 0.5f * ( q0 * gz + q1 * gy - q2 * gx) - beta * s3;

    q0 += qDot1 * dt;
    q1 += qDot2 * dt;
    q2 += qDot3 * dt;
    q3 += qDot4 * dt;

    recipNorm = 1.0f / sqrtf(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;
}

void Madgwick_GetAngles(float *yaw, float *pitch, float *roll)
{
    *yaw   = atan2f(2.0f * (q1 * q2 + q0 * q3),
                    q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 180.0f / M_PI;
    *pitch = -asinf(2.0f * (q1 * q3 - q0 * q2)) * 180.0f / M_PI;
    *roll  = atan2f(2.0f * (q0 * q1 + q2 * q3),
                    q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3) * 180.0f / M_PI;
}

// Resetear el yaw a 0 (mantiene pitch y roll)
void Madgwick_Reset(void)
{
    q0 = 1.0f;
    q1 = 0.0f;
    q2 = 0.0f;
    q3 = 0.0f;
}
