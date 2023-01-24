/*
This program controls the ligth of the hall;
The manual command has priority on the light sensor;
if the light sensor is on it controls the illumination.
*/
#include <Wire.h>

#define BUT_OUTDOOR 35
#define LED_OUTDOOR 27
opt3001 opt3001;

bool valueLOutdoor = 0;
String valueLOutdoorDB = "off"; 
int light; //to clear the project 0=light off and 1=light on
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
    Serial.println(fLux);                        // Print it on serial terminal
  }
  return(fLux);
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
  light=0;
  value_LO="sens_on";
  
}
//the loop controll if the outdoor light sensor is active or not then it change the value to light_on (or light_off) 
void loopLED_outdoor() {
  valueLOutdoorDB = getLedOutdoor(); //acquire the data from the database 
  lightLevel = getLuxSensor();  
  
  if((!valueLStudio && valueLStudioDB == "sens_on") || (!valueLStudio && valueLStudioDB == "on")){
    digitalWrite(LED_OUTDOOR, LOW);
    valueLOutdoor = 0;
    setLedOutdoor("off");
    //delay(500);
  }
  
  if((valueLStudio && valueLStudioDB == "sens_off") || (valueLStudio && valueLStudioDB == "off")){
    digitalWrite(LED_OUTDOOR, HIGH);
    valueLOutdoor = 1;
    setLedOutdoor("on");
    //delay(500);
  }
  
    
  if (valueLStudioDB == "sens_on")
  { 
    if(lightLevel<=200)
    { 
      digitalWrite(LED_OUTDOOR, HIGH);
    }
    else
    {
      digitalWrite(LED_OUTDOOR, LOW);
    }
  }
  /*
  matching reset, when the sens output matchs the leds it reset 
  example: if its dark but I decided to close the leds manually from the button
      when the sensor's output should close the leds it reset the value_LO 
      from off to sens_off; the same if I turned up the leds when there was light,
      when the sensor output should turn up the leds it reset to sens_on
  */
  if(valueLStudioDB == "on" && lightLevel <= 200) setLedOutdoor("sens_on");
  if(value_LO == "off" && lightLevel > 200) setLedOutdoor("sens_off");
  //delay(1000);  
}
