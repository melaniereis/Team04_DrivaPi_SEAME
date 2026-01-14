---
id: SRD-DC-MOTOR
header: "DC Motor Driver Interface"
text: |
  "The system shall control the DC Drive Motor via the PCA9685 driver, mapping speed inputs [-1.0, 1.0] to calibrated PWM pulse widths."

# TSF Type: Assertion
verification_method: "Integration Testing"

parents:
  - id: URD-DC-MOTOR
children:
  - id: SWD-DC-MOTOR

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"

reviewed: '2026-01-14 - Approved by Melanie Reis'

references:
  - type: "file"
    path: "tests/unit/dc_motor/src/dc_motor.h"
    description: "Motor Interface Definition"

active: true
derived: false
normative: true
level: 2.0
---
Hardware constraints dictate that specific PWM channels (e.g., L_PWM, R_PWM) are used for the drive motor.