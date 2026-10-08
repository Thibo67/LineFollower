/*
  Proof of Concept: QTR-8A Analoge Lijnvolger Uitlezen
  Microcontroller: ESP32 DevKitV1
*/

// De 8 analoge ingangspinnen voor de QTR-8A sensor
const int qtrPins[8] = {36, 39, 34, 35, 32, 33, 25, 26}; // VP, VN, D34, D35, D32, D33, D25, D26
const int irEmitterPin = 23;                            // Pin om de IR-LEDs aan te sturen

void setup() {
  // Start de seriële communicatie op 115200 baudrate
  Serial.begin(115200);

  // Schakel de IR-LEDs van de sensor in
  pinMode(irEmitterPin, OUTPUT);
  digitalWrite(irEmitterPin, HIGH);

  // Stel alle sensorpinnen in als ingang
  for (int i = 0; i < 8; i++) {
    pinMode(qtrPins[i], INPUT);
  }

  Serial.println("ESP32 Gestart! QTR-8A sensorwaarden worden uitgelezen...");
}

void loop() {
  int sensorValues[8];

  // Lees alle 8 sensoren achter elkaar uit
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = analogRead(qtrPins[i]);
  }

  // Print de waarden overzichtelijk op één regel
  for (int i = 0; i < 8; i++) {
    Serial.print("S");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(sensorValues[i]);
    Serial.print("\t"); // Tab-afstand tussen sensoren
  }
  Serial.println(); // Nieuwe regel

  delay(200); // Wacht 0.2 seconden voor de volgende meting
}