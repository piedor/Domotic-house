package com.example.domotic_house;

public class Gate {
    public static Gmove moveGate;

    Gate() {
        moveGate = Gmove.close;
    }

    Gate(Gmove a) {
        moveGate = a;
    }

    public void getGateValue(Gmove a) {
        moveGate = a;
    }

    public String setGateValue() {
        return String.valueOf(moveGate);
    }
}
