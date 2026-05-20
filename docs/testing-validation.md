# Testing and Validation

## Build Check

Run:

```bash
pio run
```

Build is valid when PlatformIO reports success for `env:esp32dev`.

## Upload Check

Run:

```bash
pio run --target upload
```

Upload requires the ESP32 to be connected and the serial port to be free.

## Blynk Connection Check

1. Open Serial Monitor at `115200`.
2. Reset ESP32.
3. Confirm the device connects to WiFi and Blynk.
4. Confirm the device shows online in Blynk.

## Blynk Control Check

- `V0 = 1` turns relay GPIO2 ON.
- `V0 = 0` turns relay GPIO2 OFF.
- Pressing `V2` toggles the relay once and resets `V2` to `0`.
- `V1 = 1` enables clap mode.
- `V1 = 0` disables clap mode.
- `V3` reflects KY-037 DO trigger state.
- `V4` increases over time.
- `V5` reports WiFi RSSI.

## Relay and Bulb Safety Check

Start without mains voltage connected to the bulb side.

- On boot, relay GPIO2 should be OFF.
- Blynk `V0 = 1` should activate the relay input.
- Blynk `V0 = 0` should deactivate the relay input.
- Relay clicking should match Blynk state.
- Only connect the bulb load after low-voltage relay input behavior is correct.

## Regression Checklist

- `platformio.ini` targets ESP32 DevKit V1.
- Firmware keeps relay pin GPIO2 / D2.
- Firmware keeps KY-037 DO on GPIO22 / D22.
- Real WiFi password and Blynk token are not committed.
- Clap detection uses arming and cooldown.
- Relay starts OFF on boot.
