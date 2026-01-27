---
id: LLTC-AUTO_TRACK_RUN
header: "Autonomous Track Run Validation"
text: |
  "System validation evidence for URD-AUTONOMOUS_PERF."

verification_method: "System Validation (Track Log + Video)"

parents:
  - id: SWD-PID_LOOP

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/autonomy/LLTC-AUTO_TRACK_RUN/track_log.csv"
#   - type: "file"
#     path: "artifacts/verification/autonomy/LLTC-AUTO_TRACK_RUN/track_video.mp4"
#   - type: "file"
#     path: "artifacts/verification/autonomy/LLTC-AUTO_TRACK_RUN/acceptance_summary.md"

active: true
derived: false
normative: true
level: 4.0
---
Autonomous mode shall satisfy performance constraints (speed cap, lateral deviation, speed variation) on a representative track run.
---
