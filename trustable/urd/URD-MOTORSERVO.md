---
id: URD-SERVOMOTOR
header: "Vehicle Motion Control"
text: |
  "The DrivaPi system shall provide unified control over vehicle actuators (Motor and Servo) to execute operator motion commands reliably."

# TSF Type: Expectation
verification_method: "System Testing"

children:
  - id: SRD-SERVOMOTOR

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"

reviewed: '2026-01-07 - Approved by Melanie Reis'

references:
  - type: "file"
    path: "docs/standards/iso26262/hara-actuation.md"
    description: "Mitigates Hazard H-02 (Unintended Actuation)"

active: true
derived: false
normative: true
level: 1.0
---
From the user's perspective, the system must translate "Go" and "Turn" commands into physical movement without erratic behavior or exceeding physical limits.