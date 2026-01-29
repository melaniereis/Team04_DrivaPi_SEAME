---
id: URD-DETERMINISTIC_SCHED
header: "Deterministic Scheduling & Concurrency"
text: |
  "Guarantees deterministic scheduling and safe concurrency."
verification_method: "System Testing"
children:
  - id: SRD-RTOS-PERIODIC_TASK
  - id: SRD-RTOS-MUTEX_DATA
  - id: SRD-RTOS-QUEUE_MON
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
Safety-critical tasks shall be scheduled periodically within their deadlines. Access to shared data shall use deterministic synchronization (mutexes/queues) to prevent race conditions. The system shall detect and report queue overruns or lost messages and enter a safe or degraded mode if they occur.
---
