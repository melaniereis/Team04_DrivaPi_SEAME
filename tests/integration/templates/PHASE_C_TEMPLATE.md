# Integration Test Execution Template - Phase C
## STM32 → Motor Controllers I2C Communication

---

## Header / Metadata
- **Date:** [YYYY-MM-DD]
- **Time Started:** [HH:MM]
- **Tester Name:** [Full Name]
- **Test Phase:** C
- **Test Cycle:** [Sprint X]

---

## Test Information
- **Phase Name:** I2C Motor Controller Communication & Addressing Verification
- **Test Objective:** Validate I2C commands sent from STM32 to the PiRacer expansion board's motor controllers via I2C interface, verify correct addressing (0x60/0x40), and confirm ACK responses
- **Related Requirements:**
  - Integration Test Plan - Phase C
  - `/docs/tests/integration-test-plan.md`
  - I2C motor controller datasheet and addressing specification

---

## Environment Setup

### Hardware Required
- [ ] STM32U585 microcontroller with I2C interface
- [ ] PiRacer expansion board (single board with integrated dual motor controllers and I2C interface)
  - Motor Controller #1 (Address: 0x60)
  - Motor Controller #2 (Address: 0x40)
- [ ] USB cables for STM32
- [ ] Power cable for expansion board
### Software Required
- [ ] STM32 firmware with I2C driver

### Configuration Settings
- **I2C Bus:** [I2C3]
- **Clock Speed:** [180] kHz
- **Expansion Board:** PiRacer Kit
- **Motor Controller 1 Address:** 0x60
- **Motor Controller 2 Address:** 0x40
- **SDA/SCL Voltage:** [3.3V]

### Prerequisites Checklist
- [ ] Connection diagram verified and matched
- [ ] STM32 I2C peripheral configured
- [ ] All devices on same I2C bus
- [ ] No address conflicts between devices

---

## Pre-Test Checklist
- [ ] I2C bus voltage: _______ V (expected 3.3V)
- [ ] SDA/SCL not shorted or stuck
- [ ] STM32 powered and firmware loaded
- [ ] Motor controllers powered
- [ ] Test environment stable and isolated

---

## Execution Steps

### Step 1: Verify I2C Bus Physical Characteristics
**Procedure:**
1. Measure voltage on SDA and SCL lines
2. Verify lines are pulled HIGH at rest
4. Check for stuck-low or stuck-high conditions

**Expected Output:**
```
SDA voltage at rest: 3.3V
SCL voltage at rest: 3.3V
No stuck conditions detected
```

**Measurements:**
- SDA idle voltage: _______ V
- SCL idle voltage: _______ V

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 2: Verify Motor Controller Addressing (0x60, 0x40)
**Procedure:**
1. Ensure both motor controllers are connected and powered
2. Configure STM32 to scan I2C bus
3. Execute I2C bus scan: address range 0x00-0x7F
4. Verify motor controllers at 0x60 and 0x40 respond with ACK
5. Log address verification

**Expected Output:**
```
I2C Bus Scan Results:
  0x60 - Motor Controller 1: PRESENT (ACK)
  0x40 - Motor Controller 2: PRESENT (ACK)
Scan complete
```

**Actual Output:**
```
[Record scan results here]
```

**Devices Found:**
- Address 0x60: [ ] PRESENT [ ] NOT FOUND - Status: _______
- Address 0x40: [ ] PRESENT [ ] NOT FOUND - Status: _______
- Other addresses: _______

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 3: Single Write Transaction to Motor Controller 1 (0x60)
**Procedure:**
1. Prepare I2C write command to 0x60
2. Write control byte: 0x01 (example: enable motor)
3. Capture I2C transaction with logic analyzer
4. Verify ACK bit received from motor controller

**Logic Analyzer Capture - Expected Sequence:**
```
START condition
Address: 0x60 (write)
ACK received
Data byte: 0x01
ACK received
STOP condition
```

**Actual Sequence:**
```
[Record actual I2C transaction capture here]
```

**Transaction Details:**
- START condition: [ ] Detected [ ] Not detected
- Address byte: 0x_______
- Address ACK: [ ] Received [ ] Not received
- Data byte transmitted: 0x_______
- Data ACK: [ ] Received [ ] Not received
- STOP condition: [ ] Detected [ ] Not detected

**Timing Measurements:**
- Clock frequency detected: _______ kHz
- Setup time: _______ ns
- Hold time: _______ ns
- Rise time: _______ µs

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 4: Single Write Transaction to Motor Controller 2 (0x40)
**Procedure:**
1. Prepare I2C write command to 0x40
2. Write control byte: 0x02 (example: set direction)
3. Capture I2C transaction with logic analyzer
4. Verify ACK bit received from motor controller

