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

## KUKSA CLI Flags (Qt App)

The Qt app supports TLS and authorization options for connecting to the KUKSA databroker.

Flags:
- `--kuksa`: Enable KUKSA mode (otherwise CAN mode)
- `--kuksa-addr <host:port>`: Databroker address (default `localhost:55555`)
- `--kuksa-tls` / `--kuksa-insecure`: Use TLS or insecure mode (default insecure)
- `--kuksa-ca <path>`: Root CA certificate path
- `--kuksa-cert <path>`: Client certificate (mTLS)
- `--kuksa-key <path>`: Client private key (mTLS)
- `--kuksa-token <jwt>`: Authorization token (adds `Authorization: Bearer <jwt>`)

Examples:

```bash
# Insecure (default)
./myqtapp --kuksa --kuksa-addr localhost:55555

# TLS with Root CA
./myqtapp --kuksa --kuksa-addr kuksa.local:55555 \
	--kuksa-tls --kuksa-ca /etc/kuksa/ca.crt

# mTLS with JWT
./myqtapp --kuksa --kuksa-addr kuksa.local:55555 \
	--kuksa-tls --kuksa-ca /etc/kuksa/ca.crt \
	--kuksa-cert /etc/kuksa/client.crt --kuksa-key /etc/kuksa/client.key \
	--kuksa-token eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...
```

## Feeder Security Flags

The feeder supports the same TLS/auth features via flags:

- `--tls` / `--insecure`
- `--ca <path>`
- `--cert <path>`
- `--key <path>`
- `--token <jwt>`

Example:

```bash
./kuksa_feeder vcan0 kuksa.local:55555 --tls --ca /etc/kuksa/ca.crt \
	--cert /etc/kuksa/client.crt --key /etc/kuksa/client.key \
	--token eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...
```
