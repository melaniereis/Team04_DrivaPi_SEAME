---
id: LLTC-FAULT_INJECTION
header: "Fault Injection & Failsafe Reaction (System Functional)"
text: |
  "Validates URD-FAULT_FAILSAFE and degraded-mode transitions under realistic injected faults."

verification_method: "System Functional Test (Fault Injection + Logs)"

parents:
  - id: SWD-FAULT_MONITOR
  - id: SWD-DEGRADED_CONTROL

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-FAULT_INJECTION/fault_matrix.md"
#     description: "Faults injected, expected reaction, observed reaction."
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-FAULT_INJECTION/fault_logs.txt"
#     description: "Timestamped logs demonstrating detection and reaction."
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-FAULT_INJECTION/fault_video.mp4"
#     description: "Optional video proof for stall/comms cases."

active: true
derived: false
normative: true
level: 4.0
---
Injected faults (sensor disconnect, comms loss, actuator stall) shall be detected and shall trigger safe/degraded behavior as specified.
---
