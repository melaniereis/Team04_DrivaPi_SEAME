---
id: SRD-UNIT_CONFORMANCE_001
header: "SI Unit Conformance"
text: |
  "Ensures signals conform to SI units defined in VSS."
verification_method: "Integration Testing"
parents:
  - id: URD-DATA_VALIDATION_001
children:
  - id: SWD-UNIT_VALIDATION
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Addresses unit mismatch hazards"
active: true
derived: false
normative: true
level: 2.0
---
The software shall validate that speed and steering values adhere to SI units (m/s for speed, degrees for steering) as defined by VSS. It shall convert or reject any input that does not conform to these units.
---
