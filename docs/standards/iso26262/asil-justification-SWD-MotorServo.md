# ASIL Justification: SWD-MOTORSERVO

## 1. Requirement Summary
- **Requirement ID:** SWD-MOTORSERVO
- **Components:** Actuation Logic (`servo_motor.c`, `dc_motor.c`)
- **Safety Goal:** SG-02 (Prevent Unintended Actuation / Loss of Physical Control)

## 2. ASIL Decomposition & Assignment
- **Assigned ASIL:** **A**
- **Justification:** This software component contains the critical logic for clamping inputs and calculating PWM pulses. If this logic fails (e.g., integer overflow or missing bounds check), it directly leads to Hazard H-02 (Unintended Actuation). Based on the HARA assessment of **Severity S1** (Light Injury) and **Controllability C2** (Normally Controllable), the target is ASIL A.

## 3. Verification Strategy (ASIL A)
To meet the "Trustable" standard for ASIL A, the following verification activities are mandatory for this specific requirement:

1.  **Unit Testing:**
    * Must verify **Boundary Values** explicitly:
        * Input 180° (Valid) vs 181° (Invalid -> Clamped).
        * Input -1.0 (Valid) vs -1.1 (Invalid -> Clamped).
    * Must verify **Zero/Deadzone** behavior.
    * **Target:** 100% Requirements Coverage.

2.  **Static Analysis:**
    * Must run `CodeQL` (Security & Quality rules).
    * **Target:** Zero "High" severity findings (Errors).

3.  **Code Coverage:**
    * **Target:** >90% Branch Coverage.
    * *Rationale:* Actuation logic uses many `if/else` conditions for clamping. Branch coverage is essential to prove the "unsafe" paths are unreachable or handled correctly.

4.  **Traceability:**
    * Must link explicitly to Source Code (`dc_motor.c`, `servo_motor.c`) and Test Code (`test_dc_motor_functions.c`, `test_servo_functions.c`).

## 4. Dependencies
This justification relies on the correct operation of the **PCA9685 Hardware Driver**. The verification of I2C communication integrity is handled at the Integration Test level (SRD), whereas this justification focuses on the logical correctness of the actuation calculations (SWD).