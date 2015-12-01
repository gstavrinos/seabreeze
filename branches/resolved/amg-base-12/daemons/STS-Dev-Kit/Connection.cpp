/***************************************************//**
 * @file    Connection.cpp
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * Wraps the TCP socket with some additional functionality for reading requests/commands.
 *
 *
 * LICENSE:
 *
 * Dev Kit Copyright (C) 2015, Ocean Optics Inc
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

#include "Connection.h"

/* Return the actual socket. Unfortunately it is occasionally necessary to expose this to other classes.
*/
boost::asio::ip::tcp::socket *Connection::Socket() {
    return m_socket.get();
}

/* Return true if there is a socket and it is open, false otherwise.
*/
bool Connection::IsOpen() {
    return m_socket && m_socket->is_open();
}

/* If there is a socket and it is open then close it.
*/
void Connection::Close() {
    if (IsOpen()) {
        m_socket->close();
    }
}

/* Read a command/request over the socket and return it as as a command/arguments pair expected by the request handlers.
*/
std::pair<unsigned short, std::string> Connection::GetRequest() {
    // Read from the socket. The first 2 bytes are the command, the next 2 the size of any following
    // parameters. If there are parameters, read them too. Put the command and parameters into a pair
    // and return.
    m_buffer.resize(ms_requestSize);
    boost::asio::read(*m_socket, boost::asio::buffer(m_buffer, ms_requestSize), boost::asio::transfer_exactly(ms_requestSize));

    unsigned short command = (m_buffer[0] << 8) | m_buffer[1];
    unsigned short paramSize = (m_buffer[2] << 8) | m_buffer[3];

    if (paramSize > 0) {
        m_buffer.resize(paramSize);
        boost::asio::read(*m_socket, boost::asio::buffer(m_buffer, paramSize), boost::asio::transfer_exactly(paramSize));
    }
    else {
        m_buffer.clear();
    }
    std::string args(m_buffer.begin(), m_buffer.end());

    return std::pair<unsigned short, std::string>(command, args);
}

/* Reset the socket.
*/
void Connection::Reset() {
    m_socket.reset(new boost::asio::ip::tcp::socket(m_service));
}

/* Constructor. Set up the IO service we use for the socket functionality.
*/
Connection::Connection(boost::asio::io_service &service) :
    m_service(service) {
}

Connection::~Connection() {
}
