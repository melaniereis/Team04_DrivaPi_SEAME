# DrivaPi Integration Test Plan

## Main Goal

Verify that all DrivaPi components work together correctly across their interface boundaries, ensuring reliable communication, correct data flow, proper timing, and safe error handling throughout the entire system chain — from PS3 controller input to motor actuation and sensor feedback.

---

## Integration Test Phases

### Phase A: PS3 Controller → Raspberry Pi

**Objective**: Verify that PS3 controller (Shanwan) commands are correctly received, decoded and validated by the Raspberry Pi.

**Test Setup**:
- Connect Shanwan PS3 controller via 2.4GHz USB dongle to Raspberry Pi
- Inject commands using the physical controller
- Monitor command reception and decoding on the Raspberry Pi

**Assertions**:
- ✅ Correct command decoded (speed, steering, mode values match input)
- ✅ Invalid signals are rejected or handled safely
- ✅ Dropped signals trigger safe fallback behavior

**Tools & Methods**:
- Shanwan PS3 controller hardware
- Software simulation for repeatable testing (mock controller input once hardware is validated)

**Pass Criteria**:
- All valid commands decoded with 100% accuracy
- Invalid/malformed commands rejected without system crash
- Signal loss triggers immediate failsafe mode

**Evidence**:
- Runtime logs from [rust/controller/src/main.rs](rust/controller/src/main.rs) showing decoded PS3 input (e.g., `println!("Button test: {:?}", data);`) alongside the CAN payloads sent.
- Video evidence: [tests/integration/joystick_integration.mp4](tests/integration/joystick_integration.mp4) demonstrating PS3 controller input successfully decoded and processed by the Raspberry Pi.

---

### Phase B: Raspberry Pi ↔ STM32 (CAN Bidirectional)

**Objective**: Validate bidirectional CAN bus communication between Raspberry Pi and STM32 microcontroller — the most critical integration point in the system (Raspberry Pi sends commands; STM32 sends feedback).

**Test Setup**:
- Use SocketCAN on the Raspberry Pi with the latency test tool `can_latency_receive_test.cpp` (docs/data-transfer/CAN/latency) to send frames and measure echoed responses.
- Run STM32 firmware `can_latency_send_test.c` (docs/data-transfer/CAN/latency) to receive frames and echo them back for RTT measurement.
- Capture STM32 UART logs that show received frames and echo timing.

**Test Cases**:
1. **CAN ID Validation**: Correct CAN identifiers used for each message type
2. **Payload Encoding**: Data bytes match expected format and values
3. **Timing**: Message latency and frequency meet requirements
4. **Error Handling**: Bus-off conditions, dropped frames, timeout behavior

**Assertions**:
- ✅ CAN messages use correct IDs
- ✅ Payload structure matches protocol specification
- ✅ Message frequency stays within defined limits (e.g., 50 Hz)
- ✅ Bus errors detected and handled without data corruption
- ✅ Timeout conditions trigger safe fallback on STM32

**Pass Criteria**:
- Zero message ID mismatches
- All payloads correctly encoded/decoded
- Latency < 10ms (or defined threshold)
- Graceful handling of all error scenarios

---

### Phase C: STM32 → Motor Controllers (I2C)

**Objective**: Verify that STM32 correctly sends I2C commands to motor controller boards and receives proper acknowledgments.

**Test Setup**:
- Inject CAN commands to STM32
- Monitor I2C bus traffic to motor controllers (logic analyzer or I2C sniffer)
- Verify I2C write operations and ACK/NACK responses
- Observe physical motor behavior

**Architecture**:
- Throttle motor controller: I2C address 0x60
- Steering motor controller: I2C address 0x40
- STM32 acts as I2C master, motor controllers as I2C slaves

**Test Cases**:
1. **Throttle I2C Communication**: CAN throttle command → I2C write to 0x60 → ACK received
2. **Steering I2C Communication**: CAN steering command → I2C write to 0x40 → ACK received
3. **I2C Error Handling**: NACK or bus timeout handled gracefully
4. **Safety Behavior**: Failsafe stops motors on command timeout or I2C failure

**Tools**:
- Logic analyzer or I2C sniffer (monitor bus traffic, verify addresses and payloads)
- UART logs from STM32 (show I2C transaction status)
- Oscilloscope (optional, to verify I2C signal integrity)

**Assertions**:
- ✅ I2C write to 0x60 (throttle) uses correct address and payload
- ✅ I2C write to 0x40 (steering) uses correct address and payload
- ✅ ACK received from both motor controllers
- ✅ NACK or I2C timeout triggers error handling
- ✅ Motors stop immediately on command timeout or I2C failure
- ✅ No unintended motor activation during startup

