/*
 * Main Source file for the Easy UBX Arduino library
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

#include "EasyUBX.h"

EasyUBX::EasyUBX(Stream & stream) :
  m_stream(stream)
{
  m_debug_stream = nullptr;
  m_initialized = false;
}

EasyUBX::~EasyUBX()
{ 
}

void EasyUBX::begin()
{
  if (EUBX_ERROR_OK == eubx_init(&m_eubx_handle)) {
    eubx_set_callback_functions(&m_eubx_handle, receive_buffer_cb, send_byte_cb, send_buffer_cb, notify_cb, this);
    m_initialized = true;

    eubx_poll_mon_gnss_selection(&m_eubx_handle);
    eubx_poll_mon_version(&m_eubx_handle);
    eubx_poll_cfg_port(&m_eubx_handle);
  }
}

void EasyUBX::set_debug_stream(Stream * stream)
{
  m_debug_stream = stream;
}

void EasyUBX::loop()
{
  if (m_initialized) {
    eubx_loop(&m_eubx_handle);
  }
}

uint16_t EasyUBX::receive_buffer_cb(void * usr_ptr, uint8_t * buffer, uint16_t max_length)
{
  return static_cast<EasyUBX *>(usr_ptr)->receive_buffer(buffer, max_length);
}

void EasyUBX::send_byte_cb(void * usr_ptr, uint8_t buffer)
{
  static_cast<EasyUBX *>(usr_ptr)->send_byte(buffer);
}

void EasyUBX::send_buffer_cb(void * usr_ptr, const uint8_t * buffer, uint16_t length)
{
  static_cast<EasyUBX *>(usr_ptr)->send_buffer(buffer, length);
}

void EasyUBX::notify_cb(void * usr_ptr, TEasyUBXEvent event)
{
  static_cast<EasyUBX *>(usr_ptr)->notify(event);
}

uint16_t EasyUBX::receive_buffer(uint8_t * buffer, uint16_t max_length)
{
  uint16_t received = 0;

  while (m_stream.available() && (received < max_length)) {
    buffer[received++] = m_stream.read();
  }

  return received;
}

void EasyUBX::send_byte(uint8_t buffer)
{
  m_stream.write(buffer);
}

void EasyUBX::send_buffer(const uint8_t * buffer, uint16_t length)
{
  m_stream.write(buffer, length);
}

void EasyUBX::notify(TEasyUBXEvent event)
{
  if (NULL != m_debug_stream) {
    m_debug_stream->print("Receives Callback=");
    m_debug_stream->print(event);
    m_debug_stream->print(" length=");
    m_debug_stream->print(m_eubx_handle.receive_message.message_length);
    m_debug_stream->print(" error=");
    m_debug_stream->print(m_eubx_handle.last_error);
    m_debug_stream->print(" class=");
    m_debug_stream->print(m_eubx_handle.receive_message.message_class, HEX);
    m_debug_stream->print(" id=");
    m_debug_stream->println(m_eubx_handle.receive_message.message_id, HEX);

    switch (event) {
      case EUBXReceivedCfgPRT:
        m_debug_stream->print("Port=");
        m_debug_stream->print(m_eubx_handle.receive_message.message_buffer[0]);
        m_debug_stream->print(" InMask=");
        m_debug_stream->print(m_eubx_handle.receive_message.message_buffer[12], HEX);
        m_debug_stream->print(m_eubx_handle.receive_message.message_buffer[13], HEX);
        m_debug_stream->print(" OutMask=");
        m_debug_stream->print(m_eubx_handle.receive_message.message_buffer[14], HEX);
        m_debug_stream->print(m_eubx_handle.receive_message.message_buffer[15], HEX);
        break;

      case EUBXReceivedMonVersion:
        m_debug_stream->print("Hardware=");
        m_debug_stream->print(m_eubx_handle.receiver_info.chipset_version);
        m_debug_stream->print(" Software=");
        m_debug_stream->println(m_eubx_handle.receiver_info.software_version);
        break;
    }
  }
}
