---
id: LLTC-STALE_DATA
header: "Stale Data Detection & Reaction"
text: |
  "Explicit TSF comm-integrity edge case proof: stale/timeout detection and controlled reaction."

verification_method: "Integration/System Test (Timeout + Logs)"

parents:
  - id: SWD-COMM_MONITOR
  - id: SWD-DEGRADED_CONTROL

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-STALE_DATA/stale_data_steps.md"
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-STALE_DATA/stale_data_logs.txt"

active: true
derived: false
normative: true
level: 4.0
---
If command/telemetry data becomes stale or times out, the system shall detect it, log it, and transition to safe/degraded behavior within the specified time.
---
