/*
This program controls the ligth of the bathroom;
look at the value on the database and turn on or off the light
then it changes the value on the same database;
if the button is pressed change the value of the database to on 
if the value was off and viceversa.
*/

#define BUT_BATH 25     // the number of the pushbutton pin
#define LED_BATH 13     // the number of the LED pin

bool valueLBath = 0;
String valueLBathDB = "off";    

void IRAM_ATTR changeValueLBath(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300){
    valueLBath = !valueLBath;
    digitalWrite(LED_BATH, valueLBath);
    if((valueLBathDB.equals("go_on")) || (valueLBathDB.equals("off") && valueLBath)){
      setLedBathroom("on");
    }
    if((valueLBathDB.equals("go_off")) || (valueLBathDB.equals("on") && !valueLBath)){
      setLedBathroom("off");
    }
  }
  last_interrupt_time = interrupt_time;
}

void setupLED_bathroom() {     
  pinMode(LED_BATH, OUTPUT);     
  pinMode(BUT_BATH, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUT_BATH), changeValueLBath, FALLING);
}

// The loop controll if the input is to turn the lights on (or off) then it change the value to light_on (or light_off) 
void loopLED_bathroom() {
  valueLBathDB = getLedBathroom();
  if(valueLBathDB.equals("go_on") || valueLBathDB.equals("go_off")){
    changeValueLBath();
  }
}
