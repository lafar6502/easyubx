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
#include "easyubx_drv_consts.h"
#include "easyubx_drv_mon.h"
#include "easyubx_drv_nav.h"

static void handle_receive_message(struct eubx_handle * pHandle);
static void handle_receive_class_rxm(struct eubx_handle * pHandle);
static void handle_receive_class_inf(struct eubx_handle * pHandle);
static void handle_receive_class_ack(struct eubx_handle * pHandle);
static void handle_receive_class_upd(struct eubx_handle * pHandle);
static void handle_receive_class_aid(struct eubx_handle * pHandle);
static void handle_receive_class_tim(struct eubx_handle * pHandle);
static void handle_receive_class_esf(struct eubx_handle * pHandle);
static void handle_receive_class_mga(struct eubx_handle * pHandle);
static void handle_receive_class_log(struct eubx_handle * pHandle);
static void handle_receive_class_sec(struct eubx_handle * pHandle);
static void handle_receive_class_hnr(struct eubx_handle * pHandle);

static void calculate_checksum(const struct eubx_message * message, uint8_t * ck_a, uint8_t * ck_b);

TEasyUBXError eubx_init(struct eubx_handle * pHandle, eubx_receive_buffer receive_buffer, eubx_send_byte send_byte, eubx_send_buffer send_buffer, eubx_notify_event notify_event, void * usr_ptr)
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
    
		pHandle->last_event = EUBXEventNone;
    pHandle->send_message.message_class = 0;
    pHandle->send_message.message_id = 0;
    pHandle->send_message.message_length = 0;
    pHandle->send_message.ck_a = 0;
    pHandle->send_message.ck_b = 0;

    pHandle->receive_buffer = receive_buffer;
    pHandle->send_byte = send_byte;
    pHandle->send_buffer = send_buffer;
    pHandle->notify_event = notify_event;
    pHandle->callback_usr_ptr = usr_ptr;

    pHandle->receiver_info.chipset_version = EUBXChipsetNotSet;
    pHandle->receiver_info.software_version[0] = 0;

    eubx_poll_mon_version(pHandle);
    eubx_poll_cfg_nav5(pHandle);
    
		rc = pHandle->last_error;
	}
	
	return rc;
}

