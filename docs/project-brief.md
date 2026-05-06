# Project Brief

## Nama Project

ClapControl IoT

## Tujuan

Membuat sistem kendali lampu berbasis IoT menggunakan ESP32 DevKit V1, sensor suara KY-037, LED 5V yang dikontrol transistor aktif HIGH, dashboard web lokal, dan deteksi tepuk tangan.

Seluruh sistem memakai satu sumber USB 5V. Relay, fitting, steker, kabel AC, dan lampu bohlam AC tidak dipakai pada implementasi utama.

## Target Pengguna

- Pengguna yang ingin mengontrol lampu dari jaringan WiFi lokal.
- Pengguna yang ingin menguji kendali lampu dengan deteksi tepuk tangan.
- Pengguna yang membutuhkan monitoring nilai sensor suara secara realtime.
- Pengguna yang membutuhkan prototype IoT mobile tanpa wiring PLN.

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
- LED 5V atau LED strip 5V pendek.
- Transistor NPN 2N2222 atau S8050.
- Resistor 1k ohm untuk base transistor.
- Resistor 220 ohm untuk LED satuan.

## Pin

| Fungsi | Pin ESP32 |
| --- | --- |
| KY-037 AO | GPIO34 |
| KY-037 DO | GPIO27 |
| Output lampu | GPIO26 |

## Batasan Teknis

- Framework: Arduino.
- Build system: PlatformIO.
- Library eksternal tidak digunakan.
- Library yang digunakan: `WiFi.h` dan `WebServer.h`.
- HTML, CSS, dan JavaScript dashboard disimpan langsung di `src/main.cpp`.
- Output LED 5V aktif HIGH: `HIGH` berarti LED ON, `LOW` berarti LED OFF.

## Risiko dan Catatan Keselamatan

Implementasi utama tidak memakai tegangan AC/PLN. Risiko utama ada pada batas arus USB, short circuit 5V, rating transistor, orientasi LED, dan panas komponen. Detail hardware ada di [5V LED Migration Guide](5v-led-migration.md).
