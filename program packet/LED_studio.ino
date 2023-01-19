/*
This program controls the ligth of the studio;
look at the value on the database and turn on or off the light
then it changes the value on the same database;
if the button is pressed change the value of the database to on 
if the value was off and viceversa.
*/

#define but_STUDIO 7  // the number of the pushbutton pin
#define LED_studio 4  // the number of the LED pin

//int buttonPin = but_STUDIO;     // the number of the pushbutton pin
int ledPin =  LED_studio;      // the number of the LED pin
int butState = 0;         // variable for reading the pushbutton status
String value_LS;

void setupLED_studio() {       
  pinMode(ledPin, OUTPUT);     
  pinMode(buttonPin, INPUT_PULLUP);
}
//the loop controll if the input is to turn the lights on (or off) then it change the value to light_on (or light_off) 
void loopLED_studio() {
  butState=digitalRead(but_STUDIO);
//  value_LS=getValue();  
if((value_LS== "go_on")||(butState==LOW && value_LS=="off"))
  {
    value_LS="on";
    butState=HIGH;
    delay(300);
  }
  
if((value_LS == "go_off")||(butState==LOW && value_LS=="on"))
  {
    value_LS="off";
    butState=HIGH;
    delay(300);
  }
  
if (value_LS=="on") digitalWrite(ledPin, HIGH);

if (value_LS=="off") digitalWrite(ledPin, LOW);
  
  Serial1.println(value_LL);
}
