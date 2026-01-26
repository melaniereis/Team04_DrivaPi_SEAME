---
id: SWD-QUEUE_MONITOR
header: "Queue Overrun Monitor"
text: |
  "Detects and handles message queue overflows."

ASIL: "QM"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-QUEUE_OVERRUN_001
children: []
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Implements queue overrun detection logic"
active: true
derived: false
normative: true
level: 3.0
---
The software shall detect when message queues or buffers overflow. It shall log the event and, if the overflow persists beyond a threshold, enter a safe or degraded state.
---
