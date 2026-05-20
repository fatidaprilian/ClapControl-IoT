# Hardware Setup

## Current Hardware Status

The active build is:

```text
ESP32 DevKit V1 + KY-037 DO + relay module + bulb + Blynk
```

The ESP32 controls the relay input. The bulb must be wired only through the relay contact side.

## Components

- ESP32 DevKit V1.
- KY-037 sound sensor.
- Relay module that works from 3.3V control/power in the current build.
- Bulb and lamp wiring.
- Jumper wires.

## Power Rails

| ESP32 | Breadboard |
| --- | --- |
| 3V3 | left red rail |
| GND | left blue rail |

All relay and KY-037 logic power comes from those rails in the current wiring.

## Relay Wiring

| Relay Module | Connection |
| --- | --- |
| VCC | left red rail |
| GND | left blue rail |
| IN | ESP32 GPIO2 / D2 |

Relay logic:

| Lamp State | GPIO2 |
| --- | --- |
| ON | `HIGH` |
| OFF | `LOW` |

GPIO2 is a boot-strapping pin on many ESP32 boards. If the ESP32 fails to boot, disconnect relay IN during boot or move relay IN to GPIO18 and update `RELAY_PIN`.

## KY-037 Wiring

| KY-037 | Connection |
| --- | --- |
| `+` | left red rail |
| `G` | left blue rail |
| `DO` | ESP32 GPIO22 / D22 |

The active firmware uses KY-037 DO only. Adjust the KY-037 trimpot if the trigger is too sensitive or never triggers.

## Bulb Wiring Safety

The relay contact side is separate from ESP32 logic. Use the relay module's rated contact terminals for the bulb circuit.

Minimum safety rules:

1. Keep mains voltage away from the ESP32, KY-037, USB cable, and breadboard logic side.
2. Disconnect power before changing bulb or relay wiring.
3. Use insulated terminals, proper wire gauge, and an enclosure.
4. Do not touch exposed relay contact wiring while powered.
5. Test firmware and relay clicking without a mains bulb before connecting the final load.

## Relay Contact Wiring

Most relay modules expose `COM`, `NO`, and `NC`.

- Use `COM` and `NO` when the bulb should be OFF by default and ON only when the relay activates.
- Use `COM` and `NC` only if you intentionally want the bulb ON by default.

## Common Ground

The ESP32, KY-037, and relay input side must share ground:

```text
ESP32 GND = KY-037 GND = relay module GND
```
