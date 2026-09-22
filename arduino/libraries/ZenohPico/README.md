# Eclipse Zenoh-Pico Arduino Library for Coral Dev Board Micro

An Arduino-compatible client library for [Eclipse Zenoh-Pico](https://zenoh.io/) (v1.10.1) tailored for the **Google Coral Dev Board Micro** with the **Coral Wireless Add-on Board** (`coral_micro_wifi`).

Zenoh-Pico is an ultra-lightweight client implementation of Eclipse Zenoh designed for resource-constrained microcontrollers. This library integrates Zenoh-Pico directly into the Coral Micro FreeRTOS and lwIP networking stack on the ARM Cortex-M7 core with hardware floating-point support (`fpv5-d16`, `hard`).

---

## Features

- **Precompiled Architecture**: Built specifically for ARM Cortex-M7 (`-mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard`).
- **FreeRTOS Background Multithreading**: Utilizes FreeRTOS background executor tasks for asynchronous message handling and network I/O.
- **Full Zenoh Primitives**: Supports Publication, Subscription, Query, Queryable, and Liveliness over TCP links.
- **Arduino Ecosystem Ready**: Cleanly integrates with `arduino-cli` and the Arduino IDE without conflicting with Arduino Streams or Print methods.

---

## Hardware Requirements

1. [Coral Dev Board Micro](https://coral.ai/products/dev-board-micro)
2. [Coral Wireless Add-on Board](https://coral.ai/products/dev-board-micro-wireless) (attached via board headers)
3. USB Type-C cable connecting Coral Micro to host computer

---

## Quick Start Guide

### 1. Run a Zenoh Router on your Host Computer

Zenoh-Pico operates in **client mode**, connecting over TCP to a Zenoh router (`zenohd`) on your local network.

To start `zenohd` on your host PC:

#### Using Docker:
```bash
docker run --rm -it --net=host eclipse/zenoh:1.10.1
```

#### Or using Cargo / Native binary:
```bash
# If installed via cargo:
cargo install zenoh-cli
zenohd --listen tcp/0.0.0.0:7447
```

Note your computer's local IP address (e.g., `192.168.1.100`), which the Coral Micro will connect to.

---

### 2. Configure Example Sketch

Open [ZenohPublisher.ino](file:///opt/coralmicro/arduino/libraries/ZenohPico/examples/ZenohPublisher/ZenohPublisher.ino) (or [ZenohSubscriber.ino](file:///opt/coralmicro/arduino/libraries/ZenohPico/examples/ZenohSubscriber/ZenohSubscriber.ino)):

```cpp
// Wi-Fi Configuration
const char kSsid[] = "Your_WiFi_SSID";
const char kPsk[]  = "Your_WiFi_Password";

// Zenoh Router Locator (replace with your host PC IP)
const char kRouterLocator[] = "tcp/192.168.1.100:7447";
const char kTopicKeyexpr[]  = "demo/coralmicro/telemetry";
```

---

### 3. Compile and Flash

#### Using `arduino-cli`:

Compile the publisher sketch for Coral Micro Wi-Fi:
```bash
arduino-cli compile --fqbn coral:coral_micro:coral_micro_wifi \
  /opt/coralmicro/arduino/libraries/ZenohPico/examples/ZenohPublisher/ZenohPublisher.ino
```

Flash to the Coral Micro:
```bash
arduino-cli upload -p /dev/ttyACM0 --fqbn coral:coral_micro:coral_micro_wifi \
  /opt/coralmicro/arduino/libraries/ZenohPico/examples/ZenohPublisher/ZenohPublisher.ino
```

---

### 4. Verify Messages

#### Monitoring Published Data:
On your host PC, listen for incoming messages from the Coral Micro:

```bash
# Using Zenoh CLI
z_sub "demo/coralmicro/**"
```

Or using Python with `eclipse-zenoh`:
```python
import zenoh

session = zenoh.open()
sub = session.declare_subscriber('demo/coralmicro/**', lambda sample: print(
    f"Received on {sample.key_expr}: {sample.payload.to_string()}"
))
input("Press Enter to stop...\n")
```

Output:
```text
Received on demo/coralmicro/telemetry: {"seq":1,"uptime_ms":1024,"device":"coral_micro_wifi"}
Received on demo/coralmicro/telemetry: {"seq":2,"uptime_ms":2025,"device":"coral_micro_wifi"}
...
```

#### Testing Subscriber:
If running [ZenohSubscriber.ino](file:///opt/coralmicro/arduino/libraries/ZenohPico/examples/ZenohSubscriber/ZenohSubscriber.ino), send a command to toggle the Coral Micro User LED:
```bash
z_pub "demo/coralmicro/command" "toggle"
```

---

## Library Structure

```
ZenohPico/
├── library.properties
├── README.md
├── src/
│   ├── ZenohPico.h             <-- Main convenience wrapper for Arduino sketches
│   ├── zenoh-pico.h            <-- Core Zenoh-Pico C API header
│   ├── zenoh-pico/
│   │   ├── config.h            <-- Board-specific Zenoh configuration
│   │   └── ...                 <-- Zenoh subheaders
│   └── cortex-m7/
│       └── fpv5-d16-hard/
│           └── libzenoh_pico.a <-- Precompiled Cortex-M7 static library
├── examples/
│   ├── ZenohPublisher/
│   │   └── ZenohPublisher.ino
│   └── ZenohSubscriber/
│       └── ZenohSubscriber.ino
└── extras/
    └── library_generation/
        └── library_generation.sh
```

---

## Rebuilding the Static Library Archive

If you modify Zenoh-Pico sources, patch configuration, or update FreeRTOS/lwIP parameters, you can regenerate the precompiled static library:

```bash
./rebuild_zenoh_pico_arduino_lib.sh
```

This compiles `libs_zenoh_pico` with the Coral Micro toolchain and copies the new archive and headers into the Arduino library.
