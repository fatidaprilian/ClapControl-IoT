#pragma once

#include <Arduino.h>

const char DASHBOARD_HTML[] PROGMEM = R"HTML(
<!doctype html>
<html lang="id">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ClapControl Signal Desk</title>
  <link rel="stylesheet" href="/styles.css">
</head>
<body>
  <main class="desk" aria-label="ClapControl signal desk">
    <section class="scope" aria-label="Sound sensor monitor">
      <div class="masthead">
        <p class="eyebrow">ESP32 / KY-037 / RELAY</p>
        <h1>Signal Desk</h1>
        <p>Kalibrasi tepukan dari nilai analog, lihat puncaknya, lalu kunci relay saat sinyal benar-benar lewat ambang.</p>
      </div>

      <div class="meter" aria-hidden="true">
        <div id="thresholdLine" class="threshold-line"><span id="thresholdBadge">1800</span></div>
        <div id="peakLine" class="peak-line"></div>
        <div id="levelFill" class="level-fill"></div>
      </div>

      <div class="readout-grid">
        <div class="readout primary">
          <span>Analog AO</span>
          <strong id="analogValue">0</strong>
        </div>
        <div class="readout">
          <span>Peak hold</span>
          <strong id="peakValue">0</strong>
        </div>
        <div class="readout">
          <span>Trigger</span>
          <strong id="soundState">IDLE</strong>
        </div>
        <div class="readout">
          <span>Digital DO</span>
          <strong id="digitalState">IDLE</strong>
        </div>
      </div>
    </section>

    <section class="controls" aria-label="Relay and sensitivity controls">
      <div class="relay-strip">
        <button id="onButton" type="button">ON</button>
        <button id="offButton" class="danger" type="button">OFF</button>
        <button id="toggleButton" class="secondary" type="button">TOGGLE</button>
      </div>

      <div class="state-row">
        <div>
          <span>Relay</span>
          <strong id="relayState">OFF</strong>
        </div>
        <div>
          <span>Clap mode</span>
          <label class="switch">
            <input id="clapMode" type="checkbox" checked>
            <i></i>
          </label>
        </div>
      </div>

      <div class="tuner">
        <label for="threshold">Threshold analog</label>
        <output id="thresholdValue">1800</output>
        <input id="threshold" type="range" min="50" max="4095" step="25" value="1800">
      </div>

      <div class="tuner two">
        <div>
          <label for="holdMs">Hold aktif</label>
          <output id="holdValue">5 ms</output>
          <input id="holdMs" type="range" min="5" max="250" step="5" value="5">
        </div>
        <div>
          <label for="cooldownMs">Cooldown</label>
          <output id="cooldownValue">650 ms</output>
          <input id="cooldownMs" type="range" min="200" max="2000" step="50" value="650">
        </div>
      </div>

      <div class="telemetry">
        <div><span>Event</span><strong id="clapCount">0</strong></div>
        <div><span>WiFi</span><strong id="wifiRssi">0 dBm</strong></div>
        <div><span>Uptime</span><strong id="uptime">0s</strong></div>
      </div>

      <p id="message" class="message" role="status" aria-live="polite"></p>
    </section>
  </main>
  <script src="/app.js"></script>
</body>
</html>
)HTML";

const char DASHBOARD_CSS[] PROGMEM = R"CSS(
:root {
  color-scheme: dark;
  --bg: #0e1114;
  --panel: #171d21;
  --panel-2: #20282e;
  --line: rgba(234, 242, 246, 0.14);
  --text: #f4f8fa;
  --muted: #a9b8c2;
  --relay: #67d6ad;
  --threshold: #ffbf47;
  --danger: #f35f62;
  --peak: #87b7ff;
  font-family: ui-sans-serif, system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
}

* { box-sizing: border-box; }

body {
  margin: 0;
  min-height: 100vh;
  color: var(--text);
  background: #0b0f12;
}

.desk {
  width: min(1180px, calc(100% - 24px));
  min-height: 100vh;
  margin: 0 auto;
  padding: 18px 0;
  display: grid;
  grid-template-columns: minmax(0, 1.35fr) minmax(320px, 0.75fr);
  gap: 14px;
}

.scope,
.controls {
  border: 1px solid var(--line);
  border-radius: 8px;
  background: var(--panel);
  overflow: hidden;
}

.scope {
  min-height: 620px;
  display: grid;
  grid-template-rows: auto 1fr auto;
}

.masthead {
  padding: 26px;
}

.eyebrow {
  margin: 0 0 10px;
  color: var(--relay);
  font-size: 0.78rem;
  font-weight: 900;
  letter-spacing: 0.08em;
}

h1 {
  margin: 0;
  max-width: 680px;
  font-size: clamp(3.4rem, 10vw, 8.4rem);
  line-height: 0.82;
  letter-spacing: 0;
}

