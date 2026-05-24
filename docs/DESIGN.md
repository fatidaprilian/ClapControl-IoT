# Design Intent

## Design Intent and Product Personality

ClapControl is a local signal-calibration surface for an ESP32 lamp controller. The UI should feel like a small audio threshold instrument: direct, readable, and tuned for quick physical testing beside the breadboard.

## Product Reading

- Product type and core verb: Embedded IoT control panel where the user calibrates a sound threshold and switches a relay.
- Highest-stakes moments: first, seeing whether the sensor crosses threshold; second, forcing relay ON/OFF; third, tuning cooldown so one clap does not double-toggle.
- Dominant data shapes: live telemetry, threshold boundary, binary relay state, and small control values.
- Latency profile: soft real-time because the screen polls ESP32 status several times per second.
- Failure modes: stale/offline dashboard, noisy sensor, threshold too high, relay polarity mismatch, and unsafe bulb wiring.
- Context of use: sustained bench testing on a laptop or phone in the same WiFi network.
- Constraints: ESP32-hosted HTML/CSS/JS, no external UI libraries, WCAG 2.2 AA intent, small flash/RAM budget.

## Audience and Use-Context Signals

The primary user is physically beside the ESP32, relay, bulb, and KY-037 module. The screen must support quick bench calibration, not long-form analytics. The first viewport must prove whether AO crosses the threshold and whether the relay is ON.

## Visual Direction and Distinctive Moves

The selected direction is a compact signal instrument, not a generic IoT dashboard. The first viewport exposes a large vertical signal meter, a threshold marker, a peak marker, relay state, clap mode, and tuning controls.

## Reference Intake

- High-performance HMI principles: borrow reserved alarm color and state hierarchy; do not copy industrial grayscale screens or SCADA layout.
- VU/peak meter behavior: borrow live level plus peak-hold and threshold line; do not copy vintage meter skins.
- Audio level UI guidance: borrow fast visible peak feedback; do not copy plugin chrome or studio-console styling.

Sources checked on 2026-05-20: W3C WCAG 2.2 notes for target/focus expectations, MDN CSS Grid guidance for responsive major regions, MDN `prefers-reduced-motion` guidance, industrial HMI design summaries, VU meter and peak meter references, and audio GUI meter behavior notes.

## Category Codes Rejected

- Typography cliche: tiny monospace labels on dark slate with generic terminal numerals. Rejected because it makes the device feel like a dev console instead of a calibration instrument.
- Palette cliche: neon-green-on-black hacker control panel with glowing cards. Rejected because color loses status meaning and reads as decoration.
- Layout cliche: three KPI cards above sliders in a generic admin dashboard. Rejected because it hides the signal/threshold relationship.
- Motion cliche: smooth fades on every card. Rejected because motion must explain signal changes, not decorate the shell.
- Imagery cliche: abstract circuit wallpaper or grid background. Rejected because the page needs measurement behavior, not electronics-themed filler.

## Morphological Exploration

| Dimension | Option A | Option B | Option C | Option D | Option E |
| --- | --- | --- | --- | --- | --- |
| Hierarchy | signal meter first | relay first | sensor table first | setup checklist first | compact mobile remote |
| Density | bench readout | sparse remote | alarm wall | oscilloscope strip | phone tile |
| State language | armed/idle/accepted | online/offline only | fault-first | calibration bands | event log |
| Motion | peak-hold decay | button pulse | route-like sweep | static states | waveform scroll |
| Color behavior | threshold amber | relay green | alarm red only | monochrome HMI | pastel consumer |

Selected combination: signal meter first, bench readout density, armed/idle/accepted state language, peak-hold decay, and threshold amber.

Uncomfortable combination: alarm wall plus monochrome HMI. It feels too severe for a small home lamp project, but it could work if the product became a safety-focused mains relay tester.

## Anchor Candidates

Selected anchor: **digital peak programme meter with threshold marker**.

Other considered anchors:

- Laboratory bench counter: strong numeric hierarchy, but too generic for sound.
- Guitar pedal tuner: clear threshold feedback, but too music-branded.
- Aircraft annunciator strip: excellent binary states, too safety-critical.
- Thermal printer status panel: compact and hardware-like, but weak for live amplitude.
- Digital peak programme meter: best fit because it combines live level, peak-hold, and threshold crossing.

Rename test for selected anchor: incoherent as a recipe app, coherent only weakly as a battery tester, incoherent as an ecommerce admin. Verdict: PASS.

## Motion and Palette Decision

Motion: live analog fill updates quickly, peak marker decays slower, and threshold marker moves immediately when tuned.

Palette: graphite instrument surface, amber threshold, green relay-on state, red destructive/off action, blue peak marker.

Typography: large tabular numeric ADC values contrast with compact uppercase hardware labels.

## Color, Typography, Spacing, and Density Decisions

Color uses graphite surfaces so status colors stay meaningful. Amber means threshold, green means relay/power acceptance, blue means peak memory, and red means destructive or error state. Typography uses large tabular numerals for ADC values and compact uppercase labels for hardware mapping. Spacing follows an 8px base so the meter, controls, and readouts stay dense without becoming cramped.

## Token Architecture and Alias Strategy

Tokens are embedded as CSS custom properties in `include/web_ui.h`. Alias roles are named by function: `--relay`, `--threshold`, `--danger`, `--peak`, `--panel`, `--panel-2`, `--text`, and `--muted`.

## Responsive Recomposition Plan

Desktop shows the meter and controls side by side. Mobile stacks the live signal first, then relay controls, then calibration sliders. The threshold meter remains visible before secondary telemetry.

## Motion, Interaction, and Feedback Rules

The analog fill updates quickly, the peak marker decays slower, and the threshold marker moves immediately while tuning. Button actions return JSON state and update the visible relay state. `aria-live` status text reports connection and command results.

## Component Language, States, and Morphology

Components use instrument-like panels with hard 8px radii, clear labels, and stable dimensions. Controls expose default, hover, focus-visible, active browser behavior, success message, and recoverable fetch failure text.

## Source Boundaries and Context Hygiene

The UI may borrow mechanics from measurement instruments and accessibility references. It must not copy external product layout, palette, component skin, brand posture, or Blynk widget structure.

## Accessibility Non-Negotiables

Use text labels beside color states, large touch targets, visible focus, readable contrast, and `aria-live` status messaging. Reduced-motion users still receive numeric updates without needing animated meter movement.

## Anti-Patterns to Avoid

Do not ship decorative circuit backgrounds, generic card stacks, terminal-only calibration, hidden relay state, or Blynk-like widget mimicry.

## Implementation Notes for Future UI Tasks

Keep the dashboard dependency-free unless a future feature needs a real charting or motion library. If a library is added later, verify official docs first and record it in `docs/design-intent.json`.
