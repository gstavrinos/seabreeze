/***************************************************//**
 * @file    OOISpectrometerFeature.cpp
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
#include "vendors/OceanOptics/features/spectrometer/OOISpectrometerFeature.h"
#include "vendors/OceanOptics/protocols/interfaces/SpectrometerProtocolInterface.h"
#include "vendors/OceanOptics/features/eeprom_slots/WavelengthEEPROMSlotFeature.h"
#include "common/exceptions/FeatureProtocolNotFoundException.h"
#include "common/exceptions/FeatureControlException.h"
#include "vendors/OceanOptics/protocols/ooi/impls/OOISpectrometerProtocol.h"
#include "api/seabreezeapi/FeatureFamilies.h"
#include "common/Log.h"

using namespace seabreeze;
using namespace seabreeze::api;
using namespace seabreeze::ooiProtocol;
using namespace std;

#ifdef _WINDOWS
#pragma warning (disable: 4101) // unreferenced local variable
#endif

OOISpectrometerFeature::OOISpectrometerFeature() {

}

OOISpectrometerFeature::~OOISpectrometerFeature() {
    vector<SpectrometerTriggerMode *>::iterator iter;

    for(iter = this->triggerModes.begin(); iter != this->triggerModes.end(); iter++) {
        delete *iter;
    }
}

vector<double> *OOISpectrometerFeature::getSpectrum(const Protocol &protocol, const Bus &bus) throw (FeatureException) {

    LOG(__FUNCTION__);
    // logger.debug("starting OOISpectrometerFeature::getSpectrum");

    ProtocolHelper *proto;
    SpectrometerProtocolInterface *spec;

    try {
        proto = lookupProtocolImpl(protocol);
        spec = static_cast<SpectrometerProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &e) {
        string error("Could not find matching protocol implementation to get raw spectrum.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    logger.debug("writing requestSpectrum");
    writeRequestSpectrum(protocol, bus);

    vector<double> *retval = NULL;

    try {
        logger.debug("reading spectrum");
        retval = spec->readSpectrum(bus);
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureControlException(error);
    }

    return retval;
}

vector<byte> *OOISpectrometerFeature::getUnformattedSpectrum(
        const Protocol &protocol, const Bus &bus) throw (FeatureException) {
    LOG(__FUNCTION__);
    writeRequestSpectrum(protocol, bus);
    return readUnformattedSpectrum(protocol, bus);
}

void OOISpectrometerFeature::writeRequestSpectrum(const Protocol &protocol,
        const Bus &bus) throw (FeatureException) {

    LOG(__FUNCTION__);
    // logger.debug("starting OOISpectrometerFeature::writeRequestSpectrum");

    ProtocolHelper *proto;
    SpectrometerProtocolInterface *spec;

    try {
        proto = lookupProtocolImpl(protocol);
        spec = static_cast<SpectrometerProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &e) {
        string error("Could not find matching protocol implementation to get raw spectrum.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        logger.error(error.c_str());
        throw FeatureProtocolNotFoundException(error);
    }

    try {
        spec->requestSpectrum(bus);
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        /* FIXME: previous exception should probably be bundled up into the new exception */
        logger.error(error.c_str());
        throw FeatureControlException(error);
    }
}

vector<byte> *OOISpectrometerFeature::readUnformattedSpectrum(const Protocol &protocol,
        const Bus &bus) throw (FeatureException) {
    LOG(__FUNCTION__);
    // logger.debug("starting OOISpectrometerFeature::readUnformattedSpectrum");

    ProtocolHelper *proto;
    SpectrometerProtocolInterface *spec;

    try {
        proto = lookupProtocolImpl(protocol);
        spec = static_cast<SpectrometerProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &e) {
        string error("Could not find matching protocol implementation to get raw spectrum.");
        logger.error(error.c_str());
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    vector<byte> *retval;

    try {
        retval = spec->readUnformattedSpectrum(bus);
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        logger.error(error.c_str());
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureControlException(error);
    }

    return retval;
}

vector<double> *OOISpectrometerFeature::getWavelengths(const Protocol &protocol,
        const Bus &bus) throw (FeatureException) {

    WavelengthEEPROMSlotFeature wlFeature(this->numberOfPixels);

    return wlFeature.readWavelengths(protocol, bus);
}

void OOISpectrometerFeature::setIntegrationTimeMicros(const Protocol &protocol,
        const Bus &bus, unsigned long time_usec)
throw (FeatureException, IllegalArgumentException) {

    ProtocolHelper *proto;
    SpectrometerProtocolInterface *spec;

    try {
        proto = lookupProtocolImpl(protocol);
        spec = static_cast<SpectrometerProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &e) {
        string error("Could not find matching protocol implementation to get raw spectrum.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    unsigned long iMin = this->integrationTimeMinimum / this->integrationTimeBase;
    unsigned long iMax = this->integrationTimeMaximum / this->integrationTimeBase;
    unsigned long iTime = time_usec / this->integrationTimeBase;

    if(iTime >= iMin && iTime <= iMax) {
        try {
            spec->setIntegrationTimeMicros(bus, time_usec);
        } catch (ProtocolException &pe) {
            string error("Caught protocol exception: ");
            error += pe.what();
            /* FIXME: previous exception should probably be bundled up into the new exception */
            throw FeatureControlException(error);
        }
    } else {
        string error("Specified integration time is out of range.");
        throw IllegalArgumentException(error);
    }
}

void OOISpectrometerFeature::setTriggerMode(const Protocol &protocol,
        const Bus &bus, SpectrometerTriggerMode &mode)
        throw (FeatureException) {
    LOG(__FUNCTION__);

    ProtocolHelper *proto;
    SpectrometerProtocolInterface *spec;

    bool validMode = false;
    vector<SpectrometerTriggerMode *>::iterator iter;

    for(iter = this->triggerModes.begin(); iter != this->triggerModes.end(); iter++) {
        if(mode == *(*iter)) {
            validMode = true;
            break;
        }
    }

    if(false == validMode) {
        string error("Specified trigger mode is not supported.");
        logger.error(error.c_str());
        throw FeatureException(error);
    }

    try {
        proto = lookupProtocolImpl(protocol);
        spec = static_cast<SpectrometerProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &e) {
        string error("Could not find matching protocol implementation to set trigger mode.");
        logger.error(error.c_str());
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    try {
        spec->setTriggerMode(bus, mode);
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        logger.error(error.c_str());
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureControlException(error);
    }

}

vector<SpectrometerTriggerMode *> OOISpectrometerFeature::getTriggerModes() const {
    return this->triggerModes;
}

vector<int> OOISpectrometerFeature::getElectricDarkPixelIndices() const {
    return this->electricDarkPixelIndices;
}

long OOISpectrometerFeature::getIntegrationTimeMinimum() const {
    return this->integrationTimeMinimum;
}

long OOISpectrometerFeature::getIntegrationTimeMaximum() const {
    return this->integrationTimeMaximum;
}

long OOISpectrometerFeature::getIntegrationTimeIncrement() const {
    return this->integrationTimeIncrement;
}

int OOISpectrometerFeature::getNumberOfPixels() const {
    return this->numberOfPixels;
}

int OOISpectrometerFeature::getMaximumIntensity() const {
    return this->maxIntensity;
}

FeatureFamily OOISpectrometerFeature::getFeatureFamily() {
    FeatureFamilies families;

    return families.SPECTROMETER;
}

