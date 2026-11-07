# H-Bridge proof of concept

Met deze test controleren we of de DRV8833 twee motoren onafhankelijk kan aansturen.

De twee motoren kunnen apart:
- Vooruit en achteruit draaien.
- Verschillende snelheden krijgen.
- Tegelijk draaien.

De ESP32 stuurt de DRV8833 aan. De DRV8833 stuurt vervolgens de twee motoren aan.

### Schema
<img width="993" height="647" alt="image" src="https://github.com/user-attachments/assets/f5eef5f7-f40f-492c-b2df-03af35a226af" />

### Resultaat

De twee motoren kunnen onafhankelijk in snelheid en richting worden geregeld. De H-Bridge werkt dus zoals verwacht.
