#include <Servo.h>
#include <Arduino.h>
#include <pyduino_bridge.h>

int pin13 = 13;


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

  pinMode(pin13, OUTPUT);
  digitalWrite(pin13, LOW);


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
  updateFlashInterval();
  updateServoPos();
  myBridge.writeEcho();
  flashLEDs();
  moveServo();
}

//============

void updateFlashInterval() {

  // this illustrates using different inputs to call different functions
  if (strcmp(myBridge.headerFromPC, "LED1") == 0) {
    updateLED1();
  }

  if (strcmp(myBridge.headerFromPC, "LED2") == 0) {
    updateLED2();
  }
}

//=============

void updateLED1() {

  if (myBridge.intsRecvd[0] > 100) {
    LEDinterval[0] = myBridge.intsRecvd[0];
  }
}

//=============

void updateLED2() {

  if (myBridge.intsRecvd[0] > 100) {
    LEDinterval[1] = myBridge.intsRecvd[0];
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
