/*
  pyduino_bridge.h - Library for transparent bidirectional
  communication between Python and Arduino. Arduino header file.
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
#ifndef pyduino_bridge_h
#define pyduino_bridge_h

//#include "Arduino.h"

class Bridge_ino
{
  public:
    Bridge_ino(int pin);
    void dot();
    void dash();
  private:
    int _pin;
};

#endif