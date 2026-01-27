---
id: SRD-SENS-FILTER_VALIDATION
header: "Filtering & Validation of Sensor Inputs"
text: |
  "Defines data validation and filtering rules for sensor signals."
verification_method: "Integration Testing"
parents:
  - id: URD-SENSOR_FILTERING
children:
  - id: SWD-SENS-DATA_VALIDATION
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''
active: true
derived: true
normative: true
level: 2.0
---
Sensor input processing shall include debounce filtering, noise reduction (e.g., low-pass filtering), and range checking (e.g., speed pulses limited to expected maximum; distance sensor readings between 0 and 10 m). Invalid inputs shall be ignored, replaced with the last valid value, or flagged for error handling.
