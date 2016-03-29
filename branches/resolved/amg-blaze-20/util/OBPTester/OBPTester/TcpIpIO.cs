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
using System.Net;
using System.Net.Sockets;
using OBP_Library;

namespace OceanUtil
{
    //
    // Summary:
    //     Extends the .Net Socket class with an implementation of ISendReceive
    // for TCPIP socket communication with an OBP spectrometer.
    //
    public class TcpIpIO : Socket, ISendReceive
    {

        //----- Static Methods -----

        // TODO: Implement multicast search???

        private IPAddress mIPAddress;
        private int mPortNum;

        private long mTotalReceiveTime = 5000 * 10000;  // 5 seconds


        //----- Constructors -----

        public TcpIpIO() :
            base(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp) { }

        public TcpIpIO(IPAddress ipAddress, int portNum) :
            base(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp)
        {
            mIPAddress = ipAddress;
            mPortNum = portNum;
        }

        public TcpIpIO(SocketInformation socketInformation) :
            base(socketInformation) { }

        public TcpIpIO(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) :
            base(addressFamily, socketType, protocolType) { }


        //----- Public Properties -----

        public IPAddress ipAddress
        {
            get { return mIPAddress;  }
            set { mIPAddress = value;  }
        }

        public int PortNum
        {
            get { return mPortNum; }
            set { mPortNum = value; }
        }

        //----- Helper Methods -----

        //
        // Summary:
        //   Connects to the IP address specified by mIPAddress and the port # specified by mPortNum.
        //
        public bool Connect()
        {
            try
            {
                // Exceptions:
                //   System.ArgumentNullException:
                //     address is null.
                //
                //   System.ArgumentOutOfRangeException:
                //     The port number is not valid.
                //
                //   System.Net.Sockets.SocketException:
                //     An error occurred when attempting to access the socket. See the Remarks section
                //     for more information.
                //
                //   System.ObjectDisposedException:
                //     The System.Net.Sockets.Socket has been closed.
                //
                //   System.NotSupportedException:
                //     This method is valid for sockets in the System.Net.Sockets.AddressFamily.InterNetwork
                //     or System.Net.Sockets.AddressFamily.InterNetworkV6 families.
                //
                //   System.ArgumentException:
                //     The length of address is zero.
                //
                //   System.InvalidOperationException:
                //     The System.Net.Sockets.Socket is System.Net.Sockets.Socket.Listen(System.Int32)ing.
                //
                Connect(mIPAddress, mPortNum);
            }
            catch (Exception)
            {
                // nothing to do
            }

            return Connected;
        }


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
                //     buffer is null.
                //
                //   System.ArgumentOutOfRangeException:
                //     offset is less than 0.-or- offset is greater than the length of buffer.-or-
                //     size is less than 0.-or- size is greater than the length of buffer minus
                //     the value of the offset parameter.
                //
                //   System.Net.Sockets.SocketException:
                //     socketFlags is not a valid combination of values.-or- An operating system
                //     error occurs while accessing the System.Net.Sockets.Socket. See the Remarks
                //     section for more information.
                //
                //   System.ObjectDisposedException:
                //     The System.Net.Sockets.Socket has been closed.
                numSent = Send(data, offset, numBytes, SocketFlags.None);
                if (numSent != numBytes)
                {
                    status = SendReceiveStatus.IO_INSUFFICIENT_DATA;
                }
            }
            catch (System.ArgumentNullException)
            {
                status = SendReceiveStatus.IO_NULL_BUFFER;
            }
            catch (System.ObjectDisposedException)
            {
                status = SendReceiveStatus.IO_PORT_ERROR;
            }
            catch (System.ArgumentOutOfRangeException)
            {
                status = SendReceiveStatus.IO_OUT_OF_RANGE;
            }
            catch (System.Net.Sockets.SocketException)
            {
                status = SendReceiveStatus.IO_PORT_ERROR;
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
                long startTime = DateTime.Now.ToFileTime();
                long curTime = startTime;
                do
                {
                    numRead += Receive(data, offset + numRead, numToRead, SocketFlags.None);
                    numToRead = numBytes - numRead;
                    if (numToRead > 0)
                    {
                        curTime = DateTime.Now.ToFileTime();
                        if ((curTime - startTime) > mTotalReceiveTime)
                        {
                            status = SendReceiveStatus.IO_TIMEOUT;
                        }
                    }
                } while (numRead < numBytes && status != SendReceiveStatus.IO_TIMEOUT);
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
            SendTimeout = timeoutMS;
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
            ReceiveTimeout = timeoutMS;
        }

    }
}
