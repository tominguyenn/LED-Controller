# LED Controller with LCD & Potentiometer (Arduino Nano ESP32)

This project controls three LEDs using an **Arduino Nano ESP32**.  
It features:
- A button to cycle between LEDs (Blue → Red → Green).
- A potentiometer to adjust the brightness of the selected LED in **real-time**.
- A 20x4 I2C LCD to display the current LED, the next LED, and brightness percentage.

---

## Features
- Real-time brightness control with **PWM (LEDC)**.
- LCD feedback showing:
  - Current LED color
  - Next LED color
  - Brightness percentage
  - Button instructions
- Debounced button handling.
- Modular and extensible (future Wi-Fi/mobile app control planned).

---

## Hardware Requirements
- Arduino Nano ESP32
- 3 × LEDs (Blue, Red, Green)
- 3 × 220Ω resistors
- 1 × Potentiometer (10kΩ recommended)
- 20x4 I2C LCD module (e.g. GeeekPi IIC LCD2004)
- Push button
- Jumper wires and breadboard

---

## Wiring

- **LCD**: SDA → D8, SCL → D9, VCC → 5V, GND → GND
- **LEDs**: D2 (Blue), D3 (Red), D4 (Green) → through resistors → GND
- **Button**: D5 to GND (with internal pull-up)
- **Potentiometer**: middle pin → A0, sides → 3.3V and GND

---

## Future Plans
- Wi-Fi control via web dashboard or mobile app
- Save last used LED and brightness in flash memory
- Remote telemetry and status display

