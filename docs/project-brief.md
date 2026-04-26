# Project Brief

## Nama Project

ClapControl IoT

## Tujuan

Membuat sistem kendali lampu berbasis IoT menggunakan ESP32 DevKit V1, sensor suara KY-037, relay 1 channel 5V aktif LOW, dan dashboard web lokal.

## Target Pengguna

- Pengguna yang ingin mengontrol lampu AC dari jaringan WiFi lokal.
- Pengguna yang ingin menguji kendali lampu dengan deteksi tepuk tangan.
- Pengguna yang membutuhkan monitoring nilai sensor suara secara realtime.

## Fitur Utama

- ESP32 terkoneksi ke WiFi sebagai web server lokal.
- Dashboard web modern dan responsif.
- Status lampu ON/OFF.
- Tombol manual ON, OFF, dan TOGGLE.
- Mode clap dapat diaktifkan atau dinonaktifkan dari dashboard.
- Nilai sensor analog KY-037 ditampilkan realtime.
- Threshold sensor bisa diatur dari web.
- Clap detection memakai cooldown untuk mengurangi double trigger.
- Endpoint JSON untuk status dan kontrol.

## Komponen

- ESP32 DevKit V1 Type-C CP2102.
- Sensor suara KY-037.
- Relay 1 channel 5V.
- Lampu AC melalui fitting dan steker.

## Pin

| Fungsi | Pin ESP32 |
| --- | --- |
| KY-037 AO | GPIO34 |
| KY-037 DO | GPIO27 |
| Relay IN | GPIO26 |

## Batasan Teknis

- Framework: Arduino.
- Build system: PlatformIO.
- Library eksternal tidak digunakan.
- Library yang digunakan: `WiFi.h` dan `WebServer.h`.
- HTML, CSS, dan JavaScript dashboard disimpan langsung di `src/main.cpp`.
- Relay aktif LOW: `LOW` berarti lampu ON, `HIGH` berarti lampu OFF.

## Risiko dan Catatan Keselamatan

Lampu AC memakai tegangan listrik rumah. Sisi AC harus diisolasi dari rangkaian ESP32. Pastikan wiring relay dilakukan saat listrik mati dan gunakan enclosure yang aman.
