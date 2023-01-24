/*
This program controls the power of an ipotetic thermostat design from 3 leds.
Digitally it takes the desired temperature from the database and activate the
right number of power to reach that temperature (power=leds).
We can control the value of the desired temperature by the button Up/Down
and turn on or off the thermostat from button On/Off
 */

#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include <Adafruit_Sensor.h> // For temperature sensor (tmp006)
#include "Adafruit_TMP006.h"

#define BUT_T_SWITCH 38
#define BUT_T_DO 39
#define BUT_T_UP 34
#define BUT_T_MODE 37
LiquidCrystal_I2C lcd(0x3F,16,2);  //Set dell'indirizzo dell'LCD con 16 caratteri e 2 righe
Adafruit_TMP006 tmp006;

/*SCHEMINO DELL'LCD PER BESTEMMIARE MENO :))
      0 1 2 3 4 5 6 7 8 9 A B C D E F
    0 T E M P : - - . - - X P O W E R
    1 S E T  : X - - . - - X X - - - X
*/

int valueTUp = 0;         // variable for reading the pushbutton status for button up
int valueTDo = 0;         // variable for reading the pushbutton status for button down
int valueTSwitch = 0;         // variable for reading the pushbutton status  for on/f
int valueTMode = 0;

int powerLevel; // (0,1,2,3) define the level power of the thermostat
float valueTempIndoor = 0.0;         //data of the temp from the sens    lcd-value sens
double valueTSetDB = 0.0;          //data of the imposted temp      lcd- value imp
String valueTSwitchDB;            //look if the term is active   lcd- on/off       
String valueTModeDB;     //acquire the mode (warm, cool)

unsigned long lastTSetChange = millis();
bool tSetChange = false;

byte snowflake[8] = {
0b00000,
0b00000,
0b10101,
0b01110,
0b11011,
0b01110,
0b10101,
0b00000,
};

byte fire[8] = {
0b00001,
0b01000,
0b10010,
0b00110,
0b01111,
0b11111,
0b11111,
0b01010,
};

void IRAM_ATTR changeValueTSwitch(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300) 
  {
    valueTSwitch = !valueTSwitch;
  }
  last_interrupt_time = interrupt_time;
}

void IRAM_ATTR changeValueTUp(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300) 
  {
    valueTSetDB += 1;
    lastTSetChange = millis();
    tSetChange = true;
  }
  last_interrupt_time = interrupt_time;
}

void IRAM_ATTR changeValueTDown(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300) 
  {
    valueTSetDB -= 1;
    lastTSetChange = millis();
    tSetChange = true;
  }
  last_interrupt_time = interrupt_time;
}

void IRAM_ATTR changeValueTMode(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300) 
  {
    valueTMode = !valueTMode;
  }
  last_interrupt_time = interrupt_time;
}

//function to define the level of the temperature of the term
void therm_level(){
  if(valueTSetDB > (valueTempIndoor + 5)){
    powerLevel=3;
  }
  if(valueTSetDB > (valueTempIndoor + 2)){
    powerLevel=2;
  }
  if(valueTSetDB >= (valueTempIndoor - 1)){
    powerLevel=1;
  }
  if(valueTSetDB < (valueTempIndoor - 2)){
    powerLevel=0;
  }
}

void lcdShow(){
  if(valueTSwitchDB=="off"){
    lcd.clear();
    lcd.setCursor(0, 0);    //Funzione per decidere dove mettere il cursore (carattere(da 0 a 15), riga(0 o 1))
    lcd.print("TEMP: ");  
    lcd.setCursor(5,0);   //L'idea sarebbe mettere su 6 invece che 5, ma non so come far mostrare un solo numero dopo la virgola, quindi per il momento lascio così
    lcd.print(String(valueTempIndoor));
    delay (1500);
  }else{
    int x;
    lcd.setCursor(0, 0);
    lcd.print("TEMP:      POWER");
    lcd.setCursor(0, 1);
    lcd.print("SET :          ");
    if (valueTModeDB.equals("warm")) x = 1;
    else x = 2;
    switch (powerLevel){
      case (0):{  lcd.setCursor(12, 1);
                  lcd.print("   ");}
      case (1):{  lcd.setCursor(12, 1);
                  lcd.write((byte)x);
                  lcd.print("  ");}
      case (2):{  lcd.write((byte)x);
                  lcd.write((byte)x);
                  lcd.print(" ");}
      case (3):{  lcd.write((byte)x);
                  lcd.write((byte)x);
                  lcd.write((byte)x);
                  }
      default: break;
    }
  }
}

float getTempIndoor(){
  return(tmp006.readDieTempC());
}

void setupRegulator_therm() {  
  lcd.init();    //init the lcd
  lcd.clear();   //clear the lcd screen
  lcd.backlight();
  lcd.createChar(1, fire);
  lcd.createChar(2, snowflake);
  tmp006.begin(); // init temperature sensor
  
  pinMode(BUT_T_SWITCH, INPUT_PULLUP);     // initialize the pushbutton pin as an input for the On/Off command
  pinMode(BUT_T_DO, INPUT_PULLUP);      // initialize the pushbutton pin as an input for the turn down command
  pinMode(BUT_T_UP, INPUT_PULLUP);      // initialize the pushbutton pin as an input for the turn up command
  pinMode(BUT_T_MODE, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUT_T_SWITCH), changeValueTSwitch, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUT_T_UP), changeValueTUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUT_T_DO), changeValueTDown, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUT_T_MODE), changeValueTMode, FALLING);
}

//the loop controll if the input is to turn the lights on (or off) then it change the value to light_on (or light_off) 
void loopRegulator_therm() {          // read the value of of the therm status
  if(tSetChange && (millis() - lastTSetChange > 3000)){
    setThermSet(valueTSetDB); 
    tSetChange = false;
  }
  else if(!tSetChange){
    valueTSetDB = getThermSet();      //acquire the data from the database   
  }
  valueTSwitchDB = getThermSwitch();
  valueTempIndoor = getTempIndoor();
  
  //when the button is pressed and the therm is off or the command from the app is to turn on the therm
  if((valueTSwitch && valueTSwitchDB == "off") || (valueTSwitchDB == "go_on")){
    valueTSwitch = 1;
    setThermSwitch("on");
    //delay(300);
  }
  if((!valueTSwitch && valueTSwitchDB == "on") || (valueTSwitchDB == "go_off"))
  {
    valueTSwitch = 0;
    setThermSwitch("off");
    //delay(300);
  }
  if(valueTMode && valueTModeDB == "warm")
  {
    valueTMode = 0;
    setThermMode("cool");
    //delay(300);
  }
  if(valueTMode && valueTModeDB == "cool")
  {
    valueTMode = 0;
    setThermMode("warm");
    //delay(300);
  }

  therm_level();
  lcdShow();
 
// delay(1000);
}
