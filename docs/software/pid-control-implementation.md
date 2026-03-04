# PID Control Loop Implementation for DC Motors

## Overview

This document outlines the design, theory, and implementation strategy for PID (proportional-integral-derivative) control loops to regulate DC motor speed on the DrivaPi platform. The implementation ensures smooth acceleration, accurate speed tracking, and stable steady-state behavior.

---

## 1. PID Theory

### 1.1 Core Concept

A PID controller continuously adjusts a control signal (motor PWM) based on the error between a desired setpoint (target speed) and actual measured output (current speed).

### 1.2 PID Equation

The fundamental PID control law:

$$u(t) = K_p \cdot e(t) + K_i \int_0^t e(\tau) d\tau + K_d \frac{de(t)}{dt}$$

Where:
- **u(t)** = Control output (PWM duty cycle, 0–4095)
- **e(t)** = Error = target_speed − actual_speed
- **K_p** = Proportional gain
- **K_i** = Integral gain
- **K_d** = Derivative gain

### 1.3 PID Terms Explained

#### Proportional (P) Term
- **Effect**: Direct, instantaneous response to error
- **Formula**: $u_p = K_p \cdot e(t)$
- **Behavior**:
  - Larger error → Larger control action
  - Fast response, but may overshoot and oscillate
- **Tuning**: Start with moderate K_p; increase until system responds quickly but doesn't oscillate excessively

#### Integral (I) Term
- **Effect**: Eliminates steady-state error over time
- **Formula**: $u_i = K_i \int_0^t e(\tau) d\tau$
- **Behavior**:
  - Accumulates past errors
  - Drives error to zero, even with constant disturbances
  - Can cause overshoot and slow response if K_i is too large
- **Tuning**: Start small; increase cautiously to avoid oscillation

#### Derivative (D) Term
- **Effect**: Dampens response; reduces overshoot and oscillation
- **Formula**: $u_d = K_d \frac{de(t)}{dt}$
- **Behavior**:
  - Responds to rate of change of error
  - Provides predictive correction before overshoot occurs
  - Sensitive to measurement noise
- **Tuning**: Increase to reduce overshoot; too high causes stiffness and noise sensitivity

### 1.4 Discrete-Time Implementation

Embedded systems operate on discrete time steps. The continuous PID equations are discretized:

$$u[n] = K_p \cdot e[n] + K_i \sum_{j=0}^{n} e[j] \cdot T_s + K_d \frac{e[n] - e[n-1]}{T_s}$$

Where:
- **n** = Current time step
- **T_s** = Sampling period (typically 10–100 ms)

#### Integral Term (Discrete)
$$\text{integral} := \text{integral} + e[n] \cdot T_s$$

#### Derivative Term (Discrete)
$$\text{derivative} = \frac{e[n] - e[n-1]}{T_s}$$

---

## 2. DC Motor Characteristics for DrivaPi

### 2.1 Motor Specifications

| Parameter | Value | Notes |
|-----------|-------|-------|
| Type | DC brushed motor | 3–6V nominal |
| Voltage Range | 3–6V | Powered from battery |
| RPM @ 6V | ~5000 RPM | Unloaded, no gearbox |
| Gearbox Ratio | 58:1 (typical) | Reduces RPM, increases torque |
| Output Speed | ~86 RPM | At gearbox output |
| Max Current | ~200 mA | Short-term peak with load |
| Encoder Resolution | Typically 20–40 PPR | Per motor shaft (pre-gearbox) |

### 2.2 Speed Measurement

**Encoder-Based Speed Calculation:**
```
raw_count = encoder_pulses_per_interrupt
speed_rpm = (raw_count / PPR) * (interrupt_frequency) * 60
```

Example:
- Interrupt frequency: 1 kHz (1 ms interval)
- Encoder: 20 PPR
- Gearbox: 58:1
- If 10 pulses counted in 1 ms:
  - Motor shaft speed ≈ 600 RPM
  - Wheel speed ≈ 600 / 58 ≈ 10.3 RPM

### 2.3 Motor Dynamics

**Non-linearities to consider:**
1. **Dead Zone**: Motor doesn't move below a minimum PWM (typically 20–30% of max)
2. **Saturation**: PWM capped at 4095 (100%)
3. **Inertia**: Speed doesn't change instantaneously; acceleration is gradual
4. **Load Variation**: Vehicle weight distribution, friction, and road incline affect motor load
5. **Thermal Effects**: Motor efficiency changes with temperature

