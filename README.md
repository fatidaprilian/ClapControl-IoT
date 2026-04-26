# ClapControl IoT

PlatformIO firmware for an ESP32 DevKit V1 that controls an AC lamp through a 5V relay, reads a KY-037 sound sensor, and exposes a responsive web dashboard over local WiFi.

## Documentation

Start from [docs/README.md](docs/README.md) for the full project documentation:

- Project brief
- Architecture
- Flow overview
- API contract
- Hardware setup
- Operation guide
- Testing and troubleshooting

## Hardware

- ESP32 DevKit V1 Type-C CP2102
- KY-037 sound sensor
- 1 channel 5V relay module, active LOW
- AC lamp through fitting and plug

## Wiring

| Component | Pin |
| --- | --- |
| KY-037 AO | GPIO34 |
| KY-037 DO | GPIO27 |
| Relay IN | GPIO26 |
| Relay VCC/GND | External 5V/GND as required by the relay module |

Make sure AC wiring is isolated and handled safely. The ESP32 side must never be exposed to mains voltage.

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
