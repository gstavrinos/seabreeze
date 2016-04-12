/***************************************************//**
 * @file    OBPMessage.h
 * @date    January 2011
 * @author  Ocean Optics, Inc.
 *
 * All messages in the Ocean Binary Protocol begin with
 * a standard 64-byte header.  It is always safe to read
 * 64 bytes for the start of a new transfer from a device
 * that supports this protocol, which works nicely with
 * the USB minimum packet size.
 *
 * This class simplifies the task of getting data into
 * and out of the header.
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

#ifndef OBPMESSAGE_H
#define OBPMESSAGE_H

#include <vector>
#include "common/SeaBreeze.h"
#include "common/exceptions/IllegalArgumentException.h"

namespace seabreeze {
  namespace oceanBinaryProtocol {
    class OBPMessage {
    public:
        OBPMessage();
        ~OBPMessage();

        static OBPMessage *parseHeaderFromByteStream(std::vector<byte> *stream) throw (IllegalArgumentException);
        static OBPMessage *parseByteStream(std::vector<byte> *stream) throw (IllegalArgumentException);

        std::vector<byte> *toByteStream();
        std::vector<byte> *getData();
        unsigned int getBytesRemaining();
        byte getChecksumType();
        unsigned short getErrno();
        unsigned short getFlags();
        std::vector<byte> *getImmediateData();
        byte getImmediateDataLength();
        unsigned int getMessageType();
        std::vector<byte> *getPayload();
        unsigned short getProtocolVersion();
        unsigned int getRegarding();

        bool isAckFlagSet();
        bool isNackFlagSet();

        void setAckRequestedFlag();
        void setBytesRemaining(unsigned int bytesRemaining);
        void setChecksumType(byte checksumType);
        void setData(std::vector<byte> *data);
        void setErrorNumber(unsigned short errorNumber);
        void setFlags(unsigned short flags);
        void setImmediateData(std::vector<byte> *immediateData);
        void setImmediateDataLength(byte immediateDataLength);
        void setMessageType(unsigned int messageType);
        void setPayload(std::vector<byte> *payload);
        void setProtocolVersion(unsigned short protocolVersion);
        void setRegarding(unsigned int regarding);

    protected:
        void setupMessage();
        std::vector<byte> *header;
        unsigned short protocolVersion;
        unsigned short flags;
        unsigned short errorNumber;
        unsigned int messageType;
        unsigned int regarding;
        byte checksumType;
        byte immediateDataLength;
        std::vector<byte> *immediateData;
        unsigned int bytesRemaining;
        std::vector<byte> *payload;
        std::vector<byte> *checksum;
        std::vector<byte> *footer;
    };

  }
}

#endif /* OBPMESSAGE_H */
