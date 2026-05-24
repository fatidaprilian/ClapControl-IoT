#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

#include "web_ui.h"

#if __has_include("secrets.h")
#include "secrets.h"
#else
const char WIFI_SSID[] = "GANTI_NAMA_WIFI";
const char WIFI_PASSWORD[] = "GANTI_PASSWORD_WIFI";
#endif

const uint8_t RELAY_PIN = 25;         // Relay drive -> ESP32 GPIO25 / D25.
const uint8_t SOUND_DIGITAL_PIN = 35; // KY-037 DO -> ESP32 GPIO35 / D35.
const uint8_t SOUND_ANALOG_PIN = 34;  // KY-037 AO -> ESP32 GPIO34 / D34.

const unsigned long SERIAL_BAUD = 115200;
const unsigned long WIFI_CONNECT_TIMEOUT_MS = 20000;

const uint8_t RELAY_ON_LEVEL = HIGH;
const uint8_t RELAY_OFF_LEVEL = LOW;
const uint8_t SOUND_ACTIVE_LEVEL = LOW;

const uint16_t SOUND_THRESHOLD_MIN = 50;
const uint16_t SOUND_THRESHOLD_MAX = 4095;
const uint16_t DEFAULT_SOUND_THRESHOLD = 1800;
const uint8_t SOUND_SAMPLE_COUNT = 12;
const unsigned long MIN_HOLD_MS_MIN = 5;
const unsigned long MIN_HOLD_MS_MAX = 250;
const unsigned long COOLDOWN_MS_MIN = 200;
const unsigned long COOLDOWN_MS_MAX = 2000;

WebServer server(80);

bool relayOn = false;
bool clapModeEnabled = true;
bool soundPeakArmed = true;
bool previousSoundActive = false;
unsigned long soundActiveSince = 0;
unsigned long lastClapAt = 0;
unsigned long clapCount = 0;
uint16_t soundThreshold = DEFAULT_SOUND_THRESHOLD;
uint16_t soundValue = 0;
uint16_t soundPeakValue = 0;
unsigned long minSoundActiveMs = 5;
unsigned long clapCooldownMs = 650;

void applyRelayOutput()
{
  digitalWrite(RELAY_PIN, relayOn ? RELAY_ON_LEVEL : RELAY_OFF_LEVEL);
}

void setRelay(bool nextRelayState)
{
  relayOn = nextRelayState;
  applyRelayOutput();

  Serial.print(F("Relay is now "));
  Serial.println(relayOn ? F("ON") : F("OFF"));
}

void toggleRelay()
{
  setRelay(!relayOn);
}

bool isSoundActive()
{
  return soundValue >= soundThreshold;
}

uint16_t readSoundPeakSample()
{
  uint16_t highestSample = 0;

  for (uint8_t index = 0; index < SOUND_SAMPLE_COUNT; index++)
  {
    uint16_t sample = static_cast<uint16_t>(analogRead(SOUND_ANALOG_PIN));
    if (sample > highestSample)
    {
      highestSample = sample;
    }
    delayMicroseconds(180);
  }

  return highestSample;
}

unsigned long clampUnsignedLong(unsigned long value, unsigned long minValue, unsigned long maxValue)
{
  if (value < minValue)
  {
    return minValue;
  }

  if (value > maxValue)
  {
    return maxValue;
  }

  return value;
}

bool parseUnsignedArg(const char *name, unsigned long &parsedValue)
{
  if (!server.hasArg(name))
  {
    return false;
  }

  String rawValue = server.arg(name);
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

  parsedValue = static_cast<unsigned long>(rawValue.toInt());
  return true;
}

