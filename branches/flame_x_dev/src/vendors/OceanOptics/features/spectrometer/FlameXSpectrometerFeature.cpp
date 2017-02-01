/***************************************************//**
 * @file    FlameXSpectrometerFeature.cpp
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
#include "vendors/OceanOptics/features/spectrometer/FlameXSpectrometerFeature.h"
#include "vendors/OceanOptics/features/wavecal/WaveCalFeature.h"
#include "vendors/OceanOptics/protocols/interfaces/WaveCalProtocolInterface.h"
#include "vendors/OceanOptics/protocols/obp/exchanges/OBPIntegrationTimeExchange.h"
#include "vendors/OceanOptics/protocols/obp/exchanges/OBPReadSpectrumWithGainExchange.h"
#include "vendors/OceanOptics/protocols/obp/exchanges/OBPRequestSpectrumExchange.h"
#include "vendors/OceanOptics/protocols/obp/exchanges/OBPTriggerModeExchange.h"
#include "vendors/OceanOptics/protocols/obp/impls/OBPSpectrometerProtocol.h"
#include "vendors/OceanOptics/protocols/obp/impls/OBPWaveCalProtocol.h"

using namespace seabreeze;
using namespace seabreeze::oceanBinaryProtocol;
using namespace std;

/* In the future, much of this will need to be probed */
/* This value should have been located in constants file for non-probable */
/* spectrometer characteristics, not buried here */

const long FlameXSpectrometerFeature::INTEGRATION_TIME_MINIMUM = 1000;
const long FlameXSpectrometerFeature::INTEGRATION_TIME_MAXIMUM = 60000000;
const long FlameXSpectrometerFeature::INTEGRATION_TIME_INCREMENT = 1000;
const long FlameXSpectrometerFeature::INTEGRATION_TIME_BASE = 1;

FlameXSpectrometerFeature::FlameXSpectrometerFeature(IntrospectionFeature *introspection, ProgrammableSaturationFeature *saturationFeature)
            : GainAdjustedSpectrometerFeature(saturationFeature) {
    
	myIntrospection = introspection;
    this->numberOfPixels = 2136;
    this->maxIntensity = 65535;

    this->integrationTimeMinimum = FlameXSpectrometerFeature::INTEGRATION_TIME_MINIMUM;
    this->integrationTimeMaximum = FlameXSpectrometerFeature::INTEGRATION_TIME_MAXIMUM;
    this->integrationTimeBase = FlameXSpectrometerFeature::INTEGRATION_TIME_BASE;
    this->integrationTimeIncrement = FlameXSpectrometerFeature::INTEGRATION_TIME_INCREMENT;

    for(int i = 14; i <= 29; i++) 
	{
        this->electricDarkPixelIndices.push_back(i);
    }

	OBPIntegrationTimeExchange *intTime = new OBPIntegrationTimeExchange(FlameXSpectrometerFeature::INTEGRATION_TIME_BASE);

	Transfer *unformattedSpectrum = new OBPReadRawSpectrumExchange(
		(numberOfPixels * 2) + 64, this->numberOfPixels);

	Transfer *formattedSpectrum = new OBPReadSpectrumWithGainExchange((numberOfPixels * 2) + 64, this->numberOfPixels, this);

	Transfer *requestSpectrum = new OBPRequestSpectrumExchange();

	OBPTriggerModeExchange *triggerMode = new OBPTriggerModeExchange();

	OBPSpectrometerProtocol *obpProtocol = new OBPSpectrometerProtocol(
		intTime, requestSpectrum, unformattedSpectrum, formattedSpectrum,
		triggerMode);
	
	this->protocols.push_back(obpProtocol);
    
	this->triggerModes.push_back(
		new SpectrometerTriggerMode(SPECTROMETER_TRIGGER_MODE_NORMAL));
	this->triggerModes.push_back(
		new SpectrometerTriggerMode(SPECTROMETER_TRIGGER_MODE_LEVEL));
	this->triggerModes.push_back(
		new SpectrometerTriggerMode(SPECTROMETER_TRIGGER_MODE_SYNCHRONIZATION));
	this->triggerModes.push_back(
		new SpectrometerTriggerMode(SPECTROMETER_TRIGGER_MODE_EDGE));

}

FlameXSpectrometerFeature::~FlameXSpectrometerFeature() {

}

vector<double> *FlameXSpectrometerFeature::getWavelengths(const Protocol &protocol,
            const Bus &bus) throw (FeatureException) {

    /* FIXME: this probably ought to attempt to create an instance based on
     * any available protocol.  However, this should suffice.
     */
    vector<ProtocolHelper *> helpers;
    helpers.push_back(new OBPWaveCalProtocol());

    WaveCalFeature WaveCal(helpers, this->numberOfPixels);

    return WaveCal.readWavelengths(protocol, bus);
}

bool FlameXSpectrometerFeature::initialize(const Protocol &protocol, const Bus &bus) throw (FeatureException)
{
	bool result = false;
	if (myIntrospection != nullptr)
	{
		this->numberOfPixels = myIntrospection->getNumberOfPixels(protocol, bus);
		this->activePixelIndices = *(myIntrospection->getActivePixelRanges(protocol, bus));
		this->electricDarkPixelIndices = *(myIntrospection->getElectricDarkPixelRanges(protocol, bus));
		this->opticalDarkPixelIndices = *(myIntrospection->getOpticalDarkPixelRanges(protocol, bus));
		
		for (auto myProtocol : this->protocols)
		{
			if (myProtocol->getProtocol().equals(protocol))
			{

				OBPIntegrationTimeExchange *intTime = new OBPIntegrationTimeExchange(FlameXSpectrometerFeature::INTEGRATION_TIME_BASE);

				Transfer *unformattedSpectrum = new OBPReadRawSpectrumExchange(
					(numberOfPixels * 2) + 64, this->numberOfPixels);

				Transfer *formattedSpectrum = new OBPReadSpectrumWithGainExchange((numberOfPixels * 2) + 64, this->numberOfPixels, this);

				Transfer *requestSpectrum = new OBPRequestSpectrumExchange();

				OBPTriggerModeExchange *triggerMode = new OBPTriggerModeExchange();

				OBPSpectrometerProtocol *anOBP = (OBPSpectrometerProtocol *)myProtocol;
				
				anOBP->Initialize(intTime, requestSpectrum, unformattedSpectrum, formattedSpectrum, triggerMode);

			}
		}
		
		result = true;
	}
	return result;
}


