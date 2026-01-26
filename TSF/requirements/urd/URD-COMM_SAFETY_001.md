---
id: URD-COMM_SAFETY_001
header: "Communication Integrity & Degraded Mode"
text: |
  "Maintains communication integrity and handles degraded operation."
verification_method: "System Testing"
children:
  - id: SRD-CAN_ID_VALIDATION_001
  - id: SRD-COMM_TIMEOUTS_001
  - id: SRD-DEGRADED_MODE_001
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Addresses communication loss, stale data and bus congestion hazards"
active: true
derived: false
normative: true
level: 1.0
---
The system shall validate CAN ID mappings at initialization and discard messages with unexpected IDs. It shall detect communication timeouts, stale data, bus off and bus congestion, entering a degraded mode (e.g., reduced speed) or stopping the vehicle within 100 ms as appropriate.
---
