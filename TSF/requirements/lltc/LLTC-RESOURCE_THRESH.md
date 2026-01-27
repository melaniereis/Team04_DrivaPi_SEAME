---
id: LLTC-RESOURCE_THRESH
header: "Resource Threshold Protection (System Functional)"
text: |
  "Proves URD-RESOURCES_MONITOR behavior with explicit threshold triggers and observed actions."

verification_method: "System Functional Test (Stress/Inject + Logs)"

parents:
  - id: SWD-RESOURCE_MONITOR

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-RESOURCE_THRESH/resource_threshold_plan.md"
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-RESOURCE_THRESH/resource_logs.txt"
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-RESOURCE_THRESH/resource_alerts.csv"

active: true
derived: false
normative: true
level: 4.0
---
When resource thresholds are exceeded (temperature/voltage/storage), the system shall detect, log, and enter the specified safe/degraded state.
---
