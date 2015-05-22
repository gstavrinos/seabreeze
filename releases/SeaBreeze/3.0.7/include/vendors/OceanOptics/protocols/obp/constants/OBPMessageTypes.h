/***************************************************//**
 * @file    OBPMessageTypes.h
 * @date    January 2011
 * @author  Ocean Optics, Inc.
 *
 * This class contains legal values for the messageType
 * field in the OBPMessage class.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2014, Ocean Optics Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *******************************************************/

#ifndef OBPMESSAGETYPES_H
#define OBPMESSAGETYPES_H

namespace seabreeze {
  namespace oceanBinaryProtocol {
    class OBPMessageTypes {
    public:
        static const unsigned int OBP_GET_SERIAL_NUMBER       = 0x00000100;
        static const unsigned int OBP_GET_BUF_SPEC32_META     = 0x00100928;
        static const unsigned int OBP_GET_SPECTRUM_NOW        = 0x00101000;
        static const unsigned int OBP_SET_ITIME_USEC          = 0x00110010;
        static const unsigned int OBP_SET_TRIG_MODE           = 0x00110110;
        static const unsigned int OBP_SET_LAMP_ENABLE         = 0x00110410;
        static const unsigned int OBP_SET_TRIG_DELAY_USEC     = 0x00110510;
        static const unsigned int OBP_SET_SHUTTER             = 0x00110610;
        static const unsigned int OBP_GET_WL_COEFF            = 0x00180101;
        static const unsigned int OBP_SET_WL_COEFF            = 0x00180111;
        static const unsigned int OBP_GET_NL_COEFF_COUNT      = 0x00181100;
        static const unsigned int OBP_GET_NL_COEFF            = 0x00181101;
        static const unsigned int OBP_SET_NL_COEFF            = 0x00181111;
        static const unsigned int OBP_GET_IRRAD_CAL_ALL       = 0x00182001;
        static const unsigned int OBP_GET_IRRAD_CAL_COLL_AREA = 0x00182003;
        static const unsigned int OBP_SET_IRRAD_CAL_ALL       = 0x00182011;
        static const unsigned int OBP_SET_IRRAD_CAL_COLL_AREA = 0x00182013;
        static const unsigned int OBP_GET_STRAY_COEFF_COUNT   = 0x00183100;
        static const unsigned int OBP_GET_STRAY_COEFF         = 0x00183101;
        static const unsigned int OBP_SET_STRAY_COEFF         = 0x00183111;
        static const unsigned int OBP_GET_GPIO_OUTPUT_ENABLE  = 0x00200100;
        static const unsigned int OBP_SET_GPIO_OUTPUT_ENABLE  = 0x00200110;
        static const unsigned int OBP_GET_GPIO_VALUE          = 0x00200300;
        static const unsigned int OBP_SET_GPIO_VALUE          = 0x00200310;
        static const unsigned int OBP_SET_SINGLE_STROBE_DELAY = 0x00300010;
        static const unsigned int OBP_SET_SINGLE_STROBE_WIDTH = 0x00300011;
        static const unsigned int OBP_SET_SINGLE_STROBE_ENABLE = 0x00300012;
        static const unsigned int OBP_SET_CONT_STROBE_PERIOD  = 0x00310010;
        static const unsigned int OBP_SET_CONT_STROBE_ENABLE  = 0x00310011;
        static const unsigned int OBP_GET_TEMPERATURE         = 0x00400001;
        static const unsigned int OBP_GET_TE_ENABLE           = 0x00420000;
        static const unsigned int OBP_GET_TE_SETPOINT         = 0x00420001;
        static const unsigned int OBP_GET_TE_FAN_ENABLE       = 0x00420002;
        static const unsigned int OBP_GET_TE_IS_STABLE        = 0x00420003;
        static const unsigned int OBP_GET_TE_TEMPERATURE      = 0x00420004;
        static const unsigned int OBP_SET_TE_ENABLE           = 0x00420010;
        static const unsigned int OBP_SET_TE_SETPOINT         = 0x00420011;
        static const unsigned int OBP_SET_TE_FAN_ENABLE       = 0x00420012;
        static const unsigned int OBP_GET_LIGHT_SOURCE_ENABLE     = 0x00810021;
        static const unsigned int OBP_SET_LIGHT_SOURCE_ENABLE     = 0x00810031;
        static const unsigned int OBP_GET_LIGHT_SOURCE_INTENSITY  = 0x00810041;
        static const unsigned int OBP_SET_LIGHT_SOURCE_INTENSITY  = 0x00810051;
    };
  }
}

#endif /* OBPMESSAGETYPES_H */
