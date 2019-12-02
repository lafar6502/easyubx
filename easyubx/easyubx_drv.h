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

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#define UBX_RECEIVE_BUFFER_SIZE     128

const uint8_t EUBX_SYNC1					  = 0xb5;
const uint8_t EUBX_SYNC2					  = 0x62;

const uint8_t EUBX_CLASS_NAV				= 0x01;
const uint8_t EUBX_CLASS_RXM				= 0x02;
const uint8_t EUBX_CLASS_INF				= 0x04;
const uint8_t EUBX_CLASS_ACK				= 0x05;
const uint8_t EUBX_CLASS_CFG				= 0x06;
const uint8_t EUBX_CLASS_MON				= 0x0a;
const uint8_t EUBX_CLASS_AID				= 0x0b;
const uint8_t EUBX_CLASS_TIM				= 0x0d;
const uint8_t EUBX_CLASS_LOG				= 0x21;

const uint8_t EUBX_ID_ACK_ACK				= 0x01;
const uint8_t EUBX_ID_ACK_NAK				= 0x00;

const uint8_t EUBX_ID_AID_ALM				= 0x00;
// TODO: to be continued

const uint8_t EUBX_ID_CFG_ANT				= 0x13;
const uint8_t EUBX_ID_CFG_CFG				= 0x09;
const uint8_t EUBX_ID_CFG_DAT				= 0x06;
const uint8_t EUBX_ID_CFG_GNSS			= 0x3d;
const uint8_t EUBX_ID_CFG_INF				= 0x02;
const uint8_t EUBX_ID_CFG_ITFM				= 0x39;
const uint8_t EUBX_ID_CFG_LOGFILTER			= 0x47;
const uint8_t EUBX_ID_CFG_MSG				= 0x01;
const uint8_t EUBX_ID_CFG_NAV5				= 0x24;
const uint8_t EUBX_ID_CFG_NAVX5				= 0x23;
const uint8_t EUBX_ID_CFG_NMEA				= 0x17;
const uint8_t EUBX_ID_CFG_PM2				= 0x3b;
const uint8_t EUBX_ID_CFG_PRT				= 0x00;
const uint8_t EUBX_ID_CFG_RATE				= 0x08;
const uint8_t EUBX_ID_CFG_RINV				= 0x34;
const uint8_t EUBX_ID_CFG_RST				= 0x04;
const uint8_t EUBX_ID_CFG_RXM				= 0x11;
const uint8_t EUBX_ID_CFG_SBAS				= 0x16;
const uint8_t EUBX_ID_CFG_TP5				= 0x31;
const uint8_t EUBX_ID_CFG_USB				= 0x1b;

const uint8_t EUBX_ID_INF_DEBUG				    = 0x04;
const uint8_t EUBX_ID_INF_ERROR				    = 0x00;
const uint8_t EUBX_ID_INF_NOTICE			    = 0x02;
const uint8_t EUBX_ID_INF_TEST				    = 0x03;
const uint8_t EUBX_ID_INF_WARNING			    = 0x01;

const uint8_t EUBX_ID_LOG_CREATE			    = 0x07;
const uint8_t EUBX_ID_LOG_ERASE				    = 0x03;
const uint8_t EUBX_ID_LOG_FINDTIME			  = 0x0e;
const uint8_t EUBX_ID_LOG_INFO				    = 0x08;
const uint8_t EUBX_ID_LOG_RETRIEVEPOS		  = 0x0b;
const uint8_t EUBX_ID_LOG_RETRIEVESTRING	= 0x0d;
const uint8_t EUBX_ID_LOG_RETRIEVE			  = 0x09;
const uint8_t EUBX_ID_LOG_STRING			    = 0x04;

typedef enum {
	EUBX_ERROR_OK = 0,
	EUBX_ERROR_NULLPTR = -1,
	EUBX_ERROR_CHECKSUM = -2,
  EUBX_ERROR_NOT_INITIALIZED = -3,
  EUBX_ERROR_RECEIVE_OVERFLOW = -4
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

struct eubx_handle {
	bool                  is_initialized;			// is set to true if handle is initialized
	TEasyUBXError			    last_error;				  // last error code, will be OK if an operation was successful
	TEasyUBXReceiveStatus	receive_status;
  uint8_t               receive_class;
  uint8_t               receive_id;
  uint16_t              receive_length;
  uint8_t               receive_ck_a;
  uint8_t               receive_ck_b;
  uint16_t              receive_position;
  uint8_t               receive_buffer[UBX_RECEIVE_BUFFER_SIZE];
	TEasyUBXSendStatus		send_status;
};

TEasyUBXError eubx_init(struct eubx_handle * pHandle);

TEasyUBXError eubx_receive_byte(struct eubx_handle * pHandle, uint8_t byte);

#ifdef __cplusplus
} // extern "C"
#endif

#endif  /* EASYUBX_DRV_H */
