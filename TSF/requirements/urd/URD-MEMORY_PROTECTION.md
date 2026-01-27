---
id: URD-MEMORY_PROTECTION
header: "Memory Protection & Fault Containment"
text: |
  "Prevents faulty components from corrupting memory or code of other modules."
verification_method: "Design Review & Integration Testing"
children:
  - id: SRD-SAFETY-MEMORY_ISOLATION
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
  - type: "file"
    path: docs/standards/iso26262/hara_memory.md
    description: "Mitigates hazard due to memory corruption"
active: true
derived: false
normative: true
level: 1.0
---
The software architecture shall leverage hardware memory protection (e.g., a Memory Protection Unit) to isolate critical tasks (safety-critical code and RTOS) from non-critical tasks, ensuring faults in non-critical components cannot corrupt memory of critical components.
---
