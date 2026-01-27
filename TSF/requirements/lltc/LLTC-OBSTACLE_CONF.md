---
id: LLTC-OBSTACLE_CONF
header: "Obstacle Recognition Confidence Validation"
text: |
  "Provides objective confidence/precision evidence for URD-OBSTACLE_RECOGNITION."

verification_method: "System Validation (Dataset/Track + Metrics)"

parents:
  - id: SWD-OBSTACLE_PIPELINE

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/vision/LLTC-OBSTACLE_CONF/metrics.json"
#   - type: "file"
#     path: "artifacts/verification/vision/LLTC-OBSTACLE_CONF/test_set_description.md"

active: true
derived: false
normative: true
level: 4.0
---
Vision obstacle recognition shall meet agreed confidence/precision thresholds in the defined ODD scenarios.
---
