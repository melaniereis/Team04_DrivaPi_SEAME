---
id: SRD-SAFETY-MEMORY_ISOLATION
header: "Memory Isolation for Safety"
text: |
  "Defines configuration of hardware memory protection for isolation."
verification_method: "Design Review & Static Analysis"
parents:
  - id: URD-MEMORY_PROTECTION
children:
  - id: SWD-SAFETY-MPU_CONFIG
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval
references:
  - type: "file"
    path: docs/standards/iso26262/hara_memory.md
    description: "Supports safe memory isolation"
active: true
derived: false
normative: true
level: 2.0
---
The system shall configure the Memory Protection Unit (MPU) or similar hardware features to assign separate memory regions for tasks and drivers; ensure write protection on code segments; detect boundary violations; and trigger a safe state on unauthorized access.
