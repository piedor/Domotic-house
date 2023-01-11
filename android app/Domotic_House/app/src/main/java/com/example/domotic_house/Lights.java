package com.example.domotic_house;

import android.widget.ToggleButton;

public class Lights {
    public static ToggleButton kitchenBtn;
    public static Lvalues kitchenLight;
    public static Lvalues bathLight;
    public static Lvalues bedLight;
    public static Lvalues studioLight;

    Lights() {
        kitchenLight = Lvalues.off;
        bathLight = Lvalues.off;
        bedLight = Lvalues.off;
        studioLight = Lvalues.off;
    }

    Lights(Lvalues a, String b, String c){
        kitchenLight=a;
        kitchenBtn.findViewById(Integer.valueOf("R.id."+b));

    }
    Lights(Lvalues a, Lvalues b, Lvalues c, Lvalues d) {
        kitchenLight = a;
        bathLight = b;
        bedLight = c;
        studioLight = d;
    }

    public void getLightsvalues(Lvalues a, Lvalues b, Lvalues c, Lvalues d) {
        kitchenLight = a;
        bathLight = b;
        bedLight = c;
        studioLight = d;
        }

    public String setLightsValues() {
        return String.valueOf(kitchenLight) + ";" + String.valueOf(bathLight) + ";" + String.valueOf(bedLight) + ";" + String.valueOf(studioLight);
    }
}
