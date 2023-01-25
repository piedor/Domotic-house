/*
 * Control continuos servo for the gate
 */

#include <Servo.h>

#define FORWARD 0
#define BACKWARDS 180
#define STOP 93
#define PIN_MOTOR 16
#define BUT_GATE 5

Servo gate;
bool valueGate = 0;
String valueGateDB = "close";    
long t0=0; //used as a timer to stop the servo
bool run_gate;

void IRAM_ATTR changeValueGate(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300) 
  {
    valueGate = !valueGate;
    t0=millis();
  }
  last_interrupt_time = interrupt_time;
}

void stopGate(){
  gate.write(STOP);
  run_gate=false;
}

void openGate(){
  gate.write(FORWARD);
  run_gate=true;
}

void closeGate(){
  gate.write(BACKWARDS);
  run_gate=true;
}

void setupGate_control() {
  gate.attach(PIN_MOTOR);
  attachInterrupt(digitalPinToInterrupt(BUT_GATE), changeValueGate, FALLING);
}

void loopGate_control() {
  valueGateDB = getGate();
  if((valueGateDB == "opening") || (valueGateDB == "close" && valueGate))
  {
    openGate();
    valueGate = 1;
    // delay(300);
  }
  
  if((valueGateDB == "closing") || (valueGateDB == "open" && !valueGate))
  {
    closeGate();
    valueGate = 0;
    // delay(300);
  }
  if(run_gate /* &&  /*quando viene premuto il pulsante*/)  //quando premo se si sta muovendo devo fermare il cancello 
    stopGate();
  
  if (run_gate && (millis()-t0)>2000){
    stopGate();
    if(valueGateDB== "opening")
      setGate("open");
    if(valueGateDB == "closing") 
      setGate("close");
 }
}
