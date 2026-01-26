---
id: SRD-CROSS_PLAUSIBILITY_001
header: "Cross-sensor Plausibility Check"
text: |
  "Implements cross-plausibility checks between sensors."
verification_method: "Integration Testing"
parents:
  - id: URD-DATA_VALIDATION_001
children:
  - id: SWD-PLAUSIBILITY_CHECK
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
# references:
#   - type: "report"
#     path: comparison_report.md
#     description: "Addresses sensor drift hazards"
active: true
derived: false
normative: true
level: 2.0
---
The system shall compare primary speed sensor measurements with a secondary source (e.g., IMU-derived velocity or vision-based odometry). If the difference between the two exceeds a calibrated threshold for more than a specified time, the system shall flag a plausibility fault and enter a safe state (e.g., stop or degrade speed).
---
