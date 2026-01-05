---
id: SWD-SpeedSensor
header: "Speed Sensor Driver Logic"
text: |
  "The 'speed_sensor' module shall implement the 'read_speed_sensor()' function to:
   1. Calculate speed using: (Pulses/30) * 0.212 / dt.
   2. Handle 16-bit timer overflows (wrap-around).
   3. Detect zero speed (timeout).
   4. Prevent division by zero for small time deltas."

# TSF Type: Assertion
ASIL: "B"
verification_method: "Unit Testing, Code Coverage"

# Links
parents:
  - id: SRD-SpeedSensor
children:
  - id: LLTC-Speed-UnitTests
  - id: LLTC-Speed-Coverage

reviewers:
  - name: "Bernardo Esteves"
    email: "bernardo.esteves@seame.pt"

reviewed: '2026-01-05 - Approved by Firmware Lead <bernardo.esteves@seame.pt>'

references:
  - type: "file"
    path: "Core/Src/speed_sensor.c"
    description: "Implementation of speed logic"
  - type: "file"
    path: "tests/unit/speed_sensor/test/test_speed_sensor.c"
    description: "Unit Test Source Code"

active: true
derived: false
normative: true
level: 3.0
---
The driver reads the STM32 hardware timer (TIM1) directly. It maintains static state to calculate `delta_ticks` and `delta_pulses` between calls.
