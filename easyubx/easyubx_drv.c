/*
 * Main Source file for the Easy UBX C library
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

#include <stddef.h>

#include "easyubx_drv.h"

static void handle_receive_message(struct eubx_handle * pHandle);

TEasyUBXError eubx_init(struct eubx_handle * pHandle)
{
	TEasyUBXError rc = EUBX_ERROR_NULLPTR;
	
	if (NULL != pHandle) {
		pHandle->is_initialized = true;
		pHandle->last_error = EUBX_ERROR_OK;
		pHandle->receive_status = EUBXReceiveExpectSync1;
    pHandle->receive_class = 0;
    pHandle->receive_id = 0;
    pHandle->receive_length = 0;
    pHandle->receive_position = 0;
    pHandle->receive_ck_a = 0;
    pHandle->receive_ck_b = 0;
		pHandle->send_status = EUBXSendIdle;
		
		rc = pHandle->last_error;
	}
	
	return rc;
}

TEasyUBXError eubx_receive_byte(struct eubx_handle * pHandle, uint8_t byte)
{
	TEasyUBXError rc = EUBX_ERROR_NULLPTR;
	
	if (NULL != pHandle) {
    if (pHandle->is_initialized) {
      rc = EUBX_ERROR_OK;
    }
    else {
      rc = EUBX_ERROR_NOT_INITIALIZED;
    }
	}

 if (EUBX_ERROR_OK == rc) {
		switch (pHandle->receive_status) {
			case EUBXReceiveExpectSync1:
				if (EUBX_SYNC1 == byte) {
					pHandle->receive_status = EUBXReceiveExpectSync2;
				}
				break;
				
			case EUBXReceiveExpectSync2:
        if (EUBX_SYNC2 == byte) {
          pHandle->receive_status = EUBXReceiveExpectClass;
        }
        else {
          pHandle->receive_status = EUBXReceiveExpectSync1;
        }
				break;
				
			case EUBXReceiveExpectClass:
        pHandle->receive_class = byte;
        pHandle->receive_status = EUBXReceiveExpectId;
				break;

			case EUBXReceiveExpectId:
        pHandle->receive_id = byte;
        pHandle->receive_status = EUBXReceiveExpectLength1;
				break;

			case EUBXReceiveExpectLength1:
        pHandle->receive_length = byte;
        pHandle->receive_status = EUBXReceiveExpectLength2;
				break;
				
      case EUBXReceiveExpectLength2:
        pHandle->receive_length = pHandle->receive_length + (256 * (uint16_t)byte);
        if (0 == pHandle->receive_length) {
          pHandle->receive_status = EUBXReceiveExpectCKA;
        }
        else {
          pHandle->receive_status = EUBXReceiveExpectContent;
          pHandle->receive_position = 0;
		    }
        break;
        
      case EUBXReceiveExpectContent:
        if (UBX_RECEIVE_BUFFER_SIZE >= pHandle->receive_position) {
          pHandle->last_error = EUBX_ERROR_RECEIVE_OVERFLOW;
        }
        else {
          pHandle->receive_buffer[pHandle->receive_position] = byte;
        }
        pHandle->receive_position += 1;
        if (pHandle->receive_position == pHandle->receive_length) {
          pHandle->receive_status = EUBXReceiveExpectCKA;
        }
        break;

      case EUBXReceiveExpectCKA:
        pHandle->receive_ck_a = byte;
        pHandle->receive_status = EUBXReceiveExpectCKB;
        break;

      case EUBXReceiveExpectCKB:
        pHandle->receive_ck_b = byte;
        handle_receive_message(pHandle);
        pHandle->receive_status = EUBXReceiveExpectSync1;
        break;
        
			default:
				pHandle->receive_status = EUBXReceiveExpectSync1;
				break;
		}
		
		pHandle->last_error = EUBX_ERROR_OK;
		rc = pHandle->last_error;
	}
	
	return rc;
}

void handle_receive_message(struct eubx_handle * pHandle)
{
}
