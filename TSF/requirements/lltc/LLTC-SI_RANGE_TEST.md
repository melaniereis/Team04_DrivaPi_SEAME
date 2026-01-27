---
id: LLTC-SI_RANGE_TEST
header: "SI Units & Range Validation"
text: |
  "Explicit TSF signal-validation proof: wrong units/range do not propagate to control decisions."

verification_method: "Integration Test (Signal Injection + Logs)"

parents:
  - id: SWD-UNIT_VALIDATION

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-SI_RANGE_TEST/signal_injection_cases.md"
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-SI_RANGE_TEST/unit_range_logs.txt"

active: true
derived: false
normative: true
level: 4.0
---
Signals shall be validated for correct SI units and acceptable numeric ranges; invalid values shall be rejected and logged.
---
