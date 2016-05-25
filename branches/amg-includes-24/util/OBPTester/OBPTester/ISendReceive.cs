//
// The MIT License (MIT)
// Copyright (c) 2015 Ocean Optics, Inc.  All rights reserved.

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OceanUtil
{
    public enum SendReceiveStatus
    {
        IO_SUCCESS = 0,  // All specified bytes were successfully sent or received
        IO_PENDING = 1,  // The buffer has not been sent or received
        IO_PORT_ERROR = 2,  // Port not open etc.
        IO_NULL_BUFFER = 3,  // null IO buffer specified
        IO_OUT_OF_RANGE = 4,  // offset, numBytes or offset + numBytes exceeds the buffer size
        IO_TIMEOUT = 5,  // A timeout occurred before all bytes were sent or received
        IO_INSUFFICIENT_DATA = 6,  // Insufficient data was send or received (not the specified amount)
        IO_PIPE_ERROR = 7, // an error specific to winUSB/winUSBNet
        IO_OTHER_EXCEPTION = 8,  // Another exception was caught during the IO
    }

    //
    // Summary:
    //     A simple interface to send and receive binary buffer data.
    //
    public interface ISendReceive
    {
        //
        // Summary:
        //     Writes numBytes from the data buffer starting at the specified offset.
        //
        // Parameters:
        //   data:
        //     The output data buffer.
        //
        //   offset:
        //     The zero-based offset in the data buffer to start copying data.
        //
        //   numBytes:
        //     The number of bytes written.
        //
        SendReceiveStatus sendBytes(byte[] data, int offset, int numBytes);

        //
        // Summary:
        //     Reads numBytes into the data buffer at the specified offset.
        //
        // Parameters:
        //   data:
        //     The input data buffer.
        //
        //   offset:
        //     The zero based offset in the data buffer to write the data read.
        //
        //   numBytes:
        //     The number of bytes to read.
        //
        //   bytesRead:
        //     The number of bytes actually read.
        //
        // Returns:
        //     The number of bytes read.
        //
        SendReceiveStatus receiveBytes(byte[] data, int offset, int numBytes);

        //
        // Summary:
        //     Sets the send buffer timeout in milliseconds.
        //
        // Parameters:
        //   timeoutMS:
        //     The send buffer timeout in milliseconds.
        //
        void setSendTimeout(int timeoutMS);

        //
        // Summary:
        //     Sets the receive buffer timeout in milliseconds.
        //
        // Parameters:
        //   timeoutMS:
        //     The receive buffer timeout in milliseconds.
        //
        void setReceiveTimeout(int timeoutMS);
    }

}
