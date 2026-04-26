#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

// WiFi credentials. Replace these values with your own network name/password.
const char *WIFI_SSID = "GANTI_NAMA_WIFI";
const char *WIFI_PASSWORD = "GANTI_PASSWORD_WIFI";

// Hardware pin map for ESP32 DevKit V1.
const uint8_t SOUND_ANALOG_PIN = 34;  // KY-037 AO -> GPIO34, input only ADC pin.
const uint8_t SOUND_DIGITAL_PIN = 27; // KY-037 DO -> GPIO27.
const uint8_t RELAY_PIN = 26;         // Relay IN -> GPIO26.

// Relay module is active LOW: LOW turns lamp on, HIGH turns lamp off.
const uint8_t RELAY_ON_LEVEL = LOW;
const uint8_t RELAY_OFF_LEVEL = HIGH;

// Clap detection guardrails.
const uint16_t DEFAULT_SOUND_THRESHOLD = 2200;
const uint16_t MIN_SOUND_THRESHOLD = 0;
const uint16_t MAX_SOUND_THRESHOLD = 4095;
const unsigned long CLAP_COOLDOWN_MS = 650;
const unsigned long WIFI_RECONNECT_INTERVAL_MS = 10000;

WebServer server(80);

bool lampOn = false;
bool clapModeEnabled = true;
bool soundPeakArmed = true;
uint16_t soundThreshold = DEFAULT_SOUND_THRESHOLD;
unsigned long lastClapAt = 0;
unsigned long lastWifiReconnectAttempt = 0;

