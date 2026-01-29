---
id: HLTC-CAN_UNKNOWN_ID
header: "Unknown CAN ID Handling"
text: |
  "Explicit TSF comm-integrity edge case proof: unknown IDs discarded + logged."

verification_method: "Integration Test (CAN Injection + Logs)"

parents:
  - id: SRD-COMM-CAN_VALIDATION

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/HLTC-CAN_UNKNOWN_ID/can_injection_script.md"
#   - type: "file"
#     path: "artifacts/verification/system/HLTC-CAN_UNKNOWN_ID/can_unknown_id_logs.txt"

active: true
derived: false
normative: true
level: 3.0
---
Unknown or unexpected CAN message identifiers shall be rejected/discarded without affecting safety-critical control flow.