---

## 3. Tuning Methods

### 3.1 Ziegler-Nichols Method (Experimental)

**Procedure:**
1. Set K_i = 0, K_d = 0; start with K_p = 0
2. Slowly increase K_p until system begins sustained oscillation (critical gain K_c)
3. Measure oscillation period (T_u)
4. Apply formulas:
   - **P-only**: K_p = 0.5 × K_c
   - **PI**: K_p = 0.45 × K_c, K_i = (K_p × 1.2) / T_u
   - **PID**: K_p = 0.6 × K_c, K_i = (K_p × 2) / T_u, K_d = K_p × T_u / 8

**Advantages**: Systematic, repeatable
**Disadvantages**: Can overshoot; requires oscillation experiment; risky on hardware

### 3.2 Manual Tuning (Recommended for DrivaPi)

**Step 1: Set Baseline (P only)**
- Set K_i = 0, K_d = 0
- Choose modest K_p (e.g., 50–100)
- Test step response: Set target speed suddenly, observe response
- Goal: Fast response, acceptable overshoot (~20%)

**Step 2: Add Integral (PI control)**
- Set K_i small (e.g., K_p / 10)
- Test steady-state error removal
- Increase K_i until error is eliminated without oscillation
- If oscillation occurs, reduce K_i

**Step 3: Add Derivative (PID)**
- Set K_d small (e.g., K_p × 0.01)
- Increase K_d to reduce overshoot and oscillation
- Watch for noise amplification; if unstable, reduce K_d
- Aim for critically damped response (fast, no oscillation)

**Empirical Starting Point:**
```
K_p = 100  (adjust based on motor response speed)
K_i = K_p / 20 = 5  (slow integral action)
K_d = K_p × 0.05 = 5  (mild derivative action)
```

### 3.3 Empirical Tuning Loop

```
for each tuning iteration:
  1. Set target speed to 50% max speed
  2. Observe rise time (time to reach target)
  3. Measure overshoot (how much speed exceeds target)
  4. Measure settling time (time to stabilize)
  
  If oscillating: reduce K_p or K_i; increase K_d
  If too slow: increase K_p
  If steady-state error persists: increase K_i
  If overshoot is excessive: increase K_d or reduce K_p
```

---

## 4. PID Architecture & Integration

### 4.1 System Architecture

```
┌─────────────────┐
│  Target Speed   │
│   (Setpoint)    │
└────────┬────────┘
         │
         ▼
┌─────────────────────────────────────┐
│  PID Controller                     │
│  ┌──────────┐  ┌──────────┐         │
│  │ Error    │  │ Integral │  ┌────┐│
│  │ × K_p    │  │ × K_i    │  │ +  ││
│  └──────────┘  └──────────┘  └─┬──┘│
│        └────────────┬──────────┘    │
│                     │               │
│         ┌──────────────────────┐    │
│         │  Derivative × K_d    │    │
│         └──────┬───────────────┘    │
│                │                    │
│                ▼                    │
│         ┌──────────────┐            │
│         │  Clamp to    │            │
│         │ [0, 4095]    │            │
│         └──────┬───────┘            │
└────────────────┼────────────────────┘
                 ▼
         ┌─────────────────┐
         │  PWM to Motor   │
         │  (0–4095)       │
         └────────┬────────┘
                  ▼
         ┌─────────────────┐
         │  Motor Driver   │
         │  (H-bridge)     │
         └────────┬────────┘
                  ▼
         ┌─────────────────┐
         │   DC Motor      │
         └────────┬────────┘
                  ▼
         ┌─────────────────┐
         │  Encoder        │
         │  (Speed Meas.)  │
         └────────┬────────┘
                  │
         ┌────────▼─────────┐
         │ Speed Calculation│
         │ (RPM)            │
         └────────┬─────────┘
                  │
         (feedback to PID)
```

### 4.2 Integration Points

#### C Firmware (`firmware/Core/`)

**File: `motor_control.h`**
```c
/**
 * @struct MotorPIDState
 * @brief PID controller state for one motor
 */
typedef struct {
    float target_speed;      // desired speed (RPM)
    float current_speed;     // measured speed (RPM)
    float error;             // e[n]
    float error_prev;        // e[n-1] for derivative
    float integral;          // sum of errors for integral term
    
    float kp, ki, kd;        // PID gains
    float pwm_output;        // computed PWM (0.0–1.0)
    uint16_t pwm_raw;        // raw PWM (0–4095)
} MotorPIDState;

/**
 * @brief Compute PID control output
 */
void MotorPID_Update(MotorPIDState *state, float current_speed);

/**
 * @brief Set PID gains
 */
void MotorPID_SetGains(MotorPIDState *state, float kp, float ki, float kd);

/**
 * @brief Reset integrator
 */
void MotorPID_Reset(MotorPIDState *state);
```

