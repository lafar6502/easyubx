/*
 * include file for the Easy UBX C library for the cfg functions 
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

#include <string.h>

#include "easyubx_drv.h"
#include "easyubx_drv_consts.h"
#include "easyubx_drv_cfg.h"

static void handle_receive_cfg_ant(struct eubx_handle *pHandle);
static void handle_receive_cfg_msg(struct eubx_handle *pHandle);
static void handle_receive_cfg_nav5(struct eubx_handle *pHandle);
static void handle_receive_cfg_nmea(struct eubx_handle *pHandle);
static void handle_receive_cfg_prt(struct eubx_handle *pHandle);
static void handle_receive_cfg_rate(struct eubx_handle *pHandle);

TEasyUBXError eubx_poll_cfg_nav5(struct eubx_handle *pHandle)
{
    pHandle->send_message.message_class = EUBX_CLASS_CFG;
    pHandle->send_message.message_id = EUBX_ID_CFG_NAV5;
    pHandle->send_message.message_length = 0;

    return eubx_send_message_wait4ack(pHandle, EUBX_CLASS_CFG, EUBX_ID_CFG_NAV5);
}

TEasyUBXError eubx_poll_cfg_port(struct eubx_handle *pHandle)
{
    pHandle->send_message.message_class = EUBX_CLASS_CFG;
    pHandle->send_message.message_id = EUBX_ID_CFG_PRT;
    pHandle->send_message.message_length = 0;

    return eubx_send_message_wait4ack(pHandle, EUBX_CLASS_CFG, EUBX_ID_CFG_PRT);
}

TEasyUBXError eubx_poll_cfg_rate(struct eubx_handle *pHandle)
{
    pHandle->send_message.message_class = EUBX_CLASS_CFG;
    pHandle->send_message.message_id = EUBX_ID_CFG_RATE;
    pHandle->send_message.message_length = 0;

    return eubx_send_message_wait4ack(pHandle, EUBX_CLASS_CFG, EUBX_ID_CFG_RATE);
}

TEasyUBXError eubx_set_dyn_model(struct eubx_handle * pHandle, TEasyUBXDynamicPlatformModel dyn_model, TEasyUBXFixMode fix_mode)
{
    pHandle->send_message.message_class = EUBX_CLASS_CFG;
    pHandle->send_message.message_id = EUBX_ID_CFG_NAV5;
    pHandle->send_message.message_length = 36;

    memset(pHandle->send_message.message_buffer, 0, pHandle->send_message.message_length);

    pHandle->send_message.message_buffer[0] = 0x05;
    pHandle->send_message.message_buffer[1] = 0x00;
    pHandle->send_message.message_buffer[2] = dyn_model;
    pHandle->send_message.message_buffer[3] = fix_mode;

    return eubx_send_message_wait4ack(pHandle, EUBX_CLASS_CFG, EUBX_ID_CFG_NAV5);
}

void eubx_drv_handle_receive_class_cfg(struct eubx_handle *pHandle)
{
    switch (pHandle->receive_message.message_id)
    {
    case EUBX_ID_CFG_ANT:
        handle_receive_cfg_ant(pHandle);
        break;

    case EUBX_ID_CFG_MSG:
        handle_receive_cfg_msg(pHandle);
        break;

    case EUBX_ID_CFG_NAV5:
        handle_receive_cfg_nav5(pHandle);
        break;

    case EUBX_ID_CFG_NMEA:
        handle_receive_cfg_nmea(pHandle);
        break;

    case EUBX_ID_CFG_PRT:
        handle_receive_cfg_prt(pHandle);
        break;

    case EUBX_ID_CFG_RATE:
        handle_receive_cfg_rate(pHandle);
        break;

    default:
        break;
    }
}

void handle_receive_cfg_ant(struct eubx_handle *pHandle)
{
}

void handle_receive_cfg_msg(struct eubx_handle *pHandle)
{
}

void handle_receive_cfg_nav5(struct eubx_handle *pHandle)
{
    pHandle->receiver_config.dynamic_platform_model = (TEasyUBXDynamicPlatformModel)pHandle->receive_message.message_buffer[2];
    pHandle->receiver_config.fix_mode = (TEasyUBXFixMode)pHandle->receive_message.message_buffer[3];

    eubx_send_notification(pHandle, EUBXReceivedCfgNAV5);
}

void handle_receive_cfg_nmea(struct eubx_handle *pHandle)
{
    eubx_send_notification(pHandle, EUBXReceivedCfgNMEA);
}

void handle_receive_cfg_prt(struct eubx_handle *pHandle)
{
    eubx_send_notification(pHandle, EUBXReceivedCfgPRT);
}

 void handle_receive_cfg_rate(struct eubx_handle *pHandle)
{
    pHandle->receiver_config.measurement_rate = 0;
    pHandle->receiver_config.navigation_rate = 0;

    pHandle->receiver_config.measurement_rate += pHandle->receive_message.message_buffer[0];
    pHandle->receiver_config.measurement_rate += pHandle->receive_message.message_buffer[1] * 256;

    pHandle->receiver_config.navigation_rate += pHandle->receive_message.message_buffer[2];
    pHandle->receiver_config.navigation_rate += pHandle->receive_message.message_buffer[3] * 256;

    eubx_send_notification(pHandle, EUBXReceivedCfgRATE);
}
