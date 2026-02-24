# Integration Test Execution Template - Phase A
## PS3 Controller → Raspberry Pi

---

## Header / Metadata
- **Date:** [YYYY-MM-DD]
- **Time Started:** [HH:MM UTC]
- **Tester Name:** [Full Name]
- **Test Phase:** A
- **Test Cycle:** [Sprint X, Iteration Y]
- **Template Version:** 1.0

---

## Test Information
- **Phase Name:** PS3 Controller Input Validation & Command Decoding
- **Test Objective:** Validate PS3 controller input is correctly decoded and CAN payload is generated with correct DBC mapping
- **Related Requirements:** 
  - Integration Test Plan - Phase A
  - `/docs/tests/integration-test-plan.md`
  - Controller application specification

---

## Environment Setup

### Hardware Required
- [ ] PS3 Controller (fully charged)
- [ ] Raspberry Pi 4B connected to network
- [ ] USB cable for PS3 controller connection
- [ ] Network connectivity (SSH or direct connection)
- [ ] Monitoring station for CAN traffic capture

### Software Required
- [ ] Controller application (version: __________)
- [ ] CANallyzer or equivalent CAN bus monitoring tool
- [ ] SSH client (if testing remotely)
- [ ] Logging enabled in controller app

### Configuration Settings
- **Controller App Port:** __________ 
- **CAN Bus:** [CAN0/CAN1]
- **Baud Rate:** [1Mbps]
- **DBC File:** [Filename]
- **Log Level:** DEBUG

### Prerequisites Checklist
- [ ] PS3 controller paired with Raspberry Pi
- [ ] Network connectivity verified
- [ ] CAN bus monitoring tool connected and running
- [ ] Controller application installed and ready
- [ ] Log directory exists and is writable
- [ ] Previous test logs backed up

---

## Pre-Test Checklist
- [ ] PS3 controller fully charged (>80%)
- [ ] Raspberry Pi powered on and responsive
- [ ] CAN monitoring tool running and capturing
- [ ] Controller application deployed and ready to start
- [ ] Test environment isolated from other network traffic
- [ ] SSH connection tested (if remote)
- [ ] Logging directory verified as empty

---

## Execution Steps

### Step 1: Start Controller Application
**Procedure:**
1. Connect to Raspberry Pi via SSH
2. Navigate to controller application directory
3. Start the controller application with debug logging enabled
4. Wait for initialization complete message (typically 2-3 seconds)

**Expected Output:**
```
Controller Application Started
Listening for PS3 controller input on [port]
CAN interface initialized
Ready for input
```

**Actual Output:**
```
[Record actual output here]
```

**Notes/Issues:** [If any]

---

### Step 2: Connect PS3 Controller
**Procedure:**
1. Hold PS button on PS3 controller for 3-5 seconds
2. Observe connection confirmation on Raspberry Pi
3. Verify controller connection status in application logs

**Expected Output:**
```
PS3 Controller Connected
Controller ID: [Device ID]
Battery Level: [%]
```

**Actual Output:**
```
[Record actual output here]
```

**Notes/Issues:** [If any]

---

### Step 3: Test Analog Stick Input
**Procedure:**
1. Move left analog stick to full left position
2. Record decoded value in application
3. Verify CAN payload generated with correct values
4. Repeat for: right, up, down, diagonals

**Expected Output:**
- Left stick: X=0x00, Y=0x80
- Each direction shows correct analog value decoding
- CAN messages generated for each movement

**Actual Output:**
```
[Record actual output here]
```

**CAN Messages Captured:**
```
[List CAN message IDs and payloads]
```

**Notes/Issues:** [If any]

**Measurements:**
- Response Time (button press → CAN message): _______ ms
- Number of CAN messages generated: _______

---

### Step 4: Test Button Inputs
**Procedure:**
1. Press each button sequentially: Cross, Circle, Triangle, Square
2. Record decoded button press in application logs
3. Verify CAN payload generated for each button
4. Test L1, L2, R1, R2 shoulder buttons

**Button Test Results:**

| Button | Decoded Value | CAN ID | CAN Payload | Status |
|--------|---------------|--------|-------------|--------|
| Cross (X) | ___ | ___ | ___ | PASS/FAIL |
| Circle (O) | ___ | ___ | ___ | PASS/FAIL |
| Triangle (Δ) | ___ | ___ | ___ | PASS/FAIL |
| Square (□) | ___ | ___ | ___ | PASS/FAIL |
| L1 | ___ | ___ | ___ | PASS/FAIL |
| L2 | ___ | ___ | ___ | PASS/FAIL |
| R1 | ___ | ___ | ___ | PASS/FAIL |
| R2 | ___ | ___ | ___ | PASS/FAIL |

**Measurements:**
- Max button to CAN encoding latency: _______ ms
- Decoding accuracy: _______ %

---

### Step 5: Test D-Pad Input
**Procedure:**
1. Press each D-Pad direction: Up, Down, Left, Right
2. Record decoded direction in application logs
3. Verify CAN payload generated
4. Test diagonal presses (Up+Left, Up+Right, Down+Left, Down+Right)

**D-Pad Test Results:**

| Direction | Decoded Value | CAN ID | CAN Payload | Status |
|-----------|---------------|--------|-------------|--------|
| Up | ___ | ___ | ___ | PASS/FAIL |
| Down | ___ | ___ | ___ | PASS/FAIL |
| Left | ___ | ___ | ___ | PASS/FAIL |
| Right | ___ | ___ | ___ | PASS/FAIL |
| Diagonals | ___ | ___ | ___ | PASS/FAIL |

