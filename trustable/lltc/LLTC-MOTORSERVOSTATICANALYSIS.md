---
id: LLTC-MOTORSERVOStaticAnalysis
header: "Static Code Analysis (CodeQL)"
text: |
  "The actuator source code shall pass CodeQL security and quality queries with zero 'error' level findings (High Severity)."

# TSF Type: Evidence/Premise (A Claim, but not a Request)

verification_method: "Static Analysis (GitHub CodeQL)"

# Links: Connects to the SWD it provides evidence for
parents:
  - id: SWD-MOTORSERVO

reviewers:
  - name: "Miguel Cardoso"
    email: "miguel.cardoso@seame.pt"
reviewed: '2026-01-07 - Approved by Miguel Cardoso <miguel.cardoso@seame.pt>'

# Evidence Linking (use 'references:', NOT deprecated 'artifact:')
references:
  - type: "file"
    path: "artifacts/verification/static-analysis/codeql.sarif"

#Automated Validation
evidence:
  type: codeql_sarif_validator
  references:
    - type: "file"
      path: "artifacts/verification/static-analysis/codeql.sarif"
  configuration:
    allowed_errors: 0

active: true
derived: false
normative: true
level: 4.0
---
This requirement ensures no buffer overflows, uninitialized variables, or unsafe type conversions exist in the motor control logic.