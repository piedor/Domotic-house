/*
This program controls the ligth of the studio;
look at the value on the database and turn on or off the light
then it changes the value on the same database;
if the button is pressed change the value of the database to on 
if the value was off and viceversa.
*/

#define BUT_STUDIO 32  // the number of the pushbutton pin
#define LED_STUDIO 14  // the number of the LED pin
bool valueLStudio = 0;
String valueLStudioDB = "off"; 

void IRAM_ATTR changeValueLStudio(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300) 
  {
    valueLStudio = !valueLStudio;
    digitalWrite(LED_STUDIO, valueLStudio);
  }
  last_interrupt_time = interrupt_time;
}

void setupLED_studio() {       
  pinMode(LED_STUDIO, OUTPUT);     
  pinMode(BUT_STUDIO, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUT_STUDIO), changeValueLStudio, FALLING);
}
//the loop controll if the input is to turn the lights on (or off) then it change the value to light_on (or light_off) 
void loopLED_studio() {
  valueLStudioDB = getLedStudio();
  if((valueLStudioDB == "go_on") || (valueLStudioDB == "off" && valueLStudio))
  {
    digitalWrite(LED_STUDIO, HIGH);
    valueLStudio = 1;
    setLedStudio("on");
    // delay(300);
  }
  
  if((valueLStudioDB == "go_off") || (valueLStudioDB == "on" && !valueLStudio))
  {
    digitalWrite(LED_STUDIO, LOW);
    valueLStudio = 0;
    setLedStudio("off");
    // delay(300);
  }
}
