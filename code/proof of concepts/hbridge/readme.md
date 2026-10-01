H-Brug Proof of ConceptMet deze test controleren we of de DRV8833 H-brug twee N20 DC-motoren onafhankelijk kan aansturen via de ESP32.   De twee motoren kunnen apart:Vooruit en achteruit draaien.Verschillende snelheden krijgen via PWM (0 tot 255).Tegelijk draaien en stoppen.De ESP32 stuurt de DRV8833 aan via de ingangen IN1, IN2, IN3 en IN4. De DRV8833 levert vervolgens het vermogen uit de 18650 batterijen door aan de twee motoren.   De snelheid wordt geregeld met een PWM-waarde van 0 tot 255. Een positieve waarde laat de motor vooruit draaien, een negatieve waarde achteruit.AansluitschemaDRV8833 IN1 $\rightarrow$ ESP32 D16 (Motor 1 richting A)   DRV8833 IN2 $\rightarrow$ ESP32 D17 (Motor 1 richting B)   DRV8833 IN3 $\rightarrow$ ESP32 D18 (Motor 2 richting A)   DRV8833 IN4 $\rightarrow$ ESP32 D19 (Motor 2 richting B)   DRV8833 EEP $\rightarrow$ ESP32 3V3 (Driver inschakelen)   DRV8833 VNC $\rightarrow$ 7.4V (Batterij +)   DRV8833 GND $\rightarrow$ ESP32 GND & Batterij - (Common GND)   Testcommando's (Seriële Monitor)Via de Seriële Monitor (op 115200 baud) kunnen de volgende commando's verzonden worden:M1 150 $\rightarrow$ Motor 1 vooruit op snelheid 150M1 -150 $\rightarrow$ Motor 1 achteruit op snelheid 150M2 100 $\rightarrow$ Motor 2 vooruit op snelheid 100M2 -100 $\rightarrow$ Motor 2 achteruit op snelheid 100STOP $\rightarrow$ Beide motoren stoppen directTestcode (ESP32 / Arduino IDE)C++// Pin definities volgens het schema
const int IN1 = 16; // Motor 1 (A1)
const int IN2 = 17;
const int IN3 = 18; // Motor 2 (A2)
const int IN4 = 19;

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotoren();
  Serial.println("DRV8833 PoC Gereed. Typ een commando (bijv: M1 150, M2 -100, STOP):");
}

void setMotor1(int snelheid) {
  snelheid = constrain(snelheid, -255, 255);
  if (snelheid > 0) {
    analogWrite(IN1, snelheid);
    analogWrite(IN2, 0);
  } else if (snelheid < 0) {
    analogWrite(IN1, 0);
    analogWrite(IN2, -snelheid);
  } else {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
  }
}

void setMotor2(int snelheid) {
  snelheid = constrain(snelheid, -255, 255);
  if (snelheid > 0) {
    analogWrite(IN3, snelheid);
    analogWrite(IN4, 0);
  } else if (snelheid < 0) {
    analogWrite(IN3, 0);
    analogWrite(IN4, -snelheid);
  } else {
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
  }
}

void stopMotoren() {
  setMotor1(0);
  setMotor2(0);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.equalsIgnoreCase("STOP")) {
      stopMotoren();
      Serial.println("Beide motoren gestopt.");
    } else if (input.startsWith("M1 ")) {
      int spd = input.substring(3).toInt();
      setMotor1(spd);
      Serial.print("Motor 1 ingesteld op: ");
      Serial.println(spd);
    } else if (input.startsWith("M2 ")) {
      int spd = input.substring(3).toInt();
      setMotor2(spd);
      Serial.print("Motor 2 ingesteld op: ");
      Serial.println(spd);
    }
  }
}
ResultaatDe twee N20 motoren kunnen onafhankelijk in snelheid en richting worden geregeld met de DRV8833 en ESP32. De H-Brug werkt dus zoals verwacht.

