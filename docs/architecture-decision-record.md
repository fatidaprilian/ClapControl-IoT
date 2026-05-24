# Architecture Decision Record

## Decision

Use an ESP32 DevKit V1 with the Arduino framework and a local WiFi web dashboard. The active build does not use Blynk because the current goal is a fully website-driven controller on the user's local network.

Keep the UI source separate in `include/web_ui.h` and keep board behavior in `src/main.cpp`. This preserves clean firmware structure without introducing a larger frontend build pipeline.

## Current Hardware Contract

- Relay drive: GPIO25 / D25.
- KY-037 DO: GPIO35 / D35, diagnostic only.
- KY-037 AO: GPIO34 / D34, used for the web-controlled threshold.
- Relay supply: 5V / VIN rail when the relay module requires 5V.
- KY-037 supply: 3.3V rail.
- Grounds must be common.

## Configuration Decision

WiFi stays enabled. Real SSID and password belong in local `include/secrets.h`, which is ignored by git. The committed source keeps only `include/secrets.example.h` with placeholder values.

## Rationale

- ESP32 has built-in WiFi, so it can host the dashboard without an external modem.
- AO on GPIO34 gives a real analog value, which makes web sensitivity possible.
- DO on GPIO35 remains useful for checking the KY-037 trimpot and module behavior.
- GPIO25 is a normal output pin and is safer for relay drive than boot-strapping pins.

## Rejected Alternatives

- Blynk: useful for remote cloud control, but less fitting when the user wants a custom website and local tuning.
- Arduino Uno as the main board: needs extra WiFi hardware for cloud or web control.
- DO-only clap detection: too dependent on the physical KY-037 trimpot and module polarity.
