/***************************************************//**
 * @file    BlazeUSB.cpp
 * @date    February 2016
 * @author  Ocean Optics, Inc.
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

#include "common/globals.h"
#include "vendors/OceanOptics/buses/usb/BlazeUSB.h"
#include "vendors/OceanOptics/buses/usb/OOIUSBProductID.h"
#include "vendors/OceanOptics/buses/usb/OOIUSBEndpointMaps.h"
#include "vendors/OceanOptics/protocols/ooi/hints/ControlHint.h"
#include "vendors/OceanOptics/protocols/ooi/hints/SpectrumHint.h"
#include "vendors/OceanOptics/buses/usb/BlazeUSBTransferHelper.h"

using namespace seabreeze;
using namespace seabreeze::ooiProtocol;

BlazeUSB::BlazeUSB() {
    this->productID = BLAZE_USB_PID;
}

BlazeUSB::~BlazeUSB() {

}

bool BlazeUSB::open() {
    bool retval = false;

    retval = OOIUSBInterface::open();

    if(true == retval) {
        OBPControlHint *controlHint = new OBPControlHint();
        OBPSpectrumHint *spectrumHint = new OBPSpectrumHint();
        OOIUSBSimpleDualEndpointMap epMap;

        clearHelpers();

        /* On the Blaze, there is only a single endpoint in
         * each direction.  All hints map to the same kind of helper.
         * The helper is special because there is a certain minimum block
         * size that must be respected when communicating over USB.
         */
        addHelper(spectrumHint, new BlazeUSBTransferHelper(
            (this->usb), epMap));
        addHelper(controlHint, new BlazeUSBTransferHelper(
            (this->usb), epMap));
    }

    return retval;
}
