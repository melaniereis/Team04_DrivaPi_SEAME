---
id: SWD-RTOS_SCHEDULER
header: "RTOS Scheduler Monitoring"
text: |
  "Ensures tasks meet deadlines and logs missed deadlines."

ASIL: "A"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-RTOS-PERIODIC_TASK
children: []
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Implements task scheduling monitoring logic"
active: true
derived: false
normative: true
level: 3.0
---
The software shall monitor the RTOS scheduler to ensure that safety-critical tasks meet their deadlines. If deadlines are missed or tasks starve, it shall log and trigger a safe or degraded state.
---
