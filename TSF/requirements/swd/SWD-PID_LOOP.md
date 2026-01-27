---
id: SWD-PID_LOOP
header: "Proportional-Integral-Derivative (PID) Autonomous Control Loop"
text: |
  "Maintains stable autonomous trajectory."
ASIL: "QM"
verification_method: "Unit Testing, System Testing"
parents:
  - id: SRD-CTRL-AUTO_LOOP
children:
  - id: LLTC-PID_UNIT
  - id: LLTC-PID_COV
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval
# references:
#   - type: "file"
#     path: firmware/Core/Src/pid_control.c
#     description: "Production Implementation (placeholder)"
active: true
derived: false
normative: true
level: 3.0
---
PID loop shall fuse sensor/vision data to maintain stable speed/trajectory <4 m/s.
