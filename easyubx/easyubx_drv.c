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
static void handle_receive_class_nav(struct eubx_handle * pHandle);
static void handle_receive_class_rxm(struct eubx_handle * pHandle);
static void handle_receive_class_inf(struct eubx_handle * pHandle);
static void handle_receive_class_ack(struct eubx_handle * pHandle);
static void handle_receive_class_cfg(struct eubx_handle * pHandle);
static void handle_receive_class_mon(struct eubx_handle * pHandle);
static void handle_receive_class_aid(struct eubx_handle * pHandle);
static void handle_receive_class_tim(struct eubx_handle * pHandle);
static void handle_receive_class_log(struct eubx_handle * pHandle);
static void calculate_checksum(const struct eubx_message * message, uint8_t * ck_a, uint8_t * ck_b);

TEasyUBXError eubx_init(struct eubx_handle * pHandle)
{
	TEasyUBXError rc = EUBX_ERROR_NULLPTR;
	
	if (NULL != pHandle) {
		pHandle->is_initialized = true;
		pHandle->last_error = EUBX_ERROR_OK;
    
		pHandle->receive_status = EUBXReceiveExpectSync1;
    pHandle->receive_message.message_class = 0;
    pHandle->receive_message.message_id = 0;
    pHandle->receive_message.message_length = 0;
    pHandle->receive_message.ck_a = 0;
    pHandle->receive_message.ck_b = 0;
    pHandle->receive_position = 0;
    
		pHandle->send_status = EUBXSendIdle;
    pHandle->send_message.message_class = 0;
    pHandle->send_message.message_id = 0;
    pHandle->send_message.message_length = 0;
    pHandle->send_message.ck_a = 0;
    pHandle->send_message.ck_b = 0;
		
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
    pHandle->last_error = EUBX_ERROR_OK;
  
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
        pHandle->receive_message.message_class = byte;
        pHandle->receive_status = EUBXReceiveExpectId;
				break;

			case EUBXReceiveExpectId:
        pHandle->receive_message.message_id = byte;
        pHandle->receive_status = EUBXReceiveExpectLength1;
				break;

			case EUBXReceiveExpectLength1:
        pHandle->receive_message.message_length = byte;
        pHandle->receive_status = EUBXReceiveExpectLength2;
				break;
				
      case EUBXReceiveExpectLength2:
        pHandle->receive_message.message_length = pHandle->receive_message.message_length + (256 * (uint16_t)byte);
        if (0 == pHandle->receive_message.message_length) {
          pHandle->receive_status = EUBXReceiveExpectCKA;
        }
        else {
          pHandle->receive_status = EUBXReceiveExpectContent;
          pHandle->receive_position = 0;
		    }
        break;
        
      case EUBXReceiveExpectContent:
        if (UBX_MESSAGE_BUFFER_SIZE >= pHandle->receive_position) {
          pHandle->last_error = EUBX_ERROR_RECEIVE_OVERFLOW;
        }
        else {
          pHandle->receive_message.message_buffer[pHandle->receive_position] = byte;
        }
        pHandle->receive_position += 1;
        if (pHandle->receive_position == pHandle->receive_message.message_length) {
          pHandle->receive_status = EUBXReceiveExpectCKA;
        }
        break;

      case EUBXReceiveExpectCKA:
        pHandle->receive_message.ck_a = byte;
        pHandle->receive_status = EUBXReceiveExpectCKB;
        break;

      case EUBXReceiveExpectCKB:
        pHandle->receive_message.ck_b = byte;
        if (EUBX_ERROR_OK == pHandle->last_error) {
          handle_receive_message(pHandle);
        }
        pHandle->receive_status = EUBXReceiveExpectSync1;
        break;
        
			default:
				pHandle->receive_status = EUBXReceiveExpectSync1;
				break;
		}
		
		rc = pHandle->last_error;
	}
	
	return rc;
}

void handle_receive_message(struct eubx_handle * pHandle)
{
  uint8_t ck_a = 0;
  uint8_t ck_b = 0;
  
  calculate_checksum(&pHandle->receive_message, &ck_a, &ck_b);

  if ((ck_a == pHandle->receive_message.ck_a) && (ck_b == pHandle->receive_message.ck_b)) {
    switch (pHandle->receive_message.message_class) {
      case EUBX_CLASS_NAV:
        handle_receive_class_nav(&pHandle->receive_message);
        break;
        
      case EUBX_CLASS_RXM:
        handle_receive_class_rxm(&pHandle->receive_message);
        break;
        
      case EUBX_CLASS_INF:
        handle_receive_class_inf(&pHandle->receive_message);
        break;
        
      case EUBX_CLASS_ACK:
        handle_receive_class_ack(&pHandle->receive_message);
        break;
        
      case EUBX_CLASS_CFG:
        handle_receive_class_cfg(&pHandle->receive_message);
        break;
        
      case EUBX_CLASS_MON:
        handle_receive_class_mon(&pHandle->receive_message);
        break;
        
      case EUBX_CLASS_AID:
        handle_receive_class_aid(&pHandle->receive_message);
        break;
        
      case EUBX_CLASS_TIM:
        handle_receive_class_tim(&pHandle->receive_message);
        break;
        
      case EUBX_CLASS_LOG:
        handle_receive_class_log(&pHandle->receive_message);
        break;

      default:
        pHandle->last_error = EUBX_ERROR_UNKNOWN_CLASS;
        break;
    }
  }
  else {
    pHandle->last_error = EUBX_ERROR_CHECKSUM;
  }
}

void handle_receive_class_nav(struct eubx_handle * pHandle)
{
  
}

void handle_receive_class_rxm(struct eubx_handle * pHandle)
{
  
}

void handle_receive_class_inf(struct eubx_handle * pHandle)
{
  
}

void handle_receive_class_ack(struct eubx_handle * pHandle)
{
  
}

void handle_receive_class_cfg(struct eubx_handle * pHandle)
{
  
}

void handle_receive_class_mon(struct eubx_handle * pHandle)
{
  
}

void handle_receive_class_aid(struct eubx_handle * pHandle)
{
  
}

void handle_receive_class_tim(struct eubx_handle * pHandle)
{
  
}

void handle_receive_class_log(struct eubx_handle * pHandle)
{
  
}

void calculate_checksum(const struct eubx_message * message, uint8_t * ck_a, uint8_t * ck_b)
{
  *ck_a = 0;
  *ck_b = 0;

  *ck_a = *ck_a + message->message_class;
  *ck_b = *ck_b + *ck_a;

  *ck_a = *ck_a + message->message_id;
  *ck_b = *ck_b + *ck_a;

  *ck_a = *ck_a + message->message_length % 256;
  *ck_b = *ck_b + *ck_a;

  *ck_a = *ck_a + message->message_length / 256;
  *ck_b = *ck_b + *ck_a;

  for (int i = 0; i < message->message_length; i++) {
    *ck_a = *ck_a + message->message_buffer[i];
    *ck_b = *ck_b + *ck_a;
  }
}
