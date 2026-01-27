---
id: SWD-SENS-DATA_VALIDATION
header: "Sensor Data Validation Implementation"
text: |
  "Implements filtering and validation mechanisms for sensor data."
ASIL: "QM"
verification_method: "Unit Testing"
parents:
  - id: SRD-SENS-FILTER_VALIDATION
children:
  - id: LLTC-SENS-VALIDATION_TEST
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''
active: true
derived: true
normative: true
level: 3.0
---
The implementation shall apply appropriate filters (e.g., moving average for ultrasonic sensors, edge debouncing for pulse counters), enforce allowable ranges for each sensor, handle out-of-range values by discarding or clamping, and trigger a fault if persistent invalid inputs are detected.
---
