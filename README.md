# PyDuino Bridge [![Build Status](https://travis-ci.com/DanielSaromo/PyDuinoBridge.svg?branch=master)](https://travis-ci.com/DanielSaromo/PyDuinoBridge) ![PyPI](https://img.shields.io/pypi/v/pyduinobridge)
Library for bi-directional communication between Python and Arduino.

Connect Python and Arduino as easily as sending a list of variables from one platform and receiving it on the other one. With PyDuino Bridge, you can bi-directionally send a string, a list of integers, and a list of floats. Thus, you can bring the Python computational power and flexibility to the real-world interfacing capability of Arduino.

The library is built on the [code](https://forum.arduino.cc/index.php?topic=566897.0) posted in the Arduino forum by Robin2, who gave me his permission to use it. The main objective of PyDuino Bridge is to allow a high-level and straightforward connection between Python and Arduino.

## Instalation

You need to install `pyduinobridge` in both Arduino IDE and Python.

### In Python:
PyDuino Bridge is on the [Python Package Index](https://pypi.org/project/pyduinobridge/ "PyDuinoBridge page on PyPI").
To install the library, first install `pip` and then use the command `pip install pyduinobridge`.

The PyDuino Bridge library requires PySerial library. You can install it with the command `pip install pyserial`.

The library uses Python>=3.6 and has been tested on different platforms (Windows, Ubuntu, Raspberry Pi, and Jetson Nano).

To use the library in your Python code, use the command `from pyduinobridge import Bridge_py`.

#### \[Linux\] Port opening error due to permission requirements:

To solve this error, follow [this tutorial](https://www.arduino.cc/en/guide/linux#toc6) from the Arduino official page. Remember that you can log out of your Linux session with the command `gnome-session-quit`.

If the problem persists, you may execute the command `sudo chmod 666 /dev/ttyACM0` (as suggested [here](https://stackoverflow.com/questions/27858041/oserror-errno-13-permission-denied-dev-ttyacm0-using-pyserial-from-pyth)). For the example command given, I assume that your Arduino board is connected to the port `ttyACM0`.

### In Arduino:

PyDuino Bridge is on the official Arduino Library Manager. Just type `pyduino bridge` in the search box and install the library. You can follow [the official Arduino tutorial](https://www.arduino.cc/en/guide/libraries).

Optionally, you can manually add the files `src/pyduino_bridge.h` and `src/pyduino_bridge.cpp` to your `.../Arduino/libraries/` folder.

To use the library in your Arduino code, add `#include <pyduino_bridge.h>` in the header.


## Example of use

Once the libraries are installed, execute`python_example.py` on the Python device.

Also, upload `examples/arduino_example/arduino_example.ino` to your Arduino board. Connect the devices physically through their serial port. You can use a USB cable or maybe a serial Bluetooth module to achieve wireless connectivity. The example code has been tested on Arduino Nano and UNO boards with Python running on Windows, Ubuntu, Raspberry Pi, and Jetson Nano.

## Configuration considerations

By default, the maximum number of characters allowed in the messages between Arduino and Python is 40. If you want to change this buffer size, you have to put the same value in both the Python (change the value `buffSize` when using the `begin` method) and the Arduino code (change the line `#define buffSize 40` in the `pyduino_bridge.h` and `pyduino_bridge.cpp` files).

Also, verify that the same `numIntValues_FromPy` and `numFloatsValues_FromPy` values are configured in your Python code according to the Arduino library files (before uploading, change the lines `#define numIntValues_FromPy 1` and `#define numFloatValues_FromPy 1` in the `pyduino_bridge.h` and `pyduino_bridge.cpp` files). By default, PyDuino Bridge is configured to send and receive a list of one integer and a list of one float (besides the header and the time information).
