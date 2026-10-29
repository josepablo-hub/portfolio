# Automated Sorting Line — Digital Twin & PackML

**Industrial Automation | PLC Programming | Digital Twin | Virtual Commissioning**

An automated conveyor sorting system developed using Rockwell Automation Emulate3D, Studio 5000 Logix Designer, and FactoryTalk Logix Echo. The project integrates a virtual industrial environment with PLC-based control logic and a PackML state-based operating architecture.

---

## Overview

The objective of this project was to develop and validate an automated material-handling system capable of sorting boxes into six designated outputs according to their assigned colors.

The system uses a virtual distance sensor to identify boxes based on their height. A PLC program processes the sensor measurements, determines the corresponding box type, and controls the appropriate stopper and pneumatic piston to divert each box to its assigned destination.

The digital twin was developed in Emulate3D and connected to a virtual PLC running through FactoryTalk Logix Echo. Communication between the virtual environment and the controller was established using EtherNet/IP.

The machine's operating sequence was structured around the PackML standard, providing a state-based approach to machine operation and control.

---

## System Architecture

The project integrates three main components:

| Component | Function |
|---|---|
| **Emulate3D** | Digital twin of the conveyor sorting system, including virtual sensors and actuators |
| **Studio 5000 Logix Designer** | PLC programming using Ladder Logic |
| **FactoryTalk Logix Echo** | Execution of the control program on a virtual PLC |
| **EtherNet/IP** | Communication between the digital twin and the virtual controller |
| **PackML** | State-based organization of machine operation |

### Communication Architecture

The digital twin and the virtual PLC exchange sensor signals, operating states, and actuator commands through EtherNet/IP.

- **Sensors:** Configured to write their values to the PLC.
- **Actuators:** Configured to read control commands from the PLC.
- **PLC:** Processes the incoming signals and executes the programmed control sequence.

This architecture enables the virtual machine and its control logic to operate synchronously.

---

## Automated Sorting Process

The sorting sequence consists of the following steps:

1. **Box release:** Boxes are introduced into the conveyor system.
2. **Presence detection:** A virtual presence sensor detects when a box enters the measurement area.
3. **Height measurement:** A virtual distance sensor measures the height of the box.
4. **Box classification:** The PLC evaluates the measured distance using `LIM` instructions and assigns an integer identifier to `BoxType`.
5. **Station detection:** Collision sensors detect the box at its corresponding sorting station.
6. **Stopper activation:** The appropriate stopper is activated to position the box for sorting.
7. **Pneumatic diversion:** A `TON` timer introduces a short delay before the corresponding pneumatic piston extends and pushes the box toward its designated output.
8. **Cycle completion:** The `CycleDone` signal resets the internal control bits associated with the completed sorting cycle.

The system supports six sorting destinations corresponding to the following box colors:

- Green
- Yellow
- Red
- Blue
- Purple
- Orange

---

## Distance-Based Box Identification

To simulate color detection without using a physical color sensor, each box was assigned a different height.

A virtual distance sensor measured the height of each box, and the PLC used the measured distance to determine its corresponding type and sorting destination.

The classification logic uses `LIM` instructions to evaluate measurement ranges. When a measurement falls within a defined range, a `MOV` instruction assigns the corresponding integer identifier to `BoxType`.

Examples of the configured measurement ranges documented in the project:

| Box Color | Distance Range |
|---|---|
| Green | 0.18–0.20 m |
| Blue | 0.20–0.22 m |
| Yellow | 0.25–0.26 m |

The complete classification logic includes six box types.

**Note:** Height measurement is used as a simulation mechanism to represent color-based identification. The system does not rely on an actual color sensor.

---

## PLC Control Logic

The control program was developed in Studio 5000 Logix Designer using Ladder Logic.

The main sections of the program include:

| Program Section | Function |
|---|---|
| Conveyor start/stop | Controls conveyor operation using a seal-in circuit |
| Box release | Provides six separate controls for releasing loads |
| Presence detection | Generates `DetectTrigger` when a box enters the measurement area |
| Distance classification | Evaluates distance ranges and assigns `BoxType` |
| Stopper control | Activates the appropriate stopper when a box reaches its station |
| Pneumatic actuation | Uses `TON` timers to delay piston activation until the box is positioned |
| Cycle completion | Activates `CycleDone` and clears latched control bits using `OTU` instructions |

### Key Control Tags

