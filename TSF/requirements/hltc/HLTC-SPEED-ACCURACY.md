---
id: HLTC-SPEED_ACCURACY
header: "Speed Measurement Accuracy Test"
text: |
  "Verifies odometer accuracy against external ground truth."
verification_method: "System Test (External Chronometer/Laser)"

parents:
  - id: SRD-SENS-ENCODER_SPD
  - id: URD-SPEED_MEASUREMENT

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '2026-02-24 - Approved by Melanie Reis <melanie.reis@seame.pt>'

references:
  - type: "file"
    path: "tests/integration/integration_tests_plan.md"
    note: "Phase D — speed sensor integration test plan + acceptance criteria."
  - type: "file"
    path: "tests/integration/motors-and-speed-sensor-integration/src/stm32u5xx_it.c"
    note: "Phase D — EXTI9 interrupt handling (PD9) used for pulse counting."
  - type: "file"
    path: "tests/integration/motors-and-speed-sensor-integration/src/i2c_integration_test.c"
    note: "Shared integration harness used alongside speed test (if applicable)."
  - type: "file"
    path: "tests/integration/speed_sensor_integration.mp4"
    note: "Phase D — evidence video showing correct speed reporting under wheel spin."

score:
  MelanieReis: 0.4
#Because Phase D proves interrupt/pulse handling chain, but not ground-truth accuracy.

active: true
derived: false
normative: true
level: 2.0
---
Reported vehicle speed shall match external ground truth measurements within +/- 5% at speeds up to 4 m/s.
