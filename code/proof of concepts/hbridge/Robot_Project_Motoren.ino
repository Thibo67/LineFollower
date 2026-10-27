// Definieer de pins voor Motor 1 (gekoppeld aan IN1 en IN2)
const int motor1Pin1 = 16;   // D16
const int motor1Pin2 = 17;  // D17

// Definieer de pins voor Motor 2 (gekoppeld aan IN3 en IN4)
const int motor2Pin1 = 18;  // D18
const int motor2Pin2 = 19;  // D19

void setup() {
  // Stel alle motorstuurpennen in als uitgang
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  
  // Start met gestopte motoren
  stopMotors();
}

void loop() {
  // 1. Laat beide motoren vooruit draaien
  moveForward();
  delay(2000); // Wacht 2 seconden

  // 2. Stop beide motoren
  stopMotors();
  delay(1000); // Wacht 1 seconde

  // 3. Laat beide motoren achteruit draaien
  moveBackward();
  delay(2000); // Wacht 2 seconden

  // 4. Stop beide motoren en pauzeer langer voor de volgende cyclus
  stopMotors();
  delay(3000); // Wacht 3 seconden
}

// Functie om beide motoren vooruit te laten rijden
void moveForward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

// Functie om beide motoren achteruit te laten rijden
void moveBackward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

// Functie om beide motoren te stoppen
void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}
