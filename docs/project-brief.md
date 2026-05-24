# Project Brief

## Project Name

ClapControl IoT

## Goal

Build an ESP32 lamp controller that toggles a bulb through a relay module, listens to a KY-037 sound sensor, and exposes manual control plus telemetry through a local WiFi web dashboard.

## Main Features

- ESP32 connects to local WiFi and serves the dashboard on port `80`.
- The dashboard turns the bulb relay ON, OFF, or toggles it.
- Clap mode can be enabled or disabled from the dashboard.
- KY-037 AO controls software sensitivity through the web threshold slider.
- KY-037 DO is shown as a diagnostic signal.
- The dashboard displays analog level, peak hold, uptime, and WiFi signal.
- Relay starts OFF after boot.

## Components

- ESP32 DevKit V1.
- KY-037 sound sensor.
- Relay module matched to the bulb voltage and current.
- Bulb and safe lamp wiring through the relay contact side.

## Pin Map

| Function | ESP32 Pin |
| --- | --- |
| Relay drive | GPIO25 / D25 |
| KY-037 DO | GPIO35 / D35 |
| KY-037 AO | GPIO34 / D34 |
| 3.3V rail | ESP32 3V3 |
| Ground rail | ESP32 GND |

## Technical Constraints

- Framework: Arduino.
- Build system: PlatformIO.
- Board target: ESP32 DevKit V1.
- Control surface: local WiFi web dashboard.
- Relay logic: active HIGH. `HIGH` turns the bulb relay ON, `LOW` turns it OFF.
- Clap detection source: KY-037 AO analog threshold from GPIO34.

## Safety Notes

The ESP32 controls only the relay input. Bulb voltage must stay on the relay contact side and must not touch ESP32 logic, breadboard sensor wiring, or USB-connected equipment.
