# Hardware Setup

## Status Hardware

Implementasi utama sekarang memakai sistem 5V DC hemat daya:

```text
ESP32 DevKit V1 + KY-037 + LED 5V + transistor switch + satu sumber USB 5V
```

Relay, fitting, steker, kabel AC, dan lampu bohlam AC tidak dipakai. Jangan sambungkan tegangan PLN/AC ke rakitan ini.

## Komponen

- ESP32 DevKit V1 Type-C CP2102.
- Sensor suara KY-037.
- LED 5V satuan atau LED strip 5V pendek.
- Transistor NPN 2N2222 atau S8050.
- Resistor 1k ohm untuk base transistor.
- Resistor 220 ohm untuk LED satuan tanpa resistor bawaan.
- Kabel jumper.
- Sumber daya USB 5V dari komputer, adaptor USB, atau power bank.

Untuk LED strip yang lebih panjang atau arusnya lebih besar, gunakan logic-level N-MOSFET sebagai pengganti NPN kecil.

## Wiring ESP32 ke KY-037

| KY-037 | ESP32 | Catatan |
| --- | --- | --- |
| VCC | 3V3 atau 5V sesuai modul | Banyak modul KY-037 bisa memakai 3.3V atau 5V, cek modul yang digunakan |
| GND | GND | Ground harus tersambung bersama ESP32 |
| AO | GPIO34 | Input analog ADC, rentang baca 0 sampai 4095 |
| DO | GPIO27 | Input digital untuk monitoring |

## Wiring LED 5V Dengan Transistor NPN

GPIO26 tidak memberi daya langsung ke LED. GPIO26 hanya memberi sinyal ke base transistor melalui resistor 1k ohm.

| Bagian | Koneksi | Catatan |
| --- | --- | --- |
| ESP32 5V/VIN | Rail 5V breadboard | Sumber daya LED kecil dan sensor |
| ESP32 GND | Rail GND breadboard | Ground bersama |
| GPIO26 | Resistor 1k ohm ke base NPN | Sinyal active HIGH |
| Base NPN | Dari resistor 1k ohm | Jangan langsung ke GPIO tanpa resistor |
| Emitter NPN | GND rail | Low-side switch |
| Collector NPN | Sisi negatif LED/load | Arus LED mengalir lewat transistor |
| Sisi positif LED | 5V melalui resistor 220 ohm | Untuk LED satuan |

Rangkaian LED satuan:

```text
5V rail
  |
  |-- resistor 220 ohm -- anode LED
                            cathode LED -- collector NPN
                                           emitter NPN -- GND rail

GPIO26 -- resistor 1k ohm -- base NPN
```

Rangkaian LED strip 5V pendek:

```text
5V rail -- LED strip +5V
LED strip negative/GND -- collector NPN
emitter NPN -- GND rail

GPIO26 -- resistor 1k ohm -- base NPN
```

Banyak LED strip 5V sudah punya resistor atau rangkaian pembatas arus. Jangan menambahkan resistor 220 ohm seri untuk seluruh strip kecuali spesifikasi strip memang memerlukannya.

## Common Ground Wajib

Semua ground harus tersambung:

```text
ESP32 GND = KY-037 GND = emitter transistor = USB 5V ground
```

Tanpa common ground, sinyal GPIO26 tidak punya referensi yang benar untuk menyalakan transistor.

## Logika Output Firmware

Transistor NPN low-side memakai active HIGH:

| Status LED | GPIO26 |
| --- | --- |
| ON | `HIGH` |
| OFF | `LOW` |

Saat ESP32 boot, firmware memanggil `setLamp(false)` agar LED berada pada kondisi OFF.

## Batas Arus USB

Satu kabel USB membuat rakitan lebih ringkas, tetapi arus tetap terbatas.

- Single LED dengan resistor 220 ohm aman untuk pengujian awal.
- Beberapa LED kecil masih masuk akal jika total arus rendah.
- LED strip panjang tidak disarankan lewat breadboard kecil.
- Jika ESP32 reset saat LED menyala, beban LED terlalu besar atau supply USB drop.
- Jika transistor panas, matikan rangkaian dan kurangi beban.

## Kalibrasi KY-037

1. Upload firmware dan buka dashboard.
2. Amati nilai analog saat ruangan tenang.
3. Tepuk tangan di jarak penggunaan normal dan catat puncak nilai analog.
4. Atur threshold sedikit di atas noise normal dan di bawah puncak tepuk.
5. Jika sering false trigger, naikkan threshold.
6. Jika tepuk tidak terdeteksi, turunkan threshold atau atur trimpot KY-037.

## Catatan Pin ESP32

GPIO34 adalah input-only, cocok untuk AO sensor. Pin ini tidak bisa dipakai sebagai output.

GPIO26 dipakai sebagai output kontrol transistor. Jangan sambungkan LED langsung ke GPIO26.

## Upload Firmware

1. Sambungkan ESP32 ke komputer via USB Type-C.
2. Pastikan board terdeteksi oleh PlatformIO.
3. Jalankan upload dari PlatformIO.
4. Jika gagal masuk mode flash, tahan tombol `BOOT` saat proses upload mulai, lalu lepas setelah writing berjalan.
