# Smart Traffic Light System Using YOLOv13

Adaptive traffic-light prototype that connects YOLOv13-based vehicle-density analysis to an ESP32-controlled physical traffic-light model.

> **Portfolio reconstruction:** This repository documents a university team project originally developed in 2025. The original development repository was not preserved. The architecture, prototype evidence, and test descriptions come from the original report; the firmware and Python communication utility here were reconstructed in 2026 from the documented system behavior. See [Reconstruction Notes](docs/reconstruction-notes.md).

## Project Overview

The documented system works as follows:

1. A camera captures traffic conditions.
2. A laptop runs Python, OpenCV, and YOLOv13 to detect/count vehicles.
3. The traffic condition is translated into a green-light timing decision.
4. A control instruction is sent wirelessly to the ESP32.
5. The ESP32 drives red, yellow, and green LEDs representing the traffic lights.

## My Role

**Embedded Systems and Hardware Developer**

- Developed ESP32-side control logic for the traffic-light LEDs.
- Wired multi-direction red, yellow, and green LED signals.
- Integrated the ESP32 with wireless commands from the laptop-side traffic analysis.
- Verified actuator response to timing instructions derived from vehicle-density analysis.
- Tested low, medium, and high traffic scenarios and checked wireless-control behavior.

This repository intentionally does **not** claim ownership of the complete YOLO training/inference implementation.

## Architecture

```text
Camera / Webcam
      |
      v
+---------------------------+
| Laptop                    |
| Python + OpenCV + YOLOv13 |
| vehicle detection/count   |
+-------------+-------------+
              |
              | timing command over Wi-Fi
              v
      +---------------+
      | ESP32         |
      | LED controller|
      +-------+-------+
              |
              v
 Red / Yellow / Green LEDs
 North / East / South / West
```

More detail: [docs/architecture.md](docs/architecture.md)

## Repository Structure

```text
smart-traffic-light-yolov13/
├── README.md
├── .gitignore
├── firmware/esp32/
│   ├── traffic_light_controller.ino
│   └── config.example.h
├── software/command_sender/
│   └── traffic_light_client.py
├── docs/
│   ├── architecture.md
│   ├── hardware.md
│   ├── communication-protocol.md
│   ├── testing.md
│   └── reconstruction-notes.md
└── assets/
```

## Documented Functional Tests

| Scenario | Example in report | Green duration reported |
| --- | ---: | ---: |
| Low / default | 2 detected vehicles | 20 s |
| Medium | 5 detected vehicles | 30 s |
| High / busy test | 6 detected vehicles in the shown test | 40 s |

The report contains inconsistent threshold/duration wording between its testing and conclusion sections, so the reconstructed code does not infer duration from vehicle-count thresholds. See [docs/testing.md](docs/testing.md).

## ESP32 Firmware

The reconstructed controller exposes a small Wi-Fi TCP interface. It receives a direction and requested green duration, performs an all-red safety transition, activates the selected direction, switches to yellow, and returns to red.

The exact original GPIO assignments and network packet format were not preserved, so these are explicitly marked as reconstruction choices.

```bash
cd firmware/esp32
cp config.example.h config.h
```

Edit `config.h` with your local Wi-Fi credentials, then upload `traffic_light_controller.ino` to an ESP32.

## Python Command Sender

```bash
python software/command_sender/traffic_light_client.py \
  --host 192.168.1.50 \
  --direction north \
  --duration 20
```

Demo scenario aliases are also available:

```bash
python software/command_sender/traffic_light_client.py \
  --host 192.168.1.50 \
  --direction west \
  --scenario medium
```

Use `--dry-run` to inspect the outgoing command without an ESP32 connected.

## Reconstructed Communication Protocol

```text
PHASE,<DIRECTION>,<GREEN_DURATION_MS>\n
```

Example:

```text
PHASE,NORTH,20000
```

See [docs/communication-protocol.md](docs/communication-protocol.md).

## Tech Stack

- ESP32
- C / C++ (Arduino framework)
- Wi-Fi
- Python
- OpenCV
- YOLOv13 (as documented in the original project report)

## Academic Context

University team project for the Internet Engineering Technology program, Vocational College, Universitas Gadjah Mada. Original project period: 2025.
