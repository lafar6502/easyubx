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

TEasyUBXError eubx_init(struct eubx_handle * pHandle)
{
	TEasyUBXError rc = EUBX_ERROR_NULLPTR;
	
	if (NULL != pHandle) {
		pHandle->is_initialized = true;
		pHandle->last_error = EUBX_ERROR_OK;
		pHandle->receive_status = EUBXReceiveIdle;
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
			case EUBXReceiveIdle:
				if (EUBX_SYNC1 == byte) {
					pHandle->receive_status = EUBXReceiveSync1;
				}
				break;
				
			case EUBXReceiveSync1:
				break;
				
			case EUBXReceiveSync2:
				break;

			case EUBXReceiveClass:
				break;

			case EUBXReceiveId:
				break;
				
			default:
				pHandle->receive_status = EUBXReceiveIdle;
				break;
		}
		
		pHandle->last_error = EUBX_ERROR_OK;
		rc = pHandle->last_error;
	}
	
	return rc;
}
