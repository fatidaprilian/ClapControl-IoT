#pragma once

#include <Arduino.h>

const char DASHBOARD_HTML[] PROGMEM = R"HTML(
<!doctype html>
<html lang="id">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ClapControl | Voltagent Console</title>
  <link rel="stylesheet" href="/styles.css">
</head>
<body>
  <div class="theme-toggle">
    <button id="themeToggleBtn" class="btn-ghost" title="Toggle Light/Dark Mode">
      <svg id="themeIcon" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
        <circle cx="12" cy="12" r="5"></circle>
        <line x1="12" y1="1" x2="12" y2="3"></line>
        <line x1="12" y1="21" x2="12" y2="23"></line>
        <line x1="4.22" y1="4.22" x2="5.64" y2="5.64"></line>
        <line x1="18.36" y1="18.36" x2="19.78" y2="19.78"></line>
        <line x1="1" y1="12" x2="3" y2="12"></line>
        <line x1="21" y1="12" x2="23" y2="12"></line>
        <line x1="4.22" y1="19.78" x2="5.64" y2="18.36"></line>
        <line x1="18.36" y1="5.64" x2="19.78" y2="4.22"></line>
      </svg>
    </button>
  </div>

  <main class="console">
    <header class="hero-band">
      <p class="eyebrow-mono">ESP32 LOCAL SIGNAL DESK</p>
      <h1>ClapControl<span class="cursor">_</span></h1>
      <p class="subtitle">Kalibrasi sensor suara, atur penjadwalan NTP, dan kendalikan relay IoT Anda secara presisi.</p>
      <div id="systemMessage" class="toast" role="status" aria-live="polite">Menghubungkan ke perangkat...</div>
    </header>

    <div class="grid-layout">
      <section class="card-feature">
        <h2 class="card-title">Live Telemetry</h2>
        <div class="data-grid">
          <div class="data-cell">
            <span class="label">Time (UTC+7)</span>
            <strong id="currentTime" class="mono">--:--</strong>
          </div>
          <div class="data-cell">
            <span class="label">Relay State</span>
            <strong id="relayState" class="mono status-off">OFF</strong>
          </div>
          <div class="data-cell">
            <span class="label">Analog In</span>
            <strong id="analogValue" class="mono">0</strong>
          </div>
          <div class="data-cell">
            <span class="label">Peak Hold</span>
            <strong id="peakValue" class="mono">0</strong>
          </div>
          <div class="data-cell">
            <span class="label">Sound Status</span>
            <strong id="soundState" class="mono">IDLE</strong>
          </div>
          <div class="data-cell">
            <span class="label">Total Claps</span>
            <strong id="clapCount" class="mono">0</strong>
          </div>
        </div>
      </section>

      <section class="card-feature">
        <h2 class="card-title">Manual Override</h2>
        <div class="action-row">
          <button id="onButton" class="btn-primary">Turn ON</button>
          <button id="offButton" class="btn-outline">Turn OFF</button>
          <button id="toggleButton" class="btn-ghost">Toggle</button>
        </div>
        <div class="setting-row mt-4">
          <label for="clapMode" class="label-strong">Enable Clap Detection</label>
          <label class="switch">
            <input id="clapMode" type="checkbox" checked>
            <span class="slider"></span>
          </label>
        </div>
      </section>

      <section class="card-feature">
        <h2 class="card-title">Sensor Calibration</h2>
        <div class="slider-group">
          <div class="slider-header">
            <label for="threshold">Analog Threshold</label>
            <span id="thresholdValue" class="mono-chip">1800</span>
          </div>
          <input id="threshold" type="range" min="50" max="4095" step="25" value="1800">
        </div>
        
        <div class="slider-group">
          <div class="slider-header">
            <label for="holdMs">Active Hold (ms)</label>
            <span id="holdValue" class="mono-chip">5</span>
          </div>
          <input id="holdMs" type="range" min="5" max="250" step="5" value="5">
        </div>

        <div class="slider-group">
          <div class="slider-header">
            <label for="cooldownMs">Cooldown (ms)</label>
            <span id="cooldownValue" class="mono-chip">650</span>
          </div>
          <input id="cooldownMs" type="range" min="200" max="2000" step="50" value="650">
        </div>
      </section>

      <section class="card-feature">
        <h2 class="card-title">Time Schedule (NTP)</h2>
        <div class="setting-row mb-4">
          <label for="scheduleMode" class="label-strong">Enable Daily Schedule</label>
          <label class="switch">
            <input id="scheduleMode" type="checkbox">
            <span class="slider"></span>
          </label>
        </div>
        <div class="time-inputs">
          <div class="input-group">
            <label for="timeOn">Turn ON At</label>
            <input type="time" id="timeOn" value="18:00" class="input-field">
          </div>
          <div class="input-group">
            <label for="timeOff">Turn OFF At</label>
            <input type="time" id="timeOff" value="06:00" class="input-field">
          </div>
        </div>
        <button id="saveScheduleBtn" class="btn-outline mt-4 w-full">Save Schedule</button>
      </section>
      
      <section class="card-feature">
        <h2 class="card-title">System Configuration</h2>
        <div class="data-grid mb-4">
          <div class="data-cell">
            <span class="label">WiFi Signal</span>
            <strong id="wifiRssi" class="mono">-- dBm</strong>
          </div>
          <div class="data-cell">
            <span class="label">Uptime</span>
            <strong id="uptime" class="mono">0s</strong>
          </div>
        </div>
        <button id="resetWifiBtn" class="btn-danger w-full">Reset WiFi Credentials & Restart</button>
      </section>
    </div>
  </main>
  <script src="/app.js"></script>
