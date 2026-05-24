# Operation Guide

## Before Upload

1. Wire the current hardware contract:
   - Relay drive to GPIO25 / D25.
   - KY-037 DO to GPIO35 / D35.
   - KY-037 AO to GPIO34 / D34.
   - Relay VCC to the 5V / VIN rail if the relay module requires 5V.
   - KY-037 `+` to the 3.3V rail.
   - All grounds common.
2. Copy `include/secrets.example.h` to `include/secrets.h`.
3. Put the local WiFi SSID and password in `include/secrets.h`.

Do not commit `include/secrets.h`.

## Build and Upload

```bash
pio run --target upload
```

If upload cannot find the board, list ports:

```bash
pio device list
```

Then upload with the detected port:

```bash
pio run --target upload --upload-port COM3
```

If ESP32 fails to enter download mode, hold `BOOT` when upload starts and release it after writing begins.

## Run

Open Serial Monitor:

```bash
pio device monitor --baud 115200
```

The firmware prints a dashboard URL such as:

```text
Dashboard: http://192.168.1.25
```

Open that URL from a device on the same WiFi network.

## Web Controls

- `ON`, `OFF`, and `TOGGLE` control the relay.
- `Clap mode` enables or disables sound-triggered relay changes.
- `Threshold analog` sets the AO threshold from GPIO34.
- `Hold aktif` requires the sound level to stay above threshold for a short time.
- `Cooldown` prevents one clap from toggling the relay repeatedly.

Start with a threshold slightly above the idle AO value shown on the dashboard. Clap near the sensor, watch the peak value, then set the threshold between idle and clap peak.

Do not use threshold `0`. The firmware clamps threshold to at least `50` because analog detection treats values at or above threshold as active.

If the peak value does not move when you clap, shout, or tap near the microphone, treat it as a hardware or wiring issue first. Confirm AO goes to GPIO34, the sensor gets 3.3V, ground is common, and the KY-037 module is not faulty.
