#include <ESP8266_01.h>

#define SSID "ssid"
#define PASSWORD "password"
#define SCRIPT_ID "AKfycbwFisqe8wPxmIKcaQnJlYU3n6ZATmZzWe7gw1YhA7TUQVhRspb5yicT2KtGj6UhhqNb"
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
String commands[N_COMMANDS];

void clearArrayCommands(){
  for(int i=0;i<N_COMMANDS;i++){
    commands[i] = "NULL";
  }
}

void getCommandsSpreadsheet(){
  // Put spreadsheet values into commands array
  String response = esp.getSecureConnection("script.google.com", "/macros/s/" + String(SCRIPT_ID) + "/exec?read", 443, true, true);
  if(response.equals("ERROR")){
    Serial.println("Error getting commands");
  }
  else{
    clearArrayCommands();
    // \r\n\r\nMESSAGE\r\n\r\n
    String msg = response.substring(response.indexOf("userHtml"), response.length());
    msg = msg.substring(msg.indexOf(":") + 5, msg.indexOf(",") - 4); // \x22ON;ON\x22 -> ON;ON
    // Split the string into substrings
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

void sendDataSpreadsheet(String ledKitchen, String ledBathroom, String ledBedroom, String ledStudio, String ledOutdoor, String thermSwitch,
String thermMode, float thermSet, float tempIndoor, String gate){
  // send data fast (no response, keep connection alive)
  
  String ack = esp.getSecureConnection("script.google.com", "/macros/s/" + String(SCRIPT_ID) + "/exec?LedKitchen=" + ledKitchen + "&LedBathroom=" + ledBathroom
  + "&LedBedroom=" + ledBedroom + "&LedStudio=" + ledStudio + "&LedOutdoor=" + ledOutdoor + "&TSwitch=" + thermSwitch + "&TMode=" + thermMode + "&TSet=" + thermSet
  + "&TempIndoor=" + tempIndoor + "&Gate=" + gate, 443, false, true);
  if(ack.equals("OK")){
    Serial.println("Data sent to spreadsheet");
  }
  else{
    Serial.println("Error sending data to spreadsheet");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.begin(115200);
  
  esp = ESP8266_01(Serial1, Serial, true);
  esp.setAsStation();
  //esp.scanWifi();
  if(esp.isConnected().equals("")){
    esp.connectWifi(SSID, PASSWORD);
  }
  //esp.setSingleConnection();
  esp.setMultipleConnection();
  // enable HTTP 302 redirect link
  esp.enableAutoRedirectUrl();
}

void loop() {
  getCommandsSpreadsheet();
  delay(1000);
// sendDataSpreadsheet("on", "off", "on", "off", "on", "off", "warm", 18.5, 17.4, "opening");
}