const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="id">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ClapControl IoT</title>
  <style>
    :root {
      color-scheme: dark;
      --bg: #071018;
      --panel: rgba(16, 31, 42, 0.88);
      --panel-strong: #132c3c;
      --line: rgba(159, 220, 255, 0.18);
      --text: #eef8ff;
      --muted: #9eb4c3;
      --accent: #38d39f;
      --accent-strong: #21b985;
      --danger: #ff6464;
      --warn: #ffd166;
      --off: #607483;
      --shadow: 0 22px 70px rgba(0, 0, 0, 0.35);
      font-family: Inter, ui-sans-serif, system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
    }

    * {
      box-sizing: border-box;
    }

    body {
      min-height: 100vh;
      margin: 0;
      color: var(--text);
      background:
        radial-gradient(circle at 12% 10%, rgba(56, 211, 159, 0.18), transparent 28rem),
        radial-gradient(circle at 86% 12%, rgba(96, 190, 255, 0.13), transparent 24rem),
        linear-gradient(145deg, #050b10 0%, var(--bg) 46%, #0b1720 100%);
    }

    main {
      width: min(1120px, calc(100% - 28px));
      margin: 0 auto;
      padding: 28px 0;
    }

    .shell {
      display: grid;
      grid-template-columns: minmax(0, 1.08fr) minmax(320px, 0.92fr);
      gap: 18px;
      align-items: stretch;
    }

    .card {
      border: 1px solid var(--line);
      border-radius: 8px;
      background: var(--panel);
      box-shadow: var(--shadow);
      backdrop-filter: blur(14px);
    }

    .hero {
      min-height: 420px;
      padding: 28px;
      display: grid;
      align-content: space-between;
      overflow: hidden;
      position: relative;
    }

    .hero::after {
      content: "";
      position: absolute;
      inset: auto -20% -40% 20%;
      height: 220px;
      background: linear-gradient(90deg, transparent, rgba(56, 211, 159, 0.18), transparent);
      transform: rotate(-8deg);
      pointer-events: none;
    }

    .eyebrow {
      margin: 0 0 12px;
      color: var(--accent);
      font-size: 0.78rem;
      font-weight: 800;
      letter-spacing: 0.08em;
      text-transform: uppercase;
    }

    h1 {
      margin: 0;
      max-width: 760px;
      font-size: clamp(2.35rem, 7vw, 5.6rem);
      line-height: 0.9;
      letter-spacing: 0;
    }

    .subcopy {
      max-width: 620px;
      margin: 18px 0 0;
      color: var(--muted);
      font-size: 1rem;
      line-height: 1.65;
    }

    .status-row {
      display: flex;
      flex-wrap: wrap;
      gap: 12px;
      margin-top: 34px;
      position: relative;
      z-index: 1;
    }

    .pill {
      min-height: 44px;
      padding: 10px 14px;
      border: 1px solid var(--line);
      border-radius: 999px;
      display: inline-flex;
      align-items: center;
      gap: 10px;
      color: var(--muted);
      background: rgba(255, 255, 255, 0.04);
      font-weight: 700;
    }

    .dot {
      width: 12px;
      height: 12px;
      border-radius: 50%;
      background: var(--off);
      box-shadow: 0 0 0 5px rgba(96, 116, 131, 0.15);
    }

    .dot.on {
      background: var(--accent);
      box-shadow: 0 0 0 5px rgba(56, 211, 159, 0.16), 0 0 28px rgba(56, 211, 159, 0.65);
    }

    .panel {
      padding: 20px;
      display: grid;
      gap: 16px;
    }

    .readout {
      padding: 18px;
      border-radius: 8px;
      background: var(--panel-strong);
      border: 1px solid var(--line);
    }

    .readout span {
      display: block;
      color: var(--muted);
      font-size: 0.8rem;
      font-weight: 800;
      text-transform: uppercase;
      letter-spacing: 0.08em;
    }

    .readout strong {
      display: block;
      margin-top: 6px;
      font-size: clamp(2rem, 8vw, 4rem);
      line-height: 1;
    }

    .meter {
      height: 14px;
      margin-top: 14px;
      overflow: hidden;
      border-radius: 999px;
      background: rgba(255, 255, 255, 0.08);
      border: 1px solid rgba(255, 255, 255, 0.08);
    }

    .meter > div {
      width: 0%;
      height: 100%;
      border-radius: inherit;
      background: linear-gradient(90deg, #38d39f, #ffd166, #ff6464);
      transition: width 180ms ease;
    }

    .button-grid {
      display: grid;
      grid-template-columns: repeat(3, minmax(0, 1fr));
      gap: 10px;
    }

    button {
      min-height: 56px;
      border: 0;
      border-radius: 8px;
      color: #03120d;
      background: var(--accent);
      font: inherit;
      font-weight: 900;
      cursor: pointer;
      transition: transform 150ms ease, filter 150ms ease, opacity 150ms ease;
    }

    button:hover {
      transform: translateY(-1px);
      filter: brightness(1.05);
    }

    button:focus-visible,
    input:focus-visible {
      outline: 3px solid rgba(255, 209, 102, 0.95);
      outline-offset: 3px;
    }

    button:active {
      transform: translateY(1px);
    }

    button.secondary {
      color: var(--text);
      background: rgba(255, 255, 255, 0.09);
      border: 1px solid var(--line);
    }

    button.danger {
      color: #fff;
      background: var(--danger);
    }

    button.mode-on {
      color: #03120d;
      background: var(--warn);
    }

    .slider-card {
      padding: 18px;
      border-radius: 8px;
      border: 1px solid var(--line);
      background: rgba(255, 255, 255, 0.045);
    }

    .label-row {
      display: flex;
      align-items: baseline;
      justify-content: space-between;
      gap: 12px;
      margin-bottom: 12px;
      color: var(--muted);
      font-weight: 800;
    }

    .label-row output {
      color: var(--text);
      font-size: 1.4rem;
    }

    input[type="range"] {
      width: 100%;
      accent-color: var(--accent);
      cursor: pointer;
    }

    .meta-grid {
      display: grid;
      grid-template-columns: repeat(2, minmax(0, 1fr));
      gap: 10px;
    }

    .meta {
      min-height: 72px;
      padding: 12px;
      border-radius: 8px;
      background: rgba(255, 255, 255, 0.055);
      border: 1px solid rgba(255, 255, 255, 0.08);
    }

    .meta span {
      color: var(--muted);
      font-size: 0.75rem;
      font-weight: 800;
      text-transform: uppercase;
    }

    .meta strong {
      display: block;
      margin-top: 6px;
      font-size: 1.25rem;
    }

    .toast {
      min-height: 22px;
      color: var(--muted);
      font-size: 0.9rem;
    }

    @media (max-width: 780px) {
      main {
        width: min(100% - 18px, 560px);
        padding: 9px 0;
      }

      .shell {
        grid-template-columns: 1fr;
      }

      .hero {
        min-height: 360px;
        padding: 20px;
      }

      .panel {
        padding: 14px;
      }

      .button-grid {
        grid-template-columns: 1fr;
      }
    }

    @media (prefers-reduced-motion: reduce) {
      *,
      *::before,
      *::after {
        transition: none !important;
        scroll-behavior: auto !important;
      }
    }
  </style>
</head>
<body>
  <main>
    <section class="shell" aria-label="ClapControl IoT dashboard">
      <article class="card hero">
        <div>
          <p class="eyebrow">ESP32 DevKit V1 + KY-037</p>
          <h1>ClapControl IoT</h1>
          <p class="subcopy">Monitor suara realtime, atur threshold, dan kendalikan relay lampu AC lewat jaringan lokal.</p>
        </div>
        <div class="status-row">
          <div class="pill" aria-live="polite">
            <span id="lampDot" class="dot" aria-hidden="true"></span>
            Lampu <span id="lampText">OFF</span>
          </div>
          <div class="pill">
            Clap mode <span id="clapText">ON</span>
          </div>
        </div>
      </article>

      <aside class="card panel">
        <div class="readout">
          <span>Nilai analog suara</span>
          <strong id="analogValue">0</strong>
          <div class="meter" aria-hidden="true"><div id="analogBar"></div></div>
        </div>

        <div class="button-grid" aria-label="Kontrol lampu">
          <button onclick="sendAction('/api/on')">ON</button>
          <button class="danger" onclick="sendAction('/api/off')">OFF</button>
          <button class="secondary" onclick="sendAction('/api/toggle')">TOGGLE</button>
        </div>

        <button id="clapButton" class="mode-on" onclick="sendAction('/api/clap-mode')">CLAP MODE</button>

        <div class="slider-card">
          <label class="label-row" for="thresholdSlider">
            Threshold
            <output id="thresholdValue">2200</output>
          </label>
          <input id="thresholdSlider" type="range" min="0" max="4095" step="10" value="2200">
        </div>

        <div class="meta-grid">
          <div class="meta">
            <span>Digital DO</span>
            <strong id="digitalValue">LOW</strong>
          </div>
          <div class="meta">
            <span>WiFi RSSI</span>
            <strong id="wifiRssi">0 dBm</strong>
          </div>
        </div>
        <div id="message" class="toast" role="status" aria-live="polite"></div>
      </aside>
    </section>
  </main>

  <script>
    const state = {
      thresholdTimer: null,
      lastThresholdSent: null
    };

    const lampDot = document.getElementById("lampDot");
    const lampText = document.getElementById("lampText");
    const clapText = document.getElementById("clapText");
    const clapButton = document.getElementById("clapButton");
    const analogValue = document.getElementById("analogValue");
    const analogBar = document.getElementById("analogBar");
    const digitalValue = document.getElementById("digitalValue");
    const wifiRssi = document.getElementById("wifiRssi");
    const thresholdSlider = document.getElementById("thresholdSlider");
    const thresholdValue = document.getElementById("thresholdValue");
    const message = document.getElementById("message");

    async function requestJson(url) {
      const response = await fetch(url, { cache: "no-store" });
      if (!response.ok) {
        throw new Error("HTTP " + response.status);
      }
      return response.json();
    }

    function showMessage(text) {
      message.textContent = text;
    }

    function render(data) {
      lampText.textContent = data.lampOn ? "ON" : "OFF";
      lampDot.classList.toggle("on", data.lampOn);
      clapText.textContent = data.clapMode ? "ON" : "OFF";
      clapButton.classList.toggle("mode-on", data.clapMode);

      analogValue.textContent = data.analog;
      analogBar.style.width = Math.min(100, Math.round((data.analog / 4095) * 100)) + "%";
      digitalValue.textContent = data.digital ? "HIGH" : "LOW";
      wifiRssi.textContent = data.wifiRssi + " dBm";

      if (document.activeElement !== thresholdSlider) {
        thresholdSlider.value = data.threshold;
        thresholdValue.textContent = data.threshold;
      }
    }

    async function refreshStatus() {
      try {
        render(await requestJson("/api/status"));
        showMessage("Terhubung. Data diperbarui otomatis setiap 500 ms.");
      } catch (error) {
        showMessage("Gagal mengambil data dari ESP32.");
      }
    }

    async function sendAction(url) {
      try {
        render(await requestJson(url));
        showMessage("Perintah berhasil dikirim.");
      } catch (error) {
        showMessage("Perintah gagal dikirim.");
      }
    }

    thresholdSlider.addEventListener("input", () => {
      const value = thresholdSlider.value;
      thresholdValue.textContent = value;
      clearTimeout(state.thresholdTimer);
      state.thresholdTimer = setTimeout(async () => {
        if (state.lastThresholdSent === value) {
          return;
        }
        state.lastThresholdSent = value;
        await sendAction("/api/threshold?value=" + encodeURIComponent(value));
      }, 180);
    });

    refreshStatus();
    setInterval(refreshStatus, 500);
  </script>
</body>
</html>
)rawliteral";

void applyLampOutput()
{
  digitalWrite(RELAY_PIN, lampOn ? RELAY_ON_LEVEL : RELAY_OFF_LEVEL);
}

void setLamp(bool nextLampState)
{
  lampOn = nextLampState;
  applyLampOutput();
}

void toggleLamp()
{
  setLamp(!lampOn);
}

bool parseThresholdValue(uint16_t &parsedValue)
{
  if (!server.hasArg("value"))
  {
    return false;
  }

  String rawValue = server.arg("value");
  rawValue.trim();
  if (rawValue.length() == 0)
  {
    return false;
  }

  for (size_t index = 0; index < rawValue.length(); index++)
  {
    if (!isDigit(rawValue[index]))
    {
      return false;
    }
  }

  long numericValue = rawValue.toInt();
  if (numericValue < MIN_SOUND_THRESHOLD || numericValue > MAX_SOUND_THRESHOLD)
  {
    return false;
  }

  parsedValue = static_cast<uint16_t>(numericValue);
  return true;
}

String buildStatusJson()
{
  int analogValue = analogRead(SOUND_ANALOG_PIN);
  int digitalValue = digitalRead(SOUND_DIGITAL_PIN);

  String json = "{";
  json += "\"lampOn\":";
  json += lampOn ? "true" : "false";
  json += ",\"clapMode\":";
  json += clapModeEnabled ? "true" : "false";
  json += ",\"analog\":";
  json += analogValue;
  json += ",\"digital\":";
  json += digitalValue;
  json += ",\"threshold\":";
  json += soundThreshold;
  json += ",\"cooldownMs\":";
  json += CLAP_COOLDOWN_MS;
  json += ",\"wifiRssi\":";
  json += WiFi.RSSI();
  json += ",\"uptimeMs\":";
  json += millis();
  json += "}";
  return json;
}

void sendStatus()
{
  server.send(200, "application/json", buildStatusJson());
}

void sendBadRequest(const char *message)
{
  String json = "{\"error\":\"";
  json += message;
  json += "\"}";
  server.send(400, "application/json", json);
}

void handleRoot()
{
  server.send_P(200, "text/html", DASHBOARD_HTML);
}

void handleLampOn()
{
  setLamp(true);
  sendStatus();
}

void handleLampOff()
{
  setLamp(false);
  sendStatus();
}

void handleLampToggle()
{
  toggleLamp();
  sendStatus();
}

void handleClapMode()
{
  if (server.hasArg("enabled"))
  {
    String enabledValue = server.arg("enabled");
    enabledValue.toLowerCase();
    clapModeEnabled = enabledValue == "1" || enabledValue == "true" || enabledValue == "on";
  }
  else
  {
    clapModeEnabled = !clapModeEnabled;
  }

  soundPeakArmed = true;
  sendStatus();
}

void handleThreshold()
{
  uint16_t nextThreshold = soundThreshold;
  if (!parseThresholdValue(nextThreshold))
  {
    sendBadRequest("threshold value must be a number from 0 to 4095");
    return;
  }

  soundThreshold = nextThreshold;
  sendStatus();
}

void handleNotFound()
{
  server.send(404, "application/json", "{\"error\":\"not found\"}");
}

void configureRoutes()
{
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/status", HTTP_GET, sendStatus);
  server.on("/api/on", HTTP_GET, handleLampOn);
  server.on("/api/off", HTTP_GET, handleLampOff);
  server.on("/api/toggle", HTTP_GET, handleLampToggle);
  server.on("/api/clap-mode", HTTP_GET, handleClapMode);
  server.on("/api/threshold", HTTP_GET, handleThreshold);
  server.onNotFound(handleNotFound);
}

void connectToWiFi()
{
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 20000)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("WiFi connected. Dashboard: http://");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("WiFi connection failed. Check SSID/password and reset the board.");
  }
}