**File: `motor_control.c`**
```c
#define PID_SAMPLE_TIME 0.01f  // 10 ms update rate
#define PWM_MIN 300u           // Minimum PWM (dead zone)
#define PWM_MAX 4095u

void MotorPID_Update(MotorPIDState *state, float current_speed)
{
    // Calculate error
    state->error = state->target_speed - current_speed;
    
    // Proportional term
    float p_term = state->kp * state->error;
    
    // Integral term (with anti-windup)
    state->integral += state->error * PID_SAMPLE_TIME;
    if (state->integral > 100.0f) state->integral = 100.0f;
    if (state->integral < -100.0f) state->integral = -100.0f;
    float i_term = state->ki * state->integral;
    
    // Derivative term
    float derivative = (state->error - state->error_prev) / PID_SAMPLE_TIME;
    float d_term = state->kd * derivative;
    
    // Sum all terms
    state->pwm_output = p_term + i_term + d_term;
    
    // Clamp to valid range [0.0, 1.0]
    if (state->pwm_output > 1.0f) state->pwm_output = 1.0f;
    if (state->pwm_output < 0.0f) state->pwm_output = 0.0f;
    
    // Convert to raw PWM (0–4095)
    state->pwm_raw = (uint16_t)(state->pwm_output * 4095.0f);
    
    // Apply dead zone minimum
    if (state->pwm_raw > 0 && state->pwm_raw < PWM_MIN) {
        state->pwm_raw = PWM_MIN;
    }
    
    // Apply to motor driver
    SetMotorPWM(state->pwm_raw);
    
    // Store for next iteration
    state->error_prev = state->error;
    state->current_speed = current_speed;
}

void MotorPID_SetGains(MotorPIDState *state, float kp, float ki, float kd)
{
    state->kp = kp;
    state->ki = ki;
    state->kd = kd;
}

void MotorPID_Reset(MotorPIDState *state)
{
    state->error = 0.0f;
    state->error_prev = 0.0f;
    state->integral = 0.0f;
    state->pwm_output = 0.0f;
}
```

#### Rust Controller (`rust/controller/`)

**Structure for PID control in Rust:**
```rust
pub struct MotorPID {
    target_speed: f64,
    current_speed: f64,
    error: f64,
    error_prev: f64,
    integral: f64,
    
    kp: f64,
    ki: f64,
    kd: f64,
    
    pwm_output: f64,
}

impl MotorPID {
    pub fn new(kp: f64, ki: f64, kd: f64) -> Self {
        Self {
            target_speed: 0.0,
            current_speed: 0.0,
            error: 0.0,
            error_prev: 0.0,
            integral: 0.0,
            kp,
            ki,
            kd,
            pwm_output: 0.0,
        }
    }

    pub fn update(&mut self, current_speed: f64) -> u16 {
        const SAMPLE_TIME: f64 = 0.01;  // 10 ms
        const PWM_MIN: f64 = 300.0;
        const PWM_MAX: f64 = 4095.0;

        self.error = self.target_speed - current_speed;

        // P term
        let p_term = self.kp * self.error;

        // I term (with anti-windup)
        self.integral += self.error * SAMPLE_TIME;
        self.integral = self.integral.clamp(-100.0, 100.0);
        let i_term = self.ki * self.integral;

        // D term
        let derivative = (self.error - self.error_prev) / SAMPLE_TIME;
        let d_term = self.kd * derivative;

        // Total output
        self.pwm_output = p_term + i_term + d_term;
        self.pwm_output = self.pwm_output.clamp(0.0, 1.0);

        // Convert to raw PWM
        let mut pwm_raw = (self.pwm_output * 4095.0) as u16;
        
        // Apply dead zone
        if pwm_raw > 0 && pwm_raw < PWM_MIN as u16 {
            pwm_raw = PWM_MIN as u16;
        }

        self.error_prev = self.error;
        self.current_speed = current_speed;
        
        pwm_raw
    }

    pub fn set_target_speed(&mut self, speed: f64) {
        self.target_speed = speed;
    }

    pub fn set_gains(&mut self, kp: f64, ki: f64, kd: f64) {
        self.kp = kp;
        self.ki = ki;
        self.kd = kd;
    }

    pub fn reset(&mut self) {
        self.error = 0.0;
        self.error_prev = 0.0;
        self.integral = 0.0;
        self.pwm_output = 0.0;
    }
}
```

