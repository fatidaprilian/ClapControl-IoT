# Hardware Setup

## Current Wiring

```text
ESP32 DevKit V1 + KY-037 DO + relay module + bulb + local WiFi dashboard
```

| Part | Connection |
| --- | --- |
| Relay drive | ESP32 GPIO25 / D25 |
| KY-037 DO | ESP32 GPIO35 / D35 |
| KY-037 AO | ESP32 GPIO34 / D34 |
| Relay VCC | 5V / VIN rail |
| Relay GND | GND rail |
| KY-037 + | 3V3 rail |
| KY-037 G | GND rail |

GPIO34 and GPIO35 are input-only pins. GPIO34 reads analog threshold data, and GPIO35 is kept as a digital diagnostic input.

## Relay Note

The current firmware assumes the ESP32 drives a transistor or relay input with:

| Relay State | GPIO25 |
| --- | --- |
| ON | `HIGH` |
| OFF | `LOW` |

If your relay module is directly connected and active LOW, swap `RELAY_ON_LEVEL` and `RELAY_OFF_LEVEL` in `src/main.cpp`.

## KY-037 Note

Use both signal pins:

- `AO -> GPIO34 / D34` for web-controlled threshold.
- `DO -> GPIO35 / D35` for diagnostic state.

The web threshold slider controls the analog reading from AO. The physical trimpot still affects the KY-037 module behavior, especially the DO indicator.

## Safety

The ESP32 must not touch mains voltage. Keep bulb wiring only on the relay contact side.

Use separate 5V/VIN and 3V3 rails:

- Relay power: 5V/VIN rail.
- Sensor power: 3V3 rail.
- Ground: common GND rail.
