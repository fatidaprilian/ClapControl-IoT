# Architecture

## Ringkasan

Firmware berjalan sebagai aplikasi monolitik kecil di ESP32. ESP32 membaca sensor KY-037, mengelola state LED 5V dan clap mode, lalu melayani dashboard web serta endpoint JSON melalui HTTP lokal.

## Struktur Project

```text
.
|-- platformio.ini
|-- src/
|   `-- main.cpp
|-- docs/
|   |-- README.md
|   |-- project-brief.md
|   |-- architecture.md
|   |-- flow-overview.md
|   |-- api-contract.md
|   |-- hardware-setup.md
|   |-- 5v-led-migration.md
|   |-- operation-guide.md
|   |-- testing-validation.md
|   `-- troubleshooting.md
`-- README.md
```

## Modul Logis di `src/main.cpp`

| Area | Tanggung Jawab |
| --- | --- |
| Konfigurasi WiFi dan pin | Menentukan kredensial lokal, pin sensor, pin output lampu, threshold awal, dan cooldown |
| State lampu | Menyimpan status ON/OFF dan menerapkan level output sesuai hardware yang dipakai |
| Web dashboard | Menyajikan HTML, CSS, dan JavaScript dari flash ESP32 |
| API HTTP | Menyediakan endpoint status, kontrol lampu, clap mode, dan threshold |
| Sensor reading | Membaca nilai AO dan DO KY-037 |
| Clap detection | Menentukan trigger berdasarkan threshold analog dan cooldown |
| WiFi reconnect | Mencoba koneksi ulang secara berkala ketika WiFi terputus |

## State Runtime

| State | Tipe | Fungsi |
| --- | --- | --- |
| `lampOn` | `bool` | Status lampu saat ini |
| `clapModeEnabled` | `bool` | Apakah tepuk tangan boleh mengubah status lampu |
| `soundPeakArmed` | `bool` | Guard agar satu puncak suara tidak dihitung berkali-kali |
| `soundThreshold` | `uint16_t` | Ambang analog untuk deteksi tepuk |
| `lastClapAt` | `unsigned long` | Waktu trigger tepuk terakhir |
| `lastWifiReconnectAttempt` | `unsigned long` | Waktu percobaan reconnect terakhir |

## Output LED 5V

Implementasi firmware memakai transistor switch aktif HIGH. Mapping output dibuat eksplisit agar fungsi kontrol lampu tidak perlu mengingat detail polaritas hardware di banyak tempat.

| State LED | Level GPIO26 |
| --- | --- |
| ON | `HIGH` |
| OFF | `LOW` |

GPIO26 masuk ke resistor 1k ohm lalu base transistor NPN. Daya LED tetap berasal dari rail 5V, bukan dari GPIO ESP32.

## Sensor Strategy

- AO KY-037 dibaca lewat ADC GPIO34.
- DO KY-037 tetap dibaca dan dikirim ke `/api/status` untuk monitoring.
- Trigger clap memakai nilai analog AO dan threshold dari web.

Alasan: modul KY-037 dapat memiliki polaritas DO yang berbeda antar modul atau pengaturan trimpot. Threshold analog lebih mudah dikontrol langsung dari dashboard.

## Web Dashboard Strategy

Dashboard disimpan dalam `PROGMEM` sebagai raw HTML string. Ini membuat firmware sederhana: tidak perlu filesystem, SPIFFS, LittleFS, atau asset pipeline tambahan.

## API Strategy

API memakai metode `GET` untuk semua endpoint karena dashboard lokal hanya membutuhkan kontrol sederhana dari browser. Endpoint yang menerima input, yaitu threshold, memvalidasi nilai agar hanya angka `0` sampai `4095`.

## Failure Handling

- WiFi gagal konek: status dicetak ke Serial Monitor, server tetap disiapkan dan firmware mencoba reconnect berkala.
- Endpoint tidak dikenal: membalas `404` JSON.
- Threshold tidak valid: membalas `400` JSON.
- Sensor noise: dikurangi dengan threshold analog, `soundPeakArmed`, dan cooldown `650 ms`.
