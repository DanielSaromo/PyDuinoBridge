/*
  pyduino_bridge.h - Library for transparent bidirectional
  communication between Python and Arduino. Arduino header file.
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Author: Daniel Saromo. Adapted from Robin2 code.
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Description:

  The Arduino board expects a message with three parts: A header, a group of ints (separated with commas),
  and a group of floats (separated with commas).
  The message to be sent to the Arduino starts with < and ends with > (startMarker and endMarker default values).
  For instance:

  <LED1,100,300,123,1.1,2.3,2.4>

  The code is as minimalist as possible, so that you can keep your
  existing python and arduino and use it with the library.
  Python code tested in: Windows, Jetson Nano and Raspberry Pi.
  Created by Daniel Saromo, Apr 23, 2020.
  Released into the public domain.

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

  Adapted from Robin2 code: https://forum.arduino.cc/index.php?topic=225329.msg1810764#msg1810764
  Robin2 modification for Python3: https://forum.arduino.cc/index.php?topic=566897.0
*/

#define buffSize 40
#define numIntValues_FromPy 1   //Number of integer values that are going to be in the message
#define numFloatValues_FromPy 1 //Number of float   values that are going to be in the message

#ifndef pyduino_bridge_h
#define pyduino_bridge_h

#include "Arduino.h"


class Bridge_ino
{
  public:
    Bridge_ino(Stream &serial);
    //const int buffSize;
    char inputBuffer[buffSize];
    char inputBuffer_original[buffSize];
    const char startMarker = '<';
    const char endMarker = '>';

    int intsRecvd[numIntValues_FromPy];
    float floatsRecvd[numFloatValues_FromPy];

    int xd_int = 123;
    float xd_float = 1.23;

    byte rightBitShifter = 10;//10: makes the divided millis value to be approx seconds

    byte bytesRecvd = 0;
    boolean readInProgress = false;
    boolean newDataFromPC = false;
    char headerOfMsg[buffSize] = {0};
    unsigned long curMillis;
    unsigned long prevReplyToPCmillis = 0;
    unsigned long replyToPCinterval = 1000;
    void read();
    void writeEcho();
    void write_HeaderAndTwoArrays(char* header, int arrInts[], int numInts, float arrayFloats[], int numFloats);
    void parseData();
    void setMillisBitShifter(byte newRightBitShifter);
    Stream& ser;
  private:
    //int _pin;
};

#endif
