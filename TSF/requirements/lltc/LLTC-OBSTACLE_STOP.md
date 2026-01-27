---
id: LLTC-OBSTACLE_STOP
header: "Obstacle → Stop/Avoid Reaction (System Functional)"
text: |
  "Proves URD-OBSTACLE_AVOIDANCE trigger and response on real obstacle presentation."

verification_method: "System Functional Test (Track/Bench + Logs + Video)"

parents:
  - id: SWD-ULTRASONIC_DRIVER

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''

# references:
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-OBSTACLE_STOP/obstacle_distance.csv"
#     description: "Distance samples and trigger point."
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-OBSTACLE_STOP/obstacle_stop_log.txt"
#     description: "Logs showing detection → stop command."
#   - type: "file"
#     path: "artifacts/verification/system/LLTC-OBSTACLE_STOP/obstacle_video.mp4"
#     description: "Video proof."

active: true
derived: false
normative: true
level: 4.0
---
When an obstacle is presented within the defined threshold, the system shall trigger stop/avoid behavior as specified.
---
