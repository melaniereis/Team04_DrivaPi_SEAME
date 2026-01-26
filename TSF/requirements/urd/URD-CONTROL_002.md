---
id: URD-CONTROL_002
header: "Real-time Control Loop"
text: |
  "Guarantees deterministic execution for safety-critical tasks."
verification_method: "Integration Testing"
children:
  - id: SRD-RTOS_SCHEDULING
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
  - type: "file"
    path: docs/standards/iso26262/hara_rtos.md
    description: "Mitigates Hazard H-06 (Scheduling Overrun)"
active: true
derived: false
normative: true
level: 1.0
---
ThreadX shall run control tasks with deterministic timing (<50ms latency).
---
