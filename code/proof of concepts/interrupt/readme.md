# start/stop interrupt proof of concept
Deze PoC verifieert de werking van de gebruikersinterface via een hardware-interrupt:
- Het systeem reageert via een interrupt op de drukknop (SW1) op GPIO 22.
- Een actieve status (cyclus) wisselt automatisch elke seconde tussen Rood, Groen en Blauw via GPIO 21, 5 en 4.
- Een onderbreking via de drukknop pauzeert de cyclus direct en schakelt de RGB LED naar continu Wit. Zodra opnieuw wordt gedrukt, hervat de cyclus.
<img width="733" height="493" alt="{7C2F54C1-5498-4CE3-A96A-77286B29D78E}" src="https://github.com/user-attachments/assets/23667f07-bb41-4659-a346-6a9b7e30bbfa" />

# Stappenplan
- Sluit de drukknop en de RGB LED correct aan volgens bovenstaand schema (met 1kΩ pull-down op D22 en 220Ω op de LED-anodes).
- Verbind de ESP32 microcontroller via USB met de pc.
- Verify/Upload de code naar de microcontroller via de Arduino IDE.
- Open de Seriële Monitor (Ctrl + Shift + M) op 115200 baud.

