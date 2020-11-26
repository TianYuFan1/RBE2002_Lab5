# wpi-32u4-library

Version: 2.1.0<br/>
Release date: 2020-09-06<br/>
Forked from [www.pololu.com](https://www.pololu.com/)

# Doxygen Documentation

https://wpiroboticsengineering.github.io/wpi-32u4-library/annotated.html

## Summary

This is a C++ library for the Arduino IDE that helps access the on-board hardware of the [Romi 32U4 Control Board](https://www.pololu.com/product/3544).

The Romi 32U4 Control Board turns the Romi chassis into a programmable, Arduino-compatible robot.  It has an integrated AVR ATmega32U4 microcontroller, motor drivers, encoders, buzzer, buttons, and an LSM6DS33 accelerometer and gyro.

This library includes code for accessing the LSM6DS33, forked from the separate [LSM6 library](https://github.com/pololu/lsm6-arduino).

## Installing the library (platformio)

Add the following lines to your platformio.ini file:

~~~{.cpp}
lib_deps = 
     Wire
     wpiroboticsengineering/wpi-32u4-library @ 2.1.0
~~~

You're done.

(You may or may not need to add Wire -- it can't hurt if you do.)

For later releases, change the last value appropriately. If you're feeling brave, drop the version number and get the latest dev version (and let us know f any problems that arise so we can fix them).

## Installing the library (Arduino)

If you are using version 1.6.2 or later of the [Arduino software (IDE)](http://www.arduino.cc/en/Main/Software), you can use the Library Manager to install this library:

1. In the Arduino IDE, open the "Sketch" menu, select "Include Library", then "Manage Libraries...".
2. Search for "wpi-32u4-library".
3. Click the entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the latest release from this page and decompress it.
2. Rename the folder "wpi-32u4-library", if needed
3. Move the "wpi-32u4-library" folder into the "libraries" directory inside your Arduino sketchbook directory. You can view your sketchbook location by opening the "File" menu and selecting "Preferences" in the Arduino IDE.  If there is not already a "libraries" folder in that location, you should make the folder yourself.
4. After installing the library, restart the Arduino IDE.

## Examples (platformio)

Accessing exmaples in platformio is a little trickier than Arduino. Because platformio grabs libraries dynamically, by default, you don't install the libraries in a static directory on your machine. None of these options are great, but you have a few:

 * Copy-paste from the dynamic library. Start a new project with a skeleton main.cpp. Include the library as describe above in Installing the Library. Build the skeleton project. Within your project tree, navigate to .pio/lib_deps/examples and copy-paste the relevant example into your main.cpp. You can have multiple projects (folders) in a single workspace, which might make accessing additional libraries easier.
 * Clone the repo to your machine. Be sure to clone the latest release, otherwise you will get a dev version. It is still better to reference the library dynamically in your platformio.ini file (see above under Installing the Library).
 * Copy-paste from the github repo. A little dangerous, since you might get an example that relies on later commits.
 * Install the library (say, using Arduino) and then use the "Import Arduino Project " option in platformio. Do *not* check the "Use libraries installed by Arduino" option.

## Examples (Arduino)

Several example sketches are available that show how to use the library. You can access them from the Arduino IDE by opening the "File" menu, selecting "Examples", and then selecting "wpi-32u4-library". If you cannot find these examples, the library was probably installed incorrectly and you should retry the installation instructions above.

### Not all examples have been converted to the wpi version of the library. At the moment we've updated the following:

* MotorTest
* Encoders
* IMU
* RotationResist (this one is fun!)
* RemoteControl will compile, but has not been tested
* Buttons
* Blink will likely run, but is untested

## Classes and functions

The main classes and functions provided by the library are listed below:

* Romi32U4ButtonA
* Romi32U4ButtonB
* Romi32U4ButtonC
* ~~Romi32U4Buzzer~~
* Romi32U4Encoders
* ~~Romi32U4LCD~~
* Romi32U4Motors
* ledRed()
* ledGreen()
* ledYellow()
* usbPowerPresent()
* readBatteryMillivolts()

## Component libraries

This library also includes copies of several other Arduino libraries inside it which are used to help implement the classes and functions above.

* [FastGPIO](https://github.com/pololu/fastgpio-arduino)
* ~~[PololuBuzzer]~~(https://github.com/pololu/pololu-buzzer-arduino)
* ~~[PololuHD44780]~~(https://github.com/pololu/pololu-hd44780-arduino)
* [Pushbutton](https://github.com/pololu/pushbutton-arduino)
* [QTRSensors](https://github.com/pololu/qtr-sensors-arduino)
* [USBPause](https://github.com/pololu/usb-pause-arduino)

You can use these libraries in your sketch automatically without any extra installation steps and without needing to add any extra `#include` lines to your sketch.

You should avoid adding extra `#include` lines such as `#include <Pushbutton.h>` because then the Arduino IDE might try to use the standalone Pushbutton library (if you previously installed it), and it would conflict with the copy of the Pushbutton code included in this library. The only `#include` line needed to access all features of this library are:

~~~{.cpp}
#include <Romi32U4.h>
~~~

## Documentation

Documentation found at https://wpiroboticsengineering.github.io/wpi-32u4-library/index.html. We are working on updating documentation for the changes to the forked Pololu library. The biggest change is to the motor class, where

~~~{.cpp}
Romi32U4Motors::setSpeeds()
~~~

has been changed to 

~~~{.cpp}
Romi32U4Motors::setEfforts()
~~~

since that better represents the behavior of that function.

Some other library files (LCD, buzzer) have been removed, since they conflict with some of the changes in the background (mostly to timers).

## Version history

* 2.1.0 (2020-09-06): Primary release for WPI courses.
* 1.0.2 (2017-07-17): Fixed a bug that caused errors for the right encoder to be reported as errors for the left encoder.
* 1.0.1 (2017-02-23):
  * Changed the internal `Romi32U4Motors::maxSpeed` variable to be an `int16_t` so it can be compared to other `int16_t` variables without warnings.
  * Fixed the InterialSensors and Demo examples to not use a compass.
  * Fixed some comments.
* 1.0.0 (2017-02-06): Original release.
