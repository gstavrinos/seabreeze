/***************************************************//**
 * @file    OBPIPv4Protocol.h
 * @date    March 2017
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

#ifndef OBPIPV4PROTOCOL_H
#define OBPIPV4PROTOCOL_H

#include "common/buses/Bus.h"
#include "vendors/OceanOptics/protocols/interfaces/IPv4ProtocolInterface.h"
#include "common/exceptions/ProtocolException.h"

namespace seabreeze {
  namespace oceanBinaryProtocol {
    class OBPIPv4Protocol : public IPv4ProtocolInterface {
    public:
        OBPIPv4Protocol();

        virtual ~OBPIPv4Protocol();

        /* Inherited from OBPIPv4ProtocolInterface */
        virtual unsigned char get_IPv4_DHCP_Enable_State(const Bus &bus, unsigned char interfaceIndex)
            throw (ProtocolException);

        virtual void set_IPv4_DHCP_Enable_State(const Bus &bus, unsigned char interfaceIndex, unsigned char enableState)
            throw (ProtocolException);

        virtual unsigned char get_Number_Of_IPv4_Addresses(const Bus &bus, unsigned char interfaceIndex)
            throw (ProtocolException);
 
        virtual void get_IPv4_Address(const Bus &bus, unsigned char interfaceIndex, unsigned char addressIndex,  std::vector<unsigned char> *IPv4_Address,  unsigned char *netMask)
            throw (ProtocolException);

        virtual std::vector<byte> get_IPv4_Default_Gateway(const Bus &bus, unsigned char interfaceIndex)
            throw (ProtocolException);

        virtual void set_IPv4_Default_Gateway(const Bus &bus, unsigned char interfaceIndex, const std::vector<byte> IPv4_Gateway_Address)
            throw (ProtocolException);

        virtual void add_IPv4_Address(const Bus &bus, unsigned char interfaceIndex, const std::vector<byte> IPv4_Address, unsigned char netMask)
            throw (ProtocolException);

        virtual void delete_IPv4_Address(const Bus &bus, unsigned char interfaceIndex, unsigned char addressIndex)
            throw (ProtocolException);

    };
  }
}

#endif /* OBPIPV4PROTOCOL_H */