.masthead p:last-child {
  max-width: 660px;
  color: var(--muted);
  line-height: 1.65;
}

.meter {
  position: relative;
  margin: 0 26px;
  min-height: 280px;
  border: 1px solid var(--line);
  border-radius: 8px;
  background:
    linear-gradient(to top, rgba(103, 214, 173, 0.08), rgba(255, 191, 71, 0.08), rgba(243, 95, 98, 0.12)),
    var(--panel-2);
  overflow: hidden;
}

.level-fill {
  position: absolute;
  inset: auto 0 0;
  height: 0%;
  background: linear-gradient(to top, var(--relay), var(--threshold), var(--danger));
  transition: height 160ms ease;
}

.threshold-line,
.peak-line {
  position: absolute;
  inset-inline: 0;
  height: 2px;
  z-index: 2;
}

.threshold-line {
  bottom: 44%;
  background: var(--threshold);
}

.threshold-line span {
  position: absolute;
  inset-inline-start: 12px;
  inset-block-start: -28px;
  padding: 4px 8px;
  border-radius: 6px;
  background: var(--threshold);
  color: #171104;
  font-weight: 900;
}

.peak-line {
  bottom: 0%;
  background: var(--peak);
  box-shadow: 0 0 18px rgba(135, 183, 255, 0.7);
  transition: bottom 260ms ease;
}

.readout-grid,
.telemetry,
.state-row,
.relay-strip {
  display: grid;
  gap: 10px;
}

.readout-grid {
  grid-template-columns: 1.35fr repeat(3, minmax(0, 1fr));
  padding: 18px 26px 26px;
}

.readout,
.state-row > div,
.telemetry > div,
.tuner {
  border: 1px solid var(--line);
  border-radius: 8px;
  background: var(--panel-2);
}

.readout,
.state-row > div,
.telemetry > div {
  padding: 14px;
}

.readout span,
.state-row span,
.telemetry span,
.tuner label {
  display: block;
  color: var(--muted);
  font-size: 0.76rem;
  font-weight: 900;
  letter-spacing: 0.04em;
  text-transform: uppercase;
}

.readout strong,
.state-row strong,
.telemetry strong {
  display: block;
  margin-top: 6px;
  font-size: 1.45rem;
}

.readout.primary strong {
  font-size: clamp(2.4rem, 8vw, 4.7rem);
  line-height: 0.95;
  font-variant-numeric: tabular-nums;
}

.controls {
  padding: 18px;
  display: grid;
  align-content: start;
  gap: 14px;
}

.relay-strip {
  grid-template-columns: repeat(3, minmax(0, 1fr));
}

button {
  min-height: 54px;
  border: 0;
  border-radius: 8px;
  background: var(--relay);
  color: #061610;
  font: inherit;
  font-weight: 900;
  cursor: pointer;
}

button:hover {
  filter: brightness(1.06);
}

button:focus-visible,
input:focus-visible,
.switch input:focus-visible + i {
  outline: 3px solid var(--threshold);
  outline-offset: 3px;
}

button.danger {
  background: var(--danger);
  color: white;
}

button.secondary {
  background: #2a333a;
  color: white;
  border: 1px solid var(--line);
}

.state-row {
  grid-template-columns: 1fr 1fr;
}

.switch {
  display: inline-flex;
  align-items: center;
  margin-top: 8px;
}

.switch input {
  position: absolute;
  opacity: 0;
}

.switch i {
  width: 54px;
  height: 30px;
  border-radius: 999px;
  background: #49535a;
  position: relative;
}

.switch i::after {
  content: "";
  position: absolute;
  width: 22px;
  height: 22px;
  inset-block-start: 4px;
  inset-inline-start: 4px;
  border-radius: 50%;
  background: white;
  transition: transform 160ms ease;
}

.switch input:checked + i {
  background: var(--relay);
}

.switch input:checked + i::after {
  transform: translateX(24px);
}

.tuner {
  padding: 14px;
}

.tuner.two {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 12px;
}

.tuner.two > div {
  min-width: 0;
}

.tuner output {
  display: block;
  margin: 6px 0 10px;
  font-size: 1.4rem;
  font-weight: 900;
  font-variant-numeric: tabular-nums;
}

input[type="range"] {
  width: 100%;
  accent-color: var(--threshold);
}

.telemetry {
  grid-template-columns: repeat(3, minmax(0, 1fr));
}

.message {
  min-height: 24px;
  margin: 0;
  color: var(--muted);
}

@media (max-width: 860px) {
  .desk,
  .readout-grid,
  .tuner.two,
  .telemetry,
  .state-row {
    grid-template-columns: 1fr;
  }

  .scope {
    min-height: auto;
  }

  .meter {
    min-height: 220px;
  }
}

