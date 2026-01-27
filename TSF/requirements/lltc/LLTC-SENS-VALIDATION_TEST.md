---
id: LLTC-SENS-VALIDATION_TEST
header: "Sensor Validation Test Cases"
text: |
  "Unit tests verifying filtering and range checking."
verification_method: "Unit Test Execution"
parents:
  - id: SWD-SENS-DATA_VALIDATION
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
active: true
derived: true
normative: true
level: 4.0
---
Test Cases:
1. Provide noisy pulses to the speed measurement input; verify that the filtering algorithm outputs stable values within acceptable variance.
2. Provide sensor readings beyond physical limits (e.g., ultrasonic distance >10 m); verify that the software discards or clamps the values and raises an appropriate warning.
3. Induce persistent invalid inputs; verify that a fault is triggered and the system enters a safe or degraded mode.
