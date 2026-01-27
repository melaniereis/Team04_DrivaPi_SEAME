---
id: SWD-RTOS_CONFIG
header: "ThreadX Task Configuration"
text: |
  "Ensures deterministic scheduling for control tasks."
ASIL: "A"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-RTOS-SCHED_LATENCY
children:
  - id: LLTC-RTOS_UNIT
  - id: LLTC-RTOS_COV
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval
# references:
#   - type: "file"
#     path: firmware/Core/Src/threadx_config.c
#     description: "Production Implementation (placeholder)"
active: true
derived: false
normative: true
level: 3.0
---
ThreadX configuration shall use priority inheritance and guarantee <50ms latency for critical tasks.
