---
id: SWD-CAN_VALIDATION
header: "CAN ID Mapping Validation Logic"
text: |
  "Validates expected CAN IDs and discards unknown frames."
ASIL: "QM"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-COMM-CAN_VALIDATION
children:
  - id: LLTC-CAN_VALID_COV
  - id: LLTC-CAN_VALID_UNIT
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''
active: true
derived: false
normative: true
level: 3.0
---
On startup, the software shall verify that the configured CAN IDs match expected definitions.
---
