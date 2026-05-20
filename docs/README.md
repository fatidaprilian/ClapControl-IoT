# ClapControl IoT Documentation

This folder contains the active project documentation for the Arduino Uno offline relay version of ClapControl IoT.

Use [Documentation Index](doc-index.md) as the routing map before opening deeper docs.

## Active Documents

| Document | Purpose |
| --- | --- |
| [Project Brief](project-brief.md) | Goal, users, features, hardware, and constraints |
| [Architecture](architecture.md) | Firmware structure, state, relay behavior, and safety boundaries |
| [Architecture Decision Record](architecture-decision-record.md) | Main hardware and runtime decisions |
| [Flow Overview](flow-overview.md) | Boot and clap-to-relay flow |
| [Control Contract](api-contract.md) | Current serial logs and hardware control contract |
| [Hardware Setup](hardware-setup.md) | Arduino Uno, KY-037, relay, and bulb wiring |
| [Operation Guide](operation-guide.md) | Build, upload, monitor, and runtime operation |
| [Testing and Validation](testing-validation.md) | Compile, sensor, relay, and safety checks |
| [Troubleshooting](troubleshooting.md) | Common failures and diagnosis steps |

## Historical Documents

| Document | Purpose |
| --- | --- |
| [5V LED Migration Guide](5v-led-migration.md) | Historical ESP32 low-voltage LED direction |
| [Design](DESIGN.md) | Historical ESP32 local dashboard UI direction |
| [Design Intent JSON](design-intent.json) | Historical UI design metadata |

## Summary

ClapControl IoT now targets an Arduino Uno controlling a bulb through a relay module. This version is offline-first and does not require Blynk, WiFi, or ESP8266 hardware.

Remote control can be added later after a network module or WiFi-capable board is available.