void keepWiFiConnected()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    return;
  }

  unsigned long now = millis();
  if (now - lastWifiReconnectAttempt < WIFI_RECONNECT_INTERVAL_MS)
  {
    return;
  }

  lastWifiReconnectAttempt = now;
  Serial.println("WiFi disconnected. Reconnecting...");
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void handleClapDetection()
{
  int analogValue = analogRead(SOUND_ANALOG_PIN);
  bool soundPeak = analogValue >= soundThreshold;
  unsigned long now = millis();

  // Clap trigger uses the web-configured analog threshold. The DO pin is still
  // monitored in /api/status, but not used for relay decisions because KY-037
  // modules can differ in digital output polarity.
  if (!soundPeak)
  {
    soundPeakArmed = true;
    return;
  }

  if (!clapModeEnabled || !soundPeakArmed)
  {
    return;
  }

  if (now - lastClapAt < CLAP_COOLDOWN_MS)
  {
    return;
  }

  toggleLamp();
  lastClapAt = now;
  soundPeakArmed = false;
  Serial.print("Clap detected. Lamp is now ");
  Serial.println(lampOn ? "ON" : "OFF");
}

void setup()
{
  Serial.begin(115200);
  delay(300);

  pinMode(SOUND_ANALOG_PIN, INPUT);
  pinMode(SOUND_DIGITAL_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  analogReadResolution(12);
  setLamp(false);

  connectToWiFi();
  configureRoutes();
  server.begin();
  Serial.println("Web server started.");
}

void loop()
{
  keepWiFiConnected();
  server.handleClient();
  handleClapDetection();
  delay(5);
}
