# Design Intent

## Design Intent and Product Personality

The dashboard behaves like a compact bench instrument for a live IoT lamp controller: direct controls first, sensor telemetry second, and configuration within thumb reach.

## Audience and Use-Context Signals

The user is likely testing near the lamp and sensor on a phone or laptop inside the same WiFi network. The interface must be readable quickly and avoid dense navigation.

## Visual Direction and Distinctive Moves

The conceptual anchor is a portable electrical test meter: strong numeric readout, high-contrast status indicators, and control surfaces that feel immediate.

## Color, Typography, Spacing, and Density Decisions

Dark background reduces glare during lamp testing. Green indicates LED/on behavior, red is reserved for OFF or risk actions, and yellow highlights clap mode and threshold attention.

## Token Architecture and Alias Strategy

CSS variables define primitive color and surface tokens, then components consume semantic roles such as accent, danger, muted, panel, and line.

## Responsive Recomposition Plan

Desktop shows command and telemetry side by side. Mobile stacks the status hero first, then promotes large ON/OFF/TOGGLE controls as full-width touch targets.

## Motion, Interaction, and Feedback Rules

Motion is limited to button press feedback, hover lift, status glow, and analog meter smoothing. Reduced-motion disables transitions.

## Component Language, States, and Morphology

Cards have restrained 8px radius, buttons are large, focus-visible states are explicit, and live status text is exposed through ARIA regions.

## Source Boundaries and Context Hygiene

The design is derived only from the current embedded IoT brief and existing PlatformIO project shape. No external UI libraries or copied visual references are used.

## Accessibility Non-Negotiables

Controls need visible focus states, readable contrast, large touch targets, and status updates that are not conveyed by color alone.

## Anti-Patterns to Avoid

Avoid decorative dashboards that hide controls, tiny mobile buttons, color-only lamp state, or external assets that burden the ESP32 web server.

## Implementation Notes for Future UI Tasks

Keep the UI dependency-free unless the hardware target changes. Any future charting or richer animation should be tested against ESP32 memory and response limits first.
