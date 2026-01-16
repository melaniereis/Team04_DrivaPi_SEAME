# ASIL Justification: SWD-DC_MOTOR

## 1. Requirement Summary
- **Requirement ID:** SWD-DC_MOTOR
- **Component:** DC Motor Logic (`dc_motor.c`)
- **Safety Goal:** SG-02 (Safe Propulsion)

## 2. ASIL Decomposition
- **Assigned ASIL:** **A**
- **Rationale:** Failure of this component directly leads to Hazard H-02 (Unintended Acceleration). As derived in `hara_actuation.md`, this risk is classified as ASIL A.

## 3. Verification Strategy
To meet ASIL A for Propulsion Control:

1. **Unit Testing:**
   - Verify `0.0` input results in `0` PWM (Deadzone check).
   - Verify `>1.0` inputs are clamped to `1.0`.
   - **Target:** 100% Branch Coverage.

2. **Static Analysis:**
   - Zero High-Severity alerts in `dc_motor.c`.

## 4. Traceability
- **Source:** `tests/unit/dc_motor/src/dc_motor.c`
- **Tests:** `tests/unit/dc_motor/test/test_dc_motor_functions.c`
