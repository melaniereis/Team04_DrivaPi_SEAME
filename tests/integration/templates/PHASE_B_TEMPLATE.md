# Integration Test Execution Template - Phase B
## Raspberry Pi ↔ STM32 CAN Bidirectional Communication

---

## Header / Metadata
- **Date:** [YYYY-MM-DD]
- **Time Started:** [HH:MM]
- **Tester Name:** [Full Name]
- **Test Phase:** C
- **Test Cycle:** [Sprint X]

---

## Test Information
- **Phase Name:** CAN Bidirectional Communicatio
- **Test Objective:** Validate CAN communication between Raspberry Pi and STM32 using the measure round-trip latency code
- **Related Requirements:**
  - Integration Test Plan - Phase B
  - `/docs/tests/integration-test-plan.md`
  - CAN communication specification

---

## Environment Setup

### Hardware Required
- [ ] Raspberry Pi 5 with CAN controller/transceiver hat
- [ ] STM32U585 microcontroller
- [ ] CAN transceiver on stm32 board
- [ ] CAN termination resistors (120Ω) at both ends
- [ ] Logic analyzer or oscilloscope (for signal capture)
- [ ] USB cables for power and logging

### Software Required
- [ ] CAN driver configured on Raspberry Pi (version: __________)
- [ ] STM32 firmware loaded (version: __________)
- [ ] Raspberry Pi c++ code 

### Configuration Settings
- **CAN Bus:** [CAN1]
- **Baud Rate:** [1Mbps]
- **Py Rx ID (STM32 → Rpi):** [0x_______]
- **Tx ID (Rpi → STM32):** [0x_______]
- **Latency Sampling Rate:** [Hz]
- **Test Duration:** [seconds]

### Prerequisites Checklist
- [ ] CAN transceiver powered and operational
- [ ] Termination resistors properly installed
- [ ] CAN bus continuity verified with multimeter
- [ ] CAN driver loaded on Raspberry Pi
- [ ] STM32 firmware programmed and running
- [ ] CANallyzer connected and configured
- [ ] Reference clock synchronized for latency measurement
- [ ] Network isolated from production traffic

---

## Pre-Test Checklist
- [ ] CAN bus powered (3.3V or 5V, as applicable)
- [ ] All CAN connectors seated properly
- [ ] No visible damage to CAN cables or connectors
- [ ] Logic analyzer connected to CAN bus (if used)
- [ ] Monitoring software started and recording enabled
- [ ] Both Raspberry Pi and STM32 powered and responsive
- [ ] CAN interface brought up: `ip link set can0 up`
- [ ] Test environment stable and isolated

---

## Execution Steps

### Step 1: Verify CAN Bus Physical Connectivity
**Procedure:**
1. Measure voltage between CAN_H and CAN_L on both ends
2. Verify CAN_H and CAN_L not shorted together
3. Check termination resistor resistance (should be ~120Ω)
4. Verify all connectors are seated properly

**Expected Output:**
```
CAN_H: ~2.5V at rest
CAN_L: ~2.5V at rest
Differential voltage: ~0V at idle
Termination: ~60Ω (two 120Ω in parallel)
```

**Actual Output:**
```
[Record measurements here]
```

**Measurements:**
- CAN_H voltage: _______ V
- CAN_L voltage: _______ V
- Differential voltage: _______ V
- Termination resistance: _______ Ω

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 2: Initialize CAN on Raspberry Pi
**Procedure:**
1. Load CAN driver: `sudo modprobe mcp251x`
2. Bring up CAN interface: `sudo ip link set can0 up type can bitrate 500000`
3. Verify interface is up: `ip link show can0`
4. Enable CAN reception and transmission

**Expected Output:**
```
can0: <NOARP,UP,LOWER_UP> mtu 16 qdisc mq state UP mode DEFAULT group 33
    link/can  promiscuity 0
    RX packets: 0  TX packets: 0
```

**Actual Output:**
```
[Record actual output here]
```

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 3: Verify STM32 CAN Configuration
**Procedure:**
1. Connect to STM32 via serial terminal or debug interface
2. Verify CAN peripheral initialized
3. Check baud rate matches Raspberry Pi
4. Verify message filters are configured correctly

**Expected Output:**
```
STM32 CAN Controller Initialized
Baud Rate: 500000 bps
RX FIFO: Ready
TX Ready
```

**Actual Output:**
```
[Record actual output here]
```

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 4: Single Message Transmission - Rpi → STM32
**Procedure:**
1. Send test message from Raspberry Pi to STM32
2. Use cansend tool: `cansend can0 123#AABBCCDDEE`
3. Capture on logic analyzer
4. Verify message received on STM32

**Expected Output:**
```
CAN message transmitted with ID 0x123
Payload: AA BB CC DD EE
STM32 received message successfully
No CAN errors reported
```

