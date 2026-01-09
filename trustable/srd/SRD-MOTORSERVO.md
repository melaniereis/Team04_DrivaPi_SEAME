---
id: SRD-MOTORSERVO
header: "PWM Actuator Interface"
text: |
  "The system shall control the DC Motor and Steering Servo via the PCA9685 driver, mapping logical inputs to calibrated 12-bit PWM pulse widths."

# TSF Type: Assertion
verification_method: "Integration Testing"

parents:
  - id: URD-MOTORSERVO
children:
  - id: SWD-MOTORSERVO

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"

reviewed: '2026-01-07 - Approved by Melanie Reis'

references:
  - type: "file"
    path: "Threadx/Core/Inc/servo_motor.h"
    description: "Hardware Constants (Channels/Limits)"

active: true
derived: false
normative: true
level: 2.0
---
Hardware constraints dictate that exceeding specific PWM counts (e.g., <200 or >500 for Servo) can physically damage the actuators. The system layer must enforce these boundaries.
