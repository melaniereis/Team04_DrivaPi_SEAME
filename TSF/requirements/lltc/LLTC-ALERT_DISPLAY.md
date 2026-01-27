---
id: LLTC-ALERT_DISPLAY
header: "Critical Alerts Display Correctness"
text: |
  "Proves URD-CRITICAL_ALERTS: alert propagation → HMI presentation within expected timing."

verification_method: "System Test (Trigger + Observation + Logs)"

parents:
  - id: SWD-QT_ALERTS

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-ALERT_DISPLAY/alert_trigger_cases.md"
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-ALERT_DISPLAY/alert_logs.txt"
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-ALERT_DISPLAY/alert_video.mp4"

active: true
derived: false
normative: true
level: 4.0
---
Critical alerts shall be displayed clearly and promptly when triggered, including correct alert type and timestamp.
