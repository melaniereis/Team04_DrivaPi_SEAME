---
id: HLTC-TIMING_JITTER
header: "Real-Time Timing Jitter Measurement"
text: |
  "Proves with objective jitter/latency evidence under load."

verification_method: "Timing Measurement (On-target Profiling + Logs)"

parents:
  - id: SRD-RTOS-PERIODIC_TASK

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/timing/HLTC-TIMING_JITTER/jitter_results.csv"
#   - type: "file"
#     path: "artifacts/verification/timing/HLTC-TIMING_JITTER/jitter_setup.md"
#   - type: "file"
#     path: "artifacts/verification/timing/HLTC-TIMING_JITTER/jitter_logs.txt"

active: true
derived: false
normative: true
level: 4.0
---
Under realistic load, the control loop latency/jitter shall remain within the defined bound (target <50ms).
---
