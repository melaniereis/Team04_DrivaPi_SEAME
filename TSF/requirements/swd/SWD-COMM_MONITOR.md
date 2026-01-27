---
id: SWD-COMM_MONITOR
header: "Communication Monitoring Logic"
text: |
  "Monitors communication health for timeouts and congestion."

ASIL: "A"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-COMM-TIMEOUT_MGMT
children: []
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Implements communication timeout and congestion monitoring"
active: true
derived: false
normative: true
level: 3.0
---
The software shall monitor CAN and internal bus communication for timeouts, stale data and congestion. When thresholds are exceeded, it shall trigger safe or degraded operation and log the event.
---
