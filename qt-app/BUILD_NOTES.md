# Build Notes

## Platform Compatibility

### macOS Build Limitations

The **kuksa_feeder** target requires Linux-specific SocketCAN headers (`<linux/can.h>`) and will **NOT** compile on macOS.

**On macOS:**
- ✅ Qt app (`myqtapp`) can build if QML resources are fixed
- ❌ CAN feeder (`kuksa_feeder`) **cannot** build (requires Linux kernel headers)

**Workaround for development:**
```bash
# Build only Qt app on macOS
cd build
cmake ..
make myqtapp
```

### Linux/RPi5 Build

Both targets build successfully on Linux with SocketCAN support:
```bash
cd build
cmake ..
make  # Builds both myqtapp and kuksa_feeder
```

## Current Build Issues

### 1. Missing QML File (Qt App)
```
make[2]: *** No rule to make target `resources/qml/components/SpeedDisplay.qml'
```

**Fix:** Check if SpeedDisplay.qml exists or remove from qml.qrc if obsolete

### 2. Linux Headers Missing (macOS only)
```
fatal error: 'linux/can.h' file not found
```

**Expected behavior** - feeder is Linux-only. Build on RPi5 instead.

## Deployment Target

This project is designed for **Raspberry Pi 5 with AGL Linux**:
- KUKSA databroker runs as systemd service
- CAN interface via SocketCAN (vcan0 or can0)
- Qt app displays dashboard
- Feeder daemon bridges CAN → KUKSA

## Next Steps

1. **Fix QML resources** in [qml.qrc](resources/qml.qrc)
2. **Deploy to RPi5** for full build validation
3. **Test feeder** with vcan0 as described in [src/feeder/README.md](src/feeder/README.md)
