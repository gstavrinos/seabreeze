/***************************************************//**
 * @file    NativeSocketPOSIX.c
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

/* Includes */
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>

#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "native/network/posix/NativeSocketPOSIX.h"

Socket *Socket::create() {
    return new NativeSocketPOSIX();
}

NativeSocketPOSIX::NativeSocketPOSIX() {
    this->sock = -1;
    this->bound = false;
    this->closed = true;
}
        
NativeSocketPOSIX::~NativeSocketPOSIX() {
    close();
}
        
void NativeSocketPOSIX::connect(Inet4Address &addr, int port)
        throw (BusConnectException) {
    struct in_addr in;
    struct sockaddr_in sockaddr;
    socklen_t addrlen;
    int server;
    
    in = addr->getAddress();
    
    memset((void *)&sockaddr, (int)0, sizeof(struct sockaddr));
    memcpy((char *)&sockaddr.sin_addr.s_addr, &in, sizeof(struct in_addr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    server = socket(PF_INET, SOCK_STREAM, 0);
    
    addrlen = sizeof(sockaddr);
    if(connect(server, (struct sockaddr *)&sockaddr, addrlen) < 0) {
        this->sock = -1;
        this->closed = true;
        throw BusConnectException("Socket connect failed:" + strerror());
    }
    
    this->bound = true;
    this->closed = false;
    this->sock = server;
    this->address = addr;
}

void NativeSocketPOSIX::connect(const string hostname, int port)
        throw (BusConnectException) {
    
    struct hostent *host_info;
    struct in_addr in;
    
    host_info = gethostbyname(hostname.c_str());
    if(0 == host_info) {
        throw BusConnectException("Failed to resolve hostname " + hostname;
    }
    
    memcpy(&in, host_info->h_addr_list[0], host_info->h_length);
    
    Inet4Address inet4addr(&ip_addr);
    
    connect(inet4addr, port);
}

void NativeSocketPOSIX::close() throw (BusException) {
    int result;
    
    if(this->sock >= 0 && false == this->closed) {
        shutdown(this->sock, SHUT_RDWR);
        result = close(this->sock);
        this->sock = -1;
        this->bound = false;
        this->closed = true;
        
        if(result < 0) {
            throw BusException("Got error when trying to close socket: "
                    + strerror());
        }
    }
}

bool NativeSocketPOSIX::isClosed() {
    return this->closed;
}

bool NativeSocketPOSIX::isBound() {
    return this->bound;
}

int NativeSocketPOSIX::getSOLinger() throw (SocketException) {
    struct linger so_linger;
    int result;
    
    if(this->sock < 0) {
        throw SocketException("Attempted to get socket options on invalid socket.");
    }
    
    result = getsockopt(this->sock, SOL_SOCKET, SO_LINGER, (char *)&so_linger,
            sizeof(so_linger));
    
    if(result < 0) {
            throw SocketException("Failed to get socket options: " + strerror()));
    }
    
    if(0 == so_linger.l_onoff) {
        return 0;
    }
    
    return so_linger.l_linger;
}

void NativeSocketPOSIX::setSOLinger(bool enable, int linger)
        throw (SocketException) {
    struct linger so_linger;
    int result;
    
    if(this->sock < 0) {
        throw SocketException("Attempted to set socket options on invalid socket.");
    }
    
    so_linger.l_onoff = (true == enable ? 1 : 0);
    so_linger.l_linger = linger;
    
    result = setsockopt(this->sock, SOL_SOCKET, SO_LINGER, (char *)&so_linger,
            sizeof(so_linger));
    
    if(result < 0) {
        throw SocketException("Failed to set socket options: " + strerror()));
    }
}

unsigned long NativeSocketPOSIX::getReadTimeoutMillis() throw (SocketException) {
    struct timeval timeout;
    int result;
    
    if(this->sock < 0) {
        throw SocketException("Attempted to get socket options on invalid socket.");
    }
    
    result = getsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
            sizeof(timeout));
    
    if(result < 0) {
        throw SocketException("Failed to get socket options: " + strerror()));
    }
    
    return (timeout.tv_sec * 1000) + (timeout.tv_usec / 1000);
}

int NativeSocketPOSIX::setReadTimeoutMillis(unsigned long timeoutMillis)
        throw (SocketException) {
    struct timeval timeout;
    int result;
    
    if(this->sock < 0) {
        throw SocketException("Attempted to get socket options on invalid socket.");
    }
    
    timeout.tv_sec = timeoutMillis / 1000;
    timeout.tv_usec = (timeoutMillis % 1000) * 1000; /* Isolate millis then convert to micros */
    
    result = setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
            sizeof(timeout));
    
    if(result < 0) {
        throw SocketException("Failed to set socket options: " + strerror()));
    }
}

int NativeSocketPOSIX::read(unsigned char *buf, unsigned int count)
            throw (BusTransferException) {
    int result = read(this->sock, buf, count);
    
    if(result < 0) {
        if(EAGAIN == errno) {
            throw SocketTimeoutException(
                    "No data available on non-blocking socket.");
        } else {
            throw SocketException("Socket error on read: " + strerror());
        }
    }
    
    return result;
}

int NativeSocketPOSIX::write(const unsigned char *buf, unsigned int count) {
    int result = write(this->sock, buf, count);
    
    if(result < 0) {
        throw BusTransferException("Socket error on write: " + strerror());
    }
    
    return result;
}
