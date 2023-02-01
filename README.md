# domotic-house
Project for embedded system
## Pinout

<table>
<tr><th colspan=2> ESP32 </th></tr>
<tr><th>Pin left </th><th>Pin right</th></tr>
<tr><td>

| pin  | description
| --- | --- 
|GND | 
|5V  |
|Vext  |
|Vext  |
|Rx  |
|Tx  | 
|Rst  |
|0  |LedKitchen
|22  |SCL
|19 |ButTMinus
|23  | ModuloWiFi
|18  | 
|5  |ButGate
|15  |
|2 |SensProximity
|4    | SensProximity
|17  | ModuloWifi
|16 |ServoMot

</td><td>

| pin  | description
| --- | --- 
|GND |
|3V3|
|3V3|
|36 |
|37 |ButTMode
|38 |ButTOn/Off
|39 |
|34 |ButTPlus
|35 |ButOutdoor	
|32| ButStudio
|33 |ButBedroom
|25 |ButBathroom
|26| ButKitchen
|27 |LedOutdoor
|14 |LedStudio
|12| LedBedroom
|13 |LedBathroom
|21 |SDA

</td></tr> </table>

## Requirements
### Hardware requirements
- Heltec Lora 32 V2
- ESP8266 - 01 wifi module
- TI BOOSTXL-SENSORS BoosterPack (temperature and light sensor are used)
- Ultrasonic sensor HCSR04 
- Continuous rotation servo motor
- LCD alphanumeric 16 x 2 with i2c pilot module
- 10 pushbuttons
- 10 x  5.5 kOhm resistor for pushbuttons
- 7 leds
- 5 breadboards
- Android device
### Software requirements
1. Android Studio IDE
   - Volley library (request from website)
2. Arduino IDE
   - [ESP8266 - 01 library](https://github.com/piedor/ESP8266-01-generic-AT-library) library (pilot ESP8266 with AT commands)
   - Servo.h library
   - Wire.h library (i2c protocol)
   - [LiquidCrystal_I2C.h](https://github.com/johnrickman/LiquidCrystal_I2C) library
   - [Adafruit_TMP006.h](https://github.com/adafruit/Adafruit_TMP006)
