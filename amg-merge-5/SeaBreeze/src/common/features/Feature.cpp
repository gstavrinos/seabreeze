/***************************************************//**
 * @file    Feature.cpp
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
#include "common/features/Feature.h"

using namespace seabreeze;
using namespace std;

Feature::Feature() {

}

Feature::~Feature() {

    vector<ProtocolHelper *>::iterator iter;

    for(iter = this->protocols.begin(); iter != this->protocols.end(); iter++) {
        delete (*iter);
    }
}

bool Feature::initialize(const Protocol &protocol, const Bus &bus)
            throw (FeatureException) {
    /* Override this to initialize device, and/or return a different status */
    return true;
}

ProtocolHelper *Feature::lookupProtocolImpl(const Protocol &protocol)
        throw (FeatureProtocolNotFoundException) {

    vector<ProtocolHelper *>::iterator iter;
    ProtocolHelper *retval = NULL;

    for(iter = this->protocols.begin(); iter != this->protocols.end(); iter++) {
        if((*iter)->getProtocol().equals(protocol)) {
            retval = *iter;
            break;
        }
    }

    if(NULL == retval) {
        string error("Could not find matching protocol implementation.");
        throw FeatureProtocolNotFoundException(error);
    }

    return retval;
}
