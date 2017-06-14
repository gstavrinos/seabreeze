/***************************************************//**
 * @file    BaseConnectionHandler.cpp
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * (Abstract) base class for the request handlers, RequestHandler and OceanHandler.
 * This class manages the list of connections, recycling them on completion but
 * creating new connections if needed. The (pure virtual) OnAcceptConnection handler
 * is queued for a socket connection here allowing derived classes to simply implement/override it
 * and get the queuing functionality for free.
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

#include "BaseConnectionHandler.h"
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

/* If there is no free connection in the list, create one and then queue it for a socket accept.
*/
void BaseConnectionHandler::QueueAcceptConnection() {
    if (m_acceptor) {
        // If the list of free connections is empty then add a new one to it
        if (m_connection.size() == 0) {
            boost::shared_ptr<Connection> p(new Connection(*m_ioService));
            p->Reset();
            m_connection.push_back(p);
        }

        // Get the next free connection
        boost::shared_ptr<Connection> currentConnection = PopConnection();

        // ... and queue the accept
        m_acceptor->async_accept(*currentConnection->Socket(),
            boost::bind(&BaseConnectionHandler::OnAcceptConnection, boost::ref(*this), currentConnection,
            boost::asio::placeholders::error));
    }
}

/* Get the first available connection from the queue.
*/
boost::shared_ptr<Connection> BaseConnectionHandler::PopConnection() {
    // Remove/return the first available connection
    boost::shared_ptr<Connection> c = m_connection.front();
    m_connection.pop_front();
    return c;
}

/* Put a connection back in the queue when it is no longer in use.
*/
void BaseConnectionHandler::PushConnection(boost::shared_ptr<Connection> connection) {
    // Put a connection on the back of the queue when finished with it.
    m_connection.push_back(connection);
}

/* Set the IO service we are using for the socket/accept functionality.
*/
void BaseConnectionHandler::SetIoService(boost::asio::io_service* ioService) {
    m_ioService = ioService;
}

/* Start listening on the configured port and queue the first connection
*/
void BaseConnectionHandler::Start(const short port) {
    // Start accepting connections
    if (m_ioService != nullptr && port != 0) {
        m_acceptor.reset(new tcp::acceptor(*m_ioService, tcp::endpoint(tcp::v4(), port)));

        QueueAcceptConnection();

    }
    else {
        // TODO handle this
    }
}

/* Construct/Initialise.
*/
BaseConnectionHandler::BaseConnectionHandler() :
    m_ioService(nullptr), m_port(0) {
}

BaseConnectionHandler::~BaseConnectionHandler() {
}