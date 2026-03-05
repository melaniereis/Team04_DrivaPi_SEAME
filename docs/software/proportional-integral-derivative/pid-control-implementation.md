# PID Control Loop Implementation for DC Motors

## Overview

This document outlines the design, theory, and implementation strategy for PID (proportional-integral-derivative) control loops to regulate DC motor speed on the DrivaPi vehicle. 

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
- Set gain_i = 0, gain_d = 0
- Choose modest gain_p (e.g., 50–100)
- Test step response: Set target speed suddenly, observe response
- Goal: Fast response, acceptable overshoot (~20%)

**Step 2: Add Integral (PI control)**
- Set gain_i small (e.g., gain_p / 10)
- Test steady-state error removal
- Increase gain_i until error is eliminated without oscillation
- If oscillation occurs, reduce gain_i

**Step 3: Add Derivative (PID)**
- Set gain_d small (e.g., gain_p × 0.01)
- Increase gain_d to reduce overshoot and oscillation
- Watch for noise amplification; if unstable, reduce gain_d
- Aim for critically damped response (fast, no oscillation)

**Empirical Starting Point:**
```
gain_p = 100  (adjust based on motor response speed)
gain_i = gain_p / 20 = 5  (slow integral action)
gain_d = gain_p × 0.05 = 5  (mild derivative action)
```

### 3.2 Practical Tuning Checklist

```
for each tuning iteration:
  1. Set target speed to 50% max speed
  2. Observe rise time (time to reach target)
  3. Measure overshoot (how much speed exceeds target)
  4. Measure settling time (time to stabilize)
  
  If oscillating: reduce gain_p or gain_i; increase gain_d
  If too slow: increase gain_p
  If steady-state error persists: increase gain_i
  If overshoot is excessive: increase gain_d or reduce gain_p
```

---

## 4. PID Architecture & Integration

### 4.1 DrivaPi Hardware Architecture

```
┌────────────────────────────────────┐
│  STM32 Microcontroller             │
│                                    │
│  ┌────────────────────────────┐    │
│  │  CAN Receiver              │    │
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
    └────────────┬─────────────┘
                 │ PWM signals 
                 ▼
    ┌──────────────────────────┐
    │  Motor Driver H-bridge   │
    │  (per motor)             │
    └────────────┬─────────────┘
                 │
                 ▼
    ┌──────────────────────────┐
    │  JGB37-520 DC Motor      │
    └────────────┬─────────────┘
                 │
                 ▼ (mechanical output)
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
    PID Controller
```

**Signal Flow:**
1. **Command:** Raspberry Pi 5 → CAN message → STM32 receives target speed
2. **Control:** PID calculates error and PWM term → I2C sends PWM value to PCA9685
3. **Motor:** PCA9685 outputs PWM signal → Motor Driver → Motor spins → Wheel rotates
4. **Feedback:** Wheel encoder → LM393 conditions pulse → STM32 timer counts → Speed calculation (m/s) → back to PID
5. **Loop:** PID continuously adjusts PWM to maintain target speed sent by remote

---

### 4.2 Integration Points

**File: `motor_control.h`**
Defines the `MotorPIDState` struct and the `MotorPIDUpdate()` function prototype. Initialize a global `g_motorPidState` instance with target speed, measured speed, and tuned PID gains (gain_p, gain_i, gain_d).

**File: `motor_control.c`**
Implements the PID control loop:
- `MotorPIDUpdate()` – Core PID calculation; call periodically (100 ms) from the main control thread
- `UpdateMotorControl()` – Main entry point; updates target speed from CAN and calls `MotorPIDUpdate()` with measured speed feedback

Call `UpdateMotorControl()` in your main control loop (e.g., from a 100 ms timer or periodic task) to continuously regulate motor speed.

---

### 4.3 Current State and Future Implementation

#### Current State (As of March 2026)

The STM32 currently receives PWM command values directly from the remote control via CAN messages (range: **-4095 to +4095**) and sends them directly to the motor driver. No speed feedback or PID control is implemented yet.

```
Remote Control → CAN (PWM: -4095 to +4095) → STM32 → Motor Driver → Motor
```

#### Future Implementation (PID Control - Phase 2)

To implement closed-loop speed control, the system will transition from receiving PWM commands to receiving **target speed commands** via CAN. At that time, the PID controller will:
- Accept target speed values (m/s) from the remote control
- Read encoder-based speed feedback from `speed_sensor.c`
- Calculate the required PWM signal using the PID algorithm
- Automatically drive the motor to match the target speed with smooth, stable behavior

**Key Changes Required for PID Implementation:**
1. CAN message format: change from transmitting PWM (-4095 to +4095) to transmitting target speed (m/s)
2. STM32 control logic: replace direct PWM forwarding with `UpdateMotorControl()` function
3. Add speed feedback via encoder and `speed_sensor.c`
4. Tune PID gains (gain_p, gain_i, gain_d) for smooth response

This phased approach allows validation of the motor and encoder hardware before adding closed-loop control complexity.

---
## 5. References & Further Reading

- **Classical Control**: Franklin, Powell, Emami-Naeini. *Feedback Control of Dynamic Systems* (7th ed.)
- **PID Tuning**: Åström & Hägglund. *PID Controllers: Theory, Design, and Tuning (2nd ed.)*
- **Discrete-Time Control**: Digital Control Systems by Phillips & Nagle
- **Motor Control**: Texas Instruments *Motor Control Basics* application notes