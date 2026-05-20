# Architecture Decision Record

## Decision

Use PlatformIO with the Arduino framework for ESP32 DevKit V1. Use Blynk as the remote-control surface because ESP32 has built-in WiFi and no extra WiFi modem is required.

Keep the firmware in a single `src/main.cpp` file because the current scope is one board, one relay output, one sound sensor, and one Blynk integration contract.

Use the current physical wiring:

- Relay IN on GPIO2 / D2.
- KY-037 DO on GPIO22 / D22.
- 3V3 to left red rail.
- GND to left blue rail.

## Context

The project returned from Arduino Uno to ESP32 DevKit V1. The user has ESP32 wired through a breadboard rail layout and can use WiFi directly from the ESP32.

## Consequences

- Arduino Uno offline code is no longer active.
- Blynk, WiFi, relay control, clap mode, and telemetry are active again.
- Real Blynk token and WiFi credentials must stay local and must not be committed.
- GPIO2 works with the current wiring, but it is a boot-strapping pin on many ESP32 boards. If the board fails to boot, relay IN should move to a safer pin such as GPIO18.

## Safety Notes

The firmware does not drive the bulb directly. It only drives the relay module input. Any mains-voltage bulb wiring must stay isolated from ESP32 logic and must use properly rated hardware.
