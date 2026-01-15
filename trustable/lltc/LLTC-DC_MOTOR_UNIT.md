---
id: LLTC-DC_MOTOR_UNIT
header: "Unit Tests for DC Motor"
text: |
  "All unit tests for the DC Motor module shall pass, verifying forward/reverse logic and PWM calculations."

# TSF Type: Evidence/Premise (A Claim, but not a Request)
verification_method: "Automated Unit Testing"

parents:
  - id: SWD-DC_MOTOR

reviewers:
  - name: "Miguel Cardoso"
    email: "miguel.cardoso@seame.pt"
reviewed: '2026-01-14 - Approved by Miguel Cardoso <miguel.cardoso@seame.pt>'

references:
  - type: "file"
    path: "tests/unit/dc_motor/test/test_dc_motor_functions.c"
  - type: "file"
    path: "tests/unit/dc_motor/test/test_threadx_dc_motor_with_mocks.c"
  - type: "file"
    path: "tests/unit/dc_motor/test/test_dc_motor_LLTC_UNIT.c"

evidence:
  type: junit_pass_rate_validator
  configuration:
    min_pass_rate: 100
    references:
    - type: file
      path: artifacts/verification/tests/junit_results.xml

active: true
derived: false
normative: true
level: 4.0
---
Verifies the logic within `dc_motor.c`.