**Actual Output:**
```
[Record actual output here]
```

**CAN Message Details:**
- Message ID Transmitted: 0x_______
- Payload Transmitted: _______________
- Time to CAN Bus: _______ µs
- STM32 Confirmation: [Received/Not Received]

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 5: Single Message Transmission - STM32 → Rpi
**Procedure:**
1. Instruct STM32 to transmit test message to Raspberry Pi
2. Send command via serial: "SEND 456 0102030405"
3. Capture message on Raspberry Pi: `candump can0`
4. Verify message ID and payload

**Expected Output:**
```
can0  456   [5]  01 02 03 04 05
Checksum valid
CRC match
```

**Actual Output:**
```
[Record actual output here]
```

**CAN Message Details:**
- Message ID Received: 0x_______
- Payload Received: _______________
- Timestamp: _______
- Status: [Valid/Corrupted/Missed]

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 6: Latency Measurement - Rpi → STM32 → Rpi (RTT)
**Procedure:**
1. Configure STM32 to echo received messages back to Raspberry Pi
2. Send timestamped message from Raspberry Pi
3. Measure round-trip time (time sent → time received back)
4. Capture 100 measurements for statistical analysis
5. Enable high-resolution timing with synchronized clocks

**Latency Measurement Procedure:**
```bash
# Send from Raspberry Pi and measure response time
for i in {1..100}; do
  # Send message with sequence number
  # Record timestamp: T_send
  cansend can0 100#$(printf '%016X' $i)
  # Wait for response with echo sequence
  candump can0 timeout 100 > /tmp/can_log_$i.txt
  # Record timestamp: T_recv
  # Calculate: RTT = T_recv - T_send
done
```

**Latency Measurements:**

| Sample # | T_Send (µs) | T_Recv (µs) | RTT (ms) | Status |
|----------|------------|------------|---------|--------|
| 1 | ___ | ___ | ___ | PASS/FAIL |
| 2 | ___ | ___ | ___ | PASS/FAIL |
| 3 | ___ | ___ | ___ | PASS/FAIL |
| ... | ... | ... | ... | ... |
| 100 | ___ | ___ | ___ | PASS/FAIL |

**Latency Statistics:**
- Min RTT: _______ ms
- Max RTT: _______ ms
- Average RTT: _______ ms
- Median RTT: _______ ms
- Std Dev: _______ ms
- 95th Percentile: _______ ms
- 99th Percentile: _______ ms

**Pass Criteria:** RTT < 10ms ✓ (Actual: _______ms)

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 7: Verify Message ID Integrity (Extended ID Support)
**Procedure:**
1. Test sending messages with standard IDs (11-bit)
2. Test sending messages with extended IDs (29-bit) if supported
3. Verify no message ID collisions
4. Capture ID field in logic analyzer

**Message ID Tests:**

| Test | ID Type | Sent ID | Received ID | Match | Status |
|------|---------|---------|-------------|-------|--------|
| Standard 1 | 11-bit | 0x123 | 0x___ | YES/NO | PASS/FAIL |
| Standard 2 | 11-bit | 0x456 | 0x___ | YES/NO | PASS/FAIL |
| Extended 1 | 29-bit | 0x12345678 | 0x___ | YES/NO | PASS/FAIL |
| Extended 2 | 29-bit | 0x9ABCDEF0 | 0x___ | YES/NO | PASS/FAIL |

**Message ID Mismatches:** _______ (Target: 0)

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 8: Payload Integrity Verification
**Procedure:**
1. Send messages with various payload patterns
2. Compare transmitted vs. received payloads byte-by-byte
3. Verify CRC/checksum if applicable
4. Test with max payload size (8 bytes for CAN 2.0)

**Payload Test Patterns:**

| Pattern | Payload Sent | Payload Received | CRC Match | Status |
|---------|-------------|-----------------|-----------|--------|
| All zeros | 00 00 00 00 00 00 00 00 | ___ | YES/NO | PASS/FAIL |
| All ones | FF FF FF FF FF FF FF FF | ___ | YES/NO | PASS/FAIL |
| Alternating | AA 55 AA 55 AA 55 AA 55 | ___ | YES/NO | PASS/FAIL |
| Sequential | 00 01 02 03 04 05 06 07 | ___ | YES/NO | PASS/FAIL |
| Random 1 | ___ | ___ | YES/NO | PASS/FAIL |
| Random 2 | ___ | ___ | YES/NO | PASS/FAIL |

**Total Payloads Transmitted:** _______
**Total Payloads Matched:** _______
**Payload Corruption Rate:** _______ % (Target: 0%)

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 9: High-Frequency Message Stream Test
**Procedure:**
1. Configure continuous message transmission at 100 msg/sec
2. Send stream for 60 seconds (6000 messages)
3. Monitor for errors, timeouts, or lost messages
4. Verify message sequence numbers

