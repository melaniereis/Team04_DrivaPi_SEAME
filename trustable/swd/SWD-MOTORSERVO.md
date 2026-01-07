---
id: SWD-MOTORSERVO
header: "Actuator Driver Logic Implementation"
text: |
  "The 'dc_motor' and 'servo_motor' software components shall implement input validation, range clamping, and safe PWM calculation to prevent invalid hardware states."

# TSF Type: Assertion
ASIL: "A"
verification_method: "Unit Testing, Code Coverage, Static Analysis"

parents:
  - id: SRD-MOTORSERVO

children:
  - id: LLTC-MOTORSERVOUnitTests
  - id: LLTC-MOTORSERVOCoverage
  - id: LLTC-MOTORSERVOStaticAnalysis

reviewers:
  - name: "Miguel Cardoso"
    email: "miguel.cardoso@seame.pt"

reviewed: '2026-01-07 - Approved by Miguel Cardoso'

references:
#  - type: "file"
#    path: "Threadx/Core/Src/servo_motor.c"
#    description: "Servo Implementation"
#  - type: "file"
#    path: "Threadx/Core/Src/dc_motor.c"
#    description: "Motor Implementation"
  - type: "file"
    path: "docs/standards/iso26262/asil-justification-swd-SERVOMOTOR.md"
    description: "ASIL A Justification"

active: true
derived: false
normative: true
level: 3.0
---
## Detailed Implementation Requirements
1.  **Input Clamping:** Inputs (Speed -1.0 to 1.0; Angle 0 to 180) must be clamped to safe limits before calculation.
2.  **Overflow Protection:** The `clamp_u16` function must ensure calculated PWM counts never exceed `PCA9685_COUNTS` (4096) or wrap around to zero.
3.  **Thread Safety:** Access to actuator hardware must be thread-safe or handled by specific tasks (`dc_motor`, `servo_motor`) consuming queues.