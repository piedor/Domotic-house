package com.example.domotic_house;

public class Thermostat {
    public static Tmode modeTherm;
    public static Ton_off onTherm;
    public static float tempImp;
    public static float tempSens;
    Thermostat() {
        onTherm = Ton_off.off;
    }

    Thermostat(Ton_off a, Tmode b, float c, float d) {
        onTherm = a;
        modeTherm = b;
        tempImp = c;
        tempSens = d;
    }

    public void getThermostatValues(Ton_off a,Tmode b, float c, float d) {
        onTherm = a;
        modeTherm= b;
        tempImp = c;
        tempSens = d;
    }

    public String setThermostatValues() {
        return onTherm + ";" + modeTherm + ";" + tempImp;
    }
}
