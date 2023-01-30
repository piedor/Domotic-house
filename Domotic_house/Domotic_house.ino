#include <ESP8266_01.h>

#define PIN_ESP_RX 23
#define PIN_ESP_TX 17
#define SSID "retenik"
#define PASSWORD "passwordxxx"
// ID of google script 
#define SCRIPT_ID "AKfycbyEn0mlY0pS9oK6ebzIZ-AeAz1RUhfqbrdEi-9vO-VtpKRNj7RWS-DZXp7vOY-rOmGAsQ"
// Google script send 11 values separated by ';' and receive 10 values because date is automatically generated
#define N_COMMANDS 11
// Indexes for commands array
#define I_LED_KITCHEN 0
#define I_LED_BATHROOM 1
#define I_LED_BEDROOM 2
#define I_LED_STUDIO 3
#define I_LED_OUTDOOR 4
#define I_T_SWITCH 5
#define I_T_MODE 6
#define I_T_SET 7
#define I_TEMP_INDOOR 8
#define I_GATE 9
#define I_DATE 10

unsigned long currentMillis;
unsigned long lastMillis;

ESP8266_01 esp;
// commands array contains the 11 values
// Update it with getCommandsSpreadsheet function
// Send the values to spreadsheet with sendDataSpreadsheet function
String commands[N_COMMANDS];
String valuesToSend = "";

void clearArrayCommands(){
  // Set commands array to NULL values
  for(int i=0;i<N_COMMANDS;i++){
    commands[i] = "NULL";
  }
}


void getCommandsSpreadsheet(){
  // Put spreadsheet values into commands array
  // A HTTP response is received, the values are after 'userHtml:' attribute
  // esp.getSecureConnection(host, request, port, want response, want keep connection alive);
  // Example of request when reading data from spreadsheet: script.google.com/macros/s/*SCRIPT_ID*/exec?read
  String response = esp.getSecureConnection("script.google.com", "/macros/s/" + String(SCRIPT_ID) + "/exec?read", 443, true, true);
  if(response.equals("ERROR")){
    Serial.println("Error getting commands");
  }
  else{
    clearArrayCommands();
    // Get substring from 'userHtml' to end of response. Example of msg after: userHtml:\x22off;on;off;off;sens_on;on;cool;12.5;22.6;close;20/01/2023 13:50:08\x22,attributes:...</body></html>\r\n\r\n\r\n
    int indexFirst = response.indexOf("$$");
    int indexLast = response.indexOf("%%");
    if(indexFirst >= 0 && indexLast >= 0){
      String msg = response.substring(indexFirst + 2, indexLast);
      // Get values separated by ';'. Example of msg after: off;on;off;off;sens_on;on;cool;12.5;22.6;close;20/01/2023 13:50:08
      // Split the values in commands array
      int StringCount = 0;
      while(msg.length() > 0){
        int index = msg.indexOf(';');
        if (index == -1) // No ; found
        {
          commands[StringCount++] = msg;
          break;
        }
        else{
          commands[StringCount++] = msg.substring(0, index);
          msg = msg.substring(index+1);
        }
      }
    }
  }
}

String getLedKitchen(){
  return(commands[I_LED_KITCHEN]);
}

String getLedBathroom(){
  return(commands[I_LED_BATHROOM]);
}

String getLedBedroom(){
  return(commands[I_LED_BEDROOM]);
}

String getLedStudio(){
  return(commands[I_LED_STUDIO]);
}

String getLedOutdoor(){
  return(commands[I_LED_OUTDOOR]);
}

String getThermSwitch(){
  return(commands[I_T_SWITCH]);
}

String getThermMode(){
  return(commands[I_T_MODE]);
}

float getThermSet(){
  return(commands[I_T_SET].toFloat());
}

String getGate(){
  return(commands[I_GATE]);
}

String getDate(){
  return(commands[I_DATE]);
}

void sendDataSpreadsheet(){
  // Send commands array values to spreadsheet
  // esp.getSecureConnection(host, request, port, want response, want keep connection alive)
  // Example of request when sending data to spreadsheet: script.google.com/macros/s/*SCRIPT_ID*/exec?*VARIABLE1*=*VALUE1*&*VARIABLE2*=*VALUE2*
  String ack = esp.getSecureConnection("script.google.com", "/macros/s/" + String(SCRIPT_ID) + "/exec?LedKitchen=" + getLedKitchen() + "&LedBathroom=" + getLedBathroom()
  + "&LedBedroom=" + getLedBedroom() + "&LedStudio=" + getLedStudio() + "&LedOutdoor=" + getLedOutdoor() + "&TSwitch=" + getThermSwitch() + "&TMode=" + getThermMode() + "&TSet=" + getThermSet()
  + "&TempIndoor=" + getTempIndoor() + "&Gate=" + getGate(), 443, false, true);
  if(ack.equals("OK")){
    Serial.println("Data sent to spreadsheet");
  }
  else{
    Serial.println("Error sending data to spreadsheet");
  }
}
void sendDataSpreadsheet2(String valueToSend){
  // Send commands array values to spreadsheet
  // esp.getSecureConnection(host, request, port, want response, want keep connection alive)
  // Example of request when sending data to spreadsheet: script.google.com/macros/s/*SCRIPT_ID*/exec?*VARIABLE1*=*VALUE1*&*VARIABLE2*=*VALUE2*
  String ack = esp.getSecureConnection("script.google.com", "/macros/s/" + String(SCRIPT_ID) + "/exec?"+ valueToSend, 443, false, true);
  if(ack.equals("OK")){
    if(valuesToSend.equals(valueToSend)){
      valuesToSend = "";
    }
    Serial.println("Data sent to spreadsheet");
  }
  else{
    Serial.println("Error sending data to spreadsheet");
  }
}

