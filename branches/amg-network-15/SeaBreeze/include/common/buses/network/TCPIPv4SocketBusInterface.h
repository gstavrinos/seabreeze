/***************************************************//**
 * @file    TCPIPv4SocketBusInterface.h
 * @date    February 2016
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

#ifndef SEABREEZE_TCPIPV4SOCKETBUSINTERFACE_H
#define SEABREEZE_TCPIPV4SOCKETBUSINTERFACE_H

#include "common/buses/Bus.h"
#include "native/rs232/NativeRS232.h"
#include "native/rs232/RS232.h"
#include "common/exceptions/IllegalArgumentException.h"

namespace seabreeze {
    class TCPIPv4SocketBusInterface : public Bus {
    public:
        TCPIPv4SocketBusInterface();
        virtual ~TCPIPv4SocketBusInterface();
        
        virtual TransferHelper *getHelper(
                const std::vector<ProtocolHint *> &hints) const;
        virtual BusFamily getBusFamily() const;
        
        virtual void setLocation(const DeviceLocatorInterface &location);
        virtual bool open() = 0;
        virtual void close() = 0;
        virtual DeviceLocatorInterface *getLocation();
    };
}

#endif /* SEABREEZE_TCPIPV4SOCKETBUSINTERFACE_H */

