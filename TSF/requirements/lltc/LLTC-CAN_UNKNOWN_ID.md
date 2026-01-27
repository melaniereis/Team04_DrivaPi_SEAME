---
id: LLTC-CAN_UNKNOWN_ID
header: "Unknown CAN ID Handling"
text: |
  "Explicit TSF comm-integrity edge case proof: unknown IDs discarded + logged."

verification_method: "Integration Test (CAN Injection + Logs)"

parents:
  - id: SWD-CAN_VALIDATION

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-CAN_UNKNOWN_ID/can_injection_script.md"
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-CAN_UNKNOWN_ID/can_unknown_id_logs.txt"

active: true
derived: false
normative: true
level: 4.0
---
Unknown or unexpected CAN message identifiers shall be rejected/discarded and logged, without affecting safety-critical control flow.
