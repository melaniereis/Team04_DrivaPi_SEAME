---
id: SWD-OTA_MANAGER
header: "OTA Update Management"
text: |
  "Manages safe OTA updates with integrity verification and rollback."

ASIL: "A"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-SYS-OTA_SAFE_PROC
children: []
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Implements safe OTA update logic"
active: true
derived: false
normative: true
level: 3.0
---
The software shall manage OTA updates by performing integrity verification on packages, preventing updates during vehicle operation, and supporting rollback if an update fails or becomes corrupted.
---
