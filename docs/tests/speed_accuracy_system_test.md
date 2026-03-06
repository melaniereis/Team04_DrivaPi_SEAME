# Speed Accuracy System Test

**Related Issue:** [#396 — System Test Validation](https://github.com/SEAME-pt/Team04_DrivaPi/issues/396)

**Objective:** Verify that the speed shown on the HMI display matches the real physical speed of the vehicle.

**Acceptance Criterion:** Speed deviation is within tolerance (< 5%).

---

## Test Setup

| Parameter | Value |
|---|---|
| Fixed distance | 2 m |
| Measurement tool | Stopwatch (precision: 0.1 s) |
| Number of speed samples | 10 |

### Distance Evidence

<img width="500" alt="Distance measurement - start point" src="REPLACE_WITH_GITHUB_ASSET_URL_1"/>

<img width="500" alt="Distance measurement - end point" src="REPLACE_WITH_GITHUB_ASSET_URL_2"/>

---

## Raw Measurements

### Display Speed Samples

| Sample Group | Speed (m/s) | Count |
|---|---|---|
| Group A | 1.0176 | × 6 |
| Group B | 1.04587 | × 2 |
| Group C | 1.03173 | × 2 |

**Average display speed:**
```
(1.0176 × 6) + (1.04587 × 2) + (1.03173 × 2) = 10.2608
10.2608 / 10 = 1.02608 m/s
```

### Physical Speed (Stopwatch)

```
Time measured = 1.9 s
Physical speed = Distance / Time = 2 / 1.9 ≈ 1.0526 m/s
```

---

## Results

| Metric | Value |
|---|---|
| Average display speed | 1.02608 m/s |
| Physical speed (stopwatch) | 1.0526 m/s |
| Absolute deviation | 0.02655 m/s |
| **Relative deviation** | **≈ 2.52 %** |
| Tolerance threshold | < 5 % |
| **Status** | ✅ **PASS** |

### Cross-check

Calculating expected time from average display speed:
```
Time = Distance / Speed = 2 / 1.02608 ≈ 1.949 s
```
This is very close to the measured 1.9 s, confirming consistency between display and physical measurements.

---

## Notes

- Stopwatch precision is limited to 0.1 s, which is the main source of measurement uncertainty.
- The real physical speed is likely closer to the peak display speed (≈ 1.047 m/s), which would reduce the deviation further.
- Tests were performed in a controlled environment as required.
