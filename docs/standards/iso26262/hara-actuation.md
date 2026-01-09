# HARA: Vehicle Actuation (Servo/Motor)


## 1. Item Definition
- **Item:** Vehicle Actuation System (Propulsion & Steering).
- **Function:** Converts digital control commands into physical movement via PWM signals.

## 2. Hazard Analysis
| ID | Hazard | Situation | Cause |
|:---|:---|:---|:---|
| **H-02** | **Unintended Actuation** | Vehicle in motion. | Software calculates invalid PWM pulse, causing unintended acceleration or steering lock. |

## 3. Risk Assessment (ISO 26262)
* **Severity (S): S1** (Light to moderate injury). The vehicle is small-scale; impact is painful but not life-threatening.
* **Exposure (E): E4** (High probability). Actuation logic runs continuously during operation.
* **Controllability (C): C2** (Normally controllable). Operator can use E-Stop, but a run-away vehicle requires faster reaction than a passive failure.

### Calculation
**S1 + E4 + C2 = ASIL A**
*(Source: ISO 26262-3:2018 Table 4)*

## 4. Safety Goals (SG)
| SG ID | ASIL | Description |
|:---|:---|:---|
| **SG-02** | **A** | **Safe Actuation Ranges:** The software must clamp all actuation commands to defined safe physical limits (0-100% or 0-180°) before signal generation. |