package com.example.domotic_house;
//this class will contain the function and the variable to comunicate with the database

//possible values for lights
enum Lvalues {on, off, go_on, go_off}

//possible values for thermostat
enum Tmode{cool, warm}
enum Ton_off{on, off, go_on, go_off}
//possible values for gate
enum Gmove{open, opening, close, closing};

public class Communication{

public static char changes='n'; // y/n needed to see if the app made changes
private static Lights LMaster=new Lights();
private static Thermostat TMaster=new Thermostat();
private static Gate GMaster=new Gate();
private static boolean x=true;
public static String language="ita";

public static void getValuesFromSpreadsheet(String value){
    if (value == null) return;
    char x[]=value.toCharArray();
    String a[]=new String[9];          //gate 4 + thermostat 3 + gate 1
    for (int i=0; i<9; i++) a[i]=null;
    int counter=0;
    for(int i=0; i<value.length();i++){
        if (x[i]==';') {
            counter++;
            continue;
            }
        if(a[counter]==null) a[counter]=String.valueOf(x[i]);
        else
            a[counter]=a[counter]+String.valueOf(x[i]);
    }
        LMaster.getLightsvalues(Lvalues.valueOf(a[0]),Lvalues.valueOf(a[1]),Lvalues.valueOf(a[2]),Lvalues.valueOf(a[3]));
        TMaster.getThermostatValues(Ton_off.valueOf(a[4]),Tmode.valueOf(a[5]),Float.valueOf(a[6]), Float.valueOf(a[7]));
        GMaster.getGateValue(Gmove.valueOf(a[8]));
    }

public static void getValueFromSpreadsheet(){
    //considera la possibilità di avere una variabile globale per verificare che sia stato modificato qualcosa

    String x=LMaster.setLightsValues()+";"+TMaster.setThermostatValues()+";"+GMaster.setGateValue();
    //send the value to the database
    }

public static void dataExample(){
    if (x) {
        x=false;
        getValuesFromSpreadsheet("on;off;off;on;on;warm;30;close");
    }
}

public static void synchronizeData(){

    if(changes=='y') {
        getValueFromSpreadsheet();
        changes='n';
    }
        else {/*
        String data = null;
        //data=get data from database;
        getValuesFromDatabase(data);
*/    }
}

}
