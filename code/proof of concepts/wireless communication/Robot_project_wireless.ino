#include "BluetoothSerial.h"

// Controleer of Bluetooth is ingeschakeld
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and select it
#endif

BluetoothSerial SerialBT;

void setup() {
  // Start de normale USB seriële monitor (voor Arduino IDE)
  Serial.begin(115200);
  
  // Start Bluetooth (voor PuTTY)
  SerialBT.begin("ESP32_LineFollower"); 
  
  Serial.println("\nSysteem gestart!");
  Serial.println("Je kunt nu typen via USB (Arduino IDE) of Bluetooth (PuTTY).");
}

void loop() {
  // --- 1. Ontvangen via Bluetooth (PuTTY) ---
  if (SerialBT.available()) {
    String incomingString = SerialBT.readStringUntil('\n'); // Lees tot je op Enter drukt
    incomingString.trim(); // Haal eventuele onnodige spaties/enters aan het einde weg
    
    if (incomingString.length() > 0) {
      // Print naar de Arduino USB monitor zodat je het daar ook ziet
      Serial.print("[Bluetooth van PuTTY ontvangen]: ");
      Serial.println(incomingString);
      
      // Stuur een antwoord terug naar PuTTY
      SerialBT.print("ESP32 ontving via Bluetooth: ");
      SerialBT.println(incomingString);
    }
  }

  // --- 2. Ontvangen via USB (Arduino IDE Seriële Monitor) ---
  if (Serial.available()) {
    String incomingString = Serial.readStringUntil('\n'); // Lees tot je op Enter drukt
    incomingString.trim();
    
    if (incomingString.length() > 0) {
      Serial.print("[USB ontvangen]: ");
      Serial.println(incomingString);
      
      // Stuur ook een bevestiging terug over de USB monitor
      Serial.print("ESP32 ontving via USB: ");
      Serial.println(incomingString);
      
      // (Optioneel) Stuur het ook door naar Bluetooth als PuTTY verbonden is
      SerialBT.print("ESP32 ontving via USB: ");
      SerialBT.println(incomingString);
    }
  }
}