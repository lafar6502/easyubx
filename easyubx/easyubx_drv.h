/*
 * Main include file for the Easy UBX C library
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

#ifndef EASYUBX_DRV_H
#define EASYUBX_DRV_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define UBX_MESSAGE_BUFFER_SIZE     128

#define EUBX_SYNC1					  0xb5
#define EUBX_SYNC2					  0x62

#define EUBX_CLASS_NAV				0x01
#define EUBX_CLASS_RXM				0x02
#define EUBX_CLASS_INF				0x04
#define EUBX_CLASS_ACK				0x05
#define EUBX_CLASS_CFG				0x06
#define EUBX_CLASS_UPD        0x09
#define EUBX_CLASS_MON				0x0a
#define EUBX_CLASS_AID				0x0b
#define EUBX_CLASS_TIM				0x0d
#define EUBX_CLASS_ESF        0x10
#define EUBX_CLASS_MGA        0x13
#define EUBX_CLASS_LOG				0x21
#define EUBX_CLASS_SEC        0x27
#define EUBX_CLASS_HNR        0x28

#define EUBX_ID_ACK_ACK				0x01
#define EUBX_ID_ACK_NAK				0x00

#define EUBX_ID_AID_ALM				0x00
// TODO: to be continued

#define EUBX_ID_CFG_ANT				0x13
#define EUBX_ID_CFG_BATCH     0x93
#define EUBX_ID_CFG_CFG				0x09
#define EUBX_ID_CFG_DAT				0x06
#define EUBX_ID_CFG_DGNSS			0x70
#define EUBX_ID_CFG_DOSC      0x61
#define EUBX_ID_CFG_ESRC      0x60
#define EUBX_ID_CFG_GEOFENCE  0x69
#define EUBX_ID_CFG_GNSS      0x3e
#define EUBX_ID_CFG_HNR       0x5c
#define EUBX_ID_CFG_INF				0x02
#define EUBX_ID_CFG_ITFM				0x39
#define EUBX_ID_CFG_LOGFILTER			0x47
#define EUBX_ID_CFG_MSG				0x01
#define EUBX_ID_CFG_NAV5				0x24
#define EUBX_ID_CFG_NAVX5				0x23
#define EUBX_ID_CFG_NMEA				0x17
#define EUBX_ID_CFG_ODO        0x1e
#define EUBX_ID_CFG_PM2				0x3b
#define EUBX_ID_CFG_PMS        0x86
#define EUBX_ID_CFG_PRT				0x00
#define EUBX_ID_CFG_PWR        0x57
#define EUBX_ID_CFG_RATE				0x08
#define EUBX_ID_CFG_RINV				0x34
#define EUBX_ID_CFG_RST				0x04
#define EUBX_ID_CFG_RXM				0x11
#define EUBX_ID_CFG_SBAS				0x16
#define EUBX_ID_CFG_SLAS        0x8d
#define EUBX_ID_CFG_SMGR        0x62
#define EUBX_ID_CFG_TP5				0x31
#define EUBX_ID_CFG_TXSLOT        0x53
#define EUBX_ID_CFG_USB				0x1b

#define EUBX_ID_ESF_INS             0x15
#define EUBX_ID_ESF_MEAS            0x02
#define EUBX_ID_ESF_RAW             0x03
#define EUBX_ID_ESF_STATUS          0x10

#define EUBX_ID_HNR_INS             0x02
#define EUBX_ID_HNR_PVT             0x00

#define EUBX_ID_INF_DEBUG				    0x04
#define EUBX_ID_INF_ERROR				    0x00
#define EUBX_ID_INF_NOTICE			    0x02
#define EUBX_ID_INF_TEST				    0x03
#define EUBX_ID_INF_WARNING			    0x01

#define EUBX_ID_LOG_BATCH           0x11
#define EUBX_ID_LOG_CREATE			    0x07
#define EUBX_ID_LOG_ERASE				    0x03
#define EUBX_ID_LOG_FINDTIME			  0x0e
#define EUBX_ID_LOG_INFO				    0x08
#define EUBX_ID_LOG_RETRIEVEBATCH   0x10
#define EUBX_ID_LOG_RETRIEVEPOSEXTRA     0x0f
#define EUBX_ID_LOG_RETRIEVEPOS     0x0b
#define EUBX_ID_LOG_RETRIEVESTRING	0x0d
#define EUBX_ID_LOG_RETRIEVE			  0x09
#define EUBX_ID_LOG_STRING			    0x04

#define EUBX_ID_MGA_ACK_DATA0       0x60
// TODO: to be continued

#define EUBX_ID_MON_BATCH           0x32
#define EUBX_ID_MON_GNSS            0x28
#define EUBX_ID_MON_HW2             0x0b
#define EUBX_ID_MON_HW              0x09
#define EUBX_ID_MON_IO              0x02
#define EUBX_ID_MON_MSGPP           0x06
#define EUBX_ID_MON_PATCH           0x27
#define EUBX_ID_MON_RXBUF           0x07
#define EUBX_ID_MON_RXR             0x21
#define EUBX_ID_MON_SMGR            0x2e
#define EUBX_ID_MON_TXBUF           0x08
#define EUBX_ID_MON_VER             0x04

#define EUBX_ID_NAV_AOPSTATUS       0x60
#define EUBX_ID_NAV_ATT             0x05
#define EUBX_ID_NAV_CLOCK           0x22
// TODO: to be continued

#define EUBX_ID_RXM_IMES            0x61
// TODO: to be continued

#define EUBX_ID_SEC_UNIQID          0x03

#define EUBX_ID_TIM_DOSC            0x11
// TODO: to be continued

#define EUBX_ID_UPD_SOS             0x14

typedef enum {
	EUBX_ERROR_OK = 0,
	EUBX_ERROR_NULLPTR = -1,
	EUBX_ERROR_CHECKSUM = -2,
  EUBX_ERROR_NOT_INITIALIZED = -3,
  EUBX_ERROR_RECEIVE_OVERFLOW = -4,
  EUBX_ERROR_UNKNOWN_CLASS = -5
} TEasyUBXError;

typedef enum {
	EUBXReceiveExpectSync1,
	EUBXReceiveExpectSync2,
	EUBXReceiveExpectClass,
	EUBXReceiveExpectId,
  EUBXReceiveExpectLength1,
  EUBXReceiveExpectLength2,
  EUBXReceiveExpectContent,
  EUBXReceiveExpectCKA,
  EUBXReceiveExpectCKB
} TEasyUBXReceiveStatus;

typedef enum {
	EUBXSendIdle,
	EUBXSending
} TEasyUBXSendStatus;

typedef enum {
  EUBXReceivedMonVersion,

  EUBXDebugMessage1 = 1000
} TEasyUBXEvent;

struct eubx_message {
  uint8_t               message_class;
  uint8_t               message_id;
  uint16_t              message_length;
  uint8_t               ck_a;
  uint8_t               ck_b;
  uint8_t               message_buffer[UBX_MESSAGE_BUFFER_SIZE];
};

typedef void (*eubx_send_byte)(void * usr_ptr, uint8_t buffer);
typedef void (*eubx_send_buffer)(void * usr_ptr, const uint8_t * buffer, uint16_t length);
typedef void (*eubx_notify_event)(void * usr_ptr, TEasyUBXEvent event);

struct eubx_handle {
	bool                  is_initialized;			// is set to true if handle is initialized
	TEasyUBXError			    last_error;				  // last error code, will be OK if an operation was successful
	TEasyUBXReceiveStatus	receive_status;
  struct eubx_message   receive_message;
  uint16_t              receive_position;
	TEasyUBXSendStatus		send_status;
  struct eubx_message   send_message;
  eubx_send_byte        send_byte;
  eubx_send_buffer      send_buffer;
  eubx_notify_event     notify_event;
  void                  * callback_usr_ptr;
};

TEasyUBXError eubx_init(struct eubx_handle * pHandle);
TEasyUBXError eubx_set_callback_functions(struct eubx_handle * pHandle, eubx_send_byte send_byte, eubx_send_buffer send_buffer, eubx_notify_event notify_event, void * usr_ptr);
TEasyUBXError eubx_receive_byte(struct eubx_handle * pHandle, uint8_t byte);

TEasyUBXError eubx_poll_mon_version(struct eubx_handle * pHandle);

TEasyUBXError eubx_send_message(struct eubx_handle * pHandle);
TEasyUBXError eubx_send_notification(struct eubx_handle * pHandle, TEasyUBXEvent event);

#ifdef __cplusplus
} // extern "C"
#endif

#endif  /* EASYUBX_DRV_H */
