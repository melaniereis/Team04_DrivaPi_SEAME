---
id: URD-COMM_001
header: "Firmware ↔ HMI Communication"
text: |
  "Ensures reliable data exchange to prevent control loss."
verification_method: "Integration Testing"
children:
  - id: SRD-CAN_PROTOCOL
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
  - type: "file"
    path: docs/standards/iso26262/hara_comms.md
    description: "Mitigates Hazard H-08 (Comms Loss)"
active: true
derived: false
normative: true
level: 1.0
---
Reliable bidirectional communication between firmware (STM32) and Qt HMI (RPi5).
---
