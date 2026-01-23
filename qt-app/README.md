# qt-app: Qt Dashboard Application

## Overview

Qt6/QML automotive dashboard for the DrivaPi project with dual-mode operation: direct CAN bus reading or KUKSA databroker integration via gRPC.

**Platform:** Raspberry Pi 5  
**Target:** AGL (Automotive Grade Linux)  
**Standards:** VSS v6.0, KUKSA VAL v2

---

## Architecture

```
STM32 (CAN) → Raspberry Pi 5 → kuksa_feeder → KUKSA Databroker → Qt Dashboard
                                   (CAN→VSS)      (localhost:55555)     (myqtapp)
```

---

## Features

- **Dashboard UI**: Speed, battery, energy, gear, temperature, distance, autonomous mode
- **CAN Mode**: Direct SocketCAN reading (default, low-latency)
- **KUKSA Mode**: gRPC subscriber with TLS/mTLS/JWT support
- **Feeder Daemon**: CAN-to-VSS bridge (`kuksa_feeder`)

---

## Directory Structure

```
qt-app/
├── CMakeLists.txt              # Build configuration
├── cross_compile.sh            # Native Pi build script
├── test_feeder.sh              # Feeder test harness
├── vss_v6.json                 # Official VSS v6.0 schema
├── inc/                        # Header files
│   ├── vehicle_data.hpp        # Qt data model
│   ├── can_reader.hpp          # SocketCAN reader
│   └── kuksa_reader.hpp        # KUKSA gRPC subscriber
├── src/                        # Implementation
│   ├── main.cpp                # Application entry point
│   ├── vehicle_data.cpp        # Data model
│   ├── can_reader.cpp          # CAN reader
│   ├── kuksa_reader.cpp        # KUKSA subscriber
│   └── feeder/                 # CAN feeder daemon
│       ├── main.cpp            # Feeder entry point
│       ├── publisher.cpp/hpp   # KUKSA VAL v2 publisher
│       ├── handlers.cpp/hpp    # CAN frame handlers
│       ├── signals.hpp         # VSS path constants
│       ├── can_decode.hpp      # CAN payload decoders
│       └── can_ids.hpp         # CAN ID definitions
├── resources/
│   ├── qml/                    # QML UI components
│   │   ├── main.qml
│   │   └── components/         # Reusable components
│   └── assets/                 # Images, fonts, icons
└── proto/                      # Protocol definitions
    └── kuksa/val/v2/           # KUKSA VAL v2 API
```

---

## Building

### Prerequisites

- **Qt 6.x** (Core, Gui, Quick, Qml, QuickControls2, SerialBus)
- **gRPC** (C++ library + protoc plugin)
- **Protobuf** (Compiler and libraries)
- **Abseil** (Google Abseil C++ library, required by gRPC)
- **CMake** 3.16+
- **C++17 compiler** (GCC 8+, Clang 7+)

### Build Steps
```bash
cd qt-app
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel

# Binaries produced:
# - ./myqtapp         (Qt Dashboard)
# - ./kuksa_feeder    (CAN Feeder)
```

### Native Build on Raspberry Pi

```bash
cd qt-app
./cross_compile.sh
```

---

## Usage

### KUKSA Mode (Default)
```bash
# Basic insecure connection
./myqtapp --kuksa --kuksa-addr localhost:55555

# With TLS
./myqtapp --kuksa \
    --kuksa-tls \
    --kuksa-addr localhost:55555 \
    --kuksa-ca /path/to/ca.crt

# With mTLS + JWT
./myqtapp --kuksa \
    --kuksa-tls \
    --kuksa-addr localhost:55555 \
    --kuksa-ca /path/to/ca.crt \
    --kuksa-cert /path/to/client.crt \
    --kuksa-key /path/to/client.key \
    --kuksa-token <jwt_token>
```

### CAN Mode (Fallback / Dev)
Requires building with `-DENABLE_CAN_MODE=ON`.

```bash
# Bring up CAN interface
sudo ip link set can0 type can bitrate 500000
sudo ip link set can0 up

# Run dashboard in CAN mode on can0
./myqtapp --can --can-if can0
```

### KUKSA Feeder
```bash
# Start with defaults (vcan0, localhost:55555, insecure)
./kuksa_feeder

# Specify CAN interface and address (positional)
./kuksa_feeder can0 192.168.1.100:55555

# Or use named flags
./kuksa_feeder --can-if can0 --address 192.168.1.100:55555

# With TLS and mTLS
./kuksa_feeder --tls \
    --ca /path/to/ca.crt \
    --cert /path/to/client.crt \
    --key /path/to/client.key

# With JWT authorization
./kuksa_feeder --tls \
    --ca /path/to/ca.crt \
    --token <jwt_token>
```

