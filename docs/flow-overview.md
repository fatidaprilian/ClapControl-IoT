# Flow Overview

## Boot Flow

```mermaid
flowchart TD
  A[Power on ESP32] --> B[Init Serial 115200]
  B --> C[Set pin mode sensor and lamp switch]
  C --> D[Set lamp output OFF]
  D --> E[Connect to WiFi]
  E --> F[Register HTTP routes]
  F --> G[Start WebServer on port 80]
  G --> H[Enter loop]
```

## Main Loop Flow

```mermaid
flowchart TD
  A[loop] --> B[Check WiFi connection]
  B --> C[Handle HTTP client]
  C --> D[Read analog sensor]
  D --> E{Analog >= threshold?}
  E -- No --> F[Rearm clap peak]
  E -- Yes --> G{Clap mode enabled?}
  G -- No --> H[Do nothing]
  G -- Yes --> I{Cooldown passed?}
  I -- No --> H
  I -- Yes --> J[Toggle 5V LED output]
  J --> K[Record last clap time and disarm peak]
  F --> L[Delay 5 ms]
  H --> L
  K --> L
```

## Dashboard Refresh Flow

```mermaid
sequenceDiagram
  participant Browser
  participant ESP32
  Browser->>ESP32: GET /
  ESP32-->>Browser: HTML dashboard
  loop every 500 ms
    Browser->>ESP32: GET /api/status
    ESP32-->>Browser: lamp, clap mode, sensor, threshold, RSSI
  end
```

## Manual Lamp Control Flow

```mermaid
sequenceDiagram
  participant User
  participant Browser
  participant ESP32
  User->>Browser: Click ON/OFF/TOGGLE
  Browser->>ESP32: GET /api/on or /api/off or /api/toggle
  ESP32->>ESP32: Write GPIO26 with active HIGH mapping
  ESP32-->>Browser: JSON status
  Browser->>User: Update lamp indicator
```

## Threshold Update Flow

```mermaid
sequenceDiagram
  participant User
  participant Browser
  participant ESP32
  User->>Browser: Move threshold slider
  Browser->>ESP32: GET /api/threshold?value=2200
  ESP32->>ESP32: Validate 0..4095
  ESP32-->>Browser: JSON status or 400 error
```

## Clap Detection Rules

1. Baca nilai analog KY-037 dari GPIO34.
2. Jika nilai analog lebih kecil dari threshold, sistem rearm untuk trigger berikutnya.
3. Jika nilai analog mencapai threshold dan clap mode aktif, cek cooldown.
4. Jika cooldown sudah lewat, toggle lampu.
5. Setelah trigger, sistem disarm sampai nilai analog turun lagi.

## Output Polarity Note

LED 5V dengan transistor NPN memakai aktif HIGH: ON menulis `HIGH`, OFF menulis `LOW`. Flow dashboard, API, threshold, dan clap detection tetap sama; mapping output fisik GPIO26 mengikuti transistor switch.
