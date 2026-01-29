---
id: URD-COMM_INTEGRITY
header: "Communication Integrity & Degraded Mode"
text: |
  "Maintains communication integrity and handles degraded operation."
verification_method: "System Testing"
children:
  - id: SRD-COMM-CAN_VALIDATION
  - id: SRD-COMM-TIMEOUT_MGMT
  - id: SRD-SAFE-DEGRADED_OPS
  - id: HLTC-VAL-COMM_INTEGRITY
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
  - type: "file"
    path: docs/standards/iso26262/hara_comms.md
    description: "Mitigates Hazard H-08 (Communication Loss/Failure)"
active: true
derived: false
normative: true
level: 1.0
---
The firmware shall accept only predefined CAN command messages and ignore or discard any unexpected or invalid CAN frames.
---
