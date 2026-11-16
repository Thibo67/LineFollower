# Instructable:

LineFollower Robot Nabouwen
# 1.Onderdelen bestellen:Bill of Materials.

Bestel alle benodigde componenten uit de onderdelenlijst
:1x ESP32 DevKit V1 (USB-C / Micro-USB)
1x DRV8833 H-brug motordriver
2x JGA12-N20-30 DC-reductiemotoren (6V - 500 RPM)
1x QTR-8A IR sensor-array
2x 18650 Li-ion batterijen + 1x dubbele 18650 batterijhouder
1x XTAR MC2 batterijlader1x Prototype PCB (8x10 cm)
2x 3D-geprinte motorsteunen + 2x 3D-geprinte wielen + 1x Mini 3PI caster wiel
1x Drukknop, 1x Aan/uit-schakelaar, 1x LED (met 220Ω weerstand), headers en 22AWG draad

# 2.Chassis & Mechanische Montage:Hardware assembleren.

Monteer het zwenkwiel (caster wheel) aan de voor/onderzijde van het prototypeboard.
Bevestig de twee N20-motoren aan het chassis met de 3D-geprinte motorsteunen en druk de 3D-geprinte wielen op de motorassen.
Bevestig de QTR-8A sensor-array horizontaal aan de voorkant onder het chassis op een vaste hoogte boven de grond.
Monteer de batterijhouder en het prototypeboard op het chassis.

# 3.Elektrische Bedrading & Soldering:Volgens elektrisch schema.

Soldeer alle verbindingen exact volgens het schakelschema:
Voeding:
Verbind de + van de 2x 18650 batterijhouder via Schakelaar (S1) met de VIN van de ESP32 en de VCC van de DRV8833.
Verbind de - van de batterijhouder met de GND van de ESP32, DRV8833 en sensoren.
DRV8833 Motor Driver:
IN1 → ESP32 GPIO 16
IN2 → ESP32 GPIO 17
IN3 → ESP32 GPIO 18
IN4 → ESP32 GPIO 19
OUT1 & OUT2 → DC Motor2
OUT3 & OUT4 → DC Motor 1
ULT & EEP → Verbinden volgens schema (Logic HIGH / 3V3).

QTR-8A Sensor Array:
VCC → ESP32 3V3
GND → ESP32 GND
IR → ESP32 GPIO 23
D1 t/m D6 → ESP32 ADC1 pinnen GPIO 36, 39, 34, 35, 32, 33.
Bediening & Indicatie:
Drukknop aansluiten tussen ESP32 GPIO 22 en GND.
LED anodeside aansluiten op ESP32 GPIO 21 via een 220Ω voorschakelweerstand naar GND.

# 4.Softwareomgeving & Compiler Configureren:Arduino IDE.

Open Arduino IDE op de computer.
Voeg de ESP32 board support toe via Bestand > Voorkeuren > Additional Boards Manager URLs: [https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json).
Ga naar Hulpmiddelen > Board > Board Beheerder, zoek op esp32 en installeer het ESP32-boardpakket.
Selecteer het board DOIT ESP32 DEVKIT V1 onder Hulpmiddelen > Board > ESP32.5.Code Compileren & Uploaden:Firmware Flashen.
Maak een nieuw bestand aan in Arduino IDE, kopieer de volledige broncode (Robot_bijna_klaar.ino) erin en sla het op.
Sluit de ESP32 via een USB-kabel aan op de computer.
Selecteer de juiste COM-poort onder Hulpmiddelen > Poort.
Klik op de knop Uploaden (of druk op Ctrl + U).
Verification: De Arduino IDE geeft de melding "Done uploading" weer en de Seriële Monitor (op 115200 baud) toont de opstartmeldingen met het IP-adres van het Wi-Fi Access Point.

# 5. Inbedrijfstelling & Kalibratie (Eerste run)
Voeding inschakelen: Plaats twee opgeladen 18650 batterijen in de batterijhouder.Schakel de robot in via Schakelaar S1.
Draadloze verbinding maken (Bluetooth):Zet Bluetooth aan op je laptop of smartphone en koppel met het apparaat ESP32_LineFollower.Open een seriële terminal (zoals PuTTY of Serial Bluetooth Terminal) en maak verbinding met een baudrate van 115200. 
(Alternatief: Sluit de USB-kabel aan en open de Arduino Seriële Monitor).

Kalibratieprogramma uploaden:Tijdelijke firmware.
Sluit de ESP32 met de USB-kabel aan op je computer.
Open het bestand Robot_kalibratie.ino (uit code/proof of concepts/Kalibratie) in de Arduino IDE.
Selecteer het board DOIT ESP32 DEVKIT V1 en de juiste COM-poort.Klik op Uploaden (Ctrl + U).

Sensoren kalibreren via Bluetooth of USB:EEPROM vullen.
Schakel de robot in via Schakelaar S1 (gegeveden door de 18650 batterijen).Maak verbinding met de robot:Via Bluetooth: Koppel op je pc of smartphone met het apparaat ESP32_LineFollower en open een seriële terminal (zoals PuTTY of Serial Bluetooth Terminal) op 115200 baud.Via USB: Open de Seriële Monitor in Arduino IDE op 115200 baud.Witte ondergrond: Plaats alle 6 de sensoren (GPIO 36, 39, 34, 35, 32, 33) boven de witte ondergrond en stuur exact het commando calibrate white.Zwarte lijn: Plaats alle 6 de sensoren boven de zwarte lijn en stuur exact het commando calibrate black.Controleer of de melding "Waardes opgeslagen in EEPROM!" op het scherm verschijnt. De minimale en maximale reflectiewaarden staan nu permanent in het EEPROM-geheugen van de ESP32 geschreven.3.

Finale hoofdcode uploaden:Finaal programma.Open het hoofdprogramma Robot_bijna_klaar.ino in de Arduino IDE.Upload de code naar de ESP32.De finale code leest tijdens de setup() automatisch de opgeslagen kalibratiewaarden uit het EEPROM-geheugen in (EEPROM_readAnything(0, params)), zodat de kwadratische interpolatie direct met de juiste drempelwaarden rekent.Verbind je smartphone met het Wi-Fi-netwerk ESP32_LineFollower (Wachtwoord: 12345678password), surf naar [http://192.168.4.1](http://192.168.4.1) en druk op START om te rijden.
