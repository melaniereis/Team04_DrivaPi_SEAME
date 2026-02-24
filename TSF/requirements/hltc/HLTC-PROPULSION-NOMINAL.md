---
id: HLTC-PROPULSION_NOMINAL
header: "Propulsion Control System Test"
text: |
  "Verifies that throttle commands translate to correct wheel rotation and direction."
verification_method: "System Test (Tachometer/Encoder + Logs)"

parents:
  - id: SRD-ACT-MOTOR_DRIVE
  - id: URD-PROPULSION_CONTROL

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

  # Phase C — motors integration evidence (downstream actuation)
  - type: "file"
    path: "tests/integration/motors-and-speed-sensor-integration/src/i2c_integration_test.c"
    note: "Phase C — I2C motor command/ack validation."
  - type: "file"
    path: "tests/integration/motors-and-speed-sensor-integration/src/stm32u5xx_it.c"
    note: "Phase C — MCU interrupt support used during integration."
  - type: "file"
    path: "tests/integration/motors_integration.mp4"
    note: "Phase C — evidence video of proportional speed + direction behavior."

score: 1.0

active: true
derived: false
normative: true
level: 2.0
---
Throttle commands (0-100%) shall produce proportional wheel speeds, and direction commands shall correctly define motor rotation.
