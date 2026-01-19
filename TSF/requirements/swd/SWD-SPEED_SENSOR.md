---
id: SWD-SPEED_SENSOR
header: "Speed Sensor Logic Implementation"
text: |
  "The 'speed_sensor' software component shall implement 'read_speed_sensor()' to:
   1. Calculate speed (m/s) as: (Pulses / 30) * 0.212 / dt.
   2. Handle 16-bit timer overflows correctly.
   3. Detect zero speed via timeout.
   4. Prevent division by zero."

# TSF Type: Assertion (Safety Claim)
ASIL: "A"
verification_method: "Unit Testing, Code Coverage, Static Analysis"

parents:
  - id: SRD-SPEED_SENSOR

children:
  - id: LLTC-SPEED_SENSOR_UNIT
  - id: LLTC-SPEED_SENSOR_COV
  - id: LLTC-SPEED_SENSOR_STATIC

reviewers:
  - name: "Bernardo Esteves"
    email: "bernardo.esteves@seame.pt"

reviewed: '2026-01-09 - Approved by Bernardo Esteves <bernardo.esteves@seame.pt>'

references:
  - type: "file"
    path: "firmware/Core/Src/speed_sensor.c"
    description: "Source code implementation"
  - type: "file"
    path: "docs/standards/iso26262/asil_justification_SWD-SPEED_SENSOR.md"
    description: "ASIL A Justification"

active: true
derived: false
normative: true
level: 3.0
---
The software reads the STM32 TIM1 hardware timer. It maintains state to calculate deltas between calls, using `(TIMER_PERIOD - last_count) + current_count + 1` to safely handle timer wrap-arounds.
