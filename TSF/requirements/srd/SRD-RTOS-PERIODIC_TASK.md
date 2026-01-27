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
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Addresses scheduling hazards"
active: true
derived: false
normative: true
level: 2.0
---
The RTOS shall schedule safety-critical tasks (sensor reading, control loops) at their nominal periods and enforce deadlines. Missed deadlines or task starvation events shall be logged and trigger a fail-safe or degraded mode.
---
