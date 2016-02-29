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
using System.Runtime.InteropServices;

//
// Summary:
//
//  This file defines Ocean Binary Protocol (OBP) messages as defined in the STS and QEPro datasheets:
//
//  http://oceanoptics.com/wp-content/uploads/OEM-Data-Sheet-QEPRO.pdf
//  http://oceanoptics.com/wp-content/uploads/STS-Data-Sheet.pdf
//
//  These messages utilize the OBPBuffer class to set and extract bits and bytes from the
//  OBP request and response buffers for each message.
//
//  Notes:
//    1. Not all messages are implemented
//    2. Not all messages work with all OBP spectrometers
//    3. Not everything has been tested (TCPIP, I2C)
//
//  TODO:
//    1. Single and Continuous Strobe messages
//    2. SPI
//    3. USB I/O
//    4. Bluetooth I/O
//

namespace OceanUtil
{

    //
    // Summary:
    //    The base OBP message class.
    //
    public class OBPMessage
    {

        //----- Static Data -----


        //----- Static Methods -----

        // Converts the specified buffer bytes to a UTF8 string if the chars are greater or equal to minVal
        static public string BytesToString(byte[] data, int offset, int length, byte minVal)
        {
            int numChars = 0;
            int iEnd = offset + length;
            minVal -= 1;
            for (int i = offset; i < iEnd; i++)
            {
                if (data[i] > minVal)
                {
                    ++numChars;
                }
                else
                {
                    break;
                }
            }
            return numChars > 0 ? Encoding.UTF8.GetString(data, offset, numChars) : "";
        }

        // Converts the specified buffer bytes to a UTF8 string
        static public string BytesToString(byte[] data, int offset, int length)
        {
            return length > 0 ? Encoding.UTF8.GetString(data, offset, length) : "";
        }

        static public string BytesToString(byte[] data)
        {
            return data.Length > 0 ? Encoding.UTF8.GetString(data, 0, data.Length) : "";
        }

        static public string BytesToString(byte[] data, byte minVal)
        {
            return BytesToString(data, 0, data.Length, minVal);
        }

        // Converts the specified byte array to a string of space separated hex bytes
        static public string ToHexByteString(byte[] data)
        {
            if (data == null || data.Length == 0) {
                return "";
            }

            StringBuilder str = new StringBuilder(3 * data.Length);
            int iLen = data.Length;
            for (int i=0; i < iLen; i++) {
                if (i > 0) {
                    str.Append(" ");
                }
                str.Append(data[i].ToString("X2"));
            }

            return str.ToString();
        }

        // Converts the specified string of space separated hex bytes to a byte array
        //
        // Exceptions:
        //    System.FormatException
        //    System.OverflowException
        //
        static public byte[] FromHexByteString(string str)
        {
            // Consolidate consecutive whitespace to a single space
            str = System.Text.RegularExpressions.Regex.Replace(str,@"\s+"," ");

            // Remove whitespace from ends of str
            str = str.Trim();

            char[] delimiterChars = { ' ' };
            string[] strA = str.Split(delimiterChars);

            byte[] byteA = new byte[strA.Length];
            int iLen = byteA.Length;
            for (int i=0; i < iLen; i++) {
                byteA[i] = Convert.ToByte(strA[i], 16);
            }

            return byteA;
        }


        //----- Data Members -----


        protected ISendReceive     mIO;

        protected OBPBuffer        mRequest;
        protected OBPBuffer        mResponse;

        // Set to true if the message type always has a response (regardless of Ack request status)
        protected bool             mHasResponse;

        protected uint             mMessageId;


        protected bool             mSendSuccess;


        //----- Constructors -----

        public OBPMessage(uint messageId)
        {
            mRequest = new OBPBuffer();
            mResponse = new OBPBuffer();

            mMessageId = messageId;

            Init();
        }

        public OBPMessage(uint messageId, ISendReceive io)
        {
            mIO = io;
            mRequest = new OBPBuffer();
            mResponse = new OBPBuffer();

            mMessageId = messageId;

            Init();
        }

        public OBPMessage(uint messageId, OBPBuffer request, OBPBuffer response)
        {
            mRequest = request;
            mResponse = response;

            mMessageId = messageId;

            Init();
        }

        public OBPMessage(uint messageId, ISendReceive io, OBPBuffer request, OBPBuffer response)
        {
            mIO = io;
            mRequest = request;
            mResponse = response;

            mMessageId = messageId;

            Init();
        }


        //----- Message Initialization Helper -----

        public void Init()
        {
            mRequest.Init();
            mRequest.MessageType = mMessageId;

            mResponse.Clear();
        }


        //----- Public Properties -----

        public ISendReceive IO
        {
            get
            {
                return mIO;
            }
            set
            {
                mIO = value;
            }
        }

        public OBPBuffer Request
        {
            get
            {
                return mRequest;
            }
            set
            {
                mRequest = value;
            }
        }

        public OBPBuffer Response
        {
            get
            {
                return mResponse;
            }
            set
            {
                mResponse = value;
            }
        }

        public bool HasResponse
        {
            get { return mHasResponse;  }
        }


        public bool SendSuccess
        {
            get { return mSendSuccess; }
        }

        public bool IsSuccess
        {
            get { return mSendSuccess && !mResponse.IsError; }
        }

        public bool IsError
        {
            get { return !mSendSuccess || mResponse.IsError; }
        }

        public uint MessageId
        {
            get
            {
                return mMessageId;
            }
        }


        //----- Public Interface Methods -----

        // Sends the request message and (optionally) retrieves the response
        //
        // Returns:
        //    true if the request was successfully sent and the response (if any) successfully received
        //    aka... true if IO was successful
        //    Note: the OBP response may still contain an error status
        //
        virtual public bool Send()
        {
            mSendSuccess = false;

            if (mIO == null || mRequest == null || mResponse == null)
            {
                return mSendSuccess;
            }

            // Ensure only one request-response at a time per I/O object
            lock (mIO)
            {
                mRequest.MessageType = mMessageId;
                mRequest.ComputeChecksum();
                mRequest.SetFooter();

                mRequest.IOStatus = mIO.sendBytes(mRequest.IOBuffer, 0, mRequest.MessageSize);
                mSendSuccess = mRequest.IOStatus == SendReceiveStatus.IO_SUCCESS;

                // If there's a response to receive...
                if (mSendSuccess && (mHasResponse || mRequest.AckRequest))
                {
                    mResponse.IOStatus = mIO.receiveBytes(mResponse.IOBuffer, 0, 44);
                    if (mResponse.IOStatus == SendReceiveStatus.IO_SUCCESS)
                    {
                        int bytesRemaining = (int)mResponse.BytesRemaining;
                        if (bytesRemaining > 0)
                        {
                            mResponse.IOStatus = mIO.receiveBytes(mResponse.IOBuffer, 44, bytesRemaining);
                            if (mResponse.IOStatus == SendReceiveStatus.IO_SUCCESS && bytesRemaining < 20)
                            {
                                mResponse.IOStatus = SendReceiveStatus.IO_INSUFFICIENT_DATA;
                            }
                        }
                        else
                        {
                            mResponse.IOStatus = SendReceiveStatus.IO_INSUFFICIENT_DATA;
                        }
                    }
                    mSendSuccess = mResponse.IOStatus == SendReceiveStatus.IO_SUCCESS;
                }
            }

            return mSendSuccess;
        }

    }

    //
    //=========================================================================
    //
    // Reset Messages
    //
    //=========================================================================
    //
    //

    public class OBPResetSpectrometer : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000000;


        //----- Data Members -----


        //----- Constructors -----

