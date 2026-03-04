# PID Control Loop Implementation for DC Motors

## Overview

This document outlines the design, theory, and implementation strategy for PID (proportional-integral-derivative) control loops to regulate DC motor speed on the DrivaPi vehicle. 

**Current Implementation (Remote Control):** The PID controller maintains stable, smooth motor speed when the remote control sends target speed commands via CAN messages to the STM32.

**Future Implementation (Autonomous Driving):** The same CAN messaging infrastructure and PID control architecture will be reused when autonomous driving is implemented. At that time, instead of the remote, AI image processing will analyze camera input and send target speed commands via CAN to the STM32. The PID loop implementation remains unchanged—only the source of the CAN messages switches from remote to AI processor.

The implementation ensures smooth acceleration, accurate speed tracking, and stable steady-state behavior for both remote-controlled and autonomous operation.

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

---

## 2. DC Motor Characteristics for DrivaPi

### 2.1 Motor Specifications

| Parameter | Value | Notes |
|-----------|-------|-------|
| Model | JGB37-520 DC | Geared DC motor |
| Type | DC brushed motor with integral gearbox | Compact, high-torque |
| Voltage | 12V | Powered from vehicle battery |
| Idle Speed | ~740 RPM | No-load speed at 12V, motor output |
| Gear Ratio | 10:1 | Reduces speed, increases torque |
| Wheel Speed (est.) | ~74 RPM | At 10:1 reduction |
| Max Current | ~200–300 mA | Short-term peak with load |
| Encoder Resolution | 30 PPR | Separate wheel encoder (not integrated in motor) |

### 2.2 Speed Measurement

**Implementation:** Speed is calculated in the firmware (`firmware/Core/Src/speed_sensor.c`) using encoder pulses from a separate wheel encoder, conditioned by an LM393 comparator, and counted by the STM32 hardware timer.

**Encoder-Based Speed Calculation (meters per second):**
```c
delta_pulses = current_encoder_count - last_encoder_count;
// Handle timer overflow
if (delta_pulses > TIMER_PERIOD / 2)
    delta_pulses -= (TIMER_PERIOD + 1);
else if (delta_pulses < -TIMER_PERIOD / 2)
    delta_pulses += (TIMER_PERIOD + 1);

rotations = delta_pulses / PULSES_PER_REV;
distance_m = rotations * WHEEL_PERIMETER_M;
speed_mps = distance_m / dt;  // dt is time interval in seconds
```

**Example Calculation (per firmware constants):**
- Sampling interval: 100 ms (0.1 s)
- Encoder: 30 PPR (pulses per revolution)
- Wheel perimeter: 0.212 m
- If 15 encoder pulses counted in 100 ms:
  - Rotations = 15 / 30 = 0.5 rotations
  - Distance = 0.5 × 0.212 m = 0.106 m
  - Speed = 0.106 m / 0.1 s = 1.06 m/s (≈3.8 km/h)

### 2.3 Motor Dynamics

**Non-linearities to consider:**
1. **Dead Zone**: Motor doesn't move below a minimum PWM (typically 20–30% of max)
2. **Saturation**: PWM capped at 4095 (100%)
3. **Inertia**: Speed doesn't change instantaneously, the acceleration is gradual
4. **Load Variation**: Vehicle weight distribution and friction may affect motor load
5. **Thermal Effects**: Motor efficiency changes with temperature

---

## 3. Tuning Methods

### 3.1 Manual Tuning (Recommended for DrivaPi)

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

### 3.2 Practical Tuning Checklist

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

### 4.1 DrivaPi Hardware Architecture

```
┌─────────────────────────────────────┐
│  STM32 Microcontroller              │
│                                     │
│  ┌────────────────────────────┐    │
│  │  CAN Receiver              │    │
│  │  (from remote control)     │    │
│  └────────────┬───────────────┘    │
│               │                    │
│               ▼ (target speed)     │
│  ┌────────────────────────────┐    │
│  │  PID Controller            │    │
│  │  (motor_control.c)         │    │
│  └────────────┬───────────────┘    │
│               │                    │
│               ▼                    │
│  ┌────────────────────────────┐    │
│  │  I2C Master                │    │
│  │  (to PCA9685)              │    │
│  └────────────┬───────────────┘    │
│               │                    │
└───────────────┼────────────────────┘
                │ I2C Bus
                ▼
    ┌──────────────────────────┐
    │  PCA9685 PWM Driver      │
    │  (16-channel PWM)        │
    └────────────┬─────────────┘
                 │ PWM signals (4 channels for 4 motors)
                 ▼
    ┌──────────────────────────┐
    │  Motor Driver H-bridge   │
    │  (per motor)             │
    └────────────┬─────────────┘
                 │
                 ▼
    ┌──────────────────────────┐
    │  JGB37-520 DC Motor      │
    │  (no encoder integrated) │
    └────────────┬─────────────┘
                 │
                 ▼ (mechanical output)
    ┌──────────────────────────┐
    │  Wheel with Encoder      │
    │  (optical/magnetic)      │
    │  Produces pulse signal   │
    └────────────┬─────────────┘
                 │
                 ▼
    ┌──────────────────────────┐
    │  LM393 Comparator        │
    │  (pulse conditioning)    │
    └────────────┬─────────────┘
                 │ Digital pulse via GPIO
                 │
┌────────────────┘
│
▼
┌──────────────────────────────────┐
│  STM32 Timer (TIM1)              │
│  GPIO Input Capture              │
│  (counts encoder pulses)         │
└──────────────────────────────────┘
         │
         ▼ (feedback)
    ┌────────────────┐
    │ Speed Sensor   │
    │ (speed_sensor.c)
    │ Calculates m/s │
    └────────────────┘
         │
         ▼ (feedback loop)
    PID Controller (update at 100 ms)
```

