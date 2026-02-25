# Integration Test Execution Template - Phase D
## Speed Sensor → STM32 Pulse Signal Validation

---

## Header / Metadata
- **Date:** [YYYY-MM-DD]
- **Time Started:** [HH:MM]
- **Tester Name:** [Full Name]
- **Test Phase:** C
- **Test Cycle:** [Sprint X]

---

## Test Information
- **Phase Name:** Speed Sensor Pulse Signal & Speed Calculation Validation
- **Test Objective:** Validate speed sensor pulse signals are correctly received by STM32, speed calculations are accurate within 1% error margin, and pulse-to-speed conversion is consistent
- **Related Requirements:**
  - Integration Test Plan - Phase D
  - `/docs/tests/integration-test-plan.md`
  - Speed sensor datasheet and pulse specification

---

## Environment Setup

### Hardware Required
- [ ] STM32U585 with input capture/timer capability
- [ ] Speed sensor (proximity sensor or wheel encoder)
- [ ] Signal generator (or test wheel with known rotation rate)
- [ ] Oscilloscope or logic analyzer (for pulse capture)
- [ ] Reference tachometer (for accuracy validation)
- [ ] Power supply for sensor and STM32
- [ ] USB cable for STM32 interface

### Software Required
- [ ] STM32 firmware with speed sensor driver (version: __________)
- [ ] Signal generator control software (if using generator)
- [ ] Oscilloscope/logic analyzer software
- [ ] Speed calculation verification tool
- [ ] Data logging application

### Configuration Settings
- **Sensor Type:** [Proximity, Hall effect, Wheel encoder, other]
- **Input Timer/Pin:** [TIM2_CH1, TIM3_CH1, GPIO portX.Y, other]
- **Expected Pulse Frequency Spec:** _______ Hz/RPM
- **Sensor Resolution:** _______ pulses/revolution
- **Accuracy Target:** ± 1% error

### Prerequisites Checklist
- [ ] Sensor power supply verified and stable
- [ ] Sensor input signal properly connected to STM32
- [ ] Timer/input capture interrupt configured
- [ ] Logging enabled in firmware
- [ ] Oscilloscope/logic analyzer connected
- [ ] Signal generator ready (if applicable)
- [ ] Reference tachometer calibrated
- [ ] Previous test data archived

---

## Pre-Test Checklist
- [ ] Sensor power voltage: _______ V (expected: _______ V)
- [ ] Signal output line not shorted
- [ ] Oscilloscope connected and showing sensor signal
- [ ] STM32 powered and responsive
- [ ] Serial/debug interface ready for logging
- [ ] Firmware compiled with latest speed sensor driver
- [ ] Test environment quiet and isolated
- [ ] Reference tachometer ready and calibrated

---

## Execution Steps

### Step 1: Verify Sensor Signal Physical Characteristics
**Procedure:**
1. Set signal generator to known low frequency (e.g., 10 Hz)
2. Measure signal voltage, frequency, and duty cycle
3. Verify signal meets sensor specifications
4. Check for noise or glitches

**Expected Output:**
```
Sensor signal detected on oscilloscope
Frequency: ~10 Hz
Amplitude: 3.3V (or 5V)
Duty cycle: ~50%
No visible glitches or noise
```

**Actual Output:**
```
[Record measurements here]
```

**Signal Measurements:**
- Signal voltage (peak): _______ V
- Signal voltage (minimum): _______ V
- Signal frequency: _______ Hz (expected: _______ Hz)
- Duty cycle: _______ %
- Rise time: _______ ns
- Fall time: _______ ns
- Noise level: _______ mV (peak-to-peak)

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 2: Configure STM32 Sensor Input
**Procedure:**
1. Load STM32 firmware with speed sensor driver
2. Configure input capture timer
3. Enable interrupts for pulse counting
4. Verify initial pulse count is zero

**Expected Output:**
```
STM32 Input Capture Configured
Timer: TIM2 (or equivalent)
Interrupt Priority: High
Pulse count: 0
Ready for sensor input
```

**Actual Output:**
```
[Record output here]
```

**Configuration Verification:**
- Timer selected: _______
- Prescaler: _______
- Capture mode: [ ] Rising edge [ ] Falling edge [ ] Both
- Interrupt enabled: [ ] Yes [ ] No
- Initial pulse count: _______

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 3: Low-Frequency Pulse Detection (10 Hz Test)
**Procedure:**
1. Set signal generator to 10 Hz (known frequency)
2. Start pulse counting on STM32
3. Let sensor run for 60 seconds
4. Calculate expected vs. actual pulse count

