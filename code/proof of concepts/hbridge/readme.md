# H-Bridge proof of concept

Met deze test controleren we of de TB6612FNG twee motoren onafhankelijk kan aansturen.

De twee motoren kunnen apart:
- Vooruit en achteruit draaien.
- Verschillende snelheden krijgen.
- Tegelijk draaien.

De ESP32 stuurt de TB6612FNG aan. De TB6612FNG stuurt vervolgens de twee motoren aan.

De snelheid wordt geregeld met PWM van 0 tot 255. Een negatieve waarde laat de motor achteruit draaien.

### Testcommando's

`M1 150` → Motor 1 vooruit  
`M1 -150` → Motor 1 achteruit  
`M2 100` → Motor 2 vooruit  
`M2 -100` → Motor 2 achteruit  
`STOP` → Beide motoren stoppen

### Resultaat

De twee motoren kunnen onafhankelijk in snelheid en richting worden geregeld. De H-Bridge werkt dus zoals verwacht.
