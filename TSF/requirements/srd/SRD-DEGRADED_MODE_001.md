---
id: SRD-DEGRADED_MODE_001
header: "Degraded Mode Operation"
text: |
  "Defines degraded mode behaviour when communication integrity is compromised."
verification_method: "Integration Testing"
parents:
  - id: URD-COMM_SAFETY_001
children:
  - id: SWD-DEGRADED_CONTROL
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Addresses safe state strategy"
active: true
derived: false
normative: true
level: 2.0
---
In case of communication loss, high latency or stale data, the system shall reduce the maximum speed to a safe level (e.g., 10 %–20 % of nominal) or stop the vehicle. It shall log the event and signal the operator via the HMI.
---
