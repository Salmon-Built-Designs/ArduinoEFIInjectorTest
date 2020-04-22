// Arduino Based PWM Control for high impedance injectors 
// Useful for flow testing and cleaning of EFI injectors
// 
//
//     Arduino                Transistor                    Injector
//                           P1(B)  P2(C)  P3(E)            12V     GND
//       ->GND  - -            |      |      |               |       |
//                |            |      |      |               |       |
//       ->P10 - ^|^ - - - - - -      - - - ^|^- - - - - - -^|^- - - - 
//                |                          |               |
//  12V Source    |                          |               |
//                |                          |               |
//       ->12V --^|^- - - - - - - - - - - - ^|^- - - - - - - - 
//                |                          |
//       ->GND - - - - - - - - - - - - - - - -
//
// Notes: 
//            |
//      A - -^|^- - Indicates that the wire from L to R does *not* connect. eg P10 only connects to P1 of the transistor (A Mosfet Can Be Used Too). 
//            |
//
//      Ensure the chosen Transistor / Mosfet has a fast enough trigger time for the openTime and adjust open time to compensate. Timing is calculated based on the arduino pin only.

//      The Injector gets 12V all the time the Transistor used should be a NPN transistor I used a ST 2SD882 
//              I had one on hand. it needs to trigger on ~5v or less and handle at least 200ma per injector or so. 
//
//      If using a benchtop power supply ensure that the grounds are all equal between the Arduino power and the 12V source. Using a battery would work as well. 
//
//      Injectors can overheat if opened too long without fuel flowing through them, they use the fuel to cool themselves as well as low open times to keep heat down. 
//
// Configuration: 
//
// Target RPM for a 4 cycle engine (1 injector pulse per 2 revolutions of the crankshaft) 
const int targetRPM = 2000;
// Target injector open time in milliseconds (usually between 1 and 15 ms) cannot exceed 100% duty cycle 
const int injectorOpenTimeMS = 4;
// PWM Pin 
const int triggerPin = 10;
// 
// Time after initial setup time to wait before triggering the first injector pulse (default 10 seconds).
const int waitTimeBeforeStartingRunMS = 10000;
// The total time to run the injector cycle (default 1 minute)
const int totalRunTimeInMS = 60000;


// Internal Timing Variables Do Not Configure, They are calculated in setup / between loops.

// time to sleep between pulses
int sleepTimeMS = -1;
// keeps track of how long we have been running
int runTimeCounter = 0;


void setup() {
  // Calculate Timing (targetRPM / 60) to get revolutions per second / 1000 to get time in milliseconds between revolutions * 2 for 4 cycle engines 
  int pulseTimeMS = (1000 / (targetRPM / 60)) * 2;
  // Calculate Time To Sleep Before Opening Injector (Cannot be negative (eg > 100% duty cycle)).
  sleepTimeMS = pulseTimeMS - injectorOpenTimeMS;

  // Set Output Pins (PWM and DEBUG LED)
  pinMode(triggerPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Check if sleep time is negative and flash on board LED on / off every 1 second do not power the injector.
  // Please ensure injectorOpenTime is not greater than pulseTime.
  while (sleepTimeMS < 1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
  
  // Wait Before Triggering The Main Loop, Ensure Fuel Pump Is Running ECT.
  delay(waitTimeBeforeStartingRunMS);
}

void loop() {
  if(runTimeCounter < totalRunTimeInMS)
  {
    // Wait for initial delay, trigger high, wait for open time, trigger low, calculate total time to run counter.
    delay(sleepTimeMS);
    digitalWrite(triggerPin, HIGH);digitalWrite(LED_BUILTIN, HIGH);
    delay(injectorOpenTimeMS);
    digitalWrite(triggerPin, LOW);digitalWrite(LED_BUILTIN, LOW);

    runTimeCounter = runTimeCounter + sleepTimeMS + injectorOpenTimeMS;
  }
}
