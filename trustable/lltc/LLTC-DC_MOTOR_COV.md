---
id: LLTC-DC_MOTOR_COV
header: "Code Coverage for DC Motor"
text: |
  "Unit tests must achieve >90% Branch Coverage for `dc_motor.c`."

verification_method: "Code Coverage (LCOV)"

parents:
  - id: SWD-DC_MOTOR

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
Ensures critical paths in propulsion logic are tested.