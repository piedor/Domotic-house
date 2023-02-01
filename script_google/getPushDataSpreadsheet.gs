var sheet_id = "SHEET_ID";
var sheet_name = "sheet1";

function doGet(e){
  var ss = SpreadsheetApp.openById(sheet_id);
  var sheet = ss.getSheetByName(sheet_name);

  // variables command
  var led_kitchen;
  var led_bathroom;
  var led_bedroom;
  var led_studio;
  var led_outdoor;
  var thermostat_switch;
  var thermostat_mode;
  var thermostat_set;
  var temp_indoor;
  var gate;
  var date;

  // if "read" return command values to client
  var read = e.parameter.read;
  if (read !== undefined){
    lastRow = sheet.getLastRow();
    led_kitchen = sheet.getRange("A" + lastRow);
    led_bathroom = sheet.getRange("B" + lastRow);
    led_bedroom = sheet.getRange("C" + lastRow);
    led_studio = sheet.getRange("D" + lastRow);
    led_outdoor = sheet.getRange("E" + lastRow);
    thermostat_switch = sheet.getRange("F" + lastRow);
    thermostat_mode = sheet.getRange("G" + lastRow);
    thermostat_set = sheet.getRange("H" + lastRow);
    temp_indoor = sheet.getRange("I" + lastRow);
    gate = sheet.getRange("J" + lastRow);
    date = sheet.getRange("K" + lastRow);
    return HtmlService.createHtmlOutput("$$" + led_kitchen.getValue() + ";" + led_bathroom.getValue() + ";" + led_bedroom.getValue() + ";" +
      led_studio.getValue() + ";" + led_outdoor.getValue() + ";" + thermostat_switch.getValue() + ";" + thermostat_mode.getValue() + ";" +
      thermostat_set.getValue() + ";" + temp_indoor.getValue() + ";" + gate.getValue() + ";" + date.getValue() + "%%");
    
    
    // ContentService.createTextOutput()
  }
  // else insert data to spreadsheet
    led_kitchen = e.parameter.LedKitchen;
if(led_kitchen==undefined)  led_kitchen=String(sheet.getRange(sheet.getLastRow() , 1, 1, 1).getValues()[0]);

  led_bathroom = e.parameter.LedBathroom;
if(led_bathroom==undefined)  led_bathroom=String(sheet.getRange(sheet.getLastRow() , 2, 1, 1).getValues()[0]);

  led_bedroom = e.parameter.LedBedroom;
if(led_bedroom==undefined)  led_bedroom=String(sheet.getRange(sheet.getLastRow() , 3, 1, 1).getValues()[0]);

  led_studio = e.parameter.LedStudio;
if(led_studio==undefined)  led_studio=String(sheet.getRange(sheet.getLastRow() , 4, 1, 1).getValues()[0]);

  led_outdoor = e.parameter.LedOutdoor;
if(led_outdoor==undefined)  led_outdoor=String(sheet.getRange(sheet.getLastRow() , 5, 1, 1).getValues()[0]);

  thermostat_switch = e.parameter.TSwitch;
if(thermostat_switch==undefined)  thermostat_switch=String(sheet.getRange(sheet.getLastRow() , 6, 1, 1).getValues()[0]);

  thermostat_mode = e.parameter.TMode;
if(thermostat_mode==undefined)  thermostat_mode=String(sheet.getRange(sheet.getLastRow() , 7, 1, 1).getValues()[0]);

  thermostat_set = e.parameter.TSet;
if(thermostat_set==undefined)  thermostat_set= Number(sheet.getRange(sheet.getLastRow() , 8, 1, 1).getValues()[0]);
  else   thermostat_set = Number(e.parameter.TSet);

  temp_indoor =e.parameter.TempIndoor;
if(temp_indoor==undefined)  temp_indoor= Number(sheet.getRange(sheet.getLastRow() , 9, 1, 1).getValues()[0]);
  else   temp_indoor = Number(e.parameter.TempIndoor);

  gate = e.parameter.Gate;
if(gate==undefined)  gate=String(sheet.getRange(sheet.getLastRow() , 10, 1, 1).getValues()[0]);

  date = Utilities.formatDate(new Date(), "GMT+1", "dd/MM/yyyy HH:mm:ss");

  sheet.appendRow([led_kitchen, led_bathroom, led_bedroom, led_studio, led_outdoor, thermostat_switch, thermostat_mode, thermostat_set,
  temp_indoor, gate, date]);
}
