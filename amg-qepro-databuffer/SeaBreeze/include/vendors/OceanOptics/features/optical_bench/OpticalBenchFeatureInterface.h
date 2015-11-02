/***************************************************//**
 * @file    OpticalBenchFeatureInterface.h
 * @date    January 2015
 * @author  Ocean Optics, Inc., Kirk Clendinning, Heliospectra
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2015, Ocean Optics Inc
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

#ifndef OPTICALBENCHFEATUREINTERFACE_H
#define OPTICALBENCHFEATUREINTERFACE_H

#include "common/protocols/Protocol.h"
#include "common/buses/Bus.h"
#include "common/exceptions/FeatureException.h"

namespace seabreeze {

    class OpticalBenchFeatureInterface {
    public:
        virtual ~OpticalBenchFeatureInterface() = 0;
        virtual unsigned short int readOpticalBenchFiberDiameterMicrons(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) = 0;
        virtual unsigned short int readOpticalBenchSlitWidthMicrons(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) = 0;
        virtual std::string *readOpticalBenchID(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) = 0;
        virtual std::string *readOpticalBenchSerialNumber(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) = 0;
        virtual std::string *readOpticalBenchCoating(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) = 0;
        virtual std::string *readOpticalBenchFilter(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) = 0;
        virtual std::string *readOpticalBenchGrating(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) = 0;                
    };

    /* Default implementation for (otherwise) pure virtual destructor */
    inline OpticalBenchFeatureInterface::~OpticalBenchFeatureInterface() {}
}

#endif /* OPTICALBENCHFEATUREINTERFACE_H */