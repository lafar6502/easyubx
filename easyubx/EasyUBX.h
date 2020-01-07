/*
 * Main Header file for the Easy UBX Arduino library
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

#include <Stream.h>

#include "easyubx_drv.h"

class EasyUBX {
    public:
        EasyUBX(Stream & stream);
        ~EasyUBX();

        void set_debug_stream(Stream * stream);

        bool begin();
        void loop();

        TEasyUBXChipsetVersion getChiptsetVersion() const;
        const char * getSoftwareVersion() const;
        TEasyUBXDynamicPlatformModel getDynamicPlatformModel() const;
        TEasyUBXFixMode getPositionFixingMode() const;
        uint16_t getMeasurementRate() const;
        uint16_t getNavigationRate() const;

        TEasyUBXError setDynamicPlatformModel(TEasyUBXDynamicPlatformModel dyn_model, TEasyUBXFixMode fix_mode = EUBXFixModeAuto2D3D);

    private:
        static uint16_t receive_buffer_cb(void * usr_ptr, uint8_t * buffer, uint16_t max_length);
        static void send_byte_cb(void * usr_ptr, uint8_t buffer);
        static void send_buffer_cb(void * usr_ptr, const uint8_t * buffer, uint16_t length);
        static void notify_cb(void * usr_ptr, TEasyUBXEvent event);

        uint16_t receive_buffer(uint8_t * buffer, uint16_t max_length);
        void send_byte(uint8_t buffer);
        void send_buffer(const uint8_t * buffer, uint16_t length);
        void notify(TEasyUBXEvent event);

    private:
        bool m_initialized;

        struct eubx_handle m_eubx_handle;
        Stream & m_stream;
        Stream * m_debug_stream;
};
