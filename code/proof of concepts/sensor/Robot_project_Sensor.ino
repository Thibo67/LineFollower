/*
  PoC: QTR-8A Sensor Uitlezen via ADC1 (6 Sensoren)
  Microcontroller: ESP32 DevKitV1
*/

// Gebruik uitsluitend ADC1 pinnen (geen conflict met Wi-Fi/Bluetooth)
const int analogPins[6] = {36, 39, 34, 35, 32, 33}; 
const int irPin = 23; // IR aangesloten op D23

void setup() {
  Serial.begin(115200);

  // IR-LEDs inschakelen
  pinMode(irPin, OUTPUT);
  digitalWrite(irPin, HIGH);

  // Pinnen instellen & maximale ADC attenuatie (0 - 4095)
  for (int i = 0; i < 6; i++) {
    pinMode(analogPins[i], INPUT);
    analogSetPinAttenuation(analogPins[i], ADC_11db);
  }
}

void loop() {
  Serial.print("Sensoren (1-6): ");
  for (int i = 0; i < 6; i++) {
    int val = analogRead(analogPins[i]);
    Serial.print(val);
    Serial.print("\t");
  }
  Serial.println();

  delay(2500);
}
