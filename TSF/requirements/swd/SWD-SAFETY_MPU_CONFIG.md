---
id: SWD-SAFETY-MPU_CONFIG
header: "MPU Configuration Implementation"
text: |
  "Implements memory protection configuration and violation handler."

ASIL: "A"
verification_method: "Unit Testing & Static Analysis"
parents:
  - id: SRD-SAFETY-MEMORY_ISOLATION
children:
  - id: SWD-SAFETY-MPU_TEST
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
active: true
derived: FALSE
normative: true
level: 3.0
---
The implementation shall configure MPU registers with distinct regions for critical and non-critical tasks, set appropriate read/write/execute permissions, and implement an exception handler that logs memory faults and initiates a safe halt.
---
