---
id: SRD-SPEEDSENSOR
header: "Speed Calculation via Wheel Encoder"
text: |
  "The system shall calculate the speed in m/s by measuring the time delta between wheel encoder pulses using a hardware timer, assuming a wheel perimeter of 0.212m and 30 pulses per revolution."

# TSF Type: Assertion
verification_method: "Integration Testing"

parents:
  - id: URD-SPEEDSENSOR
children:
  - id: SWD-SPEEDSENSOR

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"

reviewed: '2026-01-05 - Approved by Melanie Reis <melanie.reis@seame.pt>'

references:
  - type: "file"
    path: "Threadx/Core/Inc/speed_sensor.h"
    description: "Header definitions for wheel constants"

active: true
derived: false
normative: true
level: 2.0
---
This requirement bridges the user need for "Speed" with the hardware reality (Encoder Pulses + Timer).
