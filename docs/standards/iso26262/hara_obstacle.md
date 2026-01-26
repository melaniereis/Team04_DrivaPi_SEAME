# HARA: Obstacle Detection System

## 1. Item Definition & Assumptions
- **Item:** Ultrasonic Sensor and Vision Backup for Obstacle Detection.
- **Function:** Measures distance to obstacles and recognizes them via camera for collision avoidance.
- **Operational Context:** Autonomous mode in lab/track; low speeds (<4 m/s).
- **Assumption:** Vehicle is small-scale; operators nearby for intervention.

## 2. Hazard Analysis
We identified one primary hazard related to obstacle detection.

| ID | Hazard Description | Operational Situation | Cause of Hazard |
|:---|:---|:---|:---|
| **H-04** | **Missed Obstacle Detection** (System fails to detect/respond to obstacle) | Vehicle approaching obstacle in autonomous mode. | Sensor noise, failure, or low confidence in vision backup causing no trigger. |

## 3. Risk Assessment (ISO 26262)
### Severity (S)
* **Score:** **S3** (Severe injuries possible)
* **Rationale:** Collision could cause vehicle flip or impact, risking operator injury in lab.

### Exposure (E)
* **Score:** **E3** (Medium probability, 10-50%)
* **Rationale:** Obstacles common in test scenarios, but not constant.

### Controllability (C)
* **Score:** **C2** (Normally controllable)
* **Rationale:** Operator can see and stop manually, but autonomous mode reduces reaction time.

### Calculation
**S3 + E3 + C2 = ASIL C**
*(Based on ISO 26262 Part 3 Table 4)*

## 4. Safety Goals (SG)
Based on H-04, we derive the following Safety Goal:

| SG ID | ASIL | Description |
|:---|:---|:---|
| **SG-04** | **C** | **Reliable Obstacle Detection:** System must detect obstacles <50cm with filtering and trigger avoidance within 150ms, or flag error. |
