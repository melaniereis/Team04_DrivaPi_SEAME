---
id: URD-RTOS_CONCURRENCY_001
header: "Deterministic Scheduling & Concurrency"
text: |
  "Guarantees deterministic scheduling and safe concurrency."
verification_method: "System Testing"
children:
  - id: SRD-PERIODIC_TASKS_001
  - id: SRD-EXCLUSIVE_ACCESS_001
  - id: SRD-QUEUE_OVERRUN_001
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Addresses scheduling, race condition and queue overrun hazards"
active: true
derived: false
normative: true
level: 1.0
---
Safety-critical tasks shall be scheduled periodically within their deadlines. Access to shared speed or control data shall use deterministic synchronization primitives to prevent race conditions. The system shall detect and report queue overruns or lost messages and enter a safe or degraded mode.
---
