/*
 * Control continuos servo for the gate
 */

#include <Servo.h>

#define FORWARD 0
#define BACKWARDS 180
#define STOP 93
Servo gate;

void stopGate(){
  gate.write(STOP);
}

void openGate(){
  gate.write(FORWARD);
  delay(2000);
  stopGate();
}

void closeGate(){
  gate.write(BACKWARDS);
  delay(2000);
  stopGate();
}

void setup() {
  gate.attach(9);  // attaches the servo on pin 9
}

void loop() {
  openGate();
  delay(1000);
  closeGate();
  delay(1000);
}
