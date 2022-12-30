/*
This program controls the power of an ipotetic thermostat design from 3 leds.
Digitally it takes the desired temperature from the database and activate the
right number of power to reach that temperature (power=leds).
We can control the value of the desired temperature by the button Up/Down
and turn on or off the thermostat from button On/Off
*/
#undef but_O_F
#define but_O_F
#undef but_Do
#define but_Do
#undef but_Up
#define but_Up
#undef LED_therm1
#define LED_therm1
#undef LED_therm2
#define LED_therm2
#undef LED_therm3
#define LED_therm3
#undef SENS_themp
#define SENS_themp


const int butUpPin = but_Up;     // the number of the pushbutton pin
const int butDoPin = but_Do;     // the number of the pushbutton pin
const int butOFPin = but_O_F;     // the number of the pushbutton pin
const int led1Pin =  LED_therm1;      // the number of the LED pin
const int led2Pin =  LED_therm2;      // the number of the LED pin
const int led3Pin =  LED_therm3;      // the number of the LED pin

int but_Up_State = 0;         // variable for reading the pushbutton status for button up
int but_Do_State = 0;         // variable for reading the pushbutton status for button down
int but_O_F_State = 0;         // variable for reading the pushbutton status  for on/f

int valuesens_TEMP;         //data of the temp from the sens 
int valueimp_TEMP;          //data of the imposted temp      
String therm_acc;            //look if the term is active       


//function to define the level of the temperature of the term
void therm_level(){
  //valuesens_TEMP=SENS_temp.readvalue();        //acquire the temp from the sens
  if( valueimp_TEMP>(valuesens_TEMP+5) ){
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
    digitalWrite(led3Pin, HIGH);    
  }
  if( valueimp_TEMP>(valuesens_TEMP+2) ){
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
    digitalWrite(led3Pin, LOW);   
  }
  if( valueimp_TEMP>=(valuesens_TEMP-1) ){
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, LOW); }
  if( valueimp_TEMP<(valuesens_TEMP-1) ){
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, LOW);         
  }
}

void change_value(int t){
  valueimp_TEMP=valueimp_TEMP+t;                  // acquire the data from the database
  Serial1.println(valueimp_TEMP);                  // change the value to the new one
}

void setupRegulator_therm() {
  valueimp_TEMP=Serial1.read();                  //acquire the data from the database     
  therm_acc=Serial1.read();
  pinMode(LED_therm1, OUTPUT);         //initialize the led1 as output for low power
  pinMode(LED_therm2, OUTPUT);         //initialize the led1 as output for medium power
  pinMode(LED_therm3, OUTPUT);         //initialize the led1 as output for high power
  pinMode(butOFPin, INPUT_PULLUP);     // initialize the pushbutton pin as an input for the On/Off command
  pinMode(butDoPin, INPUT_PULLUP);      // initialize the pushbutton pin as an input for the turn down command
  pinMode(butUpPin, INPUT_PULLUP);      // initialize the pushbutton pin as an input for the turn up command
}

//the loop controll if the input is to turn the lights on (or off) then it change the value to light_on (or light_off) 
void loopRegulator_therm() {          // read the value of of the therm status
valuesens_TEMP=acquiredata();
but_O_F_State= digitalRead(butOFPin);  // read the state of the pushbutton value o/F
but_Do_State= digitalRead(butDoPin);  // read the state of the pushbutton value down
but_Up_State= digitalRead(butUpPin);  // read the state of the pushbutton value up

//when the button is pressed and the therm is off or the command from the app is to turn on the therm
if((but_O_F_State==LOW && therm_acc=="off") || (therm_acc=="go_on")){
    therm_acc="on";
    but_O_F_State=HIGH;
    delay(300);
    }
if((but_O_F_State==LOW && therm_acc=="on") || (therm_acc=="go_off"))
  {
    therm_acc="off";
    but_O_F_State=HIGH;
    delay(300);
  }
Serial1.println();
but_Up_State= digitalRead(but_Up);  // read the state of the button_Up value 
but_Do_State= digitalRead(but_Do);  // read the state of the button_Do value 


if(but_Up_State==LOW) {
  change_value(1);
  but_Up_State=HIGH;
  delay(300);
}

if(but_Do_State==LOW) {
  change_value(-1);
  but_Do_State=HIGH;
  delay(300);
}

if(therm_acc == "on"){
    therm_value();           //call the regulator function
  }
  
if(therm_acc == "off"){   //shutdown the sistyem
    digitalWrite(LED_therm1, LOW);
    digitalWrite(LED_therm2, LOW);
    digitalWrite(LED_therm3, LOW);       
  }
  
delay(1000);
}
