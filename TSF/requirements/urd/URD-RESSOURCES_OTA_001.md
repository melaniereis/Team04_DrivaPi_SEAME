---
id: URD-RESOURCE_OTA_001
header: "System Resource & OTA Management"
text: |
  "Monitors system resources and manages OTA updates safely."
verification_method: "System Testing"
children:
  - id: SRD-RESOURCE_MONITORING
  - id: SRD-OTA_UPDATE
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Addresses resource exhaustion and OTA hazards"
active: true
derived: false
normative: true
level: 1.0
---
The system shall monitor Raspberry Pi temperature, voltage and storage capacity during operation and generate alerts or enter safe state when thresholds are exceeded. OTA software updates shall only be applied when the vehicle is stopped and shall support rollback if an update fails or becomes corrupted.
---