</body>
</html>
)HTML";

const char DASHBOARD_CSS[] PROGMEM = R"CSS(
:root {
  /* Dark Mode Defaults */
  --canvas: #101010;
  --canvas-soft: #1a1a1a;
  --primary: #00d992;
  --primary-soft: #2fd6a1;
  --ink: #f2f2f2;
  --body: #bdbdbd;
  --hairline: #3d3a39;
  --danger: #ef4444;
  --font-sans: Inter, system-ui, -apple-system, sans-serif;
  --font-mono: SFMono-Regular, Menlo, Monaco, Consolas, monospace;
}

[data-theme="light"] {
  --canvas: #ffffff;
  --canvas-soft: #f9fafb;
  --primary: #00b87a;
  --primary-soft: #059669;
  --ink: #111827;
  --body: #4b5563;
  --hairline: #e5e7eb;
  --danger: #dc2626;
}

* { box-sizing: border-box; }

body {
  margin: 0;
  font-family: var(--font-sans);
  background-color: var(--canvas);
  color: var(--ink);
  line-height: 1.6;
  transition: background-color 0.3s, color 0.3s;
}

.theme-toggle {
  position: absolute;
  top: 16px;
  right: 16px;
}

.console {
  max-width: 1200px;
  margin: 0 auto;
  padding: 48px 24px;
}

.eyebrow-mono {
  font-family: var(--font-mono);
  font-size: 14px;
  font-weight: 600;
  letter-spacing: 2.52px;
  color: var(--primary);
  margin-bottom: 8px;
}

.hero-band {
  margin-bottom: 48px;
}

h1 {
  font-size: clamp(36px, 6vw, 60px);
  font-weight: 400;
  letter-spacing: -0.65px;
  margin: 0 0 16px 0;
  line-height: 1.1;
}

.cursor {
  color: var(--primary);
  animation: blink 1s step-end infinite;
}

@keyframes blink {
  50% { opacity: 0; }
}

.subtitle {
  font-size: 18px;
  color: var(--body);
  max-width: 600px;
  margin: 0 0 24px 0;
}

.toast {
  display: inline-block;
  padding: 8px 16px;
  border-radius: 9999px;
  border: 1px solid var(--hairline);
  font-size: 14px;
  color: var(--body);
}

.grid-layout {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(320px, 1fr));
  gap: 24px;
}

.card-feature {
  background-color: var(--canvas);
  border: 1px solid var(--hairline);
  border-radius: 8px;
  padding: 24px;
  transition: border-color 0.2s;
}

.card-title {
  font-size: 20px;
  font-weight: 600;
  margin: 0 0 20px 0;
  color: var(--ink);
}

.mono {
  font-family: var(--font-mono);
}

.data-grid {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 16px;
}

.data-cell {
  background-color: var(--canvas-soft);
  padding: 12px 16px;
  border-radius: 6px;
  border: 1px solid var(--hairline);
}

.label {
  display: block;
  font-size: 12px;
  color: var(--body);
  text-transform: uppercase;
  letter-spacing: 0.5px;
  margin-bottom: 4px;
}

.data-cell strong {
  font-size: 20px;
  display: block;
}

.status-on { color: var(--primary); }
.status-off { color: var(--body); }

.action-row {
  display: flex;
  gap: 12px;
  flex-wrap: wrap;
}

button {
  font-family: var(--font-sans);
  font-size: 14px;
  font-weight: 600;
  padding: 10px 16px;
  border-radius: 6px;
  cursor: pointer;
  transition: all 0.2s;
  outline: none;
}

