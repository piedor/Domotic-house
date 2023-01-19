/*
This program controls the ligth of the bathroom;
look at the value on the database and turn on or off the light
then it changes the value on the same database;
if the button is pressed change the value of the database to on 
if the value was off and viceversa.
*/
#define but_BATH 25
#define LED_bathroom 3 

int buttonPin = but_BATH;     // the number of the pushbutton pin
int ledPin =  LED_bathroom;      // the number of the LED pin
int butState = 0;         // variable for reading the pushbutton status

String value_LBa;    


void setupLED_bathroom() {
  Serial.begin(9600);       
  pinMode(ledPin, OUTPUT);     
  pinMode(buttonPin, INPUT_PULLUP);
}
//the loop controll if the input is to turn the lights on (or off) then it change the value to light_on (or light_off) 
void loopLED_bathroom() {
  butState=digitalRead(but_BATH);
//  value_LBa=getLBaValue();  
if((value_LBa== "go_on")||(butState==LOW && value_LBa=="off"))
  {
    value_LBa="on";
    butState=HIGH;
    delay(300);
  }
  
if((value_LBa == "go_off")||(butState==LOW && value_LBa=="on"))
  {
    value_LBa="off";
    butState=HIGH;
    delay(300);
  }
  
if (value_LBa=="on") digitalWrite(ledPin,HIGH);

if (value_LBa=="off") digitalWrite(ledPin,LOW);
  
  Serial1.println(value_LBa);
}