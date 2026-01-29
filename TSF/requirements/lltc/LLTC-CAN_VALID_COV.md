---
id: LLTC-CAN_VALID_COV
header: "Code Coverage for CAN Validation"
text: |
  "Ensures CAN ID allowlist and rejection logic is fully exercised."
verification_method: "Code Coverage (LCOV)"

parents:
  - id: SWD-CAN_VALIDATION

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

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
Unit tests must achieve >90% Line Coverage for the implementation of SWD-CAN_VALIDATION.
---
