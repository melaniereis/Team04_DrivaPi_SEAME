---
id: LLTC-ESTOP_TIME
header: "Emergency Stop Timing (System Functional)"
text: |
  "On critical stop trigger, propulsion PWM shall reach zero and steering shall enter the defined safe state within the timing target."

verification_method: "System Functional Test (Measurement + Logs + Video)"

parents:
  - id: SWD-ESTOP_EXEC

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)

# references:
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-ESTOP_TIME/estop_log.txt"
#     description: "Timestamped log showing stop trigger and PWM->0 transition."
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-ESTOP_TIME/estop_timing.csv"
#     description: "Measured stop latency samples."
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-ESTOP_TIME/estop_video.mp4"
#     description: "Video proof of stop behavior."

active: true
derived: false
normative: true
level: 4.0
---
Proves URD-EMERGENCY_STOP behavior with objective timing evidence (stop + safe steering).
