# Sprint 8 Review — Team04 DrivaPi

**Date:** February 2-13, 2026

**Sprint goal:** _Ensure hardware infrastructure stability, enhance infotainment features with real-time telemetry, and validate three key functional requirements (URD-SPEED_MEASUREMENT, URD-STEERING_CONTROL and URD-PROPULSION_CONTROL) through integration, system, and unit testing._

---

## Table of contents
- Summary
- Key achievements
- Decisions
- Measurements & validation
- Challenges & solutions
- Next steps (Sprint 9)
- Team highlights
- Team feedback (retrospective)
- Outcome

---

## Summary
Sprint 8 represented the final iteration of the project, with the team focused on consolidating hardware stability, advancing Qt-based infotainment features, and formally validating core functional requirements through integration and system testing. Whilst the team delivered the majority of planned work, the sprint was marked by significant organisational challenges — including poor inter-team communication and an overambitious scope — which prevented full completion of the emergency braking system, STM32–Qt communication, and all planned physical hardware tests.

## Key achievements
- **Hardware infrastructure:** Maintained stable hardware platform throughout the sprint, ensuring core vehicle components remained operational for testing.
- **Real-time telemetry:** Advanced infotainment telemetry features within the Qt application, improving data display and signal handling.
- **Functional requirement validation:** Made measurable progress towards validating URD-SPEED_MEASUREMENT, URD-STEERING_CONTROL, and URD-PROPULSION_CONTROL through unit and integration testing.
- **Sprint planning:** The team demonstrated solid planning capabilities, with a clearly structured backlog and well-defined acceptance criteria for each task.

## Decisions
- Carry forward incomplete emergency braking logic to Sprint 9 with dedicated ownership and a clear implementation deadline.
- Prioritise STM32–Qt communication resolution as a Sprint 9 blocker, given its impact on multiple dependent features.
- Reduce sprint scope in future iterations to ensure a sustainable and achievable workload per team member.
- Improve inter-team communication cadence to prevent parallel work from diverging silently.

## Measurements & validation
| Metric | Value | Status |
|------|------:|:-----|
| Functional Requirements Targeted | 3 (URD-SPEED_MEASUREMENT, URD-STEERING_CONTROL, URD-PROPULSION_CONTROL) | 🔄 Partial |
| Emergency Braking Implementation | Not completed | ❌ Incomplete |
| STM32–Qt Communication | Not functional | ❌ Incomplete |
| Qt Feature Completion | Partial | 🔄 In Progress |
| Physical Tests Executed | Partial — not all tests completed | 🔄 Incomplete |

## Challenges & solutions
| Challenge | Severity | Resolution |
|---|---|---|
| **Emergency braking not failure** | High | Deferred to Sprint 9 with dedicated ownership and clear acceptance criteria |
| **STM32–Qt communication failure** | High | Root cause analysis to be conducted at Sprint 9 kickoff; assigned as top priority |
| **Incomplete Qt features** | Medium | Remaining features scoped into Sprint 9 backlog with reduced overall scope |
| **Physical tests not fully executed** | Medium | Test sessions to be rescheduled in Sprint 9 with dedicated hardware time |
| **Lack of inter-team communication** | High | Team retrospective surfaced this issue; daily syncs to be reinforced going forward |
| **Overwhelming sprint scope** | High | Sprint 9 scope reduced to reflect team capacity and account for carryover work |

## Next steps (Sprint 9 priorities)
1. **Emergency braking logic:** Complete and validate STM32 emergency braking implementation with full TSF traceability.
2. **STM32–Qt communication:** Resolve communication pipeline between STM32 firmware and Qt telemetry display as a Sprint 9 blocker.
3. **Qt feature completion:** Deliver outstanding Qt infotainment features deferred from Sprint 8.
4. **Physical testing:** Execute all physical hardware tests in a dedicated, scheduled session.
5. **ADAS stack initiation:** Begin establishing the Camera/HAILO/CARLA stack for ADAS functionality.
6. **TSF traceability enforcement:** Enforce strict TSF traceability through real-code testing across all safety-critical components.
7. **Communication improvement:** Reinforce daily syncs and inter-role communication to prevent silent divergence of parallel workstreams.

## Team highlights
- **Bernardo:** Hardware integration, physical testing execution, and STM32 firmware work.
- **Gaspar:** Development environment stability, communication pipeline investigation, and repository upkeep.
- **Hugo:** Hardware platform maintenance, physical testing coordination, and integration support.
- **Melanie:** Qt application development, GUI coordination, telemetry feature progress, and team coordination.
- **Miguel:** Sprint planning, backlog management, Agile facilitation, and progress tracking.

## Team feedback (retrospective)

### What went well
- **Bernardo:** Delivery was average; the core objectives were partially met.
- **Gaspar:** Could have been better — the fundamentals were there but execution fell short of expectations.
- **Hugo:** It was good overall — the team delivered what was planned for the most part.
- **Melanie:** Average — goals were broadly addressed, though the workload was excessive.
- **Miguel:** Average — the sprint achieved a reasonable baseline of its intended outcomes.

### What went poorly
- **Bernardo:** The sprint felt chaotic, with insufficient structure during execution.
- **Gaspar:** Lack of communication; agrees with Hugo's assessment that execution did not match the quality of planning.
- **Hugo:** Good planning, poor execution — the disconnect between plan and delivery was the core issue.
- **Melanie:** Overwhelming — the scope was too large for the available capacity.
- **Miguel:** Chaotic — coordination between workstreams was insufficient.

---

## Outcome
Sprint 8 goal **partially achieved**: the team maintained hardware stability and advanced telemetry and Qt features, but fell short of completing the emergency braking system, STM32–Qt communication, and all planned physical tests. The sprint exposed a structural challenge — an ambitious scope combined with inadequate inter-team communication — which resulted in a chaotic and at times overwhelming experience for the team.

The retrospective produced clear, actionable insights: reduce sprint scope to match capacity, enforce daily communication rituals, and assign explicit ownership to high-priority items carried over from Sprint 8.

**Foundation for Sprint 9:** The team enters Sprint 9 with a focused and realistic backlog centred on completing the vehicle's safety-critical features, validating functional requirements end-to-end, and initiating the ADAS stack.

**Next review:** Sprint 9 Review
