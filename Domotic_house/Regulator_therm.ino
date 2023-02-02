/*
This program controls the power of an ipotetic thermostat design from 3 leds.
Digitally it takes the desired temperature from the database and activate the
right number of power to reach that temperature (power=leds).
We can control the value of the desired temperature by the button Up/Down
and turn on or off the thermostat from button On/Off*/

#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include <Adafruit_Sensor.h> // For temperature sensor (tmp006)
#include "Adafruit_TMP006.h"

#define BUT_T_SWITCH 38
#define BUT_T_DO 19
#define BUT_T_UP 34
#define BUT_T_MODE 37
LiquidCrystal_I2C lcd(0x3F,16,2);  //Set dell'indirizzo dell'LCD con 16 caratteri e 2 righe
Adafruit_TMP006 tmp006;

/*  Lcd scheme sample
      0 1 2 3 4 5 6 7 8 9 A B C D E F
    0 T E M P : - - . - - X P O W E R
    1 S E T   : - - . - - X X - - - X
*/

int powerLevel; // (0,1,2,3) define the level power of the thermostat
float valueTempIndoor = 18.0;         //data of the temp from the sens    lcd-value sens
double valueTSetDB = 18.0;          //data of the imposted temp      lcd- value imp
String valueTSwitchDB;            //look if the term is active   lcd- on/off       
String valueTModeDB;     //acquire the mode (warm, cool)

unsigned long lastTSetChange = millis();
bool tSetChange = false;

//snoflake byte image 
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

//fire byte image 
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

//interrupt function to switch the status
void IRAM_ATTR changeValueTSwitch(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300){
    if((valueTSwitchDB.equals("off")) || (valueTSwitchDB.equals("go_on"))){
      setThermSwitch("on");
    }
    if((valueTSwitchDB.equals("on")) || (valueTSwitchDB.equals("go_off"))){
      setThermSwitch("off");
    }
  }
  last_interrupt_time = interrupt_time;
}

//interrupt function to change up the Tset
void IRAM_ATTR changeValueTUp(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300){
    valueTSetDB += 0.5;
    lastTSetChange = millis();
    tSetChange = true;
  }
  last_interrupt_time = interrupt_time;
}

//interrupt function to change down the Tset
void IRAM_ATTR changeValueTDown(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300){
    valueTSetDB -= 0.5;
    lastTSetChange = millis();
    tSetChange = true;
  }
  last_interrupt_time = interrupt_time;
}

//interrupt function to change Tmode
void IRAM_ATTR changeValueTMode(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300){
    if(valueTModeDB.equals("warm")){
      setThermMode("cool");
    }
    if(valueTModeDB.equals("cool")){
      setThermMode("warm");
    }
    
  }
  last_interrupt_time = interrupt_time;
}

//function to define the power level of the term
void therm_level(){
  if(valueTModeDB.equals("warm")){
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
  }else{
    if(valueTSetDB > (valueTempIndoor - 1)){
      powerLevel=1;
    }
    if(valueTSetDB > (valueTempIndoor + 2)){
      powerLevel=2;
    }
    if(valueTSetDB >= (valueTempIndoor + 5)){
      powerLevel=3;
    }
    if(valueTSetDB < (valueTempIndoor - 2)){
      powerLevel=0;
    }
  }
}

//function to set the lcd display layout
void lcdShow(){
  if(valueTSwitchDB.equals("on")){
    int x;
    lcd.setCursor(0, 0);
    lcd.print("TEMP:      POWER");
    lcd.setCursor(5,0);   //L'idea sarebbe mettere su 6 invece che 5, ma non so come far mostrare un solo numero dopo la virgola, quindi per il momento lascio così
    lcd.print(String(valueTempIndoor));
    lcd.setCursor(0, 1);
    lcd.print("SET :           ");
    lcd.setCursor(5,1);
    lcd.print(String(valueTSetDB));

    if (valueTModeDB.equals("warm")) x = 1;
    else x = 2;
    lcd.setCursor(12, 1);
    switch (powerLevel){
      case (0):{  if(x==1) {
                      lcd.print(" W  ");
                    }else{
                      lcd.print(" C  ");
                    }
      }
      case (1):{  lcd.write(x);
                  lcd.print("   ");}
      case (2):{  lcd.write(x);
                  lcd.write(x);
                  lcd.print(" ");}
      case (3):{  lcd.write(x);
                  lcd.write(x);
                  lcd.write(x);
                  }
      default: break;
    }
  }else{
    lcd.clear();
    lcd.setCursor(0, 0);    //Funzione per decidere dove mettere il cursore (carattere(da 0 a 15), riga(0 o 1))
    lcd.print("TEMP: ");  
    lcd.setCursor(5,0);   //L'idea sarebbe mettere su 6 invece che 5, ma non so come far mostrare un solo numero dopo la virgola, quindi per il momento lascio così
    lcd.print(String(valueTempIndoor));
  }
}

//function to read the temp value from the sensor
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
  
  pinMode(BUT_T_SWITCH, INPUT);     // initialize the pushbutton pin as an input for the On/Off command
  pinMode(BUT_T_DO, INPUT);      // initialize the pushbutton pin as an input for the turn down command
  pinMode(BUT_T_UP, INPUT);      // initialize the pushbutton pin as an input for the turn up command
  pinMode(BUT_T_MODE, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUT_T_SWITCH), changeValueTSwitch, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUT_T_UP), changeValueTUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUT_T_DO), changeValueTDown, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUT_T_MODE), changeValueTMode, FALLING);
}

//each loop start by checking the local changes, then it get values from the main loop 
//and sync the local values then it calculate the therm level and show in the LCD the status
void loopRegulator_therm() {          // read the value of of the therm status
  if(tSetChange && (millis() - lastTSetChange > 3000)){
    setThermSet(valueTSetDB); 
    tSetChange = false;
  }
  else if(!tSetChange){
    if(getThermSet()!= 0.0){
    valueTSetDB = getThermSet();    //acquire the data from the database     
    }
  }
  valueTSwitchDB = getThermSwitch();
  if(valueTSwitchDB.equals("go_on") || valueTSwitchDB.equals("go_off")){
    changeValueTSwitch();
  }
  valueTempIndoor = getTempIndoor();
  if(!valueTModeDB.equals(getThermMode())){
    valueTModeDB = getThermMode();
  }

  therm_level();
  lcdShow();
}
