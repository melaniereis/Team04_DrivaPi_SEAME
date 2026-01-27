---
id: SRD-RESOURCE_MONITORING_001
header: "System Resource Monitoring"
text: |
  "Implements monitoring of critical system resources."
verification_method: "Integration Testing"
parents:
  - id: URD-RESOURCES_MONITOR
children:
  - id: SWD-RESOURCE_MONITOR
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Mitigates system resource hazards"
active: true
derived: false
normative: true
level: 2.0
---
The system shall monitor CPU temperature, supply voltage and available storage with thresholds defined in the hazard analysis. On threshold violation, it shall log the event, notify the operator and enter a safe state (e.g., throttle limit or shutdown) if necessary.
---
