# HARA Overview: DrivaPi Autonomous Vehicle System

## 1. Item Definition & Scope
- **Item:** Full DrivaPi System (PiRacer with Raspberry Pi 5, STM32 firmware, ThreadX RTOS, Qt HMI, sensors/vision).
- **Functions:** Propulsion, steering, speed monitoring, obstacle detection (vision primary/ultrasonic fallback), autonomous control, HMI, communications.
- **Operational Context:** Educational/lab environment; controlled test track; max speed 4 m/s; trained operators always present.
- **Global Assumptions:** Small-scale vehicle (low mass/torque); no public road use; E-stop always available; hazards limited to minor injuries.

## 2. Consolidated Hazard Summary
All identified hazards (H-01 to H-10) with ASIL and linked URDs.

| Hazard ID | Description | Linked URD(s) | Severity (S) | Exposure (E) | Controllability (C) | ASIL | Safety Goal (SG) |
|:----------|:------------|:--------------|:-------------|:-------------|:--------------------|:-----|:-----------------|
| H-01      | False Zero Speed Indication | URD-SPEED-001, URD-SAFETY-001 | S1 | E2 | C1 | A | SG-01 |
| H-02      | Unintended Acceleration | URD-PROP-001, URD-CONTROL-001, URD-SAFETY-001 | S1 | E3 | C2 | A | SG-02 |
| H-03      | Unintended Steering | URD-STEER-001 | S1 | E3 | C2 | A | SG-03 |
| H-04      | Missed Obstacle Detection (Vision primary) | URD-VISION-002, URD-DIST-001 | S1 | E3 | C1 | A | SG-04 |
| H-05      | Lane Misdetection | URD-VISION-001 | S1 | E2 | C1 | QM | SG-05 |
| H-06      | Scheduling Overrun/Delay | URD-CONTROL-002 | S2 | E2 | C2 | A | SG-06 |
| H-07      | Stale or Inaccurate HMI Display | URD-HMI-001, URD-HMI-002 | S1 | E3 | C1 | QM | SG-07 |
| H-08      | Communication Loss | URD-COMM-001 | S2 | E3 | C2 | A | SG-08 |
| H-09      | Calibration/Startup Failure | URD-SYSTEM-001 | S1 | E3 | C1 | QM | SG-09 |
| H-10      | Control Instability | URD-PERF-001 | S1 | E3 | C1 | QM | SG-10 |

## 3. Overall Risk Assessment Rationale
- **Severity (S)**: Max S2 (moderate) – small vehicle/low speed limits harm to bruises/cuts (no S3 life-threat).
- **Exposure (E)**: E2-E3 – lab tests intermittent/supervised (not E4 continuous like production vehicles).
- **Controllability (C)**: C1-C2 – trained operators with direct view/E-stop (high mitigation).
- **ASIL Distribution**: 5 A, 5 QM – rigorous tailoring for educational project (per ISO 26262 Part 9).

## 4. References
- Detailed per-hazard HARAs: hara_actuation.md, hara_motor_speed.md, hara_obstacle.md, etc.
- Traceability: All SGs feed into URDs for TSF DAG.
