# Hardware Setup

## Komponen

- ESP32 DevKit V1 Type-C CP2102.
- Sensor suara KY-037.
- Relay 1 channel 5V, aktif LOW.
- Lampu AC, fitting, dan steker.
- Kabel jumper.
- Power supply yang sesuai untuk ESP32 dan relay.

## Wiring ESP32 ke KY-037

| KY-037 | ESP32 | Catatan |
| --- | --- | --- |
| VCC | 3V3 atau 5V sesuai modul | Banyak modul KY-037 bisa memakai 3.3V atau 5V, cek modul yang digunakan |
| GND | GND | Ground harus tersambung bersama ESP32 |
| AO | GPIO34 | Input analog ADC, rentang baca 0 sampai 4095 |
| DO | GPIO27 | Input digital untuk monitoring |

## Wiring ESP32 ke Relay

| Relay | ESP32 / Supply | Catatan |
| --- | --- | --- |
| IN | GPIO26 | Relay aktif LOW |
| GND | GND ESP32 dan supply relay | Ground harus common |
| VCC | 5V relay supply | Gunakan supply yang cukup untuk coil relay |

## Wiring Relay ke Lampu AC

Umumnya wiring AC memakai terminal relay:

| Terminal Relay | Koneksi |
| --- | --- |
| COM | Jalur live dari steker |
| NO | Jalur live ke lampu |
| NC | Tidak digunakan untuk mode default OFF |

Dengan konfigurasi COM ke NO, lampu default mati saat relay tidak aktif.

## Peringatan Listrik AC

- Matikan sumber listrik sebelum wiring.
- Jangan sentuh sisi AC ketika perangkat tersambung listrik.
- Gunakan enclosure dan isolasi yang baik.
- Pisahkan kabel AC dari kabel sinyal ESP32.
- Jika tidak yakin, minta bantuan teknisi listrik.

## Kalibrasi KY-037

1. Upload firmware dan buka dashboard.
2. Amati nilai analog saat ruangan tenang.
3. Tepuk tangan di jarak penggunaan normal dan catat puncak nilai analog.
4. Atur threshold sedikit di atas noise normal dan di bawah puncak tepuk.
5. Jika sering false trigger, naikkan threshold.
6. Jika tepuk tidak terdeteksi, turunkan threshold atau atur trimpot KY-037.

## Catatan Pin ESP32

GPIO34 adalah input-only, cocok untuk AO sensor. Pin ini tidak bisa dipakai sebagai output.

## Upload Firmware

1. Sambungkan ESP32 ke komputer via USB Type-C.
2. Pastikan board terdeteksi oleh PlatformIO.
3. Jalankan upload dari PlatformIO.
4. Jika gagal masuk mode flash, tahan tombol `BOOT` saat proses upload mulai, lalu lepas setelah writing berjalan.
