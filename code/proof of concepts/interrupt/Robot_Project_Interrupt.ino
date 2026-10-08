
// ===============================
// PINNEN
// ===============================
const int pinR = 21;
const int pinG = 5;
const int pinB = 4;

const int buttonPin = 22;


// ===============================
// INTERRUPT
// ===============================
volatile bool buttonPressed = false;


// ===============================
// LED CYCLUS
// ===============================
bool isInterrupted = false;

unsigned long previousMillis = 0;

int colorState = 0;
// 0 = rood
// 1 = groen
// 2 = blauw

const unsigned long interval = 1000;


// ===============================
// DEBOUNCE
// ===============================
unsigned long lastButtonTime = 0;
const unsigned long debounceDelay = 200;


// ===============================
// INTERRUPT FUNCTIE
// ===============================
void IRAM_ATTR handleButtonPress()
{
  buttonPressed = true;
}


// ===============================
// SETUP
// ===============================
void setup()
{
  Serial.begin(115200);

  Serial.println();
  Serial.println("ESP32 RGB LED gestart");

  // LED-pinnen
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  // Knop
  // Externe 1k pulldown staat al in jouw schema
  pinMode(buttonPin, INPUT);

  // Interrupt bij stijgende flank
  attachInterrupt(
    digitalPinToInterrupt(buttonPin),
    handleButtonPress,
    RISING
  );

  // LED uit bij opstart
  setColor(LOW, LOW, LOW);

  previousMillis = millis();
}


// ===============================
// LOOP
// ===============================
void loop()
{
  // --------------------------------
  // KNOP / INTERRUPT AFHANDELEN
  // --------------------------------
  if (buttonPressed)
  {
    unsigned long currentMillis = millis();

    // Debounce
    if (currentMillis - lastButtonTime >= debounceDelay)
    {
      lastButtonTime = currentMillis;

      // Interrupt-vlag resetten
      buttonPressed = false;

      // Status veranderen
      isInterrupted = !isInterrupted;

      if (isInterrupted)
      {
        // INTERRUPT ACTIEF
        Serial.println("INTERRUPT -> WIT");

        setColor(HIGH, HIGH, HIGH);
      }
      else
      {
        // INTERRUPT UIT
        Serial.println("CYCLUS HERVAT");

        // Timing opnieuw starten
        previousMillis = millis();
      }
    }
  }


  // --------------------------------
  // NORMALE RGB-CYCLUS
  // --------------------------------
  if (!isInterrupted)
  {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;

      nextColor();
    }
  }
}


// ===============================
// VOLGENDE KLEUR
// ===============================
void nextColor()
{
  switch (colorState)
  {
    case 0:
      Serial.println("ROOD");

      setColor(HIGH, LOW, LOW);
      break;


    case 1:
      Serial.println("GROEN");

      setColor(LOW, HIGH, LOW);
      break;


    case 2:
      Serial.println("BLAUW");

      setColor(LOW, LOW, HIGH);
      break;
  }

  colorState++;

  if (colorState > 2)
  {
    colorState = 0;
  }
}


// ===============================
// RGB LED AANSTUREN
// ===============================
void setColor(int r, int g, int b)
{
  digitalWrite(pinR, r);
  digitalWrite(pinG, g);
  digitalWrite(pinB, b);
}