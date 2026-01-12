---
id: LLTC-MOTORSERVOCoverage
header: "Actuator Code Coverage"
text: |
  "Combined unit tests must achieve >90% Branch Coverage for `dc_motor.c` and `servo_motor.c` to satisfy ASIL A requirements."

# TSF Type: Evidence/Premise (A Claim, but not a Request)

verification_method: "Code Coverage (LCOV)"

parents:
  - id: SWD-MOTORSERVO

reviewers:
  - name: "Miguel Cardoso"
    email: "miguel.cardoso@seame.pt"
reviewed: '2026-01-07 - Approved by Miguel Cardoso <miguel.cardoso@seame.pt>'

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
This evidence ensures that the test suite actually exercises the critical paths of the actuation logic (including error handling branches).
