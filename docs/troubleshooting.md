# Troubleshooting

## Upload Port Not Found

1. Unplug and reconnect the ESP32 USB cable.
2. Run `pio device list`.
3. Use the detected port with `pio run --target upload --upload-port COMx`.
4. Try another USB cable if no port appears.
5. If upload connects but fails boot mode, hold `BOOT` when upload starts and release after writing begins.

## Dashboard Does Not Open

- Confirm `include/secrets.h` contains the correct local WiFi SSID and password.
- Confirm Serial Monitor prints `Dashboard: http://...`.
- Open the URL from a phone or laptop on the same WiFi network.
- Check that the router did not isolate WiFi clients from each other.

## Relay Does Not Follow UI

- Confirm relay drive uses GPIO25 / D25.
- Confirm relay VCC and GND are on the correct power rails.
- Confirm the transistor emitter, base resistor, and collector are wired correctly.
- If the relay is inverted, swap `RELAY_ON_LEVEL` and `RELAY_OFF_LEVEL` in `src/main.cpp`.
- If the relay module is 5V-only, do not power it from the 3.3V rail.

## Sensor Always Active

- Confirm KY-037 AO goes to GPIO34 / D34.
- Confirm KY-037 `+` goes to 3.3V and `G` goes to GND.
- Raise `Threshold analog` above the idle AO value.
- Increase `Cooldown` if one clap creates repeated events.
- Adjust the KY-037 trimpot only if the module output is still unstable.

## Sensor Does Not Respond

- Clap close to the microphone and watch `Peak hold`.
- Lower `Threshold analog` until clap peaks cross the marker.
- Confirm AO is connected, not only DO.
- Confirm GPIO34 is not used as an output; it must remain input-only.

## Relay Flickers When Sensor Is Connected

- Keep 5V relay power and 3.3V sensor power on separate rails.
- Keep GND common between rails.
- Do not power a 5V relay from ESP32 3.3V.
- Use shorter jumper wires for sensor signal if readings are noisy.
- Start with `Threshold analog` above idle plus a clear margin.
