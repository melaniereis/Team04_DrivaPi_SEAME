---
id: SWD-SERVO_MOTOR
header: "Servo Logic Implementation"
text: |
  "The 'servo_motor' component shall implement angle validation (0-180) and mapping to hardware-specific PWM counts."

# TSF Type: Assertion
ASIL: "A"
verification_method: "Unit Testing, Code Coverage"

parents:
  - id: SRD-SERVO_MOTOR
children:
  - id: LLTC-SERVO_MOTOR_UNIT
  - id: LLTC-SERVO_MOTOR_COV
  - id: LLTC-SERVO_MOTOR_STATIC

reviewers:
  - name: "Miguel Cardoso"
    email: "miguel.cardoso@seame.pt"

reviewed: '2026-01-14 - Approved by Miguel Cardoso <miguel.cardoso@seame.pt>'

references:
  - type: "file"
    path: "Threadx/Core/Src/servo_motor.c"
    description: "Servo Implementation (production source)"
  - type: "file"
    path: "docs/standards/iso26262/asil_justification_SWD-SERVO_MOTOR.md"
    description: "ASIL Justification for SWD-SERVO_MOTOR"

active: true
derived: false
normative: true
level: 3.0
---
## Implementation Requirements
1. **Angle Clamping:** Inputs > 180 or < 0 must be clamped.
2. **Pulse Calculation:** Map angle linearly between `SERVO_MIN_PULSE` and `SERVO_MAX_PULSE`.
