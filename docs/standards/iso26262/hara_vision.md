# HARA: Computer Vision System

## 1. Item Definition & Assumptions
- **Item:** Hailo Pipeline for Lane and Obstacle Recognition.
- **Function:** Processes camera feeds for lane detection and main obstacle recognition.
- **Operational Context:** Autonomous driving on marked paths; lab environment.
- **Assumption:** Backup sensors (ultrasonic) available; low vehicle mass reduces impact risk.

## 2. Hazard Analysis
We identified one primary hazard related to vision processing.

| ID | Hazard Description | Operational Situation | Cause of Hazard |
|:---|:---|:---|:---|
| **H-05** | **Lane Misdetection** (System fails to detect lane markings accurately) | Vehicle in autonomous lane-following mode. | Low light, errors in pipeline, or low confidence causing drift into hazards. |

## 3. Risk Assessment (ISO 26262)
### Severity (S)
* **Score:** **S2** (Moderate injuries)
* **Rationale:** Drift could lead to minor collision or off-path, but small vehicle limits harm.

### Exposure (E)
* **Score:** **E3** (Medium probability, 10-50%)
* **Rationale:** Lane mode used frequently in tests, but not always.

### Controllability (C)
* **Score:** **C2** (Normally controllable)
* **Rationale:** Operator can override, but automation reduces immediate control.

### Calculation
**S2 + E3 + C2 = ASIL B**
*(Based on ISO 26262 Part 3 Table 4)*

## 4. Safety Goals (SG)
Based on H-05, we derive the following Safety Goal:

| SG ID | ASIL | Description |
|:---|:---|:---|
| **SG-05** | **B** | **Accurate Lane Detection:** System must detect lanes with >80% confidence or fallback to manual/sensors. |
