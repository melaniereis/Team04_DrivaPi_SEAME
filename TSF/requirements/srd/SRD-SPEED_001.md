---
id: SRD-SPEED_001
header: "Wheel Encoder Speed Calculation"
text: |
  "Provides accurate speed feedback for monitoring and control loops."
verification_method: "Integration Testing"
parents:
  - id: URD-SPEED_001
children:
  - id: SWD-SPEED_CALC
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval
references:
  - type: "file"
    path: docs/standards/iso26262/hara_motor_speed.md
    description: "Supports SG-01 (Prevent False Zero Speed)"
active: true
derived: false
normative: true
level: 2.0
---
The system shall calculate linear speed (m/s) from encoder pulses using wheel perimeter 0.212m and 30 pulses/rev, with overflow/timeout handling.
