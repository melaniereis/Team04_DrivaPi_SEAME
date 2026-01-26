---
id: SWD-PLAUSIBILITY_CHECK
header: "Cross-sensor Plausibility Check Logic"
text: |
  "Ensures sensor consistency through cross-checks."

ASIL: "A"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-CROSS_PLAUSIBILITY_001
children: []
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Implements cross-sensor plausibility logic"
active: true
derived: false
normative: true
level: 3.0
---
The software shall implement logic to compare primary speed sensor data with secondary sources (e.g., IMU-derived velocity or vision-based odometry). If the deviation exceeds a calibrated threshold for a defined time, it shall trigger a plausibility fault.
---
