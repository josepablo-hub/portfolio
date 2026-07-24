# Arduino Goalkeeper Robot

An interactive electromechanical goalkeeper robot developed using Arduino. The system allows a user to manually control the goalkeeper's horizontal movement with a joystick while activating a solenoid-based kicking mechanism to block incoming shots.

This project integrates mechanical design, embedded programming, electronics, and rapid prototyping into a fully functional mechatronic system.

---

## Overview

The objective of this project was to design and build an interactive robotic goalkeeper capable of moving laterally along the goal while simulating a kicking motion using a solenoid actuator.

The system combines an Arduino-based control system, a stepper motor with linear motion transmission, a custom electronic circuit, and 3D-printed mechanical components.

---

## Features

- Manual joystick control
- Stepper motor linear positioning
- Solenoid-based kicking mechanism
- Custom electronic controller
- Limit switch homing
- 3D printed mechanical components
- Modular electromechanical design

---

## Hardware

- Arduino Uno
- Stepper Motor
- A4988 Stepper Driver
- Solenoid Actuator
- Joystick Module
- Limit Switches
- Power Supply
- Custom Mechanical Assembly

---

## Software

- Arduino IDE
- C/C++
- Stepper Motor Control
- Digital and Analog I/O

---

## Electronics

The electronic system consists of:

- Arduino Uno
- A4988 Stepper Driver
- Solenoid Driver Circuit
- Limit Switches
- Joystick Interface

A complete wiring diagram is included in the **photos/** directory.

---

## My Contributions

During this project I was responsible for:

- Mechanical CAD design of the solenoid housing
- Mechanical CAD design of the controller enclosure
- Electronic system integration
- Arduino programming
- Electrical wiring and assembly
- System integration and testing

---

## System Architecture

The controller reads the joystick position and generates movement commands for the stepper motor through the A4988 driver. Limit switches are used for homing and position reference, while a push button activates the solenoid mechanism that simulates the goalkeeper kick.

The mechanical assembly combines linear motion, custom 3D-printed components, and electronic control to provide smooth goalkeeper movement.

---

## Repository Structure

```
code/
│
├── Arduino source code

photos/
│
├── Prototype
├── CAD Models
├── Electronics
├── Assembly
├── Testing

documents/
│
├── Final Report
└── Presentation

videos/
│
└── Demonstration videos
```

---

## Results

The final prototype successfully demonstrates:

- Stable joystick-controlled motion
- Reliable linear positioning
- Functional solenoid actuation
- Complete integration of electronics, mechanics, and software

---

## Gallery

Project images are available in the **photos/** directory.

---

## Documentation

- Final Report
- Project Presentation

---

## Demonstration

Videos showing the complete system operation are available in the **videos/** directory.
