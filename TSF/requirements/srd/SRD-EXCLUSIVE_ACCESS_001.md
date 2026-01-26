---
id: SRD-EXCLUSIVE_ACCESS_001
header: "Exclusive Access to Shared Data"
text: |
  "Ensures deterministic access to shared data."
verification_method: "Integration Testing"
parents:
  - id: URD-RTOS_CONCURRENCY_001
children:
  - id: SWD-MUTEX_IMPLEMENTATION
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Addresses race condition hazards"
active: true
derived: false
normative: true
level: 2.0
---
All shared data structures (e.g., speed buffers) shall be protected by deterministic synchronization primitives (e.g., mutexes or message queues). The RTOS configuration shall prevent unbounded priority inversion for safety-critical tasks.
---
