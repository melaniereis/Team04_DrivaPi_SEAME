---
id: LLTC-RTOS_UNIT
header: "Unit Tests for RTOS Configuration"
text: |
  "Verifies RTOS configuration helpers and timing-related utilities used by safety-critical tasks."

# TSF Type: Evidence/Premise (A Claim, but not a Request)
verification_method: "Automated Unit Testing"

parents:
  - id: SWD-RTOS_CONFIG

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)

# references:
#   - type: "file"
#     path: "tests/unit/rtos/test/test_rtos_config.c"
#   - type: "file"
#     path: "tests/unit/rtos/test/test_rtos_config_with_mocks.c"

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
All unit tests for RTOS configuration and scheduling utilities shall pass, validating timers, tick conversion, and configuration boundaries.
---
