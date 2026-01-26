---
id: SRD-CONTROL_002
header: "ThreadX Task Scheduling"
text: |
  "Ensures real-time predictability for safety loops."
verification_method: "Integration Testing"
parents:
  - id: URD-CONTROL_002
children:
  - id: SWD-RTOS_CONFIG
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval
references:
  - type: "file"
    path: docs/standards/iso26262/hara_rtos.md
    description: "Supports SG-06 (Deterministic Timing)"
active: true
derived: false
normative: true
level: 2.0
---
ThreadX shall schedule control tasks with <50ms latency using priority inheritance.
