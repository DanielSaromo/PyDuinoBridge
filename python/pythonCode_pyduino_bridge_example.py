#this code works with the library `pyduino_bridge.py`
#======================================

# NOTE the user must ensure that the serial port and baudrate are correct
# depending on your OS, use one of these serial port name types
# serPort = 'COM17'
# serPort = "/dev/ttyS80"
# serPort = "/dev/ttyACM0"

# According to https://www.arduino.cc/en/Serial.Begin, default Arduino baud rates are:
# 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, or 115200.

serPort = 'COM17'
baudRate = 115200

from pyduino_bridge import Bridge_py
myBridge = Bridge_py()
myBridge.begin(serPort, baudRate)
myBridge.setSleepTime(2)

testData = []
testData.append("<LED1,200,0.2>")
testData.append("<LED1,800,0.7>")
testData.append("<LED2,800,0.5>")
testData.append("<LED2,200,0.2>")
testData.append("<LED1,200,0.7>")

dataFromArduino = myBridge.writeAndRead_Strings(testData)

#intsFromArduino, floatsFromArduino = myBridge.writeAndRead_TwoLists(listIntsFromPython, listFloatsFromPython)

myBridge.close()