void eubx_loop(struct eubx_handle * pHandle)
{
  if (pHandle->receive_buffer != NULL) {
    uint8_t buffer[16];
    uint16_t length = pHandle->receive_buffer(pHandle->callback_usr_ptr, buffer, sizeof(buffer));

    for (uint16_t i = 0; i < length; i++) {
      eubx_receive_byte(pHandle, buffer[i]);
    }
  }
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
        if (EUBX_MESSAGE_BUFFER_SIZE < pHandle->receive_position) {
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

TEasyUBXError eubx_send_message(struct eubx_handle * pHandle)
{
  TEasyUBXError rc = EUBX_ERROR_NULLPTR;

  if ((NULL != pHandle) && (NULL != pHandle->send_byte)) {
    pHandle->last_event = EUBXEventNone;

    calculate_checksum(&pHandle->send_message, &pHandle->send_message.ck_a, &pHandle->send_message.ck_b);

    pHandle->send_byte(pHandle->callback_usr_ptr, EUBX_SYNC1);
    pHandle->send_byte(pHandle->callback_usr_ptr, EUBX_SYNC2);
    pHandle->send_byte(pHandle->callback_usr_ptr, pHandle->send_message.message_class);
    pHandle->send_byte(pHandle->callback_usr_ptr, pHandle->send_message.message_id);
    pHandle->send_byte(pHandle->callback_usr_ptr, pHandle->send_message.message_length % 256);
    pHandle->send_byte(pHandle->callback_usr_ptr, pHandle->send_message.message_length / 256);
    if (NULL != pHandle->send_buffer) {
      pHandle->send_buffer(pHandle->callback_usr_ptr, pHandle->send_message.message_buffer, pHandle->send_message.message_length);
    }
    else {
      for (int i = 0; i < pHandle->send_message.message_length; i++) {
        pHandle->send_byte(pHandle->callback_usr_ptr, pHandle->send_message.message_buffer[i]);
      }
    }
    pHandle->send_byte(pHandle->callback_usr_ptr, pHandle->send_message.ck_a);
    pHandle->send_byte(pHandle->callback_usr_ptr, pHandle->send_message.ck_b);

    rc = EUBX_ERROR_OK;
  }

  return rc;
}

TEasyUBXError eubx_waitfor_event(struct eubx_handle * pHandle, TEasyUBXEvent event)
{
  while (event != pHandle->last_event) {
    eubx_loop(pHandle);
  }

  return EUBX_ERROR_OK;
}

TEasyUBXError eubx_waitfor_ack(struct eubx_handle * pHandle, uint8_t message_class, uint8_t message_id)
{
  TEasyUBXError rc = EUBX_ERROR_OK;
  
  while ((EUBXReceivedACK != pHandle->last_event) && (EUBXReceivedNAK != pHandle->last_event) && (message_class != pHandle->receive_message.message_class) && (message_id != pHandle->receive_message.message_id)) {
    eubx_loop(pHandle);
  }

  if (EUBXReceivedNAK == pHandle->last_event) {
    rc = EUBX_ERROR_NAK;
  }

  return rc;
}

TEasyUBXError eubx_send_notification(struct eubx_handle * pHandle, TEasyUBXEvent event)
{
  TEasyUBXError rc = EUBX_ERROR_NULLPTR;

  if (NULL != pHandle) {
    pHandle->last_event = event;
    
    if (NULL != pHandle->notify_event) {
      pHandle->notify_event(pHandle->callback_usr_ptr, event);
    }
  }
}

void handle_receive_message(struct eubx_handle * pHandle)
{
  uint8_t ck_a = 0;
  uint8_t ck_b = 0;
  
  calculate_checksum(&pHandle->receive_message, &ck_a, &ck_b);

  if ((ck_a == pHandle->receive_message.ck_a) && (ck_b == pHandle->receive_message.ck_b)) {
    switch (pHandle->receive_message.message_class) {
      case EUBX_CLASS_NAV:
        eubx_drv_handle_receive_class_nav(pHandle);
        break;
        
      case EUBX_CLASS_RXM:
        handle_receive_class_rxm(pHandle);
        break;
        
      case EUBX_CLASS_INF:
        handle_receive_class_inf(pHandle);
        break;
        
      case EUBX_CLASS_ACK:
        handle_receive_class_ack(pHandle);
        break;
        
      case EUBX_CLASS_CFG:
        eubx_drv_handle_receive_class_cfg(pHandle);
        break;
        
      case EUBX_CLASS_UPD:
        handle_receive_class_upd(pHandle);
        break;
        
      case EUBX_CLASS_MON:
        eubx_drv_handle_receive_class_mon(pHandle);
        break;
        
      case EUBX_CLASS_AID:
        handle_receive_class_aid(pHandle);
        break;
        
      case EUBX_CLASS_TIM:
        handle_receive_class_tim(pHandle);
        break;
        
      case EUBX_CLASS_ESF:
        handle_receive_class_esf(pHandle);
        break;
        
      case EUBX_CLASS_MGA:
        handle_receive_class_mga(pHandle);
        break;
        
      case EUBX_CLASS_LOG:
        handle_receive_class_log(pHandle);
        break;

      case EUBX_CLASS_SEC:
        handle_receive_class_sec(pHandle);
        break;
        
      case EUBX_CLASS_HNR:
        handle_receive_class_hnr(pHandle);
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

void handle_receive_class_rxm(struct eubx_handle * pHandle)
{
}

void handle_receive_class_inf(struct eubx_handle * pHandle)
{
}

void handle_receive_class_ack(struct eubx_handle * pHandle)
{
  switch (pHandle->receive_message.message_id) {
    case EUBX_ID_ACK_ACK:
      eubx_send_notification(pHandle, EUBXReceivedACK);
      break;

    case EUBX_ID_ACK_NAK:
      eubx_send_notification(pHandle, EUBXReceivedNAK);
      break;
    
    default:
      break;
  }
}

void handle_receive_class_upd(struct eubx_handle * pHandle)
{
}

void handle_receive_class_aid(struct eubx_handle * pHandle)
{
}

void handle_receive_class_tim(struct eubx_handle * pHandle)
{
}

void handle_receive_class_esf(struct eubx_handle * pHandle)
{
}

void handle_receive_class_mga(struct eubx_handle * pHandle)
{
}

void handle_receive_class_log(struct eubx_handle * pHandle)
{
}

void handle_receive_class_sec(struct eubx_handle * pHandle)
{
}

void handle_receive_class_hnr(struct eubx_handle * pHandle)
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
