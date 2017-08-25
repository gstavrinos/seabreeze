/***************************************************//**
 * @file    OBPReadNumberOfRawSpectraWithMetadataExchange.cpp
 * @date    September 2017
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

#include <stdint.h>
#include "common/globals.h"
#include "vendors/OceanOptics/protocols/obp/exchanges/OBPReadNumberOfRawSpectraWithMetadataExchange.h"
#include "vendors/OceanOptics/protocols/obp/hints/OBPSpectrumHint.h"
#include "vendors/OceanOptics/protocols/obp/constants/OBPMessageTypes.h"
#include "vendors/OceanOptics/protocols/obp/exchanges/OBPMessage.h"
#include "common/ByteVector.h"

using namespace seabreeze;
using namespace seabreeze::oceanBinaryProtocol;
using namespace std;
using std::vector;

#ifdef _WINDOWS
#pragma warning (disable: 4101) // unreferenced local variable
#endif

#define METADATA_LENGTH         64
#define OBP_MESSAGE_OVERHEAD    64

OBPReadNumberOfRawSpectraWithMetadataExchange::OBPReadNumberOfRawSpectraWithMetadataExchange(unsigned int pixels, unsigned int numberOfBytesPerPixel) 
{

	derivedClassPointer = this;
	setParametersFunction = &(this->setNumberOfSamplesToRequest);

    this->hints->push_back(new OBPSpectrumHint());
    this->direction = Transfer::FROM_DEVICE;

    this->metadataLength = METADATA_LENGTH;
	this->checkSumLength = sizeof(unsigned int);
    setNumberOfPixels(pixels);
	setNumberOfBytesPerPixel(numberOfBytesPerPixel);

}

OBPReadNumberOfRawSpectraWithMetadataExchange::~OBPReadNumberOfRawSpectraWithMetadataExchange() 
{

}

void OBPReadNumberOfRawSpectraWithMetadataExchange::setNumberOfPixels(int pixels) 
{
    this->numberOfPixels = pixels;
}

void OBPReadNumberOfRawSpectraWithMetadataExchange::setNumberOfBytesPerPixel(int numberOfBytes)
{
	this->numberOfBytesPerPixel = numberOfBytes;
}

unsigned int OBPReadNumberOfRawSpectraWithMetadataExchange::isLegalMessageType(unsigned int t) 
{
    if(OBPMessageTypes::OBP_GET_N_BUF_RAW_SPECTRA_META == t) 
	{
        return 1;
    }

    /* More message types can be added as legal if required, or this function
     * can be overridden so that subclasses can specify more exactly what kind
     * of message they are meant to decode.
     */

    return 0;
}

#define OBP_PAYLOAD_START 44

Data *OBPReadNumberOfRawSpectraWithMetadataExchange::transfer(TransferHelper *helper)
        throw (ProtocolException) 
{
    Data *xfer;
    OBPMessage *message = NULL;
    vector<byte> *bytes;
    vector<byte> obpHeader(OBP_PAYLOAD_START, 0);
    int flag = 0;

    // this particular call can return less than the number of bytes requested and still be valid.
    //  read the OBP header first, then request the remaining bytes.
    try {
        flag = helper->receive(obpHeader, OBP_PAYLOAD_START);
        if(((unsigned int)flag) != OBP_PAYLOAD_START) {
            /* FIXME: retry, throw exception, something here */
        }
    } catch (BusException &be) {
        string error("Failed to write to bus.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        /* FIXME: there is probably a more descriptive type for this than ProtocolException */
        throw ProtocolException(error);
    }

    this->length = *reinterpret_cast<const uint32_t*>(&obpHeader[OBP_PAYLOAD_START-sizeof(uint32_t)]);

    // the original vector does not need to be resized because it will be the same size, or larger than the
    //  data sent back by the spectrometer, since this->length was the maximum number of spectra to be returned
    // the next transfer will retrieve the number of remaining bytes.

    /* This will use the superclass to transfer data from the device
     */
    xfer = Transfer::transfer(helper);
    if(NULL == xfer) 
	{
        string error("Expected Transfer::transfer to produce a non-null result "
                        "containing raw spectral data.  Without this data, it is not "
                        "possible to generate a valid formatted spectrum.");
        throw ProtocolException(error);
    }
    /* xfer is a copy of what is already stored in this->buffer so delete it.
     */
    delete xfer;

    /* Try to parse the buffer into an OBPMessage.  This may throw an exception
     * if the message is badly formed.
     */
    try 
	{

        // the current buffer does not include the header. For the parse to work, those
        //  44 bytes must be inserted at the front of this->buffer
        this->buffer->insert(this->buffer->begin(), obpHeader.begin(), obpHeader.end());

        message = OBPMessage::parseByteStream(this->buffer);
    } 
	catch (IllegalArgumentException &iae) 
	{
        string error("Failed to parse message transferred from device");
        throw ProtocolException(error);
    }

    if(0 == isLegalMessageType(message->getMessageType())) 
	{
        string error("Did not get expected message type, got ");
        error += (char)(message->getMessageType());
        throw ProtocolException(error);
    }

    bytes = message->getData();
    if((bytes->size() % ((numberOfPixels * numberOfBytesPerPixel)+metadataLength+checkSumLength)) != 0) // the number of bytes should be an integral of the spectrum size
	{
        string error("Spectrum response does not have enough data.");
        delete message;
        throw ProtocolException(error);
    }
    /* This incurs a copy of the buffer so it is safe to delete the message. */
    ByteVector *retval = new ByteVector(*bytes);
    delete message;

    return retval;
}


void OBPReadNumberOfRawSpectraWithMetadataExchange::setNumberOfSamplesToRequest(void *myClass, unsigned int numberOfSamples)
{
	unsigned int readoutLength;
    unsigned int sampleSize = 1;

    // the spectrometer currently 20170822 will return one spectrum, if available, even if 0 were requested. Make sure there is space for the rouge transmission.
    if(numberOfSamples != 0)
        sampleSize = numberOfSamples;

	OBPReadNumberOfRawSpectraWithMetadataExchange *parentClass = (OBPReadNumberOfRawSpectraWithMetadataExchange *)myClass;

	parentClass->numberOfSamplesToRetrieve = numberOfSamples;
	readoutLength = (((parentClass->numberOfPixels * parentClass->numberOfBytesPerPixel) + parentClass->metadataLength + parentClass->checkSumLength) * sampleSize) + OBP_MESSAGE_OVERHEAD;
	
	parentClass->buffer->resize(readoutLength);
	parentClass->length = readoutLength;
	parentClass->checkBufferSize();
}