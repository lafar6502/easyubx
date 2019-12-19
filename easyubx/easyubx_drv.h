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

#define EUBX_MESSAGE_BUFFER_SIZE     128
#define EUBX_SW_VERSION_LENGTH       24

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
  EUBXReceivedCfgNMEA,
  EUBXReceivedCfgPRT,
  EUBXReceivedMonGNSS,
  EUBXReceivedMonVersion,

  EUBXDebugMessage1 = 1000
} TEasyUBXEvent;

typedef enum {
  EUBXChipsetNotSet,
  EUBXChipsetAntaris,
  EUBXChipsetAntaris4,
  EUBXChipsetUblox5,
  EUBXChipsetUblox6_1,
  EUBXChipsetUblox6_2,
  EUBXChipsetUblox7,
  EUBXChipsetUblox8,
  EUBXChipsetUblox9,
  EUBXChipsetUnknown = 1000
} TEasyUBXChipsetVersion;

struct eubx_message {
  uint8_t               message_class;
  uint8_t               message_id;
  uint16_t              message_length;
  uint8_t               ck_a;
  uint8_t               ck_b;
  uint8_t               message_buffer[EUBX_MESSAGE_BUFFER_SIZE];
};

struct eubx_receiver_info {
  TEasyUBXChipsetVersion  chipset_version;
  char                    software_version[EUBX_SW_VERSION_LENGTH];
};

typedef uint16_t (*eubx_receive_buffer)(void * usr_ptr, uint8_t * buffer, uint16_t max_length);
typedef void (*eubx_send_byte)(void * usr_ptr, uint8_t buffer);
typedef void (*eubx_send_buffer)(void * usr_ptr, const uint8_t * buffer, uint16_t length);
typedef void (*eubx_notify_event)(void * usr_ptr, TEasyUBXEvent event);

struct eubx_handle {
	bool                        is_initialized;			// is set to true if handle is initialized
	TEasyUBXError			          last_error;				  // last error code, will be OK if an operation was successful
	TEasyUBXReceiveStatus	      receive_status;
  struct eubx_message         receive_message;
  uint16_t                    receive_position;
	TEasyUBXSendStatus		      send_status;
  struct eubx_message         send_message;
  eubx_receive_buffer         receive_buffer;
  eubx_send_byte              send_byte;
  eubx_send_buffer            send_buffer;
  eubx_notify_event           notify_event;
  void                        * callback_usr_ptr;
  struct eubx_receiver_info   receiver_info;
};

TEasyUBXError eubx_init(struct eubx_handle * pHandle);
TEasyUBXError eubx_set_callback_functions(struct eubx_handle * pHandle, eubx_receive_buffer receive_buffer, eubx_send_byte send_byte, eubx_send_buffer send_buffer, eubx_notify_event notify_event, void * usr_ptr);
TEasyUBXError eubx_receive_byte(struct eubx_handle * pHandle, uint8_t byte);
TEasyUBXError eubx_loop(struct eubx_handle * pHandle);

TEasyUBXError eubx_poll_cfg_port(struct eubx_handle * pHandle);

TEasyUBXError eubx_poll_mon_gnss_selection(struct eubx_handle * pHandle);
TEasyUBXError eubx_poll_mon_version(struct eubx_handle * pHandle);

TEasyUBXError eubx_send_message(struct eubx_handle * pHandle);
TEasyUBXError eubx_send_notification(struct eubx_handle * pHandle, TEasyUBXEvent event);

#ifdef __cplusplus
} // extern "C"
#endif

#endif  /* EASYUBX_DRV_H */
