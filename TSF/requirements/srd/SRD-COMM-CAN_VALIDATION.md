---
id: SRD-COMM-CAN_VALIDATION
header: "CAN ID Mapping Validation"
text: |
  "Validates CAN ID mappings."
verification_method: "Integration Testing"
parents:
  - id: URD-COMM_INTEGRITY
children:
  - id: SWD-CAN_VALIDATION
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Addresses CAN ID mapping hazards"
active: true
derived: false
normative: true
level: 2.0
---
The CAN interface shall filter incoming frames by identifier, forwarding only recognized command IDs (e.g. speed, steering) to the appropriate software queues and ignoring any others.
---