---

## 5. Implementation Checklist

### Phase 1: Research & Feasibility

- [ ] Document PID theory (equations, discrete forms, DrivaPi context)
- [ ] Measure encoder resolution and speed calculation accuracy
- [ ] Test motor response to PWM step input (no control loop) to understand dynamics
- [ ] Determine sampling rate (10 ms recommended for motor control)
- [ ] Design anti-windup strategy for integral term
- [ ] Plan test bench setup (isolated motor + encoder on test stand)
- [ ] Document team approval of architecture

### Phase 2: Implementation

- [ ] Implement PID struct in firmware/Core/motor_control.c
- [ ] Implement PID struct in rust/controller/
- [ ] Create speed measurement routine (encoder filtering, outlier rejection)
- [ ] Implement test script: ramp target speed from 0% → 50% → 100%
- [ ] Set up data logging (time, target, actual, error, PWM) for analysis
- [ ] Perform Ziegler-Nichols or manual tuning on test bench
- [ ] Record step response (rise time, overshoot, settling time)
- [ ] Test on vehicle under load (ensure stability on road)
- [ ] Document final PID gains in code and wiki

### Phase 3: Validation

- [ ] Test steady-state speed accuracy at multiple setpoints (25%, 50%, 75%, 100%)
- [ ] Verify acceleration smoothness (no jerking or oscillation)
- [ ] Measure max speed error under nominal load
- [ ] Test robustness to disturbances (weight, incline, friction changes)
- [ ] Validate on all 4 motors (left front, right front, left rear, right rear)

---

## 6. Testing & Measurement

### 6.1 Test Bench Setup

```
Motor + Gearbox → Encoder → Load (friction/wheel disc)
                     │
                     └─→ Counter (RPM measurement)
```

### 6.2 Data to Collect

| Parameter | Tool | Purpose |
|-----------|------|---------|
| Target Speed | Command input | Reference |
| Actual Speed | Encoder counter | Feedback |
| PWM Output | GPIO measurement | Control signal |
| Time | System clock | Correlation |
| Temperature | Thermal camera / sensor | Drift analysis |

### 6.3 Expected Results

| Metric | Target | Notes |
|--------|--------|-------|
| Rise Time (10% → 90%) | < 500 ms | Acceptable motor response speed |
| Overshoot | < 20% | Reasonable oscillation damping |
| Settling Time | < 1.5 s | Stability achieved quickly |
| Steady-State Error | < 5% | Speed tracking accuracy |
| Oscillation Freq | 1–3 Hz | If any (prefer none) |

---

## 7. Potential Issues & Solutions

| Issue | Cause | Solution |
|-------|-------|----------|
| Oscillation / ringing | K_p too high, K_d too low | Reduce K_p; increase K_d |
| Slow response | K_p too low | Increase K_p step-by-step |
| Steady-state error persists | K_i too low | Increase K_i gradually |
| Instability / divergence | K_d too high; encoder noise | Reduce K_d; add low-pass filter to speed measurement |
| Jerky acceleration | Dead zone mismatch; PWM resolution | Calibrate dead zone; increase resolution |
| Motor doesn't move | PWM below dead zone | Ensure PWM > minimum threshold |

---

## 8. References & Further Reading

- **Classical Control**: Franklin, Powell, Emami-Naeini. *Feedback Control of Dynamic Systems* (7th ed.)
- **PID Tuning**: Åström & Hägglund. *PID Controllers: Theory, Design, and Tuning (2nd ed.)*
- **Discrete-Time Control**: Digital Control Systems by Phillips & Nagle
- **Motor Control**: Texas Instruments *Motor Control Basics* application notes

---

## 9. Future Enhancements

1. **Adaptive Tuning**: Adjust K_p, K_i, K_d based on load or speed range
2. **Anti-Windup Refinement**: Improved clamping strategies
3. **Feedforward Control**: Add motor model to predict required PWM upfront
4. **State Estimation**: Use Kalman filter for more robust speed estimation
5. **Multi-Motor Coordination**: Synchronized speed control across all 4 motors

---

**Document Version**: 1.0  
**Last Updated**: March 2026  
**Owner**: Hardware & Control Team  
**Status**: In Development
