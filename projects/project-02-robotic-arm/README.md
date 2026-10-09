# Assistive Feeding Robot – 3-DOF Robotic Manipulator

A robotic assistance project focused on the design and simulation of a **3-degree-of-freedom (3-DOF) robotic manipulator** intended to assist people with upper-limb mobility limitations during feeding activities.

The project integrates robot kinematics, workspace analysis, dynamic modeling, motion control, computer vision, and robotic simulation to explore the feasibility of an assistive feeding system.

---

## Project Overview

As part of an engineering project at **Tecnológico de Monterrey**, a robotic manipulator was designed to support feeding tasks by transporting food between a plate and a user's mouth.

The system was modeled as a planar **RRR robotic manipulator**, consisting of three revolute joints. The design considers different target positions and movement configurations required to perform the feeding task.

The project combines mathematical modeling and simulation to analyze the robot's movement, evaluate its workspace, and investigate control strategies for smooth and controlled motion.

The main components of the project include:

* 3-DOF robotic manipulator design
* Forward and inverse kinematics
* Denavit–Hartenberg parameterization
* Workspace analysis
* Dynamic modeling using the Euler–Lagrange formulation
* PD control with gravity compensation
* Computer vision for papaya maturity classification
* Robotic simulation using RoboDK
* Safety considerations for assistive robotic applications

---

## Project Objectives

* Design a robotic manipulator capable of supporting feeding activities.
* Define the robot's kinematic structure and joint configuration.
* Calculate forward and inverse kinematics for target positioning.
* Analyze the robot's reachable workspace.
* Develop a dynamic model of the manipulator.
* Design a control strategy for the main joints.
* Investigate computer vision techniques for food maturity classification.
* Simulate the robot's movement between relevant feeding positions.
* Identify safety considerations associated with assistive robotic systems.

---

## System Architecture

The proposed system combines robotic motion, control, and computer vision to support an assistive feeding task.

### Robotic Manipulator

The robot is modeled as a **3-DOF RRR manipulator**, where each joint provides rotational movement.

The manipulator is designed to move between three main operational positions:

* **Rest position** — The initial or idle configuration.
* **Plate position** — The position used to approach and collect food.
* **Mouth position** — The target position for delivering food.

The kinematic model is used to determine the joint configurations required to reach these positions.

### Software Architecture

The project uses MATLAB and RoboDK for different aspects of the development process.

* **MATLAB** — Mathematical modeling, kinematics, workspace analysis, and control design.
* **RoboDK** — Robot configuration and motion simulation.
* **Computer Vision** — Classification of papaya maturity using image data.

---

## Robot Kinematics

Kinematic analysis establishes the relationship between joint variables and the position of the robot's end effector.

### Forward Kinematics

Forward kinematics calculates the end-effector pose from the joint angles.

The manipulator is described using the **Denavit–Hartenberg (DH) convention**, which provides a systematic way to represent the geometry and transformations between consecutive links.

Homogeneous transformation matrices are used to calculate the position and orientation of the end effector relative to the robot's base.

### Inverse Kinematics

Inverse kinematics determines the joint angles required to reach a specified target position.

The analysis considers alternative configurations, including **elbow-up and elbow-down solutions**, when applicable to the target position.

This allows the robot to select different joint configurations to reach a desired location within its workspace.

### Workspace Analysis

Workspace analysis evaluates the positions that can be reached by the manipulator based on its link geometry and joint configurations.

The reported maximum reach is approximately **906 mm**, providing a reference for evaluating the feasibility of the proposed feeding positions.

---

## Dynamic Modeling

The manipulator's dynamics were investigated using the **Euler–Lagrange formulation**.

This approach models the relationship between joint motion, applied torques, inertial effects, and gravitational forces.

The dynamic model provides a mathematical foundation for analyzing the behavior of the manipulator and designing a control strategy for its main joints.

The model was used to investigate the control of the first two joints, considering their contribution to the robot's positioning motion.

---

## Control System

A **Proportional–Derivative (PD) controller with gravity compensation** was considered for joint control.

The PD controller uses position error and its derivative to generate a control action, while gravity compensation accounts for gravitational effects in the modeled manipulator.

The reported controller gains are:

| Joint   | Proportional Gain (Kp) | Derivative Gain (Kd) |
| ------- | ---------------------: | -------------------: |
| Joint 1 |                     30 |                    5 |
| Joint 2 |                     20 |                    3 |

The controller design aims to improve positioning behavior and provide controlled movement between the target configurations.

---

## Computer Vision

Computer vision was incorporated into the project to investigate **papaya maturity classification**.

The objective is to use image data to distinguish maturity conditions relevant to food selection in the proposed assistive feeding application.

The computer vision component complements the robotic manipulator by exploring how visual information could support food-related decisions within the overall system.

The classification component was developed as part of the project, rather than as a clinically validated food-handling system.

---

## Robotic Simulation

RoboDK was used to represent and simulate the robotic manipulator and its movement.

The simulation supports the evaluation of robot configurations and target positions before considering physical implementation.

The main simulation objectives include:

* Representing the robot's kinematic structure.
* Evaluating the manipulator's reachable positions.
* Simulating movement between relevant target configurations.
* Supporting the analysis of the proposed feeding sequence.

---

## Safety Considerations

Because the proposed application involves interaction with a person, safety was considered as an important part of the system design.

The project reviewed standards and guidelines relevant to robotics and assistive applications, including:

* ISO 13482 — Safety requirements for personal care robots.
* ISO 10218 — Safety requirements for industrial robots.
* ISO/TS 15066 — Collaborative robot applications.
* BS 8611 — Guidance on the ethical design and application of robots.

These references informed the safety considerations discussed during the project. **The system was not certified against these standards, and the project did not establish clinical validation.**

---

## Technologies

**MATLAB · RoboDK · Robot Kinematics · Denavit–Hartenberg Convention · Inverse Kinematics · Euler–Lagrange Modeling · PD Control · Gravity Compensation · Computer Vision · Robotic Simulation**

---

## Project Limitations

The project focused on mathematical modeling, software development, and simulation of the proposed assistive feeding system.

The work did not constitute a fully validated physical feeding robot. Additional development would be required to address hardware implementation, real-time perception, food manipulation, human–robot interaction, and experimental safety validation.

---

## Team

* José Pablo Rodríguez Pinal
* Verónica Ixtchel Aculco Alva
* Maximiliano Mireles Escobar
* Gabriela Lizeth Peña Zuñiga
