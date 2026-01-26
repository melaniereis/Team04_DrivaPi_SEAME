---
id: SRD-CAN_ID_VALIDATION_001
header: "CAN ID Mapping Validation"
text: |
  "Validates CAN ID mappings."
verification_method: "Integration Testing"
parents:
  - id: URD-COMM_SAFETY_001
children:
  - id: SWD-CAN_VALIDATION
# reviewers:
#   - name: "Melanie Reis"
#     email: "melanie.reis@seame.pt"
# reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
  - type: "report"
    path: comparison_report.md
    description: "Addresses CAN ID mapping hazards"
active: true
derived: false
normative: true
level: 2.0
---
At system initialization, the software shall validate that all expected CAN IDs match their defined control variables. Frames received with unexpected IDs shall be silently discarded.
---
