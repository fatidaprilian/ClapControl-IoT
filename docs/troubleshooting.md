# Troubleshooting

## ESP32 Upload Fails

Possible causes:

- Serial Monitor is open.
- Wrong COM port is selected.
- USB cable is charge-only or unstable.
- ESP32 did not enter bootloader mode.

Diagnosis:

1. Close Serial Monitor.
2. Reconnect ESP32.
3. Run `pio device list`.
4. Run `pio run --target upload`.
5. If upload stalls, hold `BOOT` when upload starts, then release after writing begins.

## ESP32 Does Not Boot

GPIO2 is a boot-strapping pin on many ESP32 boards. A relay module can affect boot if it pulls the pin to the wrong level.

Solutions:

1. Disconnect relay IN and reset the ESP32.
2. If boot works without relay IN, move relay IN to GPIO18 and update `RELAY_PIN`.
3. Keep relay VCC/GND on the rails and only move the signal wire.

## Blynk Device Stays Offline

Possible causes:

- Wrong Blynk auth token.
- Wrong template ID or template name.
- Wrong WiFi SSID or password.
- WiFi is 5 GHz only.
- ESP32 is too far from router.

Solutions:

1. Use a 2.4 GHz WiFi network.
2. Recheck credentials in `src/main.cpp`.
3. Watch Serial Monitor at `115200`.
4. Move ESP32 closer to router.

## Relay Does Not Click

Possible causes:

- Relay IN is not connected to GPIO2 / D2.
- Relay VCC or GND is missing.
- Relay input side and ESP32 do not share ground.
- Relay module is active LOW.

Solutions:

1. Confirm relay IN goes to GPIO2 / D2.
2. Confirm relay VCC goes to the red rail and GND to the blue rail.
3. If the relay module is active LOW, swap `RELAY_ON_LEVEL` and `RELAY_OFF_LEVEL` in `src/main.cpp`.

## Clap Is Not Detected

Possible causes:

- KY-037 DO is not connected to GPIO22 / D22.
- KY-037 VCC or GND is missing.
- KY-037 trimpot is poorly adjusted.
- Clap mode is disabled in Blynk.

Solutions:

1. Set Blynk `V1` to `1`.
2. Watch Blynk `V3` while clapping.
3. Adjust the KY-037 trimpot.
4. Confirm the KY-037 module LED changes when clapping.

## Too Many False Triggers

Possible causes:

- KY-037 trimpot is too sensitive.
- Sensor picks up table vibration or nearby noise.
- Relay click or bulb fixture vibration triggers the sensor.

Solutions:

1. Reduce sensor sensitivity with the trimpot.
2. Move the KY-037 away from relay and bulb wiring.
3. Increase `CLAP_COOLDOWN_MS` in `src/main.cpp` if needed.
