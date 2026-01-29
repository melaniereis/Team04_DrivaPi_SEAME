---
id: HLTC-ALERT_DISPLAY
header: "Critical Alerts Display Correctness"
text: |
  "Proves SRD-HMI-ALERTS: alert propagation → HMI presentation within expected timing."

verification_method: "System Test (Trigger + Observation + Logs)"

parents:
  - id: SRD-HMI-ALERTS

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/HLTC-ALERT_DISPLAY/alert_trigger_cases.md"
#   - type: "file"
#     path: "artifacts/verification/system/HLTC-ALERT_DISPLAY/alert_logs.txt"
#   - type: "file"
#     path: "artifacts/verification/system/HLTC-ALERT_DISPLAY/alert_video.mp4"

active: true
derived: false
normative: true
level: 3.0
---
Critical alerts shall be displayed clearly and promptly when triggered, including correct alert type and timestamp.
