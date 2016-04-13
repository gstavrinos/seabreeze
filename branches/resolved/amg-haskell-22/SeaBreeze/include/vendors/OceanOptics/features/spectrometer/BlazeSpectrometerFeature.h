/***************************************************//**
 * @file    BlazeSpectrometerFeature.h
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

#ifndef BLAZESPECTROMETERFEATURE_H
#define BLAZESPECTROMETERFEATURE_H

#include "vendors/OceanOptics/features/spectrometer/GainAdjustedSpectrometerFeature.h"

namespace seabreeze {

    class BlazeSpectrometerFeature : public GainAdjustedSpectrometerFeature {
    public:
        BlazeSpectrometerFeature(
                ProgrammableSaturationFeature *saturationFeature);
        virtual ~BlazeSpectrometerFeature();

        /* The Blaze gets wavelengths a bit differently */
        virtual std::vector<double> *getWavelengths(const Protocol &protocol,
            const Bus &bus) throw (FeatureException);

    private:
        static const long INTEGRATION_TIME_MINIMUM;
        static const long INTEGRATION_TIME_MAXIMUM;
        static const long INTEGRATION_TIME_INCREMENT;
        static const long INTEGRATION_TIME_BASE;
    };

}

#endif /* BLAZESPECTROMETERFEATURE_H */
