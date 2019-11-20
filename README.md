# Easy-UBX

Easy-UBX is a C library to utilize the UBX binary protocol for the u-blox GPS receiver like NEO-6M and others. These GPS devices are often used on embedded IoT devices.  

This library is a portable implementation in C and does not depend on any serial port implementation.

Features currently under development:

* Read GPS Version information
* Read and change port settings
* Configure protocol parameter for NMEA and UBX protocol
* Change settins like navigation mode and update frequency

Features which may be implemented at some later point:

* Library implementation for Arduino
* C++ Wrapper for Arduino and other platforms
* command line tool for linux
* reading position information to eliminate need for a NMEA library
