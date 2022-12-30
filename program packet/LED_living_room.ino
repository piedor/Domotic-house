/*
This program controls the ligth of the livingroom;
look at the value on the database and turn on or off the light
then it changes the value on the same database;
if the button is pressed change the value of the database to on 
if the value was off and viceversa.
*/
#undef but_O_F
#define but_O_F
#undef LED_livingr
#define LED_livingr 38  //port 2.4

const int buttonPin = but_O_F;     // the number of the pushbutton pin
const int ledPin =  LED_livingr;      // the number of the LED pin
int butState = 0;         // variable for reading the pushbutton status

String value_LL;

void setupLED_livingroom() {
  Serial.begin(9600);       
  pinMode(ledPin, OUTPUT);     
  pinMode(buttonPin, INPUT_PULLUP);
}
//the loop controll if the input is to turn the lights on (or off) then it change the value to light_on (or light_off) 
void loopLED_livingroom() {
  butState=digitalWrite(but_O_F);
//  value_LL=getValue();  
if((value_LL== "go_on")||(butState==LOW && value_LL=="off"))
  {
    value_LL="on";
    butState=HIGH;
    delay(300);
  }
  
if((value_LL == "go_off")||(butState==LOW && value_LL=="on"))
  {
    value_LL="off";
    butState=HIGH;
    delay(300);
  }
  
if (value_LL=="on") digitalWrite(ledPin,HIGH);

if (value_LL=="off") digitalWrite(ledPin,LOW);
  
  Serial1.println(value_LL);
}
