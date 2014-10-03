/***************************************************//**
 * @file    OOIProtocol.h
 * @date    February 2009
 * @author  Ocean Optics, Inc.
 *
 * This provides a singleton object that identifies the
 * OOI legacy USB command set.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2014, Ocean Optics Inc
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

#ifndef OOIPROTOCOL_H
#define OOIPROTOCOL_H

#include "common/protocols/Protocol.h"

namespace seabreeze {
  namespace ooiProtocol {
    class OOIProtocol : public Protocol {
    public:
        OOIProtocol();
        virtual ~OOIProtocol();

        /* Must be overridden from Protocol */
        virtual ProtocolFamily getProtocolFamily();
    };
  }
}

#endif /* OOIPROTOCOL_H */
