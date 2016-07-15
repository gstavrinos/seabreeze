/***************************************************//**
 * @file    Connection.h
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * This class is used to contain a socket and perform some of the basic operations
 * such as reading a command and any arguments.
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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <vector>
#include <utility>

class Connection {
public:
    /* Return the actual socket we are communicating on.
    */
    boost::asio::ip::tcp::socket *Socket();
    /* Convenience method. Return true if the socket is open, false otherwise.
    */
    bool IsOpen();
    /* Convenience method. Close the socket.
    */
    void Close();

    /* Return the received command and the arguments accompanying it.
    */
    std::pair<unsigned short, std::string> GetRequest();

    /* Convenience method. Reset the socket.
    */
    void Reset();

    /* Set up the connection
    */
    Connection(boost::asio::io_service &service);
    ~Connection();
private:
    boost::asio::io_service &m_service;
    std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;

    // the buffer we use for the IO service communication
    std::vector<unsigned char> m_buffer;
    /* a request is 4 bytes long. the first 2 bytes are the command (MSB first) followed by 2 bytes
     * specifying the size of the accompanying arguments (MSB first).
    */
    static const size_t ms_requestSize = 4;
};
#endif