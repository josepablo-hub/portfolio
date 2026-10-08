# Automated Manufacturing Cell – Rockwell Automation

An industrial manufacturing cell automation project developed in collaboration with **Rockwell Automation**, integrating PLC-based process control, machine safety, industrial networking, robotics, machine vision, conveyors, pneumatic actuators, and production monitoring.

The project focused on automating the transportation, manipulation, assembly, and inspection of a manufactured component within an integrated production cell.

---

## Project Overview

As part of the **Manufacturing Systems Automation** course at Tecnológico de Monterrey, a multi-team project was developed to automate an industrial manufacturing cell in collaboration with Rockwell Automation.

The objective was to coordinate multiple automation systems into a single production sequence while maintaining safe and reliable operation.

The cell integrates:

- PLC-based process control
- Safety PLC
- Industrial Ethernet communication
- Conveyor systems
- Pneumatic actuators
- FANUC industrial robots
- Cognex machine vision
- Variable frequency drives
- HMI / SCADA monitoring
- Industrial safety devices

The project required coordination between multiple teams responsible for different subsystems of the manufacturing cell.

---

## Project Objectives

- Implement the required manufacturing sequence.
- Coordinate multiple PLCs and automation subsystems.
- Integrate robot, conveyor, pneumatic, and vision systems.
- Implement industrial communication using EtherNet/IP.
- Detect and handle process faults.
- Implement machine safety and emergency-stop behavior.
- Coordinate the different project teams into a single functional cell.
- Validate the complete automated manufacturing sequence.

---

## System Architecture

The manufacturing cell uses a distributed control architecture based on Allen-Bradley GuardLogix controllers.

### Process Control

Two **Compact GuardLogix 5069-L306ERS2** controllers are used for process automation:

- **PLC 1** — Controls the first stage of the manufacturing process.
- **PLC 2** — Controls the downstream process after robot assembly.

### Safety Control

A **GuardLogix 1756-L73S** controller is dedicated to machine safety.

The Safety PLC supervises safety devices such as:

- Emergency stops
- Interlocks
- Safety light curtains
- Safety I/O
- Power isolation

This architecture separates safety functions from standard process control.

---

## Industrial Network

The cell uses **EtherNet/IP** as the primary industrial communication protocol.

The network integrates:

- PLC 1
- PLC 2
- Safety PLC
- POINT I/O
- POINT Guard I/O
- PowerFlex 525 drives
- FANUC robot systems
- Cognex vision systems

The network enables the exchange of process states, synchronization signals, robot status, inspection triggers, and safety conditions.

---

## Automation Systems

### PLC Control

The PLCs coordinate the complete manufacturing sequence, including:

- Conveyor movement
- Pneumatic actuators
- Part positioning
- Robot synchronization
- Vision inspection
- Line switching
- Fault handling
- Process states

### Conveyor Systems

Two conveyors transport parts through the manufacturing cell.

Sensors and pneumatic actuators are used to:

- Detect parts
- Position components
- Control spacing
- Prevent collisions
- Transfer parts between conveyor sections

### FANUC Robot

The PLC communicates with the FANUC robot to coordinate:

1. Part availability
2. Robot cycle initiation
3. Part pickup
4. Assembly operation
5. Robot completion confirmation
6. Safe robot clearance

### Cognex Vision

The vision system is integrated into the process through trigger signals from the PLC.

The PLC coordinates:

- Inspection position
- Camera trigger
- Lighting activation
- Inspection timing
- Process continuation

---

## Machine Safety

Safety was implemented as an independent control layer using a dedicated Safety PLC.

The system monitors emergency stops, interlocks, and other safety devices.

When an unsafe condition is detected:

1. Safety outputs are disabled.
2. Process motion is stopped.
3. The PLC receives the safety status.
4. The system remains stopped until the fault is corrected.
5. A manual reset is required before restarting the process.

Process interlocks were also implemented to prevent actuators from operating under invalid conditions.

---

## Control Sequence

The manufacturing sequence is divided into multiple states.

### PLC 1

The first PLC manages:

1. Idle state
2. Part feeding
3. Initial positioning
4. Vision inspection sequence
5. Robot transfer
6. Line change
7. Base management
8. Fault / emergency stop

### PLC 2

The second PLC manages the downstream process:

1. Initial state
2. Conveyor operation
3. Line change
4. Camera positioning
5. Vision inspection
6. Final transfer

This state-based architecture allows the system to coordinate multiple devices while maintaining clear process conditions and interlocks.

---

## SCADA & Process Monitoring

Process variables were prepared for integration with a SCADA system.

The system includes variables for:

- Process progress
- Completed parts
- Approved parts
- Rejected parts
- Cycle time
- Individual process-stage timing

These variables allow production performance and potential bottlenecks to be monitored.

---

## Hardware

### Controllers

- Allen-Bradley Compact GuardLogix 5069-L306ERS2
- Allen-Bradley GuardLogix 1756-L73S

### I/O

- POINT I/O
- POINT Guard I/O
- 1734-AENTR/B
- 1734-IB8/C
- 1734-OB8/C
- 1734-OW4/C
- 1734-IB8S/B
- 1734-OB8S/B

### Motion & Actuation

- PowerFlex 525 Variable Frequency Drives
- Conveyor motors
- Pneumatic cylinders
- Pneumatic valves
- Industrial sensors
- Limit switches

### Robotics & Vision

- FANUC industrial robot
- Cognex vision system
- Industrial lighting system

---

## My Contribution

This was a collaborative team project involving multiple automation subsystems and project teams.

My specific contribution to the project included:

- [ADD YOUR SPECIFIC CONTRIBUTION HERE]
- [ADD YOUR SPECIFIC CONTRIBUTION HERE]
- [ADD YOUR SPECIFIC CONTRIBUTION HERE]

The project required coordination between the PLC, robotics, vision, safety, and manufacturing subsystems.

---

## Results

The automated manufacturing cell successfully completed the integrated production sequence.

Testing verified:

- Communication between the three PLC controllers.
- Coordinated operation of both conveyors.
- Pneumatic actuator operation.
- Synchronization with the FANUC robot.
- Integration of the Cognex vision system.
- Safety system response.
- Coordinated process operation.

The final system achieved stable and coordinated operation of the transportation, manipulation, inspection, and safety functions of the manufacturing cell.

---

## Project Gallery

Project photographs are available in the `photos/` directory.

The gallery includes:

- Manufacturing cell
- PLC hardware
- Industrial network
- Conveyors
- Pneumatic systems
- FANUC robot
- Cognex vision system
- Testing and commissioning

---

## System Diagrams

Technical diagrams are available in the `diagrams/` directory, including:

- Control architecture
- Industrial network architecture
- Process flow
- Control sequence

---

## Repository Structure

```text
rockwell-automated-manufacturing-cell/
│
├── README.md
│
├── code/
│   ├── plc-1/
│   ├── plc-2/
│   └── plc-safety/
│
├── photos/
│   ├── manufacturing-cell/
│   ├── plc/
│   ├── robot/
│   ├── vision/
│   ├── conveyors/
│   └── testing/
│
├── diagrams/
│   ├── control-architecture.png
│   ├── network-architecture.png
│   ├── process-flow.png
│   └── sequence-diagram.png
│
├── documents/
│   ├── final-report.pdf
│   └── presentation.pdf
│
└── videos/
    └── README.md
```

---

## Technologies

**Allen-Bradley · GuardLogix · Studio 5000 · EtherNet/IP · POINT I/O · PowerFlex · FANUC · Cognex · PLC · SCADA · HMI · Industrial Automation · Machine Safety**
