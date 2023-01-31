package com.example.domotic_house;

import android.widget.ToggleButton;

public class Lights {
    private static Lvalues kitchenLight;
    private static Lvalues bathLight;
    private static Lvalues bedLight;
    private static Lvalues studioLight;
    private static Loutdoor outdoorLight;

    Lights() {
        kitchenLight = Lvalues.off;
        bathLight = Lvalues.off;
        bedLight = Lvalues.off;
        studioLight = Lvalues.off;
        outdoorLight=Loutdoor.off;
    }

    Lights(Lvalues a, Lvalues b, Lvalues c, Lvalues d, Loutdoor e) {
        kitchenLight = a;
        bathLight = b;
        bedLight = c;
        studioLight = d;
        outdoorLight= e;
    }

    //methods to set and get the values of the object
    public void setKitchenLight(String a){
        kitchenLight = Lvalues.valueOf(a);
    }
    public void setBedLight(String a){
        bedLight = Lvalues.valueOf(a);
    }
    public void setBathLight(String a){
        bathLight = Lvalues.valueOf(a);
    }
    public void setStudioLight(String a){
        System.out.println("chiamata");
        studioLight = Lvalues.valueOf(a);
    }
    public void setOutdoorLight(Loutdoor a){
        outdoorLight = a;
    }
    public String getKitchenLight() {
        return String.valueOf(kitchenLight);
    }
    public String getBathLight() {
        return String.valueOf(bathLight);
    }
    public String getBedLight() {
        return String.valueOf(bedLight);
    }
    public String getStudioLight() {
        System.out.println("Ciao"+studioLight);
        return String.valueOf(studioLight);
    }
    public String getOutdoorLight() {
        return String.valueOf(outdoorLight);
    }

}
