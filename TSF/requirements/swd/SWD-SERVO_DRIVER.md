---
id: SWD-SERVO_DRIVER
header: "Servo PWM Driver"
text: |
  "Implements safe steering logic with angle clamping."
ASIL: "A"
verification_method: "Unit Testing, Code Coverage"
parents:
  - id: SRD-STEER_001
children:
  - id: LLTC-SERVO_UNIT
  - id: LLTC-SERVO_COV
  - id: LLTC-SERVO_STATIC
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval
references:
  - type: "file"
    path: firmware/Core/Src/servo_motor.c
    description: "Production Implementation"
  - type: "file"
    path: docs/standards/iso26262/asil_justification_SWD-SERVO_MOTOR.md
    description: "ASIL A Justification"
active: true
derived: false
normative: true
level: 3.0
---
The 'servo_motor' component shall implement angle validation (clamp 0-180) and linear mapping to hardware-specific PWM counts via PCA9685.
