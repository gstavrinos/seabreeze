/***************************************************//**
 * @file    RequestHandler.cpp
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * The request handler for the spectrometer(s) functionality. This class responds to requests
 * for e.g. setting the integration time, initiating an acquisition and returns the result to the
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

#include "RequestHandler.h"
#include "RequestHandlerConfiguration.h"
#include "Spectrometer.h"
#include <api/seabreezeapi/SeaBreezeAPI.h>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>

/* Return the result of a command/request on the same connection the request was received on. The response consists of
* the result code followed by the response string. The implication of this is that e.g. spectra must be encoded as a string, in this
* case as space separated numbers, to be returned.
* The connection is returned for recycling when we are finished with it.
*/
void RequestHandler::NotifyResponse(boost::shared_ptr<Connection> connection, const unsigned int resultCode, const std::string &response) {
    if (connection->IsOpen()) {
        // copy the result code into the first two characters of the response buffer, MSB first...
        const int codeCount = 2;
        m_responseBuffer.resize(response.size() + codeCount);
        unsigned char code[codeCount];
        code[0] = (resultCode >> 8) & 0xFF;
        code[1] = resultCode & 0xFF;
        std::copy(code, code + codeCount, m_responseBuffer.begin());

        // ...followed by the rest of the response
        std::copy(response.begin(), response.end(), m_responseBuffer.begin() + codeCount);

        boost::asio::write(*connection->Socket(), boost::asio::buffer(m_responseBuffer, m_responseBuffer.size()));
        connection->Close();
        PushConnection(connection);
    }
}

/* The only general, sensible thing we can do is to close the connection on error.
*/
void RequestHandler::NotifyError(boost::shared_ptr<Connection> connection) {
    // If we get here the only sensible thing to do is close the connection and make it available for re-use.
    if (connection->IsOpen()) {
        connection->Close();
        PushConnection(connection);
    }
}

/* Handle an incoming command/request. Work out which channel (spectrometer) it is intended for and then get that spectrometer
* to accept the command. The spectrometer number is the (integer) first part of the request followed by a semicolon delimiter
* then any further arguments required by the command.
*/
bool RequestHandler::HandleRequest(boost::shared_ptr<Connection> connection, const CommandType command, const std::string &request) {

    bool result = false;
    // m_command holds the command number and m_buffer holds the spectrometer number followed by the arguments
    std::pair<int, std::string> args = SeparateSpectrometerNumber(request);
    if (m_spectrometer.find(args.first) != m_spectrometer.end()) {
        result = m_spectrometer.at(args.first)->Accept(connection, command, args.second);
    }
    else {
        NotifyError(connection);
    }
    return result;
}

/* Parse the request to separate the spectrometer the command is intended for from the arguments to the command.
*/
std::pair<int, std::string> RequestHandler::SeparateSpectrometerNumber(const std::string &params) {

    int spectrometerNumber = 0;
    std::string args(params.begin(), params.end());
    std::string remainder = args;

    std::string::iterator pos = std::find(args.begin(), args.end(), ms_spectrometerIndexDelimiter);

    if (pos != args.end()) {
        remainder = std::string(pos + 1, args.end());

        if (pos != args.begin()) {
            std::string number = std::string(args.begin(), pos);
            try {
                spectrometerNumber = boost::lexical_cast<int>(number);
            }
            catch (boost::bad_lexical_cast) {
                spectrometerNumber = -1;
            }
        }
    }

    return std::pair<int, std::string>(spectrometerNumber, remainder);
}

/* Handler for accepting a connection. This implements the BaseConnectionHandler::OnAcceptConnection method.
* On a successful socket connection we read the request and handle it. When we have handled the request
* we queue another connection accept ready for the next request.
*/
void RequestHandler::OnAcceptConnection(boost::shared_ptr<Connection> connection, const boost::system::error_code& error) {

    // on success queue a read, otherwise close the socket (if possible) and requeue the accept
    if (error.value() == 0) {
        std::pair<unsigned short, std::string> request = connection->GetRequest();
        HandleRequest(connection, request.first, request.second);
    }
    else {
        NotifyError(connection);
    }

    m_ioService.post(boost::bind(&RequestHandler::QueueAcceptConnection, boost::ref(*this)));
}

/* Constructor. Initialise from the configuration and set up the spectrometers.
*/
RequestHandler::RequestHandler(boost::asio::io_service &ioService, RequestHandlerConfiguration &configuration) :

    BaseConnectionHandler(),
    m_spectrometerError(false),
    m_seabreezeApi(nullptr),
    m_spectrometerCount(0),
    m_ioService(ioService),
    m_configuration(configuration) {

    m_seabreezeApi = SeaBreezeAPI::getInstance();
    int error = 0;
    if (m_seabreezeApi == nullptr) {
        m_spectrometerError = true;
    }
    else {
        m_spectrometerCount = m_seabreezeApi->probeDevices();
        std::vector<long> deviceHandles(m_spectrometerCount);
        m_seabreezeApi->getDeviceIDs(deviceHandles.data(), m_spectrometerCount);

        int count = 0;
        for (int index = 0; index < m_spectrometerCount; ++index) {
            int result = m_seabreezeApi->openDevice(deviceHandles[index], &error);
            if (result == 0) {
                SpectrometerPtr p(new Spectrometer(this, deviceHandles[index], m_seabreezeApi, m_configuration));
                m_spectrometer.insert(SpectrometerMap::value_type(index, p));
                ++count;
            }
            else {
                break;
            }
        }
        m_spectrometerError = m_spectrometerError && (count == 0);
    }
}

/* Clean up the spectrometers.
*/
RequestHandler::~RequestHandler() {
    if (m_seabreezeApi != nullptr) {
        int error = 0;
        const size_t numSpectrometers = m_spectrometer.size();
        for (size_t i = 0; i < numSpectrometers; ++i) {
            m_seabreezeApi->closeDevice(m_spectrometer[i]->Handle(), &error);
        }
    }
}

// The delimiter for the spectrometer number in the command/request string
const char RequestHandler::ms_spectrometerIndexDelimiter(';');
