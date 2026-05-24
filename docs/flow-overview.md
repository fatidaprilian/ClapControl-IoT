# Flow Overview

## Boot Flow

```mermaid
flowchart TD
  A[Power on ESP32] --> B[Start Serial 115200]
  B --> C[Set relay and KY-037 pin modes]
  C --> D[Set relay OFF]
  D --> E[Connect WiFi]
  E --> F[Register web routes]
  F --> G[Enter loop]
```

## Main Loop Flow

```mermaid
flowchart TD
  A[loop] --> B[Handle web client]
  B --> C[Read KY-037 AO on GPIO34]
  C --> D{AO above threshold?}
  D -- No --> F[Rearm sound peak]
  D -- Yes --> G{Clap mode enabled, hold met, cooldown passed?}
  G -- No --> A
  G -- Yes --> H[Toggle relay GPIO25]
  H --> I[Increment clap count]
  I --> J[Record last clap time and disarm peak]
  F --> A
  J --> A
```

## Web Control Flow

```mermaid
sequenceDiagram
  participant User
  participant ESP32
  participant Relay
  User->>ESP32: Open dashboard over local WiFi
  User->>ESP32: GET /api/relay or /api/sensitivity
  ESP32->>Relay: Write GPIO25 HIGH or LOW
  ESP32-->>User: Return JSON status
```

## Clap Detection Rules

1. Read KY-037 AO from GPIO34.
2. Treat values at or above the web threshold as active sound.
3. Require the active signal to hold for `minActiveMs`.
4. Rearm only after the analog value drops below threshold.
5. Ignore triggers inside the configured cooldown window.
6. Toggle relay GPIO25 after a valid clap.