**Test Configuration:**
- Generator frequency: 10 Hz
- Expected pulses (60s): 10 × 60 = 600 pulses
- Test duration: 60 seconds

**Expected Result:**
```
Pulse count after 60s: ~600 pulses (exact: 600)
Error: 0%
Frequency calculated: 10.0 Hz
```

**Actual Result:**
```
[Record measurements here]
```

**Pulse Count Results:**
- Expected pulse count: 600
- Actual pulse count: _______
- Difference: _______ pulses
- Percentage error: _______ %
- Calculated frequency: _______ Hz
- Frequency error: _______ %

**Pass Criteria:** Error < 1% ✓ (Actual: _______ %)

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 4: Medium-Frequency Pulse Detection (100 Hz Test)
**Procedure:**
1. Set signal generator to 100 Hz
2. Run for 60 seconds
3. Compare expected vs. actual pulse count
4. Verify timing accuracy at higher frequency

**Test Configuration:**
- Generator frequency: 100 Hz
- Expected pulses (60s): 100 × 60 = 6000 pulses
- Test duration: 60 seconds

**Expected Result:**
```
Pulse count after 60s: ~6000 pulses
Error: < 1%
Frequency calculated: 100.0 Hz
```

**Actual Result:**
```
[Record measurements here]
```

**Pulse Count Results:**
- Expected pulse count: 6000
- Actual pulse count: _______
- Difference: _______ pulses
- Percentage error: _______ %
- Calculated frequency: _______ Hz
- Frequency error: _______ %

**Pass Criteria:** Error < 1% ✓ (Actual: _______ %)

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 5: High-Frequency Pulse Detection (1000 Hz Test)
**Procedure:**
1. Set signal generator to 1000 Hz (1 kHz)
2. Run for 10 seconds (enough for accurate measurement)
3. Compare expected vs. actual pulse count
4. Verify timer can handle high frequency without overflow

**Test Configuration:**
- Generator frequency: 1000 Hz
- Expected pulses (10s): 1000 × 10 = 10,000 pulses
- Test duration: 10 seconds

**Expected Result:**
```
Pulse count after 10s: ~10,000 pulses
Error: < 1%
Frequency calculated: 1000.0 Hz
No timer overflow
```

**Actual Result:**
```
[Record measurements here]
```

**Pulse Count Results:**
- Expected pulse count: 10,000
- Actual pulse count: _______
- Difference: _______ pulses
- Percentage error: _______ %
- Calculated frequency: _______ Hz
- Frequency error: _______ %
- Timer overflows: _______

**Pass Criteria:** Error < 1% ✓ (Actual: _______ %)

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 6: Speed Calculation Verification (Low Speed)
**Procedure:**
1. Configure STM32 speed calculation based on wheel diameter
2. Set signal generator to simulate low vehicle speed (e.g., 5 mph / 8 km/h)
3. Measure calculated speed vs. expected speed
4. Verify conversion formula accuracy

**Calculation Formula:**
```
Pulse frequency = [Pulses / Time interval]
Speed (mph) = [Pulse frequency × 60] / [Pulses per mile]
Speed (km/h) = [Pulse frequency × 3.6] / [Pulses per km]
```

**Test Configuration:**
- Simulated low speed: 5 mph (8 km/h)
- Expected pulse frequency: _______ Hz
- Sensor resolution: _______ pulses/wheel rotation
- Wheel diameter: _______ inches

**Expected Result:**
```
Calculated speed: ~5.0 mph
Error: < ±0.05 mph
Speed match: 100%
```

**Actual Result:**
```
[Record measurements here]
```

**Speed Calculation Results:**
- Expected speed: 5.0 mph
- Actual speed calculated: _______ mph
- Speed error: _______ mph (_______ %)
- Frequency measurement: _______ Hz
- Formula validation: [ ] Correct [ ] Incorrect

**Pass Criteria:** Error < ±1% ✓ (Actual: _______ %)

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 7: Speed Calculation Verification (Medium Speed)
**Procedure:**
1. Set signal generator to simulate medium vehicle speed (e.g., 25 mph / 40 km/h)
2. Measure calculated speed vs. expected speed
3. Verify accuracy maintained across speed range
4. Document any non-linearity

