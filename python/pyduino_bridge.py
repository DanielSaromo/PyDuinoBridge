# pyduino_bridge.py - PyDuino Bridge: Library for transparent bidirectional
# communication between Python and Arduino. Python File.
# PyDuino Bride uses pyserial library. You can install it with: `pip install pyserial`
###############################################################################################################
# Author: Daniel Saromo. Adapted from Robin2 code.
###############################################################################################################
# Description:
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
#    the message content comprises a string, an integer and a float
#    the numbers are sent as their ascii equivalents
#    for example <LED1,200,0.2>
#    this means set the flash interval for LED1 to 200 millisecs
#      and move the servo to 20% of its range
#
# receiving a message from the Arduino involves
#    waiting until the startMarker is detected
#    saving all subsequent bytes until the end marker is detected
#
# NOTES
#       this program does not include any timeouts to deal with delays in communication
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
# Timeline:
#
# 23 Apr 2020 - DanielSaromo: Created Bridge_py (this Python code) and Bridge_ino (in Arduino code)
# classes to allow high-level use.
###############################################################################################################

import serial # PyDuino requires the serial library. You can install it with `pip install serial`.
import time

class Bridge_py:
    """This class allows transparent bidirectional communication between Python and Arduino."""
    def __init__(self):
        self.intsRecvd = []
        self.floatsRecvd = []
        self.sleepTime = 5.0

    def begin(self, serPort, baudRate, numIntValues_FromPy, numFloatValues_FromPy,\
        openingMsg = "Arduino is ready", startMarker=ord("<"), endMarker=ord(">"), buffSize=40):
        print("Please verify in the header of the Arduino code that it is expecting to receive",numIntValues_FromPy,"integers and",numFloatValues_FromPy,"floats.")
        print("Verify that you are sending from Python that number of elements mentioned above.")
        """Starts the Python-Arduino serial bridge for serial data transmission."""
        self.ser = serial.Serial(serPort, baudRate)
        print ("PyDuino Bridge opened in port " + serPort + "! Baudrate " + str(baudRate)+".")
        self.openingMsg = openingMsg

        self.startMarker = startMarker # 60: is the ASCII code of <
        self.endMarker = endMarker   # 62: is the ASCII code of >
        self.buffSize = buffSize
        self.numIntValues_FromPy = numIntValues_FromPy
        self.numFloatValues_FromPy = numFloatValues_FromPy

        self.waitForArduino()

    def waitForArduino(self):
        """Waits until Arduino sends `openingMsg` (default is: 'Arduino Ready')."""

        # this allows time for Arduino reset. it also ensures that any bytes left
        # over from a previous message are discarded

        msg = ""
        while msg.find(self.openingMsg) == -1:

            while self.ser.inWaiting() == 0:
                pass

            msg = self.read()

            print ("Received from arduino:",msg) # python3 requires parenthesis
            print ()

    def read(self):
        """Receives data from arduino."""

        ck = ""
        x = "z" # any value that is not an end or startMarker
        byteCount = -1 # to allow for the fact that the last increment will be one too many

        # wait for the start character
        while  ord(x) != self.startMarker:
            x = self.ser.read()

        # save data until the end marker is found
        while ord(x) != self.endMarker:
            if ord(x) != self.startMarker:
                ck = ck + x.decode("utf-8") # change for Python3
                byteCount += 1
            x = self.ser.read()

        return(ck)

    def write(self, sendStr):
        """Sends a string to arduino."""
        if not type(sendStr) is str:
            raise TypeError("The message must be a string.")

        assert len(sendStr)<= self.buffSize, "The maximum buffer size (message length) for the string to send to arduino is "+str(self.buffSize)
        self.ser.write(sendStr.encode('utf-8')) # change for Python3

    def writeAndRead_HeaderAndTwoLists(self, header, listIntsFromPython, listFloatsFromPython):
        """Sends to Arduino a list of `self.numIntValues_FromPy` integers and a list of `self.numFloatValues_FromPy` floats.
        Returns from the reply from Arduino: a string header, a list of ints, a list of floats and the `millis` value."""
        assert len(listIntsFromPython)==self.numIntValues_FromPy, "You are expected to send `self.numIntValues_FromPy` integers."
        assert len(listFloatsFromPython)==self.numFloatValues_FromPy, "You are expected to send `self.numIntValues_FromPy` integers."
        
        strListInts = []
        for e in listIntsFromPython:
            if type(e) is int:
                strListInts.append(str(e))
            else:
                raise TypeError("The element "+str(e)+" must be an integer!")

        strListFloats=[]
        for e in listFloatsFromPython:
            if type(e) is float:
                strListFloats.append(str(e))
            else:
                raise TypeError("The element "+str(e)+" must be a float!")

        toSendStr = ",".join([chr(self.startMarker)+str(header)] + strListInts + strListFloats)
        toSendStr += chr(self.endMarker)

        waitingForReply = False
        receivedData = []

        while len(receivedData)==0:

            if waitingForReply == False:
                self.write(toSendStr)
                print ("Sent: " + toSendStr)
                waitingForReply = True

            if waitingForReply == True:

                while self.ser.inWaiting() == 0:
                    pass

                dataRecvd = self.read()
                print ("Reply received: " + dataRecvd)
                receivedData.append(dataRecvd)
                waitingForReply = False

                print ("===========")

        #Until here, `receivedData` is a list with one element: the string received.

        receivedData = receivedData[0]

        numInts = receivedData.count("Int")
        numFloats = receivedData.count("Float")

        receivedData = receivedData.split(" ")
        receivedData.reverse()
        header = receivedData.pop()

        assert header!="Int", "The header must not be \"Int\"!"
        assert header!="Float", "The header must not be \"Float\"!"

        listIntsFromArduino = []
        for _ in range(numInts):
            receivedData.pop()
            listIntsFromArduino.append(int(receivedData.pop()))
        
        listFloatsFromArduino = []
        for _ in range(numFloats):
            receivedData.pop()
            listFloatsFromArduino.append(float(receivedData.pop()))
        
        return header, listIntsFromArduino, listFloatsFromArduino, float(receivedData[0])

    def sleep(self, nap):
        """Waits `nap` seconds. It just uses the `time.sleep` method."""
        time.sleep(nap)

    def setSleepTime(self, sleepTime):
        """Sets the sleep time (in seconds) after each sending and response pair."""
        self.sleepTime = sleepTime

    def writeAndRead_Strings(self, td):#td: transmitted data
        """Sends a list of strings to Arduino and returns the list of string replies."""
        numLoops = len(td)
        waitingForReply = False
        n = 0
        receivedData = []

        while n < numLoops:
            teststr = td[n]

            if waitingForReply == False:
                self.write(teststr)
                print ("Sent (str # " + str(n) + "): " + teststr)
                waitingForReply = True

            if waitingForReply == True:

                while self.ser.inWaiting() == 0:
                    pass

                dataRecvd = self.read()
                print ("Reply received: " + dataRecvd)
                receivedData.append(dataRecvd)
                n += 1
                waitingForReply = False

                print ("===========")

            time.sleep(self.sleepTime)

        return receivedData

    def close(self):
        """Closes the serial bridge between Python and Arduino."""
        self.ser.close
        print("PyDuino Bridge closed!")