**CLI Options:**
- `--tls` - Enable TLS encryption
- `--ca <path>` - Root CA certificate for server verification
- `--cert <path>` - Client certificate for mTLS
- `--key <path>` - Client private key for mTLS
- `--token <jwt>` - JWT bearer token for authorization
- `--can-if <name>` - CAN interface name (default: vcan0)
- `--address <addr>` - KUKSA databroker host:port (default: localhost:55555)
- `--insecure` - Force insecure mode (default)

---

## Configuration

### TLS Certificates

For production deployments, generate TLS certificates:

```bash
# Generate CA key and certificate
openssl genrsa -out ca.key 2048
openssl req -new -x509 -days 365 -key ca.key -out ca.crt -subj "/CN=KUKSA-CA"

# Generate server key and CSR
openssl genrsa -out server.key 2048
openssl req -new -key server.key -out server.csr -subj "/CN=localhost"

# Create extension file for subjectAltName
cat > ext.cnf <<EOF
subjectAltName = DNS:localhost,IP:127.0.0.1
EOF

# Sign server certificate
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial \
    -out server.crt -days 365 -extfile ext.cnf

# Generate client key and certificate (for mTLS)
openssl genrsa -out client.key 2048
openssl req -new -key client.key -out client.csr -subj "/CN=client"
openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial \
    -out client.crt -days 365
```

### Databroker Setup

On Raspberry Pi:

```bash
# Create VSS directory and schema
mkdir -p /etc/kuksa
cp vss_v6.json /etc/kuksa/vss.json
chown -R kuksa:kuksa /etc/kuksa
chmod 644 /etc/kuksa/vss.json

# Configure service (production: with TLS/authorization)
cat > /etc/default/kuksa-databroker <<EOF
EXTRA_ARGS="--address 0.0.0.0 --port 55555 --vss /etc/kuksa/vss.json --tls-cert /etc/kuksa/server.crt --tls-private-key /etc/kuksa/server.key"
EOF

# For development/testing ONLY (no transport security or access control):
# WARNING: The configuration below exposes all vehicle signals over unencrypted,
# unauthenticated gRPC. This allows any process on the local network to read/actuate
# signals without credentials. Use this ONLY in isolated test environments.
#
# cat > /etc/default/kuksa-databroker <<EOF
# EXTRA_ARGS="--address 0.0.0.0 --port 55555 --vss /etc/kuksa/vss.json --insecure --disable-authorization"
# EOF

# Restart
systemctl restart kuksa-databroker
systemctl status kuksa-databroker

### VSS Signal Mappings

| VSS Path | Type | Unit | Description |
|----------|------|------|-------------|
| `Vehicle.Speed` | float | km/h | Vehicle speed |
| `Vehicle.Powertrain.TractionBattery.StateOfCharge` | int | % | Battery SoC |
| `Vehicle.Powertrain.TractionBattery.NetCapacity` | double | kWh | Energy capacity |
| `Vehicle.Powertrain.Transmission.CurrentGear` | int8 | - | Current gear |

### CAN Frame Format

| CAN ID | DLC | Payload | VSS Path | Handler |
|--------|-----|---------|----------|---------|
| 0x100 | 4 | float32 (LE) m/s | `Vehicle.Speed` | `HandleSpeed0x100()` |

---
```

## Testing
### Automated Test

```bash
./test_feeder.sh
```

### Manual CAN Testing

```bash
# Install can-utils
sudo apt-get install can-utils

# Send test CAN frame (5.3 m/s = 19.08 km/h)
cansend vcan0 100#CDCC28410000FF00

# Monitor CAN traffic
candump vcan0

# Query databroker
databroker-cli
> get Vehicle.Speed

---
```

## Troubleshooting

**CAN interface not found**
sudo ip link set can0 type can bitrate 500000
```bash
sudo ip link set can0 up
```

**Databroker connection refused**
```bash
systemctl status kuksa-databroker
journalctl -u kuksa-databroker -n 50
```

**Path not found error**
- Ensure signal exists in `/etc/kuksa/vss.json`
- Restart databroker after schema changes
- Check path syntax matches VSS exactly (case-sensitive)

**Missing gRPC plugin**
```bash
sudo apt-get install protobuf-compiler-grpc
```

**Qt6 not found**
```bash
find /usr -name Qt6Config.cmake
# Set CMAKE_PREFIX_PATH to parent directory
```

---

## Documentation

- **[Feeder Details](src/feeder/README.md)** - CAN feeder daemon documentation
- **[Implementation Review](../docs/software/qt/implementation_review.md)** - Design decisions and rationale
- **[KUKSA Integration](../docs/software/agl/kuksa_integration.md)** - KUKSA middleware setup
- **[Test Validation](../docs/data-transfer/KUKSA/test_validation_procedure.md)** - Testing procedures

---

## Standards & Compliance

- **VSS**: COVESA Vehicle Signal Specification v6.0
- **KUKSA**: Eclipse KUKSA.val VAL v2 API
- **ISO 26262**: Automotive functional safety
- **CAN**: ISO 11898 (CAN 2.0B)

