# PyDuinoBridge
Library for bidirectional communication between Python and Arduino.

## Instalation

You need to install `pyduinobridge` in both Arduino IDE and Python.

### In Python:
PyDuinoBridge is on the [Python Package Index](https://pypi.org/project/pyduinobridge/ "PyDuinoBridge page on PyPI").
To install the library, use `pip install pyduinobridge`.

The PyDuinoBridge library requires PySerial library. You can install it with `pip install pyserial`.

The library uses Python>=3.6 and has been tested on different platforms (Windows, Raspberry Pi and Jetson Nano). 

### In Arduino:

You need to manually add the files `src/pyduino_bridge.h` and `src/pyduino_bridge.cpp` to your `.../Arduino/libraries` folder.

In your arduino code, add `#include <pyduino_bridge.h>`.

## Examples

Once the libraries are installed, you can use `python_example.py` on the Python device and upload `examples/arduino_example/arduino_example.ino` to your arduino board. The example has been tested on Arduino Nano and UNO.
