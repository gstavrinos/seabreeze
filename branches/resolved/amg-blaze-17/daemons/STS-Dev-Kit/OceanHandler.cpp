/***************************************************//**
 * @file    OceanHandler.cpp
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * The request handler for the acquisition sequence functionality. This class responds to requests
 * for e.g. setting the maximum number of acquisitions, file prefix and returns the result to the
 * response handler.
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

#include "OceanHandler.h"
#include "Version.h"
#include "OceanHandlerConfiguration.h"
#include <boost/lexical_cast.hpp>

/* Overrides the RequestHandler method to intercept any requests/commands for one of the sequences of acquisition. If this is
* not an acquisition sequence command it is forwarded to the RequestHandler to see if it can handle the command.
* The first step is to handle a get software version request directly because it doesn't fit neatly into the set of
* commands for either the sequences or spectrometers.
*/
bool OceanHandler::HandleRequest(boost::shared_ptr<Connection> connection, const CommandType command, const std::string &request) {

    bool result = false;
    // m_command holds the command number and request holds the spectrometer number followed by the arguments
    std::pair<int, std::string> args = SeparateSpectrometerNumber(request);

    //intercept the request for version number first
    if (command == GET_VERSION)
    {
        std::string version = boost::lexical_cast<std::string>(mg_majorVersion).append(".").append(boost::lexical_cast<std::string>(mg_minorVersion));
        NotifyResponse(connection, SUCCESS, version);
    }
    else if (m_sequence.find(args.first) == m_sequence.end()) {
        NotifyError(connection);
    }
    else if (m_sequence.at(args.first)->Accept(connection, command, args.second)) {
        result = true;
    }
    else {
        // it isn't a sequence command, try the spectrometer instead
        result = RequestHandler::HandleRequest(connection, command, request);
    }
    return result;
}

/* Constructor. Initialise from configuration then set up once sequence per spectrometer.
*/
OceanHandler::OceanHandler(boost::asio::io_service &ioService, OceanHandlerConfiguration &configuration) :

    RequestHandler(ioService, configuration),
    m_configuration(configuration) {

    m_timeStart = boost::posix_time::microsec_clock::local_time();

    const size_t numSpectrometers = m_spectrometer.size();
    for (size_t s = 0; s < numSpectrometers; ++s) {
        SequencePtr p(new Sequence(this, ioService, *m_spectrometer.at(s), configuration));
        m_sequence.insert(SequenceMap::value_type(s, p));
        m_sequence.at(s)->SetTimeZero(m_timeStart);
    }
}
 
OceanHandler::~OceanHandler() {
}