**Test Configuration:**
- Simulated medium speed: 25 mph (40 km/h)
- Expected pulse frequency: _______ Hz

**Expected Result:**
```
Calculated speed: ~25.0 mph
Error: < ±0.25 mph
Speed match: 99.5-100%
```

**Actual Result:**
```
[Record measurements here]
```

**Speed Calculation Results:**
- Expected speed: 25.0 mph
- Actual speed calculated: _______ mph
- Speed error: _______ mph (_______ %)

**Pass Criteria:** Error < ±1% ✓ (Actual: _______ %)

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 8: Speed Calculation Verification (High Speed)
**Procedure:**
1. Set signal generator to simulate high vehicle speed (e.g., 50 mph / 80 km/h)
2. Measure calculated speed vs. expected speed
3. Verify no timer saturation or overflow errors
4. Check for any accuracy degradation at high frequency

**Test Configuration:**
- Simulated high speed: 50 mph (80 km/h)
- Expected pulse frequency: _______ Hz

**Expected Result:**
```
Calculated speed: ~50.0 mph
Error: < ±0.5 mph
No timer errors
Speed continuous and stable
```

**Actual Result:**
```
[Record measurements here]
```

**Speed Calculation Results:**
- Expected speed: 50.0 mph
- Actual speed calculated: _______ mph
- Speed error: _______ mph (_______ %)
- Timer status: [ ] Normal [ ] Overflow [ ] Error

**Pass Criteria:** Error < ±1% ✓ (Actual: _______ %)

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 9: Speed Accuracy Across Range (Sweep Test)
**Procedure:**
1. Gradually increase/decrease signal frequency from 10 Hz to 1000 Hz
2. Record calculated speed at each frequency point
3. Compare against theoretical speed values
4. Generate error curve to identify any non-linear regions

**Speed Sweep Test:**

| Gen Freq (Hz) | Expected Speed (mph) | Calculated Speed (mph) | Error (%) | Status |
|---------------|-------------------|---------------------|-----------|--------|
| 10 | ___ | ___ | ___ | PASS/FAIL |
| 25 | ___ | ___ | ___ | PASS/FAIL |
| 50 | ___ | ___ | ___ | PASS/FAIL |
| 100 | ___ | ___ | ___ | PASS/FAIL |
| 250 | ___ | ___ | ___ | PASS/FAIL |
| 500 | ___ | ___ | ___ | PASS/FAIL |
| 750 | ___ | ___ | ___ | PASS/FAIL |
| 1000 | ___ | ___ | ___ | PASS/FAIL |

**Accuracy Statistics:**
- Max error: _______ %
- Min error: _______ %
- Average error: _______ %
- Error std dev: _______ %

**Pass Criteria:** All points < ±1% error ✓

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 10: Rapid Speed Change Detection
**Procedure:**
1. Rapidly change signal generator frequency (simulating acceleration/deceleration)
2. Verify STM32 detects speed changes correctly
3. Measure response time
4. Verify no calculation glitches during transitions

**Speed Change Scenarios:**

| Scenario | From Speed | To Speed | Detection Time | Accuracy |
|----------|-----------|---------|----------------|----------|
| Acceleration 1 | 10 mph | 50 mph | ___ ms | ± ___ % |
| Deceleration 1 | 50 mph | 10 mph | ___ ms | ± ___ % |
| Acceleration 2 | 5 mph | 30 mph | ___ ms | ± ___ % |
| Rapid pulse | Static | 100 Hz spike | ___ ms | [Handled/Failed] |

**Expected Behavior:**
```
Speed change detected within 100ms
New speed calculated correctly within 1 measurement cycle
No spurious reading glitches
Smooth transition without major jumps
```

**Actual Behavior:**
```
[Record observations here]
```

**Detection Performance:**
- Average detection time: _______ ms
- Average new speed accuracy: _______ %
- Glitch count: _______
- Smooth transitions: [ ] Yes [ ] No

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

## Results Recording

### Overall Status
- [ ] **PASS** - All acceptance criteria met
- [ ] **FAIL** - One or more acceptance criteria not met
- [ ] **CONDITIONAL PASS** - Minor issues requiring documentation

