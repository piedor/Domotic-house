/*
This program controls the ligth of the kitchen;
look at the value on the database and turn on or off the light
then it changes the value on the same database;
if the button is pressed change the value of the database to on 
if the value was off and viceversa.
*/
#define but_KITCHEN 5
#define LED_kitchen 2  

//int buttonPin = but_KITCHEN;     // the number of the pushbutton pin
  int ledPin =  LED_kitchen;      // the number of the LED pin
int butState = 0;         // variable for reading the pushbutton status

String value_LK;    


void setupLED_kitchen() {
  Serial.begin(9600);       
  pinMode(ledPin, OUTPUT);     
  pinMode(buttonPin, INPUT_PULLUP);
}
//the loop controll if the input is to turn the lights on (or off) then it change the value to light_on (or light_off) 
void loopLED_kitchen() {
  butState=digitalRead(but_KITCHEN);
//  value_LK=getValue();  
if((value_LK == "go_on")||(butState==LOW && value_LK=="off"))
  {
    value_LK="on";
    butState=HIGH;
    delay(300);
  }
  
if((value_LK == "go_off")||(butState==LOW && value_LK=="on"))
  {
    value_LK="off";
    butState=HIGH;
    delay(300);
  }
  
if (value_LK=="on") digitalWrite(ledPin,HIGH);

if (value_LK=="off") digitalWrite(ledPin,LOW);
  
  Serial1.println(value_LK);
}
