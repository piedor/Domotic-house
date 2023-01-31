package com.example.domotic_house;

public class Thermostat {
    private static Tmode modeTherm;
    private static TSwitch onTherm;
    private static double tempImp;
    private static double tempSens;
    Thermostat() {
        onTherm = TSwitch.off;
        modeTherm=Tmode.warm;
        tempSens=18;
        tempImp=18;
    }

    Thermostat(TSwitch a, Tmode b, float c, float d) {
        onTherm = a;
        modeTherm = b;
        tempImp = c;
        tempSens = d;
    }

//methods to set and get the values of the object
    public void setTempImp(float a){
        tempImp=a;
    }
    public void setModeTherm(String a){
        modeTherm=Tmode.valueOf(a);
    }
    public void setOnTherm(String a){
        onTherm= TSwitch.valueOf(a);
    }
    public void setTempSens(float a){
        tempSens=a;
    }

    public String getModeTherm(){
        return String.valueOf(modeTherm);
    }
    public String getSwitchTherm(){
        return String.valueOf(onTherm);
    }
    public double getTempImp(){
        return tempImp;
    }
    public double getTempSens(){
        return tempSens;
    }

    public void setThermostatValues(TSwitch a, Tmode b, float c, float d) {
        onTherm = a;
        modeTherm= b;
        tempImp = c;
        tempSens = d;
    }
}
