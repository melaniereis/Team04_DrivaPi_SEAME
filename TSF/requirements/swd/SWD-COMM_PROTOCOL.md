---
id: SWD-COMM_PROTOCOL
header: "Bidirectional Communication Protocol"
text: |
  "Implements reliable data exchange with timeout handling."
ASIL: "A"
verification_method: "Unit Testing, Integration Testing"
parents:
  - id: SRD-COMM-PROTOCOL
children:
  - id: LLTC-COMM_UNIT
  - id: LLTC-COMM_COV
reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '' # Manually fill on PR approval
# references:
#   - type: "file"
#     path: firmware/Core/Src/comm_protocol.c
#     description: "Production Implementation (placeholder)"
active: true
derived: false
normative: true
level: 3.0
---
Communication module shall handle bidirectional packets.
