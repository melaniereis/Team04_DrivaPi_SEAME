---
id: URD-STEERING_CONTROL
header: "Steering Control"
text: |
  "The user expects the steering command input to translate into physical wheel angle changes."
verification_method: "System Testing"
children:
  - id: SRD-ACT-SERVO_STEER
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
The vehicle shall respond to commanded steering angles between 0° and 180°, clamping any input outside this range to protect the mechanical limits of the steering servo.
---
