# Architecture

The firmware is a small ESP32 Arduino application. It connects to WiFi, serves a local dashboard, controls the relay output, reads KY-037 AO/DO, and exposes JSON endpoints for the browser UI.

## Modules

| Area | Responsibility |
| --- | --- |
| WiFi setup | Connect to the configured local WiFi network |
| Web server | Serve HTML, CSS, JavaScript, and JSON endpoints |
| Relay control | Apply manual and clap-triggered relay state on GPIO25 |
| Sound detection | Read AO on GPIO34 and compare it with the web threshold |
| Diagnostics | Report DO on GPIO35, analog value, peak value, WiFi RSSI, and uptime |
| UI assets | Store the dashboard HTML/CSS/JS in `include/web_ui.h` |

## Runtime State

| State | Type | Purpose |
| --- | --- | --- |
| `relayOn` | `bool` | Current relay output state |
| `clapModeEnabled` | `bool` | Whether sound may toggle the relay |
| `soundThreshold` | `uint16_t` | AO threshold, range `0..4095` |
| `soundValue` | `uint16_t` | Latest AO reading |
| `soundPeakValue` | `uint16_t` | Slowly decaying peak value for UI feedback |
| `minSoundActiveMs` | `unsigned long` | Minimum active time before a trigger is accepted |
| `clapCooldownMs` | `unsigned long` | Delay before another accepted trigger |
| `clapCount` | `unsigned long` | Accepted trigger count |

## Hardware Boundary

The ESP32 controls only the relay input circuit. Bulb voltage must stay on the relay contact side and must not touch ESP32 pins, sensor wiring, breadboard signal rows, or USB-connected equipment.

Relay logic is currently active HIGH because GPIO25 drives a transistor path. If a different relay module is wired directly and behaves inverted, swap `RELAY_ON_LEVEL` and `RELAY_OFF_LEVEL` in `src/main.cpp`.

## Web Strategy

The dashboard is local-network only. The browser fetches `/api/status` regularly and sends simple GET commands for relay, clap mode, and sensitivity changes.

This avoids cloud dependency while keeping the control surface fully website-driven.
