/*
This program controls the ligth of the bedroom;
look at the value on the database and turn on or off the light
then it changes the value on the same database;
if the button is pressed change the value of the database to on 
if the value was off and viceversa.
*/

#define but_BED 26
#define LED_bedroom 23

//int buttonPin = but_BED;     // the number of the pushbutton pin
int ledPin =  LED_bedroom;      // the number of the LED pin
int butState = 0;         // variable for reading the pushbutton status

String value_LBe;    


void setupLED_bedroom() {
  Serial.begin(9600);       
  pinMode(ledPin, OUTPUT);     
  pinMode(buttonPin, INPUT_PULLUP);
}
//the loop controll if the input is to turn the lights on (or off) then it change the value to light_on (or light_off) 
void loopLED_bedroom() {
  butState=digitalRead(but_BED);
//  value_LBe=getValue();  
if((value_LBe== "go_on")||(butState==LOW && value_LBe=="off"))
  {
    value_LBe="on";
    butState=HIGH;
    delay(300);
  }
  
if((value_LBe == "go_off")||(butState==LOW && value_LBe=="on"))
  {
    value_LBe="off";
    butState=HIGH;
    delay(300);
  }
  
if (value_LBe=="on") digitalWrite(ledPin,HIGH);

if (value_LBe=="off") digitalWrite(ledPin,LOW);
  
  Serial1.println(value_LBe);
}