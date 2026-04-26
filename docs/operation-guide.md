# Operation Guide

## Prasyarat

- Visual Studio Code dengan extension PlatformIO, atau PlatformIO CLI.
- ESP32 DevKit V1 tersambung ke komputer.
- Jaringan WiFi 2.4 GHz.

ESP32 umumnya tidak mendukung WiFi 5 GHz, jadi pastikan SSID yang dipakai adalah 2.4 GHz atau mixed mode.

## Konfigurasi WiFi

Buka `src/main.cpp`, lalu sesuaikan:

```cpp
const char *WIFI_SSID = "NAMA_WIFI";
const char *WIFI_PASSWORD = "PASSWORD_WIFI";
```

Jangan commit kredensial WiFi pribadi ke repository publik.

## Build

```bash
pio run
```

Jika `pio` tidak tersedia di terminal, gunakan PlatformIO extension di VS Code atau path CLI PlatformIO yang terpasang di komputer.

## Upload

```bash
pio run --target upload
```

## Serial Monitor

```bash
pio device monitor --baud 115200
```

Setelah WiFi terkoneksi, Serial Monitor akan menampilkan alamat dashboard:

```text
WiFi connected. Dashboard: http://<ip-esp32>
```

## Menggunakan Dashboard

1. Buka IP ESP32 dari browser di jaringan yang sama.
2. Lihat indikator status lampu.
3. Gunakan tombol ON, OFF, atau TOGGLE.
4. Aktifkan atau nonaktifkan clap mode.
5. Amati nilai analog suara.
6. Geser threshold sampai deteksi tepuk stabil.

## Default Penting

| Setting | Nilai |
| --- | --- |
| Baud Serial | `115200` |
| Web server port | `80` |
| Threshold awal | `2200` |
| Cooldown clap | `650 ms` |
| Relay ON | `LOW` |
| Relay OFF | `HIGH` |

## Setelah Perubahan Kode

Jalankan build ulang:

```bash
pio run
```

Jika build sukses, upload ke board:

```bash
pio run --target upload
```
