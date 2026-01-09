---
id: URD-SPEEDSENSOR
header: "Vehicle Speed Monitoring"
text: |
  "The DrivaPi system shall calculate and display the current vehicle linear speed to enable the operator to monitor the vehicle's state during autonomous operation."

# TSF Type: Expectation
verification_method: "System Testing"

children:
  - id: SRD-SPEEDSENSOR

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"

reviewed: '2026-01-05 - Approved by Melanie Reis <melanie.reis@seame.pt>'

references:
  - type: "file"
    path: "docs/standards/iso26262/hara-motor-speed.md"
    description: "HARA Hazard H-01 (Need for speed monitoring to prevent injury)"

active: true
derived: false
normative: true
level: 1.0
---
The operator must be able to verify if the vehicle is stopped or moving, and at what speed, to ensure safety during testing.
