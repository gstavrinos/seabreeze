/***************************************************//**
 * @file    BlazeUSBTransferHelper.h
 * @date    February 2016
 * @author  Ocean Optics, Inc.
 *
 * This class encapsulates the behavior of the USB4000 and HR4000
 * in the case where they are connected via a USB2.0 bus.  For the
 * case where the device is connected via USB 1.1, then the
 * OOIUSBSpectrumTransferHelper should be used instead.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2016, Ocean Optics Inc
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

#ifndef BLAZEUSBTRANSFERHELPER_H
#define BLAZEUSBTRANSFERHELPER_H

#include "common/buses/usb/USBTransferHelper.h"
#include "vendors/OceanOptics/buses/usb/OOIUSBEndpointMaps.h"

namespace seabreeze {

    class BlazeUSBTransferHelper : public USBTransferHelper {
    public:
        BlazeUSBTransferHelper(USB *usb,
            const OOIUSBBidrectionalEndpointMap &map);
        virtual ~BlazeUSBTransferHelper();

        /* Inherited */
        virtual int receive(std::vector<byte> &buffer, unsigned int length)
            throw (BusTransferException);
        virtual int send(const std::vector<byte> &buffer, unsigned int length) const
            throw (BusTransferException);
        
    private:
        static const int WORD_SIZE_BYTES;
    };

}

#endif /* BLAZEUSBTRANSFERHELPER_H */
