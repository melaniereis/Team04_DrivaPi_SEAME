---
id: URD-CONTROL_001
header: "Emergency Stop"
text: |
  "Ensures immediate safe halt upon detection of hazards or faults."
verification_method: "System Testing"
children:
  - id: SRD-EMERGENCY_STOP
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
  - type: "file"
    path: docs/standards/iso26262/hara_actuation.md
    description: "Mitigates Hazard H-02 (Unintended Acceleration)"
active: true
derived: false
normative: true
level: 1.0
---
The system shall execute a safe emergency stop when obstacle or critical fault is detected.
---