button:focus-visible {
  outline: 2px solid var(--primary);
  outline-offset: 2px;
}

.btn-primary {
  background-color: var(--primary);
  color: #101010;
  border: 1px solid var(--primary);
}
.btn-primary:hover { background-color: var(--primary-soft); }

.btn-outline {
  background-color: var(--canvas);
  color: var(--ink);
  border: 1px solid var(--hairline);
}
.btn-outline:hover { border-color: var(--ink); }

.btn-ghost {
  background-color: transparent;
  color: var(--primary);
  border: 1px solid transparent;
}
.btn-ghost:hover { color: var(--primary-soft); }

.btn-danger {
  background-color: var(--canvas);
  color: var(--danger);
  border: 1px solid var(--danger);
}
.btn-danger:hover {
  background-color: var(--danger);
  color: #fff;
}

.mt-4 { margin-top: 16px; }
.mb-4 { margin-bottom: 16px; }
.w-full { width: 100%; }

.setting-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 12px 16px;
  background-color: var(--canvas-soft);
  border: 1px solid var(--hairline);
  border-radius: 6px;
}

.label-strong {
  font-size: 14px;
  font-weight: 600;
}

.switch {
  position: relative;
  display: inline-block;
  width: 44px;
  height: 24px;
}
.switch input { opacity: 0; width: 0; height: 0; }
.slider {
  position: absolute;
  cursor: pointer;
  top: 0; left: 0; right: 0; bottom: 0;
  background-color: var(--hairline);
  transition: .2s;
  border-radius: 24px;
}
.slider:before {
  position: absolute;
  content: "";
  height: 18px; width: 18px;
  left: 3px; bottom: 3px;
  background-color: var(--ink);
  transition: .2s;
  border-radius: 50%;
}
input:checked + .slider { background-color: var(--primary); }
input:checked + .slider:before {
  transform: translateX(20px);
  background-color: #101010;
}
input:focus-visible + .slider {
  outline: 2px solid var(--primary);
  outline-offset: 2px;
}

.slider-group {
  margin-bottom: 20px;
}
.slider-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 8px;
  font-size: 14px;
}
.mono-chip {
  font-family: var(--font-mono);
  background-color: var(--canvas-soft);
  padding: 2px 8px;
  border-radius: 4px;
  font-size: 13px;
  border: 1px solid var(--hairline);
}

input[type=range] {
  -webkit-appearance: none;
  width: 100%;
  background: transparent;
}
input[type=range]::-webkit-slider-thumb {
  -webkit-appearance: none;
  height: 16px; width: 16px;
  border-radius: 50%;
  background: var(--primary);
  cursor: pointer;
  margin-top: -6px;
}
input[type=range]::-webkit-slider-runnable-track {
  width: 100%;
  height: 4px;
  cursor: pointer;
  background: var(--hairline);
  border-radius: 2px;
}
input[type=range]:focus-visible::-webkit-slider-thumb {
  outline: 2px solid var(--primary);
  outline-offset: 2px;
}

.time-inputs {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 12px;
}
.input-group label {
  display: block;
  font-size: 12px;
  margin-bottom: 4px;
  color: var(--body);
}
.input-field {
  width: 100%;
  padding: 10px;
  background-color: var(--canvas-soft);
  border: 1px solid var(--hairline);
  color: var(--ink);
  font-family: var(--font-mono);
  border-radius: 6px;
  font-size: 16px;
  outline: none;
}
.input-field:focus-visible {
  border-color: var(--primary);
}

@media (max-width: 768px) {
  .hero-band { margin-bottom: 32px; }
  .grid-layout { grid-template-columns: 1fr; }
}
@media (prefers-reduced-motion: reduce) {
  * { transition: none !important; animation: none !important; }
}
)CSS";

const char DASHBOARD_JS[] PROGMEM = R"JS(
// Theme Toggle Logic
const themeBtn = document.getElementById('themeToggleBtn');
const htmlEl = document.documentElement;

// Initialize theme from localStorage or default to dark
const savedTheme = localStorage.getItem('theme') || 'dark';
htmlEl.setAttribute('data-theme', savedTheme);

themeBtn.addEventListener('click', () => {
  const currentTheme = htmlEl.getAttribute('data-theme');
  const newTheme = currentTheme === 'dark' ? 'light' : 'dark';
  htmlEl.setAttribute('data-theme', newTheme);
  localStorage.setItem('theme', newTheme);
});

