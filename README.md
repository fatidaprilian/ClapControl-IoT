# ClapControl IoT

PlatformIO firmware for an ESP32 DevKit V1 that controls a 5V LED output through an active HIGH transistor switch, reads a KY-037 sound sensor, and exposes a responsive web dashboard over local WiFi.

The project now targets a USB-only low-voltage build: ESP32, KY-037, LED 5V, transistor switch, and one 5V USB power source. The old AC relay direction is kept only as historical context in the docs.

## Documentation

Start from [docs/README.md](docs/README.md) for the full project documentation:

- Project brief
- Architecture
- Flow overview
- API contract
- Hardware setup
- 5V LED migration guide
- Operation guide
- Testing and troubleshooting

## Hardware

- ESP32 DevKit V1 Type-C CP2102
- KY-037 sound sensor
- 5V LED or short 5V LED strip
- NPN transistor, such as 2N2222 or S8050, or a logic-level N-MOSFET for larger LED loads
- 1k ohm base resistor
- LED current-limiting resistor for bare LEDs, such as 220 ohm

## Wiring

| Component | Pin |
| --- | --- |
| KY-037 AO | GPIO34 |
| KY-037 DO | GPIO27 |
| Lamp output control | GPIO26 |
| NPN base | GPIO26 through 1k ohm resistor |
| NPN emitter | Common GND |
| NPN collector | LED negative side |
| LED positive side | 5V rail through current limiting as required |

Do not connect AC mains to this build. Follow [docs/5v-led-migration.md](docs/5v-led-migration.md) for the complete wiring and validation guide.

## Setup

1. Open `src/main.cpp`.
2. Replace `GANTI_NAMA_WIFI` and `GANTI_PASSWORD_WIFI`.
3. Build and upload with PlatformIO:

```bash
pio run --target upload
```

4. Open the Serial Monitor at `115200` baud and visit the printed ESP32 IP address.

## Web Endpoints

- `GET /api/status`
- `GET /api/on`
- `GET /api/off`
- `GET /api/toggle`
- `GET /api/clap-mode`
- `GET /api/threshold?value=2200`

The dashboard auto-refreshes status every 500 ms and lets you control the lamp, clap mode, and sound threshold.
