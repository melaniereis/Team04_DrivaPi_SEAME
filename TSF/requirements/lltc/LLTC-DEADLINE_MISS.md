---
id: LLTC-DEADLINE_MISS
header: "Deadline Miss / Queue Overrun Detection"
text: |
  "Explicit TSF determinism proof: deadline misses/overruns are handled deterministically and safely."

verification_method: "Stress/Injection Test (Overrun + Logs)"

parents:
  - id: SWD-QUEUE_MONITOR
  - id: SWD-RTOS_SCHEDULER
  - id: SWD-DEGRADED_CONTROL

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-DEADLINE_MISS/overrun_steps.md"
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-DEADLINE_MISS/overrun_logs.txt"

active: true
derived: false
normative: true
level: 4.0
---
If task deadlines are missed or queues overrun persistently, the system shall detect, log, and enter the specified safe/degraded behavior.
---
