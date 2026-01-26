---
id: SWD-DEGRADED_CONTROL
header: "Degraded Mode Control"
text: |
  "Implements reduced performance control during degraded communication."

ASIL: "A"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-DEGRADED_MODE_001
children: []
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Implements degraded mode control logic"
active: true
derived: false
normative: true
level: 3.0
---
The software shall implement a degraded control mode that limits the maximum speed or stops the vehicle when communication integrity cannot be guaranteed. It shall provide visual or audible feedback to the operator.
---
