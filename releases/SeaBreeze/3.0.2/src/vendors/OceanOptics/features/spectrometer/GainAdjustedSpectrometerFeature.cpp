/***************************************************//**
 * @file    GainAdjustedSpectrometerFeature.cpp
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
#include "vendors/OceanOptics/features/spectrometer/GainAdjustedSpectrometerFeature.h"
#include "vendors/OceanOptics/features/eeprom_slots/EEPROMSlotFeature.h"

using namespace seabreeze;
using namespace std;

GainAdjustedSpectrometerFeature::GainAdjustedSpectrometerFeature() {
    this->saturationLevel = 65535;
}

GainAdjustedSpectrometerFeature::~GainAdjustedSpectrometerFeature() {

}

unsigned int GainAdjustedSpectrometerFeature::getSaturationLevel() {
    return this->saturationLevel;
}


bool GainAdjustedSpectrometerFeature::initialize(const Protocol &proto, const Bus &bus)
        throw (FeatureException) {

    unsigned int saturation;

    EEPROMSlotFeature eeprom(18);
    vector<byte> *slot = eeprom.readEEPROMSlot(proto, bus, 0x0011);

    saturation = ((*slot)[4] & 0x00FF) | (((*slot)[5] & 0x00FF) << 8);

    if(0 == saturation) {
        /* May not be initialized right, but could cause division by zero */
        saturation = this->maxIntensity;
    }

    this->saturationLevel = saturation;

    delete slot;

    return true;
}