### Pass/Fail Criteria Met
- [ ] Pulse signal detection working at all frequency ranges (10 Hz - 1000 Hz)
- [ ] Pulse count accuracy within ±1% at low frequency
- [ ] Pulse count accuracy within ±1% at medium frequency
- [ ] Pulse count accuracy within ±1% at high frequency
- [ ] Speed calculation formula verified and correct
- [ ] Calculated speed within ±1% of expected across entire speed range
- [ ] No calculation errors or timer overflows observed
- [ ] Speed transitions detected and calculated correctly

### Key Measurements Summary
| Metric | Value | Threshold | Status |
|--------|-------|-----------|--------|
| Low Freq Accuracy | ___% | ±1% | PASS/FAIL |
| Medium Freq Accuracy | ___% | ±1% | PASS/FAIL |
| High Freq Accuracy | ___% | ±1% | PASS/FAIL |
| Average Speed Error | ___% | ±1% | PASS/FAIL |
| Max Speed Error | ___% | ±1% | PASS/FAIL |
| Detection Response Time | ___ ms | < 100ms | PASS/FAIL |
| Timer Overflow Events | ___ | 0 | PASS/FAIL |

### Issues Encountered
- [ ] No issues
- [ ] Issues found (describe below):

**Issue #1:** [Description]
- Severity: [Critical/High/Medium/Low]
- Frequency range affected: [10Hz/100Hz/1000Hz/All]
- Root cause: [If known]
- Workaround: [If applicable]

**Issue #2:** [Description]
- Severity: [Critical/High/Medium/Low]
- Frequency range affected: [10Hz/100Hz/1000Hz/All]
- Root cause: [If known]
- Workaround: [If applicable]

### Performance Metrics Summary
- Average pulse accuracy across all frequencies: _______ %
- Average speed calculation error: _______ %
- Maximum frequency handled: _______ Hz
- Minimum detectable frequency: _______ Hz
- Speed update rate: _______ Hz
- CPU usage during speed calculation: _______ %

---

## Evidence Artifacts

### Pulse Waveforms
- [ ] Low frequency waveform (10 Hz): `[filename]`
  - Duration: _______ sec
  - Location: _______________
  
- [ ] Medium frequency waveform (100 Hz): `[filename]`
  - Duration: _______ sec
  - Location: _______________

- [ ] High frequency waveform (1000 Hz): `[filename]`
  - Duration: _______ sec
  - Location: _______________

### Speed Calculation Data
- [ ] Speed calculation log: `[filename]`
  - Format: [CSV, JSON, log]
  - Data points: _______
  - Location: _______________

- [ ] Error analysis file: `[filename]`
  - Min error: _______ %
  - Max error: _______ %
  - Location: _______________

### Screenshots
- [ ] Oscilloscope low frequency: `[filename]`
- [ ] Oscilloscope high frequency: `[filename]`
- [ ] Speed calculation graph: `[filename]`
- [ ] Error distribution chart: `[filename]`

### Logs
- [ ] STM32 speed sensor log: `[filename]`
  - Location: _______________
  - Entries: _______
  
- [ ] Signal generator log (if applicable): `[filename]`
  - Location: _______________

---

## Notes / Observations

### Sensor Signal Quality
[Observations about signal quality, noise, jitter at different frequencies]

### Speed Calculation Accuracy
[Analysis of accuracy performance, consistency across speed range]

### Timer and Hardware Performance
[Notes on timer behavior, interrupt handling, CPU usage]

### Environmental Factors
[Temperature, noise, other environmental effects noted]

### Recommendations for Next Iteration
[Suggestions for improvements or calibration adjustments]

### Follow-up Actions
- [ ] Action 1: [Description] - Owner: _______ - Due: _______
- [ ] Action 2: [Description] - Owner: _______ - Due: _______

---

## Sign-Off

### Test Status
**Final Status:** [ ] PASS [ ] FAIL [ ] CONDITIONAL PASS

### Review & Approval
- **Tested By:** [Tester Name] - Signature/Date: _______________
- **Reviewed By:** [Reviewer Name] - Signature/Date: _______________
- **Approved By:** [Team Lead Name] - Signature/Date: _______________

### Date Approved
**Date:** [YYYY-MM-DD]

### Archive Reference
**Location in Repository:** `/tests/integration/executions/phase_d/`
**Committed:** [ ] Yes [ ] No
**Commit Hash:** _______________

---

**Template Version:** 1.0 | **Last Updated:** 2026-02-23 | **Created by:** Team04_DrivaPi Integration Test Team
