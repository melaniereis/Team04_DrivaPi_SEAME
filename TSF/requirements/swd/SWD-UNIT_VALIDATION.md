---
id: SWD-UNIT_VALIDATION
header: "SI Unit Validation Logic"
text: |
  "Implements validation of SI unit adherence."

ASIL: "QM"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-UNIT_CONFORMANCE_001
children: []
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Implements SI unit checks"
active: true
derived: false
normative: true
level: 3.0
---
The software shall implement logic to validate that speed is measured in meters per second (m/s) and steering angles in degrees. Invalid units shall either be converted to SI or rejected and logged.
---
