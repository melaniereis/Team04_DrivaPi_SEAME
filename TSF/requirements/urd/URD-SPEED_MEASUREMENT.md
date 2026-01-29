---
id: URD-SPEED_MEASUREMENT
header: "Vehicle Speed Measurement"
text: |
  "Operators need accurate speed for monitoring and control during autonomous operation."
verification_method: "System Testing"
children:
  - id: SRD-SENS-ENCODER_SPD
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
The microcontroller shall compute and report vehicle speed in metres per second using the wheel encoder, updating every 100 ms, with proper handling of timer wrap‑around and division‑by‑zero.
---
