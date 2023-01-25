/*
This program controls the ligth of the hall;
The manual command has priority on the light sensor;
if the light sensor is on it controls the illumination.
*/
#include <Wire.h>

#define BUT_OUTDOOR 35
#define LED_OUTDOOR 27

bool valueLOutdoor = 0;
String valueLOutdoorDB = "off"; 
bool light=false; //to clear the project 0=light off and 1=light on
float lightLevel; // lux from light sensor

void IRAM_ATTR changeValueLOutdoor(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300) 
  {
    valueLOutdoor = !valueLOutdoor;
    digitalWrite(LED_OUTDOOR, valueLOutdoor);
  }
  last_interrupt_time = interrupt_time;
}

float getLuxSensor(){
  float fLux = 0.0;
  Wire.beginTransmission(0x44);
  Wire.write(0x00);                              // Send result register address
  Wire.endTransmission();
  delay(100);
  
  Wire.requestFrom(0x44, 2);                     // Request 2 bytes data from OPT3001
  uint16_t iData;
  uint8_t  iBuff[2];
  if(Wire.available()) 
  { 
    Wire.readBytes(iBuff, 2);
    iData = (iBuff[0] << 8) | iBuff[1];
    fLux = SensorOpt3001_convert(iData);   // Calculate LUX from sensor data
  }
  return(fLux);
}

float SensorOpt3001_convert(uint16_t iRawData)
{
  uint16_t iExponent, iMantissa;
  iMantissa = iRawData & 0x0FFF;                 // Extract Mantissa
  iExponent = (iRawData & 0xF000) >> 12;         // Extract Exponent 
  return iMantissa * (0.01 * pow(2, iExponent)); // Calculate final LUX
}

void setupLED_outdoor() {
  // begin opt3001                
  Wire.begin();                                  // Initialize Arduino in I2C master.
  Wire.beginTransmission(0x44);                  // I2C address of OPT3001 = 0x44
  Wire.write(0x01);                              // Config register address 0x01
  Wire.write(0xCE);
  Wire.write(0x10);                              // Write 0xCE10 to turn on sensor
  Wire.endTransmission();
        
  pinMode(LED_OUTDOOR, OUTPUT);     
  pinMode(BUT_OUTDOOR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUT_OUTDOOR), changeValueLOutdoor, FALLING);
}
//the loop controll if the outdoor light sensor is active or not then it change the value to light_on (or light_off) 
void loopLED_outdoor() {
  valueLOutdoorDB = getLedOutdoor(); //acquire the data from the database 
  lightLevel = getLuxSensor();  
  
  if((!valueLOutdoor && valueLOutdoorDB == "sens_on") || (!valueLOutdoor && valueLOutdoorDB == "on")){
    light=false;
    valueLOutdoor = 0;
    setLedOutdoor("off");
    //delay(500);
  }
  
  if((valueLOutdoor && valueLOutdoorDB == "sens_off") || (valueLOutdoor && valueLOutdoorDB == "off")){
    light=true;
    valueLOutdoor = 1;
    setLedOutdoor("on");
    //delay(500);
  }
  
  if (valueLOutdoorDB == "sens_on")
  { 
    if(lightLevel<=200)
    { 
      light=true;
     }
  }else if(valueLOutdoorDB=="on") {
      light=true;
    }else {
      light=false;
    }
  
  /*
  matching reset, when the sens output matchs the leds it reset 
  example: if its dark but I decided to close the leds manually from the button
      when the sensor's output should close the leds it reset the value_LO 
      from off to sens_off; the same if I turned up the leds when there was light,
      when the sensor output should turn up the leds it reset to sens_on
  */
  if(light) digitalWrite(LED_OUTDOOR, HIGH);
  else digitalWrite(LED_OUTDOOR, LOW);
  if(valueLOutdoorDB == "on" && lightLevel <= 200) setLedOutdoor("sens_on");
  if(valueLOutdoorDB == "off" && lightLevel > 200) setLedOutdoor("sens_off");
  //delay(1000);  
}
