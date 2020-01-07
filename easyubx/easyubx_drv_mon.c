/*
 * source file for the Easy UBX C library for the mon functions 
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
#include "easyubx_drv_mon.h"

static void handle_receive_mon_batch(struct eubx_handle *pHandle);
static void handle_receive_mon_gnss(struct eubx_handle *pHandle);
static void handle_receive_mon_hw2(struct eubx_handle *pHandle);
static void handle_receive_mon_hw(struct eubx_handle *pHandle);
static void handle_receive_mon_io(struct eubx_handle *pHandle);
static void handle_receive_mon_msgpp(struct eubx_handle *pHandle);
static void handle_receive_mon_patch(struct eubx_handle *pHandle);
static void handle_receive_mon_rxbuf(struct eubx_handle *pHandle);
static void handle_receive_mon_rxr(struct eubx_handle *pHandle);
static void handle_receive_mon_smgr(struct eubx_handle *pHandle);
static void handle_receive_mon_txbuf(struct eubx_handle *pHandle);
static void handle_receive_mon_ver(struct eubx_handle *pHandle);

TEasyUBXError eubx_poll_mon_gnss_selection(struct eubx_handle *pHandle)
{
    pHandle->send_message.message_class = EUBX_CLASS_MON;
    pHandle->send_message.message_id = EUBX_ID_MON_GNSS;
    pHandle->send_message.message_length = 0;

    return eubx_send_message(pHandle);
}

TEasyUBXError eubx_poll_mon_version(struct eubx_handle *pHandle)
{
    TEasyUBXError rc = EUBX_ERROR_OK;

    pHandle->send_message.message_class = EUBX_CLASS_MON;
    pHandle->send_message.message_id = EUBX_ID_MON_VER;
    pHandle->send_message.message_length = 0;

    rc = eubx_send_message(pHandle);

    if (EUBX_ERROR_OK == rc)
    {
        eubx_waitfor_event(pHandle, EUBXReceivedMonVersion);
    }

    return rc;
}

void eubx_drv_handle_receive_class_mon(struct eubx_handle *pHandle)
{
    switch (pHandle->receive_message.message_id)
    {
    case EUBX_ID_MON_BATCH:
        handle_receive_mon_batch(pHandle);
        break;

    case EUBX_ID_MON_GNSS:
        handle_receive_mon_gnss(pHandle);
        break;

    case EUBX_ID_MON_HW2:
        handle_receive_mon_hw2(pHandle);
        break;

    case EUBX_ID_MON_HW:
        handle_receive_mon_hw(pHandle);
        break;

    case EUBX_ID_MON_IO:
        handle_receive_mon_io(pHandle);
        break;

    case EUBX_ID_MON_MSGPP:
        handle_receive_mon_msgpp(pHandle);
        break;

    case EUBX_ID_MON_PATCH:
        handle_receive_mon_patch(pHandle);
        break;

    case EUBX_ID_MON_RXBUF:
        handle_receive_mon_rxbuf(pHandle);
        break;

    case EUBX_ID_MON_RXR:
        handle_receive_mon_rxr(pHandle);
        break;

    case EUBX_ID_MON_SMGR:
        handle_receive_mon_smgr(pHandle);
        break;

    case EUBX_ID_MON_TXBUF:
        handle_receive_mon_txbuf(pHandle);
        break;

    case EUBX_ID_MON_VER:
        handle_receive_mon_ver(pHandle);
        break;

    default:
        break;
    }
}

void handle_receive_mon_batch(struct eubx_handle *pHandle)
{
}

void handle_receive_mon_gnss(struct eubx_handle *pHandle)
{
    eubx_send_notification(pHandle, EUBXReceivedMonGNSS);
}

void handle_receive_mon_hw2(struct eubx_handle *pHandle)
{
}

void handle_receive_mon_hw(struct eubx_handle *pHandle)
{
}

void handle_receive_mon_io(struct eubx_handle *pHandle)
{
}

void handle_receive_mon_msgpp(struct eubx_handle *pHandle)
{
}

void handle_receive_mon_patch(struct eubx_handle *pHandle)
{
}

void handle_receive_mon_rxbuf(struct eubx_handle *pHandle)
{
}

void handle_receive_mon_rxr(struct eubx_handle *pHandle)
{
}

void handle_receive_mon_smgr(struct eubx_handle *pHandle)
{
}

void handle_receive_mon_txbuf(struct eubx_handle *pHandle)
{
}

void handle_receive_mon_ver(struct eubx_handle *pHandle)
{
    const char *hw_ver_ptr = (const char *)&pHandle->receive_message.message_buffer[30];

    if (strcmp(hw_ver_ptr, EUBX_CHIPSET_UBLOX9) == 0)
    {
        pHandle->receiver_info.chipset_version = EUBXChipsetUblox9;
    }
    else if (strcmp(hw_ver_ptr, EUBX_CHIPSET_UBLOX8) == 0)
    {
        pHandle->receiver_info.chipset_version = EUBXChipsetUblox8;
    }
    else if (strcmp(hw_ver_ptr, EUBX_CHIPSET_UBLOX7) == 0)
    {
        pHandle->receiver_info.chipset_version = EUBXChipsetUblox7;
    }
    else if (strcmp(hw_ver_ptr, EUBX_CHIPSET_UBLOX6_2) == 0)
    {
        pHandle->receiver_info.chipset_version = EUBXChipsetUblox6_2;
    }
    else if (strcmp(hw_ver_ptr, EUBX_CHIPSET_UBLOX6_1) == 0)
    {
        pHandle->receiver_info.chipset_version = EUBXChipsetUblox6_1;
    }
    else if (strcmp(hw_ver_ptr, EUBX_CHIPSET_UBLOX5) == 0)
    {
        pHandle->receiver_info.chipset_version = EUBXChipsetUblox5;
    }
    else if (strcmp(hw_ver_ptr, EUBX_CHIPSET_ANTARIS4) == 0)
    {
        pHandle->receiver_info.chipset_version = EUBXChipsetAntaris4;
    }
    else if (strcmp(hw_ver_ptr, EUBX_CHIPSET_ANTARIS) == 0)
    {
        pHandle->receiver_info.chipset_version = EUBXChipsetAntaris;
    }
    else
    {
        pHandle->receiver_info.chipset_version = EUBXChipsetUnknown;
    }

    strncpy(pHandle->receiver_info.software_version, (const char *)pHandle->receive_message.message_buffer, EUBX_SW_VERSION_LENGTH);
    pHandle->receive_message.message_buffer[EUBX_SW_VERSION_LENGTH] = 0;

    eubx_send_notification(pHandle, EUBXReceivedMonVersion);
}