**Automation Tip**:
Log I2C transaction status (ACK/NACK) from STM32 firmware and parse logs programmatically for pass/fail validation.

**Pass Criteria**:
- 100% correct I2C addressing (0x60 for throttle, 0x40 for steering)
- All valid commands receive ACK from motor controllers
- I2C errors handled without system crash
- Failsafe triggers within 100ms of timeout or communication failure

**Evidence**:
- Test implementation files: [tests/integration/motors-integration/src/](tests/integration/motors-integration/src/) and [tests/integration/motors-integration/inc/](tests/integration/motors-integration/inc/) containing I2C integration test code (`i2c_integration_test.c/h`), PCA9685 driver (`pca9685.c/h`), and test main (`main.c/h`)
- Video evidence: [tests/integration/motors_integration.mp4](tests/integration/motors_integration.mp4) demonstrating successful I2C communication with motor controllers and proper motor actuation

---

### Phase D: Speed Sensor → STM32

**Objective**: Validate that speed sensor data is correctly read and processed by STM32.

**Test Setup**:
- Connect speed sensor to GPIO pin **PD9** (rising edge interrupt)
- Monitor UART output for pulse detection messages
- Count pulses and verify accuracy over time

**Hardware Configuration**:
- Speed sensor output → PD9 (EXTI9 interrupt, rising edge trigger)
- UART output for validation (115200 baud)
- Interrupt increments counter and prints message on each pulse

**Test Cases**:
1. **Pulse Detection**: Interrupt triggered on each sensor pulse
2. **Counter Accuracy**: Pulse count incremented correctly
3. **UART Output**: Message printed for each pulse received
4. **Zero Input Handling**: System stable with no pulses
5. **Sustained Operation**: Counter remains stable over extended testing

**Assertions**:
- ✅ Interrupt fires on rising edge of sensor signal
- ✅ Counter increments correctly for each pulse
- ✅ UART prints confirmation message per pulse
- ✅ No data corruption or missed pulses
- ✅ System remains stable under continuous operation

**Tools**:
- Signal generator or GPIO-based pulse simulation
- UART terminal monitor (115200 baud, 8N1)
- Oscilloscope (optional, to verify signal integrity)

**Pass Criteria**:
- Interrupt correctly triggered on PD9
- Counter increments match actual pulse count
- UART output message printed for each pulse
- No system crashes or hangs
- Stable operation over sustained pulse input

**Evidence**:
- Test implementation: [tests/integration/motors-and-speed-sensor-integration/src/stm32u5xx_it.c](tests/integration/motors-and-speed-sensor-integration/src/stm32u5xx_it.c) - `EXTI9_IRQHandler()` with interrupt service routine
- Main firmware: [tests/integration/speed-sensor-integration/main.c](tests/integration/speed-sensor-integration/main.c) - speed sensor interrupt configuration
- Video evidence: [tests/integration/speed_sensor_integration.mp4](tests/integration/speed_sensor_integration.mp4) demonstrating successful pulse detection on PD9 with UART output confirming each pulse received

---

## Evidence / Proof Artifacts

- **Bidirectional CAN**: test artifacts from [docs/data-transfer/CAN/bidirectionalCommunication.md](docs/data-transfer/CAN/bidirectionalCommunication.md) and the latency tools in [docs/data-transfer/CAN/latency](docs/data-transfer/CAN/latency) (`can_latency_receive_test.cpp` on Raspberry Pi, `can_latency_send_test.c` on STM32). Logs from these tests demonstrate Raspberry Pi → STM32 command frames and STM32 → Raspberry Pi echoes with correct IDs/payloads and measured RTT.
- **Controller → Raspberry Pi**: runtime logs from [rust/controller/src/main.rs](rust/controller/src/main.rs) printing decoded PS3 input (buttons/axes) alongside the CAN payloads sent; these logs correlate controller actions to outbound CAN frames.
- **STM32 reception** (optional but recommended): UART/SWO logs from STM32 showing received frames and echo behavior, matching the bidirectional/latency tests.

## Test Execution Guidelines

1. **Test in Order**: Start with Phase A and progress sequentially — each phase builds on the previous one
2. **Document Results**: Record pass/fail status, measurements, and any anomalies
3. **Automate Where Possible**: Use scripts for CAN injection, PWM measurement, and result validation
4. **Iterate on Failures**: Debug and retest failed phases before moving forward
5. **Safety First**: Always have emergency stop capability during motor testing

---

## Success Criteria

All integration phases pass their defined criteria, demonstrating:
- Reliable end-to-end communication
- Correct data transformation across interfaces
- Proper error handling and failsafe behavior
- System stability under normal and edge-case conditions
