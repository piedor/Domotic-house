/*
This program controls the ligth of the bedroom;
look at the value on the database and turn on or off the light
then it changes the value on the same database;
if the button is pressed change the value of the database to on 
if the value was off and viceversa.
*/

#define BUT_BED 33
#define LED_BED 12
bool valueLBed = 0;
String valueLBedDB = "off";  

void IRAM_ATTR changeValueLBed(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300) 
  {
    valueLBed = !valueLBed;
    digitalWrite(LED_BED, valueLBed);
  }
  last_interrupt_time = interrupt_time;
}

void setupLED_bedroom() {    
  pinMode(LED_BED, OUTPUT);     
  pinMode(BUT_BED, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUT_BED), changeValueLBed, FALLING);
}

//the loop controll if the input is to turn the lights on (or off) then it change the value to light_on (or light_off) 
void loopLED_bedroom() {
  valueLBedDB = getLedBedroom();
  if((valueLBedDB == "go_on") || (valueLBedDB == "off" && valueLBed))
  {
    digitalWrite(LED_BED, HIGH);
    valueLBed = 1;
    setLedBedroom("on");
    // delay(300);
  }
  
  if((valueLBedDB == "go_off") || (valueLBedDB == "on" && !valueLBed))
  {
    digitalWrite(LED_BED, LOW);
    valueLBed = 0;
    setLedBedroom("off");
    // delay(300);
  }
}
