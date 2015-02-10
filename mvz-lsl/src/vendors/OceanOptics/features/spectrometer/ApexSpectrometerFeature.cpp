/***************************************************//**
 * @file    ApexSpectrometerFeature.cpp
 * @date    January 2013
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
#include "vendors/OceanOptics/features/spectrometer/ApexSpectrometerFeature.h"
#include "vendors/OceanOptics/protocols/ooi/exchanges/IntegrationTimeExchange.h"
#include "vendors/OceanOptics/protocols/ooi/exchanges/ReadSpectrumExchange.h"
#include "vendors/OceanOptics/protocols/ooi/exchanges/MayaProSpectrumExchange.h"
#include "vendors/OceanOptics/protocols/ooi/exchanges/RequestSpectrumExchange.h"
#include "vendors/OceanOptics/protocols/ooi/exchanges/TriggerModeExchange.h"
#include "vendors/OceanOptics/protocols/ooi/impls/OOISpectrometerProtocol.h"
#include "vendors/OceanOptics/features/eeprom_slots/EEPROMSlotFeature.h"

using namespace seabreeze;
using namespace seabreeze::ooiProtocol;
using namespace std;

const long ApexSpectrometerFeature::INTEGRATION_TIME_MINIMUM = 15000;
const long ApexSpectrometerFeature::INTEGRATION_TIME_MAXIMUM = 1600000000;
const long ApexSpectrometerFeature::INTEGRATION_TIME_INCREMENT = 1000;
const long ApexSpectrometerFeature::INTEGRATION_TIME_BASE = 1;

ApexSpectrometerFeature::ApexSpectrometerFeature() {

    this->numberOfPixels = 2068;
    this->maxIntensity = 64000;
    int readoutLength = 2304 * 2 + 1;

    this->integrationTimeMinimum = ApexSpectrometerFeature::INTEGRATION_TIME_MINIMUM;
    this->integrationTimeMaximum = ApexSpectrometerFeature::INTEGRATION_TIME_MAXIMUM;
    this->integrationTimeBase = ApexSpectrometerFeature::INTEGRATION_TIME_BASE;
    this->integrationTimeIncrement = ApexSpectrometerFeature::INTEGRATION_TIME_INCREMENT;

    for(int i = 0; i < 4; i++) {
        this->electricDarkPixelIndices.push_back(i);
    }
    for(int i = 2064; i < 2068; i++) {
        this->electricDarkPixelIndices.push_back(i);
    }

    IntegrationTimeExchange *intTime = new IntegrationTimeExchange(
            ApexSpectrometerFeature::INTEGRATION_TIME_BASE);

    Transfer *unformattedSpectrum = new ReadSpectrumExchange(
            readoutLength, this->numberOfPixels);

    /* The Apex uses the same protocol as the Maya so this just
     * delegates to the existing exchange.
     */
    Transfer *formattedSpectrum = new MayaProSpectrumExchange(
            readoutLength, this->numberOfPixels, this);

    Transfer *requestSpectrum = new RequestSpectrumExchange();

    TriggerModeExchange *triggerMode = new TriggerModeExchange();

    OOISpectrometerProtocol *ooiProtocol = new OOISpectrometerProtocol(
            intTime, requestSpectrum, unformattedSpectrum, formattedSpectrum,
            triggerMode);

    this->protocols.push_back(ooiProtocol);

    /* The Apex does not have an external connector so it only supports
     * the default trigger mode.
     */
    this->triggerModes.push_back(
        new SpectrometerTriggerMode(SPECTROMETER_TRIGGER_MODE_NORMAL));
}

ApexSpectrometerFeature::~ApexSpectrometerFeature() {

}

bool ApexSpectrometerFeature::initialize(const Protocol &proto, const Bus &bus)
        throw (FeatureException) {

    /* This overrides the default GainAdjustedSpectrometerFeature::initialize
     * because the saturation level is stored in a different location.  Sadly,
     * this is also different from the NIRQuest, so each of these gets its own
     * version.
     */

    int saturation;

    EEPROMSlotFeature eeprom(18);
    vector<byte> *slot = eeprom.readEEPROMSlot(proto, bus, 0x0011);

    saturation = ((*slot)[0] & 0x00FF)
                 | (((*slot)[1] & 0x00FF) << 8);

    if(saturation <= 32768 || saturation > this->maxIntensity) {
        /* The gain adjustment was added to the Apex a bit after its
         * initial release, so there may be some units that have this slot
         * unprogrammed.  This may manifest as a value of 65535 (since
         * the EEPROM reads back 0xFF for all unprogrammed bytes).  In case
         * there was some other suspect value (zero or something small)
         * this also resets back to a safe value.
         */
        saturation = this->maxIntensity;
    }

    this->saturationLevel = saturation;
    delete slot;
    return true;
}
