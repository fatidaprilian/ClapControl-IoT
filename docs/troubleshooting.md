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

## LED 5V Tidak Sesuai Status Dashboard

Jika LED ON saat dashboard OFF, kemungkinan mapping firmware salah atau transistor/wiring tertukar.

Solusi:

1. Pastikan firmware memakai `HIGH` untuk ON dan `LOW` untuk OFF.
2. Pastikan GPIO26 melewati resistor 1k ohm ke base transistor.
3. Pastikan emitter ke GND bersama.
4. Pastikan collector ke sisi negatif LED/load.
5. Pastikan LED satuan memakai resistor seri.
6. Jika LED strip menarik arus besar, ganti transistor NPN kecil dengan logic-level N-MOSFET.

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
- Sensor menangkap getaran meja atau lingkungan sekitar.

Solusi:

1. Naikkan threshold.
2. Jauhkan sensor dari sumber getaran.
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
