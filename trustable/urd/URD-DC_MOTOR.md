---
id: URD-DC_MOTOR
header: "Propulsion Control"
text: |
  "The system shall provide propulsion control to move the vehicle forward and backward at requested speeds."

# TSF Type: Expectation
verification_method: "System Testing"

children:
  - id: SRD-DC_MOTOR

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"

reviewed: '2026-01-14 - Approved by Melanie Reis'

references:
  - type: "file"
    path: "docs/standards/iso26262/hara-actuation.md"
    description: "Mitigates Hazard H-02 (Unintended Acceleration)"

active: true
derived: false
normative: true
level: 1.0
---
The user expects the "Gas" and "Brake/Reverse" commands to translate into physical motion.
