package com.example.domotic_house;

public class Gate {
    public static Gmove moveGate;

    Gate() {
        moveGate = Gmove.close;
    }

    Gate(Gmove a) {
        moveGate = a;
    }

    //methods to set and get the value of the object
    public void setMoveGate(Gmove a) {
        moveGate = a;
    }
    public String getMoveGate() {
        return String.valueOf(moveGate);
    }
}
