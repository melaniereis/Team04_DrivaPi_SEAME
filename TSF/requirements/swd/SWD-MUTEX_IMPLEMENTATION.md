---
id: SWD-MUTEX_IMPLEMENTATION
header: "Safe Concurrency Implementation"
text: |
  "Ensures exclusive access to shared data using RTOS primitives."

ASIL: "A"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-EXCLUSIVE_ACCESS_001
children: []
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Implements concurrency safety logic"
active: true
derived: false
normative: true
level: 3.0
---
The software shall use deterministic synchronization primitives, such as mutexes or message queues, to protect shared speed buffers and other shared data structures. It shall avoid unbounded priority inversion using priority inheritance or similar mechanisms.
---
