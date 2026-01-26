---
id: URD-STEER_001
header: "Steering Control"
text: |
  "The user expects the steering command input to translate into physical wheel angle changes."
verification_method: "System Testing"
children:
  - id: SRD-SERVO_ANGLE
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
  - type: "file"
    path: docs/standards/iso26262/hara_actuation.md
    description: "Mitigates Hazard H-03 (Unintended Steering)"
active: true
derived: false
normative: true
level: 1.0
---
The vehicle shall steer left and right according to commanded angle.
---
