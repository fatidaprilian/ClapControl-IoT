# Troubleshooting

## ESP32 Tidak Terkoneksi WiFi

Kemungkinan penyebab:

- SSID atau password salah.
- WiFi hanya 5 GHz.
- Sinyal terlalu lemah.
- Router memblokir perangkat baru.

Langkah diagnosis:

1. Cek Serial Monitor `115200`.
2. Pastikan SSID adalah jaringan 2.4 GHz.
3. Dekatkan ESP32 ke router.
4. Reset ESP32 setelah mengubah kredensial.

## Dashboard Tidak Bisa Dibuka

Kemungkinan penyebab:

- Komputer atau HP tidak satu jaringan dengan ESP32.
- IP ESP32 berubah setelah reconnect.
- Browser memakai cache lama.

Langkah diagnosis:

1. Lihat IP terbaru di Serial Monitor.
2. Buka `http://<ip-esp32>` tanpa HTTPS.
3. Refresh browser.

## Relay Tidak Menyala

Kemungkinan penyebab:

- Relay membutuhkan supply 5V yang lebih stabil.
- Ground ESP32 dan relay tidak common.
- Kabel IN relay tidak ke GPIO26.
- Modul relay yang digunakan bukan aktif LOW.

Langkah diagnosis:

1. Coba endpoint `/api/on` dan `/api/off`.
2. Dengarkan bunyi klik relay.
3. Ukur GPIO26 jika punya multimeter.
4. Cek label IN, VCC, dan GND pada modul relay.

## Lampu Terbalik ON/OFF

Jika lampu ON saat status dashboard OFF, kemungkinan relay module aktif HIGH atau wiring AC memakai terminal NC.

Solusi:

- Pindahkan wiring AC dari NC ke NO.
- Jika modul relay aktif HIGH, ubah mapping relay di firmware dengan hati-hati.

## Clap Tidak Terdeteksi

Kemungkinan penyebab:

- Threshold terlalu tinggi.
- Sensor terlalu jauh dari sumber suara.
- Trimpot KY-037 kurang sensitif.
- AO tidak terhubung ke GPIO34.

Solusi:

1. Lihat nilai analog di dashboard ketika tepuk tangan.
2. Turunkan threshold secara bertahap.
3. Atur trimpot KY-037.
4. Pastikan AO ke GPIO34 dan GND tersambung.

## Terlalu Sering False Trigger

Kemungkinan penyebab:

- Threshold terlalu rendah.
- Lingkungan terlalu bising.
- Sensor menangkap getaran relay atau meja.

Solusi:

1. Naikkan threshold.
2. Jauhkan sensor dari relay atau sumber getaran.
3. Pastikan clap mode dimatikan ketika tidak dibutuhkan.

## Upload Gagal

Kemungkinan penyebab:

- Driver CP2102 belum terpasang.
- Port serial sedang dipakai Serial Monitor.
- Board tidak masuk bootloader mode.

Solusi:

1. Tutup Serial Monitor.
2. Cabut dan pasang ulang ESP32.
3. Tahan tombol `BOOT` saat upload mulai.
4. Pastikan driver CP2102 sudah terpasang.
