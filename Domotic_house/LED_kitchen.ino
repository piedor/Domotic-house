/*
This program controls the ligth of the kitchen;
look at the value on the database and turn on or off the light
then it changes the value on the same database;
if the button is pressed change the value of the database to on 
if the value was off and viceversa.
*/
#define BUT_KITCHEN 26
#define LED_KITCHEN 0  
bool valueLKitchen = 0;
String valueLKitchenDB = "off"; 

void IRAM_ATTR changeValueLKitchen(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300) 
  {
    valueLKitchen = !valueLKitchen;
    digitalWrite(LED_KITCHEN, valueLKitchen);
  }
  last_interrupt_time = interrupt_time;
}

void setupLED_kitchen() {   
  pinMode(LED_KITCHEN, OUTPUT);     
  pinMode(BUT_KITCHEN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUT_KITCHEN), changeValueLKitchen, FALLING);
}

//the loop controll if the input is to turn the lights on (or off) then it change the value to light_on (or light_off) 
void loopLED_kitchen() {
  valueLKitchenDB = getLedKitchen();
  if((valueLKitchenDB == "go_on") || (valueLKitchenDB == "off" && valueLKitchen))
  {
    digitalWrite(LED_KITCHEN, HIGH);
    valueLKitchen = 1;
    setLedKitchen("on");
    // delay(300);
  }
  
  if((valueLKitchenDB == "go_off") || (valueLKitchenDB == "on" && !valueLKitchen))
  {
    digitalWrite(LED_KITCHEN, LOW);
    valueLKitchen = 0;
    setLedKitchen("off");
    // delay(300);
  }
}