String buildStatusJson()
{
  String json = "{";
  json += "\"relayOn\":";
  json += (relayOn ? "true" : "false");
  json += ",\"analog\":";
  json += soundValue;
  json += ",\"peak\":";
  json += soundPeakValue;
  json += ",\"digitalActive\":";
  json += ((digitalRead(SOUND_DIGITAL_PIN) == SOUND_ACTIVE_LEVEL) ? "true" : "false");
  json += ",\"soundActive\":";
  json += (isSoundActive() ? "true" : "false");
  json += ",\"clapMode\":";
  json += (clapModeEnabled ? "true" : "false");
  json += ",\"threshold\":";
  json += soundThreshold;
  json += ",\"minActiveMs\":";
  json += minSoundActiveMs;
  json += ",\"cooldownMs\":";
  json += clapCooldownMs;
  json += ",\"clapCount\":";
  json += clapCount;
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

void handleStyles()
{
  server.send_P(200, "text/css", DASHBOARD_CSS);
}

void handleScript()
{
  server.send_P(200, "application/javascript", DASHBOARD_JS);
}

void handleRelay()
{
  if (!server.hasArg("state"))
  {
    sendBadRequest("state is required");
    return;
  }

  String state = server.arg("state");
  state.toLowerCase();

  if (state == "on")
  {
    setRelay(true);
  }
  else if (state == "off")
  {
    setRelay(false);
  }
  else if (state == "toggle")
  {
    toggleRelay();
  }
  else
  {
    sendBadRequest("state must be on, off, or toggle");
    return;
  }

  sendStatus();
}

void handleClapMode()
{
  if (!server.hasArg("enabled"))
  {
    clapModeEnabled = !clapModeEnabled;
  }
  else
  {
    String enabledValue = server.arg("enabled");
    enabledValue.toLowerCase();
    clapModeEnabled = enabledValue == "1" || enabledValue == "true" || enabledValue == "on";
  }

  soundPeakArmed = true;
  sendStatus();
}

void handleSensitivity()
{
  unsigned long nextThreshold = soundThreshold;
  unsigned long nextHoldMs = minSoundActiveMs;
  unsigned long nextCooldownMs = clapCooldownMs;

  if (server.hasArg("threshold") && !parseUnsignedArg("threshold", nextThreshold))
  {
    sendBadRequest("threshold must be a number");
    return;
  }

  if (server.hasArg("hold") && !parseUnsignedArg("hold", nextHoldMs))
  {
    sendBadRequest("hold must be a number");
    return;
  }

  if (server.hasArg("cooldown") && !parseUnsignedArg("cooldown", nextCooldownMs))
  {
    sendBadRequest("cooldown must be a number");
    return;
  }

  soundThreshold = static_cast<uint16_t>(clampUnsignedLong(nextThreshold, SOUND_THRESHOLD_MIN, SOUND_THRESHOLD_MAX));
  minSoundActiveMs = clampUnsignedLong(nextHoldMs, MIN_HOLD_MS_MIN, MIN_HOLD_MS_MAX);
  clapCooldownMs = clampUnsignedLong(nextCooldownMs, COOLDOWN_MS_MIN, COOLDOWN_MS_MAX);
  sendStatus();
}

void handleNotFound()
{
  server.send(404, "application/json", "{\"error\":\"not found\"}");
}

void configureRoutes()
{
  server.on("/", HTTP_GET, handleRoot);
  server.on("/styles.css", HTTP_GET, handleStyles);
  server.on("/app.js", HTTP_GET, handleScript);
  server.on("/api/status", HTTP_GET, sendStatus);
  server.on("/api/relay", HTTP_GET, handleRelay);
  server.on("/api/clap", HTTP_GET, handleClapMode);
  server.on("/api/sensitivity", HTTP_GET, handleSensitivity);
  server.onNotFound(handleNotFound);
}

void connectToWiFi()
{
  Serial.print(F("Connecting to WiFi: "));
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startedAt = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startedAt < WIFI_CONNECT_TIMEOUT_MS)
  {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print(F("Dashboard: http://"));
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println(F("WiFi failed. Check SSID/password, then reset the ESP32."));
  }
}

void handleSoundDetection()
{
  soundValue = readSoundPeakSample();
  if (soundValue > soundPeakValue)
  {
    soundPeakValue = soundValue;
  }
  else if (soundPeakValue > 0)
  {
    soundPeakValue--;
  }

  bool soundActive = isSoundActive();
  unsigned long now = millis();

  if (soundActive && !previousSoundActive)
  {
    soundActiveSince = now;
  }

  previousSoundActive = soundActive;

  if (!soundActive)
  {
    soundPeakArmed = true;
    return;
  }

  if (!clapModeEnabled || !soundPeakArmed)
  {
    return;
  }

  if (now - soundActiveSince < minSoundActiveMs)
  {
    return;
  }

  if (now - lastClapAt < clapCooldownMs)
  {
    return;
  }

  toggleRelay();
  clapCount++;
  lastClapAt = now;
  soundPeakArmed = false;
  Serial.println(F("Sound trigger accepted."));
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  delay(300);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SOUND_DIGITAL_PIN, INPUT);
  pinMode(SOUND_ANALOG_PIN, INPUT);
  analogReadResolution(12);

  setRelay(false);

  Serial.println(F("ClapControl ESP32 web mode"));
  Serial.println(F("Relay drive: GPIO25 / D25"));
  Serial.println(F("KY-037 DO: GPIO35 / D35"));
  Serial.println(F("KY-037 AO: GPIO34 / D34"));

  connectToWiFi();
  configureRoutes();
  server.begin();
  Serial.println(F("Web server started."));
}

void loop()
{
  server.handleClient();
  handleSoundDetection();
}
