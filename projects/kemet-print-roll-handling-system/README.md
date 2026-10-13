# KEMET – Automated Print Roll Handling System

A mechanical design and prototyping project developed as part of the **Materials Analysis and Manufacturing** course at Tecnológico de Monterrey, in the context of an industry-oriented challenge with KEMET.

The project focused on designing a roll-handling system intended to improve the handling of printed materials used in capacitor manufacturing, with the goal of reducing manual inspection time and supporting the identification of material waste.

## Project Overview

The proposed system was designed to support the handling of heavy material rolls while providing a framework for future automated print counting and visual inspection.

The mechanical design was developed in **SolidWorks**, using iterative design improvements and static structural simulations to evaluate load-bearing performance and identify critical stress concentrations.

A physical prototype was also constructed to assess the mechanical arrangement, stability, and general functionality of the proposed design.

## Key Features

* **3D Mechanical Design:** Developed a mechanical assembly in SolidWorks for handling printed material rolls.
* **Structural Simulation:** Used SolidWorks Simulation to evaluate structural behavior under different loading conditions.
* **Iterative Design Optimization:** Identified critical areas in the initial design and introduced structural reinforcements and additional supports.
* **Load-Bearing Design:** Considered loads of up to 80 kg per support arm during the design and simulation process.
* **Physical Prototyping:** Built a prototype to evaluate component arrangement, mechanical stability, and overall feasibility.
* **Automation Concept:** Proposed a future vision-based inspection system using an ESP32-CAM and OpenCV for print-pattern analysis and error counting.

## Engineering Tools

| Tool                  | Application                                                  |
| --------------------- | ------------------------------------------------------------ |
| SolidWorks            | 3D modeling and mechanical assembly                          |
| SolidWorks Simulation | Static structural analysis and finite element analysis (FEA) |
| ESP32-CAM             | Proposed image acquisition system                            |
| Python / OpenCV       | Proposed image processing and visual inspection              |

## Design and Development Process

### 1. Concept Development

Two alternative design concepts were explored. The final approach prioritized a more compact configuration inspired by the existing equipment used by KEMET.

The design incorporated roll supports and an expanding arm intended to secure and stabilize the rolls during handling.

### 2. CAD Modeling and Structural Analysis

The mechanical assembly was developed in SolidWorks through an iterative design process.

Static simulations were used to investigate structural weaknesses under different loads. The results helped identify critical stress concentrations, particularly around support elements and structural connections.

The design was subsequently modified by reinforcing structural profiles, improving support elements, and adjusting the distribution of material.

### 3. Physical Prototyping

A physical prototype was manufactured to represent the final CAD design and evaluate the arrangement of its components, mechanical stability, and general operation.

The prototype also provided an opportunity to identify potential improvements before considering full-scale industrial manufacturing.

### 4. Computer Vision and Automated Inspection

A computer vision system was implemented and tested using an **ESP32-CAM** to capture images of printed material patterns.

The captured images were processed using **Python and OpenCV** to support pattern analysis and the identification of potential printing defects. The system was demonstrated operating as part of the project.

Although the source code is not included in this repository, a demonstration video documents the implemented system and its operation.

**Technologies:** ESP32-CAM, Python, OpenCV, computer vision, image processing.

### System Demonstration

A video demonstrating the operation of the ESP32-CAM-based vision system is available in the [`videos/`](videos/) directory.

### Key Results

* Developed a SolidWorks mechanical assembly and physical prototype.
* Applied finite element analysis to identify structural weaknesses and guide mechanical design improvements.
* Implemented and demonstrated an ESP32-CAM-based computer vision system.
* Combined mechanical design, structural simulation, prototyping, and image processing in an industrial engineering project.

## Results

* Developed a 3D mechanical design and physical prototype of a roll-handling system.
* Used finite element analysis to identify structural weaknesses and guide design improvements.
* Incorporated additional structural reinforcements to improve the design's expected stability under load.
* Established a conceptual approach for future automated visual inspection of printed materials.

The project demonstrated how CAD modeling, structural simulation, and physical prototyping can be combined to develop and improve an industrial equipment concept.


## Project Information

* **Course:** Materials Analysis and Manufacturing
* **Institution:** Tecnológico de Monterrey
* **Industry Partner:** KEMET
* **Project Date:** May 2025
* **Project Type:** Team-based engineering project

*This project was developed collaboratively. The repository presents the team's engineering work and the mechanical design and simulation activities to which I contributed.*
