/***************************************************//**
 * @file    OOISpectrometerFeatureInterface.h
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

#ifndef OOISPECTROMETERFEATUREINTERFACE_H
#define OOISPECTROMETERFEATUREINTERFACE_H

#include <vector>
#include "common/protocols/Protocol.h"
#include "common/buses/Bus.h"
#include "common/exceptions/FeatureException.h"
#include "common/exceptions/IllegalArgumentException.h"
#include "vendors/OceanOptics/features/spectrometer/SpectrometerTriggerMode.h"

namespace seabreeze {

    class OOISpectrometerFeatureInterface {
    public:
        virtual ~OOISpectrometerFeatureInterface() = 0;

        /* Request and read out a spectrum formatted into intensity (A/D counts) */
        virtual std::vector<double> *getSpectrum(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) = 0;

        /* Request and read out the raw spectrum data stream */
        virtual std::vector<byte> *getUnformattedSpectrum(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) = 0;

        /* Request and read out the wavelengths in nanometers as a vector of doubles */
        virtual std::vector<double> *getWavelengths(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) = 0;

        /* Read the raw spectrum data stream.  No request is made first. */
        virtual std::vector<byte> *readUnformattedSpectrum(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) = 0;

        /* Set the integration time of the spectrometer */
        virtual void setIntegrationTimeMicros(const Protocol &protocol,
                const Bus &bus, unsigned long time_usec)
                throw (FeatureException, IllegalArgumentException) = 0;

        /* Request that the spectrometer make a spectrum available for
         * reading (e.g. with readUnformattedSpectrum())
         */
        virtual void writeRequestSpectrum(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) = 0;

        /* Setting the external trigger mode for the spectrometer */
        virtual void setTriggerMode(const Protocol &protocol,
                const Bus &bus, SpectrometerTriggerMode &mode) throw (FeatureException) = 0;

        virtual std::vector<SpectrometerTriggerMode *> getTriggerModes() const = 0;

        virtual std::vector<int> getElectricDarkPixelIndices() const = 0;

        virtual long getIntegrationTimeMinimum() const = 0;
        virtual long getIntegrationTimeMaximum() const = 0;
        virtual long getIntegrationTimeIncrement() const = 0;

        virtual int getNumberOfPixels() const = 0;
        virtual int getMaximumIntensity() const = 0;

    };

    /* Default implementation for (otherwise) pure virtual destructor */
    inline OOISpectrometerFeatureInterface::~OOISpectrometerFeatureInterface() {}

}

#endif /* OOISPECTROMETERFEATUREINTERFACE_H */
