/*
 * kalmanfilter.h
 *
 *  Created on: Nov 13, 2025
 *      Author: L03504926
 */

#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <math.h>

void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az,
                        float mx, float my, float mz, float dt);
void Madgwick_GetAngles(float *yaw, float *pitch, float *roll);

#endif
