# Functional vs Integration Tests — Study Note (DrivaPi)

This note explains what functional tests and integration tests are, how they differ, and how to apply them to DrivaPi. It is framework-agnostic and focuses on concepts, examples, and practical guidance.

---

## Functional Tests

- Purpose: Verify that a specific function or feature behaves according to its requirement (black-box focus on behavior, not implementation details).
- Scope: A single feature or narrow slice of behavior; surrounding factors are controlled so the behavior under test is clear and repeatable.
- Inputs/Stimuli: Clearly defined inputs (commands, signals, UI actions) and preconditions.
- Expected Results: Observable outputs/behaviors and acceptance criteria.
- Pass/Fail: Determined by matching behavior to the requirement.

DrivaPi examples:
- Throttle input mapping produces the correct target value across the full input range.
- PWM driver setup applies the intended duty cycle to the configured channel (see firmware/Core/Src/pca9685.c).
- GUI “Start” control changes state and shows the correct status indicator.

---

## Integration Tests

- Purpose: Verify that multiple components work together correctly and respect their interface contracts.
- Scope: Two or more components connected (e.g., controller + CAN + peripheral), validating data flows and timing across boundaries.
- Inputs/Stimuli: End-to-end or cross-component stimuli that traverse interfaces.
- Expected Results: Correct interactions, message formats, state transitions, and timing across modules.
- Pass/Fail: Based on interface and end-to-end behavior criteria.

DrivaPi examples:
- Sensor pipeline to actuation: sensor reading → processing → controller decision → PWM update on the actuator driver.
- Controller publishes periodic status over CAN; a receiver interprets the message and updates system state.
- GUI command triggers a controller action and receives an acknowledgement that updates the display.

---

## Key Differences

- Focus: Functional = requirement/feature behavior; Integration = component interactions.
- Granularity: Functional = narrow slice; Integration = cross-component slice.
- Dependencies: Functional = controlled surroundings; Integration = real interfaces between components.
- Failure Localization: Functional = pinpoints behavior in one feature; Integration = reveals interface/contract issues.
- Test Data: Functional = covers feature boundaries and typical/edge cases; Integration = covers protocol/flow/timing across modules.

---

## When to Use Which in DrivaPi

- Use functional tests to validate a single requirement or feature without involving full stacks.
- Use integration tests when changes affect interfaces, flows, or timing between modules (e.g., controller ↔ CAN, controller ↔ PWM driver, GUI ↔ controller).
- Combine both: start with functional tests for clarity and speed; add integration tests to catch cross-component regressions.

---

## Minimal Test Case Template

- ID/Title: Short, descriptive name
- Objective: Requirement or interaction being verified
- Preconditions: System/setup assumptions
- Steps/Stimuli: Inputs and actions
- Expected Results: Observable outcomes and criteria
- Notes: Data sets, corner cases, measurements

---

## Common Pitfalls

- Blurry scope (mixing feature and multi-component checks in one test)
- Flaky timing-sensitive integration tests without defined tolerances
- Ambiguous expected results (lack of precise acceptance criteria)
- Over-mocking in functional tests that hides real-world constraints, or over-integrating too early, making failures hard to localize

---

## Quick DrivaPi Example Scenarios (Sketches)

- Functional: "Given throttle = 60%, then target command = X within tolerance Y."
- Functional: "Given PWM channel = 3 and duty = 25%, the configured value matches the expected register scaling."
- Integration: "Given sensor value S, then controller output drives PWM to P and the GUI status shows state Z."
- Integration: "Given periodic controller status, a message is seen on the bus and the receiving component updates its state accordingly."

---

## Outcome for Spike

This note is the deliverable: it defines functional vs integration tests, lists key differences, provides DrivaPi-aligned examples, a minimal test case template, and pitfalls to avoid.
