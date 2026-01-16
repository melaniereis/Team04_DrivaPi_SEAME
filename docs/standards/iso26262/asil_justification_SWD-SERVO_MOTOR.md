# ASIL Justification: SWD-SERVO_MOTOR

## 1. Requirement Summary
- **Requirement ID:** SWD-SERVO_MOTOR
- **Component:** Servo Logic (`servo_motor.c`)
- **Safety Goal:** SG-03 (Safe Steering)

## 2. ASIL Decomposition
- **Assigned ASIL:** **A**
- **Rationale:** Failure of this component leads to Hazard H-03 (Unintended Steering). As derived in `hara_actuation.md`, this risk is classified as ASIL A.

## 3. Verification Strategy
To meet ASIL A for Steering Control:

1. **Unit Testing:**
   - Verify Angle `>180` is clamped to `180`.
   - Verify Pulse Width mapping remains within `[SERVO_MIN, SERVO_MAX]`.
   - **Target:** 100% Branch Coverage.

2. **Static Analysis:**
   - Zero High-Severity alerts in `servo_motor.c`.

## 4. Traceability
- **Source:** `tests/unit/servo-motor/src/servo_motor.c`
- **Tests:** `tests/unit/servo-motor/test/test_servo_functions.c`
