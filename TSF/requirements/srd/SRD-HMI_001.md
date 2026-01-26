---
id: SRD-HMI_001
header: "Real-Time Dashboard Display"
text: |
  "Provides operator with current vehicle state."
verification_method: "System Testing"
parents:
  - id: URD-HMI_001
children:
  - id: SWD-QT_DASHBOARD
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval
references:
  - type: "file"
    path: docs/standards/iso26262/hara_hmi.md
    description: "Supports SG-07 (Fresh Display)"
active: true
derived: false
normative: true
level: 2.0
---
Qt HMI shall refresh speed, battery, status with <1s latency.
