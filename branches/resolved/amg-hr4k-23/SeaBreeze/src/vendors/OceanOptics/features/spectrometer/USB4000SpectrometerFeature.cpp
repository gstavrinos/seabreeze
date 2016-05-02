/***************************************************//**
 * @file    USB4000SpectrometerFeature.cpp
 * @date    May 2009
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
#include "vendors/OceanOptics/features/spectrometer/USB4000SpectrometerFeature.h"
#include "vendors/OceanOptics/protocols/ooi/exchanges/IntegrationTimeExchange.h"
#include "vendors/OceanOptics/protocols/ooi/exchanges/ReadSpectrumExchange.h"
#include "vendors/OceanOptics/protocols/ooi/exchanges/USBFPGASpectrumExchange.h"
#include "vendors/OceanOptics/protocols/ooi/exchanges/RequestSpectrumExchange.h"
#include "vendors/OceanOptics/protocols/ooi/exchanges/TriggerModeExchange.h"
#include "vendors/OceanOptics/protocols/ooi/impls/OOISpectrometerProtocol.h"

using namespace seabreeze;
using namespace ooiProtocol;

const long USB4000SpectrometerFeature::INTEGRATION_TIME_MINIMUM = 10;
const long USB4000SpectrometerFeature::INTEGRATION_TIME_MAXIMUM = 655350000;
const long USB4000SpectrometerFeature::INTEGRATION_TIME_INCREMENT = 10;
const long USB4000SpectrometerFeature::INTEGRATION_TIME_BASE = 1;

USB4000SpectrometerFeature::USB4000SpectrometerFeature(
        ProgrammableSaturationFeature *saturationFeature)
            : GainAdjustedSpectrometerFeature(saturationFeature) {
    int readoutLength = 7681;

    this->numberOfPixels = 3648;
    this->maxIntensity = 65535;

    this->integrationTimeMinimum = USB4000SpectrometerFeature::INTEGRATION_TIME_MINIMUM;
    this->integrationTimeMaximum = USB4000SpectrometerFeature::INTEGRATION_TIME_MAXIMUM;
    this->integrationTimeBase = USB4000SpectrometerFeature::INTEGRATION_TIME_BASE;
    this->integrationTimeIncrement = USB4000SpectrometerFeature::INTEGRATION_TIME_INCREMENT;

    // safest range for USB4000 and Flame-T is (5, 15)
    for(int i = 5; i < 16; i++) {
        this->electricDarkPixelIndices.push_back(i);
    }

    IntegrationTimeExchange *intTime = new IntegrationTimeExchange(
            USB4000SpectrometerFeature::INTEGRATION_TIME_BASE);

    Transfer *unformattedSpectrum = new ReadSpectrumExchange(
            readoutLength, this->numberOfPixels);

    Transfer *formattedSpectrum = new USBFPGASpectrumExchange(
            readoutLength, this->numberOfPixels, this);

    Transfer *requestSpectrum = new RequestSpectrumExchange();

    TriggerModeExchange *triggerMode = new TriggerModeExchange();

    OOISpectrometerProtocol *ooiProtocol = new OOISpectrometerProtocol(
            intTime, requestSpectrum, unformattedSpectrum, formattedSpectrum,
            triggerMode);

    this->protocols.push_back(ooiProtocol);

    this->triggerModes.push_back(
        new SpectrometerTriggerMode(SPECTROMETER_TRIGGER_MODE_NORMAL));
    this->triggerModes.push_back(
        new SpectrometerTriggerMode(SPECTROMETER_TRIGGER_MODE_SOFTWARE));
    this->triggerModes.push_back(
        new SpectrometerTriggerMode(SPECTROMETER_TRIGGER_MODE_SYNCHRONIZATION));
    this->triggerModes.push_back(
        new SpectrometerTriggerMode(SPECTROMETER_TRIGGER_MODE_HARDWARE));

}

USB4000SpectrometerFeature::~USB4000SpectrometerFeature() {

}
