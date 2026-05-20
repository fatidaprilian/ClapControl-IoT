# Architecture

## Summary

Firmware runs as a small monolithic ESP32 Arduino application. ESP32 connects to WiFi and Blynk, controls the bulb relay, reads KY-037 digital output, and publishes telemetry.

## Logical Areas in `src/main.cpp`

| Area | Responsibility |
| --- | --- |
| Blynk and WiFi config | Holds safe placeholder credentials and template identifiers |
| Pin map | Defines relay and KY-037 DO pins |
| Relay state | Stores lamp ON/OFF state and writes active HIGH relay output |
| Blynk handlers | Receives switch, clap mode, and toggle commands |
| Telemetry | Publishes sound trigger, uptime, and WiFi RSSI |
| Clap detection | Reads KY-037 DO with arming and cooldown |

## Runtime State

| State | Type | Purpose |
| --- | --- | --- |
| `lampOn` | `bool` | Current relay/lamp state |
| `clapModeEnabled` | `bool` | Whether clap detection may toggle the relay |
| `soundPeakArmed` | `bool` | Prevents one sound peak from triggering repeatedly |
| `lastClapAt` | `unsigned long` | Last accepted clap time |

## Relay Output

The relay module input is active HIGH.

| Lamp State | GPIO2 |
| --- | --- |
| ON | `HIGH` |
| OFF | `LOW` |

If the physical relay module is active LOW, swap `RELAY_ON_LEVEL` and `RELAY_OFF_LEVEL` in `src/main.cpp`.

## Sensor Strategy

- KY-037 DO connects to GPIO22 / D22.
- Firmware treats DO active LOW as sound detected.
- Cooldown is `650 ms`.
- `soundPeakArmed` prevents one continuous sound pulse from toggling repeatedly.

## Blynk Strategy

Blynk is the public control surface:

- `V0`: lamp switch
- `V1`: clap mode
- `V2`: momentary toggle
- `V3`: sound trigger telemetry
- `V4`: uptime seconds
- `V5`: WiFi RSSI

Telemetry is sent by `BlynkTimer` every 1000 ms. The main loop keeps `Blynk.run()`, `timer.run()`, and clap detection short.
