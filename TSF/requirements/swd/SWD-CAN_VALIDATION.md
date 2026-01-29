---
id: SWD-CAN_VALIDATION
header: "CAN ID Mapping Validation Logic"
text: |
  "Validates expected CAN IDs and discards unknown frames."

ASIL: "QM"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-COMM-CAN_VALIDATION
children: []
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Implements CAN ID validation logic"
active: true
derived: false
normative: true
level: 3.0
---
On startup, the software shall verify that the configured CAN IDs match expected definitions. At runtime, it shall ignore or discard frames with unknown IDs.
---
