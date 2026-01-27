---
id: URD-STARTUP_CALIBRATION
header: "System Startup & Calibration"
text: |
  "Ensures safe and reliable system initialization."
verification_method: "System Testing"
children:
  - id: SRD-SYS-BOOT_CALIB
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
  - type: "file"
    path: docs/standards/iso26262/hara_startup.md
    description: "Mitigates Hazard H-09 (Calibration Error)"
active: true
derived: false
normative: true
level: 1.0
---
On power-up, system shall auto-calibrate servo neutral position and perform self-tests.
---
