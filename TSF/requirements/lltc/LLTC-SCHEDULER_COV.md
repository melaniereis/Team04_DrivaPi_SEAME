---
id: LLTC-SCHEDULER_COV
header: "Code Coverage for Scheduler Monitor"
text: |
  "Ensures the scheduler monitoring and deadline tracking logic is fully covered."
verification_method: "Code Coverage (LCOV)"

parents:
  - id: SWD-RTOS_SCHEDULER

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
Unit tests must achieve >90% Line Coverage for the implementation of SWD-RTOS_SCHEDULER.
---
