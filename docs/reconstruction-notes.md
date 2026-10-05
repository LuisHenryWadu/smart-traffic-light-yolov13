# Reconstruction Notes

## Purpose

This repository is a portfolio reconstruction of a university project originally completed in 2025. The original report was preserved, but the original source-code repository was not.

The goal is to document the work accurately without presenting newly written files as recovered historical source code.

## Preserved Project Information

Derived from the original report:

- Camera -> laptop -> ESP32 -> LED architecture
- Python, OpenCV, YOLOv13, ESP32, and Wi-Fi as documented technologies/components
- ESP32 role as command receiver and physical LED controller
- North/East/South/West direction-processing concept
- Vehicle-detection test descriptions
- Physical prototype evidence
- Recorded functional-test descriptions

## Reconstructed in 2026

Newly written for portfolio/documentation purposes:

- `firmware/esp32/traffic_light_controller.ino`
- `firmware/esp32/config.example.h`
- `software/command_sender/traffic_light_client.py`
- Markdown documentation
- Repository structure and `.gitignore`

Reconstruction choices include:

- Exact ESP32 GPIO numbers
- TCP as the transport
- Port `8080`
- `PHASE,<DIRECTION>,<DURATION_MS>` message format
- One-second all-red transition
- Three-second yellow transition

## Scope of Portfolio Claim

The repository supports an **Embedded Systems and Hardware Developer** portfolio claim: ESP32 actuator control, multi-direction LED integration, wireless control integration, and functional response testing.

It intentionally avoids claiming that the reconstructed Python client is the original YOLOv13 inference pipeline or that the portfolio owner authored every part of the team project's computer-vision stack.
