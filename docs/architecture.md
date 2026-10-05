# System Architecture

The original project report describes a camera-based traffic monitoring system in which a laptop performs the image-processing work and an ESP32 controls the physical traffic-light LEDs.

```text
Camera
  |
  | real-time video/image
  v
Laptop
  |- Python
  |- OpenCV
  `- YOLOv13 vehicle detection
  |
  | traffic-density decision
  | green-light duration
  | wireless instruction
  v
ESP32
  |
  | actuator output
  v
Red / Yellow / Green traffic-light LEDs
```

## Component Responsibilities

### Camera
Provides the live image/video input used for vehicle detection.

### Laptop / Processing Layer
The report assigns the laptop the central processing role and lists Python, OpenCV, and YOLOv13. It detects/counts vehicles, determines traffic density and timing, and sends the resulting instruction to the ESP32.

### ESP32 / Actuator Layer
The ESP32 is documented as the command receiver and actuator controller. It receives control data over Wi-Fi and switches the corresponding traffic-light LEDs.

### Traffic-Light LEDs
Red, yellow, and green LEDs represent stop, transition/caution, and go states.

## Reconstruction Boundary

The original report does not preserve the exact network transport, packet syntax, ESP32 GPIO assignments, original ESP32 source code, or complete YOLO inference code. Those details are therefore treated as reconstruction choices, not recovered historical implementation.
