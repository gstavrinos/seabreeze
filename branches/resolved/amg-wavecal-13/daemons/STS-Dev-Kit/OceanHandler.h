/***************************************************//**
 * @file    OceanHandler.h
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * OceanHandler provides the additional functionality needed for e.g. supplying the
 * current spectrum on demand, saving a sequence of acquisitions. It is derived from RequestHandler
 * which provides the basic spectrometer functionality. Request that are received and can be handled by
 * OceanHandler are passed to a Sequence for execution and the result returned to the response handler.
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

#ifndef OCEAN_HANDLER_H
#define OCEAN_HANDLER_H

#include "RequestHandler.h"
#include "Sequence.h"
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>

class OceanHandlerConfiguration;

class OceanHandler : public RequestHandler {
public:
    /* Unfortunately we need this here for backwards compatibility of the command value. We will intercept it
    * before it reaches Spectrometer or Sequence.
    */
    enum CommandValue {
        GET_VERSION = 0x0D
    };
    enum ResultCode {
        SUCCESS = 1
    };

    /* Constructor. Set up the IO service and configuration.
    */
    OceanHandler(boost::asio::io_service &ioService, OceanHandlerConfiguration &configuration);
    virtual ~OceanHandler();

private:
    // Override the request handler for provided by RequestHandler
    bool HandleRequest(boost::shared_ptr<Connection> connection, const CommandType command, const std::string &request);

    OceanHandlerConfiguration &m_configuration;
    // Convenience typedefs for the acquisition sequences
    typedef boost::shared_ptr<Sequence> SequencePtr;
    typedef std::map<int, SequencePtr> SequenceMap;
    //... and the actual acquisition sequences
    SequenceMap m_sequence;

    // Keep track of the time at construction. All times will be relative to this.
    boost::posix_time::ptime m_timeStart;

    // copy constructor and assignment not implemented
    OceanHandler(const OceanHandler& o);
    OceanHandler& operator=(const OceanHandler &o);
};
#endif