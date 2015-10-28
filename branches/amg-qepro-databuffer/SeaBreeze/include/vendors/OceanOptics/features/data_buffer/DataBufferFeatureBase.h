/***************************************************//**
 * @file    DataBufferFeature.h
 * @date    October 2015
 * @author  Ocean Optics, Inc.
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

#ifndef DATABUFFERFEATUREBASE_H
#define DATABUFFERFEATUREBASE_H

#include <vector>

#include "vendors/OceanOptics/features/data_buffer/DataBufferFeatureInterface.h"
#include "common/features/Feature.h"

namespace seabreeze {

    class DataBufferFeatureBase : public Feature, public DataBufferFeatureInterface {
    public:
        DataBufferFeatureBase();
        virtual ~DataBufferFeatureBase();

        virtual DataBufferCount_t getNumberOfBuffers();

        /* These functions remain abstract because they will depend on the
         * device implementation.
         */
        virtual void clearBuffer(const Protocol &protocol, const Bus &bus,
                const DataBufferIndex_t bufferIndex)
                throw (FeatureException) = 0;
        virtual DataBufferElementCount_t getNumberOfElements(
                const Protocol &protocol, const Bus &bus,
                const DataBufferIndex_t bufferIndex)
                throw (FeatureException) = 0;
        virtual DataBufferElementCount_t getBufferCapacity(
                const Protocol &protocol,
                const Bus &bus, const DataBufferIndex_t bufferIndex)
                throw (FeatureException) = 0;
        virtual DataBufferElementCount_t getBufferCapacityMinimum(
                const Protocol &protocol, const Bus &bus,
                const DataBufferIndex_t bufferIndex)
                throw (FeatureException) = 0;
        virtual DataBufferElementCount_t getBufferCapacityMaximum(
                const Protocol &protocol, const Bus &bus,
                const DataBufferIndex_t bufferIndex)
                throw (FeatureException) = 0;
        virtual void setBufferCapacity(const Protocol &protocol, const Bus &bus,
                const DataBufferIndex_t bufferIndex,
                const DataBufferElementCount_t bufferSize)
                throw (FeatureException) = 0;

        /* Overriding from Feature */
        /* initialize is still abstract in case the device needs
         * to probe anything with regards to device capabilities
         */
        virtual bool initialize(const Protocol &protocol, const Bus &bus,
                throw (FeatureException) = 0;
        virtual FeatureFamily getFeatureFamily();

    protected:
        DataBufferIndex_t numberOfBuffers;
    };

} /* end namespace */

#endif /* DATABUFFERFEATUREBASE_H */

