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
  unsigned long cycleTime; // in microseconden, bvb 20000 (20ms)
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
float motorTrim = 0.85; // Pas dit getal aan (bijv. 0.50 tot 0.98)

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

  // Standaardwaarden als EEPROM leeg is
  if (params.cycleTime == 0 || params.cycleTime > 1000000) {
    params.cycleTime = 20000; // 20 ms
    params.kp = 1.0;
    params.ki = 0.0;
    params.kd = 0.0;
    params.power = 100;
    params.diff = 0.5;
  }

  // Start Wi-Fi AP
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("\n--- Robot Systeem met Web Invoer & PID Gestart ---");
  Serial.print("IP-adres voor gsm: http://");
  Serial.println(myIP);

  server.begin();

  pinMode(irPin, OUTPUT);
  digitalWrite(irPin, HIGH);

  for (int i = 0; i < SensorCount; i++) {
    pinMode(sensor[i], INPUT);
    analogSetPinAttenuation(sensor[i], ADC_11db);
  }

  pinMode(motorA_IN1, OUTPUT);
  pinMode(motorA_IN2, OUTPUT);
  pinMode(motorB_IN1, OUTPUT);
  pinMode(motorB_IN2, OUTPUT);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonPress, FALLING);
  
  previous = micros();
}

void driveMotor(int IN1, int IN2, int speed, bool invert = false) {
  if (invert) {
    speed = -speed; 
  }

  // --- MOTOR BALANS CORRECTIE VOOR MOTOR A (OUT 1 & 2) ---
  if (IN1 == motorA_IN1) {
    speed = (float)speed * motorTrim; 
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

// Hulpfunctie om URL-parameters te decoderen
String urldecode(String str) {
  str.replace("+", " ");
  char buf[str.length() + 1];
  str.toCharArray(buf, sizeof(buf));
  char *p = buf;
  String decoded = "";
  while (*p) {
    if (*p == '%' && p[1] && p[2]) {
      char hex[3] = { p[1], p[2], 0 };
      decoded += (char)strtol(hex, NULL, 16);
      p += 3;
    } else {
      decoded += *p;
      p++;
    }
  }
  return decoded;
}

void loop()
{
  // 1. Fysieke knop / Interrupt afhandelen
  if (buttonPressed)
  {
    buttonPressed = false;
    unsigned long currentMillis = millis();

    if (currentMillis - lastButtonTime >= debounceDelay)
    {
      lastButtonTime = currentMillis;
      robotRunning = !robotRunning; 

      if (!robotRunning) {
        driveMotor(motorA_IN1, motorA_IN2, 0, true);
        driveMotor(motorB_IN1, motorB_IN2, 0, false);
        iTerm = 0; lastErr = 0;
        Serial.println("STOP via knop");
      } else {
        iTerm = 0; lastErr = 0;
        Serial.println("START via knop");
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
            
            if (requestString.indexOf("GET /start") >= 0) {
              robotRunning = true;
              iTerm = 0; lastErr = 0;
              Serial.println("WEB: Start ontvangen via gsm!");
            }
            if (requestString.indexOf("GET /stop") >= 0) {
              robotRunning = false;
              driveMotor(motorA_IN1, motorA_IN2, 0, true);
              driveMotor(motorB_IN1, motorB_IN2, 0, false);
              Serial.println("WEB: Stop ontvangen via gsm!");
            }

            // --- ROBUUSTE PARAMETER PARSER ---
            int idxUpdate = requestString.indexOf("GET /update?");
            if (idxUpdate >= 0) {
              int endOfLine = requestString.indexOf('\n', idxUpdate);
              if (endOfLine < 0) endOfLine = requestString.length();
              String requestLine = requestString.substring(idxUpdate, endOfLine);
              
              int qPos = requestLine.indexOf('?');
              int sPos = requestLine.indexOf(' ', qPos);
              if (sPos < 0) sPos = requestLine.length();
              String queryString = requestLine.substring(qPos + 1, sPos);
              
              auto getParamVal = [&](String q, String name) {
                int p = q.indexOf(name + "=");
                if (p < 0) return String("");
                int ampersand = q.indexOf("&", p);
                if (ampersand < 0) ampersand = q.length();
                return urldecode(q.substring(p + name.length() + 1, ampersand));
              };

              String sKp = getParamVal(queryString, "kp");
              String sKi = getParamVal(queryString, "ki");
              String sKd = getParamVal(queryString, "kd");
              String sPow = getParamVal(queryString, "power");
              String sDiff = getParamVal(queryString, "diff");

              if (sKp.length() > 0) params.kp = sKp.toFloat();
              if (sKi.length() > 0) {
                float kiUser = sKi.toFloat();
                float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
                params.ki = kiUser * cycleTimeInSec;
              }
              if (sKd.length() > 0) {
                float kdUser = sKd.toFloat();
                float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
                params.kd = kdUser / cycleTimeInSec;
              }
              if (sPow.length() > 0) params.power = sPow.toInt();
              if (sDiff.length() > 0) params.diff = sDiff.toFloat();

              EEPROM_writeAnything(0, params);
              Serial.println("Parameters bijgewerkt & opgeslagen in EEPROM!");
              Serial.print("Nieuwe Power: "); Serial.println(params.power);
            }

            float currentKiDisplay = params.ki / (((float) params.cycleTime) / 1000000);
            float currentKdDisplay = params.kd * (((float) params.cycleTime) / 1000000);

            // HTML Pagina naar gsm sturen
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            client.println("<!DOCTYPE html><html><head>");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<style>");
            client.println("body { font-family: Arial; text-align: center; background: #222; color: #fff; margin: 10px; padding-bottom: 30px; }");
            client.println("h1 { color: #4CAF50; font-size: 22px; }");
            client.println("h2 { color: #ff9800; font-size: 18px; margin-top: 20px; }");
            client.println(".btn { font-size: 18px; padding: 12px 20px; margin: 5px; border: none; border-radius: 8px; cursor: pointer; color: white; width: 45%; }");
            client.println(".btn-start { background-color: #4CAF50; }");
            client.println(".btn-stop { background-color: #f44336; }");
            client.println(".btn-save { background-color: #2196F3; width: 92%; padding: 14px; margin-top: 15px; font-size: 20px; }");
            client.println(".status { font-size: 20px; font-weight: bold; margin: 10px; color: " + String(robotRunning ? "#4CAF50" : "#f44336") + "; }");
            client.println("form { background: #333; padding: 15px; border-radius: 10px; display: inline-block; width: 95%; max-width: 400px; text-align: left; }");
            client.println(".input-group { margin-bottom: 10px; }");
            client.println("label { display: inline-block; width: 40%; font-size: 16px; }");
            client.println("input { width: 55%; padding: 8px; font-size: 16px; background: #222; color: #fff; border: 1px solid #555; border-radius: 5px; }");
            client.println("</style></head><body>");
            
            client.println("<h1>ESP32 Lijnvolger Besturing</h1>");
            client.println("<div class=\"status\">Status: " + String(robotRunning ? "RIJDT" : "GESTOPT") + "</div>");
            
            client.println("<p><a href=\"/start\"><button class=\"btn btn-start\">START</button></a>");
            client.println("<a href=\"/stop\"><button class=\"btn btn-stop\">STOP</button></a></p>");
            
            client.println("<h2>PID & Motor Instellingen</h2>");
            client.println("<form action=\"/update\" method=\"GET\">");
            client.println("<div class=\"input-group\"><label>Kp:</label><input type=\"text\" name=\"kp\" value=\"" + String(params.kp) + "\"></div>");
            client.println("<div class=\"input-group\"><label>Ki:</label><input type=\"text\" name=\"ki\" value=\"" + String(currentKiDisplay) + "\"></div>");
            client.println("<div class=\"input-group\"><label>Kd:</label><input type=\"text\" name=\"kd\" value=\"" + String(currentKdDisplay) + "\"></div>");
            client.println("<div class=\"input-group\"><label>Power:</label><input type=\"text\" name=\"power\" value=\"" + String(params.power) + "\"></div>");
            client.println("<div class=\"input-group\"><label>Diff:</label><input type=\"text\" name=\"diff\" value=\"" + String(params.diff) + "\"></div>");
            client.println("<button type=\"submit\" class=\"btn btn-save\">Opslaan & Doorvoeren</button>");
            client.println("</form>");
            
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

  // 4. Cyclusgestuurde Regellus (Normalisatie, Kwadratische Interpolatie & PID)
  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;

    // --- A. NORMALISATIE ---
    long normalised[SensorCount];
    for (int i = 0; i < SensorCount; i++)
    {
      long value = analogRead(sensor[i]);
      value = constrain(value, min(params.black[i], params.white[i]), max(params.black[i], params.white[i]));
      normalised[i] = map(value, params.black[i], params.white[i], 0, 1000);
    }

    float position = 0.0;

    if (robotRunning)
    {
      // --- B. KWADRATISCHE INTERPOLATIE ---
      int index = 0;
      for (int i = 1; i < SensorCount; i++) {
        if (normalised[i] < normalised[index]) index = i; 
      }

      if (index == 0) index = 1;
      else if (index == SensorCount - 1) index = SensorCount - 2;

      long sZero = normalised[index];
      long sMinusOne = normalised[index - 1];
      long sPlusOne = normalised[index + 1];

      float b = ((float)(sPlusOne - sMinusOne)) / 2;
      float a = sPlusOne - b - sZero;

      if (a != 0) {
        position = -b / (2 * a);
      } else {
        position = 0;
      }
      
      position += index - ((SensorCount - 1) / 2.0);
      position *= 15; // Sensorafstand in mm

      // --- C. PID REGELAAR ---
      float error = -position; 

      float output = error * params.kp;

      iTerm += params.ki * error;
      iTerm = constrain(iTerm, -510, 510);
      output += iTerm;

      output += params.kd * (error - lastErr);
      lastErr = error;

      output = constrain(output, -510, 510);

      // --- D. MOTOR AANSTURING ---
      int powerLeft, powerRight;
      if (output >= 0)
      {
        powerLeft = constrain(params.power + params.diff * output, -255, 255);
        powerRight = constrain(powerLeft - output, -255, 255);
        powerLeft = powerRight + output;
      }
      else
      {
        powerRight = constrain(params.power - params.diff * output, -255, 255);
        powerLeft = constrain(powerRight + output, -255, 255);
        powerRight = powerLeft - output;
      }

      driveMotor(motorA_IN1, motorA_IN2, powerLeft, true);
      driveMotor(motorB_IN1, motorB_IN2, powerRight, false);
    }
  }

  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}
