#include <ESP8266_01.h>

#define PIN_ESP_RX 23
#define PIN_ESP_TX 17
#define SSID "prova"
#define PASSWORD "password"
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
// Prevent concurrent access to commands[] variable
portMUX_TYPE commandsMux = portMUX_INITIALIZER_UNLOCKED;
// Prevent concurrent access to valuesToSend variable
portMUX_TYPE valuesToSendMux = portMUX_INITIALIZER_UNLOCKED;

void setCommands(String value, int index){
  // Insert value into commands[index]
  portENTER_CRITICAL(&commandsMux);
  commands[index] = value;
  portEXIT_CRITICAL(&commandsMux);
}

String getCommands(int index){
  // Get commands[index]
  String temp;
  portENTER_CRITICAL(&commandsMux);
  temp = commands[index];
  portEXIT_CRITICAL(&commandsMux);
  return(temp);
}

String getValuesToSend(){
  String temp;
  portENTER_CRITICAL(&valuesToSendMux);
  temp = valuesToSend;
  portEXIT_CRITICAL(&valuesToSendMux);
  return(temp);
}

void clearArrayCommands(){
  // Set commands array to NULL values
  for(int i=0;i<N_COMMANDS;i++){
    setCommands("NULL", i);
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
          setCommands(msg, StringCount++);
          break;
        }
        else{
          setCommands(msg.substring(0, index), StringCount++);
          msg = msg.substring(index+1);
        }
      }
    }
  }
}

String getLedKitchen(){
  return(getCommands(I_LED_KITCHEN));
}

String getLedBathroom(){
  return(getCommands(I_LED_BATHROOM));
}

String getLedBedroom(){
  return(getCommands(I_LED_BEDROOM));
}

String getLedStudio(){
  return(getCommands(I_LED_STUDIO));
}

String getLedOutdoor(){
  return(getCommands(I_LED_OUTDOOR));
}

String getThermSwitch(){
  return(getCommands(I_T_SWITCH));
}

String getThermMode(){
  return(getCommands(I_T_MODE));
}

float getThermSet(){
  return(getCommands(I_T_SET).toFloat());
}

String getGate(){
  return(getCommands(I_GATE));
}

String getDate(){
  return(getCommands(I_DATE));
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
    if(getValuesToSend().equals(valueToSend)){
      portENTER_CRITICAL(&valuesToSendMux);
      valuesToSend = "";
      portEXIT_CRITICAL(&valuesToSendMux);
    }
    Serial.println("Data sent to spreadsheet");
  }
  else{
    Serial.println("Error sending data to spreadsheet");
  }
}

void setLedKitchen(String value){
  String temp = "LedKitchen=" + getCommands(I_LED_KITCHEN) + "&";
  portENTER_CRITICAL(&valuesToSendMux);
  valuesToSend.replace(temp, "");
  valuesToSend += "LedKitchen=" + value + "&";
  portEXIT_CRITICAL(&valuesToSendMux);
  setCommands(value, I_LED_KITCHEN);
}

void setLedBathroom(String value){
  String temp = "LedBathroom=" + getCommands(I_LED_BATHROOM) + "&";
  portENTER_CRITICAL(&valuesToSendMux);  
  valuesToSend.replace(temp, "");
  valuesToSend += "LedBathroom=" + value + "&";
  portEXIT_CRITICAL(&valuesToSendMux);
  setCommands(value, I_LED_BATHROOM);
}

void setLedBedroom(String value){
  String temp = "LedBedroom=" + getCommands(I_LED_BEDROOM) + "&";
  portENTER_CRITICAL(&valuesToSendMux); 
  valuesToSend.replace(temp, "");
  valuesToSend += "LedBedroom=" + value + "&";
  portEXIT_CRITICAL(&valuesToSendMux);
  setCommands(value, I_LED_BEDROOM);
}

void setLedStudio(String value){
  String temp = "LedStudio=" + getCommands(I_LED_STUDIO) + "&";
  portENTER_CRITICAL(&valuesToSendMux); 
  valuesToSend.replace(temp, "");
  valuesToSend += "LedStudio=" + value + "&";
  portEXIT_CRITICAL(&valuesToSendMux);
  setCommands(value, I_LED_STUDIO);
}

void setLedOutdoor(String value){
  String temp = "LedOutdoor=" + getCommands(I_LED_OUTDOOR) + "&";
  portENTER_CRITICAL(&valuesToSendMux); 
  valuesToSend.replace(temp, "");
  valuesToSend += "LedOutdoor=" + value + "&";
  portEXIT_CRITICAL(&valuesToSendMux);
  setCommands(value, I_LED_OUTDOOR);
}

void setThermSwitch(String value){
  String temp = "TSwitch=" + getCommands(I_T_SWITCH) + "&";
  portENTER_CRITICAL(&valuesToSendMux);
  valuesToSend.replace(temp, "");
  valuesToSend += "TSwitch=" + value + "&";
  portEXIT_CRITICAL(&valuesToSendMux);
  setCommands(value, I_T_SWITCH);
}

void setThermMode(String value){
  String temp = "TMode=" + getCommands(I_T_MODE) + "&";
  portENTER_CRITICAL(&valuesToSendMux);
  valuesToSend.replace(temp, "");
  valuesToSend += "TMode=" + value + "&";
  portEXIT_CRITICAL(&valuesToSendMux);
  setCommands(value, I_T_MODE);
}

void setThermSet(float value){
  String temp = "TSet=" + getCommands(I_T_SET) + "&";
  portENTER_CRITICAL(&valuesToSendMux);
  valuesToSend.replace(temp, "");
  valuesToSend +="TSet=" + String(value) + "&";
  portEXIT_CRITICAL(&valuesToSendMux);
  setCommands(String(value), I_T_SET);
}

void setTempIndoor(float value){
  String temp = "TempIndoor=" + getCommands(I_TEMP_INDOOR) + "&";
  portENTER_CRITICAL(&valuesToSendMux);
  valuesToSend.replace(temp, "");
  valuesToSend += "TempIndoor=" + String(value) + "&";
  portEXIT_CRITICAL(&valuesToSendMux);
  setCommands(String(value), I_TEMP_INDOOR);
}

void setGate(String value){
  String temp = "Gate=" + getCommands(I_GATE) + "&";
  portENTER_CRITICAL(&valuesToSendMux);
  valuesToSend.replace(temp, "");
  valuesToSend += "Gate=" + value + "&";
  portEXIT_CRITICAL(&valuesToSendMux);
  setCommands(value, I_GATE);
}

void initSpreadsheet(){
  // Set default commands to spreadsheet
  setCommands("off", I_LED_KITCHEN);
  setCommands("off", I_LED_BATHROOM);
  setCommands("off", I_LED_BEDROOM);
  setCommands("off", I_LED_STUDIO);
  setCommands("off", I_LED_OUTDOOR);
  setCommands("off", I_T_SWITCH);
  setCommands("18.0", I_T_SET);
  setCommands("warm", I_T_MODE);
  setCommands(String(getTempIndoor()), I_TEMP_INDOOR);
  setCommands("close", I_GATE);
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
  if((currentMillis - lastMillis > 5000) && getValuesToSend().equals("")){
    getCommandsSpreadsheet();
    lastMillis = millis();
  }
  if(!getValuesToSend().equals("")){
    sendDataSpreadsheet2(getValuesToSend());
  }
}
