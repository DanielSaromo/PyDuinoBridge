/*
  pyduino_bridge.cpp - Library for transparent bidirectional
  communication between Python and Arduino. Arduino source file.
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Author: Daniel Saromo. Adapted from Robin2 code.
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Description:

  The code is as minimalist as possible, so that you can keep your
  existing python and arduino and use it with the library.
  Python code tested in: Windows. To test: Jetson Nano, Raspberry Pi and Linux. 
  Created by Daniel Saromo, Apr 23, 2020.
  Released into the public domain.

  Adapted from Robin2 code: https://forum.arduino.cc/index.php?topic=225329.msg1810764#msg1810764
  Robin2 modification for Python3: https://forum.arduino.cc/index.php?topic=566897.0
*/

//#include "Arduino.h"
#include "pyduino_bridge.h"

Bridge_ino::Bridge_ino(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Bridge_ino::dot()
{
  digitalWrite(_pin, HIGH);
  delay(250);
  digitalWrite(_pin, LOW);
  delay(250);  
}

void Bridge_ino::dash()
{
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(250);
}