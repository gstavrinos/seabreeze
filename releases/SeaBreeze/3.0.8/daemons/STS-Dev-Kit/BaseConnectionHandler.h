/***************************************************//**
 * @file    BaseConnectionHandler.h
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * The base class for the classes that handle commands. This is the class that accepts connections and
 * recycles connections that are no longer being used.
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

#ifndef BASE_CONNECTION_HANDLER_H
#define BASE_CONNECTION_HANDLER_H

#include "Connection.h"
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <list>

class BaseConnectionHandler {
public:
    /* Handler for accepting socket connections. Concrete classes such as RequestHandler and OceanHandler must implement this.
    */
    virtual void OnAcceptConnection(boost::shared_ptr<Connection> connection, const boost::system::error_code& error) = 0;

    /* Set the IO service we will use for communication
    */
    void SetIoService(boost::asio::io_service* ioService);

    /* Start listening for connections on the specified port.
    */
    void Start(const short port);

    BaseConnectionHandler();
    virtual ~BaseConnectionHandler();
protected:
    /* Queue the OnAcceptConnection method to handle the next command/request. This method recycles previously
    *  used connections but will create a new connection if none are available.
    */
    void QueueAcceptConnection();

    /* Get the next available connection if there is one
    */
    boost::shared_ptr<Connection> PopConnection();
    /* Put a connection back in the list of available connections once we have finished with it.
    */
    void PushConnection(boost::shared_ptr<Connection> connection);

    // The IO service used to provide the socket and queuing of the accept handler
    boost::asio::io_service* m_ioService;
    short m_port;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
    // The list of connections. If the list is empty we create a new connection, otherwise take one for re-use.
    std::list< boost::shared_ptr<Connection> > m_connection;
};
#endif