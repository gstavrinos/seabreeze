/***************************************************//**
 * @file    WriteEEPROMSlotExchange.cpp
 * @date    February 2009
 * @author  Ocean Optics, Inc.
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

#include "common/globals.h"
#include "vendors/OceanOptics/protocols/ooi/exchanges/WriteEEPROMSlotExchange.h"
#include "vendors/OceanOptics/protocols/ooi/constants/OpCodes.h"
#include "vendors/OceanOptics/protocols/ooi/hints/ControlHint.h"

#define MAX_EEPROM_SLOT_DATA_LENGTH 15

using namespace seabreeze;
using namespace seabreeze::ooiProtocol;
using std::vector;

WriteEEPROMSlotExchange::WriteEEPROMSlotExchange(int slot, const vector<byte>& data) {
    vector<ProtocolHint *> *requestHints = new vector<ProtocolHint *>;

    requestHints->push_back(new ControlHint());

    vector<byte> *requestBuffer = new vector<byte>(2 + MAX_EEPROM_SLOT_DATA_LENGTH, 0x00);
    requestBuffer->at(0) = OpCodes::OP_SETINFO;
    requestBuffer->at(1) = (byte) slot;

    for (unsigned i = 0; i < data.size(); i++) {
        requestBuffer->at(i + 2) = data[i];
    }

    Transfer *request = new Transfer(requestHints, requestBuffer, Transfer::TO_DEVICE, (unsigned int) requestBuffer->size());

    this->addTransfer(request);
}

WriteEEPROMSlotExchange::~WriteEEPROMSlotExchange() {

}
