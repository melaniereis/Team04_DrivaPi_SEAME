---
id: URD-OTA_SAFE
header: "Safe OTA Update Process"
text: |
  "Manages OTA updates only in safe conditions with rollback."
verification_method: "System Testing + Validation Evidence (Logs/Video)"
children:
  - id: SRD-SYS-OTA_SAFE_PROC
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''
# references:
#   - type: "file"
#     path: docs/standards/iso26262/hara_overview.md
#     description: "General Update Hazard"
active: true
derived: false
normative: true
level: 1.0
---
OTA updates shall only be applied when the vehicle is stopped, and shall support rollback if the update fails or becomes corrupted.
