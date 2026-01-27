---
id: URD-SENSOR_FILTERING
header: "Sensor Signal Filtering & Validation"
text: |
  "Filters noise and invalid values from sensor inputs before use."
verification_method: "System Testing"
children:
  - id: SRD-SENS-FILTER_VALIDATION
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''
active: true
derived: false
normative: true
level: 1.0
---
The system shall filter out spurious or noisy pulses from sensors and discard values that exceed physical limits or are outside valid ranges before using them for control or monitoring.
---