**Expected Output:**
```
Sending 100 messages/second for 60 seconds
Total messages: 6000
Errors: 0
Timeouts: 0
Sequence check: OK
```

**Actual Output:**
```
[Record output here]
```

**Measurements:**
- Total messages sent: _______
- Total messages received: _______
- Lost messages: _______ (Target: 0)
- Duplicate messages: _______
- Out-of-sequence messages: _______
- Bus load: _______ %

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 10: CAN Error Conditions & Recovery
**Procedure:**
1. Deliberately inject CAN errors (if testing framework allows)
2. Disconnect/reconnect CAN cable
3. Verify error detection on both ends
4. Confirm automatic recovery without reset

**Error Tests:**

| Error Type | Detection | Recovery | Time to Recovery | Status |
|-----------|-----------|----------|------------------|--------|
| Bus Off | [ ] Yes | [ ] Yes | _______ sec | PASS/FAIL |
| Frame Error | [ ] Yes | [ ] Yes | _______ sec | PASS/FAIL |
| Cable Disconnect | [ ] Yes | [ ] Yes | _______ sec | PASS/FAIL |
| CRC Error | [ ] Yes | [ ] Yes | _______ sec | PASS/FAIL |

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

## Results Recording

### Overall Status
- [ ] **PASS** - All acceptance criteria met
- [ ] **FAIL** - One or more acceptance criteria not met
- [ ] **CONDITIONAL PASS** - Minor issues requiring documentation

### Pass/Fail Criteria Met
- [ ] CAN bus physical connectivity verified
- [ ] Bidirectional communication working (Rpi ↔ STM32)
- [ ] Message IDs correct and no collisions detected
- [ ] Payload integrity verified (100% match, 0 corruption)
- [ ] Round-trip latency < 10ms
- [ ] No message loss during high-frequency test
- [ ] Error detection and recovery working

### Key Measurements Summary
| Metric | Value | Threshold | Status |
|--------|-------|-----------|--------|
| Average RTT | ___ ms | < 10 ms | PASS/FAIL |
| Max RTT | ___ ms | < 15 ms | PASS/FAIL |
| Message ID Mismatches | ___ | 0 | PASS/FAIL |
| Payload Corruption | ___% | 0% | PASS/FAIL |
| Message Loss | ___ | 0 | PASS/FAIL |
| Bus Error Rate | ___% | < 0.1% | PASS/FAIL |

### Issues Encountered
- [ ] No issues
- [ ] Issues found (describe below):

**Issue #1:** [Description]
- Severity: [Critical/High/Medium/Low]
- Impact: [Impact on system]
- Workaround: [If applicable]

**Issue #2:** [Description]
- Severity: [Critical/High/Medium/Low]
- Impact: [Impact on system]
- Workaround: [If applicable]

### Performance Metrics Summary
- RTT Mean: _______ ms
- RTT Median: _______ ms
- RTT Std Dev: _______ ms
- Message throughput: _______ msg/sec
- CAN bus utilization: _______ %
- Error rate during extended test: _______ %

---

## Evidence Artifacts

### CAN Logs
- [ ] CAN trace file (Rpi → STM32): `[filename]`
  - Location: _______________
  - Duration: _______ sec
  - Message count: _______
  
- [ ] CAN trace file (STM32 → Rpi): `[filename]`
  - Location: _______________
  - Duration: _______ sec
  - Message count: _______

### Latency Measurements
- [ ] Latency log file: `[filename]`
  - Samples: _______
  - Location: _______________
  - Format: [CSV, JSON, other]

### Logic Analyzer Captures
- [ ] CAN signal capture: `[filename]`
  - Duration: _______ sec
  - Resolution: _______ MHz
  - Location: _______________

### Screenshots
- [ ] Latency graph: `[filename]`
- [ ] Message ID verification: `[filename]`
- [ ] Error statistics: `[filename]`

### Logs
- [ ] Raspberry Pi CAN log: `[filename]`
  - Location: _______________
- [ ] STM32 debug log: `[filename]`
  - Location: _______________

---

## Notes / Observations

### General Observations
[Record general notes about test execution and environment]

### CAN Bus Quality
[Notes on signal quality, noise, interference observed]

### Latency Observations
[Analysis of latency pattern - consistent, variable, spikes?]

### Recommendations for Next Iteration
[Suggestions for improvements]

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
**Location in Repository:** `/tests/integration/executions/phase_b/`
**Committed:** [ ] Yes [ ] No
**Commit Hash:** _______________

---

**Template Version:** 1.0 | **Last Updated:** 2026-02-23 | **Created by:** Team04_DrivaPi Integration Test Team
