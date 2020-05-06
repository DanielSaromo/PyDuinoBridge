# python_test.py - PyDuino Bridge: Library for transparent bidirectional
# communication between Python and Arduino. Python Example Code.
###############################################################################################################
# Author: Daniel Saromo.
###############################################################################################################
# Description:
#
# This code tests if the library was imported correctly.
#
###############################################################################################################

from pyduinobridge import Bridge_py
print(pyduinobridge.__version__)
myBridge = Bridge_py()
