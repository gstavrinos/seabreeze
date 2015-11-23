/***************************************************//**
 * @file    RequestHandler.h
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * RequestHandler subclasses BaseConnectionHandler to provide the specific behaviour for querying
 * the spectrometer. 
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

#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

// make RequestHandler available via a DLL in Windows
#ifdef _WINDOWS
    #ifdef BUILD_DLL
        #define DLL_DECL __declspec(dllexport)
    #else
        #define DLL_DECL __declspec(dllimport)
    #endif

    #pragma warning (disable: 4251)

    #pragma warning (disable: 4275)
#else
    #define DLL_DECL
#endif

#include "BaseConnectionHandler.h"
#include "IResponseHandler.h"
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

class SeaBreezeAPI;
class RequestHandlerConfiguration;
class Spectrometer;

class DLL_DECL RequestHandler : public BaseConnectionHandler, public IResponseHandler {

public:
    /* Implementation of IResponseHandler::NotifyResponse
    */
    void NotifyResponse(boost::shared_ptr<Connection> connection, const unsigned int resultCode, const std::string &response);

    /* Implementation of IResponseHandler::NotifyError
    */
    void NotifyError(boost::shared_ptr<Connection> connection);

    /* Constructor. Set up the IO service and configuration.
    */
    RequestHandler(boost::asio::io_service &ioService, RequestHandlerConfiguration &configuration);
    virtual ~RequestHandler();

protected:
    static const char ms_spectrometerIndexDelimiter;

    // Convenience typedef for the received command
    typedef unsigned short CommandType;
    // Convenient typedef for the received parameter size
    typedef unsigned short ParameterSizeType;

    // Convenience method to call the command handler base on the command value received
    virtual bool HandleRequest(boost::shared_ptr<Connection> connection, const CommandType command, const std::string &request);

    // Convenience method for separating the spectrometer number from the remaining arguments
    std::pair<int, std::string> SeparateSpectrometerNumber(const std::string &params);

    bool m_spectrometerError;

    // Hold the result status of a handler in this
    int m_resultCode;

    // the instance of SeaBreeze
    SeaBreezeAPI *m_seabreezeApi;
    int m_spectrometerCount;
    boost::asio::io_service &m_ioService;

    // buffers used for the IO service
    std::vector<unsigned char> m_buffer;
    std::vector<char> m_responseBuffer;
    std::shared_ptr<char> m_resultBuffer;

    // store the command and the size of the parameters
    CommandType m_command;
    ParameterSizeType m_parameterSize;

    // convenience typesdefs and the spectrometers mapped by their (SeaBreeze) index
    typedef boost::shared_ptr<Spectrometer> SpectrometerPtr;
    typedef std::map<int, SpectrometerPtr> SpectrometerMap;
    SpectrometerMap m_spectrometer;

private:
    /* Handler for the socket connection event.
    */
    void OnAcceptConnection(boost::shared_ptr<Connection> connection, const boost::system::error_code& error);

    /* The configuration for the RequestHandler.
    */
    RequestHandlerConfiguration &m_configuration;
};
#endif
