# Flow Overview

## Boot Flow

```mermaid
flowchart TD
  A[Power on ESP32] --> B[Start Serial 115200]
  B --> C[Set relay and KY-037 pin modes]
  C --> D[Set relay OFF]
  D --> E[Connect WiFi and Blynk]
  E --> F[Register telemetry timer]
  F --> G[Enter loop]
```

## Main Loop Flow

```mermaid
flowchart TD
  A[loop] --> B[Blynk.run]
  B --> C[timer.run]
  C --> D[Read KY-037 DO on GPIO22]
  D --> E{DO active LOW?}
  E -- No --> F[Rearm sound peak]
  E -- Yes --> G{Clap mode enabled and cooldown passed?}
  G -- No --> A
  G -- Yes --> H[Toggle relay GPIO2]
  H --> I[Publish V0 lamp state]
  I --> J[Record last clap time and disarm peak]
  F --> A
  J --> A
```

## Blynk Control Flow

```mermaid
sequenceDiagram
  participant User
  participant Blynk
  participant ESP32
  participant Relay
  User->>Blynk: Set V0 or press V2
  Blynk->>ESP32: Virtual pin update
  ESP32->>Relay: Write GPIO2 HIGH or LOW
  ESP32-->>Blynk: Publish lamp state
```

## Clap Detection Rules

1. Read KY-037 DO from GPIO22.
2. Treat `LOW` as sound detected.
3. Rearm only after DO returns inactive.
4. Ignore triggers inside the `650 ms` cooldown window.
5. Toggle relay GPIO2 after a valid clap.
6. Publish the new relay state to Blynk.
