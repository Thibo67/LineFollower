# H-Bridge proof of concept

Met deze test controleren we of de DRV8833 twee motoren onafhankelijk kan aansturen.

De twee motoren kunnen apart:
- Vooruit en achteruit draaien.
- Verschillende snelheden krijgen.
- Tegelijk draaien.

De ESP32 stuurt de DRV8833 aan. De DRV8833 stuurt vervolgens de twee motoren aan.

De snelheid wordt geregeld met PWM van 0 tot 255. Een negatieve waarde laat de motor achteruit draaien.

### Testcommando's

- `M1 150` -> Motor 1 vooruit
- `M1 -150` -> Motor 1 achteruit
- `M2 100` -> Motor 2 vooruit
- `M2 -100` -> Motor 2 achteruit
- `STOP` -> Beide motoren stoppen
### Schema
<img width="816" height="600" alt="{0566AB66-4D2F-42B8-AA5E-0511847F4952}" src="https://github.com/user-attachments/assets/b919c25f-1768-4f7b-a70c-94b44cbfc0e0" />

### Resultaat

De twee motoren kunnen onafhankelijk in snelheid en richting worden geregeld. De H-Bridge werkt dus zoals verwacht.
