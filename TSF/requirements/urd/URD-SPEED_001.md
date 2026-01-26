---
id: URD-SPEED_001
header: "Vehicle Speed Measurement"
text: |
  "Operators need accurate speed for monitoring and control during autonomous operation."
verification_method: "System Testing"
children:
  - id: SRD-SPEED_CALC
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
  - type: "file"
    path: docs/standards/iso26262/hara_motor_speed.md
    description: "Mitigates Hazard H-01 (False Zero Speed)"
active: true
derived: false
normative: true
level: 1.0
---
The system shall accurately measure and report current vehicle speed in m/s.
---
