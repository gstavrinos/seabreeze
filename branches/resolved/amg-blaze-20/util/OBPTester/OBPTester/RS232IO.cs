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
using System.IO.Ports;
using OBP_Library;

namespace OceanUtil {

    //
    // Summary:
    //     Extends the .Net SerialPort class with an implementation of ISendReceive
    // for RS232 serial port communication with an OBP spectrometer.
    //
    public class RS232IO : SerialPort, ISendReceive
    {

        //----- Static Methods -----

        static public string[] GetPortNames()
        {
            return SerialPort.GetPortNames();
        }


        //----- Constructors -----

        public RS232IO() :
            base() { }

        public RS232IO(string portName) :
            base(portName) { }


        //----- ISendReceive Implementation -----

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
        //     The number of bytes to be written.
        //
        // Returns:
        //     The send status, IO_SUCCESS if all bytes were sent
        //
        public SendReceiveStatus sendBytes(byte[] data, int offset, int numBytes)
        {
            SendReceiveStatus status = SendReceiveStatus.IO_SUCCESS;
            int numSent = 0;
            try
            {
                // Exceptions:
                //   System.ArgumentNullException:
                //     The buffer passed is null.
                //
                //   System.InvalidOperationException:
                //     The port is not open.
                //
                //   System.ArgumentOutOfRangeException:
                //     The offset or numBytes parameters are outside a valid region of the buffer being
                //     passed. Either offset or numBytes is less than zero.
                //
                //   System.ArgumentException:
                //     offset plus numBytes is greater than the length of the buffer.
                //
                //   System.ServiceProcess.TimeoutException:
                //     The operation did not complete before the time-out period ended.
                Write(data, offset, numBytes);
                numSent = numBytes;
            }
            catch (System.ArgumentNullException)
            {
                status = SendReceiveStatus.IO_NULL_BUFFER;
            }
            catch (System.InvalidOperationException)
            {
                status = SendReceiveStatus.IO_PORT_ERROR;
            }
            catch (System.ArgumentOutOfRangeException)
            {
                status = SendReceiveStatus.IO_OUT_OF_RANGE;
            }
            catch (System.ArgumentException)
            {
                status = SendReceiveStatus.IO_OUT_OF_RANGE;
            }
            catch (System.TimeoutException)
            {
                status = SendReceiveStatus.IO_TIMEOUT;
            }
            catch (Exception)
            {
                status = SendReceiveStatus.IO_OTHER_EXCEPTION;             
            }

            return status;
        }

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
        // Returns:
        //     The receive status, IO_SUCCESS if all bytes were received
        //
        public SendReceiveStatus receiveBytes(byte[] data, int offset, int numBytes)
        {
            SendReceiveStatus status = SendReceiveStatus.IO_SUCCESS;
            int numRead = 0;
            int numToRead = numBytes;
            try
            {
                //
                // Exceptions:
                //   System.ArgumentNullException:
                //     The buffer passed is null.
                //
                //   System.InvalidOperationException:
                //     The specified port is not open.
                //
                //   System.ArgumentOutOfRangeException:
                //     The offset or count parameters are outside a valid region of the buffer being
                //     passed. Either offset or count is less than zero.
                //
                //   System.ArgumentException:
                //     offset plus count is greater than the length of the buffer.
                //
                //   System.TimeoutException:
                //     No bytes were available to read.
                //
                do
                {
                    numRead += Read(data, offset + numRead, numToRead);
                    numToRead = numBytes - numRead;
                } while (numRead < numBytes);
            }
            catch (System.ArgumentNullException)
            {
                status = SendReceiveStatus.IO_NULL_BUFFER;
            }
            catch (System.InvalidOperationException)
            {
                status = SendReceiveStatus.IO_PORT_ERROR;
            }
            catch (System.ArgumentOutOfRangeException)
            {
                status = SendReceiveStatus.IO_OUT_OF_RANGE;
            }
            catch (System.ArgumentException)
            {
                status = SendReceiveStatus.IO_OUT_OF_RANGE;
            }
            catch (System.TimeoutException)
            {
                status = SendReceiveStatus.IO_TIMEOUT;
            }
            catch (Exception)
            {
                status = SendReceiveStatus.IO_OTHER_EXCEPTION;
            }

            return status;
        }

        //
        // Summary:
        //     Sets the send buffer timeout in milliseconds.
        //
        // Parameters:
        //   timeoutMS:
        //     The send buffer timeout in milliseconds.
        //
        public void setSendTimeout(int timeoutMS)
        {
            WriteTimeout = timeoutMS;
        }

        //
        // Summary:
        //     Sets the receive buffer timeout in milliseconds.
        //
        // Parameters:
        //   timeoutMS:
        //     The receive buffer timeout in milliseconds.
        //
        public void setReceiveTimeout(int timeoutMS)
        {
            ReadTimeout = timeoutMS;
        }

    }
}
