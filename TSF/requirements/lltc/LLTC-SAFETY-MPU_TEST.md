---
id: LLTC
header: "MPU Configuration Test"
text: |
  "Tests memory protection and fault containment."

# TSF Type: Evidence/Premise (A Claim, but not a Request)
verification_method: "Unit Test Execution"

parents:
  - id: SWD-SAFETY_MPU_CONFIG

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)

# references:
#   - type: "file"
#     path: "tests/unit/safety/test/test_safety_mpu_config.c"
#   - type: "file"
#     path: "tests/unit/safety/test/test_safety_mpu_config_with_mocks.c"
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
All unit tests for the MPU configuration and fault containment shall pass, validating memory region setup, access permissions, and fault response behavior.
---
