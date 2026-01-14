---
id: LLTC-SPEEDSENSORUNITTESTS
header: "Unit Test Verification for Speed Logic"
text: |
  "Speed sensor unit tests must pass (100%) covering movement, overflows, and edge cases."

# TSF Type: Evidence
verification_method: "Automated Unit Testing (CTest/Unity)"

parents:
  - id: SWD-SPEED

reviewers:
  - name: "Bernardo Esteves"
    email: "bernardo.esteves@seame.pt"

reviewed: '2026-01-09 - Approved by Bernardo Esteves <bernardo.esteves@seame.pt>'

references:
  - type: "file"
    path: "tests/unit/speed_sensor/test/test_speed_sensor.c"
    description: "Test Source Code"

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
Verifies logic correctness via CI-generated `junit_results.xml`.
