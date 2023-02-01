package com.example.domotic_house;

import android.os.CountDownTimer;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

//this class will contain the function and the variable to comunicate with the database

//possible values for lights
enum Lvalues {on, off, go_on, go_off}
enum Loutdoor{on, off, sens_on, sens_off}
//possible values for thermostat
enum Tmode{cool, warm}
enum TSwitch {on, off, go_on, go_off}
//possible values for gate
enum Gmove{open, opening, close, closing};

public class Communication{
    public static int retryTimes=0;             //when the app cant connect more than 5 times in a row it will send the user to the main page with a message
    public static long receivingTimer=3000;     //timer that describe everywhen the data is received
    public static CountDownTimer countDownTimer=null;

    public final static String URL="https://script.google.com/macros/s/SCRIPT_ID/exec"+"?";

    protected static int lastData = 0; //variable that is setted with the last value of the length of the spreadsheet (column)
    public static Lights LMaster = new Lights();
    public static Thermostat TMaster = new Thermostat();
    public static Gate GMaster = new Gate();
    public static String language = "ita";

//method to insert the values from the spreadsheet to the local variable of the app
    public static void insertValues(String response, char page) throws JSONException {
        JSONObject data = new JSONObject(response);     //create the json object
        JSONArray lastRow=data.getJSONArray("items");   //gain the items' json array, from the spreadsheet we send just the last row to semplify this passage
        JSONObject values=lastRow.getJSONObject(0);     //create the json object with the values from the array

        if(lastData!=Integer.valueOf(values.getString("Last_value"))) {
            lastData=Integer.valueOf(values.getString("Last_value"));
            LMaster.setKitchenLight(values.getString("Led_kitchen"));
            LMaster.setBathLight(values.getString("Led_bathroom"));
            LMaster.setBedLight(values.getString("Led_bedroom"));

            LMaster.setStudioLight(values.getString("Led_studio"));
            LMaster.setOutdoorLight(Loutdoor.valueOf(values.getString("Led_outdoor")));
            TMaster.setOnTherm(values.getString("Thermostat_switch"));
            TMaster.setModeTherm(values.getString("Thermostat_mode"));
            TMaster.setTempImp(Float.valueOf(values.getString("Thermostat_set")));
            TMaster.setTempSens(Float.valueOf(values.getString("Temperature_data")));
            GMaster.setMoveGate(Gmove.valueOf(values.getString("Gate")));
            switch (page){
                case 'l': LightsControl.matchingLights();
                    break;
                case 't': ThermostatControl.matchingThermostat();
                    break;
                case 'g': GateControl.matchingGate();
                    break;
            }
            return;
        }
    }

//this method create the url that will communicate the values to the spreadsheet
    public static String setValues() {
        String values=URL; //will contain the values to update to the spreadsheet
        values+="LedKitchen="+Communication.LMaster.getKitchenLight()+"&LedBathroom="+Communication.LMaster.getBathLight()+"&LedBedroom="+Communication.LMaster.getBedLight()+"&LedStudio="+Communication.LMaster.getStudioLight()+"&LedOutdoor="+Communication.LMaster.getOutdoorLight();
        values+="&TSwitch="+Communication.TMaster.getSwitchTherm()+"&TMode="+Communication.TMaster.getModeTherm()+"&TSet="+String.valueOf(Communication.TMaster.getTempImp());
        values+="&Gate="+Communication.GMaster.getMoveGate();
        return values;
    }
    public static String createUrl(String temp){
        return URL+temp;
    }
}

