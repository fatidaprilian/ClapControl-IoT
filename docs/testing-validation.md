# Testing and Validation

## Compile Check

Run:

```bash
pio run
```

The build must pass for the `esp32dev` environment.

## Upload Check

Run:

```bash
pio run --target upload
```

Confirm Serial Monitor prints the pin map and dashboard URL.

## Web Check

1. Open the dashboard URL on the same WiFi network.
2. Confirm relay state, analog value, peak value, DO state, WiFi RSSI, and uptime update.
3. Press `ON`, `OFF`, and `TOGGLE`.
4. Confirm the relay state in the UI matches the physical relay module.

## Sound Check

1. Watch the idle AO value.
2. Clap near the KY-037 and watch the peak value.
3. Move `Threshold analog` between idle and peak.
4. Keep `Hold aktif` around `20 ms` first.
5. Keep `Cooldown` around `650 ms` first.
6. Confirm one clap creates one accepted event.

## Safety Check

- Bulb mains wiring stays only on the relay contact side.
- ESP32, sensor, USB, and breadboard logic never touch bulb voltage.
- Relay VCC matches the relay module requirement.
- ESP32 does not reset when relay turns ON.
- Real WiFi credentials are not committed.
