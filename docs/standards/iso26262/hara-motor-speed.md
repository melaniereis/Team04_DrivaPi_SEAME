# HARA: Motor Speed Monitoring System

## 1. Item Definition & Assumptions
- **Item:** DrivaPi Speed Sensor Software Component.
- **Function:** Reads inputs from a hall-effect encoder, calculates linear velocity, and reports it to the main control loop/dashboard.
- **Operational Context:** Lab environment or controlled test track.
- **Assumption:** The vehicle is small-scale (RC type) or educational; operators are trained to recognize mechanical noise.

## 2. Hazard Analysis
We identified one primary hazard related to the speed sensing logic.

| ID | Hazard Description | Operational Situation | Cause of Hazard |
|:---|:---|:---|:---|
| **H-01** | **False Zero Speed Indication** (System reports 0 m/s while moving) | **Maintenance / Tuning:** Operator is close to the vehicle, relying on the dashboard/logs to determine if it is safe to touch the wheels/drivetrain. | Software bug (e.g., variable overflow, division by zero crash, or stuck logic) causing the reported speed to freeze at 0. |

## 3. Risk Assessment (ISO 26262)

### Severity (S)
* **Score:** **S1** (Light to moderate injuries)
* **Rationale:** The DrivaPi electric motors have torque. If an operator touches a spinning wheel/gear believing it is stopped, it may cause friction burns, cuts, or bruises. Life-threatening injuries are not expected given the motor size.

### Exposure (E)
* **Score:** **E2** (Low probability, <10%)
* **Rationale:** Operators usually rely on visual confirmation (looking at the wheel). Relying *solely* on the software value happens primarily during remote debugging or specific calibration tasks.

### Controllability (C)
* **Score:** **C1** (Simply controllable)
* **Rationale:** A moving motor generates noise and vibration. A trained operator can easily perceive the discrepancy between "Dashboard says 0" and "Motor is screaming" and avoid touching it.

### Calculation
**S1 + E2 + C1 = ASIL A**
*(Based on project-specific tailoring of ISO 26262 Part 3 Table 4)*

## 4. Safety Goals (SG)
Based on H-01, we derive the following Safety Goal for the software:

| SG ID | ASIL | Description |
|:---|:---|:---|
| **SG-01** | **A** | **Prevent False Zero Speed Reporting:** The software must detect invalid states (like timer overflows or timeouts) and calculate speed accurately, or explicitly flag an error state instead of failing silently to 0. |
