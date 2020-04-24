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

#define buffSize 40
#define numIntValues_FromPy 1   //Number of integer values that are going to be in the message from Python
#define numFloatValues_FromPy 1 //Number of float   values that are going to be in the message from Python

#include "Arduino.h"
#include "pyduino_bridge.h"

Bridge_ino::Bridge_ino(Stream &serial): ser(serial){
  curMillis = millis();
}

void Bridge_ino::read() {
  // receive data from PC and save it into inputBuffer

  if (ser.available() > 0) {
    char x = ser.read();

    // the order of these IF clauses is significant

    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      strcpy(inputBuffer_original, inputBuffer);
      parseData();
    }

    if (readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) {
      bytesRecvd = 0;
      readInProgress = true;
    }
  }
}

void Bridge_ino::parseData() {

  // split the data into its parts

  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(inputBuffer, ","); // get the first part - the string
  strcpy(headerFromPC, strtokIndx); // copy it to headerFromPC  

  /*
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  intsRecvd[0] = atoi(strtokIndx); // convert this part to an integer

  strtokIndx = strtok(NULL, ",");
  floatsRecvd[0] = atof(strtokIndx);     // convert this part to a float
  */
  
  for (int i = 0; i < numIntValues_FromPy; i++){
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off  
    intsRecvd[i]=atoi(strtokIndx);// convert this part to an integer
  }

  for (int i = 0; i < numFloatValues_FromPy; i++){
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off  
    floatsRecvd[i]=atof(strtokIndx);// convert this part to a float
  }
}

void Bridge_ino::writeEcho() {

  if (newDataFromPC) {
    newDataFromPC = false;
    ser.print("<");
    ser.print(headerFromPC);
    ser.print(" ");
    
    for (int i = 0; i < numIntValues_FromPy; i++){
      ser.print("Int#");
      ser.print(i);
      ser.print(" ");
      ser.print(intsRecvd[i]);
      ser.print(" ");
    }

    for (int i = 0; i < numFloatValues_FromPy; i++){
      ser.print("Float#");
      ser.print(i);
      ser.print(" ");
      ser.print(floatsRecvd[i]);
      ser.print(" ");
    }

    //ser.print(" curMillis ");
    //ser.print(curMillis);
    //ser.print(" inBuf ");
    //ser.print(inputBuffer_original);

    ser.print("Time ");
    ser.print(curMillis >> 10);  // divide by 512 is approx = half-seconds.
    //the value there was 9. changing it to 10 results in approx = seconds.
    ser.println(">");
  }
}

void Bridge_ino::writeTwoArrays(int arrInts[], int numInts, float arrayFloats[], int numFloats){
  
  if (newDataFromPC) {
    newDataFromPC = false;
    ser.print("<");
    ser.print(headerFromPC);
    ser.print(" ");
    
    for (int i = 0; i < numInts; i++){
      ser.print("Int#");
      ser.print(i);
      ser.print(" ");
      ser.print(arrInts[i]);
      ser.print(" ");
    }

    for (int i = 0; i < numFloats; i++){
      ser.print("Float#");
      ser.print(i);
      ser.print(" ");
      ser.print(arrayFloats[i]);
      ser.print(" ");
    }

    //ser.print(" curMillis ");
    //ser.print(curMillis);
    //ser.print(" inBuf ");
    //ser.print(inputBuffer_original);

    ser.print("Time ");
    ser.print(curMillis >> 10);  // divide by 512 is approx = half-seconds.
    //the value there was 9. changing it to 10 results in approx = seconds.
    ser.println(">");
  }
}
