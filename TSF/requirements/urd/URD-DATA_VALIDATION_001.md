---
id: URD-DATA_VALIDATION_001
header: "Data Unit & Plausibility Validation"
text: |
  "Ensures that data units and values are valid and plausible."
verification_method: "System Testing"
children:
  - id: SRD-UNIT_CONFORMANCE_001
  - id: SRD-CROSS_PLAUSIBILITY_001
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Addresses unit mismatch and plausibility hazards"
active: true
derived: false
normative: true
level: 1.0
---
All speed, steering and sensor signals shall conform to the SI units defined in the COVESA Vehicle Signal Specification. Values shall be validated against defined minimum and maximum ranges, and the system shall perform cross-plausibility checks between the primary speed sensor and a secondary source (e.g., IMU-derived velocity or vision) to detect inconsistencies.
---
