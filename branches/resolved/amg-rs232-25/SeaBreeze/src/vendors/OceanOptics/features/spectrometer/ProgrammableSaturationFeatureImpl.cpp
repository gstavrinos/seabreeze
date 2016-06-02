/***************************************************//**
 * @file    ProgrammableSaturationFeature.cpp
 * @date    March 2016
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

#include "vendors/OceanOptics/features/spectrometer/ProgrammableSaturationFeatureImpl.h"
#include "vendors/OceanOptics/protocols/interfaces/ProgrammableSaturationProtocolInterface.h"
#include "common/exceptions/FeatureControlException.h"
#include <string>

using namespace seabreeze;
using namespace std;

ProgrammableSaturationFeatureImpl::ProgrammableSaturationFeatureImpl(
        vector<ProtocolHelper *> helpers) {
    
    /* TODO: Can these be delegated up to Feature */
    vector<ProtocolHelper *>::iterator iter;
    for(iter = helpers.begin(); iter != helpers.end(); iter++) {
        this->protocols.push_back(*iter);
    }
}

ProgrammableSaturationFeatureImpl::~ProgrammableSaturationFeatureImpl() {
    
}


bool ProgrammableSaturationFeatureImpl::initialize(const Protocol &protocol, const Bus &bus)
            throw (FeatureException) {
    return FeatureImpl::initialize(protocol, bus)
            && ProgrammableSaturationFeatureBase::initialize(protocol, bus);
}

unsigned int ProgrammableSaturationFeatureImpl::getSaturation(const Protocol &protocol,
        const Bus &bus) throw (FeatureException) {
    
    ProgrammableSaturationProtocolInterface *saturation = NULL;
    unsigned int saturationValue;
    ProtocolHelper *proto = NULL;

    try {
        proto = lookupProtocolImpl(protocol);
        saturation = static_cast<ProgrammableSaturationProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &e) {
        string error(
                "Could not find matching protocol implementation to get saturation.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    try {
        saturationValue = saturation->getSaturation(bus);
        return saturationValue;
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureControlException(error);
    }
}

FeatureFamily ProgrammableSaturationFeatureImpl::getFeatureFamily() {
    return ProgrammableSaturationFeatureBase::getFeatureFamily();
}
