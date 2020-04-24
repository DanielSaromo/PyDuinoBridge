#include <Servo.h>
#include <Arduino.h>
#include <pyduino_bridge.h>


Servo myServo;
byte servoPin = 8;
byte servoMin = 10;
byte servoMax = 170;
byte servoPos = 0;
byte newServoPos = servoMin;

const byte numLEDs = 2;
byte ledPin[numLEDs] = {2, 13};
unsigned long LEDinterval[numLEDs] = {50, 50};
unsigned long prevLEDmillis[numLEDs] = {0, 0};

int newFlashInterval = 0;
float servoFraction = 0.0; // fraction of servo range to move

//=============

void setup() {
  //Daniel zone
#define depurador1 4
#define depurador2 5
#define depurador3 6
#define depurador4 7


  pinMode(depurador1, OUTPUT);
  digitalWrite(depurador1, LOW);


  pinMode(depurador2, OUTPUT);
  digitalWrite(depurador2, LOW);


  pinMode(depurador3, OUTPUT);
  digitalWrite(depurador3, LOW);


  pinMode(depurador4, OUTPUT);
  digitalWrite(depurador4, LOW);

  //End daniel zone

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
  moveServo();

  // tell the PC we are ready

  Serial.begin(115200);
  Serial.println("<Arduino is ready>");


}
Bridge_ino myBridge(Serial);

//=============

void loop() {
  myBridge.curMillis = millis();
  myBridge.read();
  servoFraction = myBridge.floatsRecvd[0];
  int newFlashInterval = myBridge.intsRecvd[0];
  const char* ledName = myBridge.headerOfMsg;
  updateFlashInterval(ledName, newFlashInterval);
  updateServoPos();
  myBridge.writeEcho();
  //the line below gives the same result as using myBridge.writeEcho(), since we receive only 1 int and 1 float.
  //myBridge.writeTwoArrays(myBridge.intsRecvd,1,myBridge.floatsRecvd,1);
  //int arrX[] = {1,3,5,7,9};
  //myBridge.writeTwoArrays(arrX, 5, myBridge.floatsRecvd,1);
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

  if (newFlashInterval > 80) {
    LEDinterval[0] = newFlashInterval;
  }
}

//=============

void updateLED2(int newFlashInterval) {

  if (newFlashInterval > 80) {
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
