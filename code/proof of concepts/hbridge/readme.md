# H-Bridge proof of concept

## Doel

Deze Proof of Concept (PoC) toont aan dat twee DC-motoren onafhankelijk van elkaar kunnen worden aangestuurd met een ESP32 en een TB6612FNG motor driver.

De volgende functionaliteiten worden aangetoond:

- Motor 1 en motor 2 onafhankelijk aansturen
- Beide motoren vooruit laten draaien
- Beide motoren achteruit laten draaien
- De snelheid van iedere motor afzonderlijk regelen
- Beide motoren tegelijkertijd met verschillende snelheden laten draaien
- De draairichting van iedere motor afzonderlijk veranderen

## Hardware

- ESP32 WROOM 4MB DevKit V1
- TB6612FNG Dual H-Bridge Motor Driver
- 2x DC-motor
- 5-6 V motorvoeding
- USB-C kabel
- Jumper wires

## Werking

De TB6612FNG bevat twee onafhankelijke H-bruggen.

De eerste H-brug stuurt motor 1 aan en de tweede H-brug stuurt motor 2 aan.

De ESP32 bepaalt:

- De draairichting via IN1 en IN2
- De snelheid via PWM
- Of de H-brug actief is via STBY

De snelheid wordt ingesteld met een waarde van -255 tot +255.

| Waarde | Betekenis |
|---|---|
| -255 | Volledig achteruit |
| -100 | Achteruit |
| 0 | Stop |
| +100 | Vooruit |
| +255 | Volledig vooruit |

## Aansluitingen

### Motor 1

| ESP32 | TB6612FNG |
|---|---|
| GPIO 25 | PWMA |
| GPIO 27 | AIN1 |
| GPIO 14 | AIN2 |

Motor 1 wordt aangesloten op:

```text
AO1 - Motor 1 - AO2
