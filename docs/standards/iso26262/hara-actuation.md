# HARA: Vehicle Actuation System

## 1. Item Definition
- **Item:** Vehicle Actuation System.
- **Function:** Converts digital control commands into physical movement via PWM signals.

## 2. Hazard Analysis

| ID | Hazard | Situation | Cause |
|:---|:---|:---|:---|
| **H-02** | **Unintended Acceleration** | Vehicle stopped or moving. | Software calculates invalid DC Motor PWM, causing uncommanded motion. |
| **H-03** | **Unintended Steering** | Vehicle moving at speed. | Software calculates invalid Servo PWM, causing sudden steering lock. |

## 3. Risk Assessment (ISO 26262)

### H-02: Unintended Acceleration
* **S1 (Light Injury):** Small scale vehicle, low mass.
* **E4 (High Exposure):** Propulsion is used continuously.
* **C2 (Normally Controllable):** Operator can use E-Stop or brake.
* **Result:** **ASIL A**

### H-03: Unintended Steering
* **S1 (Light Injury):** potential collision with obstacles.
* **E4 (High Exposure):** Steering active continuously.
* **C2 (Normally Controllable):** Visual feedback allows operator reaction.
* **Result:** **ASIL A**

## 4. Safety Goals (SG)

| SG ID | Linked Hazard | ASIL | Description |
|:---|:---|:---|:---|
| **SG-02** | H-02 | **A** | **Safe Propulsion:** The DC Motor logic must clamp speed requests to +/- 100% and map 0.0 to a guaranteed stop state. |
| **SG-03** | H-03 | **A** | **Safe Steering:** The Servo logic must clamp angle requests to [0, 180] degrees to prevent mechanical binding/locking. |
