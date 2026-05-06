# Dokumentasi ClapControl IoT

Folder ini berisi dokumentasi utama untuk project ESP32 ClapControl IoT.

## Daftar Dokumen

| Dokumen | Isi |
| --- | --- |
| [Project Brief](project-brief.md) | Tujuan, fitur, komponen, dan batasan project |
| [Architecture](architecture.md) | Struktur firmware, data flow, state, dan batasan desain |
| [Architecture Decision Record](architecture-decision-record.md) | Keputusan teknis utama dan konsekuensinya |
| [Flow Overview](flow-overview.md) | Alur boot, web dashboard, API, dan clap detection |
| [API Contract](api-contract.md) | Endpoint JSON, response, validasi, dan contoh request |
| [Hardware Setup](hardware-setup.md) | Wiring ESP32, KY-037, LED 5V, transistor, dan kalibrasi |
| [5V LED Migration Guide](5v-led-migration.md) | Rationale migrasi dari relay/lampu AC ke LED 5V, transistor, dan satu sumber USB |
| [Operation Guide](operation-guide.md) | Cara build, upload, monitor, dan menggunakan dashboard |
| [Testing and Validation](testing-validation.md) | Checklist pengujian firmware, API, sensor, dan relay |
| [Troubleshooting](troubleshooting.md) | Masalah umum dan cara diagnosis |
| [Design](DESIGN.md) | Arah UI dashboard dan aturan visual |
| [Design Intent JSON](design-intent.json) | Kontrak desain machine-readable |

## Ringkasan

ClapControl IoT adalah firmware PlatformIO untuk ESP32 DevKit V1 yang membaca sensor suara KY-037, mengontrol LED 5V lewat transistor aktif HIGH, dan menyediakan dashboard web lokal untuk monitoring serta kontrol manual. Arah AC relay lama tidak lagi menjadi wiring utama.

Project ini sengaja dibuat dependency-free di luar Arduino core: firmware hanya memakai `WiFi.h` dan `WebServer.h`.
