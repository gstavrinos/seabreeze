/***************************************************//**
 * @file    i2cMasterFeature.h
 * @date    May 2017
 * @author  Ocean Optics, Inc.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2017, Ocean Optics Inc
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

#ifndef I2CMASTERFEATURE_H
#define I2CMASTERFEATURE_H

#include "vendors/OceanOptics/features/i2c_master/i2cMasterFeatureInterface.h"
#include "common/protocols/Protocol.h"
#include "common/features/FeatureImpl.h"
#include "common/buses/Bus.h"
#include "common/exceptions/FeatureException.h"

namespace seabreeze 
{

    class i2cMasterFeature : public FeatureImpl, public i2cMasterFeatureInterface
    {
    public:
		i2cMasterFeature(std::vector<ProtocolHelper *> helpers);
        virtual ~i2cMasterFeature();

		virtual unsigned char i2cMasterGetNumberOfBuses(
			const Protocol &protocol,
			const Bus &bus)
			throw (FeatureException);

		virtual std::vector<unsigned char> i2cMasterReadBus(
			const Protocol &protocol,
			const Bus &bus,
			unsigned char busIndex,
			unsigned char slaveAddress,
			unsigned short numberOfBytes)
			throw (FeatureException);
		
		virtual unsigned short i2cMasterWriteBus(
			const Protocol &protocol,
			const Bus &bus,
			unsigned char busIndex, 
			unsigned char slaveAddress, 
			const std::vector<unsigned char> writeData)
			throw (FeatureException);	

        /* Overriding from Feature */
        virtual FeatureFamily getFeatureFamily();

    };

}

#endif /* I2CMASTERFEATURE_H */

