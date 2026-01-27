---
id: URD-OBSTACLE_AVOIDANCE
header: "Obstacle Detection"
text: |
  "Ensures vehicle safety by preventing collisions in autonomous mode."
verification_method: "Integration Testing"
children:
  - id: SRD-SENS-US_DISTANCE
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
The system shall detect obstacles in front and trigger collision avoidance.
---
