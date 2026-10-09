# Rotary Inverted Pendulum – State Feedback Control

A control engineering project focused on the mathematical modeling, controllability analysis, and stabilization of a **rotary inverted pendulum** using state-space representation and state feedback control.

The project combines Euler–Lagrange modeling, linearization, pole placement, MATLAB, Simulink, and QUARC to investigate the stabilization of an inherently unstable mechanical system.

---

## Project Overview

As part of a control systems engineering project at **Tecnológico de Monterrey**, a rotary inverted pendulum was modeled and controlled using a state-space approach.

The system consists of a rotating arm driven by a motor and a pendulum attached to the arm. The objective is to stabilize the pendulum in its upright position by applying an appropriate motor voltage based on the system's state.

Because the upright equilibrium is inherently unstable, the controller must respond to deviations in the arm and pendulum positions to maintain balance.

The project involved:

* Mathematical modeling using the Euler–Lagrange formulation
* Nonlinear system modeling and linearization
* State-space representation
* Controllability analysis
* State feedback controller design
* Pole-placement methods
* MATLAB and Simulink implementation
* Experimental validation using a Quanser QUBE Servo 3 rotary pendulum

---

## Project Objectives

* Develop a mathematical model of the rotary inverted pendulum.
* Derive the system's equations of motion.
* Obtain a linearized state-space representation.
* Analyze the controllability of the system.
* Design a state feedback controller using pole placement.
* Calculate controller gains using different mathematical approaches.
* Implement the control strategy in MATLAB and Simulink.
* Test the controller on the physical experimental platform.
* Evaluate the system's stabilization performance and transient response.

---

## System Architecture

The experimental setup uses a **Quanser QUBE Servo 3 rotary pendulum**, which provides a motor-driven rotating arm and an attached pendulum.

The control architecture uses measured system states to calculate the motor input required to stabilize the pendulum.

### Physical System

The main components are:

* Rotary arm
* Inverted pendulum
* Motor and actuator
* Position measurement system
* Real-time control platform

The motor applies torque to the rotary arm, indirectly influencing the pendulum's movement. The controller uses the system's state information to generate the required control action.

### Control Software

The project uses the following software tools:

* **MATLAB** — Mathematical modeling, state-space analysis, and controller gain calculation.
* **Simulink** — Implementation and simulation of the control system.
* **QUARC** — Real-time execution and interaction with the experimental hardware.
* **Quanser QUBE Servo 3** — Physical rotary inverted pendulum platform.

---

## Mathematical Modeling

The system was modeled using the **Euler–Lagrange formulation**, which describes the relationship between the system's kinetic energy, potential energy, and generalized forces.

The resulting equations of motion describe the coupled dynamics of the rotating arm and the pendulum.

Because the original system is nonlinear, a linearized model was developed around the operating equilibrium to support the state feedback controller design.

---

## State-Space Representation

The system was represented using the standard state-space formulation:

$$
\dot{x}=Ax+Bu
$$

$$
y=Cx+Du
$$

where:

* \(x\) is the state vector.
* \(u\) is the motor input voltage.
* \(y\) is the measured output vector.
* \(A\) is the state matrix.
* \(B\) is the input matrix.
* \(C\) is the output matrix.
* \(D\) is the feedthrough matrix.

The state vector is defined as:

$$
x=
\begin{bmatrix}
\theta_r &
\dot{\theta}_r &
\theta_p &
\dot{\theta}_p
\end{bmatrix}^{T}
$$

where:

* \(\theta_r\) — Rotary arm angle.
* \(\dot{\theta}_r\) — Rotary arm angular velocity.
* \(\theta_p\) — Pendulum angle.
* \(\dot{\theta}_p\) — Pendulum angular velocity.

This representation allows the dynamic behavior of the system to be analyzed using linear control theory.

---

## Controllability Analysis

Controllability analysis was performed to determine whether the system's state could be driven to a desired configuration through the available motor input.

For a four-state system, the controllability matrix is defined as:

$$
\mathcal{C}=
\begin{bmatrix}
B & AB & A^2B & A^3B
\end{bmatrix}
$$

