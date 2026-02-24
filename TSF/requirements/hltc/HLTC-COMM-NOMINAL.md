---
id: HLTC-COMM_NOMINAL
header: "Bidirectional Communication Nominal Test"
text: |
  "Verifies reliable packet exchange under normal load conditions."
verification_method: "Integration Test (Packet Loss Analysis)"

parents:
  - id: SRD-COMM-PROTOCOL
  - id: URD-HMI_COMMUNICATION

reviewers:
  - name: "Melanie Reis"
    email: "melanie.reis@seame.pt"
reviewed: '2026-02-24 - Approved by Melanie Reis <melanie.reis@seame.pt>'

references:
  - type: "file"
    path: "tests/integration/integration_tests_plan.md"
    note: "Phase B — Raspberry Pi ↔ STM32 CAN bidirectional integration test plan + criteria."
  - type: "file"
    path: "docs/data-transfer/CAN/latency/can_latency_receive_test.cpp"
    note: "Phase B — RPi sends frames, measures receive/latency via SocketCAN."
  - type: "file"
    path: "docs/data-transfer/CAN/latency/can_latency_send_test.c"
    note: "Phase B — STM32 echoes frames back for bidirectional validation."

score: 1.0

active: true
derived: false
normative: true
level: 2.0
---
Bidirectional communication between Firmware and HMI shall maintain <1% packet loss at nominal 10Hz data rates.
