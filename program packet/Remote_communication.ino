#include <ESP8266_01.h>

#define SSID "ssid"
#define PASSWORD "password"
// ID of google script 
#define SCRIPT_ID "AKfycbwFisqe8wPxmIKcaQnJlYU3n6ZATmZzWe7gw1YhA7TUQVhRspb5yicT2KtGj6UhhqNb"
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

ESP8266_01 esp;
// commands array contains the 11 values
// Update it with getCommandsSpreadsheet function
// Send the values to spreadsheet with sendDataSpreadsheet function
String commands[N_COMMANDS];

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
    String msg = response.substring(response.indexOf("userHtml"), response.length());
    // Get values separated by ';'. Example of msg after: off;on;off;off;sens_on;on;cool;12.5;22.6;close;20/01/2023 13:50:08
    msg = msg.substring(msg.indexOf(":") + 5, msg.indexOf(",") - 4);
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

float getTempIndoor(){
  return(commands[I_TEMP_INDOOR].toFloat());
}

String getGate(){
  return(commands[I_GATE]);
}

String getDate(){
  return(commands[I_DATE]);
}

void setLedKitchen(String value){
  commands[I_LED_KITCHEN] = value;
}

void setLedBathroom(String value){
  commands[I_LED_BATHROOM] = value;
}

void setLedBedroom(String value){
  commands[I_LED_BEDROOM] = value;
}

void setLedStudio(String value){
  commands[I_LED_STUDIO] = value;
}

void setLedOutdoor(String value){
  commands[I_LED_OUTDOOR] = value;
}

void setThermSwitch(String value){
  commands[I_T_SWITCH] = value;
}

void setThermMode(String value){
  commands[I_T_MODE] = value;
}

void setThermSet(float temp){
  commands[I_T_SET] = temp;
}

void setTempIndoor(float temp){
  commands[I_TEMP_INDOOR] = temp;
}

void setGate(String value){
  commands[I_GATE] = value;
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

void setup() {
  Serial.begin(115200);
  // ESP8266 connected to Serial1 (pin 3.2 and 3.3 in MSP432 board) 
  Serial1.begin(115200);

  // ESP8266_01(ESP82266 serial reference, default serial reference (for debugging), want debugging);
  esp = ESP8266_01(Serial1, Serial, true);
  // Set ESP8266 as station (can connect to WI-FI)
  esp.setAsStation();
  // Connect WI-FI
  if(esp.isConnected().equals("")){
    esp.connectWifi(SSID, PASSWORD);
  }
  // As ESP8266 will only connect to one server (script.google.com) is not needed the multi connection support 
  esp.setSingleConnection();
}

void loop() {
  getCommandsSpreadsheet();
  delay(1000);
// sendDataSpreadsheet("on", "off", "on", "off", "on", "off", "warm", 18.5, 17.4, "opening");
}