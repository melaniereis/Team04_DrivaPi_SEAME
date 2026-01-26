---
id: SRD-OTA_UPDATE_001
header: "Safe OTA Update Process"
text: |
  "Defines a safe process for OTA updates."
verification_method: "Integration Testing"
parents:
  - id: URD-RESSOURCES_OTA_001
children:
  - id: SWD-OTA_MANAGER
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Mitigates OTA update hazards"
active: true
derived: false
normative: true
level: 2.0
---
OTA software updates shall only be started when the vehicle is stopped and the drive system is disengaged. The update mechanism shall verify the integrity of update packages and provide a rollback mechanism if the update fails or becomes corrupted. The system shall not operate the vehicle using an update in progress.
---
