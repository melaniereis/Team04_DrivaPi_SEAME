---
id: LLTC-SPEEDSTATICANALYSIS
header: "Static Code Analysis (CodeQL)"
text: |
  "The source code shall pass CodeQL security and quality queries with zero
  high-severity errors (as defined in the SARIF output)."

# TSF Type: Evidence
verification_method: "Static Analysis (GitHub CodeQL)"

parents:
  - id: SWD-SPEEDSENSOR

reviewers:
  - name: "Hugo Lopes"
    email: "hugo.lopes@seame.pt"

reviewed: '2026-01-06 - Approved by Hugo Lopes <hugo.lopes@seame.pt>'

references:
  - type: "file"
    path: "artifacts/verification/static-analysis/codeql.sarif"
    description: "CodeQL SARIF Report"

evidence:
  type: codeql_sarif_validator
  configuration:
    allowed_errors: 0
    references:
    - type: file
      path: artifacts/verification/static-analysis/codeql.sarif

active: true
derived: false
normative: true
level: 4.0
---
Uses the `codeql_sarif_validator` to ensure strict compliance.
