---
id: URD-RESOURCES_MONITOR
header: "System Resource Monitoring"
text: |
  "Monitors Raspberry Pi/STM32 resources and alerts on thresholds."
verification_method: "System Testing + Validation Evidence (Logs)"
children:
  - id: SRD-SYS-RES_MONITOR
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: ''
# references:
#   - type: "file"
#     path: docs/standards/iso26262/hara_overview.md
#     description: "General Resource Hazard"
active: true
derived: false
normative: true
level: 1.0
---
The system shall monitor data from Raspberry Pi and STM32 resources during operation and generate alerts or enter safe state (degraded mode or stop) when any threshold is exceeded.
---
