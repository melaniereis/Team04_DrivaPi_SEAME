---
id: SWD-RESOURCE_MONITOR
header: "System Resource Monitoring"
text: |
  "Monitors CPU temperature, voltage and storage."

ASIL: "QM"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-SYS-RES_MONITOR
children: []
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Implements resource monitoring logic"
active: true
derived: false
normative: true
level: 3.0
---
At defined intervals, the software shall read system temperature, supply voltage, and free storage. If any value violates its threshold, the software shall log the event, notify the operator, and enter a safe state.
---
