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
- Tombol ON menyalakan LED 5V.
- Tombol OFF mematikan LED 5V.
- Tombol TOGGLE membalik status LED.
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

## 5V LED Output Check

- Saat ESP32 boot, LED harus OFF.
- Perintah ON harus membuat GPIO26 `HIGH`.
- Perintah OFF harus membuat GPIO26 `LOW`.
- LED tidak boleh tersambung langsung ke GPIO ESP32.
- Resistor 1k ohm harus berada antara GPIO26 dan base transistor.
- LED satuan harus memakai resistor pembatas arus, misalnya 220 ohm.
- Emitter transistor harus ke GND bersama.
- Collector transistor harus ke sisi negatif LED/load.
- ESP32 tidak boleh reset berulang saat LED ON.
- Transistor tidak boleh panas berlebihan.
- Untuk LED strip yang lebih besar, validasi total arus USB dan pertimbangkan logic-level N-MOSFET.

## Regression Checklist

- Tidak ada library eksternal di `platformio.ini`.
- Endpoint tetap mengembalikan JSON.
- Threshold menolak nilai di luar `0..4095`.
- UI tetap auto-refresh setiap 500 ms.
- DO sensor tetap tampil di `/api/status`.
- Output lampu tetap active HIGH untuk transistor switch.
