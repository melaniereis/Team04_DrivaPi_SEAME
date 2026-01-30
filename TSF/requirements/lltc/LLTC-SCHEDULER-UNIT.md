---
id: LLTC-SCHEDULER_UNIT
header: "Unit Tests for Scheduler Configuration"
text: |
  "Verifies task periodicity and configuration settings."
verification_method: "Automated Unit Testing"

parents:
  - id: SWD-RTOS_SCHEDULER

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# evidence:
#   type: junit_pass_rate_validator
#   configuration:
#     min_pass_rate: 100
#     references:
#     - type: file
#       path: artifacts/verification/tests/scheduler.xml

active: true
derived: false
normative: true
level: 4.0
---
Unit tests shall verify that the scheduler is initialized with the correct task periods and priorities.
