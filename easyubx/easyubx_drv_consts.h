/*
 * Constants include file for the Easy UBX C library
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

#ifndef EASYUBX_DRV_CONSTS_H
#define EASYUBX_DRV_CONSTS_H

#define EUBX_CHIPSET_ANTARIS              "00000040"
#define EUBX_CHIPSET_ANTARIS4             "00040001"
#define EUBX_CHIPSET_UBLOX5               "00040005"
#define EUBX_CHIPSET_UBLOX6_1             "00040006"
#define EUBX_CHIPSET_UBLOX6_2             "00040007"
#define EUBX_CHIPSET_UBLOX7               "00070000"
#define EUBX_CHIPSET_UBLOX8               "00080000"
#define EUBX_CHIPSET_UBLOX9               "00090000"

#define EUBX_SYNC1                        0xb5
#define EUBX_SYNC2                        0x62

#define EUBX_CLASS_NAV                    0x01
#define EUBX_CLASS_RXM                    0x02
#define EUBX_CLASS_INF                    0x04
#define EUBX_CLASS_ACK                    0x05
#define EUBX_CLASS_CFG                    0x06
#define EUBX_CLASS_UPD                    0x09
#define EUBX_CLASS_MON                    0x0a
#define EUBX_CLASS_AID                    0x0b
#define EUBX_CLASS_TIM                    0x0d
#define EUBX_CLASS_ESF                    0x10
#define EUBX_CLASS_MGA                    0x13
#define EUBX_CLASS_LOG                    0x21
#define EUBX_CLASS_SEC                    0x27
#define EUBX_CLASS_HNR                    0x28

#define EUBX_ID_ACK_ACK                   0x01
#define EUBX_ID_ACK_NAK                   0x00

#define EUBX_ID_AID_ALM                   0x00
// TODO: to be continued

#define EUBX_ID_CFG_ANT                   0x13
#define EUBX_ID_CFG_BATCH                 0x93
#define EUBX_ID_CFG_CFG                   0x09
#define EUBX_ID_CFG_DAT                   0x06
#define EUBX_ID_CFG_DGNSS                 0x70
#define EUBX_ID_CFG_DOSC                  0x61
#define EUBX_ID_CFG_ESRC                  0x60
#define EUBX_ID_CFG_GEOFENCE              0x69
#define EUBX_ID_CFG_GNSS                  0x3e
#define EUBX_ID_CFG_HNR                   0x5c
#define EUBX_ID_CFG_INF                   0x02
#define EUBX_ID_CFG_ITFM                  0x39
#define EUBX_ID_CFG_LOGFILTER             0x47
#define EUBX_ID_CFG_MSG                   0x01
#define EUBX_ID_CFG_NAV5                  0x24
#define EUBX_ID_CFG_NAVX5                 0x23
#define EUBX_ID_CFG_NMEA                  0x17
#define EUBX_ID_CFG_ODO                   0x1e
#define EUBX_ID_CFG_PM2                   0x3b
#define EUBX_ID_CFG_PMS                   0x86
#define EUBX_ID_CFG_PRT                   0x00
#define EUBX_ID_CFG_PWR                   0x57
#define EUBX_ID_CFG_RATE                  0x08
#define EUBX_ID_CFG_RINV                  0x34
#define EUBX_ID_CFG_RST                   0x04
#define EUBX_ID_CFG_RXM                   0x11
#define EUBX_ID_CFG_SBAS                  0x16
#define EUBX_ID_CFG_SLAS                  0x8d
#define EUBX_ID_CFG_SMGR                  0x62
#define EUBX_ID_CFG_TP5                   0x31
#define EUBX_ID_CFG_TXSLOT                0x53
#define EUBX_ID_CFG_USB                   0x1b

#define EUBX_ID_ESF_INS                   0x15
#define EUBX_ID_ESF_MEAS                  0x02
#define EUBX_ID_ESF_RAW                   0x03
#define EUBX_ID_ESF_STATUS                0x10

#define EUBX_ID_HNR_INS                   0x02
#define EUBX_ID_HNR_PVT                   0x00

#define EUBX_ID_INF_DEBUG                 0x04
#define EUBX_ID_INF_ERROR                 0x00
#define EUBX_ID_INF_NOTICE                0x02
#define EUBX_ID_INF_TEST                  0x03
#define EUBX_ID_INF_WARNING               0x01

#define EUBX_ID_LOG_BATCH                 0x11
#define EUBX_ID_LOG_CREATE                0x07
#define EUBX_ID_LOG_ERASE                 0x03
#define EUBX_ID_LOG_FINDTIME              0x0e
#define EUBX_ID_LOG_INFO                  0x08
#define EUBX_ID_LOG_RETRIEVEBATCH         0x10
#define EUBX_ID_LOG_RETRIEVEPOSEXTRA      0x0f
#define EUBX_ID_LOG_RETRIEVEPOS           0x0b
#define EUBX_ID_LOG_RETRIEVESTRING        0x0d
#define EUBX_ID_LOG_RETRIEVE              0x09
#define EUBX_ID_LOG_STRING                0x04

#define EUBX_ID_MGA_ACK_DATA0             0x60
// TODO: to be continued

#define EUBX_ID_MON_BATCH                 0x32
#define EUBX_ID_MON_GNSS                  0x28
#define EUBX_ID_MON_HW2                   0x0b
#define EUBX_ID_MON_HW                    0x09
#define EUBX_ID_MON_IO                    0x02
#define EUBX_ID_MON_MSGPP                 0x06
#define EUBX_ID_MON_PATCH                 0x27
#define EUBX_ID_MON_RXBUF                 0x07
#define EUBX_ID_MON_RXR                   0x21
#define EUBX_ID_MON_SMGR                  0x2e
#define EUBX_ID_MON_TXBUF                 0x08
#define EUBX_ID_MON_VER                   0x04

#define EUBX_ID_NAV_AOPSTATUS             0x60
#define EUBX_ID_NAV_ATT                   0x05
#define EUBX_ID_NAV_CLOCK                 0x22
#define EUBX_ID_NAV_DGPS                  0x31
#define EUBX_ID_NAV_DOP                   0x04
#define EUBX_ID_NAV_EOE                   0x61
#define EUBX_ID_NAV_GEOFENCE              0x39
#define EUBX_ID_NAV_HPPESECEF             0x13
#define EUBX_ID_NAV_HPPOSLLH              0x14
#define EUBX_ID_NAV_ODO                   0x09
#define EUBX_ID_NAV_ORB                   0x34
#define EUBX_ID_NAV_POSECEF               0x01
#define EUBX_ID_NAV_POSLLH                0x02
#define EUBX_ID_NAV_PVT                   0x07
#define EUBX_ID_NAV_RELPOSNED             0x3c
#define EUBX_ID_NAV_RESETODO              0x10
#define EUBX_ID_NAV_SAT                   0x35
#define EUBX_ID_NAV_SBAS                  0x32
#define EUBX_ID_NAV_SOL                   0x06
#define EUBX_ID_NAV_STATUS                0x03
#define EUBX_ID_NAV_SVINFO                0x30
#define EUBX_ID_NAV_SVIN                  0x3b
#define EUBX_ID_NAV_TIMEBDS               0x24
#define EUBX_ID_NAV_TIMEGAL               0x25
#define EUBX_ID_NAV_TIMEGLO               0x23
#define EUBX_ID_NAV_TIMEGPS               0x20
#define EUBX_ID_NAV_TIMELS                0x26
#define EUBX_ID_NAV_TIMEUTC               0x21
#define EUBX_ID_NAV_VELECEF               0x11
#define EUBX_ID_NAV_VELENED               0x12

#define EUBX_ID_RXM_IMES                  0x61
// TODO: to be continued

#define EUBX_ID_SEC_UNIQID                0x03

#define EUBX_ID_TIM_DOSC                  0x11
// TODO: to be continued

#define EUBX_ID_UPD_SOS                   0x14

#endif  /* EASYUBX_DRV_CONSTS_H */
