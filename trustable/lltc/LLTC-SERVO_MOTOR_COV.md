---
id: LLTC-SERVO_MOTOR_COV
header: "Code Coverage for Servo"
text: |
  "Unit tests must achieve >90% Branch Coverage for `servo_motor.c`."

verification_method: "Code Coverage (LCOV)"

parents:
  - id: SWD-SERVO_MOTOR

reviewers:
  - name: "Miguel Cardoso"
    email: "miguel.cardoso@seame.pt"
reviewed: '2026-01-14 - Approved by Miguel Cardoso'

evidence:
  type: coverage_threshold_validator
  configuration:
    min_line_rate: 90
    references:
    - type: file
      path: artifacts/verification/coverage/coverage.xml

active: true
derived: false
normative: true
level: 4.0
---
Ensures steering safety limits are exercised during testing.
