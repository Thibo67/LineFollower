#include <EEPROM.h>
#include "BluetoothSerial.h"

#define EEPROM_SIZE 512
const int SensorCount = 6;

// De exact aangesloten pinnen volgens jouw nieuwe schema (D2 t/m D7)
const int analogPins[SensorCount] = {36, 39, 34, 35, 32, 33}; // VP, VN, D34, D35, D32, D33
const int irPin = 23; // IR aangesloten op D23

BluetoothSerial SerialBT;

// EEPROM structuur voor parameters en kalibratiewaardes (voor 6 sensoren)
struct param_t
{
  unsigned long cycleTime;
  int black[SensorCount];
  int white[SensorCount];
} params;

// Hulpfuncties voor EEPROM opslag op de ESP32
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

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  // Laad eventuele opgeslagen kalibratiewaardes uit de EEPROM bij opstarten
  EEPROM_readAnything(0, params);

  // Start Bluetooth (voor PuTTY) en USB
  SerialBT.begin("ESP32_LineFollower");

  // IR-LEDs inschakelen
  pinMode(irPin, OUTPUT);
  digitalWrite(irPin, HIGH);

  // Pinnen instellen & maximale ADC attenuatie (0 - 4095)
  for (int i = 0; i < SensorCount; i++) {
    pinMode(analogPins[i], INPUT);
    analogSetPinAttenuation(analogPins[i], ADC_11db);
  }

  Serial.println("\nSysteem gestart met 6 sensoren (D2 t/m D7)!");
  Serial.println("Typ 'calibrate black' of 'calibrate white' via USB of PuTTY.");
}

void loop() {
  // Controleer op inkomende commando's via Bluetooth (PuTTY) of USB Seriële Monitor
  String inputLine = "";
  if (SerialBT.available()) {
    inputLine = SerialBT.readStringUntil('\n');
  } else if (Serial.available()) {
    inputLine = Serial.readStringUntil('\n');
  }

  inputLine.trim();
  if (inputLine.length() > 0) {
    onCalibrate(inputLine);
  }

  // Standaard sensoren uitlezen en printen (Sensoren 2 t/m 7 uit het schema)
  Serial.print("Sensoren (D2-D7): ");
  for (int i = 0; i < SensorCount; i++) {
    int val = analogRead(analogPins[i]);
    Serial.print(val);
    Serial.print("\t");
  }
  Serial.println();

  delay(500);
}

// Kalibratie eventhandler
void onCalibrate(String param) {
  if (param == "black")
  {
    Serial.println("Start calibrating black... ");
    for (int i = 0; i < SensorCount; i++) {
      params.black[i] = analogRead(analogPins[i]);
    }
    Serial.println("done");
  }
  else if (param == "white")
  {
    Serial.println("Start calibrating white... ");
    for (int i = 0; i < SensorCount; i++) {
      params.white[i] = analogRead(analogPins[i]);
    }
    Serial.println("done");
  }
  else {
    Serial.print("Onbekend commando: ");
    Serial.println(param);
    return;
  }

  // Sla de struct direct op in het EEPROM geheugen van de ESP32
  EEPROM_writeAnything(0, params);
  Serial.println("Waardes opgeslagen in EEPROM!");
}