**Notes/Issues:** [If any]

---

### Step 6: Verify DBC Mapping Accuracy
**Procedure:**
1. Enable DBC translation in CAN monitoring tool
2. Verify all CAN messages match DBC file definitions
3. Check that signal names are correctly mapped
4. Validate payload byte ordering (big-endian/little-endian)

**DBC Verification Results:**

| Signal Name | Expected DBC | Actual | Match | Status |
|-------------|-------------|--------|-------|--------|
| Input_Button | [DBC def] | [Actual] | YES/NO | PASS/FAIL |
| Analog_Left | [DBC def] | [Actual] | YES/NO | PASS/FAIL |
| Analog_Right | [DBC def] | [Actual] | YES/NO | PASS/FAIL |
| [Other signals] | [DBC def] | [Actual] | YES/NO | PASS/FAIL |

---

### Step 7: Stress Test - Rapid Input
**Procedure:**
1. Perform rapid button presses (10 presses/second for 10 seconds)
2. Monitor for dropped inputs or errors
3. Check for message loss or corruption in CAN logging
4. Verify no buffer overflow or crash occurs

**Expected Output:**
- All inputs captured without loss
- No errors reported in controller logs
- CAN message count matches input count

**Actual Output:**
```
[Record measurements here]
```

**Measurements:**
- Total inputs sent: _______
- Total CAN messages generated: _______
- Missed inputs: _______
- Error count: _______

**Notes/Issues:** [If any]

---

### Step 8: Stop Test & Verify Clean Shutdown
**Procedure:**
1. Disconnect PS3 controller
2. Stop controller application gracefully
3. Verify no errors in shutdown logs
4. Save all logs to evidence directory

**Expected Output:**
```
PS3 Controller Disconnected
Controller Application Shutting Down
All resources released
Save complete
```

**Actual Output:**
```
[Record actual output here]
```

**Notes/Issues:** [If any]

---

## Results Recording

### Overall Status
- [ ] **PASS** - All acceptance criteria met
- [ ] **FAIL** - One or more acceptance criteria not met
- [ ] **CONDITIONAL PASS** - Minor issues requiring documentation

### Pass/Fail Criteria Met
- [ ] 100% command decoding accuracy achieved
- [ ] All button inputs correctly decoded
- [ ] All analog values correctly mapped
- [ ] Correct DBC message IDs and payloads generated
- [ ] No message loss during stress test
- [ ] Application started and stopped cleanly

### Key Measurements Summary
| Metric | Value | Threshold | Status |
|--------|-------|-----------|--------|
| Decoding Accuracy | ___% | 100% | PASS/FAIL |
| Button Response Latency | ___ ms | < 50 ms | PASS/FAIL |
| Analog Latency | ___ ms | < 50 ms | PASS/FAIL |
| Message Loss Count | ___ | 0 | PASS/FAIL |
| Stress Test Inputs Captured | ___% | 100% | PASS/FAIL |

### Issues Encountered
- [ ] No issues
- [ ] Issues found (describe below):

**Issue #1:** [Description]
- Severity: [Critical/High/Medium/Low]
- Workaround: [If applicable]
- Follow-up Required: [Yes/No]

**Issue #2:** [Description]
- Severity: [Critical/High/Medium/Low]  
- Workaround: [If applicable]
- Follow-up Required: [Yes/No]

### Performance Metrics Summary
- Average button decode latency: _______ ms
- Average analog decode latency: _______ ms
- CAN message generation rate: _______ msg/sec
- Peak CPU usage during test: _______ %
- Peak memory usage during test: _______ MB

---

## Evidence Artifacts

### Logs Captured
- [ ] Controller application log: `[filename]`
  - Location: _______________
  - Size: _______ KB
  
- [ ] System log: `[filename]`
  - Location: _______________
  - Size: _______ KB

### Data Captures
- [ ] CAN bus trace file: `[filename]`
  - Format: [.csv, .dbc, .log, other]
  - Duration: _______ seconds
  - Message count: _______
  - Location: _______________

### Screenshots
- [ ] Controller app interface: `[filename]`
- [ ] CAN analyzer tool: `[filename]`
- [ ] Error messages (if any): `[filename]`

### Video Record
- [ ] Test execution video: `[filename]`
  - Duration: _______ seconds
  - Location: _______________
  - Notes: _______________

### Physical Observations
- [ ] PS3 Controller condition: [Good/Acceptable/Issues]
  - Notes: _______________
- [ ] Raspberry Pi behavior: [Normal/Concerning]
  - Notes: _______________

---

## Notes / Observations

### General Observations
[Record general notes about test execution, environment, or interesting findings]

### Anomalies Found
[Document any unexpected behaviors or discrepancies]

### Recommendations for Next Iteration
[Suggestions for improvement or follow-up testing]

### Follow-up Actions
- [ ] Action 1: [Description] - Owner: _______ - Due: _______
- [ ] Action 2: [Description] - Owner: _______ - Due: _______

### Environment Notes
- Network conditions: _______________________
- Background processes: _______________________
- Any interference or disruptions: _______________________

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
**Location in Repository:** `/tests/integration/executions/phase_a/`
**Committed:** [ ] Yes [ ] No
**Commit Hash:** _______________

---

**Template Version:** 1.0 | **Last Updated:** 2026-02-23 | **Created by:** Team04_DrivaPi Integration Test Team
