---
id: SWD-RTOS_SCHEDULER
header: "RTOS Scheduler Monitoring"
text: |
  "Ensures tasks meet deadlines and logs missed deadlines."
ASIL: "A"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-RTOS-PERIODIC_TASK
children:
  - id: LLTC-SCHEDULER_COV
  - id: LLTC-SCHEDULER_UNIT
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''
active: true
derived: false
normative: true
level: 3.0
---
The software shall monitor the RTOS scheduler to ensure that safety-critical tasks meet their deadlines.
---
