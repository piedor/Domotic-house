/*
This program controls the ligth of the hall;
The manual command has priority on the light sensor;
if the light sensor is on it controls the illumination.
*/
#include "OPT3001.h"
#undef but_O_F
#define but_O_F
#undef LED_outdoor
#define LED_outdoor 39    //port 2.6
#undef SENSOR_light
#define SENSOR_light sensor_portxx

const int buttonPin = but_O_F;     // the number of the pushbutton pin
const int ledPin =  LED_outdoor;      // the number of the LED pin
int butState = 0;         // variable for reading the pushbutton status

opt3001 opt3001;
String value_LO; 
int light; //to clear the project 0=light off and 1=light on
int32_t lightLevel;

void setupLED_outdoor() {      
  Serial.begin(9600); // initialize serial communication           
  opt3001.begin(); 
        
  pinMode(ledPin, OUTPUT);     
  pinMode(buttonPin, INPUT_PULLUP);
  light=0;
  value_LO="sens_on";
  
}
//the loop controll if the outdoor light sensor is active or not then it change the value to light_on (or light_off) 
void loopLED_outdoor() {
  butState= digitalRead(buttonPin);  // read the state of the pushbutton value
  //value_LO=getValue();  //acquire the data from the database 
  
  if((butState==LOW && value_LO=="sens_on")||(butState==LOW && value_LO=="on")){
    value_LO="off";
    light=0;
    butState=HIGH;
    delay(500);
    Serial.println("Button: turn off");
  }
  
  if((butState==LOW && value_LO=="sens_off")||(butState==LOW && value_LO=="off")){
    value_LO="on";
    light=1;
    butState=HIGH;
    delay(500);
    Serial.println("Button: turn on");
  }
  
    
  if (value_LO == "sens_on")
  {
    lightLevel = opt3001.readResult();  
    Serial.print("LUX Readings = ");
    Serial.println(lightLevel);
    
      if(lightLevel<=200)
      { 
        light=1; 
        Serial.println("Sensor: turn on");
      }
      else
      {
        light=0;
        
        Serial.println("Sensor: turn off");
      }
  }
    
  //open (or close) the light:
  if(light== 1) digitalWrite(ledPin, HIGH);
    else digitalWrite(ledPin, LOW);

  /*
  matching reset, when the sens output matchs the leds it reset 
  example: if its dark but I decided to close the leds manually from the button
      when the sensor's output should close the leds it reset the value_LO 
      from off to sens_off; the same if I turned up the leds when there was light,
      when the sensor output should turn up the leds it reset to sens_on
  */
  if(value_LO=="on" && lightLevel<=200) value_LO="sens_on";
  if(value_LO=="off" && lightLevel>200) value_LO="sens_off";
  delay(1000);
  
}
