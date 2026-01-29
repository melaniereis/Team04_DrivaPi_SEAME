---
id: LLTC-CAN_VALID_UNIT
header: "Unit Tests for CAN Validation"
text: |
  "Verifies ID allowlist logic in isolation."
verification_method: "Automated Unit Testing"

parents:
  - id: SWD-CAN_VALIDATION

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

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
Unit tests shall verify that the validation function accepts known IDs and returns rejection status for unknown IDs, or ignore them.
