---
id: URD-SERVO_MOTOR
header: "Steering Control"
text: |
  "The system shall provide steering control to turn the vehicle left and right based on operator commands."

# TSF Type: Expectation
verification_method: "System Testing"

children:
  - id: SRD-SERVO_MOTOR

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"

reviewed: '2026-01-14 - Approved by Melanie Reis'

references:
  - type: "file"
    path: "docs/standards/iso26262/hara-actuation.md"
    description: "Mitigates Hazard H-02 (Unintended Steering)"

active: true
derived: false
normative: true
level: 1.0
---
The user expects the steering wheel or command input to translate into physical wheel angle changes.