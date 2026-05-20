# ClapControl IoT

PlatformIO firmware for an ESP32 DevKit V1 that controls a bulb relay through Blynk and toggles the relay from a KY-037 sound sensor.

The current wiring uses ESP32 GPIO2 / D2 for the relay input and GPIO22 / D22 for KY-037 digital output.

## Documentation

Start from [docs/doc-index.md](docs/doc-index.md) for the full documentation map.

Core docs:

- [Project Brief](docs/project-brief.md)
- [Architecture Decision Record](docs/architecture-decision-record.md)
- [Flow Overview](docs/flow-overview.md)
- [Blynk Contract](docs/api-contract.md)
- [Hardware Setup](docs/hardware-setup.md)
- [Operation Guide](docs/operation-guide.md)
- [Testing and Validation](docs/testing-validation.md)
- [Troubleshooting](docs/troubleshooting.md)

## Hardware

- ESP32 DevKit V1
- KY-037 sound sensor
- Relay module for bulb switching
- Bulb load connected through the relay contact side

Do not connect mains voltage directly to the ESP32, breadboard logic pins, or sensor wiring. The ESP32 only drives the relay input.

## Wiring

| Component | ESP32 Pin |
| --- | --- |
| ESP32 3V3 | left red rail |
| ESP32 GND | left blue rail |
| Relay VCC | left red rail |
| Relay GND | left blue rail |
| Relay IN | GPIO2 / D2 |
| KY-037 + | left red rail |
| KY-037 G | left blue rail |
| KY-037 DO | GPIO22 / D22 |

GPIO2 is a boot-strapping pin on many ESP32 boards. If the board fails to boot with the relay connected, move relay IN to a safer output pin such as GPIO18 and update `RELAY_PIN`.

## Setup

1. Create a Blynk template and device.
2. Open `src/main.cpp`.
3. Replace `GANTI_TEMPLATE_ID`, `GANTI_TEMPLATE_NAME`, `GANTI_AUTH_TOKEN`, `GANTI_NAMA_WIFI`, and `GANTI_PASSWORD_WIFI`.
4. Build and upload with PlatformIO:

```bash
pio run --target upload
```

5. Open the Serial Monitor at `115200` baud.
6. Control the relay from Blynk or by clapping near the KY-037.

## Blynk Virtual Pins

| Virtual Pin | Purpose |
| --- | --- |
| `V0` | Lamp switch |
| `V1` | Clap mode switch |
| `V2` | Momentary toggle button |
| `V3` | KY-037 digital trigger indicator |
| `V4` | Uptime seconds |
| `V5` | WiFi RSSI |
