---
id: LLTC-PLAUSIBILITY_TEST
header: "Cross-Signal Plausibility Validation"
text: |
  "Explicit TSF signal-validation proof: plausibility mismatch triggers controlled fail-safe behavior."

verification_method: "Integration/System Test (Mismatch Injection + Logs)"

parents:
  - id: SWD-PLAUSIBILITY_CHECK
  - id: SWD-FAULT_MONITOR
  - id: SWD-DEGRADED_CONTROL

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-PLAUSIBILITY_TEST/plausibility_cases.md"
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-PLAUSIBILITY_TEST/plausibility_logs.txt"

active: true
derived: false
normative: true
level: 4.0
---
Plausibility mismatches shall be detected, logged, and shall trigger the specified fault response and safe/degraded mode.
---
