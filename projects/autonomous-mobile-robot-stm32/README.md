# Autonomous Mobile Robot using STM32

An autonomous differential-drive mobile robot developed on an **STM32 NUCLEO-F103RB** platform. The robot integrates multiple sensors and control algorithms to perform accurate autonomous navigation, maintain a desired trajectory, avoid obstacles, and execute predefined paths.

---

## Overview

This project combines sensor fusion, fuzzy logic control, wheel odometry, and ultrasonic sensing to create a mobile robot capable of autonomous navigation.

The robot continuously estimates its orientation using an MPU9250 IMU and the Madgwick sensor fusion algorithm, while wheel encoders provide displacement feedback. A fuzzy logic controller adjusts the PWM signals sent to each motor in real time, compensating for heading deviations and maintaining the desired velocity.

Additionally, the robot detects nearby obstacles using an ultrasonic sensor and can execute predefined square trajectories through Bluetooth commands.

---

## Features

- Differential-drive autonomous mobile robot
- Fuzzy Logic speed and steering controller
- MPU9250 IMU integration
- Madgwick sensor fusion algorithm
- Wheel encoder odometry
- Ultrasonic obstacle detection
- Bluetooth communication (HC-05)
- Automatic IMU calibration
- Closed-loop heading correction
- Automatic square trajectory execution (3×3 m, 5×5 m, and 7×7 m)

---

## Hardware

- STM32 NUCLEO-F103RB
- MPU9250 9-axis IMU
- HC-SR04 Ultrasonic Sensor
- L298N Motor Driver
- HC-05 Bluetooth Module
- DC Motors with Quadrature Encoders
- Differential-drive mobile platform

---

## Software

- STM32CubeIDE
- STM32 HAL Drivers
- C Programming Language
- STM32CubeMX
- UART Communication
- PWM Motor Control
- I2C Communication

---

## Control Architecture

The control strategy combines multiple feedback sources:

- Wheel encoders estimate the traveled distance.
- The MPU9250 provides orientation measurements.
- The Madgwick filter estimates the robot heading (Yaw).
- A fuzzy logic controller computes the appropriate PWM duty cycle for each motor.
- The ultrasonic sensor prevents collisions with nearby obstacles.

This architecture allows the robot to maintain a straight trajectory despite motor asymmetries and external disturbances.

---

## Bluetooth Commands

| Command | Function |
|----------|----------|
| `1 - 10` | Move forward the specified distance (meters) |
| `A` | Execute a 3 × 3 meter square |
| `B` | Execute a 5 × 5 meter square |
| `C` | Execute a 7 × 7 meter square |
| `X` | Stop current movement |

---

## Results

The implemented system successfully demonstrates:

- Autonomous straight-line navigation
- Closed-loop heading correction
- Real-time speed regulation
- Obstacle detection and stopping
- Automatic square trajectory execution
- Bluetooth remote operation

---

## Demonstration

Project photos can be found in the **photos/** directory.

Videos demonstrating the robot operation are available in the **videos/** directory.

