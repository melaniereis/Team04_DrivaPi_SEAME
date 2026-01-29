---
id: SRD-RTOS-PERIODIC_TASK
header: "Periodic Task Scheduling"
text: |
  "Ensures periodic execution of safety-critical tasks."
verification_method: "Integration Testing"
parents:
  - id: URD-DETERMINISTIC_SCHED
children:
  - id: SWD-RTOS_SCHEDULER
  - id: HLTC-TIMING_JITTER
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''
references:
  - type: "file"
    path: docs/standards/iso26262/hara_rtos.md
    description: "Mitigates Hazard H-06 (Missed Deadline)"
active: true
derived: false
normative: true
level: 2.0
---
The RTOS shall schedule safety-critical tasks (sensor reading, control loops) at their nominal periods and enforce deadlines.
---
