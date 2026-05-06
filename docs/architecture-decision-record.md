# Architecture Decision Record

## Decision

Use PlatformIO with the Arduino framework for ESP32 DevKit V1, keeping the firmware in a single `src/main.cpp` file because the web dashboard HTML, CSS, JavaScript, lamp output control, sensor reading, and API endpoints are tightly scoped for one embedded device.

Use a USB-only 5V LED output switched by an NPN transistor or logic-level MOSFET. GPIO26 is active HIGH: `HIGH` turns the LED load on, `LOW` turns it off.

## Context

The requested device monitors a KY-037 sound sensor, controls a lamp output, and serves a local web dashboard. The user explicitly constrained the implementation to Arduino with `WiFi.h` and `WebServer.h`, with no external libraries.

The hardware direction changed from AC relay switching to a lower-risk, mobile 5V DC system powered by one USB source.

## Consequences

- The firmware can be built by a normal PlatformIO ESP32 environment without additional dependencies.
- The web UI is embedded in flash as a raw string, which keeps deployment simple.
- API endpoints validate user-provided threshold values before applying them.
- WiFi credentials are placeholders in code and should be replaced locally before upload.
- KY-037 DO is exposed for monitoring, while clap trigger uses AO plus web-configured threshold for predictable calibration.
- Dashboard, API, threshold, clap mode, KY-037 monitoring, and WiFi reconnect behavior stay unchanged.
- GPIO26 output polarity is active HIGH for the transistor switch.

## Safety Notes

The 5V LED direction removes mains voltage from the prototype, but still requires USB current limits, resistor sizing, transistor rating, and short-circuit safety to be respected.

## Related Docs

- [Architecture](architecture.md)
- [Flow Overview](flow-overview.md)
- [API Contract](api-contract.md)
- [Hardware Setup](hardware-setup.md)
- [5V LED Migration Guide](5v-led-migration.md)
