---
id: URD-HMI_002
header: "Critical Alerts"
text: |
  "Ensures operator awareness of critical conditions."
verification_method: "System Testing"
children:
  - id: SRD-HMI_ALERTS
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
  - type: "file"
    path: docs/standards/iso26262/hara_hmi.md
    description: "Mitigates Hazard H-07 (Stale HMI Data)"
active: true
derived: false
normative: true
level: 1.0
---
HMI shall clearly show critical alerts (collision, stall, comms loss).
---
