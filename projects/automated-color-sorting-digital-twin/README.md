
# Automated Color Sorting Line — Digital Twin

A PLC-controlled automated sorting line developed using **Rockwell Automation Emulate3D**, **Studio 5000 Logix Designer**, and **SolidWorks**.

---

## Overview

This project consists of a simulated industrial sorting line designed to automatically classify and route boxes into six different outputs according to their assigned colors:

- 🟢 Green
- 🟡 Yellow
- 🔴 Red
- 🔵 Blue
- 🟣 Purple
- 🟠 Orange

The system integrates a digital twin, PLC control logic, distance-based object identification, and pneumatic actuators to simulate an automated material-handling process.

---

## System Functionality

The sorting process follows these steps:

1. **Box identification:** Each box is assigned a different height corresponding to its designated color.
2. **Distance measurement:** A simulated distance sensor measures the height of each box.
3. **PLC-based decision-making:** The PLC interprets the measured distance to determine the box's corresponding color and sorting destination.
4. **Stopper activation:** The appropriate stopper is activated for the selected sorting station.
5. **Pneumatic diversion:** The corresponding pneumatic cylinder pushes the box toward its designated output.
6. **Automated sorting:** Each box is routed to the correct destination according to the programmed control logic.

---

## Color Detection Mechanism

To simulate color detection, each box was assigned a different height. A distance sensor measured the height of each box, allowing the PLC to identify its corresponding color based on the measured distance.

The PLC control logic then activated the appropriate stopper and pneumatic cylinder to divert each box to its designated sorting output.

This approach simulated color-based sorting using height measurements as a proxy for color detection, without requiring an actual color sensor.

---

## Technologies Used

| Technology | Application |
| :--- | :--- |
| **Rockwell Automation Emulate3D** | Digital twin development and industrial process simulation |
| **Studio 5000 Logix Designer** | PLC programming and control logic |
| **SolidWorks** | Mechanical CAD modeling and design |

---

## Engineering Concepts

- Industrial automation and PLC-based control
- Digital twins and virtual industrial environments
- Conveyor systems and automated material handling
- Sensor-based object identification
- Pneumatic actuation and sequential control
- Integration of mechanical design, simulation, and control logic

---

## Repository Structure

```text
automated-color-sorting-digital-twin/
│
├── README.md
│
├── Emulate3D/
│   └── Digital twin project files
│
├── SolidWorks/
│   ├── Parts/
│   └── Assemblies/
│
├── PLC/
│   └── Studio 5000 project files
│
└── Media/
    ├── images/
    └── demo-video.mp4
```

*Note: The directory structure above represents the proposed organization. Adapt it to the files actually included in the repository.*

---

## Project Demonstration

The `Media/` directory can contain screenshots and a demonstration video illustrating:

- The overall sorting line and digital twin.
- The box identification and sorting sequence.
- The stopper and pneumatic cylinder operation.
- The PLC control logic.
- The mechanical CAD design.

---

## Project Objective

The objective was to develop and simulate an automated material-handling system capable of sorting boxes into six different outputs.

The project demonstrates how sensor measurements, PLC-based decision-making, pneumatic actuators, and a digital twin can be integrated to reproduce an industrial sorting process in a virtual environment.

---

## Key Takeaways

This project provided practical experience in:

- PLC-based industrial automation.
- Digital twin development and simulation.
- Sensor integration and measurement-based identification.
- Pneumatic actuator control.
- Mechanical design and automated material handling.

---
