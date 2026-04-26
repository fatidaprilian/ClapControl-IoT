# Testing and Validation

## Build Check

Jalankan:

```bash
pio run
```

Build dinyatakan valid jika PlatformIO menampilkan `SUCCESS`.

## WiFi Check

1. Buka Serial Monitor di `115200`.
2. Reset ESP32.
3. Pastikan muncul alamat IP dashboard.
4. Buka alamat IP tersebut dari browser di jaringan yang sama.

## Dashboard Check

- Status lampu tampil sebagai ON atau OFF.
- Tombol ON menyalakan relay.
- Tombol OFF mematikan relay.
- Tombol TOGGLE membalik status relay.
- Clap mode bisa berubah ON/OFF.
- Nilai analog berubah ketika ada suara.
- Slider threshold mengubah nilai threshold.

## API Check

Contoh dengan browser atau HTTP client:

```text
GET http://<ip-esp32>/api/status
GET http://<ip-esp32>/api/on
GET http://<ip-esp32>/api/off
GET http://<ip-esp32>/api/toggle
GET http://<ip-esp32>/api/clap-mode
GET http://<ip-esp32>/api/threshold?value=2200
```

## Clap Detection Check

1. Aktifkan clap mode.
2. Set threshold awal sekitar `2200`.
3. Tepuk sekali dan amati apakah lampu toggle satu kali.
4. Tepuk dua kali sangat cepat dan pastikan cooldown mencegah trigger ganda yang tidak diinginkan.
5. Sesuaikan threshold jika ruangan terlalu bising atau sensor kurang sensitif.

## Relay Safety Check

- Saat ESP32 boot, relay harus berada pada kondisi OFF.
- Perintah ON harus membuat GPIO26 `LOW`.
- Perintah OFF harus membuat GPIO26 `HIGH`.
- Jika perilaku terbalik, cek apakah modul relay yang dipakai benar-benar aktif LOW.

## Regression Checklist

- Tidak ada library eksternal di `platformio.ini`.
- Endpoint tetap mengembalikan JSON.
- Threshold menolak nilai di luar `0..4095`.
- UI tetap auto-refresh setiap 500 ms.
- DO sensor tetap tampil di `/api/status`.
