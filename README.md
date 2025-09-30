# Dual-Axis Solar Tracker (Simulink + Tinkercad)

This project implements and models a **dual-axis solar tracking system** using:
- **MATLAB/Simulink** for system-level modeling and control design
- **Tinkercad (Arduino simulation)** for hardware prototyping with sensors and actuators

---

## Simulink Model

The main file is `solartracker.slx`, which contains the **system model** of the dual-axis tracker.  
Key features of the Simulink model:
- **Plant and controller separation** for clean modular design
- **Model Predictive Control (MPC)**-based tracking logic
- **Day/Night and safe/unsafe operating modes** implemented with Stateflow
- Inputs modeled as **LDR sensors** for sunlight intensity
- Outputs driving **two servo motors** for pan/tilt movement
- Performance metrics such as **energy efficiency and stability**

You can open the `.slx` file directly in MATLAB Simulink (tested on **R2024a**) to run simulations.

---

## Tinkercad Implementation

Alongside the Simulink modeling, the system was **implemented and simulated in Tinkercad** to prototype the Arduino-based hardware design.  
- **Components:** Arduino UNO, 4 × LDR sensors, 2 × servo motors, resistors, and wiring
- **Logic:** Arduino code calculates light intensity differences and adjusts servo angles to align the panel
- **Extra features:** energy tracking, safe mode (overheating/limit check), and LCD display for outputs

🔗 **Tinkercad project link:** [https://www.tinkercad.com/things/3aOzqTIOPHp/editel?sharecode=tt-Uel6SWCf_vfpTMM5noo_atUxuWAmXxymq8j4Sjrs]  

---

## How to Run

### Simulink
1. Open MATLAB.
2. Load the system model: `solartracker.slx`.
3. Run the simulation to observe servo response and energy tracking logic.

### Tinkercad
1. Open the provided [Tinkercad project link](https://www.tinkercad.com/things/3aOzqTIOPHp/editel?sharecode=tt-Uel6SWCf_vfpTMM5noo_atUxuWAmXxymq8j4Sjrs).
2. Upload the Arduino code (`tinkercad/arduino_code.ino`).
3. Simulate to visualize servo rotation and sensor response.

---

## Applications
- Solar energy optimization and renewable energy systems
- Automatic alignment of photovoltaic panels
- Educational demonstrations for control systems

---
