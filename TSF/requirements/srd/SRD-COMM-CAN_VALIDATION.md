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
  - id: HLTC-CAN_UNKNOWN_ID
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''
references:
  - type: "file"
    path: docs/standards/iso26262/hara_comms.md
    description: "Mitigates Hazard H-08 (Invalid Data)"
active: true
derived: false
normative: true
level: 2.0
---
The CAN interface shall filter incoming frames by identifier, forwarding only recognized command IDs (e.g. speed, steering) to the appropriate software queues and ignoring any others.
---
