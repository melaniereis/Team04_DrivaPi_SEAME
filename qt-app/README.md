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

# Specify CAN interface and address
./kuksa_feeder can0 192.168.1.100:55555

# With TLS and JWT
./kuksa_feeder can0 localhost:55555 \
    --tls \
    --ca /path/to/ca.crt \
    --cert /path/to/client.crt \
    --key /path/to/client.key \
    --token <jwt_token>

---
```

## Configuration

On Raspberry Pi:

```bash
# Create VSS directory and schema
mkdir -p /etc/kuksa
cp vss_v6.json /etc/kuksa/vss.json
chown -R kuksa:kuksa /etc/kuksa
chmod 644 /etc/kuksa/vss.json

# Configure service
cat > /etc/default/kuksa-databroker <<EOF
EXTRA_ARGS="--address 0.0.0.0 --port 55555 --vss /etc/kuksa/vss.json --insecure --disable-authorization"
EOF

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

