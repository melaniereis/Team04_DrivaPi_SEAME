---
id: URD-HMI_001
header: "Driver Dashboard"
text: |
  "Provides real-time vehicle status to the operator."
verification_method: "System Testing"
children:
  - id: SRD-QT_DISPLAY
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
Qt HMI shall display speed, battery, warnings and status in real-time.
---
