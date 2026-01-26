---
id: SRD-COMM_TIMEOUTS_001
header: "Communication Timeout & Congestion Handling"
text: |
  "Detects and handles communication timeouts and congestion."
verification_method: "Integration Testing"
parents:
  - id: URD-COMM_SAFETY_001
children:
  - id: SWD-COMM_MONITOR
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Addresses comms loss, stale data and bus congestion"
active: true
derived: false
normative: true
level: 2.0
---
The system shall detect communication timeouts, stale data and bus congestion on the CAN or internal bus. If a timeout or stale data persists for more than a defined period, it shall enter safe or degraded mode, stopping the vehicle within 100 ms if necessary. Bus congestion shall trigger message rejection and degraded operation.
---