**Logic Analyzer Capture - Expected Sequence:**
```
START condition
Address: 0x40 (write)
ACK received
Data byte: 0x02
ACK received
STOP condition
```

**Actual Sequence:**
```
[Record actual I2C transaction capture here]
```

**Transaction Details:**
- START condition: [ ] Detected [ ] Not detected
- Address byte: 0x_______
- Address ACK: [ ] Received [ ] Not received
- Data byte transmitted: 0x_______
- Data ACK: [ ] Received [ ] Not received
- STOP condition: [ ] Detected [ ] Not detected

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 5: Read Transaction from Motor Controller 1 (0x60)
**Procedure:**
1. Prepare I2C read command from 0x60
2. Request status byte (usually register 0x00)
3. Capture I2C transaction with logic analyzer
4. Verify motor controller transmits data and STM32 sends ACK

**Logic Analyzer Capture - Expected Sequence:**
```
START condition
Address: 0x60 (read)
ACK received from motor controller
Data byte: 0x## (status)
NACK sent by STM32
STOP condition
```

**Actual Sequence:**
```
[Record actual I2C transaction capture here]
```

**Transaction Details:**
- START condition: [ ] Detected [ ] Not detected
- Address byte: 0x_______
- Address ACK: [ ] Received from controller [ ] Not received
- Data byte received: 0x_______
- STM32 response: [ ] ACK [ ] NACK
- STOP condition: [ ] Detected [ ] Not detected

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 6: Multi-Byte Write Transaction (Motor Controller Sequence)
**Procedure:**
1. Send multi-byte write sequence to 0x60
2. Example: Write PWM value as 2 bytes (0x12 0x34)
3. Capture all I2C frames
4. Verify each byte generates ACK

**Expected Sequence (3-byte write):**
```
START
Address: 0x60 (write) - ACK
Data byte 1: 0x12 - ACK
Data byte 2: 0x34 - ACK
STOP
```

**Actual Sequence:**
```
[Record actual transaction here]
```

**Multi-Byte Details:**
| Byte # | Value | ACK Received | Status |
|--------|-------|-------------|--------|
| Address | 0x60 | [ ] Yes [ ] No | PASS/FAIL |
| 1 | 0x__ | [ ] Yes [ ] No | PASS/FAIL |
| 2 | 0x__ | [ ] Yes [ ] No | PASS/FAIL |
| 3 | 0x__ | [ ] Yes [ ] No | PASS/FAIL |

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 7: Simultaneous Access - Address Verification
**Procedure:**
1. Send commands to both controllers rapidly
2. Verify no address collisions
3. Confirm both 0x60 and 0x40 receive their respective data
4. Verify arbitration handling (if occurring)

**Test Sequence:**
```
Command 1: Write to 0x60 (e.g., speed control)
Command 2: Write to 0x40 (e.g., steering control)
Command 3: Read from 0x60
Command 4: Read from 0x40
```

**Results:**

| Command | Dest Address | Data | ACK Status | Status |
|---------|-------------|------|-----------|--------|
| 1 | 0x60 | ___ | [ ] ACK [ ] NACK | PASS/FAIL |
| 2 | 0x40 | ___ | [ ] ACK [ ] NACK | PASS/FAIL |
| 3 | 0x60 | ___ | [ ] ACK [ ] NACK | PASS/FAIL |
| 4 | 0x40 | ___ | [ ] ACK [ ] NACK | PASS/FAIL |

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 8: ACK/NACK Protocol Verification
**Procedure:**
1. Send write command to valid address (0x60): expect ACK
2. Send write command to invalid address (0x61): expect NACK
3. Verify STM32 handles NACK correctly
4. Verify no repeated START or retry by default

**ACK/NACK Test Results:**

| Test | Address | Expected | Actual | Status |
|------|---------|----------|--------|--------|
| Valid address write | 0x60 | ACK | [ ] ACK [ ] NACK | PASS/FAIL |
| Invalid address write | 0x61 | NACK | [ ] ACK [ ] NACK | PASS/FAIL |
| Valid address read | 0x60 | ACK | [ ] ACK [ ] NACK | PASS/FAIL |
| Invalid address read | 0x7F | NACK | [ ] ACK [ ] NACK | PASS/FAIL |

**STM32 Error Handling:**
- NACK detected? [ ] Yes [ ] No
- Error flag set? [ ] Yes [ ] No
- Timeout triggered? [ ] Yes [ ] No

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 9: High-Frequency Transaction Stress Test
**Procedure:**
1. Send I2C transactions at high frequency (100+ Hz)
2. Alternate between 0x60 and 0x40
3. Vary transaction size (1-byte to 8-byte)
4. Monitor for errors, timeouts, or data corruption
5. Capture for 30 seconds minimum

