---
id: LLTC-MOTORSERVOUnitTests
header: "Unit Verification for Actuators"
text: |
  "Unit tests shall verify that the actuator logic correctly clamps invalid inputs and produces valid PWM counts."

verification_method: "Automated Unit Testing"
parents:
  - id: SWD-SERVOMOTOR
reviewed: '2026-01-07 - Approved by QA'

references:
  - type: "file"
    path: "tests/unit/motor_servo/test/test_servo_functions.c"
  - type: "file"
    path: "tests/unit/motor_servo/test/test_dc_motor_functions.c"

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
Validates logic in `servo_functions.c` and `dc_motor_functions.c`.