| Tag | Data Type | Description |
|---|---|---|
| `ConveyorRun` | BOOL | Controls conveyor movement |
| `Distance` | REAL | Distance measured by the virtual sensor |
| `BoxPresent` | BOOL | Indicates box presence in the detection area |
| `BoxType` | INT | Identifies the classified box type |
| `DetectTrigger` | BOOL | Initiates the classification sequence |
| `CycleDone` | BOOL | Indicates completion of the sorting cycle |
| `Collision_1` – `Collision_6` | BOOL | Detects box presence at the sorting stations |
| `Stopper_1` – `Stopper_6` | BOOL | Controls the six physical stoppers in the simulation |
| `Piston_1` – `Piston_6` | BOOL | Controls the six pneumatic pistons |
| `StopperCmd_1` – `StopperCmd_6` | BOOL | Internal commands for stopper activation |

---

## PackML State-Based Control

The machine's operating architecture was developed following the principles of **ANSI/ISA TR88.00.02-2015 (PackML)**, with particular emphasis on the PRODUCTION mode and standardized machine states.

The implementation includes the following states:

| State | Purpose |
|---|---|
| `IDLE` | Represents the idle state |
| `STARTING` | Represents the machine startup sequence |
| `EXECUTE` | Represents active machine operation |
| `COMPLETING` | Represents the completion phase |
| `COMPLETE` | Represents completion of the operation |
| `RESETTING` | Represents the reset sequence |
| `STOPPED` | Represents the stopped state |
| `ABORTED` | Represents the aborted state |

The state-based architecture provides a structured approach to machine operation, supporting clearer control logic, troubleshooting, and future scalability.

---

## Digital Twin and Virtual PLC Integration

The digital twin was developed in Emulate3D and connected to the control program running on a virtual PLC through FactoryTalk Logix Echo.

The integration was validated by verifying:

- Successful execution of the Ladder Logic program on the virtual PLC.
- Real-time updates of PLC tags.
- Communication of virtual sensor measurements to the controller.
- Transmission of actuator commands to the digital twin.
- Activation of the corresponding stoppers and pneumatic pistons.
- Synchronization between box detection, classification, and sorting operations.

The virtual environment made it possible to test the control sequence without requiring a physical conveyor system or physical PLC hardware.

---

## Engineering Challenges and Solutions

### 1. Sensor Synchronization and Cycle Management

Momentary sensor signals and load detection required careful coordination with the PLC control sequence.

**Solution:** Latched bits and a cycle-completion signal were used to maintain the necessary control information throughout the sorting cycle and reset it when the operation finished.

### 2. Actuator Orientation

Some virtual stoppers responded in the opposite direction due to their mechanical orientation in the Emulate3D model.

**Solution:** The internal control logic was separated from the physical actuator outputs, allowing signal inversion to be handled at the output layer without modifying the main control sequence.

### 3. PLC-to-Digital-Twin Communication

Reliable synchronization between the virtual sensors, PLC tags, and actuators was essential for correct sorting behavior.

**Solution:** EtherNet/IP communication was configured between Emulate3D and FactoryTalk Logix Echo, enabling bidirectional data exchange between the virtual machine and its controller.

---

## Technologies and Engineering Skills

- **PLC Programming:** Studio 5000 Logix Designer, Ladder Logic
- **Digital Twins:** Rockwell Automation Emulate3D
- **Virtual Commissioning:** FactoryTalk Logix Echo
- **Industrial Communication:** EtherNet/IP
- **Machine Control:** PackML state-based architecture
- **Sensors and Actuators:** Virtual distance sensors, presence sensors, stoppers, and pneumatic pistons
- **Control Logic:** Timers, range comparison instructions, latching, and cycle management
- **Industrial Automation:** Conveyor systems and automated material handling

---

## Project Demonstration

The demonstration video illustrates the digital twin, the sorting sequence, and the PLC control logic.

Recommended visual documentation includes:
- An overview of the complete sorting line.
- The distance-based box identification mechanism.
- The activation of the corresponding stoppers and pneumatic pistons.
- Selected Ladder Logic sections from Studio 5000.
- The communication between Emulate3D and FactoryTalk Logix Echo.

---

## Conclusion

This project demonstrated the integration of a digital twin with PLC-based control and a PackML state-based operating architecture.

By combining Emulate3D, Studio 5000 Logix Designer, FactoryTalk Logix Echo, and EtherNet/IP, the system successfully reproduced an automated sorting process in a virtual industrial environment.

The project also addressed practical control challenges involving sensor synchronization, latched signals, actuator orientation, and cycle management.

Overall, the work provided hands-on experience in industrial automation, virtual PLC execution, digital twin integration, and structured machine control, demonstrating how automated systems can be developed and validated in simulation before physical implementation.

---

## Project Information

Course: Automation Manufacturing Systems

Institution: Tecnológico de Monterrey, Campus Monterrey
Date: May 2026

Developed by:
- José Pablo Rodríguez Pinal