**Test Configuration:**
- Transaction rate: _______ Hz
- Total transactions: _______
- Test duration: _______ sec

**Expected Result:**
```
All transactions completed successfully
No timeouts or errors detected
ACK rate: 100%
Data integrity: 100%
```

**Actual Result:**
```
[Record measurements here]
```

**Stress Test Results:**

| Metric | Value | Threshold | Status |
|--------|-------|-----------|--------|
| Successful transactions | ___ | 100% | PASS/FAIL |
| Failed transactions | ___ | 0 | PASS/FAIL |
| Timeouts | ___ | 0 | PASS/FAIL |
| Data errors | ___ | 0 | PASS/FAIL |
| ACK rate | ___% | 100% | PASS/FAIL |

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

## Results Recording

### Overall Status
- [ ] **PASS** - All acceptance criteria met
- [ ] **FAIL** - One or more acceptance criteria not met
- [ ] **CONDITIONAL PASS** - Minor issues requiring documentation

### Pass/Fail Criteria Met
- [ ] Motor controller 1 (0x60) detected and responding with ACK
- [ ] Motor controller 2 (0x40) detected and responding with ACK
- [ ] Write transactions to both controllers successful (100% ACK)
- [ ] Read transactions from both controllers successful
- [ ] Multi-byte transactions handled correctly
- [ ] ACK/NACK protocol working as expected
- [ ] No address collisions during simultaneous access
- [ ] High-frequency stress test passed (100% success, 0% errors)

### Key Measurements Summary
| Metric | Value | Threshold | Status |
|--------|-------|-----------|--------|
| Controller 0x60 Detection | [Found/Not Found] | Found | PASS/FAIL |
| Controller 0x40 Detection | [Found/Not Found] | Found | PASS/FAIL |
| ACK Rate | ___% | 100% | PASS/FAIL |
| NACK Errors | ___ | 0 | PASS/FAIL |
| Addressing Errors | ___ | 0 | PASS/FAIL |
| Transaction Success Rate | ___% | 100% | PASS/FAIL |

### Issues Encountered
- [ ] No issues
- [ ] Issues found (describe below):

**Issue #1:** [Description]
- Severity: [Critical/High/Medium/Low]
- Devices affected: [0x60/0x40/Both]
- Root cause analysis: [If known]

**Issue #2:** [Description]
- Severity: [Critical/High/Medium/Low]
- Devices affected: [0x60/0x40/Both]
- Root cause analysis: [If known]

### Performance Metrics Summary
- I2C clock frequency observed: _______ kHz
- Average transaction duration: _______ ms
- Minimum transaction time: _______ ms
- Maximum transaction time: _______ ms
- Bus errors during stress test: _______

---

## Evidence Artifacts

### Logic Analyzer Captures
- [ ] I2C transaction capture (0x60 writes): `[filename]`
  - Duration: _______ sec
  - Transactions captured: _______
  - Location: _______________
  
- [ ] I2C transaction capture (0x40 writes): `[filename]`
  - Duration: _______ sec
  - Transactions captured: _______
  - Location: _______________

- [ ] Read transactions capture: `[filename]`
  - Duration: _______ sec
  - Location: _______________

### Decoded Traces
- [ ] I2C protocol decode file: `[filename]`
  - Format: [CSV, JSON, Saleae format, other]
  - Transactions decoded: _______
  - Location: _______________

### Analysis Files
- [ ] Address verification report: `[filename]`
  - Location: _______________
  
- [ ] ACK/NACK analysis: `[filename]`
  - Location: _______________

### Screenshots
- [ ] Logic analyzer waveform (0x60 write): `[filename]`
- [ ] Logic analyzer waveform (0x40 write): `[filename]`
- [ ] Bus scan result: `[screenshot]`

### Logs
- [ ] STM32 I2C log: `[filename]`
  - Location: _______________
- [ ] Motor controller response log: `[filename]`
  - Location: _______________

---

## Notes / Observations

### I2C Bus Quality
[Observations on signal integrity, timing, quality]

### Motor Controller Behavior
[Notes on how motor controllers responded to commands]

### Addressing Observations
[Specific observations about addressing, collisions, or quirks]

### Parameter Accuracy
[Did actual timing, voltages, frequencies match specifications?]

### Recommendations for Next Iteration
[Suggestions for improvements or additional testing]

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
**Location in Repository:** `/tests/integration/executions/phase_c/`
**Committed:** [ ] Yes [ ] No
**Commit Hash:** _______________

---

**Template Version:** 1.0 | **Last Updated:** 2026-02-23 | **Created by:** Team04_DrivaPi Integration Test Team
