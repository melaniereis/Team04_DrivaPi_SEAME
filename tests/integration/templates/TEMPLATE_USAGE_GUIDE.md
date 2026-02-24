# Integration Test Execution Templates - Usage Guide

## Overview
These templates provide a standardized format for executing and documenting integration tests across all 4 phases of the system validation.

## Quick Start

### 1. Select Template
Choose the appropriate phase template:
- **Phase A:** `PHASE_A_TEMPLATE.md` - PS3 Controller → Raspberry Pi
- **Phase B:** `PHASE_B_TEMPLATE.md` - Raspberry Pi ↔ STM32 CAN communication
- **Phase C:** `PHASE_C_TEMPLATE.md` - STM32 → Motor Controllers I2C
- **Phase D:** `PHASE_D_TEMPLATE.md` - Speed Sensor → STM32

### 2. Create Execution Instance
Copy the template to `/tests/integration/executions/phase_[X]/` directory with naming convention:
```
YYYY-MM-DD_phase_X_execution_NNN.md
```

Example:
```
2026-02-23_phase_a_execution_001.md
2026-02-23_phase_b_execution_002.md
```

### 3. Fill Metadata
Update the header section with:
- **Date:** Test execution date (YYYY-MM-DD)
- **Time Started:** When test begins (HH:MM UTC)
- **Tester Name:** Your full name
- **Test Phase:** A, B, C, or D
- **Test Cycle:** Current sprint & iteration
- **Template Version:** Version of template used

### 4. Pre-Test Validation
Before starting execution:
1. Verify all prerequisites from header section
2. Check all hardware/software required is available
3. Ensure environment is clean and isolated
4. Complete the Pre-Test Checklist (all items must be checked)

### 5. Execute Tests
Follow each step in the "Execution Steps" section sequentially:
- Record the actual output from each step
- Note any discrepancies vs. expected output
- Document any unexpected behaviors
- Capture metrics/measurements

### 6. Record Results
Immediately after completion:
- Mark overall status as PASS or FAIL
- Document key measurements
- List any issues encountered
- Complete results recording section

### 7. Collect Evidence
Gather all evidence artifacts:
- Log files (name and location)
- Data captures/traces (CAN, I2C, etc.)
- Screenshots or video
- Physical observations
- Check off each item collected

### 8. Add Observations
Record in the Notes section:
- Any anomalies or unusual behaviors
- Environment issues
- Timing concerns
- Suggestions for next iterations
- Follow-up actions needed

### 9. Get Sign-Off
Before archiving:
- Request review from test lead or team member
- Get approval from appropriate authority
- Record approval date
- Ensure proper naming before committing to git

### 10. Archive & Commit
Final step:
```bash
cd /tests/integration/executions/phase_[X]/
git add YYYY-MM-DD_phase_X_execution_NNN.md
git commit -m "Test Execution: Phase [X] - [DATE] - [Status]"
git push
```

## Common Issues & Troubleshooting

### Evidence Not Captured
- For CAN logs: Ensure CANallyzer or equivalent is running before test
- For I2C captures: Connect logic analyzer before setup
- For logs: Verify logging is enabled in software before starting

### Timing Measurements
- Always use synchronized clocks (UTC recommended)
- Use high-resolution timing if available
- Document measurement precision in notes

### Equipment Connectivity
- Pre-test checklist must verify power and connections
- Document exact cable connections in setup section
- Use same equipment across iterations for consistency

## Template Sections Reference

### Header/Metadata
- Uniquely identifies test execution
- Links to specific sprint/iteration
- Enables audit trail

### Test Information
- Connects to system requirements
- Explains validation purpose
- Links to design documentation

### Environment Setup
- Captures hardware/software configuration
- Ensures reproducibility
- Documents prerequisites

### Pre-Test Checklist
- Prevents invalid test execution
- Ensures system readiness
- Captures setup state

### Execution Steps
- Provides procedural guidance
- Documents expected outputs
- Enables verification of each step

### Results Recording
- Captures pass/fail determination
- Documents measurement data
- Records issues encountered

### Evidence Artifacts
- Ensures complete documentation
- Enables post-test analysis
- Creates audit trail

### Notes/Observations
- Captures context and learnings
- Documents anomalies
- Enables process improvement

## Best Practices

1. **Complete ALL Sections** - Don't skip sections, even if empty
2. **Be Specific** - Avoid vague descriptions; use exact values, times, filenames
3. **Date Everything** - Include timestamps in logs and captures
4. **Document Issues** - Record any problems immediately, don't rely on memory
5. **Consistent Formatting** - Follow the markdown structure exactly
6. **Time-Sensitive Work** - Fill out results immediately after execution
7. **Version Control** - Always commit completed templates to git
8. **Cross-Reference** - Link to related tests or requirements when applicable
9. **Evidence Naming** - Use descriptive filenames for all captured evidence
10. **Approval Required** - Don't archive without proper sign-off

## Template Versions

- **v1.0** (2026-02-23): Initial template creation based on integration test plan
  - Generic structure for all phases
  - Phase-specific sections defined
  - Usage guide created

---

For questions or template improvements, refer to `/docs/tests/integration-test-plan.md` or contact the test lead.
