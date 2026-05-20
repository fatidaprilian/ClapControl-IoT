#define BLYNK_TEMPLATE_ID "GANTI_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "ClapControl IoT"
#define BLYNK_AUTH_TOKEN "GANTI_AUTH_TOKEN"
#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const char WIFI_SSID[] = "GANTI_NAMA_WIFI";
const char WIFI_PASSWORD[] = "GANTI_PASSWORD_WIFI";

const uint8_t RELAY_PIN = 2;        // Relay IN -> ESP32 GPIO2 / D2.
const uint8_t SOUND_DIGITAL_PIN = 22; // KY-037 DO -> ESP32 GPIO22 / D22.

const unsigned long SERIAL_BAUD = 115200;
const unsigned long CLAP_COOLDOWN_MS = 650;
const unsigned long TELEMETRY_INTERVAL_MS = 1000;

const uint8_t RELAY_ON_LEVEL = HIGH;
const uint8_t RELAY_OFF_LEVEL = LOW;
const uint8_t SOUND_ACTIVE_LEVEL = LOW;

const uint8_t VPIN_LAMP_SWITCH = V0;
const uint8_t VPIN_CLAP_MODE = V1;
const uint8_t VPIN_TOGGLE_BUTTON = V2;
const uint8_t VPIN_SOUND_TRIGGER = V3;
const uint8_t VPIN_UPTIME_SECONDS = V4;
const uint8_t VPIN_WIFI_RSSI = V5;

BlynkTimer timer;

bool lampOn = false;
bool clapModeEnabled = true;
bool soundPeakArmed = true;
unsigned long lastClapAt = 0;

void applyRelayOutput()
{
  digitalWrite(RELAY_PIN, lampOn ? RELAY_ON_LEVEL : RELAY_OFF_LEVEL);
}

void publishLampState()
{
  Blynk.virtualWrite(VPIN_LAMP_SWITCH, lampOn ? 1 : 0);
}

void setLamp(bool nextLampState)
{
  lampOn = nextLampState;
  applyRelayOutput();
  publishLampState();

  Serial.print(F("Bulb relay is now "));
  Serial.println(lampOn ? F("ON") : F("OFF"));
}

void toggleLamp()
{
  setLamp(!lampOn);
}

bool isSoundActive()
{
  return digitalRead(SOUND_DIGITAL_PIN) == SOUND_ACTIVE_LEVEL;
}

void publishTelemetry()
{
  Blynk.virtualWrite(VPIN_SOUND_TRIGGER, isSoundActive() ? 1 : 0);
  Blynk.virtualWrite(VPIN_UPTIME_SECONDS, millis() / 1000UL);
  Blynk.virtualWrite(VPIN_WIFI_RSSI, WiFi.RSSI());
}

void handleClapDetection()
{
  bool soundActive = isSoundActive();
  unsigned long now = millis();

  if (!soundActive)
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
  Serial.println(F("Clap detected."));
}

void publishControlState()
{
  publishLampState();
  Blynk.virtualWrite(VPIN_CLAP_MODE, clapModeEnabled ? 1 : 0);
  publishTelemetry();
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(VPIN_LAMP_SWITCH, VPIN_CLAP_MODE);
  publishControlState();
}

BLYNK_WRITE(VPIN_LAMP_SWITCH)
{
  setLamp(param.asInt() == 1);
}

BLYNK_WRITE(VPIN_CLAP_MODE)
{
  clapModeEnabled = param.asInt() == 1;
  soundPeakArmed = true;
  Blynk.virtualWrite(VPIN_CLAP_MODE, clapModeEnabled ? 1 : 0);

  Serial.print(F("Clap mode is now "));
  Serial.println(clapModeEnabled ? F("ON") : F("OFF"));
}

BLYNK_WRITE(VPIN_TOGGLE_BUTTON)
{
  if (param.asInt() == 1)
  {
    toggleLamp();
    Blynk.virtualWrite(VPIN_TOGGLE_BUTTON, 0);
  }
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  delay(300);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SOUND_DIGITAL_PIN, INPUT);

  setLamp(false);

  Serial.println(F("ClapControl IoT - ESP32 DevKit V1 + Blynk"));
  Serial.println(F("Relay IN: GPIO2 / D2"));
  Serial.println(F("KY-037 DO: GPIO22 / D22"));
  Serial.println(F("Connecting to WiFi and Blynk..."));

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);
  timer.setInterval(TELEMETRY_INTERVAL_MS, publishTelemetry);
}

void loop()
{
  Blynk.run();
  timer.run();
  handleClapDetection();
}
