/*
  PoC: QTR-8A Sensor Uitlezen (8 Sensoren)
  Microcontroller: ESP32 DevKitV1
*/

// Alle 8 analoge pinnen volgens jouw aansluiting
const int analogPins[8] = {34, 35, 32, 33, 25, 26, 27, 14};
const int irPin = 23; // IR aangesloten op D23

void setup() {
  Serial.begin(115200);

  // IR-LEDs inschakelen
  pinMode(irPin, OUTPUT);
  digitalWrite(irPin, HIGH);

  // Pinnen instellen & maximale ADC attenuatie (0 - 4095)
  for (int i = 0; i < 8; i++) {
    pinMode(analogPins[i], INPUT);
    analogSetPinAttenuation(analogPins[i], ADC_11db);
  }
}

void loop() {
  Serial.print("Sensoren (1-8): ");
  for (int i = 0; i < 8; i++) {
    int val = analogRead(analogPins[i]);
    Serial.print(val);
    Serial.print("\t");
  }
  Serial.println();

  delay(250);
}