**Signal Flow:**
1. **Command:** Remote → CAN message → STM32 receives target speed
2. **Control:** PID calculates error and PWM term → I2C sends PWM value to PCA9685
3. **Motor:** PCA9685 outputs PWM signal → Motor Driver → Motor spins → Wheel rotates
4. **Feedback:** Wheel encoder → LM393 conditions pulse → STM32 timer counts → Speed calculation (m/s) → back to PID
5. **Loop:** PID continuously adjusts PWM to maintain target speed sent by remote

---

### 4.2 Integration Points

#### C Firmware (`firmware/Core/`)

**File: `motor_control.h`**
```c
/**
 * @struct MotorPIDState
 * @brief PID controller state for one motor
 */
typedef struct {
    float       target_speed;      // desired speed (m/s)
    float       current_speed;     // measured speed (m/s) from speed_sensor.c
    float       error;             // e[n]
    float       error_prev;        // e[n-1] for derivative
    float       integral;          // sum of errors for integral term
    
    float       kp, ki, kd;        // PID gains
    float       pwm_output;        // computed PWM (0.0–1.0)
    uint16_t    pwm_raw;        // raw PWM (0–4095) for PCA9685
    uint8_t     pca9685_channel; // which PCA9685 channel (0–15)
} MotorPIDState;

/**
 * @brief Compute PID control output and send to PCA9685
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

**File: `motor_control.c` – PID Update Function**
```c
#define PID_SAMPLE_TIME 0.1f   // 100 ms update rate (matches SpeedSensor thread)
#define PWM_MIN 300u           // Minimum PWM to overcome dead zone
#define PWM_MAX 4095u          // Maximum PWM value

void MotorPID_Update(MotorPIDState *state, float current_speed)
{
    // Step 1: Calculate error (difference between target and current speed)
    state->error = state->target_speed - current_speed;
    
    // Step 2: Proportional term (immediate response)
    float p_term = state->kp * state->error;
    
    // Step 3: Integral term (eliminates steady-state error)
    state->integral += state->error * PID_SAMPLE_TIME;
    
    // Anti-windup: prevent integral from growing too large
    if (state->integral > 100.0f) 
        state->integral = 100.0f;
    if (state->integral < -100.0f) 
        state->integral = -100.0f;
    
    float i_term = state->ki * state->integral;
    
    // Step 4: Derivative term (reduces overshoot)
    float derivative = (state->error - state->error_prev) / PID_SAMPLE_TIME;
    float d_term = state->kd * derivative;
    
    // Step 5: Sum all three terms
    state->pwm_output = p_term + i_term + d_term;
    
    // Step 6: Clamp output to valid PWM range [0.0, 1.0]
    if (state->pwm_output > 1.0f) 
        state->pwm_output = 1.0f;
    if (state->pwm_output < 0.0f) 
        state->pwm_output = 0.0f;
    
    // Step 7: Convert normalized PWM (0.0–1.0) to raw value (0–4095)
    state->pwm_raw = (uint16_t)(state->pwm_output * 4095.0f);
    
    // Step 8: Apply dead zone minimum (motor won't move below this PWM)
    if (state->pwm_raw > 0 && state->pwm_raw < PWM_MIN)
        state->pwm_raw = PWM_MIN;
    
    // Step 9: Send PWM to PCA9685 via I2C
    // Example: PCA9685_SetPWM(state->pca9685_channel, 0, state->pwm_raw);
    PCA9685_SetPWM(state->pca9685_channel, 0, state->pwm_raw);
    
    // Step 10: Store current error for derivative calculation in next cycle
    state->error_prev = state->error;
    state->current_speed = current_speed;
}
```

**Integration with Speed Sensor and CAN:**
```c
// In your main control loop (100 ms interval, matches SpeedSensor thread):
extern float    g_vehicleSpeed;         // from speed_sensor.c (measured m/s)
extern float    g_target_speed;         // from CAN message (remote command m/s)
extern          MotorPIDState motor_pid_states[4];  // for 4 motors

void UpdateMotorControl(void)
{
    // CAN receiver has already populated g_target_speed from remote command
    
    // Update each motor's PID controller with current speed feedback
    // and target speed from remote
    for (int i = 0; i < 4; i++) {
        motor_pid_states[i].target_speed = g_target_speed;
        MotorPID_Update(&motor_pid_states[i], g_vehicleSpeed);
    }
}
```

**Quick Reference – Helper Functions:**
```c
// Initialize gains for the PID controller
void MotorPID_SetGains(MotorPIDState *state, float kp, float ki, float kd)
{
    state->kp = kp;
    state->ki = ki;
    state->kd = kd;
}

// Reset integrator when starting a new motion command
void MotorPID_Reset(MotorPIDState *state)
{
    state->error = 0.0f;
    state->error_prev = 0.0f;
    state->integral = 0.0f;
    state->pwm_output = 0.0f;
}

// Set target speed for the controller
void MotorPID_SetTargetSpeed(MotorPIDState *state, float target_mps)
{
    state->target_speed = target_mps;
}
```
---

## 5. References & Further Reading

- **Classical Control**: Franklin, Powell, Emami-Naeini. *Feedback Control of Dynamic Systems* (7th ed.)
- **PID Tuning**: Åström & Hägglund. *PID Controllers: Theory, Design, and Tuning (2nd ed.)*
- **Discrete-Time Control**: Digital Control Systems by Phillips & Nagle
- **Motor Control**: Texas Instruments *Motor Control Basics* application notes