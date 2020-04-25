# PyDuinoBridge [![Build Status](https://travis-ci.com/DanielSaromo/PyDuinoBridge.svg?branch=master)](https://travis-ci.com/DanielSaromo/PyDuinoBridge) ![PyPI](https://img.shields.io/pypi/v/pyduinobridge)
Library for bidirectional communication between Python and Arduino.

Connect Python and Arduino as easily as sending a list of variables from one platform and receiving it on the other one. With PyDuino Bridge, you can bi-directionally send a string, a list of integers, and a list of floats. Thus, you can bring the Python computational power and flexibility to the real-world interfacing capability of Arduino.

The library is built on the [code](https://forum.arduino.cc/index.php?topic=566897.0) given by Robin2 in the Arduino forum, but simplified to allow a high-level use.

## Instalation

You need to install `pyduinobridge` in both Arduino IDE and Python.

### In Python:
PyDuinoBridge is on the [Python Package Index](https://pypi.org/project/pyduinobridge/ "PyDuinoBridge page on PyPI").
To install the library, install `pip` and then use the command `pip install pyduinobridge`.

The PyDuinoBridge library requires PySerial library. You can install it with the command `pip install pyserial`.

The library uses Python>=3.6 and has been tested on different platforms (Windows, Raspberry Pi, and Jetson Nano).

#### \[Linux\] Port opening error due to permission requirements:

To solve this error, follow [this tutorial](https://www.arduino.cc/en/guide/linux#toc6) from the Arduino official page. Remember that you can log out of your Linux session with the command `gnome-session-quit`. This error occurs when installing the library on the **Jetson Nano** board.

### In Arduino:

You need to manually add the files `src/pyduino_bridge.h` and `src/pyduino_bridge.cpp` to your `.../Arduino/libraries` folder.

To use the library in your Arduino code, add `#include <pyduino_bridge.h>` in the header.


## Example of use

Once the libraries are installed, execute`python_example.py` on the Python device.

Also, upload `examples/arduino_example/arduino_example.ino` to your Arduino board. Connect the devices physically through their serial port (e.g., USB). The example has been tested on Arduino Nano and UNO boards.
