---
id: HLTC-TIMING_JITTER
header: "Real-Time Timing Jitter Measurement"
text: |
  "Proves SRD-RTOS-PERIODIC_TASK with objective jitter/latency evidence under load."

verification_method: "Timing Measurement (On-target Profiling + Logs)"

parents:
  - id: SRD-RTOS-PERIODIC_TASK

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

active: true
derived: false
normative: true
level: 2.0
---
Under realistic load, the control loop latency/jitter shall remain within the defined bound (target <50ms).
---
