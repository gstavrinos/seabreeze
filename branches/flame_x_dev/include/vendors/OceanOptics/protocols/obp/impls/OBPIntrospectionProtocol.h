/***************************************************//**
 * @file    OBPIntrospectionProtocol.h
 * @date    January 2017
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

#ifndef OBPINTROSPECTIONPROTOCOL_H
#define OBPINTROSPECTIONPROTOCOL_H

#include "common/SeaBreeze.h"
#include "common/buses/Bus.h"
#include "vendors/OceanOptics/protocols/interfaces/IntrospectionProtocolInterface.h"
#include <vector>

namespace seabreeze 
{
    namespace oceanBinaryProtocol 
	{
        class OBPIntrospectionProtocol : public IntrospectionProtocolInterface 
		{
			public:
				OBPIntrospectionProtocol();
				virtual ~OBPIntrospectionProtocol();

				virtual uint16_t getNumberOfPixels(const Bus &bus) throw (ProtocolException);

				virtual std::vector<uint32_t> *getActivePixelRanges(const Bus &bus) throw (ProtocolException);

				virtual std::vector<uint32_t> *getElectricDarkPixelRanges(const Bus &bus) throw (ProtocolException);

				virtual std::vector<uint32_t> *getOpticalDarkPixelRanges(const Bus &bus) throw (ProtocolException);
        };
    } /* end namespace oceanBinaryProtocol */
} /* end namespace seabreeze */

#endif /* OBPINTROSPECTIONPROTOCOL_H */

