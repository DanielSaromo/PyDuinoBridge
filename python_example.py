# python_example.py - PyDuino Bridge: Library for transparent bidirectional
# communication between Python and Arduino. Python Example Code.
###############################################################################################################
# Author: Daniel Saromo. Adapted from Robin2 code.
###############################################################################################################
# Description:
#
# The purpose of this program and the associated Arduino program is to demonstrate a system for sending
#  and receiving data between a PC and an Arduino.
#
# The key functions are:
#    write(str) which sends the given string to the Arduino. The string may
#                       contain characters with any of the values 0 to 255
#
#    read()  which returns an array.
#                         The first element contains the number of bytes that the Arduino said it included in
#                             message. This can be used to check that the full message was received.
#                         The second element contains the message as a string
#
#
# The overall process followed by the demo program is as follows
#   open the serial connection to the Arduino - which causes the Arduino to reset
#   wait for a message from the Arduino to give it time to reset
#   loop through a series of test messages
#      send a message and display it on the PC screen
#      wait for a reply and display it on the PC
#
# to facilitate debugging the Arduino code this program interprets any message from the Arduino
#    with the message length set to 0 as a debug message which is displayed on the PC screen
#
# the message to be sent to the Arduino starts with < and ends with > (startMarker and endMarker
# default values)
#    the message content comprises a string, an list of integers and a list of floats
#    the numbers are sent as their ascii equivalents
#    for example: <LED1,200,0.2>    has: numIntValues_FromPy=1, numFloatValues_FromPy=1
#    this means set the flash interval for LED1 to 200 millisecs
#      and move the servo to 20% of its range
#
# receiving a message from the Arduino involves
#    waiting until the startMarker is detected
#    saving all subsequent bytes until the end marker is detected
#
# NOTES
#       this program allows to include a sleep time after each string from the list of strings is
#       send, when using the function `writeAndRead_Strings`. If the sleep time is not desired,
#       it can be set to zero by calling `setSleepTime(0)` before calling the function
#       `writeAndRead_Strings`.
#
#       for simplicity the program does NOT search for the comm port - the user must modify the
#         code to include the correct reference.
#         search for the lines
#               serPort = "/dev/ttyS80"
#               baudRate = 9600
#               ser = serial.Serial(serPort, baudRate)
#
# The code is as minimalist as possible, so that you can keep your
# existing python and arduino and use it with the library.
# Adapted from Robin2 code: https://forum.arduino.cc/index.php?topic=225329.msg1810764#msg1810764
# Robin2 modification for Python3: https://forum.arduino.cc/index.php?topic=566897.0
#
###############################################################################################################
#
# NOTE the user must ensure that the serial port and baudrate are correct.
# Depending on your OS, use one of these serial port name types
# serPort = 'COM17'
# serPort = "/dev/ttyS80"
# serPort = "/dev/ttyACM0"
#
# According to https://www.arduino.cc/en/Serial.Begin, default Arduino baud rates are:
# 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, or 115200.
#
# Timeline:
#
# 19 Jul 2014 - Robin2: Original code for Python 2
# 08 Dec 2016 - Robin2: Updated for Python3
# 23 Apr 2020 - DanielSaromo: Created Bridge_py (in Python code) and Bridge_ino (in Arduino code)
# classes to allow high-level use.
# 24 Apr 2020 - Daniel Saromo: Created python_example.py and arduino_example.py as a
#  MWE pair code files.

serPort = 'COM12'
baudRate = 115200

from pyduinobridge import Bridge_py
myBridge = Bridge_py()
myBridge.begin(serPort, baudRate, numIntValues_FromPy=1, numFloatValues_FromPy=1)
myBridge.setSleepTime(4)

testData = []
testData.append("<LED1,200,0.5>")
testData.append("<LED2,800,1.0>")
testData.append("<LED1,1000,0.0>")
testData.append("<LED2,100,1.0>")
testData.append("<LED1,100,0.0>")
testData.append("<LED2,1000,1.0>")

# When using this function, the program sends a list of strings and receives a list of strings.
dataFromArduino = myBridge.writeAndRead_Strings(testData)

# When using this function, the program sends a header, a list of ints and a list of floats.
dataFromArduino2 = myBridge.writeAndRead_HeaderAndTwoLists("LED1", [800], [0.])
# The values received are
header_FromArdu, listInts_FromArdu, listFloats_FromArdu, millis_FromArdu = dataFromArduino2

myBridge.close()
