---
id: SRD-CTRL-AUTO_LOOP
header: "Autonomous Control Loop Logic"
text: |
  "Translates perception data into stable propulsion and steering commands."
verification_method: "Integration Testing"
parents:
  - id: URD-AUTONOMOUS_PERF
children:
  - id: SWD-PID_LOOP
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''
references:
  - type: "file"
    path: docs/standards/iso26262/hara_performance.md
    description: "Supports SG-10 (Control Stability)"
active: true
derived: false
normative: true
level: 2.0
---
The system shall implement a closed-loop control frequency of at least 20Hz. It shall fuse lane-keeping offsets from the vision pipeline and current speed from encoders to calculate PWM outputs that maintain a target trajectory while enforcing a hard speed limit of 4 m/s.
