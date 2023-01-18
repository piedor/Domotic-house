/*
This program controls the power of an ipotetic thermostat design from 3 leds.
Digitally it takes the desired temperature from the database and activate the
right number of power to reach that temperature (power=leds).
We can control the value of the desired temperature by the button Up/Down
and turn on or off the thermostat from button On/Off
 */
 
#include "LiquidCrystal_I2C.h"

#define but_O_F 28
#define but_Do 30
#define but_Up 12
#define but_MODE 29
LiquidCrystal_I2C lcd(0x3F,16,2);  //Set dell'indirizzo dell'LCD con 16 caratteri e 2 righe

/*SCHEMINO DELL'LCD PER BESTEMMIARE MENO :))
      0 1 2 3 4 5 6 7 8 9 A B C D E F
    0 T E M P : - - . - - X P O W E R
    1 S E T  : X - - . - - X X - - - X
*/


const int butUpPin = but_Up;     // the number of the pushbutton pin
const int butDoPin = but_Do;     // the number of the pushbutton pin
const int butOFPin = but_O_F;     // the number of the pushbutton pin

int but_Up_State = 0;         // variable for reading the pushbutton status for button up
int but_Do_State = 0;         // variable for reading the pushbutton status for button down
int but_O_F_State = 0;         // variable for reading the pushbutton status  for on/f

int powerLevel; // (0,1,2,3) define the level power of the thermostat
float valuesens_TEMP;         //data of the temp from the sens    lcd-value sens
float valueimp_TEMP;          //data of the imposted temp      lcd- value imp
String therm_acc;            //look if the term is active   lcd- on/off       
String mode;     //acquire the mode (warm, cool)

byte Snowflake[8] = {
0b00000,
0b00000,
0b10101,
0b01110,
0b11011,
0b01110,
0b10101,
0b00000,
};

byte Fire[8] = {
0b00001,
0b01000,
0b10010,
0b00110,
0b01111,
0b11111,
0b11111,
0b01010,
};
//function to define the level of the temperature of the term
void therm_level(){
  //valuesens_TEMP=SENS_temp.readvalue();        //acquire the temp from the sens
  if( valueimp_TEMP>(valuesens_TEMP+5) ){
    powerLevel=3;
  }
  if( valueimp_TEMP>(valuesens_TEMP+2) ){
    powerLevel=2;
    }
  if( valueimp_TEMP>=(valuesens_TEMP-1) ){
    powerLevel=1;
    }
  if( valueimp_TEMP<(valuesens_TEMP-2) ){
    powerLevel=0;
  }
}

void change_value(float t){
  valueimp_TEMP=valueimp_TEMP+t;                  // acquire the data from the database
  Serial1.println(valueimp_TEMP);                  // change the value to the new one
}

void lcdShow(){
  if(therm_acc=="off"){
    lcd.clear();
    lcd.setCursor(0, 0);    //Funzione per decidere dove mettere il cursore (carattere(da 0 a 15), riga(0 o 1))
    lcd.print("TEMP: ");  
    lcd.setCursor(5,0);   //L'idea sarebbe mettere su 6 invece che 5, ma non so come far mostrare un solo numero dopo la virgola, quindi per il momento lascio così
    lcd.print(String(valuesens_TEMP));
    delay (1500);
  }else{
    byte x;
    lcd.setCursor(0, 0);
    lcd.print("TEMP:      POWER");
    lcd.setCursor(0, 1);
    lcd.print("SET :          ");
    if (mode=="warm") x=Fire;
      else x=Snowflakes;
    switch (powerLevel){
      case (0):{  lcd.setCursor(12, 1);
                  lcd.print("   ");}
      case (1):{  lcd.setCursor(12, 1);
                  lcd.write(x);
                  lcd.print("  ");}
      case (2):{  lcd.write(x);
                  lcd.write(x);
                  lcd.print(" ");}
      case (3):{  lcd.write(x);
                  lcd.write(x);
                  lcd.write(x);
                  }
      default: break;
    }
  }
}

void setupRegulator_therm() {
  valueimp_TEMP=Serial1.read();                  //acquire the data from the database     
  therm_acc=Serial1.read();
  
  lcd.init();    //init the lcd
  lcd.clear();   //clear the lcd screen
  lcd.backlight();
  
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

therm_level();
lcdShow();
 
delay(1000);
}
