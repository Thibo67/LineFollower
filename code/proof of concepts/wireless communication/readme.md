# draadloze communicatie proof of concept
Deze PoC verifieert de werking van volgende zaken:

- er kan duplex gecommuniceerd worden tussen de microcontroller en een laptop
- gebruikmaken van PuTTy
Bij een correcte werking zal de seriële monitor berichten ontvangen van PuTTY, en zal PuTTy berichten ontvangen van de seriële monitor.
<img width="603" height="1311" alt="IMG_9249" src="https://github.com/user-attachments/assets/4b7991c4-eb4b-4271-8cc0-c79fb1ce3767" />


# Stappenplan
- Verbind de microcontroller met de pc
- Verify/Upload de code naar de mictrocontroller
- Controleer of de bibliotheek BluetoothSerial.h is geïnstalleerd
- Verbind met "ESP32_LineFollower" via bluetooth op laptop
- Kijk in device manager welke COM word gebruikt door de bluetooth verbinding
- Connecteer via PuTTY met deze seriële COM en stel de correcte baudrate in
- Verifieer de werking door te vergelijken met bovenstaande foto, zenden ontvangen in beide richtingen werken






