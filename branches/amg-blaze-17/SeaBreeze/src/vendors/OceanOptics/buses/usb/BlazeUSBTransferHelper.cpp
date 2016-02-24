/***************************************************//**
 * @file    BlazeUSBTransferHelper.cpp
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
#include "vendors/OceanOptics/buses/usb/BlazeUSBTransferHelper.h"
#include <string.h> /* for memcpy() */

using namespace seabreeze;
using namespace std;

const int BlazeUSBTransferHelper::WORD_SIZE_BYTES = 4;

BlazeUSBTransferHelper::BlazeUSBTransferHelper(USB *usb,
        const OOIUSBBidrectionalEndpointMap &map) : USBTransferHelper(usb) {
    this->sendEndpoint = map.getPrimaryOutEndpoint();
    this->receiveEndpoint = map.getPrimaryInEndpoint();
}

BlazeUSBTransferHelper::~BlazeUSBTransferHelper() {

}

int BlazeUSBTransferHelper::receive(vector<byte> &buffer,
        unsigned int length) throw (BusTransferException) {
    if(0 != (length % WORD_SIZE_BYTES)) {
        vector<byte> *inBuffer;
        int paddedLength;
        
        paddedLength = length + (WORD_SIZE_BYTES - (length % WORD_SIZE_BYTES));
        inBuffer = new vector<byte>(paddedLength);
        
        int result = USBTransferHelper::receive(*inBuffer, paddedLength);
        if(result != paddedLength) {
            string error("Failed to read padded message length: ");
            error += result;
            error += " != ";
            error += paddedLength;
            throw BusTransferException(error);
        }
        memcpy(&buffer[0], &inBuffer[0], length);
        delete inBuffer;
        return length;
    } else {
        return USBTransferHelper::receive(buffer, length);
    }
}

int BlazeUSBTransferHelper::send(const std::vector<byte> &buffer,
        unsigned int length) const throw (BusTransferException) {
    
    if(0 != (length % WORD_SIZE_BYTES)) {
        /* Pad up to a multiple of the word size */
        int paddedLength = length + (WORD_SIZE_BYTES - (length % WORD_SIZE_BYTES));
        vector<byte> *outBuffer = new vector<byte>(paddedLength);
        memcpy(&outBuffer[0], &buffer[0], length);
        int result = USBTransferHelper::send(*outBuffer, paddedLength);
        delete outBuffer;
        return result;
    } else {
        return USBTransferHelper::send(buffer, length);
    }
}
