---
id: LLTC-SpeedCoverage
header: "Code Coverage for Speed Sensor"
text: |
  "The unit tests must achieve at least 90% line coverage for 'speed_sensor.c'
  to ensure sufficient testing of safety-critical branches."

# TSF Type: Evidence
verification_method: "Code Coverage Analysis (LCOV/GCOVR)"

# Links: Connects to parent Assertion
parents:
  - id: SWD-SpeedSensor

reviewers:
  - name: "Bernardo Esteves"
    email: "bernardo.esteves@seame.pt"

reviewed: '2026-01-05 - Approved by Bernardo Esteves <bernardo.esteves@seame.pt>'

references:
  - type: "file"
    path: "artifacts/verification/coverage/coverage.xml"
    description: "Cobertura XML Report"

# AUTOMATED VALIDATION
evidence:
  type: coverage_threshold_validator
  configuration:
    min_line_rate: 90
    references:
    - type: file
      # This path must match the output defined in your CI workflow
      path: artifacts/verification/coverage/coverage.xml

active: true
derived: false
normative: true
level: 4.0
---
This evidence relies on the `coverage.xml` converted from LCOV data in the CI pipeline. It ensures that the unit tests exercised the vast majority of the source code.
