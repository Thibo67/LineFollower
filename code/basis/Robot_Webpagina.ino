#include <Arduino.h>
#include <EEPROM.h>
#include <WiFi.h>

#define EEPROM_SIZE 512
const int SensorCount = 6;

// ===============================
// WI-FI CONFIGURATIE (Access Point Modus)
// ===============================
// De ESP32 maakt zijn eigen Wi-Fi netwerk aan waar je gsm mee verbindt
const char* ssid = "ESP32_LineFollower";
const char* password = "12345678password"; // Minimaal 8 tekens

WiFiServer server(80);

// ===============================
// PINNEN CONFIGURATIE
// ===============================
const int analogPins[SensorCount] = {36, 39, 34, 35, 32, 33}; 
const int irPin = 23;

const int motorA_IN1 = 16;
const int motorA_IN2 = 17;
const int motorB_IN1 = 18;
const int motorB_IN2 = 19;

const int ledPin = 21;       // LED op D21
const int buttonPin = 22;   // Drukknop op D22 (interne pull-up)

// Instelbare snelheden (aanpasbaar via de webpagina)
int baseSpeed = 100;  
int turnSpeed = 80;   

// ===============================
// EEPROM & STRUCT
// ===============================
struct param_t
{
  unsigned long cycleTime;
  int black[SensorCount];
  int white[SensorCount];
} params;

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

  // Start Wi-Fi in Access Point (AP) modus
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("\n--- Wi-Fi Web Server Gestart ---");
  Serial.print("Verbind je gsm met Wi-Fi: ");
  Serial.println(ssid);
  Serial.print("Open in je browser op je gsm: http://");
  Serial.println(myIP); // Dit is het IP-adres (vaak 192.168.4.1)

  server.begin();

  pinMode(irPin, OUTPUT);
  digitalWrite(irPin, HIGH);

  for (int i = 0; i < SensorCount; i++) {
    pinMode(analogPins[i], INPUT);
    analogSetPinAttenuation(analogPins[i], ADC_11db);
  }

  pinMode(motorA_IN1, OUTPUT);
  pinMode(motorA_IN2, OUTPUT);
  pinMode(motorB_IN1, OUTPUT);
  pinMode(motorB_IN2, OUTPUT);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonPress, FALLING);
}

void driveMotor(int IN1, int IN2, int speed, bool invert = false) {
  if (invert) {
    speed = -speed; 
  }

  if (speed > 0) {
    analogWrite(IN1, speed);
    analogWrite(IN2, 0);
  } else if (speed < 0) {
    analogWrite(IN1, 0);
    analogWrite(IN2, -speed);
  } else {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
  }
}

void loop()
{
  // 1. Knop / Interrupt afhandelen
  if (buttonPressed)
  {
    buttonPressed = false;
    unsigned long currentMillis = millis();

    if (currentMillis - lastButtonTime >= debounceDelay)
    {
      lastButtonTime = currentMillis;
      robotRunning = !robotRunning; 

      if (robotRunning) {
        Serial.println("START via knop");
      } else {
        Serial.println("STOP via knop");
        driveMotor(motorA_IN1, motorA_IN2, 0, true);
        driveMotor(motorB_IN1, motorB_IN2, 0, false);
      }
    }
  }

  // 2. LED Gedrag
  if (robotRunning)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisLED >= intervalLED)
    {
      previousMillisLED = currentMillis;
      ledBlinkState = !ledBlinkState;
      digitalWrite(ledPin, ledBlinkState);
    }
  }
  else
  {
    digitalWrite(ledPin, HIGH);
  }

  // 3. Web Server Verzoeken van GSM afhandelen
  WiFiClient client = server.available();   
  if (client) {
    String currentLine = "";
    String requestString = "";
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        requestString += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            
            // --- URL COMMANDO'S VERWERKEN ---
            if (requestString.indexOf("GET /start") >= 0) {
              robotRunning = true;
              Serial.println("WEB: Start");
            }
            if (requestString.indexOf("GET /stop") >= 0) {
              robotRunning = false;
              driveMotor(motorA_IN1, motorA_IN2, 0, true);
              driveMotor(motorB_IN1, motorB_IN2, 0, false);
              Serial.println("WEB: Stop");
            }
            if (requestString.indexOf("GET /speedup") >= 0) {
              baseSpeed = min(255, baseSpeed + 15);
              Serial.print("WEB: Snelheid verhoogd naar: "); Serial.println(baseSpeed);
            }
            if (requestString.indexOf("GET /speeddown") >= 0) {
              baseSpeed = max(40, baseSpeed - 15);
              Serial.print("WEB: Snelheid verlaagd naar: "); Serial.println(baseSpeed);
            }

            // --- HTML PAGINA NAAR GSM STUREN ---
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            client.println("<!DOCTYPE html><html><head>");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<style>");
            client.println("body { font-family: Arial; text-align: center; background: #222; color: #fff; margin-top: 30px; }");
            client.println("h1 { color: #4CAF50; }");
            client.println(".btn { font-size: 20px; padding: 15px 30px; margin: 10px; border: none; border-radius: 8px; cursor: pointer; color: white; width: 80%; max-width: 300px; }");
            client.println(".btn-start { background-color: #4CAF50; }");
            client.println(".btn-stop { background-color: #f44336; }");
            client.println(".btn-speed { background-color: #ff9800; }");
            client.println(".status { font-size: 24px; font-weight: bold; margin: 20px; color: " + String(robotRunning ? "#4CAF50" : "#f44336") + "; }");
            client.println("</style></head><body>");
            
            client.println("<h1>ESP32 Lijnvolger</h1>");
            client.println("<div class=\"status\">Status: " + String(robotRunning ? "RIJDT" : "GESTOPT") + "</div>");
            client.println("<p>Huidige basissnelheid: " + String(baseSpeed) + "</p>");
            
            client.println("<p><a href=\"/start\"><button class=\"btn btn-start\">START</button></a></p>");
            client.println("<p><a href=\"/stop\"><button class=\"btn btn-stop\">STOP</button></a></p>");
            client.println("<p><a href=\"/speedup\"><button class=\"btn btn-speed\">Snelheid +</button></a></p>");
            client.println("<p><a href=\"/speeddown\"><button class=\"btn btn-speed\">Snelheid -</button></a></p>");
            
            client.println("</body></html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }

  // 4. Sensoren & Lijnvolg-logica (Alleen als robotRunning waar is)
  int sensorValues[SensorCount];
  for (int i = 0; i < SensorCount; i++) {
    sensorValues[i] = analogRead(analogPins[i]);
  }

  if (robotRunning)
  {
    int midLeft = sensorValues[2];   
    int midRight = sensorValues[3];  
    int threshold = 3800; 

    if (midLeft > threshold && midRight > threshold) {
      driveMotor(motorA_IN1, motorA_IN2, baseSpeed, true);
      driveMotor(motorB_IN1, motorB_IN2, baseSpeed, false);
    }
    else if (midLeft > threshold) {
      driveMotor(motorA_IN1, motorA_IN2, -turnSpeed, true);
      driveMotor(motorB_IN1, motorB_IN2, baseSpeed, false);
    }
    else if (midRight > threshold) {
      driveMotor(motorA_IN1, motorA_IN2, baseSpeed, true);
      driveMotor(motorB_IN1, motorB_IN2, -turnSpeed, false);
    }
    else {
      driveMotor(motorA_IN1, motorA_IN2, baseSpeed - 20, true);
      driveMotor(motorB_IN1, motorB_IN2, baseSpeed - 20, false);
    }
  }

  delay(20);
}