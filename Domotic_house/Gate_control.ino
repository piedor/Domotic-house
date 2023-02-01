/*
 * Control continuos servo for the gate
 */

#include <Servo.h>

#define FORWARD 0         //definition of forward movement for servo motor
#define BACKWARDS 180     //definition of backwards movement for servo motor
#define STOP 93           //definition of stop for servo motor
#define PIN_MOTOR 16
#define BUT_GATE 5
// Echo pin of HCSR04
#define PIN_DISTANCE_SENSOR_INPUT 4
// Trig pin of HCSR04
#define PIN_DISTANCE_SENSOR_OUTPUT 2
// Update distance interval (working only when gate is moving) (microseconds)
#define UPDATE_DISTANCE_INTERVAL 25000 

Servo gate;
String valueGate = "close";                   //define the status of the gate
String valueGateDB = "close";                 //define the action of the gate from the DB
unsigned long startMicrosGateMoving = 0;      //define how much time 
unsigned long microsElapsedGateMoving = 0;    //define how much time 
bool gateRunning = false;                     //define if the gate is running

hw_timer_t * timerGate = NULL;
hw_timer_t * timerSensor = NULL;

// the time the pulse was sent
volatile long ultrasonic_echo_start = 0;
// the distance once it's been measured
volatile long ultrasonic_distance = 0;

bool externalStopper = false;
int countObstacle = 0;

//ultrasonic 
void IRAM_ATTR ultrasonicPulse(){
  // Sets the trigger on HIGH state for 10 micro seconds to send a series of pulses
  digitalWrite(PIN_DISTANCE_SENSOR_OUTPUT, HIGH);
  // blocks 10 microseconds from the interrupt
  delayMicroseconds(10);
  // disable the sending again so we can wait for a response
  digitalWrite(PIN_DISTANCE_SENSOR_OUTPUT, LOW);
  // record the send time
  ultrasonic_echo_start = micros();
}

void IRAM_ATTR ultrasonicEcho(){
  // don't do anything if no pulse has been sent
  if(ultrasonic_echo_start != 0){
    // calculate the distance by measuring how long it took to return the sound
    // The speed of sound is 343 m/s and we need half the time it took (since
    // the sound has to travel towards the object _AND_ back). So a single echo does
    // 1/(343/2) = 0.005831 seconds per meter
    ultrasonic_distance = (micros() - ultrasonic_echo_start) / 58;
    // If obstacle releaved
    if(ultrasonic_distance < 26){
      externalStopper = true;
      countObstacle++;
      if(countObstacle == 4){
        // Stop timers
        timerStop(timerGate);
        timerStop(timerSensor);
        if(valueGate.equals("opening")){
          setGate("open"); 
          valueGate = "open";
        }
        else{
          setGate("close"); 
          valueGate = "close";
        }
        // Stop gate
        stopGate();
        countObstacle = 0;
      }
    }
    else{
      countObstacle = 0;
    }
    ultrasonic_echo_start = 0;
  }
}

//interrupt timer function called when the movement timer of the servo ends
void IRAM_ATTR endOfRunGate(){
  externalStopper = false;
  stopGate();
  timerStop(timerSensor);
  timerAlarmDisable(timerSensor);
  detachInterrupt(digitalPinToInterrupt(PIN_DISTANCE_SENSOR_INPUT));
  if(valueGate.equals("opening")){
    setGate("open"); 
    valueGate = "open";
  }
  else if(valueGate.equals("closing")){
    setGate("close"); 
    valueGate = "close";
  }
}

//
void IRAM_ATTR setTimersGateSensor(){
  // Reset timer gate and sensor
  timerRestart(timerGate);
  timerWrite(timerGate, 0);
  timerWrite(timerSensor, 0);
  timerRestart(timerSensor);
  // Set alarm to call endOfRunGate function after  2 second (value in microseconds).
  // Do not repeat the alarm (third parameter)
  if(externalStopper){
    timerAlarmWrite(timerGate, microsElapsedGateMoving, false);
    externalStopper = false;
  }
  else{
    timerAlarmWrite(timerGate, 2000000, false);
  }
  // Start an alarm
  timerAlarmEnable(timerGate);

  // Link the echo function to the echo pin and start timer for exec ultrasonicPulse
  attachInterrupt(digitalPinToInterrupt(PIN_DISTANCE_SENSOR_INPUT), ultrasonicEcho, FALLING);
  timerAlarmEnable(timerSensor);

  // Track how many micros gate move
  startMicrosGateMoving = micros();
  microsElapsedGateMoving = 0;
}

//interrupt function to change the value of the gate
void IRAM_ATTR changeValueGate(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300) 
  {
    if(valueGateDB.equals("opening") || valueGate.equals("close")){
      openGate();
      setGate("opening"); 
      valueGate = "opening";
      setTimersGateSensor();
    }
    else if(valueGateDB.equals("closing") || valueGate.equals("open")){
      closeGate();
      setGate("closing"); 
      valueGate = "closing";
      setTimersGateSensor();
    }
    else if(valueGate.equals("opening")){
      externalStopper = true;
      // Stop timers
      timerStop(timerGate);
      timerStop(timerSensor);
      stopGate();
      setGate("open");
      valueGate = "open"; 
    }
    else{
      externalStopper = true;
      // Stop timers
      timerStop(timerGate);
      timerStop(timerSensor);
      stopGate();
      setGate("close");
      valueGate = "close"; 
    }
  }
  last_interrupt_time = interrupt_time;
}


void stopGate(){
  microsElapsedGateMoving = micros() - startMicrosGateMoving; // the new moving time will be how much time did the gate moved
  gateRunning = false;
  gate.write(STOP);
}

void openGate(){
  gateRunning = true;
  gate.write(FORWARD);
}

void closeGate(){
  gateRunning = true;
  gate.write(BACKWARDS);
}

void setupGate_control() {
  gate.attach(PIN_MOTOR);
  pinMode(BUT_GATE, INPUT);
  pinMode(PIN_DISTANCE_SENSOR_INPUT, INPUT);
  pinMode(PIN_DISTANCE_SENSOR_OUTPUT, OUTPUT);
  // Set the echo pin to receive interrupts
  attachInterrupt(digitalPinToInterrupt(BUT_GATE), changeValueGate, FALLING);
  
  // Timers
  // Set 80 divider for prescaler
  timerGate = timerBegin(0, 80, true);
  timerSensor = timerBegin(1, 80, true);

  // Attach endOfRunGate function to our timer
  timerAttachInterrupt(timerGate, &endOfRunGate, true);

  // Attach ultrasonicPulse function to our timer
  timerAttachInterrupt(timerSensor, &ultrasonicPulse, true);

  // Update distance every 100 ms (value in microseconds)
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timerSensor, UPDATE_DISTANCE_INTERVAL, true);
}

//for each loop 
void loopGate_control() {
  if(!gateRunning){
    //descalimer for late data input
    if(valueGate.equals("open") && getGate().equals("opening")){
      setGate("open");
    }
    else if(valueGate.equals("close") && getGate().equals("closing")){
      setGate("close");
    }
    else{
      valueGateDB = getGate();
    }

    //if the command from the spreadsheet is to open/close the gate we call the interrupt function to activate the servo
    if(valueGateDB.equals("opening"))
    {
      changeValueGate();
    }
    if(valueGateDB.equals("closing"))
    {
      changeValueGate();
    }
  }
}
