# Hardware Documentation

## Hardware Supported by the Original Report

- ESP32 microcontroller
- Camera connected to a laptop
- Laptop running Python/OpenCV/YOLOv13
- Red, yellow, and green LEDs
- Wi-Fi communication between the processing system and ESP32

The report does not preserve a complete bill of materials or the original GPIO wiring table.

## Reference GPIO Reconstruction

> This mapping is a **reference reconstruction**, not a recovered original pinout.

| Direction | Red | Yellow | Green |
| --- | ---: | ---: | ---: |
| North | GPIO 13 | GPIO 14 | GPIO 16 |
| East | GPIO 17 | GPIO 18 | GPIO 19 |
| South | GPIO 21 | GPIO 22 | GPIO 23 |
| West | GPIO 25 | GPIO 26 | GPIO 27 |

If the physical prototype is rebuilt, each LED should use an appropriate current-limiting resistor and the GPIO mapping should be adjusted to the actual wiring.
