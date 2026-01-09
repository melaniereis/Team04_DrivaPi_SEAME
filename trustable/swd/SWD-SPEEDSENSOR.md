---
id: SWD-SPEEDSENSOR
header: "Speed Sensor Logic Implementation"
text: |
  "The 'speed_sensor' software component shall implement the 'read_speed_sensor()' function to:
   1. Calculate linear vehicle speed (m/s) using the formula: (Pulses / 30) * 0.212 / dt.
   2. Handle 16-bit timer overflows (wrap-around) ensuring continuous measurement.
   3. Detect zero speed conditions via timeout logic (no pulses within threshold).
   4. Prevent division by zero for small time deltas (<1ms) by returning 0.0."

# TSF Type: Assertion (Safety Claim)
ASIL: "A"
verification_method: "Unit Testing, Code Coverage, Static Analysis"

parents:
  - id: SRD-SPEEDSENSOR

children:
  - id: LLTC-SPEEDUNITTESTS
  - id: LLTC-SPEEDCOVERAGE
  - id: LLTC-SPEEDSTATICANALYSIS

reviewers:
  - name: "Bernardo Esteves"
    email: "bernardo.esteves@seame.pt"
reviewed: '2026-01-05 - Approved by Bernardo Esteves <bernardo.esteves@seame.pt>'

references:
  - type: "file"
    path: "Threadx/Core/Src/speed_sensor.c"
    description: "Source code implementation"

  - type: "file"
    path: "tests/unit/speed_sensor/test/test_speed_sensor.c"
    description: "Unit Test Source Code"

  - type: "file"
    path: "artifacts/verification/tests/junit_results.xml"
    description: "Unit Test Results (Pass/Fail)"

  - type: "file"
    path: "artifacts/verification/coverage/coverage.xml"
    description: "Code Coverage Report"

  - type: "file"
    path: "docs/standards/iso26262/hara-motor-speed.md"
    description: "Hazard Analysis (HARA)"

  - type: "file"
    path: "docs/standards/iso26262/asil-justification-SWD-SpeedSensor.md"
    description: "ASIL A Justification and Verification Strategy"

active: true
derived: false
normative: true
level: 3.0
---
The software reads the STM32 hardware timer (TIM1) directly. It maintains static state to calculate `delta_ticks` and `delta_pulses` between calls.
The critical safety logic is the handling of the 16-bit timer wrap-around: `(TIMER_PERIOD - last_count) + current_count + 1`. Without this, the system would calculate a massive negative delta or invalid speed during the overflow event.
