# Web API Contract

The active ESP32 build exposes a local WiFi dashboard and JSON endpoints on port `80`.

## Pages and Assets

| Route | Purpose |
| --- | --- |
| `GET /` | Dashboard HTML |
| `GET /styles.css` | Dashboard CSS |
| `GET /app.js` | Dashboard JavaScript |

The UI source is separated in `include/web_ui.h`.

## Status

### `GET /api/status`

Response:

```json
{
  "relayOn": false,
  "analog": 1200,
  "peak": 2200,
  "digitalActive": false,
  "soundActive": false,
  "clapMode": true,
  "threshold": 1800,
  "minActiveMs": 5,
  "cooldownMs": 650,
  "clapCount": 0,
  "wifiRssi": -54,
  "uptimeMs": 12345
}
```

## Relay Control

### `GET /api/relay?state=on`
### `GET /api/relay?state=off`
### `GET /api/relay?state=toggle`

All relay actions return the same JSON shape as `/api/status`.

## Clap Mode

### `GET /api/clap`

Toggles clap mode.

### `GET /api/clap?enabled=1`
### `GET /api/clap?enabled=0`

Sets clap mode explicitly.

## Sensitivity

### `GET /api/sensitivity?threshold=1800&hold=20&cooldown=650`

| Query | Range | Purpose |
| --- | --- | --- |
| `hold` | `5..250` ms | Minimum time KY-037 AO must stay above threshold |
| `cooldown` | `200..2000` ms | Delay before another clap can toggle relay |
| `threshold` | `50..4095` | Analog AO threshold from GPIO34 |

This changes the firmware threshold used for clap detection. KY-037 DO is still exposed as diagnostic state.
