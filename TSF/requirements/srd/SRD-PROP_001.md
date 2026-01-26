---
id: SRD-PROP_001
header: "DC Motor Propulsion Interface"
text: |
  "Bridges user propulsion command to hardware PWM constraints."
verification_method: "Integration Testing"
parents:
  - id: URD-PROP_001
children:
  - id: SWD-DC_MOTOR_DRIVER
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval
references:
  - type: "file"
    path: docs/standards/iso26262/hara_actuation.md
    description: "Supports SG-02 (Safe Propulsion)"
active: true
derived: false
normative: true
level: 2.0
---
The system shall control the DC drive motor via PCA9685 PWM channels, mapping commanded speed [-1.0, 1.0] to calibrated pulse widths with dead-zone at 0.0.
