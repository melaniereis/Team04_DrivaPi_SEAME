# ASIL Justification: SWD-SpeedSensor

## 1. Requirement Summary
- **Requirement ID:** SWD-SpeedSensor
- **Component:** Speed Sensor Software Component (`speed_sensor.c`)
- **Safety Goal:** SG-01 (Prevent False Zero Speed Reporting)

## 2. ASIL Decomposition & Assignment
- **Assigned ASIL:** **A**
- **Justification:** This software component is the direct implementation of Safety Goal SG-01. If this code fails (e.g., miscalculates pulse delta due to timer overflow), it directly creates Hazard H-01 (False Zero Speed). Therefore, it inherits the ASIL A rating from the HARA.

## 3. Verification Strategy (ASIL A)
To meet the "Trustable" standard for ASIL A, the following verification activities are mandatory for this specific requirement:

1.  **Unit Testing:**
    * Must verify specific edge cases: Timer Wrap-around (Overflow), Division by Zero protection.
    * **Target:** 100% Requirements Coverage.

2.  **Static Analysis:**
    * Must run `codeQL` or equivalent.
    * **Target:** Zero "Error" severity findings.

3.  **Code Coverage:**
    * **Target:** >90% Line Coverage (High confidence required for calculation logic).

4.  **Traceability:**
    * Must link explicitly to Source Code (`speed_sensor.c`) and Test Code (`test_speed_sensor.c`).

## 4. Dependencies
This justification assumes the hardware timer (TIM1) is functioning. Hardware failures are handled by a separate safety mechanism (Hardware Watchdog), which is outside the scope of this software requirement.
