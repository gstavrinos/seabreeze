/***************************************************//**
 * @file    Socket.h
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

#ifndef SEABREEZE_SOCKET_H
#define SEABREEZE_SOCKET_H

/* Includes */
#include "common/exceptions/BusTransferException.h"
#include "common/exceptions/BusConnectException.h"
#include "native/network/UnknownHostException.h"
#include <string>

namespace seabreeze {
    
    class Socket {
    public:
        Socket(std::string host, int port)
                throw (UnknownHostException, BusConnectException);
        
        void close() throw (BusException);
        
        /* Socket options */
        int getSOLinger() throw (SocketException);
        void setSOLinger(bool enable, int linger) throw (SocketException);
        int getSOTimeout() throw (SocketException);
        int setSOTimeout(int timeout) throw (SocketException);
        
        /* Data transfer */
        int read(unsigned char *buffer, unsigned long length)
            throw (BusTransferException);
        int write(const unsigned char *buffer, unsigned long length)
            throw (BusTransferException);
        
    private:
        void *descriptor;
    };
}

#endif /* SOCKET_H */
