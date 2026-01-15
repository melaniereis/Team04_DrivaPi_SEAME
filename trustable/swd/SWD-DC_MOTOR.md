---
id: SWD-DC_MOTOR
header: "DC Motor Logic Implementation"
text: |
  "The 'dc_motor' component shall implement input validation, dead-zone handling, and safe PWM calculation for the propulsion motor."

# TSF Type: Assertion
ASIL: "A"
verification_method: "Unit Testing, Code Coverage"

parents:
  - id: SRD-DC_MOTOR

children:
  - id: LLTC-DC_MOTOR_UNIT
  - id: LLTC-DC_MOTOR_COV
  - id: LLTC-DC_MOTOR_STATIC

reviewers:
  - name: "Miguel Cardoso"
    email: "miguel.cardoso@seame.pt"

reviewed: '2026-01-14 - Approved by Miguel Cardoso <miguel.cardoso@seame.pt>'

references:
  - type: "file"
    path: "Threadx/Core/Src/dc_motor.c"
    description: "Motor Implementation (production source)"
  - type: "file"
    path: "docs/standards/iso26262/asil-justification-SWD-DC_MOTOR.md"
    description: "ASIL Justification for SWD-DC_MOTOR"

active: true
derived: false
normative: true
level: 3.0
---
## Implementation Requirements
1. **Input Clamping:** Speed inputs > 1.0 or < -1.0 must be clamped.
2. **Direction Control:** Positive speed maps to Forward GPIOs, Negative to Backward GPIOs.
3. **PWM Calculation:** `speed * 4095` mapping.
