---
id: SRD-SENS-ENCODER_SPD
header: "Wheel Encoder Speed Calculation"
text: |
  "Provides accurate speed feedback for monitoring and control loops."
verification_method: "Integration Testing"
parents:
  - id: URD-SPEED_MEASUREMENT
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
Calculate speed as (pulse_count / 30) × 0.212 / Δt in m/s, update at least every 100 ms, clamp results to 0–4 m/s and transmit the speed over CAN as a float.
