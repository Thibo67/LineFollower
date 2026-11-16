# Gebruiksaanwijzing

### opladen / vervangen batterijen
Voeding: De robot werkt op twee 18650 Li-ion batterijen in serie (in totaal 7.4V nominale spanning; opgeladen tot 8.4V).

Opladen:
Schakel de robot altijd eerst uit.
Haal de 18650 batterijen uit de houder en plaats ze in een geschikte externe Li-ion lader (zoals een 18650 balancer/lader).
Let op: Laad de 18650 batterijen niet op via de micro-USB/USB-C poort van de ESP32 zelf; de USB-poort dient enkel voor het voeden van de ESP32 tijdens het programmeren of testen.

Vervangen:
Verwijder de lege 18650 cellen uit de batterijhouder.
Laadt eerst de 18650 batterijen op met bijhorende lader.
Belangrijk: Let zeer strikt op de polariteit (+ en - zijde op de houder). Het verkeerd om plaatsen van 18650 batterijen veroorzaakt direct een kortsluiting en kan de motorcontroller of ESP32 permanent beschadigen.

### draadloze communicatie
#### verbinding maken
Verbinding maken met smartphone of laptop:Schakel de robot in met de hoofdschakelaar. 
De ESP32 start automatisch een eigen Wi-Fi Access Point (AP) op.
Zoek op je smartphone of laptop naar het Wi-Fi-netwerk met de naam ESP32_LineFollower.   
Maak verbinding met dit netwerk en voer het wachtwoord 12345678password in.   
Open een webbrowser (bijv. Chrome of Safari) en navigeer naar het IP-adres: [http://192.168.4.1](http://192.168.4.1) (het IP-adres wordt ook getoond via de Seriële Monitor op 115200 baud).   
Je komt nu op het web-dashboard van de robot waar je live de status kunt aflezen, de robot kunt starten/stoppen en de PID-parameters direct kunt aanpassen. 

#### commando's
Webinterface Commando's & Instellingen

De webinterface biedt een dashboard met de status ("RIJDT" of "GESTOPT") en de volgende bedieningsmogelijkheden:
START (Knop): Zet de robot in de rij-modus (robotRunning = true).
STOP (Knop): Zet de robot stil en schakelt de motoren uit (robotRunning = false).
Kp: Proportionele versterking van de PID-regelaar (invoerveld).
Ki: Integrale versterking van de PID-regelaar (invoerveld, wordt intern geschaald naar de vaste cyclustijd van 20 ms).
Kd: Derivatieve versterking van de PID-regelaar (invoerveld, wordt intern geschaald naar de vaste cyclustijd van 20 ms).
Power: Basismotorvermogen (invoerveld, waarde tussen 0 en 255).
Diff: Stuurgevoeligheid / differentieelcoëfficiënt (invoerveld, waarde tussen 0.0 en 1.0).
Opslaan & Doorvoeren (Knop): Schrijft de ingevoerde parameters direct weg naar het EEPROM-geheugen (512 bytes) van de ESP32, zodat deze na herstart bewaard blijven.

### kalibratie
De robot gebruikt kwadratische interpolatie over de 6 IR-sensoren (geplaatst op een onderlinge afstand van 15 mm).De min/max omgevingswaarden van het zwarte en witte oppervlak worden opgeslagen in het geheugen.  
Dit zorgt ervoor dat elke analoge sensormeting wordt omgerekend naar een genormaliseerde waarde van 0 tot 1000, zodat de robot exact de afwijking t.o.v. de lijn in millimeters kan berekenen.

### settings  
Cycle time: 20000 µs (20 ms)   
Power: 100 (bereik 0..255)   
Diff: 0.5   
Kp: 15.0  
Ki: 0.0  
Kd: 0.60  

### start/stop button
Locatie: Fysieke drukknop aangesloten op pin D22 (buttonPin = 22) met een interne pull-up weerstand. 
Een status-LED bevindt zich op pin D21 (ledPin = 21).

Werking:Start/Stop: Eén keer drukken wisselt de status van de robot (start/stop) via een hardware-interrupt met 200 ms software-debounce.

Status-LED indicator:Robot GESTOPT: LED op D21 brandt continu (HIGH).  
Robot RIJDT: LED knippert elke 300 ms om te laten zien dat het algoritme actief is.   
