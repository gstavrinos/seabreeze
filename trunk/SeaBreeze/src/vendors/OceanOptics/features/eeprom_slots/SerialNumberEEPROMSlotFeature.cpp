/***************************************************//**
 * @file    SerialNumberEEPROMSlotFeature.cpp
 * @date    February 2009
 * @author  Ocean Optics, Inc.
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

#include "common/globals.h"
#include "vendors/OceanOptics/features/eeprom_slots/SerialNumberEEPROMSlotFeature.h"
#include "api/seabreezeapi/FeatureFamilies.h"
#include <vector>

using namespace std;
using namespace seabreeze;
using namespace seabreeze::api;

SerialNumberEEPROMSlotFeature::SerialNumberEEPROMSlotFeature() {

}

SerialNumberEEPROMSlotFeature::~SerialNumberEEPROMSlotFeature() {

}

string *SerialNumberEEPROMSlotFeature::readSerialNumber(const Protocol &protocol,
        const Bus &bus) throw (FeatureException) {

    /* Slot zero has the serial number as an ASCII string. */
    /* Note that this may throw a FeatureException. */
    vector<byte> *data = readEEPROMSlot(protocol, bus, 0);

    string *retval = new string();
    vector<byte>::iterator iter;
    /* This is probably not the most efficient way to copy
     * from a vector of bytes into a string, but at least
     * this way issues of string encoding should be
     * avoided (i.e. the sizeof a string element is not
     * assumed here).  Since this function will not be called
     * continuously nor is the serial number ever very long,
     * this should suffice.
     */
    for(iter = data->begin(); iter != data->end(); iter++) {
        retval->push_back((char) * iter);
        if('\0' == *iter) {
            break;
        }
    }

    delete data;

    return retval;
}

FeatureFamily SerialNumberEEPROMSlotFeature::getFeatureFamily() {
    FeatureFamilies families;

    return families.SERIAL_NUMBER;
}
