# Operation Guide

## Prerequisites

- Visual Studio Code with PlatformIO, or PlatformIO CLI.
- ESP32 DevKit V1 connected by USB.
- Relay IN connected to GPIO2 / D2.
- KY-037 DO connected to GPIO22 / D22.
- Blynk account, template, device, and auth token.
- 2.4 GHz WiFi network.

## Blynk Setup

1. Create a Blynk template.
2. Create a device from that template.
3. Copy the template ID, template name, and auth token.
4. Create datastreams from [Blynk Contract](api-contract.md).
5. Add widgets for lamp switch, clap mode, toggle, sound trigger, uptime, and RSSI.

## Firmware Configuration

Open `src/main.cpp`, then replace:

```cpp
#define BLYNK_TEMPLATE_ID "GANTI_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "ClapControl IoT"
#define BLYNK_AUTH_TOKEN "GANTI_AUTH_TOKEN"

const char WIFI_SSID[] = "GANTI_NAMA_WIFI";
const char WIFI_PASSWORD[] = "GANTI_PASSWORD_WIFI";
```

Do not commit real credentials or tokens.

## Build

```bash
pio run
```

## Upload

```bash
pio run --target upload
```

Close Serial Monitor before upload.

## Serial Monitor

```bash
pio device monitor --baud 115200
```

Startup output includes:

```text
ClapControl IoT - ESP32 DevKit V1 + Blynk
Relay IN: GPIO2 / D2
KY-037 DO: GPIO22 / D22
Connecting to WiFi and Blynk...
```

## Default Values

| Setting | Value |
| --- | --- |
| Serial baud | `115200` |
| Relay pin | GPIO2 / D2 |
| Relay ON | `HIGH` |
| Relay OFF | `LOW` |
| Sound DO pin | GPIO22 / D22 |
| Sound active level | `LOW` |
| Clap cooldown | `650 ms` |
| Telemetry interval | `1000 ms` |
