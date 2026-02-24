# Integration Test Execution Template - GENERIC Framework
## Reusable Template for Custom Integration Tests

---

## Header / Metadata
- **Date:** [YYYY-MM-DD]
- **Time Started:** [HH:MM UTC]
- **Tester Name:** [Full Name]
- **Test Phase:** [A/B/C/D/Other]
- **Test Name:** [Descriptive test name]
- **Test Cycle:** [Sprint X, Iteration Y]
- **Template Version:** 1.0

---

## Test Information
- **Phase Name:** [Full descriptive name of what is being tested]
- **Test Objective:** [Clear statement of what this test validates]
- **Related Requirements:** [Links to specifications or design documents]

---

## Environment Setup

### Hardware Required
- [ ] [Equipment 1]
- [ ] [Equipment 2]
- [ ] [Equipment 3]

### Software Required
- [ ] [Software 1 - version: __]
- [ ] [Software 2 - version: __]
- [ ] [Software 3 - version: __]

### Configuration Settings
- **Key Parameter 1:** [Value]
- **Key Parameter 2:** [Value]
- **Key Parameter 3:** [Value]

### Prerequisites Checklist
- [ ] [Prerequisite 1]
- [ ] [Prerequisite 2]
- [ ] [Prerequisite 3]

---

## Pre-Test Checklist
- [ ] [Pre-test item 1]
- [ ] [Pre-test item 2]
- [ ] [Pre-test item 3]
- [ ] [Pre-test item 4]

---

## Execution Steps

### Step 1: [Step Name]
**Procedure:**
1. [Action 1]
2. [Action 2]
3. [Action 3]

**Expected Output:**
```
[Expected output or behavior]
```

**Actual Output:**
```
[Record actual output here]
```

**Measurements/Observations:**
- [Measurement 1]: _______
- [Measurement 2]: _______

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 2: [Step Name]
**Procedure:**
1. [Action 1]
2. [Action 2]
3. [Action 3]

**Expected Output:**
```
[Expected output or behavior]
```

**Actual Output:**
```
[Record actual output here]
```

**Measurements/Observations:**
- [Measurement 1]: _______
- [Measurement 2]: _______

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

### Step 3: [Step Name]
**Procedure:**
1. [Action 1]
2. [Action 2]
3. [Action 3]

**Expected Output:**
```
[Expected output or behavior]
```

**Actual Output:**
```
[Record actual output here]
```

**Measurements/Observations:**
- [Measurement 1]: _______
- [Measurement 2]: _______

**Status:** [ ] PASS [ ] FAIL

**Notes/Issues:** [If any]

---

## Results Recording

### Overall Status
- [ ] **PASS** - All acceptance criteria met
- [ ] **FAIL** - One or more acceptance criteria not met
- [ ] **CONDITIONAL PASS** - Minor issues requiring documentation

### Pass/Fail Criteria Met
- [ ] Criterion 1
- [ ] Criterion 2
- [ ] Criterion 3

### Key Measurements Summary
| Metric | Value | Threshold | Status |
|--------|-------|-----------|--------|
| Measurement 1 | ___ | [Spec] | PASS/FAIL |
| Measurement 2 | ___ | [Spec] | PASS/FAIL |
| Measurement 3 | ___ | [Spec] | PASS/FAIL |

### Issues Encountered
- [ ] No issues
- [ ] Issues found (describe below):

**Issue #1:** [Description]
- Severity: [Critical/High/Medium/Low]
- Impact: [Impact description]

**Issue #2:** [Description]
- Severity: [Critical/High/Medium/Low]
- Impact: [Impact description]

### Performance Metrics Summary
- [Metric 1]: _______
- [Metric 2]: _______
- [Metric 3]: _______

---

## Evidence Artifacts

### Logs
- [ ] [Log file 1]: `[filename]`
  - Location: _______________
  
- [ ] [Log file 2]: `[filename]`
  - Location: _______________

### Data Captures
- [ ] [Capture file 1]: `[filename]`
  - Format: [Type]
  - Location: _______________

### Screenshots
- [ ] [Screenshot 1]: `[filename]`
- [ ] [Screenshot 2]: `[filename]`

### Other Evidence
- [ ] [Other artifact]: `[filename]`
  - Location: _______________

---

## Notes / Observations

### General Observations
[Record general notes about test execution]

### Anomalies Found
[Document any unexpected behaviors]

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
**Location in Repository:** `/tests/integration/executions/[phase]/`
**Committed:** [ ] Yes [ ] No
**Commit Hash:** _______________

---

**Template Version:** 1.0 | **Last Updated:** 2026-02-23 | **Created by:** Team04_DrivaPi Integration Test Team
