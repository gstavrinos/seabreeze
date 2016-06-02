/***************************************************//**
 * @file    IResponseHandler.h
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * The interface for classes handling the response or error resulting from a command.
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

#ifndef I_RESPONSE_HANDLER_H
#define I_RESPONSE_HANDLER_H

#include "Connection.h"
#include <string>
#include <boost/asio.hpp>

class IResponseHandler {
public:
    /* Notify the response handler of a result on the same connection the request came in on.
    */
    virtual void NotifyResponse(boost::shared_ptr<Connection> connection, const unsigned int resultCode, const std::string &response) = 0;

    /* Notify the response handler of an error on the same connection the request came in on.
    */
    virtual void NotifyError(boost::shared_ptr<Connection> connection) = 0;
};
#endif