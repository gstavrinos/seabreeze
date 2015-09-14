/***************************************************//**
 * @file    OBPLightSourceIntensityQuery.cpp
 * @date    April 2013
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
#include "vendors/OceanOptics/protocols/obp/exchanges/OBPLightSourceIntensityQuery.h"
#include "vendors/OceanOptics/protocols/obp/hints/OBPControlHint.h"
#include "vendors/OceanOptics/protocols/obp/constants/OBPMessageTypes.h"
#include <vector>

using namespace seabreeze;
using namespace seabreeze::oceanBinaryProtocol;
using namespace std;

OBPLightSourceIntensityQuery::OBPLightSourceIntensityQuery(int module,
            int source) {

    this->hints->push_back(new OBPControlHint());

    this->messageType = OBPMessageTypes::OBP_GET_LIGHT_SOURCE_INTENSITY;

    this->payload.resize(2);

    this->moduleIndex = module;
    this->lightSourceIndex = source;
}

OBPLightSourceIntensityQuery::~OBPLightSourceIntensityQuery() {

}

float OBPLightSourceIntensityQuery::queryIntensity(TransferHelper *helper)
        throw (ProtocolException) {

    vector<byte>::iterator iter;
    float retval = 0;
    unsigned int i;
    byte *cptr = NULL;
    vector<byte> *result;

    this->payload[0] = (byte)this->moduleIndex;
    this->payload[1] = (byte)this->lightSourceIndex;

    result = this->queryDevice(helper);
    if(NULL == result || result->size() < sizeof(float)) {
        throw ProtocolException("Got back no data when trying to get enable status");
    }

    cptr = (byte *)&retval;

    for(iter = result->begin(), i = 0;
                iter != result->end() && i < sizeof(float); iter++, i++) {
        /* FIXME: this assumes the host is little-endian.  Is this safe? */
        cptr[i] = *iter;
    }

    delete result;

    return retval;
}
