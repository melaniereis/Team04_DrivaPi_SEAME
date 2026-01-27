---
id: URD-OBSTACLE_RECOGNITION
header: "Obstacle Recognition (Vision)"
text: |
  "Provides redundant obstacle detection as primary to backup sensors."
verification_method: "System Testing"
children:
  - id: SRD-SENS-OBJ_DET
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
  - type: "file"
    path: docs/standards/iso26262/hara_obstacle.md
    description: "Mitigates Hazard H-04 (Missed Obstacle)"
active: true
derived: false
normative: true
level: 1.0
---
Vision pipeline shall detect obstacles as primary to backup ultrasonic sensor.
---
