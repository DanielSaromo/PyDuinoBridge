/*
  arduino_example.ino - PyDuino Bridge: Library for transparent bidirectional
  communication between Python and Arduino. Arduino Example Code.
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Author: Daniel Saromo. Adapted from Robin2 code.
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  Description:
  
  Changes the flash time interval of 2 LEDs and the angular position of a servomotor
  according to the values received from Python throught serial communication.

  The Arduino replies with the same information received, and the digital state of a button.

  Also, PyDuino Bridge always returns the scaled millis value of the arduino, this value
  is set by default so that the time returned is approximately seconds. It can be modified
  by calling the function `setMillisDivider(byte newDivider)` of the Bridge_ino object.

  Turns on and off a light emitting diode (LED) connected to a digital pin,
  without using the delay() function. This means that other code can run at the
  same time without being interrupted by the LED code.

  Circuit:
  - Two LEDs connected to the pins 3 and 4.
  - A servomotor, which control pin is connected to the pin 5.
  - A button (switch), connected to the pin 8.

  Configuration considerations:

  By default, the maximum number of characters allowed in the messages between Arduino and Python is 40.
  If you want to change this buffer size, you have to put the same value in both the Python (change the value `buffSize`
  when using the `begin` method) and the Arduino code (change the line `#define buffSize 40` in the `pyduino_bridge.h` and
  `pyduino_bridge.cpp` files).

  Also, verify that the same `numIntValues_FromPy` and `numFloatsValues_FromPy` values are configured in the Python code
  you are using, and also in the Arduino library files (before uploading, change the lines `#define numIntValues_FromPy 1`
  and `#define numFloatValues_FromPy 1` in the `pyduino_bridge.h` and `pyduino_bridge.cpp`). In the Arduino library files,
  this number is set by the `#define` command, since the value representing the number of elements in the array must be a
  constant expression, because arrays are blocks of static memory whose size must be determined at the compilation time,
  before the program runs (for more information, see http://www.cplusplus.com/doc/tutorial/arrays/).

  Timeline:

  24 Apr 2020 - Daniel Saromo: Created python_example.py and arduino_example.py as a
  MWE pair code files.
*/

#include <pyduino_bridge.h>
#include <Servo.h>

Servo myServo;
byte servoPin = 5;
byte servoMin = 0;
byte servoMax = 180;
byte servoPos = 0;
byte newServoPos = servoMin;
float servoFraction = 0.0; // fraction of servo range to move

const byte numLEDs = 2;
byte ledPin[numLEDs] = {3, 4};
byte buttonPin = 8;
unsigned long LEDinterval[numLEDs] = {50, 50};
unsigned long prevLEDmillis[numLEDs] = {0, 0};

float toneFraction = 0.0;

int newFlashInterval = 0;

//=============

void setup() {

  // initialize the button
  pinMode(buttonPin, INPUT);

  // flash LEDs so we know we are alive
  for (byte n = 0; n < numLEDs; n++) {
    pinMode(ledPin[n], OUTPUT);
    digitalWrite(ledPin[n], HIGH);
  }
  delay(500); // delay() is OK in setup as it only happens once

  for (byte n = 0; n < numLEDs; n++) {
    digitalWrite(ledPin[n], LOW);
  }

  // initialize the servo
  myServo.attach(servoPin);
  myServo.write(servoMin);


  // initialize the serial object
  Serial.begin(115200);
  Serial.println("<Arduino is ready>"); // tell the PC we are ready
}

//Create the Bridge_ino object for communication with Python
Bridge_ino myBridge(Serial);

//=============

void loop() {
  myBridge.curMillis = millis(); 
  myBridge.read();
  
  /*since in Arduino code, arrays are blocks of static memory whose size must be determined at compile time, before the program runs,
    you need to define the number of integer and float values received in the .h and .cpp files of the library
    in pyduino_bridge.h and pyduino_bridge.cpp, change the lines:
    #define numIntValues_FromPy 1
    #define numFloatValues_FromPy 1
    the values in both files must be the same
    */
  const char* ledName = myBridge.headerOfMsg;   //myBridge.headerOfMsg is the header characters received from Python
  int newFlashInterval = myBridge.intsRecvd[0]; //myBridge.intsRecvd is the array containing the integer values received from Python
  servoFraction = myBridge.floatsRecvd[0];      //myBridge.floatsRecvd is the array containing the integer values received from Python
  updateFlashInterval(ledName, newFlashInterval);
  updateServoPos();
  
  //myBridge.writeEcho(); //sends to Python the same values received, and the millis time scaled by a bit shifting.
  //Internally, the following bit shifting is applied: `millis >> rightBitShifter`. This is equal to `millis/pow(2,rightBitShifter)`.
  //By default, Arduino sends to Python the millis value in apprx seconds, since the default bitShifter value is 10.
  //Arduino always sends a time value to Python.
  
  //the line below gives the same result as using myBridge.writeEcho(), since we receive from Python only 1 int and 1 float.
  //myBridge.writeTwoArrays(myBridge.intsRecvd,1,myBridge.floatsRecvd,1);

  //Let's send the int value received, the state of the button, and the float received. Remember that PyDuino Bridge always sends the
  //millis value from arduino, scaled (as was explained above).
  int buttonState = readPin(buttonPin);
  int arrX[] = {myBridge.intsRecvd[0], myBridge.intsRecvd[1], buttonState};
  myBridge.write_HeaderAndTwoArrays(ledName, arrX, 3, myBridge.floatsRecvd, 1);
  flashLEDs();
  moveServo();
}

//============

void updateFlashInterval(const char* ledname, int newFlashInt) {

  // this illustrates using different inputs to call different functions
  if (strcmp(ledname, "LED1") == 0) {
    updateLED1(newFlashInt);
  }

  if (strcmp(ledname, "LED2") == 0) {
    updateLED2(newFlashInt);
  }
}

//=============

void updateLED1(int newFlashInterval) {

  if (newFlashInterval > 80) {// if it is lower, it is too fast to see it well
    LEDinterval[0] = newFlashInterval;
  }
}

//=============

void updateLED2(int newFlashInterval) {

  if (newFlashInterval > 80) {// if it is lower, it is too fast to see it well
    LEDinterval[1] = newFlashInterval;
  }
}

//=============

void flashLEDs() {

  for (byte n = 0; n < numLEDs; n++) {
    if (myBridge.curMillis - prevLEDmillis[n] >= LEDinterval[n]) {
      prevLEDmillis[n] += LEDinterval[n];
      digitalWrite( ledPin[n], ! digitalRead( ledPin[n]) );
    }
  }
}

//=============

int readPin(int pinButton) {
  return digitalRead(pinButton);
}
//=============

void updateServoPos() {

  byte servoRange = servoMax - servoMin;
  if (servoFraction >= 0 && servoFraction <= 1) {
    newServoPos = servoMin + ((float) servoRange * servoFraction);
  }
}

//=============

void moveServo() {
  if (servoPos != newServoPos) {
    servoPos = newServoPos;
    myServo.write(servoPos);
  }
}