@media (prefers-reduced-motion: reduce) {
  *,
  *::before,
  *::after {
    transition-duration: 0.01ms !important;
    animation-duration: 0.01ms !important;
    animation-iteration-count: 1 !important;
    scroll-behavior: auto !important;
  }
}
)CSS";

const char DASHBOARD_JS[] PROGMEM = R"JS(
const relayState = document.getElementById("relayState");
const soundState = document.getElementById("soundState");
const digitalState = document.getElementById("digitalState");
const analogValue = document.getElementById("analogValue");
const peakValue = document.getElementById("peakValue");
const levelFill = document.getElementById("levelFill");
const peakLine = document.getElementById("peakLine");
const thresholdLine = document.getElementById("thresholdLine");
const thresholdBadge = document.getElementById("thresholdBadge");
const clapMode = document.getElementById("clapMode");
const threshold = document.getElementById("threshold");
const thresholdValue = document.getElementById("thresholdValue");
const holdMs = document.getElementById("holdMs");
const holdValue = document.getElementById("holdValue");
const cooldownMs = document.getElementById("cooldownMs");
const cooldownValue = document.getElementById("cooldownValue");
const clapCount = document.getElementById("clapCount");
const uptime = document.getElementById("uptime");
const wifiRssi = document.getElementById("wifiRssi");
const message = document.getElementById("message");

let pendingTimer = null;

async function requestJson(url) {
  const response = await fetch(url, { cache: "no-store" });
  if (!response.ok) throw new Error("HTTP " + response.status);
  return response.json();
}

function percent(value) {
  return Math.max(0, Math.min(100, Math.round((value / 4095) * 100)));
}

function render(data) {
  relayState.textContent = data.relayOn ? "ON" : "OFF";
  soundState.textContent = data.soundActive ? "ACTIVE" : "IDLE";
  digitalState.textContent = data.digitalActive ? "ACTIVE" : "IDLE";
  analogValue.textContent = data.analog;
  peakValue.textContent = data.peak;

  levelFill.style.height = percent(data.analog) + "%";
  peakLine.style.bottom = percent(data.peak) + "%";
  thresholdLine.style.bottom = percent(data.threshold) + "%";
  thresholdBadge.textContent = data.threshold;

  clapMode.checked = data.clapMode;

  if (document.activeElement !== threshold) {
    threshold.value = data.threshold;
    thresholdValue.textContent = data.threshold;
  }

  if (document.activeElement !== holdMs) {
    holdMs.value = data.minActiveMs;
    holdValue.textContent = data.minActiveMs + " ms";
  }

  if (document.activeElement !== cooldownMs) {
    cooldownMs.value = data.cooldownMs;
    cooldownValue.textContent = data.cooldownMs + " ms";
  }
  clapCount.textContent = data.clapCount;
  uptime.textContent = Math.floor(data.uptimeMs / 1000) + "s";
  wifiRssi.textContent = data.wifiRssi + " dBm";
}

async function send(url, okMessage) {
  try {
    render(await requestJson(url));
    message.textContent = okMessage;
  } catch (error) {
    message.textContent = "Perintah gagal dikirim.";
  }
}

async function refresh() {
  try {
    render(await requestJson("/api/status"));
    message.textContent = "Terhubung ke ESP32.";
  } catch (error) {
    message.textContent = "Gagal mengambil status ESP32.";
  }
}

function scheduleSensitivityUpdate() {
  clearTimeout(pendingTimer);
  thresholdValue.textContent = threshold.value;
  holdValue.textContent = holdMs.value + " ms";
  cooldownValue.textContent = cooldownMs.value + " ms";
  thresholdLine.style.bottom = percent(Number(threshold.value)) + "%";
  thresholdBadge.textContent = threshold.value;
  pendingTimer = setTimeout(() => {
    send("/api/sensitivity?threshold=" + encodeURIComponent(threshold.value) +
      "&hold=" + encodeURIComponent(holdMs.value) +
      "&cooldown=" + encodeURIComponent(cooldownMs.value), "Kalibrasi diperbarui.");
  }, 180);
}

document.getElementById("onButton").addEventListener("click", () => send("/api/relay?state=on", "Relay dinyalakan."));
document.getElementById("offButton").addEventListener("click", () => send("/api/relay?state=off", "Relay dimatikan."));
document.getElementById("toggleButton").addEventListener("click", () => send("/api/relay?state=toggle", "Relay ditoggle."));
clapMode.addEventListener("change", () => send("/api/clap?enabled=" + (clapMode.checked ? "1" : "0"), "Clap mode diperbarui."));
threshold.addEventListener("input", scheduleSensitivityUpdate);
holdMs.addEventListener("input", scheduleSensitivityUpdate);
cooldownMs.addEventListener("input", scheduleSensitivityUpdate);

refresh();
setInterval(refresh, 350);
)JS";
