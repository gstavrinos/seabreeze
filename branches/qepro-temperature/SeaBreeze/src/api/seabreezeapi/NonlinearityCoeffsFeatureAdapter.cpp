/***************************************************//**
 * @file    NonlinearityCoeffsFeatureAdapter.cpp
 * @date    February 2012
 * @author  Ocean Optics, Inc.
 *
 * This is a wrapper that allows access to SeaBreeze
 * NonlinearityCoeffsFeatureInterface instances.
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
#include "api/seabreezeapi/SeaBreezeAPIConstants.h"
#include "api/seabreezeapi/NonlinearityCoeffsFeatureAdapter.h"
#include <string.h> /* for memcpy pre c++11 */
#include <vector>

using namespace seabreeze;
using namespace seabreeze::api;
using namespace std;

NonlinearityCoeffsFeatureAdapter::NonlinearityCoeffsFeatureAdapter(
        NonlinearityCoeffsFeatureInterface *intf, const FeatureFamily &f,
                    Protocol *p, Bus *b, unsigned short instanceIndex)
        : FeatureAdapterTemplate<NonlinearityCoeffsFeatureInterface>(intf, f, p, b, instanceIndex) {

    /* Nothing else to do here, the initialization list takes care of it */
}

NonlinearityCoeffsFeatureAdapter::~NonlinearityCoeffsFeatureAdapter() {
    /* This is just a wrapper around existing instances -- nothing to delete */
}

#ifdef _WINDOWS
#pragma warning (disable: 4101) // unreferenced local variable
#endif
int NonlinearityCoeffsFeatureAdapter::readNonlinearityCoeffs(int *errorCode, double *buffer,
                    int bufferLength) {
    int doublesCopied = 0;

    vector<double> *cal;

    try {
        cal = this->feature->readNonlinearityCoefficients(*this->protocol, *this->bus);
        if(NULL == cal) {
            /* This may happen if the device does not have any coefficients
             * stored.  An exception is not likely to be thrown because the
             * driver itself did not fail, there was just no data.
             */
            SET_ERROR_CODE(ERROR_VALUE_NOT_FOUND);
            return 0;
        }
        int doubles = (int) cal->size();
        doublesCopied = (doubles < bufferLength) ? doubles : bufferLength;
        memcpy(buffer, &((*cal)[0]), doublesCopied * sizeof(double));

        delete cal;
        SET_ERROR_CODE(ERROR_SUCCESS);
    } catch (FeatureException &fe) {
        SET_ERROR_CODE(ERROR_TRANSFER_ERROR);
        return 0;
    }
    return doublesCopied;
}
