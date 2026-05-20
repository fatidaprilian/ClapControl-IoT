# Project Brief

## Project Name

ClapControl IoT

## Goal

Build an ESP32 lamp controller that toggles a bulb through a relay module, listens to a KY-037 sound sensor, and exposes manual control plus telemetry through Blynk.

## Main Features

- ESP32 connects to WiFi and Blynk.
- Blynk switch turns the bulb relay ON or OFF.
- Blynk momentary button toggles the relay.
- Clap mode can be enabled or disabled from Blynk.
- KY-037 DO can toggle the relay when clap mode is enabled.
- Blynk displays sound trigger, uptime, and WiFi signal.
- Relay starts OFF after boot.

## Components

- ESP32 DevKit V1.
- KY-037 sound sensor.
- Relay module matched to the bulb voltage and current.
- Bulb and safe lamp wiring through the relay contact side.

## Pin Map

| Function | ESP32 Pin |
| --- | --- |
| Relay IN | GPIO2 / D2 |
| KY-037 DO | GPIO22 / D22 |
| 3.3V rail | ESP32 3V3 |
| Ground rail | ESP32 GND |

## Technical Constraints

- Framework: Arduino.
- Build system: PlatformIO.
- Board target: ESP32 DevKit V1.
- Cloud control: Blynk static token flow.
- Relay logic: active HIGH. `HIGH` turns the bulb relay ON, `LOW` turns it OFF.
- Clap detection source: KY-037 DO active LOW.

## Safety Notes

The ESP32 controls only the relay input. Bulb voltage must stay on the relay contact side and must not touch ESP32 logic, breadboard sensor wiring, or USB-connected equipment.
