# API Contract

Base URL adalah alamat IP ESP32 di jaringan lokal, misalnya:

```text
http://192.168.1.25
```

Semua response API memakai `application/json`.

## `GET /api/status`

Mengambil status lengkap perangkat.

### Response 200

```json
{
  "lampOn": true,
  "clapMode": true,
  "analog": 2375,
  "digital": 1,
  "threshold": 2200,
  "cooldownMs": 650,
  "wifiRssi": -54,
  "uptimeMs": 125430
}
```

### Field

| Field | Tipe | Keterangan |
| --- | --- | --- |
| `lampOn` | boolean | `true` jika LED/lampu 5V ON |
| `clapMode` | boolean | `true` jika clap mode aktif |
| `analog` | number | Nilai ADC KY-037 AO, rentang 0 sampai 4095 |
| `digital` | number | Nilai KY-037 DO, `0` atau `1` |
| `threshold` | number | Threshold analog aktif |
| `cooldownMs` | number | Cooldown clap detection |
| `wifiRssi` | number | Kekuatan sinyal WiFi dalam dBm |
| `uptimeMs` | number | Waktu hidup ESP32 dalam milidetik |

## `GET /api/on`

Menyalakan LED/lampu 5V.

### Response 200

Response sama seperti `/api/status`.

## `GET /api/off`

Mematikan LED/lampu 5V.

### Response 200

Response sama seperti `/api/status`.

## `GET /api/toggle`

Mengubah status LED/lampu 5V dari ON ke OFF atau OFF ke ON.

### Response 200

Response sama seperti `/api/status`.

## `GET /api/clap-mode`

Toggle clap mode.

### Response 200

Response sama seperti `/api/status`.

## `GET /api/clap-mode?enabled=true`

Mengatur clap mode secara eksplisit.

Nilai yang dianggap aktif:

- `true`
- `1`
- `on`

Nilai lain dianggap nonaktif.

### Response 200

Response sama seperti `/api/status`.

## `GET /api/threshold?value=2200`

Mengatur threshold analog untuk clap detection.

### Query Parameter

| Parameter | Wajib | Rentang | Keterangan |
| --- | --- | --- | --- |
| `value` | Ya | `0` sampai `4095` | Ambang analog KY-037 |

### Response 200

Response sama seperti `/api/status`.

### Response 400

```json
{
  "error": "threshold value must be a number from 0 to 4095"
}
```

## Endpoint Tidak Dikenal

### Response 404

```json
{
  "error": "not found"
}
```

## Catatan Keamanan

API ini dirancang untuk jaringan lokal dan tidak memiliki autentikasi. Jangan expose ESP32 langsung ke internet tanpa gateway yang aman.
