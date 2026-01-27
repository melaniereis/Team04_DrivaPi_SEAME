---
id: SRD-RTOS-QUEUE_MON
header: "Queue Overrun Detection"
text: |
  "Detects and reports queue overrun events."
verification_method: "Integration Testing"
parents:
  - id: URD-DETERMINISTIC_SCHED
children:
  - id: SWD-QUEUE_MONITOR
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Addresses queue overrun hazards"
active: true
derived: false
normative: true
level: 2.0
---
The system shall detect when message queues or buffers overflow and log the event. If the overflow persists beyond a threshold, the system shall enter a fail-safe or degraded mode.
---
