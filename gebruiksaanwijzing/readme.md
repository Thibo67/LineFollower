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
uitleg over het verbinden van de robot met laptop / smartphone

#### commando's
debug [on/off]  
start  
stop  
set cycle [µs]  
set power [0..255]  
set diff [0..1]  
set kp [0..]  
set ki [0..]  
set kd [0..]  
calibrate black  
calibrate white  

### kalibratie
uitleg kalibratie  

### settings
De robot rijdt stabiel met volgende parameters:  

### start/stop button
uitleg locatie + werking start/stop button
