/***************************************************//**
 * @file    FlameNIRSpectrumExchange.h
 * @date    Apr 2016
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

#ifndef SEABREEZE_FLAMENIRSPECTRUMEXCHANGE_H
#define SEABREEZE_FLAMENIRSPECTRUMEXCHANGE_H

#include "common/Data.h"
#include "vendors/OceanOptics/protocols/ooi/exchanges/ReadSpectrumExchange.h"
#include "vendors/OceanOptics/features/spectrometer/GainAdjustedSpectrometerFeature.h"

namespace seabreeze {
  namespace ooiProtocol {
    /**
     * @brief   Spectrum exchange for FlameNIR spectrometer
     *
     * This class was needed because unlike other FX2-era spectrometers,
     * the Flame-NIR does not return a "sync byte" at the end of a spectrum.
     */
    class FlameNIRSpectrumExchange : public ReadSpectrumExchange {
    public:
        FlameNIRSpectrumExchange(unsigned int readoutLength,
                unsigned int numberOfPixels, GainAdjustedSpectrometerFeature *feature);
        virtual ~FlameNIRSpectrumExchange();

        virtual Data *transfer(TransferHelper *helper) throw (ProtocolException);

    protected:
        GainAdjustedSpectrometerFeature *spectrometerFeature;
    };
  }
}

#endif
