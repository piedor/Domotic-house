#include <ESP8266_01.h>

#define SSID "ssid"
#define PASSWORD "password"
#define N_COMMANDS 2

ESP8266_01 esp;
String scriptId="GOOGLE_SCRIPT_ID";
String commands[N_COMMANDS];

void clearArrayCommands(){
  for(int i=0;i<N_COMMANDS;i++){
    commands[i] = "NULL";
  }
}

void getCommandsSpreadsheet(){
  clearArrayCommands();
  String response = esp.getSecureConnection("script.google.com", "/macros/s/" + scriptId + "/exec?read", 443, true, true);
  if(response.equals("ERROR")){
    Serial.println("Error getting commands");
  }
  else{
    // \r\n\r\nMESSAGE\r\n\r\n
    String msg = response.substring(response.indexOf("\r\n\r\n") + 7, response.length() - 5);
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
        String command = msg.substring(0, index);
        if(command.equals("")){
          commands[StringCount++] = "NULL";
        }
        else{
          commands[StringCount++] = command;
        }
        msg = msg.substring(index+1);
      }
    }
    Serial.println("Commands:");
    for(int i=0;i<N_COMMANDS;i++){
      Serial.println(commands[i]);
    }
  }
}

void sendDataSpreadsheet(float temp, float hum){
  // send data fast (no response, keep connection alive)
  String ack = esp.getSecureConnection("script.google.com", "/macros/s/" + scriptId + "/exec?temp=" + temp + "&hum=" + hum, 443, false, true);
  if(ack.equals("OK")){
    Serial.println("Data sent to spreadsheet");
  }
  else{
    Serial.println("Error sending data to spreadsheet");
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  
  esp = ESP8266_01(Serial2, Serial, true);
  //esp.setAsStation();
  //esp.scanWifi();
  if(esp.isConnected().equals("")){
    esp.connectWifi(SSID, PASSWORD);
  }
  //esp.setSingleConnection();
  esp.setMultipleConnection();
  //Serial.println(esp.getStatus());
  // enable HTTP 302 redirect link
  esp.enableAutoRedirectUrl();
}

void loop() {
  getCommandsSpreadsheet();

  float temp = 21;
  float hum = 54;

  sendDataSpreadsheet(temp, hum);
  
}
