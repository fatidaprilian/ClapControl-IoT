# Architecture Decision Record

## Decision

Use PlatformIO with the Arduino framework for ESP32 DevKit V1, keeping the firmware in a single `src/main.cpp` file because the web dashboard HTML, CSS, JavaScript, relay control, sensor reading, and API endpoints are tightly scoped for one embedded device.

## Context

The requested device monitors a KY-037 sound sensor, controls an active LOW relay, and serves a local web dashboard. The user explicitly constrained the implementation to Arduino with `WiFi.h` and `WebServer.h`, with no external libraries.

## Consequences

- The firmware can be built by a normal PlatformIO ESP32 environment without additional dependencies.
- The web UI is embedded in flash as a raw string, which keeps deployment simple.
- API endpoints validate user-provided threshold values before applying them.
- WiFi credentials are placeholders in code and should be replaced locally before upload.
- KY-037 DO is exposed for monitoring, while clap trigger uses AO plus web-configured threshold for predictable calibration.

## Safety Notes

The relay switches AC load, so mains wiring must be isolated from the ESP32 circuit and handled with correct electrical safety practices.

## Related Docs

- [Architecture](architecture.md)
- [Flow Overview](flow-overview.md)
- [API Contract](api-contract.md)
- [Hardware Setup](hardware-setup.md)
