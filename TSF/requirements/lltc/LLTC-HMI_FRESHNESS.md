---
id: LLTC-HMI_FRESHNESS
header: "HMI Freshness / Latency"
text: |
  "Demonstrates dashboard freshness performance and supports URD-DRIVER_DASHBOARD evidence needs."

verification_method: "System Test (Latency Measurement + Logs)"

parents:
  - id: SWD-QT_DASHBOARD
  - id: SWD-COMM_PROTOCOL

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-HMI_FRESHNESS/hmi_latency.csv"
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-HMI_FRESHNESS/hmi_latency_logs.txt"

active: true
derived: false
normative: true
level: 4.0
---
The dashboard shall display telemetry with acceptable freshness/latency under nominal and stressed conditions.
---
