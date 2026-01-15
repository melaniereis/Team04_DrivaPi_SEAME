---
id: URD-SPEED_SENSOR
header: "Vehicle Speed Monitoring"
text: |
  "The system shall display current linear speed to enable operator monitoring during autonomous operation."

# TSF Type: Expectation
verification_method: "System Testing"

children:
  - id: SRD-SPEED_SENSOR

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"

reviewed: '2026-01-09 - Approved by Melanie Reis <melanie.reis@seame.pt>'

references:
  - type: "file"
    path: "docs/standards/iso26262/hara-motor-speed.md"
    description: "HARA Hazard H-01"

active: true
derived: false
normative: true
level: 1.0
---
Operators need to verify vehicle state (stopped vs. moving) to ensure testing safety.
