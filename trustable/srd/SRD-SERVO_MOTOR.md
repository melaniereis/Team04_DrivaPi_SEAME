---
id: SRD-SERVO_MOTOR
header: "Steering Servo Interface"
text: |
  "The system shall control the Steering Servo via the PCA9685 driver, mapping angle inputs [0, 180] to calibrated PWM pulse widths (typically 200-500 counts)."

# TSF Type: Assertion
verification_method: "Integration Testing"

parents:
  - id: URD-SERVO_MOTOR
children:
  - id: SWD-SERVO_MOTOR

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"

reviewed: '2026-01-14 - Approved by Melanie Reis'

references:
  - type: "file"
    path: "tests/unit/servo_motor/src/servo_motor.h"
    description: "Servo Interface Definition"

active: true
derived: false
normative: true
level: 2.0
---
Exceeding mechanical limits of the servo (PWM counts <200 or >500) can cause physical damage.