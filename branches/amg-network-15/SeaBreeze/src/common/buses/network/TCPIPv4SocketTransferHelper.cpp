/***************************************************//**
 * @file    TCPIPv4SocketTransferHelper.cpp
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

#include "common/buses/network/TCPIPv4SocketTransferHelper.h"

using namespace seabreeze;
using namespace std;

TCPIPv4SocketTransferHelper::TCPIPv4SocketTransferHelper(Socket *sock) {
    this->socket = sock;
}

TCPIPv4SocketTransferHelper::~TCPIPv4SocketTransferHelper() {
    /* Note that disposal of the socket is the responsibility of the Bus
     * (i.e. TCPIPv4SocketBus).
     */
}

int TCPIPv4SocketTransferHelper::receive(vector<byte> &buffer,
        unsigned int length) throw (BusTransferException) {
    
    unsigned char *rawBuffer = (unsigned char *)&buffer[0];
    
    /* TODO: need to check for exceptions and possibly poll */
    return this->socket->read(rawBuffer, buffer.size());
}

int TCPIPv4SocketTransferHelper::send(const vector<byte> &buffer,
        unsigned int length) const throw (BusTransferException) {
    
    unsigned char *rawBuffer = (unsigned char *)&buffer[0];
    
    /* TODO: need to check for exceptions and possibly poll */
    return this->socket->write(rawBuffer, buffer.size());
}
