---
id: HLTC-VAL-COMM_INTEGRITY
header: "Command Integrity & Rejection Validation"
text: |
  "Validates that the vehicle ignores invalid commands and continues safe operation."

verification_method: "System Validation (Fuzzing/Injection)"

parents:
  - id: URD-COMM_INTEGRITY

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

active: true
derived: false
normative: true
level: 2.0
---
The system shall maintain nominal operation and reject execution when subjected to a stream of invalid or undefined CAN command IDs.
