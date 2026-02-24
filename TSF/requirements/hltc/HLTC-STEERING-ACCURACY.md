---
id: HLTC-STEERING_ACCURACY
header: "Steering Angle Accuracy System Test"
text: |
  "Verifies that steering commands result in correct physical wheel angles."
verification_method: "System Test (Angle Measurement)"

parents:
  - id: SRD-ACT-SERVO_STEER
  - id: URD-STEERING_CONTROL

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '2026-02-24 - Approved by Melanie Reis <melanie.reis@seame.pt>'

references:
  # Phase A — PS3 controller input decoding on RPi (upstream command source)
  - type: "file"
    path: "tests/integration/integration_tests_plan.md"
    note: "Phase A — PS3 Controller → Raspberry Pi input receive/decode/validate."
  - type: "file"
    path: "rust/controller/src/main.rs"
    note: "Phase A — controller decoding implementation."
  - type: "file"
    path: "tests/integration/joystick_integration.mp4"
    note: "Phase A — evidence video/log capture of decoded inputs."

score:
  MelanieReis: 0.2
#Because we only have Phase A (command decoding) evidence, not physical angle measurement.

active: true
derived: false
normative: true
level: 2.0
---
Commanded steering angles (Left/Right) shall correspond to physical wheel angles within defined tolerance (+/- 2 degrees).
