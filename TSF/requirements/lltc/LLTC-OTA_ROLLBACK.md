---
id: LLTC-OTA_ROLLBACK
header: "OTA Safe Update & Rollback"
text: |
  "Proves URD-OTA_SAFE: safe gating + rollback on failure/corruption."

verification_method: "System Functional Test (OTA + Logs)"

parents:
  - id: SWD-OTA_MANAGER

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-OTA_ROLLBACK/ota_test_plan.md"
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-OTA_ROLLBACK/ota_logs.txt"
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-OTA_ROLLBACK/ota_rollback_evidence.md"

active: true
derived: false
normative: true
level: 4.0
---
OTA updates shall be blocked while moving, allowed only in the safe state, and failed/corrupt updates shall rollback safely.$
---