void setLedKitchen(String value){
  String temp = "LedKitchen=" + commands[I_LED_KITCHEN] + "&";
  valuesToSend.replace(temp, "");
  commands[I_LED_KITCHEN] = value;
  valuesToSend += "LedKitchen=" + value + "&";
}

void setLedBathroom(String value){
  String temp = "LedBathroom=" + commands[I_LED_BATHROOM] + "&";
  valuesToSend.replace(temp, "");
  commands[I_LED_BATHROOM] = value;
  valuesToSend += "LedBathroom=" + value + "&";
}

void setLedBedroom(String value){
  String temp = "LedBedroom=" + commands[I_LED_BEDROOM] + "&";
  valuesToSend.replace(temp, "");
  commands[I_LED_BEDROOM] = value;
  valuesToSend += "LedBedroom=" + value + "&";
}

void setLedStudio(String value){
  String temp = "LedStudio=" + commands[I_LED_STUDIO] + "&";
  valuesToSend.replace(temp, "");
  commands[I_LED_STUDIO] = value;
  valuesToSend += "LedStudio=" + value + "&";
}

void setLedOutdoor(String value){
  String temp = "LedOutdoor=" + commands[I_LED_OUTDOOR] + "&";
  valuesToSend.replace(temp, "");
  commands[I_LED_OUTDOOR] = value;
  valuesToSend += "LedOutdoor=" + value + "&";
}

void setThermSwitch(String value){
  String temp = "TSwitch=" + commands[I_T_SWITCH] + "&";
  valuesToSend.replace(temp, "");
  commands[I_T_SWITCH] = value;
  valuesToSend += "TSwitch=" + value + "&";
}

void setThermMode(String value){
  String temp = "TMode=" + commands[I_T_MODE] + "&";
  valuesToSend.replace(temp, "");
  commands[I_T_MODE] = value;
  valuesToSend += "TMode=" + value + "&";
}

void setThermSet(float value){
  String temp = "TSet=" + commands[I_T_SET] + "&";
  valuesToSend.replace(temp, "");
  commands[I_T_SET] = String(value);
  valuesToSend +="TSet=" + String(value) + "&";
}

void setTempIndoor(float value){
  String temp = "TempIndoor=" + commands[I_TEMP_INDOOR] + "&";
  valuesToSend.replace(temp, "");
  commands[I_TEMP_INDOOR] = String(value);
  valuesToSend += "TempIndoor=" + String(value) + "&";
}

void setGate(String value){
  String temp = "Gate=" + commands[I_GATE] + "&";
  valuesToSend.replace(temp, "");
  commands[I_GATE] = value;
  valuesToSend += "Gate=" + value + "&";
}

void initSpreadsheet(){
  // Set default commands to spreadsheet
  commands[I_LED_KITCHEN] = "off";
  commands[I_LED_BATHROOM] = "off";
  commands[I_LED_BEDROOM] = "off";
  commands[I_LED_STUDIO] = "off";
  commands[I_LED_OUTDOOR] = "off";
  commands[I_T_SWITCH] = "off";
  commands[I_T_SET] = "18.0";
  commands[I_T_MODE] = "warm";
  commands[I_TEMP_INDOOR] = String(getTempIndoor());
  commands[I_GATE] = "close";
  sendDataSpreadsheet();
}

void setup() {
  Serial.begin(115200);

  // Various setup
  setupLED_kitchen();
  setupLED_bathroom();
  setupLED_bedroom();
  setupLED_studio();
  setupLED_outdoor();
  setupGate_control();
  setupRegulator_therm();
  
  // ESP8266 serial
  Serial2.begin(115200, SERIAL_8N1, PIN_ESP_RX, PIN_ESP_TX);
  

  // ESP8266_01(ESP82266 serial reference, default serial reference (for debugging), want debugging);
  esp = ESP8266_01(Serial2, Serial, true);
  // Set ESP8266 as station (can connect to WI-FI)
  esp.setAsStation();
  // The ESP tries to reconnect to AP at the interval of 10 seconds for 100 times when disconnected
  esp.setAutoReconnectWifi(10, 100);
  // Connect WI-FI (attempt 2 times)
  int i=0;
  while(esp.isConnected().equals("") && i < 2){
    esp.connectWifi(SSID, PASSWORD);
    delay(500);
    i++;
  }
  // As ESP8266 will only connect to one server (script.google.com) is not needed the multi connection support 
  esp.setSingleConnection();
  // Set all commands in spreadsheet to default
  initSpreadsheet();

  delay(2000);

  lastMillis = millis();
}

void loop() {
  // Various loop
  loopLED_kitchen();
  loopLED_bathroom();
  loopLED_bedroom();
  loopLED_studio();
  loopLED_outdoor();
  loopGate_control();
  loopRegulator_therm();

  // Check for commands remote timer
  currentMillis = millis();
  if((currentMillis - lastMillis > 1000) && valuesToSend.equals("")){
    getCommandsSpreadsheet();
    lastMillis = millis();
  }
  if(!valuesToSend.equals("")){
    sendDataSpreadsheet2(valuesToSend);
  }
}
