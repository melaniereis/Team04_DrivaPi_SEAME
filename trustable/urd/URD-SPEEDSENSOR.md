---
id: URD-SpeedSensor
header: "Vehicle Speed Monitoring"
text: |
  "The DrivaPi system shall calculate and display the current vehicle linear speed to enable the operator to monitor the vehicle's state during autonomous operation."

# TSF Type: Expectation
verification_method: "System Testing"

# Links: Connects to child Assertion
children:
  - id: SRD-SpeedSensor

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"

# Auto-approved for this task
reviewed: '2026-01-05 - Approved by Melanie Reis <melanie.reis@seame.pt>'

references: []

active: true
derived: false
normative: true
level: 1.0
---
The operator must be able to verify if the vehicle is stopped or moving, and at what speed, to ensure safety during testing.