The system is controllable if:

$$
\operatorname{rank}(\mathcal{C})=4
$$

The analysis reported a controllability matrix rank of **4**, confirming that the linearized system is fully controllable.

This result supports the use of state feedback and pole-placement techniques for stabilizing the pendulum.

---

## State Feedback Controller

A state feedback controller was designed to stabilize the pendulum around its upright equilibrium.

The control law follows the general form:

$$
u=-Kx
$$

where \(K\) is the state feedback gain matrix.

The controller modifies the motor input according to the system's state, allowing the arm and pendulum dynamics to be influenced simultaneously.

### Pole Placement

Pole placement was used to select the desired closed-loop dynamics by assigning the eigenvalues of the closed-loop system.

The reported design specifications include:

| Parameter                 |             Target Value |
| ------------------------- | -----------------------: |
| Maximum overshoot         |                    6.81% |
| Settling time             |                   1.54 s |
| Desired complex pole pair | \(-2.5974 \pm j3.03708\) |

These specifications guided the controller design and the selection of the desired closed-loop pole locations.

---

## Controller Gain Calculation

Two approaches were investigated for calculating the state feedback gains:

* **Transformation matrix method**
* **Companion form method**

The resulting gain matrices were evaluated as part of the controller design and comparison process.

### Comparison Gain Matrix

$$
K_{\text{comparison}}=
\begin{bmatrix}
-2 & 50 & -1 & 3
\end{bmatrix}
$$

### Designed Gain Matrix

$$
K_{\text{designed}}=
\begin{bmatrix}
-4.4226 & 49.2709 & -1.8678 & 3.2597
\end{bmatrix}
$$

The comparison helps evaluate how different feedback gains influence the stabilization response and control effort.

---

## MATLAB & Simulink Implementation

MATLAB was used for the mathematical analysis and controller design, while Simulink provided a block-based environment for implementing the control strategy.

The implementation connects the state-space model and feedback controller to the system input.

The workflow includes:

1. Deriving the system's mathematical model.
2. Linearizing the equations around the operating equilibrium.
3. Constructing the state-space matrices.
4. Evaluating controllability.
5. Selecting desired closed-loop poles.
6. Calculating the feedback gain matrix.
7. Implementing the controller in Simulink.
8. Testing the control strategy using the experimental platform.

QUARC was used for real-time interaction with the Quanser hardware.

---

## Experimental Validation

The controller was tested using the physical rotary inverted pendulum platform.

The experiments investigated the system's ability to stabilize the pendulum and evaluated its transient behavior.

The reported results indicated:

* Successful pendulum stabilization.
* Low steady-state error.
* Improved transient response with the designed gain matrix.
* Differences in motor voltage effort between the evaluated controllers.

The designed controller provided a faster transient response in the reported comparison, although this improvement involved higher motor voltage effort.

---

## Results

The project demonstrated the application of state-space control theory to an inherently unstable mechanical system.

The main outcomes were:

* Development of a mathematical model for the rotary inverted pendulum.
* Linearization and state-space representation of the system.
* Verification of full controllability for the linearized model.
* Design of a state feedback controller using pole placement.
* Calculation of feedback gains using two different methods.
* Implementation of the control strategy in MATLAB and Simulink.
* Experimental stabilization using the Quanser QUBE Servo 3 platform.

The results illustrate the relationship between controller gain selection, closed-loop response, stabilization performance, and actuator effort.

---

## Hardware

### Experimental Platform

* Quanser QUBE Servo 3 rotary pendulum
* Motor-driven rotary arm
* Inverted pendulum
* Position measurement system
* Real-time control hardware

---

## Technologies

**MATLAB · Simulink · QUARC · Quanser QUBE Servo 3 · State-Space Modeling · Euler–Lagrange Equations · Linearization · Controllability · State Feedback · Pole Placement · Control Systems · Real-Time Control**

---

## Team

* José Pablo Rodríguez Pinal
* German Emiliano Rojas Lobatón
* Daniel Santiago Espinosa Cárdenas
* Daniel Andrés De La Ossa González
* Monserrath Sánchez Ruiz
