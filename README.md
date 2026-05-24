# ClapControl IoT

PlatformIO firmware for an ESP32 DevKit V1 that controls a bulb relay from a local WiFi web dashboard and reads a KY-037 sound sensor.

The current test wiring uses ESP32 GPIO25 / D25 for the relay drive, GPIO35 / D35 for KY-037 digital output, and GPIO34 / D34 for KY-037 analog output.

## Hardware

| Component | ESP32 Pin |
| --- | --- |
| Relay drive | GPIO25 / D25 |
| KY-037 DO | GPIO35 / D35 |
| KY-037 AO | GPIO34 / D34 |
| Relay VCC | 5V / VIN rail |
| Relay GND | GND rail |
| KY-037 + | 3V3 rail |
| KY-037 G | GND rail |

Do not connect mains voltage directly to the ESP32, breadboard logic pins, or sensor wiring. The ESP32 only drives the relay control path.

## Setup

1. Copy `include/secrets.example.h` to `include/secrets.h`.
2. Put your local WiFi SSID and password in `include/secrets.h`.
3. Build and upload:

```bash
pio run --target upload
```

4. Open Serial Monitor at `115200`.
5. Open the printed dashboard URL.

## Web Features

- Manual relay ON/OFF/TOGGLE.
- Clap mode ON/OFF.
- KY-037 analog level, peak hold, and digital trigger status.
- Software sensitivity controls:
  - Threshold analog: real web-controlled threshold from AO on GPIO34.
  - Hold suara aktif: how long the analog value must stay over threshold.
  - Cooldown toggle: delay before the next accepted clap.

KY-037 DO is still shown as a diagnostic indicator. Clap detection uses AO and the web threshold.
