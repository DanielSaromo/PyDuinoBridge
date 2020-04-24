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
  Python code tested in: Windows. To test: Jetson Nano, Raspberry Pi and Linux. 
  Created by Daniel Saromo, Apr 23, 2020.
  Released into the public domain.

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

    byte millisDivider = 10;//10: makes the divided millis value to be approx seconds

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
    void setMillisDivider(byte newDivider);
    Stream& ser;
  private:
    //int _pin;
};

#endif
