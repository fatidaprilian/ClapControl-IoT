# Blynk Contract

The active ESP32 build uses Blynk virtual pins for remote relay control and telemetry.

## Credentials

Set these placeholders in `src/main.cpp` from the Blynk Console and local WiFi:

```cpp
#define BLYNK_TEMPLATE_ID "GANTI_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "ClapControl IoT"
#define BLYNK_AUTH_TOKEN "GANTI_AUTH_TOKEN"

const char WIFI_SSID[] = "GANTI_NAMA_WIFI";
const char WIFI_PASSWORD[] = "GANTI_PASSWORD_WIFI";
```

Do not commit real tokens or WiFi passwords.

## Virtual Pins

| Virtual Pin | Direction | Data Type | Range | Purpose |
| --- | --- | --- | --- | --- |
| `V0` | App to device, device to app | Integer | `0` or `1` | Lamp switch. `1` turns relay ON, `0` turns relay OFF |
| `V1` | App to device, device to app | Integer | `0` or `1` | Clap mode. `1` enables clap control |
| `V2` | App to device | Integer | `0` or `1` | Momentary toggle button. Firmware resets it to `0` after use |
| `V3` | Device to app | Integer | `0` or `1` | KY-037 DO trigger indicator |
| `V4` | Device to app | Integer | `0` and up | Uptime in seconds |
| `V5` | Device to app | Integer | negative dBm value | WiFi RSSI |

## Recommended Widgets

| Widget | Datastream |
| --- | --- |
| Switch | `V0` |
| Switch | `V1` |
| Button, push mode | `V2` |
| LED or Label | `V3` |
| Label | `V4` |
| Label | `V5` |

## Hardware Contract

| Signal | ESP32 Pin | Behavior |
| --- | --- | --- |
| Relay IN | GPIO2 / D2 | Active HIGH by default |
| KY-037 DO | GPIO22 / D22 | Active LOW sound trigger |

## Security Notes

Blynk auth token and WiFi credentials are secrets. Keep placeholders in committed source and use local values only for flashing the physical device.

Do not expose relay-controlled mains wiring while testing Blynk commands. Validate relay behavior with no mains load first.
