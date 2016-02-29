/***************************************************//**
 * @file    Daemon.h
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * The Daemon class pulls together OceanHandler, its configuration and the IO service
 * and enables the service to be started.
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

#ifndef DAEMON_H
#define DAEMON_H

#include <boost/asio.hpp>
#include <memory>
#include <list>
#include <string>

class OceanHandlerConfiguration;
class OceanHandler;

class Daemon {

public:
    /* Start the service that provides an OceanHandler.
    */
    void Run();

    /* Constructor that pulls together the handler, its configuration and the IO service.
    */
	Daemon(boost::asio::io_service& ioService, OceanHandlerConfiguration& configuration, OceanHandler& connectionHandler);
	~Daemon();

private:
    OceanHandlerConfiguration& m_configuration;

    // core i/o functionality is supplied by io_service
    boost::asio::io_service& m_ioService;

    OceanHandler& m_connectionHandler;

    // not implemented
    Daemon(const Daemon& d);
    Daemon& operator=(const Daemon& d);
};
#endif