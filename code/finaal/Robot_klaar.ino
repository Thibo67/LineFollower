#include <Arduino.h>
#include <EEPROM.h>
#include <WiFi.h>

#define EEPROM_SIZE 512
const int SensorCount = 6;

// ===============================
// WI-FI CONFIGURATIE (Access Point)
// ===============================
const char* ssid = "ESP32_LineFollower";
const char* password = "12345678password";
WiFiServer server(80);

// ===============================
// PINNEN CONFIGURATIE
// ===============================
const int sensor[SensorCount] = {36, 39, 34, 35, 32, 33}; // ADC1 pinnen
const int irPin = 23;

const int motorA_IN1 = 16;
const int motorA_IN2 = 17;
const int motorB_IN1 = 18;
const int motorB_IN2 = 19;

const int ledPin = 21;      // LED op D21
const int buttonPin = 22;   // Drukknop op D22 (interne pull-up)

// ===============================
// PARAMETERS & PID VARIABELEN
// ===============================
struct param_t
{
  unsigned long cycleTime; // in microseconden (20000 µs = 20ms)
  int black[SensorCount];
  int white[SensorCount];
  float kp;
  float ki;
  float kd;
  int power;
  float diff;
} params;

bool debug = false;
unsigned long calculationTime = 0;
unsigned long previous = 0;

float iTerm = 0;
float lastErr = 0;
float motorTrim = 0.85; // Correctiefactor voor motor A

// ===============================
// VARIABELEN VOOR BESTURING & LED
// ===============================
volatile bool buttonPressed = false;
bool robotRunning = false; 

unsigned long lastButtonTime = 0;
const unsigned long debounceDelay = 200; 

unsigned long previousMillisLED = 0;
const unsigned long intervalLED = 300; 
bool ledBlinkState = LOW;

// Hulpfuncties voor EEPROM
template <class T> int EEPROM_writeAnything(int ee, const T& value) {
  const byte* p = (const byte*)(const void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    EEPROM.write(ee++, *p++);
  EEPROM.commit();
  return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value) {
  byte* p = (byte*)(void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
  return i;
}

void IRAM_ATTR handleButtonPress()
{
  buttonPressed = true;
}

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  EEPROM_readAnything(0, params);

  // Standaardwaarden als EEPROM leeg is of ongeldige gegevens bevat
  if (params.cycleTime == 0 || params.cycleTime > 1000000) {
    params.cycleTime = 20000; // 20 ms
    params.kp = 1.0;
    params.ki = 0.0;
    params.kd = 0.0;
    params.power = 100;
    params.diff = 0.5;

    // Standaard ADC-drempels instellen om deling door nul te voorkomen
    for (int i = 0; i < SensorCount; i++) {
      params.black[i] = 3000;
      params.white[i] = 500;
    }
    EEPROM_writeAnything(0, params);
