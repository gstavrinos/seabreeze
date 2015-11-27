/***************************************************//**
 * @file    SpectrumProcessingFeatureAdapter.h
 * @date    January 2015
 * @author  Ocean Optics, Inc., Kirk Clendinning, Heliospectra
 *
 * This is a wrapper that allows
 * access to SeaBreeze SpectrumProcessingFeatureInterface instances.
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

#ifndef SEABREEZE_SPECTRUMPROCESSINGFEATUREADAPTER_H
#define SEABREEZE_SPECTRUMPROCESSINGFEATUREADAPTER_H

#include "api/seabreezeapi/FeatureAdapterTemplate.h"
#include "vendors/OceanOptics/features/spectrum_processing/SpectrumProcessingFeatureInterface.h"

namespace seabreeze {
    namespace api {

        class SpectrumProcessingFeatureAdapter
                : public FeatureAdapterTemplate<SpectrumProcessingFeatureInterface> {
        public:
            SpectrumProcessingFeatureAdapter(SpectrumProcessingFeatureInterface *intf,
                    const FeatureFamily &f,
                    Protocol *p, Bus *b, unsigned short instanceIndex);
            virtual ~SpectrumProcessingFeatureAdapter();

			unsigned char readSpectrumProcessingBoxcarWidth(int *errorCode);
            unsigned short int readSpectrumProcessingScansToAverage(int *errorCode);
            void writeSpectrumProcessingBoxcarWidth(int *errorCode, unsigned char boxcarWidth);
            void writeSpectrumProcessingScansToAverage(int *errorCode, unsigned short int scansToAverage);

        };

    }
}

#endif