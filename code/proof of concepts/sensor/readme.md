# Sensoren proof of concept

Deze PoC verifieert de werking van volgende zaken:

- Minimaal 6 sensoren kunnen onafhankelijk van elkaar worden uitgelezen (geen calibratie, normalisatie of interpolatie).
- Hierbij wordt een zo groot mogelijk bereik van de AD converter benut (0 tot 4095 op de ESP32 door het instellen van de juiste attenuatie).
- Bij een correcte werking zal de sensor een hoge waarde geven bij lichte kleuren (max 4095) en een lage waarde bij donkere kleuren.

<img width="757" height="513" alt="{A4496DDC-DFF1-4712-8406-C0DA797E6595}" src="https://github.com/user-attachments/assets/55fa3153-e828-4a8b-aa1b-629b26d04bbc" />

Stappenplan
- Sluit de componenten correct aan volgens bovenstaande schema.
- Verbind de microcontroller met de pc via een datakabel.
- Verify/Upload de code naar de microcontroller via de Arduino IDE.
- Open de Seriële Monitor (Ctrl + Shift + M) op 115200 baud.
- Verifieer de werking door te vergelijken met bovenstaande video (hoge waarden bij lichte oppervlakken, lage waarden bij donkere/zwarte oppervlakken).
