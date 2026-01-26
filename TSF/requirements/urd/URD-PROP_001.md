---
id: URD-PROP_001
header: "Propulsion Control"
text: |
  "The user expects the "Gas" and "Brake/Reverse" commands to translate into physical motion."
verification_method: "System Testing"
children:
  - id: SRD-PROP_001
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
  - type: "file"
    path: docs/standards/iso26262/hara_actuation.md
    description: "Mitigates Hazard H-02 (Unintended Acceleration)"
active: true
derived: false
normative: true
level: 1.0
---
The vehicle shall provide propulsion control to move forward and backward at requested speeds (0-100%).
---
