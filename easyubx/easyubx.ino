/*
 * Test Program for the Easy UBX Arduino library
 */
 
/*
   MIT License

  Copyright (c) 2019 Bernd Wiegmann (bernd@iotinsights.de)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <HardwareSerial.h>
#include <WiFi.h>

#include "EasyUBX.h"

#define GPS_SERIAL_DEVICE       1
#define GPS_TX                  12
#define GPS_RX                  15

HardwareSerial gps_serial(GPS_SERIAL_DEVICE);
EasyUBX easy_ubx(gps_serial);

void setup() {
  Serial.begin(115200);
  Serial.println(F("EasyUBX Test program 2019-12-15"));

  gps_serial.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX);
  gps_serial.setTimeout(2);
  gps_serial.flush();

  easy_ubx.set_debug_stream(&Serial);

  easy_ubx.begin();
}

void loop() {
  while (1) {
    easy_ubx.loop();
  }
}
