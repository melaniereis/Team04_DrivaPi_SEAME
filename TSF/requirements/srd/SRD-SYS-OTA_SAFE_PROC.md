---
id: SRD-SYS-OTA_SAFE_PROC
header: "Safe OTA Procedure"
text: |
  "Ensures OTA only in safe conditions with rollback."
verification_method: "Integration + System Testing"
parents:
  - id: URD-OTA_SAFE
children:
  - id: SWD-OTA_PROC
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''
references:
  - type: "file"
    path: docs/standards/iso26262/hara_overview.md
    description: "General Update Hazard"
active: true
derived: false
normative: true
level: 2.0
---
OTA process shall check vehicle stopped state before apply, and support rollback on failure.