// UI Elements
const relayState = document.getElementById("relayState");
const soundState = document.getElementById("soundState");
const analogValue = document.getElementById("analogValue");
const peakValue = document.getElementById("peakValue");
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
const message = document.getElementById("systemMessage");
const currentTime = document.getElementById("currentTime");
const scheduleMode = document.getElementById("scheduleMode");
const timeOn = document.getElementById("timeOn");
const timeOff = document.getElementById("timeOff");

let pendingTimer = null;

async function requestJson(url, options = { cache: "no-store" }) {
  const response = await fetch(url, options);
  if (!response.ok) throw new Error("HTTP " + response.status);
  return response.json();
}

function render(data) {
  if (data.relayOn) {
    relayState.textContent = "ON";
    relayState.className = "mono status-on";
  } else {
    relayState.textContent = "OFF";
    relayState.className = "mono status-off";
  }
  
  soundState.textContent = data.soundActive ? "ACTIVE" : "IDLE";
  analogValue.textContent = data.analog;
  peakValue.textContent = data.peak;
  clapCount.textContent = data.clapCount;
  uptime.textContent = Math.floor(data.uptimeMs / 1000) + "s";
  wifiRssi.textContent = data.wifiRssi + " dBm";
  currentTime.textContent = data.currentTime || "--:--";

  clapMode.checked = data.clapMode;

  if (document.activeElement !== threshold) {
    threshold.value = data.threshold;
    thresholdValue.textContent = data.threshold;
  }
  if (document.activeElement !== holdMs) {
    holdMs.value = data.minActiveMs;
    holdValue.textContent = data.minActiveMs;
  }
  if (document.activeElement !== cooldownMs) {
    cooldownMs.value = data.cooldownMs;
    cooldownValue.textContent = data.cooldownMs;
  }
  if (document.activeElement !== scheduleMode && document.activeElement !== timeOn && document.activeElement !== timeOff) {
    scheduleMode.checked = data.scheduleEnabled;
    const pad = (n) => n.toString().padStart(2, '0');
    timeOn.value = `${pad(data.onHour)}:${pad(data.onMinute)}`;
    timeOff.value = `${pad(data.offHour)}:${pad(data.offMinute)}`;
  }
}

async function send(url, okMessage, options = {}) {
  try {
    const data = await requestJson(url, options);
    if(data.status !== "restarting") {
      render(data);
    }
    message.textContent = okMessage;
  } catch (error) {
    message.textContent = "Failed: " + error.message;
  }
}

async function refresh() {
  try {
    render(await requestJson("/api/status"));
    message.textContent = "Connected to ESP32 / Telemetry live";
  } catch (error) {
    message.textContent = "Offline or reconnecting...";
  }
}

function scheduleSensitivityUpdate() {
  clearTimeout(pendingTimer);
  thresholdValue.textContent = threshold.value;
  holdValue.textContent = holdMs.value;
  cooldownValue.textContent = cooldownMs.value;
  pendingTimer = setTimeout(() => {
    send(`/api/sensitivity?threshold=${threshold.value}&hold=${holdMs.value}&cooldown=${cooldownMs.value}`, "Calibration updated");
  }, 300);
}

document.getElementById("onButton").addEventListener("click", () => send("/api/relay?state=on", "Relay ON"));
document.getElementById("offButton").addEventListener("click", () => send("/api/relay?state=off", "Relay OFF"));
document.getElementById("toggleButton").addEventListener("click", () => send("/api/relay?state=toggle", "Relay Toggled"));

clapMode.addEventListener("change", () => send("/api/clap?enabled=" + (clapMode.checked ? "1" : "0"), "Clap mode updated"));
threshold.addEventListener("input", scheduleSensitivityUpdate);
holdMs.addEventListener("input", scheduleSensitivityUpdate);
cooldownMs.addEventListener("input", scheduleSensitivityUpdate);

document.getElementById("saveScheduleBtn").addEventListener("click", () => {
  const onParts = timeOn.value.split(":");
  const offParts = timeOff.value.split(":");
  if (onParts.length !== 2 || offParts.length !== 2) return;
  const q = `?enabled=${scheduleMode.checked ? "1" : "0"}&onHour=${onParts[0]}&onMinute=${onParts[1]}&offHour=${offParts[0]}&offMinute=${offParts[1]}`;
  send("/api/schedule" + q, "Schedule saved");
});

document.getElementById("resetWifiBtn").addEventListener("click", () => {
  if(confirm("Are you sure you want to reset WiFi credentials? The device will restart and open the 'ClapControl-Setup' Access Point.")) {
    send("/api/reset-wifi", "Resetting WiFi and restarting device...", { method: 'POST' });
  }
});

refresh();
setInterval(refresh, 1000);
)JS";
