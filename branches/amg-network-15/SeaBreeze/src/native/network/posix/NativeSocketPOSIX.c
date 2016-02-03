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
#include <netinet/in.h>
#include <netinet/tcp.h>

typedef struct  {
    int sock;
    unsigned char bound;
    unsigned char closed;
} __socket_handle_t;

void *SocketConnect(char *hostname, unsigned int port) {
    struct hostent *host_info;
}

void SocketDestroy(void *handle) {
    
}

int SocketClose(void *handle) {
    
}

int SocketGetSOLinger(void *handle) {
    
}

int SocketSetSOLinger(void *handle, unsigned char enable, unsigned int linger) {
    
}

int SocketGetSOTimeout(void *handle) {
    
}

int SocketSetSOTimeout(void *handle, unsigned int timeout) {
    
}

int SocketRead(void *handle, unsigned char *buffer, unsigned int length) {
    
}

int SocketWrite(void *handle, unsigned char *buffer, unsigned int length) {
    
}