        public OBPResetSpectrometer()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPResetSpectrometer(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPResetSpectrometer(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPResetSpectrometer(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----


        //----- Override Send() -----


    }


    public class OBPResetDefaults : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000001;


        //----- Data Members -----


        //----- Constructors -----

        public OBPResetDefaults()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPResetDefaults(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPResetDefaults(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPResetDefaults(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----


        //----- Override Send() -----


    }


    //
    //=========================================================================
    //
    // Revision Information Messages
    //
    //=========================================================================
    //

    public class OBPGetHWRevision : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000080;


        //----- Data Members -----

        protected byte mHwRevision = 0;


        //----- Constructors -----

        public OBPGetHWRevision()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetHWRevision(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetHWRevision(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetHWRevision(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public byte HwRevision
        {
            get { return mHwRevision; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            bool bSuccess = base.Send();

            mHwRevision = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPGetFWRevision : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000090;


        //----- Data Members -----

        protected ushort mFwRevision = 0;


        //----- Constructors -----

        public OBPGetFWRevision()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetFWRevision(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetFWRevision(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetFWRevision(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public ushort FwRevision
        {
            get { return mFwRevision; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            bool bSuccess = base.Send();

            mFwRevision = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUShort() : (ushort)0;

            return bSuccess;
        }

    }

    public class OBPGetFPGARevision : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000091;


        //----- Data Members -----

        protected ushort mFPGARevision = 0;


        //----- Constructors -----

        public OBPGetFPGARevision()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetFPGARevision(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetFPGARevision(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetFPGARevision(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public ushort FPGARevision
        {
            get { return mFPGARevision; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            bool bSuccess = base.Send();

            mFPGARevision = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUShort() : (ushort)0;

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // Device Identification Messages
    //
    //=========================================================================
    //

    public class OBPGetSerialNumber : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000100;


        //----- Data Members -----

        protected string mSerialNum = "";


        //----- Constructors -----

        public OBPGetSerialNumber()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetSerialNumber(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetSerialNumber(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetSerialNumber(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public string SerialNum
        {
            get { return mSerialNum;  }
        }


        //----- Override Send() -----

        
        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                byte[] data = mResponse.GetImmediateData();
                mSerialNum = BytesToString(data, 32);  // Convert chars >= 32 to a string
            }
            else
            {
                mSerialNum = "";
            }

            return bSuccess;
        }

    }

    public class OBPGetSerialNumberLen : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000101 ;


        //----- Data Members -----

        protected byte mSerialNumLen = 0;


        //----- Constructors -----

        public OBPGetSerialNumberLen()
            : base(MESSAGE_ID) { mHasResponse = true;  }

        public OBPGetSerialNumberLen(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetSerialNumberLen(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetSerialNumberLen(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte SerialNumberLen
        {
            get { return mSerialNumLen; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mSerialNumLen = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte) 0;

            return bSuccess;
        }

    }

    public class OBPGetDeviceAliasLen : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000201;


        //----- Data Members -----

        protected byte mDeviceAliasLen = 0;


        //----- Constructors -----

        public OBPGetDeviceAliasLen()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetDeviceAliasLen(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetDeviceAliasLen(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetDeviceAliasLen(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public byte DeviceAliasLen
        {
            get { return mDeviceAliasLen; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            bool bSuccess = base.Send();

            mDeviceAliasLen = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPGetDeviceAlias : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000200;


        //----- Data Members -----

        protected string mDeviceAlias = "";


        //----- Constructors -----

        public OBPGetDeviceAlias()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetDeviceAlias(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetDeviceAlias(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetDeviceAlias(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public string DeviceAlias
        {
            get { return mDeviceAlias; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                byte[] data = (mResponse.ImmediateDataLen > 0) ? mResponse.GetImmediateData() : mResponse.GetPayload();
                mDeviceAlias = BytesToString(data, 32);  // Convert chars >= 32 to a string
            }
            else
            {
                mDeviceAlias = "";
            }

            return bSuccess;
        }

    }


    public class OBPSetDeviceAlias : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000210;


        //----- Data Members -----

        protected string mDeviceAlias = "";


        //----- Constructors -----

        public OBPSetDeviceAlias()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetDeviceAlias(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetDeviceAlias(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetDeviceAlias(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----

        public string DeviceAlias
        {
            get { return mDeviceAlias; }
            set { mDeviceAlias = (value == null) ? "" : value; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            if (mDeviceAlias.Length < 17)
            {
                mRequest.SetImmediateData(Encoding.UTF8.GetBytes(mDeviceAlias));
            }
            else
            {
                mRequest.SetPayload(Encoding.UTF8.GetBytes(mDeviceAlias));
            }

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // User Storage Messages
    //
    //=========================================================================
    //

    public class OBPGetNumUserStrings : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000300;


        //----- Data Members -----

        protected byte mNumUserStrings = 0;


        //----- Constructors -----

        public OBPGetNumUserStrings()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetNumUserStrings(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetNumUserStrings(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetNumUserStrings(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public byte NumUserStrings
        {
            get { return mNumUserStrings; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            bool bSuccess = base.Send();

            mNumUserStrings = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPGetUserStringLen : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000301;


        //----- Data Members -----

        protected ushort mUserStringLen = 0;


        //----- Constructors -----

        public OBPGetUserStringLen()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetUserStringLen(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetUserStringLen(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetUserStringLen(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public ushort UserStringLen
        {
            get { return mUserStringLen; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            bool bSuccess = base.Send();

            mUserStringLen = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUShort() : (ushort)0;

            return bSuccess;
        }

    }

    public class OBPGetUserString : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000302;


        //----- Data Members -----

        protected byte mUserStringIndex = 0;
        protected string mUserString = "";


        //----- Constructors -----

        public OBPGetUserString()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetUserString(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetUserString(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetUserString(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public byte UserStringIndex
        {
            get { return mUserStringIndex; }
            set { mUserStringIndex = value; }
        }

        public string UserString
        {
            get { return mUserString; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mUserStringIndex);

            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                byte[] data = (mResponse.ImmediateDataLen > 0) ? mResponse.GetImmediateData() : mResponse.GetPayload();
                mUserString = BytesToString(data, 32);  // Convert chars >= 32 to a string
            }
            else
            {
                mUserString = "";
            }

            return bSuccess;
        }

    }


    public class OBPSetUserString : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000310;


        //----- Data Members -----

        protected byte mUserStringIndex = 0;
        protected string mUserString = "";


        //----- Constructors -----

        public OBPSetUserString()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetUserString(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetUserString(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetUserString(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----

        public byte UserStringIndex
        {
            get { return mUserStringIndex; }
            set { mUserStringIndex = value; }
        }

        public string UserString
        {
            get { return mUserString; }
            set { mUserString = (value == null) ? "" : value; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            if (mUserString.Length < 16)
            {
                mRequest.SetImmediateData(mUserStringIndex);
                mRequest.SetImmediateDataAtOffset(Encoding.UTF8.GetBytes(mUserString), 1);
            }
            else
            {
                mRequest.SetPayload(mUserStringIndex, 0);
                mRequest.SetPayloadAtOffset(Encoding.UTF8.GetBytes(mUserString), 1);
            }

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // RS232 Configuration Messages
    //
    //=========================================================================
    //

    public class OBPGetBaudRate : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000800;


        //----- Data Members -----

        protected uint mBaudRate = 0;


        //----- Constructors -----

        public OBPGetBaudRate()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetBaudRate(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetBaudRate(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetBaudRate(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint BaudRate
        {
            get { return mBaudRate; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                mBaudRate = mResponse.GetImmediateUInt();
            }
            else
            {
                mBaudRate = 0;
            }

            return bSuccess;
        }

    }


    public class OBPSetBaudRate : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000810;


        //----- Data Members -----

        protected uint mBaudRate = 0;


        //----- Constructors -----

        public OBPSetBaudRate()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetBaudRate(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetBaudRate(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetBaudRate(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public uint BaudRate
        {
            get { return mBaudRate; }
            set { mBaudRate = value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {

            mRequest.SetImmediateData(mBaudRate);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }


    public class OBPGetFlowControl : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000804 ;


        //----- Data Members -----

        protected byte mFlowControl = 0;


        //----- Constructors -----

        public OBPGetFlowControl()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetFlowControl(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetFlowControl(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetFlowControl(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte FlowControl
        {
            get { return mFlowControl; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mFlowControl = (bSuccess && !mResponse.IsError) ?  mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }


    public class OBPSetFlowControl : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00000814;


        //----- Data Members -----

        protected byte mFlowControl = 0;


        //----- Constructors -----

        public OBPSetFlowControl()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetFlowControl(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetFlowControl(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetFlowControl(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public byte FlowControl
        {
            get { return mFlowControl; }
            set { mFlowControl = value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {

            mRequest.SetImmediateData(mFlowControl);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }


    public class OBPSaveRS232Settings : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x000008F0;


        //----- Data Members -----



        //----- Constructors -----

        public OBPSaveRS232Settings()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSaveRS232Settings(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSaveRS232Settings(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSaveRS232Settings(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----



        //----- Override Send() -----


    }


    //
    //=========================================================================
    //
    // Status LED Messages
    //
    //=========================================================================
    //

    public class OBPSetStatusLED : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00001010;


        //----- Data Members -----

        protected byte mLedIndex = 0;
        protected byte mLedStatus = 0;


        //----- Constructors -----

        public OBPSetStatusLED()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetStatusLED(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetStatusLED(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetStatusLED(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----

        public byte LedIndex
        {
            get { return mLedIndex; }
            set { mLedIndex = value; }
        }

        public byte LedStatus
        {
            get { return mLedStatus; }
            set { mLedStatus = value; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mLedIndex);
            mRequest.SetImmediateData(mLedStatus, 1);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // Firmware Reprogramming Messages
    //
    //=========================================================================
    //

    //
    //=========================================================================
    //
    // Spectral Acquisition Control Messages
    //
    //=========================================================================
    //

    public class OBPAbortAcquisition : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00100000;


        //----- Data Members -----


        //----- Constructors -----

        public OBPAbortAcquisition()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPAbortAcquisition(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPAbortAcquisition(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPAbortAcquisition(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----


        //----- Override Send() -----


    }


    //
    //=========================================================================
    //
    // Spectrum Retrieval Messages (non-buffered)
    //
    //=========================================================================
    //

    public class OBPGetCorrectedSpectrum : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00101000;


        //----- Data Members -----

        protected ushort[] mCorrectedSpectrum = null;
        protected ushort[] mPixels = null;

        protected ushort mMaxValue = 0;
        protected int mMaxValuePixelNum = 0;


        //----- Constructors -----

        public OBPGetCorrectedSpectrum()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetCorrectedSpectrum(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetCorrectedSpectrum(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetCorrectedSpectrum(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public ushort[] CorrectedSpectrum
        {
            get { return mCorrectedSpectrum; }
        }

        public ushort[] Pixels
        {
            get { return mPixels; }
        }

        public ushort MaxValue
        {
            get { return mMaxValue; }
        }

        public int MaxValuePixelNum
        {
            get { return mMaxValuePixelNum; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mCorrectedSpectrum = (bSuccess && !mResponse.IsError) ? mResponse.GetPayloadUShortArray(0) : new ushort[0];

            mPixels = new ushort[mCorrectedSpectrum.Length];
            mMaxValue = 0;
            mMaxValuePixelNum = 0;
            for (ushort i = 0; i < mCorrectedSpectrum.Length; i++)
            {
                mPixels[i] = i;
                if (mCorrectedSpectrum[i] > mMaxValue)
                {
                    mMaxValue = mCorrectedSpectrum[i];
                    mMaxValuePixelNum = i;
                }
            }

            return bSuccess;
        }

    }

    public class OBPGetRawSpectrum : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00101100;


        //----- Data Members -----

        protected ushort[] mRawSpectrum = null;
        protected ushort[] mPixels = null;

        protected ushort mMaxValue = 0;
        protected int mMaxValuePixelNum = 0;


        //----- Constructors -----

        public OBPGetRawSpectrum()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetRawSpectrum(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetRawSpectrum(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetRawSpectrum(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public ushort[] RawSpectrum
        {
            get { return mRawSpectrum; }
        }

        public ushort[] Pixels
        {
            get { return mPixels; }
        }

        public ushort MaxValue
        {
            get { return mMaxValue; }
        }

        public int MaxValuePixelNum
        {
            get { return mMaxValuePixelNum; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mRawSpectrum = (bSuccess && !mResponse.IsError) ? mResponse.GetPayloadUShortArray(0) : new ushort[0];

            mPixels = new ushort[mRawSpectrum.Length];
            mMaxValue = 0;
            mMaxValuePixelNum = 0;
            for (ushort i = 0; i < mRawSpectrum.Length; i++)
            {
                mPixels[i] = i;
                if (mRawSpectrum[i] > mMaxValue)
                {
                    mMaxValue = mRawSpectrum[i];
                    mMaxValuePixelNum = i;
                }
            }

            return bSuccess;
        }

    }

    public class OBPGetPartialCorrectedSpectrum : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00102080;


        //----- Data Members -----

        protected ushort[] mPartialCorrectedSpectrum = null;
        protected ushort[] mPixels = null;

        protected ushort mMaxValue = 0;
        protected int mMaxValuePixelNum = 0;


        //----- Constructors -----

        public OBPGetPartialCorrectedSpectrum()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetPartialCorrectedSpectrum(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetPartialCorrectedSpectrum(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetPartialCorrectedSpectrum(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public ushort[] PartialCorrectedSpectrum
        {
            get { return mPartialCorrectedSpectrum; }
        }

        public ushort[] Pixels
        {
            get { return mPixels; }
        }

        public ushort MaxValue
        {
            get { return mMaxValue; }
        }

        public int MaxValuePixelNum
        {
            get { return mMaxValuePixelNum; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                mPartialCorrectedSpectrum = (mResponse.ImmediateDataLen > 0) ? mResponse.GetImmediateDataUShortArray() : mResponse.GetPayloadUShortArray(0);
            }
            else
            {
                mPartialCorrectedSpectrum = new ushort[0];
            }

            mPixels = new ushort[mPartialCorrectedSpectrum.Length];
            mMaxValue = 0;
            mMaxValuePixelNum = 0;
            for (ushort i = 0; i < mPartialCorrectedSpectrum.Length; i++)
            {
                mPixels[i] = i;
                if (mPartialCorrectedSpectrum[i] > mMaxValue)
                {
                    mMaxValue = mPartialCorrectedSpectrum[i];
                    mMaxValuePixelNum = i;
                }
            }

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // Buffered Spectrum Retrieval Messages
    //
    //=========================================================================
    //

    public class OBPGetBufferedSpectrum : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00100928;


        //----- Data Members -----

        protected uint[] mBufferedSpectrum = null;
        protected uint[] mPixels = null;

        protected uint mMaxValue = 0;
        protected int mMaxValuePixelNum = 0;

        // The defined metadata header fields
        protected uint mSpectrumCount = 0;  // incremented every time a spectrum is digitized
        protected ulong mTickCount = 0;  // tick count at time of acquisition
        protected uint mIntegrationTime = 0;  // integration time in microseconds
        protected byte mTriggerMode = 0;  // the spectrometer trigger mode that was in effect


        //----- Constructors -----

        public OBPGetBufferedSpectrum()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetBufferedSpectrum(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetBufferedSpectrum(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetBufferedSpectrum(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint[] BufferedSpectrum
        {
            get { return mBufferedSpectrum; }
        }

        public uint[] Pixels
        {
            get { return mPixels; }
        }

        public uint MaxValue
        {
            get { return mMaxValue; }
        }

        public int MaxValuePixelNum
        {
            get { return mMaxValuePixelNum; }
        }

        //--- Metadata Header Propperties ---

        public uint SpectrumCount
        {
            get { return mSpectrumCount; }
        }

        public ulong TickCount
        {
            get { return mTickCount; }
        }

        public uint IntegrationTime
        {
            get { return mIntegrationTime; }
        }

        public uint TriggerMode
        {
            get { return mTriggerMode; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                mBufferedSpectrum = mResponse.GetPayloadUIntArray(32);
                mSpectrumCount = mResponse.GetPayloadUInt(0);
                mTickCount = mResponse.GetPayloadULong(4);
                mIntegrationTime = mResponse.GetPayloadUInt(12);
                mTriggerMode = mResponse.GetPayloadByte(18);
            }
            else
            {
                mBufferedSpectrum = new uint[0];
                mSpectrumCount = 0;
                mTickCount = 0;
                mIntegrationTime = 0;
                mTriggerMode = 0;
            }

            mPixels = new uint[mBufferedSpectrum.Length];
            mMaxValue = 0;
            mMaxValuePixelNum = 0;
            for (uint i = 0; i < mBufferedSpectrum.Length; i++)
            {
                mPixels[i] = i;
                if (mBufferedSpectrum[i] > mMaxValue)
                {
                    mMaxValue = mBufferedSpectrum[i];
                    mMaxValuePixelNum = (int)i;
                }
            }

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // Buffered Spectrum Control Messages
    //
    //=========================================================================
    //

    public class OBPGetMaxBufferSize : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00100820;


        //----- Data Members -----

        protected uint mMaxBufferSize = 0;


        //----- Constructors -----

        public OBPGetMaxBufferSize()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetMaxBufferSize(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetMaxBufferSize(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetMaxBufferSize(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint MaxBufferSize
        {
            get { return mMaxBufferSize; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mMaxBufferSize = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }

    public class OBPGetBufferSize : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00100822;


        //----- Data Members -----

        protected uint mBufferSize = 0;


        //----- Constructors -----

        public OBPGetBufferSize()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetBufferSize(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetBufferSize(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetBufferSize(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint BufferSize
        {
            get { return mBufferSize; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mBufferSize = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }

    public class OBPSetBufferSize : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00100832;


        //----- Data Members -----

        protected uint mBufferSize = 0;


        //----- Constructors -----

        public OBPSetBufferSize()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetBufferSize(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetBufferSize(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetBufferSize(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public uint BufferSize
        {
            get { return mBufferSize; }
            set { mBufferSize = value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            mRequest.SetImmediateData(mBufferSize);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPGetNumSpectraInBuffer : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00100900;


        //----- Data Members -----

        protected uint mNumSpectraInBuffer = 0;


        //----- Constructors -----

        public OBPGetNumSpectraInBuffer()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetNumSpectraInBuffer(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetNumSpectraInBuffer(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetNumSpectraInBuffer(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint NumSpectraInBuffer
        {
            get { return mNumSpectraInBuffer; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mNumSpectraInBuffer = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }


    public class OBPRemoveOldestBufferedSpectra : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00100831 ;


        //----- Data Members -----

        protected uint mNumSpectra = 1;


        //----- Constructors -----

        public OBPRemoveOldestBufferedSpectra()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPRemoveOldestBufferedSpectra(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPRemoveOldestBufferedSpectra(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPRemoveOldestBufferedSpectra(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public uint NumSpectra
        {
            get { return mNumSpectra; }
            set { mNumSpectra = value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            mRequest.SetImmediateData(mNumSpectra);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPClearBufferedSpectra : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00100830 ;


        //----- Data Members -----


        //----- Constructors -----

        public OBPClearBufferedSpectra()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPClearBufferedSpectra(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPClearBufferedSpectra(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPClearBufferedSpectra(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----


        //----- Override Send() -----


    }

    public class OBPAcquireSpectraIntoBuffer : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00100902;


        //----- Data Members -----


        //----- Constructors -----

        public OBPAcquireSpectraIntoBuffer()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPAcquireSpectraIntoBuffer(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPAcquireSpectraIntoBuffer(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPAcquireSpectraIntoBuffer(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----


        //----- Override Send() -----


    }

    public class OBPQueryIsIdle : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00100908;


        //----- Data Members -----

        protected byte mIsIdle = 0;


        //----- Constructors -----

        public OBPQueryIsIdle()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPQueryIsIdle(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPQueryIsIdle(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPQueryIsIdle(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte IsIdle
        {
            get { return mIsIdle; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mIsIdle = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // Integration Time Messages
    //
    //=========================================================================
    //

    public class OBPGetIntegrationTime : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110000;


        //----- Data Members -----

        protected uint mIntegrationTime = 0;  // in microns


        //----- Constructors -----

        public OBPGetIntegrationTime()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetIntegrationTime(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetIntegrationTime(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetIntegrationTime(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint IntegrationTime
        {
            get { return mIntegrationTime; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mIntegrationTime = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }

    public class OBPSetIntegrationTime : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110010;


        //----- Data Members -----

        protected uint mIntegrationTime = 0;  // in microseconds


        //----- Constructors -----

        public OBPSetIntegrationTime()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetIntegrationTime(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetIntegrationTime(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetIntegrationTime(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public uint IntegrationTime
        {
            get { return mIntegrationTime; }
            set { mIntegrationTime = value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            mRequest.SetImmediateData(mIntegrationTime);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPGetMinIntegrationTime : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110001;


        //----- Data Members -----

        protected uint mMinIntegrationTime = 0;  // in microseconds


        //----- Constructors -----

        public OBPGetMinIntegrationTime()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetMinIntegrationTime(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetMinIntegrationTime(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetMinIntegrationTime(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint MinIntegrationTime
        {
            get { return mMinIntegrationTime; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mMinIntegrationTime = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }

    public class OBPGetMaxIntegrationTime : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110002;


        //----- Data Members -----

        protected uint mMaxIntegrationTime = 0;  // in microseconds


        //----- Constructors -----

        public OBPGetMaxIntegrationTime()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetMaxIntegrationTime(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetMaxIntegrationTime(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetMaxIntegrationTime(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint MaxIntegrationTime
        {
            get { return mMaxIntegrationTime; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mMaxIntegrationTime = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }

    public class OBPGetIntegrationTimeIncrement : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110003;


        //----- Data Members -----

        protected uint mIntegrationTimeIncrement = 0;  // in microseconds


        //----- Constructors -----

        public OBPGetIntegrationTimeIncrement()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetIntegrationTimeIncrement(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetIntegrationTimeIncrement(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetIntegrationTimeIncrement(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint IntegrationTimeIncrement
        {
            get { return mIntegrationTimeIncrement; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mIntegrationTimeIncrement = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // Triggering Messages
    //
    //=========================================================================
    //

    public class OBPGetTriggerMode : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110100;


        //----- Data Members -----

        protected byte mTriggerMode = 0;


        //----- Constructors -----

        public OBPGetTriggerMode()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetTriggerMode(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetTriggerMode(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetTriggerMode(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte TriggerMode
        {
            get { return mTriggerMode; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mTriggerMode = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPSetTriggerMode : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110110;


        //----- Data Members -----

        protected byte mTriggerMode = 0;


        //----- Constructors -----

        public OBPSetTriggerMode()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetTriggerMode(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetTriggerMode(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetTriggerMode(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public byte TriggerMode
        {
            get { return mTriggerMode; }
            set { mTriggerMode = value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            mRequest.SetImmediateData(mTriggerMode);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPSimulateTriggerPulse : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110120;


        //----- Data Members -----


        //----- Constructors -----

        public OBPSimulateTriggerPulse()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSimulateTriggerPulse(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSimulateTriggerPulse(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSimulateTriggerPulse(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----


        //----- Override Send() -----


    }


    //
    //=========================================================================
    //
    // Other Acquisition Parameter Messages
    //
    //=========================================================================
    //

    public class OBPGetLampEnable : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110400;


        //----- Data Members -----

        protected byte mLampEnable = 0;


        //----- Constructors -----

        public OBPGetLampEnable()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetLampEnable(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetLampEnable(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetLampEnable(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte LampEnable
        {
            get { return mLampEnable; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mLampEnable = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPSetLampEnable : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110410;


        //----- Data Members -----

        protected byte mLampEnable = 0;


        //----- Constructors -----

        public OBPSetLampEnable()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetLampEnable(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetLampEnable(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetLampEnable(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public byte LampEnable
        {
            get { return mLampEnable; }
            set { mLampEnable = value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            mRequest.SetImmediateData(mLampEnable);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPGetAcquisitionDelay : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110500;


        //----- Data Members -----

        protected uint mAcquisitionDelay = 0;


        //----- Constructors -----

        public OBPGetAcquisitionDelay()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetAcquisitionDelay(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetAcquisitionDelay(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetAcquisitionDelay(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint AcquisitionDelay
        {
            get { return mAcquisitionDelay; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mAcquisitionDelay = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }

    public class OBPSetAcquisitionDelay : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110510;


        //----- Data Members -----

        protected uint mAcquisitionDelay = 0;


        //----- Constructors -----

        public OBPSetAcquisitionDelay()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetAcquisitionDelay(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetAcquisitionDelay(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetAcquisitionDelay(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public uint AcquisitionDelay
        {
            get { return mAcquisitionDelay; }
            set { mAcquisitionDelay = value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            mRequest.SetImmediateData(mAcquisitionDelay);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPGetMinAcquisitionDelay : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110501;


        //----- Data Members -----

        protected uint mMinAcquisitionDelay = 0;


        //----- Constructors -----

        public OBPGetMinAcquisitionDelay()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetMinAcquisitionDelay(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetMinAcquisitionDelay(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetMinAcquisitionDelay(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint MinAcquisitionDelay
        {
            get { return mMinAcquisitionDelay; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mMinAcquisitionDelay = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }

    public class OBPGetMaxAcquisitionDelay : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110502;


        //----- Data Members -----

        protected uint mMaxAcquisitionDelay = 0;


        //----- Constructors -----

        public OBPGetMaxAcquisitionDelay()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetMaxAcquisitionDelay(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetMaxAcquisitionDelay(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetMaxAcquisitionDelay(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint MaxAcquisitionDelay
        {
            get { return mMaxAcquisitionDelay; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mMaxAcquisitionDelay = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }

    public class OBPGetAcquisitionDelayIncrement : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110503;


        //----- Data Members -----

        protected uint mAcquisitionDelayIncrement = 0;


        //----- Constructors -----

        public OBPGetAcquisitionDelayIncrement()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetAcquisitionDelayIncrement(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetAcquisitionDelayIncrement(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetAcquisitionDelayIncrement(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint AcquisitionDelayIncrement
        {
            get { return mAcquisitionDelayIncrement; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mAcquisitionDelayIncrement = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }

    public class OBPGetScansToAvg : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00120000;


        //----- Data Members -----

        protected ushort mScansToAvg = 0;


        //----- Constructors -----

        public OBPGetScansToAvg()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetScansToAvg(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetScansToAvg(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetScansToAvg(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public ushort ScansToAvg
        {
            get { return mScansToAvg; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mScansToAvg = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUShort() : (ushort)0;

            return bSuccess;
        }

    }

    public class OBPSetScansToAvg : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00120010;


        //----- Data Members -----

        protected uint mScansToAvg = 0;


        //----- Constructors -----

        public OBPSetScansToAvg()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetScansToAvg(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetScansToAvg(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetScansToAvg(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public uint ScansToAvg
        {
            get { return mScansToAvg; }
            set { mScansToAvg = value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            mRequest.SetImmediateData(mScansToAvg);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPGetBoxcarWidth : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00121000;


        //----- Data Members -----

        protected byte mBoxcarWidth = 0;


        //----- Constructors -----

        public OBPGetBoxcarWidth()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetBoxcarWidth(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetBoxcarWidth(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetBoxcarWidth(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte BoxcarWidth
        {
            get { return mBoxcarWidth; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mBoxcarWidth = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPSetBoxcarWidth : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00121010;


        //----- Data Members -----

        protected byte mBoxcarWidth = 0;


        //----- Constructors -----

        public OBPSetBoxcarWidth()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetBoxcarWidth(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetBoxcarWidth(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetBoxcarWidth(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public byte BoxcarWidth
        {
            get { return mBoxcarWidth; }
            set { mBoxcarWidth = value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            mRequest.SetImmediateData(mBoxcarWidth);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPGetPixelBinningFactor : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110280;


        //----- Data Members -----

        protected byte mBinningFactor = 0;


        //----- Constructors -----

        public OBPGetPixelBinningFactor()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetPixelBinningFactor(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetPixelBinningFactor(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetPixelBinningFactor(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte BinningFactor
        {
            get { return mBinningFactor; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mBinningFactor = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPSetPixelBinningFactor : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110290;


        //----- Data Members -----

        protected byte mBinningFactor = 0;


        //----- Constructors -----

        public OBPSetPixelBinningFactor()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetPixelBinningFactor(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetPixelBinningFactor(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetPixelBinningFactor(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public byte BinningFactor
        {
            get { return mBinningFactor; }
            set { mBinningFactor = value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            mRequest.SetImmediateData(mBinningFactor);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPGetDefaultPixelBinningFactor : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110285;


        //----- Data Members -----

        protected byte mDefaultBinningFactor = 0;


        //----- Constructors -----

        public OBPGetDefaultPixelBinningFactor()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetDefaultPixelBinningFactor(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetDefaultPixelBinningFactor(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetDefaultPixelBinningFactor(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte DefaultBinningFactor
        {
            get { return mDefaultBinningFactor; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mDefaultBinningFactor = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPSetDefaultPixelBinningFactor : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110295;


        //----- Data Members -----

        protected byte mDefaultBinningFactor = 0;

        // Determines if mDefaultBinningFactor is sent
        // false = send mDefaultBinningFactor
        // true = don't send mDefaultBinningFactor (send zero bytes to reset to factory default setting)
        protected bool mResetDefault = false;  


        //----- Constructors -----

        public OBPSetDefaultPixelBinningFactor()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetDefaultPixelBinningFactor(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetDefaultPixelBinningFactor(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetDefaultPixelBinningFactor(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public byte DefaultBinningFactor
        {
            get { return mDefaultBinningFactor; }
            set { mDefaultBinningFactor = value; }
        }

        public bool ResetDefault
        {
            get { return mResetDefault; }
            set { mResetDefault = value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            if (!mResetDefault)
            {
                mRequest.SetImmediateData(mDefaultBinningFactor);
            }

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPGetMaxPixelBinningFactor : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00110281;


        //----- Data Members -----

        protected byte mMaxBinningFactor = 0;


        //----- Constructors -----

        public OBPGetMaxPixelBinningFactor()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetMaxPixelBinningFactor(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetMaxPixelBinningFactor(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetMaxPixelBinningFactor(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte MaxBinningFactor
        {
            get { return mMaxBinningFactor; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mMaxBinningFactor = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }


    public class OBPGetPartialSpectrumMode : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00102000;


        //----- Data Members -----

        protected byte[] mPartialSpectrumMode = new byte[0];


        //----- Constructors -----

        public OBPGetPartialSpectrumMode()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetPartialSpectrumMode(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetPartialSpectrumMode(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetPartialSpectrumMode(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte[] PartialSpectrumMode
        {
            get { return mPartialSpectrumMode; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                mPartialSpectrumMode = (mResponse.ImmediateDataLen > 0) ? mResponse.GetImmediateData() : mResponse.GetPayload();
            }
            else
            {
                mPartialSpectrumMode = new byte[0];
            }

            return bSuccess;
        }

    }

    public class OBPSetPartialSpectrumMode : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00102010;


        //----- Data Members -----

        protected byte[] mPartialSpectrumMode = new byte[0];


        //----- Constructors -----

        public OBPSetPartialSpectrumMode()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetPartialSpectrumMode(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetPartialSpectrumMode(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetPartialSpectrumMode(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }



        //----- Message Specific Properties -----

        public byte[] PartialSpectrumMode
        {
            get { return mPartialSpectrumMode; }
            set { mPartialSpectrumMode = (value == null) ? new byte[0] : value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            if (mPartialSpectrumMode.Length > 0)
            {
                if (mPartialSpectrumMode.Length > 16)
                {
                    mRequest.SetPayload(mPartialSpectrumMode);
                }
                else {
                    mRequest.SetImmediateData(mPartialSpectrumMode);
                }
            }

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // Wavelength Calibration Messages
    //
    //=========================================================================
    //

    public class OBPGetNumWavecalCoefficients : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00180100;


        //----- Data Members -----

        protected byte mNumWavecalCoeffs = 0;


        //----- Constructors -----

        public OBPGetNumWavecalCoefficients()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetNumWavecalCoefficients(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetNumWavecalCoefficients(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetNumWavecalCoefficients(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public byte NumWavecalCoeffs
        {
            get { return mNumWavecalCoeffs; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            bool bSuccess = base.Send();

            mNumWavecalCoeffs = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPGetWavecalCoefficient : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00180101;


        //----- Data Members -----

        protected byte mWavecalIndex = 0;
        protected float mWavecalCoeff = 0;


        //----- Constructors -----

        public OBPGetWavecalCoefficient()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetWavecalCoefficient(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetWavecalCoefficient(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetWavecalCoefficient(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public byte WavecalIndex
        {
            get { return mWavecalIndex; }
            set { mWavecalIndex = value; }
        }

        public float WavecalCoeff
        {
            get { return mWavecalCoeff; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mWavecalIndex);

            bool bSuccess = base.Send();

            mWavecalCoeff = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateFloat() : 0f;

            return bSuccess;
        }

    }


    public class OBPSetWavecalCoefficient : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00180111;


        //----- Data Members -----

        protected byte mWavecalIndex = 0;
        protected float mWavecalCoeff = 0f;


        //----- Constructors -----

        public OBPSetWavecalCoefficient()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetWavecalCoefficient(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetWavecalCoefficient(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetWavecalCoefficient(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----

        public byte WavecalIndex
        {
            get { return mWavecalIndex; }
            set { mWavecalIndex = value; }
        }

        public float WavecalCoeff
        {
            get { return mWavecalCoeff; }
            set { mWavecalCoeff = value; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mWavecalIndex);
            mRequest.SetImmediateData(mWavecalCoeff, 1);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // Nonlinearity Correction Calibration Messages
    //
    //=========================================================================
    //

    public class OBPGetNumNonLinearityCoefficients : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00181100;


        //----- Data Members -----

        protected byte mNumNonLinearityCoeffs = 0;


        //----- Constructors -----

        public OBPGetNumNonLinearityCoefficients()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetNumNonLinearityCoefficients(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetNumNonLinearityCoefficients(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetNumNonLinearityCoefficients(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public byte NumNonLinearityCoeffs
        {
            get { return mNumNonLinearityCoeffs; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            bool bSuccess = base.Send();

            mNumNonLinearityCoeffs = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPGetNonLinearityCoefficient : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00181101 ;


        //----- Data Members -----

        protected byte mNonLinearityIndex = 0;
        protected float mNonLinearityCoeff = 0;


        //----- Constructors -----

        public OBPGetNonLinearityCoefficient()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetNonLinearityCoefficient(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetNonLinearityCoefficient(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetNonLinearityCoefficient(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public byte NonLinearityIndex
        {
            get { return mNonLinearityIndex; }
            set { mNonLinearityIndex = value; }
        }

        public float NonLinearityCoeff
        {
            get { return mNonLinearityCoeff; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mNonLinearityIndex);

            bool bSuccess = base.Send();

            mNonLinearityCoeff = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateFloat() : 0f;

            return bSuccess;
        }

    }


    public class OBPSetNonLinearityCoefficient : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00181111;


        //----- Data Members -----

        protected byte mNonLinearityIndex = 0;
        protected float mNonLinearityCoeff = 0f;


        //----- Constructors -----

        public OBPSetNonLinearityCoefficient()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetNonLinearityCoefficient(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetNonLinearityCoefficient(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetNonLinearityCoefficient(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----

        public byte NonLinearityIndex
        {
            get { return mNonLinearityIndex; }
            set { mNonLinearityIndex = value; }
        }

        public float NonLinearityCoeff
        {
            get { return mNonLinearityCoeff; }
            set { mNonLinearityCoeff = value; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mNonLinearityIndex);
            mRequest.SetImmediateData(mNonLinearityCoeff, 1);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // Irradiance Calibration Messages
    //
    //=========================================================================
    //

    //
    //=========================================================================
    //
    // Stray Light Coefficients
    //
    //=========================================================================
    //

    public class OBPGetNumStrayLightCoefficients : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00183100;


        //----- Data Members -----

        protected byte mNumStrayLightCoeffs = 0;


        //----- Constructors -----

        public OBPGetNumStrayLightCoefficients()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetNumStrayLightCoefficients(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetNumStrayLightCoefficients(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetNumStrayLightCoefficients(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public byte NumStrayLightCoeffs
        {
            get { return mNumStrayLightCoeffs; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            bool bSuccess = base.Send();

            mNumStrayLightCoeffs = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPGetStrayLightCoefficient : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00183101;


        //----- Data Members -----

        protected byte mStrayLightIndex = 0;
        protected float mStrayLightCoeff = 0;


        //----- Constructors -----

        public OBPGetStrayLightCoefficient()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetStrayLightCoefficient(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetStrayLightCoefficient(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetStrayLightCoefficient(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public byte StrayLightIndex
        {
            get { return mStrayLightIndex; }
            set { mStrayLightIndex = value; }
        }

        public float StrayLightCoeff
        {
            get { return mStrayLightCoeff; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mStrayLightIndex);

            bool bSuccess = base.Send();

            mStrayLightCoeff = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateFloat() : 0f;

            return bSuccess;
        }

    }


    public class OBPSetStrayLightCoefficient : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00183111 ;


        //----- Data Members -----

        protected byte mStrayLightIndex = 0;
        protected float mStrayLightCoeff = 0f;


        //----- Constructors -----

        public OBPSetStrayLightCoefficient()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetStrayLightCoefficient(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetStrayLightCoefficient(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetStrayLightCoefficient(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----

        public byte StrayLightIndex
        {
            get { return mStrayLightIndex; }
            set { mStrayLightIndex = value; }
        }

        public float StrayLightCoeff
        {
            get { return mStrayLightCoeff; }
            set { mStrayLightCoeff = value; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mStrayLightIndex);
            mRequest.SetImmediateData(mStrayLightCoeff, 1);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // Bench Information Messages
    //
    //=========================================================================
    //

    public class OBPGetBenchID : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x001B0000;


        //----- Data Members -----

        protected string mBenchID = "";


        //----- Constructors -----

        public OBPGetBenchID()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetBenchID(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetBenchID(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetBenchID(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public string BenchID
        {
            get { return mBenchID; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                byte[] data = (mResponse.ImmediateDataLen > 0) ? mResponse.GetImmediateData() : mResponse.GetPayload();
                mBenchID = BytesToString(data, 32);  // Convert chars >= 32 to a string
            }
            else
            {
                mBenchID = "";
            }

            return bSuccess;
        }

    }

    public class OBPGetBenchSerialNum : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x001B0100;


        //----- Data Members -----

        protected string mBenchSerialNum = "";


        //----- Constructors -----

        public OBPGetBenchSerialNum()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetBenchSerialNum(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetBenchSerialNum(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetBenchSerialNum(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public string BenchSerialNum
        {
            get { return mBenchSerialNum; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                byte[] data = (mResponse.ImmediateDataLen > 0) ? mResponse.GetImmediateData() : mResponse.GetPayload();
                mBenchSerialNum = BytesToString(data, 32);  // Convert chars >= 32 to a string
            }
            else
            {
                mBenchSerialNum = "";
            }

            return bSuccess;
        }

    }

    public class OBPGetBenchSlitWidth : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x001B0200;


        //----- Data Members -----

        protected ushort mBenchSlitWidth = 0;  // in microns


        //----- Constructors -----

        public OBPGetBenchSlitWidth()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetBenchSlitWidth(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetBenchSlitWidth(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetBenchSlitWidth(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public ushort BenchSlitWidth
        {
            get { return mBenchSlitWidth; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mBenchSlitWidth = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUShort() : (ushort)0;

            return bSuccess;
        }

    }

    public class OBPGetBenchFiberDiameter : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x001B0300;


        //----- Data Members -----

        protected ushort mBenchFiberDiameter = 0;  // in microns


        //----- Constructors -----

        public OBPGetBenchFiberDiameter()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetBenchFiberDiameter(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetBenchFiberDiameter(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetBenchFiberDiameter(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public ushort BenchFiberDiameter
        {
            get { return mBenchFiberDiameter; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mBenchFiberDiameter = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUShort() : (ushort)0;

            return bSuccess;
        }

    }

    public class OBPGetBenchGrating : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x001B0400;


        //----- Data Members -----

        protected string mBenchGrating = "";


        //----- Constructors -----

        public OBPGetBenchGrating()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetBenchGrating(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetBenchGrating(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetBenchGrating(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public string BenchGrating
        {
            get { return mBenchGrating; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                byte[] data = (mResponse.ImmediateDataLen > 0) ? mResponse.GetImmediateData() : mResponse.GetPayload();
                mBenchGrating = BytesToString(data, 32);  // Convert chars >= 32 to a string
            }
            else
            {
                mBenchGrating = "";
            }

            return bSuccess;
        }

    }

    public class OBPGetBenchFilter : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x001B0500;


        //----- Data Members -----

        protected string mBenchFilter = "";


        //----- Constructors -----

        public OBPGetBenchFilter()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetBenchFilter(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetBenchFilter(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetBenchFilter(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public string BenchFilter
        {
            get { return mBenchFilter; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                byte[] data = (mResponse.ImmediateDataLen > 0) ? mResponse.GetImmediateData() : mResponse.GetPayload();
                mBenchFilter = BytesToString(data, 32);  // Convert chars >= 32 to a string
            }
            else
            {
                mBenchFilter = "";
            }

            return bSuccess;
        }

    }

    public class OBPGetBenchCoating : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x001B0600;


        //----- Data Members -----

        protected string mBenchCoating = "";


        //----- Constructors -----

        public OBPGetBenchCoating()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetBenchCoating(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetBenchCoating(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetBenchCoating(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public string BenchCoating
        {
            get { return mBenchCoating; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                byte[] data = (mResponse.ImmediateDataLen > 0) ? mResponse.GetImmediateData() : mResponse.GetPayload();
                mBenchCoating = BytesToString(data, 32);  // Convert chars >= 32 to a string
            }
            else
            {
                mBenchCoating = "";
            }

            return bSuccess;
        }

    }

    public class OBPGetDetectorSerialNum : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x001B0700;


        //----- Data Members -----

        protected string mDetectorSerialNum = "";


        //----- Constructors -----

        public OBPGetDetectorSerialNum()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetDetectorSerialNum(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetDetectorSerialNum(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetDetectorSerialNum(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public string DetectorSerialNum
        {
            get { return mDetectorSerialNum; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                byte[] data = (mResponse.ImmediateDataLen > 0) ? mResponse.GetImmediateData() : mResponse.GetPayload();
                mDetectorSerialNum = BytesToString(data, 32);  // Convert chars >= 32 to a string
            }
            else
            {
                mDetectorSerialNum = "";
            }

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // GPIO Messages
    //
    //=========================================================================
    //

    public class OBPGetNumGPIOPins : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00200000;


        //----- Data Members -----

        protected byte mNumPins = 0;


        //----- Constructors -----

        public OBPGetNumGPIOPins()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetNumGPIOPins(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetNumGPIOPins(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetNumGPIOPins(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte NumPins
        {
            get { return mNumPins; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mNumPins = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPGetGPIOOutputEnable : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00200100;


        //----- Data Members -----

        protected uint mOutputEnable = 0;


        //----- Constructors -----

        public OBPGetGPIOOutputEnable()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetGPIOOutputEnable(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetGPIOOutputEnable(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetGPIOOutputEnable(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint OutputEnable
        {
            get { return mOutputEnable; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mOutputEnable = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }

    public class OBPSetGPIOOutputEnable : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00200110;


        //----- Data Members -----

        protected uint mOutputEnable = 0;
        protected uint mOutputEnableMask = 0xffffffff;


        //----- Constructors -----

        public OBPSetGPIOOutputEnable()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetGPIOOutputEnable(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetGPIOOutputEnable(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetGPIOOutputEnable(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----

        public uint OutputEnable
        {
            get { return mOutputEnable; }
            set { mOutputEnable = value; }
        }

        public uint OutputEnableMask
        {
            get { return mOutputEnableMask; }
            set { mOutputEnableMask = value; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mOutputEnable);
            mRequest.SetImmediateData(mOutputEnableMask, 4);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPGetGPIOValue : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00200300;


        //----- Data Members -----

        protected uint mGPIOValue = 0;


        //----- Constructors -----

        public OBPGetGPIOValue()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetGPIOValue(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetGPIOValue(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetGPIOValue(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public uint GPIOValue
        {
            get { return mGPIOValue; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mGPIOValue = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUInt() : (uint)0;

            return bSuccess;
        }

    }

    public class OBPSetGPIOValue : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00200310;


        //----- Data Members -----

        protected uint mValue = 0;
        protected uint mValueMask = 0xffffffff;


        //----- Constructors -----

        public OBPSetGPIOValue()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetGPIOValue(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetGPIOValue(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetGPIOValue(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----

        public uint GPIOValue
        {
            get { return mValue; }
            set { mValue = value; }
        }

        public uint GPIOValueMask
        {
            get { return mValueMask; }
            set { mValueMask = value; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mValue);
            mRequest.SetImmediateData(mValueMask, 4);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // Strobe Messages
    //
    //=========================================================================
    //

    //
    //=========================================================================
    //
    // Temperature Messages
    //
    //=========================================================================
    //

    public class OBPGetNumTemperatureSensors : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00400000;


        //----- Data Members -----

        protected byte mNumTempSensors = 0;


        //----- Constructors -----

        public OBPGetNumTemperatureSensors()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetNumTemperatureSensors(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetNumTemperatureSensors(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetNumTemperatureSensors(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte NumTempSensors
        {
            get { return mNumTempSensors; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mNumTempSensors = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPGetTemperatureSensor : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00400001;


        //----- Data Members -----

        protected byte mTempSensorIndex = 0;
        protected float mTempSensor = 0f;


        //----- Constructors -----

        public OBPGetTemperatureSensor()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetTemperatureSensor(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetTemperatureSensor(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetTemperatureSensor(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public byte TempSensorIndex
        {
            get { return mTempSensorIndex; }
            set { mTempSensorIndex = value; }
        }

        public float TempSensor
        {
            get { return mTempSensor; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mTempSensorIndex);

            bool bSuccess = base.Send();

            mTempSensor = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateFloat() : 0f;

            return bSuccess;
        }

    }

    public class OBPGetAllTemperatureSensors : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00400002;


        //----- Data Members -----

        protected float[] mTempSensors = new float[0];


        //----- Constructors -----

        public OBPGetAllTemperatureSensors()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetAllTemperatureSensors(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetAllTemperatureSensors(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetAllTemperatureSensors(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }


        //----- Message Specific Properties -----

        public float[] TempSensors
        {
            get { return mTempSensors; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                mTempSensors = (mResponse.ImmediateDataLen > 0) ? mResponse.GetImmediateDataFloatArray() : Response.GetPayloadFloatArray(0);
            }
            else
            {
                mTempSensors = new float[0];
            }

            return bSuccess;
        }

    }

    public class OBPGetTecEnable : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00420000;


        //----- Data Members -----

        protected byte mTecEnable = 0;


        //----- Constructors -----

        public OBPGetTecEnable()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetTecEnable(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetTecEnable(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetTecEnable(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte TecEnable
        {
            get { return mTecEnable; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mTecEnable = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPSetTecEnable : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00420010;


        //----- Data Members -----

        protected byte mTecEnable = 0;


        //----- Constructors -----

        public OBPSetTecEnable()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetTecEnable(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetTecEnable(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetTecEnable(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----

        public byte TecEnable
        {
            get { return mTecEnable; }
            set { mTecEnable = value; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mTecEnable);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPGetTecSetpoint : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00420001;


        //----- Data Members -----

        protected float mTecSetpoint = 0;


        //----- Constructors -----

        public OBPGetTecSetpoint()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetTecSetpoint(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetTecSetpoint(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetTecSetpoint(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public float TecSetpoint
        {
            get { return mTecSetpoint; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mTecSetpoint = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateFloat() : 0f;

            return bSuccess;
        }

    }

    public class OBPSetTecSetpoint : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00420011;


        //----- Data Members -----

        protected float mTecSetpoint = 0;


        //----- Constructors -----

        public OBPSetTecSetpoint()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetTecSetpoint(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetTecSetpoint(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetTecSetpoint(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----

        public float TecSetpoint
        {
            get { return mTecSetpoint; }
            set { mTecSetpoint = value; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mTecSetpoint);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }

    public class OBPGetTecStable : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00420003;


        //----- Data Members -----

        protected byte mTecStable = 0;


        //----- Constructors -----

        public OBPGetTecStable()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetTecStable(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetTecStable(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetTecStable(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte TecStable
        {
            get { return mTecStable; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mTecStable = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPGetTecTemperature : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00420004;


        //----- Data Members -----

        protected float mTecTemperature = 0;


        //----- Constructors -----

        public OBPGetTecTemperature()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetTecTemperature(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetTecTemperature(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetTecTemperature(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public float TecTemperature
        {
            get { return mTecTemperature; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mTecTemperature = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateFloat() : 0f;

            return bSuccess;
        }

    }


    //
    //=========================================================================
    //
    // SPI Messages
    //
    //=========================================================================
    //

    //
    //=========================================================================
    //
    // I2C Messages
    //
    //=========================================================================
    //

    public class OBPGetNumI2CBuses : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00600000;


        //----- Data Members -----

        protected byte mNumI2CBuses = 0;


        //----- Constructors -----

        public OBPGetNumI2CBuses()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPGetNumI2CBuses(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPGetNumI2CBuses(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPGetNumI2CBuses(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte NumI2CBuses
        {
            get { return mNumI2CBuses; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            bool bSuccess = base.Send();

            mNumI2CBuses = (bSuccess && !mResponse.IsError) ? mResponse.ImmediateData0 : (byte)0;

            return bSuccess;
        }

    }

    public class OBPReadI2CBus : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00600010;


        //----- Data Members -----

        protected byte mI2CBusNum = 0;
        protected byte mI2CAddress = 0;
        protected ushort mNumBytes = 0;

        protected byte[] mI2CData = new byte[0];


        //----- Constructors -----

        public OBPReadI2CBus()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPReadI2CBus(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPReadI2CBus(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPReadI2CBus(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte I2CBusNum
        {
            get { return mI2CBusNum; }
            set { mI2CBusNum = value; }
        }

        public byte I2CAddress
        {
            get { return mI2CAddress; }
            set { mI2CAddress = value; }
        }

        public ushort NumBytes
        {
            get { return mNumBytes; }
            set { mNumBytes = value; }
        }

        public byte[] I2CData
        {
            get { return mI2CData; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            mRequest.SetImmediateData(mI2CBusNum);
            mRequest.SetImmediateData(mI2CAddress, 1);
            mRequest.SetImmediateData(mNumBytes, 2);

            bool bSuccess = base.Send();

            if (bSuccess && !mResponse.IsError)
            {
                mI2CData = (mResponse.ImmediateDataLen > 0) ? mResponse.GetImmediateData() : mResponse.GetPayload();
            }
            else
            {
                mI2CData = new byte[0];
            }

            return bSuccess;
        }

    }

    public class OBPWriteI2CBus : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00600020;


        //----- Data Members -----

        protected byte mI2CBusNum = 0;
        protected byte mI2CAddress = 0;
        protected ushort mNumBytes = 0;

        protected byte[] mI2CData = new byte[0];


        //----- Constructors -----

        public OBPWriteI2CBus()
            : base(MESSAGE_ID) { mHasResponse = true; }

        public OBPWriteI2CBus(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = true; }

        public OBPWriteI2CBus(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = true; }

        public OBPWriteI2CBus(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = true; }



        //----- Message Specific Properties -----

        public byte I2CBusNum
        {
            get { return mI2CBusNum; }
            set { mI2CBusNum = value; }
        }

        public byte I2CAddress
        {
            get { return mI2CAddress; }
            set { mI2CAddress = value; }
        }

        public ushort NumBytes
        {
            get { return mNumBytes; }
        }

        public byte[] I2CData
        {
            get { return mI2CData; }
            set { mI2CData = (value == null) ? new byte[0] : value; }
        }


        //----- Override Send() -----

        override public bool Send()
        {
            if (mI2CData.Length > 14)
            {
                mRequest.SetPayload(mI2CBusNum, 0);
                mRequest.SetPayload(mI2CAddress, 1);
                mRequest.SetPayloadAtOffset(mI2CData, 2);
            }
            else
            {
                mRequest.SetImmediateData(mI2CBusNum);
                mRequest.SetImmediateData(mI2CAddress, 1);
                mRequest.SetImmediateDataAtOffset(mI2CData, 2);
            }

            bool bSuccess = base.Send();

            mNumBytes = (bSuccess && !mResponse.IsError) ? mResponse.GetImmediateUShort() : (ushort)0;

            return bSuccess;
        }

    }

    public class OBPSetI2CClockLimit : OBPMessage
    {
        //----- Static Data -----

        public const uint MESSAGE_ID = 0x00600090;


        //----- Data Members -----

        protected byte mI2CBusNum = 0;
        protected uint mClockLimit = 0;  // in hz


        //----- Constructors -----

        public OBPSetI2CClockLimit()
            : base(MESSAGE_ID) { mHasResponse = false; }

        public OBPSetI2CClockLimit(ISendReceive io)
            : base(MESSAGE_ID, io) { mHasResponse = false; }

        public OBPSetI2CClockLimit(OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, request, response) { mHasResponse = false; }

        public OBPSetI2CClockLimit(ISendReceive io, OBPBuffer request, OBPBuffer response)
            : base(MESSAGE_ID, io, request, response) { mHasResponse = false; }


        //----- Message Specific Properties -----

        public byte I2CBusNum
        {
            get { return mI2CBusNum; }
            set { mI2CBusNum = value; }
        }

        public uint ClockLimit
        {
            get { return mClockLimit; }
            set { mClockLimit = value; }
        }


        //----- Override Send() -----


        override public bool Send()
        {
            mRequest.SetImmediateData(mI2CBusNum);
            mRequest.SetImmediateData(mClockLimit, 1);

            bool bSuccess = base.Send();

            return bSuccess;
        }

    }


}
