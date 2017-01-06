/***************************************************//**
 * @file    IntrospectionFeatureAdapter.cpp
 * @date    January 2017
 * @author  Ocean Optics, Inc.
 *
 * This is a wrapper that allows
 * access to SeaBreeze IntrospectionFeatureInterface instances.
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

#include "common/globals.h"
#include "api/seabreezeapi/IntrospectionFeatureAdapter.h"
#include "api/seabreezeapi/SeaBreezeAPIConstants.h"

using namespace seabreeze;
using namespace seabreeze::api;

IntrospectionFeatureAdapter::IntrospectionFeatureAdapter(IntrospectionFeatureInterface *intf,
            const FeatureFamily &f, Protocol *p, Bus *b,
            unsigned short instanceIndex)
        : FeatureAdapterTemplate<IntrospectionFeatureInterface>(intf, f, p, b, instanceIndex) {

}


IntrospectionFeatureAdapter::~IntrospectionFeatureAdapter() {

}

#ifdef _WINDOWS
#pragma warning (disable: 4101) // unreferenced local variable
#endif
unsigned long IntrospectionFeatureAdapter::getIntrospection_example(
        int *errorCode) {

    unsigned long retval = 0;

    try {
        retval = this->feature->getIntrospection_example(*this->protocol, *this->bus);
        SET_ERROR_CODE(ERROR_SUCCESS);
    } catch (FeatureException &fe) {
        SET_ERROR_CODE(ERROR_TRANSFER_ERROR);
    }

    return retval;
}

void IntrospectionFeatureAdapter::setIntrospection_example(int *errorCode,
        const unsigned long delay_usec) {

    try {
        this->feature->setIntrospection_example(*this->protocol,
            *this->bus, delay_usec);
        SET_ERROR_CODE(ERROR_SUCCESS);
    } catch (FeatureException &fe) {
        SET_ERROR_CODE(ERROR_TRANSFER_ERROR);
    }
}

