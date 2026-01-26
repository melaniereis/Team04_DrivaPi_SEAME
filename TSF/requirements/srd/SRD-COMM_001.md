---
id: SRD-COMM_001
header: "Bidirectional Control Protocol"
text: |
  "Maintains reliable link for control integrity."
verification_method: "Integration Testing"
parents:
  - id: URD-COMM_001
children:
  - id: SWD-COMM_PROTOCOL
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval
references:
  - type: "file"
    path: docs/standards/iso26262/hara_comms.md
    description: "Supports SG-08 (Reliable Communications)"
active: true
derived: false
normative: true
level: 2.0
---
System shall exchange commands/status between STM32 firmware and RPi5 HMI with timeout detection.
