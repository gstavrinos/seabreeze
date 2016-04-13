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
using System.Security.Cryptography;


namespace OBP_Library
{
    //
    // Summary:
    //     Implements an Ocean Binary Protocol (OBP) Message Buffer against a binary data buffer.
    // The various properties and methods of this class facilitate setting and extracting bits
    // and bytes from OBP request and response buffers.
    //
    public class OBPBuffer
    {
        // OBP Header
        //{                                       // byte offset
        //    unsigned char  start_bytes[2];      // 0-1: 0xC1, 0xC0
        //    unsigned short protocol_version;    // 2-3: 0x1100
        //    unsigned short flags;               // 4-5
        //    unsigned short err_no;              // 6-7
        //    unsigned int   message_type;        // 8-11
        //    unsigned int   regarding;           // 12-15
        //    unsigned char  reserved[6];         // 16-21
        //    unsigned char  checksum_type;       // 22
        //    unsigned char  immediate_data_len;  // 23
        //    unsigned char  immediate_data[16];  // 24-39
        //    unsigned int   bytes_remaining;     // 40-43
        //} OBPHeader;                            // 44 bytes total

        //----- Static Data -----

        // Error Number Constants
        public const ushort ERROR_NUMBER_SUCCESS = 0x0000;
        public const ushort ERROR_NUMBER_UNSUPPORTED_PROTOCOL = 1;
        public const ushort ERROR_NUMBER_UNKNOWN_MESSAGE_TYPE = 2;
        public const ushort ERROR_NUMBER_BAD_CHECKSUM = 3;
        public const ushort ERROR_NUMBER_MESSAGE_TOO_LONG = 4;
        public const ushort ERROR_NUMBER_PAYLOAD_LENGTH_WRONG = 5;
        public const ushort ERROR_NUMBER_PAYLOAD_DATA_INVALID = 6;
        public const ushort ERROR_NUMBER_DEVICE_NOT_READY = 7;
        public const ushort ERROR_NUMBER_UNKNOWN_CHECKSUM_TYPE = 8;
        public const ushort ERROR_NUMBER_UNEXPECTED_DEVICE_RESET = 9;
        public const ushort ERROR_NUMBER_TOO_MANY_BUSES = 10;
        public const ushort ERROR_NUMBER_OUT_OF_MEMORY = 11;
        public const ushort ERROR_NUMBER_COMMAND_VALID_INFORMATION_VOID = 12;
        public const ushort ERROR_NUMBER_INTERNAL_DEVICE_ERROR = 13;

        public const ushort ERROR_NUMBER_COULD_NOT_DECRYPT = 100;
        public const ushort ERROR_NUMBER_FIRMWARE_LAYOUT_INVALID = 101;
        public const ushort ERROR_NUMBER_DATA_PACKET_WRONG_SIZE = 102;
        public const ushort ERROR_NUMBER_HARDWARE_REVISION_INCOMPATIBLE_WITH_FIRMWARE = 103;
        public const ushort ERROR_NUMBER_FLASH_MAP_INCOMPATIBLE_WITH_FIRMWARE = 104;

        public const Byte CHECKSUM_TYPE_NONE = 0x00;
        public const Byte CHECKSUM_TYPE_MD5 = 0x01;
        public const Byte CHECKSUM_TYPE_CRC16 = 0x02;
        public const Byte CHECKSUM_TYPE_CRC32 = 0x03;


        static public string[] ERROR_STRINGS = {
            "Success",  // 0
            "Invalid/Unsupported Protocol",  // 1
            "Unknown message type",  // 2
            "Bad checksum",  // 3
            "Message too large",  // 4
            "Payload length does not match message type",  // 5
            "Payload data invalid",  // 6
            "Device not ready for given message type",  // 7
            "Unknown checksum type",  // 8
            "Device reset unexpectedly",  // 9
            "Too many buses. Messages have come from too many different bus interfaces",  // 10
            "Out of memory",  // 11
            "Message is valid, but requested information does not exist",  // 12
            "Internal Error. May be unrecoverable.",  // 13
            "Message did not end properly",  // 14
            "Current scan interrupted",  // 15
        };

        static public string[] FW_REPROGRAMMING_ERROR_STRINGS = {
            "Could not decrypt properly",  // 100
            "Firmware layout invalid",  // 101
            "Data packet was wrong size (not 64 bytes)",  // 102
            "Hardware revision is not compatible with downloaded firmware",  // 103
            "Existing flash map not compatible with downloaded firmware",  // 104
        };

        // Flag field bit definitions
        public const ushort FLAG_DEVICE_RESPONSE       = 0x0001;
        public const ushort FLAG_DEVICE_ACK            = 0x0002;
        public const ushort FLAG_HOST_ACK_REQUEST      = 0x0004;
        public const ushort FLAG_DEVICE_NACK           = 0x0008;
        public const ushort FLAG_DEVICE_EXCEPTION      = 0x0010;
        public const ushort FLAG_DEVICE_PROTOCOL_VERSION_DEPRECATED = 0x0020;
        public const ushort FLAG_DEVICE_MESSAGE_DEPRECATED = 0x0040;

        
        //----- Data Members -----

        // The IO buffer
        protected byte[]    mIoBuffer;

        // The IO status
        protected SendReceiveStatus mIoStatus = SendReceiveStatus.IO_PENDING;


        //----- Constructors -----

        public OBPBuffer() :
            this(5 * 1024)  // default to 5k... should be enough for all OBP messages
        {
        }

        public OBPBuffer(int bufferSize)
        {
            mIoBuffer = new byte[bufferSize];
        }

        public OBPBuffer(OBPBuffer aBufferToClone)
        {
            mIoStatus = aBufferToClone.mIoStatus;
            mIoBuffer = new byte[aBufferToClone.mIoBuffer.Length];
            aBufferToClone.mIoBuffer.CopyTo(this.mIoBuffer, 0);
        }

        //----- Initialization Helpers -----

        public void Init()
        {
            // Zero all bytes
            Array.Clear(mIoBuffer, 0, mIoBuffer.Length);

            StartByte0 = 0xC1;
            StartByte1 = 0xC0;

            ProtocolVersion = 0x1100;
            BytesRemaining = 20;
            SetFooter();

            mIoStatus = SendReceiveStatus.IO_PENDING;
        }

        public void Clear()
        {
            // Zero all bytes
            Array.Clear(mIoBuffer, 0, mIoBuffer.Length);

            mIoStatus = SendReceiveStatus.IO_PENDING;
        }
        

        //----- Provide Access to the IO Buffer and Status -----

        public byte[] IOBuffer
        {
            get { return mIoBuffer; }
        }

        public SendReceiveStatus IOStatus
        {
            get { return mIoStatus; }
            set { mIoStatus = value; }
        }


        //----- Header Properties -----

        public byte StartByte0 { get { return mIoBuffer[0]; } set { mIoBuffer[0] = value; } }
        public byte StartByte1 { get { return mIoBuffer[1]; } set { mIoBuffer[1] = value; } }

        public ushort ProtocolVersion
        {
            get { return (ushort)(mIoBuffer[3] << 8 | mIoBuffer[2]); }
            set {
                mIoBuffer[2] = (byte)(value & 0xff);
                mIoBuffer[3] = (byte)(value >> 8);
            }
        }

        public ushort Flags
        {
            get { return (ushort)(mIoBuffer[5] << 8 | mIoBuffer[4]); }
            set
            {
                mIoBuffer[4] = (byte)(value & 0xff);
                mIoBuffer[5] = (byte)(value >> 8);
            }
        }

        public ushort ErrorNum
        {
            get { return (ushort)(mIoBuffer[7] << 8 | mIoBuffer[6]); }
            set
            {
                mIoBuffer[6] = (byte)(value & 0xff);
                mIoBuffer[7] = (byte)(value >> 8);
            }
        }

        // Return the error string associated with the error number
        public string GetErrorString()
        {
            string errorStr = "Unknown Error";

            int errorNum = ErrorNum;
            if (errorNum < ERROR_STRINGS.Length)
            {
                errorStr = ERROR_STRINGS[errorNum];
            }
            else if (errorNum >= 100)
            {
                errorNum -= 100;
                if (errorNum < FW_REPROGRAMMING_ERROR_STRINGS.Length)
                {
                    errorStr = FW_REPROGRAMMING_ERROR_STRINGS[errorNum];
                }
            }

            return errorStr;
        }

        // Return true if the response indicates an error (NACK or Exception)
        public bool IsError
        {
            get { return (Flags & (FLAG_DEVICE_NACK | FLAG_DEVICE_EXCEPTION)) != 0; }
        }

        public bool AckRequest
        {
            get { return (Flags & FLAG_HOST_ACK_REQUEST) == FLAG_HOST_ACK_REQUEST; }
            set
            {
                if (value) { Flags |= FLAG_HOST_ACK_REQUEST; }
                else { Flags &= (ushort)((~(uint)FLAG_HOST_ACK_REQUEST) & 0xFFFF); }
            }
        }

        public uint MessageType
        {
            get { return (uint)(mIoBuffer[11] << 24 | mIoBuffer[10] << 16 | mIoBuffer[9] << 8 | mIoBuffer[8]); }
            set
            {
                mIoBuffer[8] = (byte)(value & 0xff);
                mIoBuffer[9] = (byte)(value >> 8);
                mIoBuffer[10] = (byte)(value >> 16);
                mIoBuffer[11] = (byte)(value >> 24);
            }
        }

        public uint Regarding
        {
            get { return (uint)(mIoBuffer[15] << 24 | mIoBuffer[14] << 16 | mIoBuffer[13] << 8 | mIoBuffer[12]); }
            set
            {
                mIoBuffer[12] = (byte)(value & 0xff);
                mIoBuffer[13] = (byte)(value >> 8);
                mIoBuffer[14] = (byte)(value >> 16);
                mIoBuffer[15] = (byte)(value >> 24);
            }
        }

        public byte Reserved0 { get { return mIoBuffer[16]; } set { mIoBuffer[16] = value; } }
        public byte Reserved1 { get { return mIoBuffer[17]; } set { mIoBuffer[17] = value; } }
        public byte Reserved2 { get { return mIoBuffer[18]; } set { mIoBuffer[18] = value; } }
        public byte Reserved3 { get { return mIoBuffer[19]; } set { mIoBuffer[19] = value; } }
        public byte Reserved4 { get { return mIoBuffer[20]; } set { mIoBuffer[20] = value; } }
        public byte Reserved5 { get { return mIoBuffer[21]; } set { mIoBuffer[21] = value; } }

        public byte ChecksumType
        {
            get { return mIoBuffer[22]; }
            set { mIoBuffer[22] = value; }
        }

        public byte ImmediateDataLen
        {
            get { return mIoBuffer[23]; }
            set { mIoBuffer[23] = value; }
        }

        public byte ImmediateData0 { get { return mIoBuffer[24]; } set { mIoBuffer[24] = value; } }
        public byte ImmediateData1 { get { return mIoBuffer[25]; } set { mIoBuffer[25] = value; } }
        public byte ImmediateData2 { get { return mIoBuffer[26]; } set { mIoBuffer[26] = value; } }
        public byte ImmediateData3 { get { return mIoBuffer[27]; } set { mIoBuffer[27] = value; } }
        public byte ImmediateData4 { get { return mIoBuffer[28]; } set { mIoBuffer[28] = value; } }
        public byte ImmediateData5 { get { return mIoBuffer[29]; } set { mIoBuffer[29] = value; } }
        public byte ImmediateData6 { get { return mIoBuffer[30]; } set { mIoBuffer[30] = value; } }
        public byte ImmediateData7 { get { return mIoBuffer[31]; } set { mIoBuffer[31] = value; } }
        public byte ImmediateData8 { get { return mIoBuffer[32]; } set { mIoBuffer[32] = value; } }
        public byte ImmediateData9 { get { return mIoBuffer[33]; } set { mIoBuffer[33] = value; } }
        public byte ImmediateData10 { get { return mIoBuffer[34]; } set { mIoBuffer[34] = value; } }
        public byte ImmediateData11 { get { return mIoBuffer[35]; } set { mIoBuffer[35] = value; } }
        public byte ImmediateData12 { get { return mIoBuffer[36]; } set { mIoBuffer[36] = value; } }
        public byte ImmediateData13 { get { return mIoBuffer[37]; } set { mIoBuffer[37] = value; } }
        public byte ImmediateData14 { get { return mIoBuffer[38]; } set { mIoBuffer[38] = value; } }
        public byte ImmediateData15 { get { return mIoBuffer[39]; } set { mIoBuffer[39] = value; } }

        public uint BytesRemaining
        {
            get { return (uint)(mIoBuffer[43] << 24 | mIoBuffer[42] << 16 | mIoBuffer[41] << 8 | mIoBuffer[40]); }
            set
            {
                mIoBuffer[40] = (byte)(value & 0xff);
                mIoBuffer[41] = (byte)(value >> 8);
                mIoBuffer[42] = (byte)(value >> 16);
                mIoBuffer[43] = (byte)(value >> 24);
            }
        }

        // Note: Checksum location is a function of BytesRemaining
        public void ComputeChecksum() {
            uint offset = 44;
            uint bytesRemaining = BytesRemaining;
            if (bytesRemaining < 20) // safety
            {
                BytesRemaining = 20;
            }
            else
            {
                offset += bytesRemaining - 20;
            }

            // Note: 'offset' points to first checksum byte which is also the length of the data to be check summed

            // If an MD5 checksum...
            if (ChecksumType == 1)
            {
                MD5 md5 = MD5.Create();
                byte[] md5Hash = md5.ComputeHash(mIoBuffer, 0, (int)offset);
                if (md5Hash != null)
                {
                    int hashLen = md5Hash.Length;
                    if (hashLen > 16)
                    {
                        hashLen = 16;
                    }
                    // Copy the MD5 hash to the IO buffer checksum
                    Array.Copy(md5Hash, 0, mIoBuffer, offset, hashLen);
                }
            }
            else
            {
                // Initialize to all zeros
                Array.Clear(mIoBuffer, (int)offset, 16);
            }

        }

        public bool IsChecksumMatch()
        {
            bool isMatch = true;

            // If an MD5 checksum...
            if (ChecksumType == 1)
            {
                uint offset = 44;
                uint bytesRemaining = BytesRemaining;
                if (bytesRemaining < 20) // safety
                {
                    BytesRemaining = 20;
                }
                else
                {
                    offset += bytesRemaining - 20;
                }

                // Note: 'offset' points to first checksum byte which is also the length of the data to be check summed

                MD5 md5 = MD5.Create();
                byte[] md5Hash = md5.ComputeHash(mIoBuffer, 0, (int)offset);
                if (md5Hash == null || md5Hash.Length != 16)
                {
                    isMatch = false;
                }
                else
                {
                    for (int i = 0; i < 16; i++)
                    {
                        if (mIoBuffer[offset + i] != md5Hash[i])
                        {
                            isMatch = false;
                            break;
                        }
                    }
                }
            }

            return isMatch;
        }

        // Note: Footer location is a function of BytesRemaining
        public uint Footer
        {
            get
            {
                uint offset = 44;
                uint bytesRemaining = BytesRemaining;
                if (bytesRemaining < 20)
                {
                    BytesRemaining = 20;
                    offset = 60;
                }
                else
                {
                    offset += bytesRemaining - 4;
                }
                return (uint)(mIoBuffer[offset + 3] << 24 | mIoBuffer[offset + 2] << 16 | mIoBuffer[offset + 1] << 8 | mIoBuffer[offset]);
            }
            set
            {
                uint offset = 44;
                uint bytesRemaining = BytesRemaining;
                if (bytesRemaining < 20)
                {
                    BytesRemaining = 20;
                    offset = 60;
                }
                else
                {
                    offset += bytesRemaining - 4;
                }
                mIoBuffer[offset] = (byte)(value & 0xff);
                mIoBuffer[offset + 1] = (byte)(value >> 8);
                mIoBuffer[offset + 2] = (byte)(value >> 16);
                mIoBuffer[offset + 3] = (byte)(value >> 24);
            }
        }

        // Sets the footer value
        public void SetFooter()
        {
            Footer = 0xC2C3C4C5;  // Note: 0xC5 is at offset 60... 0xC2 at offset 63
        }

        // Returns the total message size
        public int MessageSize
        {
            get { return 44 + (int)BytesRemaining; }
        }


        //----- ImmediateData Helpers -----

        public void SetImmediateData(byte[] data, int sourceDataOffset, int offsetIntoImmediateData, int numBytes)
        {
            int iEnd = offsetIntoImmediateData + numBytes;
            if (iEnd > 16)
            {
                iEnd = 16;
                numBytes = iEnd - offsetIntoImmediateData;
                if (numBytes < 0) { numBytes = 0; }
            }
            // Note: dataOffset is not bound checked
            Array.Copy(data, sourceDataOffset, mIoBuffer, 24 + offsetIntoImmediateData, numBytes);
            ImmediateDataLen = (byte)(offsetIntoImmediateData + numBytes);
        }

        public void SetImmediateData(byte[] data, int offsetIntoImmediateData, int numBytes)
        {
            SetImmediateData(data, 0, offsetIntoImmediateData, numBytes);
        }

        public void SetImmediateDataAtOffset(byte[] data, int offsetIntoImmediateData)
        {
            SetImmediateData(data, 0, offsetIntoImmediateData, data.Length);
        }

        public void SetImmediateData(byte[] data, int numberOfBytesFromInputArray)
        {
            if (numberOfBytesFromInputArray > 16)
            {
                numberOfBytesFromInputArray = 16;
            }
            Array.Copy(data, 0, mIoBuffer, 24, numberOfBytesFromInputArray);
            ImmediateDataLen = (byte)numberOfBytesFromInputArray;
        }

        public void SetImmediateData(byte[] data)
        {
            SetImmediateData(data, data.Length);
        }

        public void SetImmediateData(byte data, int offsetIntoImmediateData)
        {
            mIoBuffer[24 + offsetIntoImmediateData] = data;
            ImmediateDataLen = (byte)(offsetIntoImmediateData + 1);
        }

        public void SetImmediateData(byte data)
        {
            SetImmediateData(data, 0);
        }

        public void SetImmediateData(char data, int offsetIntoImmediateData)
        {
            mIoBuffer[24 + offsetIntoImmediateData] = (byte)data;
            ImmediateDataLen = (byte)(offsetIntoImmediateData + 1);
        }

        public void SetImmediateData(char data)
        {
            SetImmediateData(data, 0);
        }

        public void SetImmediateData(ushort data, int offsetIntoImmediateData)
        {
            mIoBuffer[24 + offsetIntoImmediateData] = (byte)(data & 0xff);
            mIoBuffer[25 + offsetIntoImmediateData] = (byte)(data >> 8);
            ImmediateDataLen = (byte)(offsetIntoImmediateData + 2);
        }

        public void SetImmediateData(ushort data)
        {
            SetImmediateData(data, 0);
        }

        public void SetImmediateData(short data, int offsetIntoImmediateData)
        {
            mIoBuffer[24 + offsetIntoImmediateData] = (byte)(data & 0xff);
            mIoBuffer[25 + offsetIntoImmediateData] = (byte)(data >> 8);
            ImmediateDataLen = (byte)(offsetIntoImmediateData + 2);
        }

        public void SetImmediateData(short data)
        {
            SetImmediateData(data, 0);
        }

        public void SetImmediateData(uint data, int offsetIntoImmediateData)
        {
            mIoBuffer[24 + offsetIntoImmediateData] = (byte)(data & 0xff);
            mIoBuffer[25 + offsetIntoImmediateData] = (byte)(data >> 8);
            mIoBuffer[26 + offsetIntoImmediateData] = (byte)(data >> 16);
            mIoBuffer[27 + offsetIntoImmediateData] = (byte)(data >> 24);
            ImmediateDataLen = (byte)(offsetIntoImmediateData + 4);
        }

        public void SetImmediateData(uint data)
        {
            SetImmediateData(data, 0);
        }

        public void SetImmediateData(int data, int offsetIntoImmediateData)
        {
            mIoBuffer[24 + offsetIntoImmediateData] = (byte)(data & 0xff);
            mIoBuffer[25 + offsetIntoImmediateData] = (byte)(data >> 8);
            mIoBuffer[26 + offsetIntoImmediateData] = (byte)(data >> 16);
            mIoBuffer[27 + offsetIntoImmediateData] = (byte)(data >> 24);
            ImmediateDataLen = (byte)(offsetIntoImmediateData + 4);
        }

        public void SetImmediateData(int data)
        {
            SetImmediateData(data, 0);
        }

        public void SetImmediateData(ulong data, int offsetIntoImmediateData)
        {
            mIoBuffer[24 + offsetIntoImmediateData] = (byte)(data & 0xff);
            mIoBuffer[25 + offsetIntoImmediateData] = (byte)(data >> 8);
            mIoBuffer[26 + offsetIntoImmediateData] = (byte)(data >> 16);
            mIoBuffer[27 + offsetIntoImmediateData] = (byte)(data >> 24);
            mIoBuffer[28 + offsetIntoImmediateData] = (byte)(data >> 32);
            mIoBuffer[29 + offsetIntoImmediateData] = (byte)(data >> 40);
            mIoBuffer[30 + offsetIntoImmediateData] = (byte)(data >> 48);
            mIoBuffer[31 + offsetIntoImmediateData] = (byte)(data >> 56);
            ImmediateDataLen = 8;
        }

        public void SetImmediateData(ulong data)
        {
            SetImmediateData(data, 0);
        }

        public void SetImmediateData(long data, int offsetIntoImmediateData)
        {
            mIoBuffer[24 + offsetIntoImmediateData] = (byte)(data & 0xff);
            mIoBuffer[25 + offsetIntoImmediateData] = (byte)(data >> 8);
            mIoBuffer[26 + offsetIntoImmediateData] = (byte)(data >> 16);
            mIoBuffer[27 + offsetIntoImmediateData] = (byte)(data >> 24);
            mIoBuffer[28 + offsetIntoImmediateData] = (byte)(data >> 32);
            mIoBuffer[29 + offsetIntoImmediateData] = (byte)(data >> 40);
            mIoBuffer[30 + offsetIntoImmediateData] = (byte)(data >> 48);
            mIoBuffer[31 + offsetIntoImmediateData] = (byte)(data >> 56);
            ImmediateDataLen = (byte)(offsetIntoImmediateData + 8);
        }

        public void SetImmediateData(long data)
        {
            SetImmediateData(data, 0);
        }

        public void SetImmediateData(float data, int offsetIntoImmediateData)
        {
            SetImmediateDataAtOffset(BitConverter.GetBytes(data), offsetIntoImmediateData);
        }

        public void SetImmediateData(float data)
        {
            SetImmediateData(data, 0);
        }

        public void SetImmediateData(double data, int offsetIntoImmediateData)
        {
            SetImmediateDataAtOffset(BitConverter.GetBytes(data), offsetIntoImmediateData);
        }

        public void SetImmediateData(double data)
        {
            SetImmediateData(data, 0);
        }

        // Return the immediate data as a new byte array
        public byte[] GetImmediateData()
        {
            int dataLen = ImmediateDataLen;
            byte[] immediateData = new byte[dataLen];
            Array.Copy(mIoBuffer, 24, immediateData, 0, dataLen);
            return immediateData;
        }

        // Return the immediate data as a new ushort array
        public ushort[] GetImmediateDataUShortArray()
        {
            int numVals = ImmediateDataLen / 2;
            if (numVals < 0) { numVals = 0; }
            ushort[] dataA = new ushort[numVals];

            int startOffset = 24;
            Buffer.BlockCopy(mIoBuffer, startOffset, dataA, 0, numVals * 2);

            return dataA;
        }

        // Return the immediate data as a new uint array
        public uint[] GetImmediateDataUIntArray()
        {
            int numVals = ImmediateDataLen / 4;
            if (numVals < 0) { numVals = 0; }
            uint[] dataA = new uint[numVals];

            int startOffset = 24;
            Buffer.BlockCopy(mIoBuffer, startOffset, dataA, 0, numVals * 4);

            return dataA;
        }

        // Return the immediate data as a new float array
        public float[] GetImmediateDataFloatArray()
        {
            int numVals = ImmediateDataLen / 4;
            if (numVals < 0) { numVals = 0; }
            float[] dataA = new float[numVals];

            int startOffset = 24;
            Buffer.BlockCopy(mIoBuffer, startOffset, dataA, 0, numVals * 4);

            return dataA;
        }

        public Byte GetImmediateByte(int offsetIntoImmediateData)
        {
            return (Byte)(mIoBuffer[24 + offsetIntoImmediateData]);
        }

        public Byte GetImmediateByte()
        {
            return GetImmediateByte(0);
        }

        public short GetImmediateShort(int offsetIntoImmediateData)
        {
            return (short)(mIoBuffer[25 + offsetIntoImmediateData] << 8 | mIoBuffer[24 + offsetIntoImmediateData]);
        }

        public short GetImmediateShort()
        {
            return GetImmediateShort(0);
        }

        public ushort GetImmediateUShort(int offsetIntoImmediateData)
        {
            return (ushort)(mIoBuffer[25 + offsetIntoImmediateData] << 8 | mIoBuffer[24 + offsetIntoImmediateData]);
        }

        public ushort GetImmediateUShort()
        {
            return GetImmediateUShort(0);
        }

        public int GetImmediateInt(int offsetIntoImmediateData)
        {
            return (int)(mIoBuffer[27 + offsetIntoImmediateData] << 24 | mIoBuffer[26 + offsetIntoImmediateData] << 16 | mIoBuffer[25 + offsetIntoImmediateData] << 8 | mIoBuffer[24 + offsetIntoImmediateData]);
        }

        public int GetImmediateInt()
        {
            return GetImmediateInt(0);
        }

        public uint GetImmediateUInt(int offsetIntoImmediateData)
        {
            return (uint)(mIoBuffer[27 + offsetIntoImmediateData] << 24 | mIoBuffer[26 + offsetIntoImmediateData] << 16 | mIoBuffer[25 + offsetIntoImmediateData] << 8 | mIoBuffer[24 + offsetIntoImmediateData]);
        }

        public uint GetImmediateUInt()
        {
            return GetImmediateUInt(0);
        }

        public long GetImmediateLong(int offsetIntoImmediateData)
        {
            return BitConverter.ToInt64(mIoBuffer, 24 + offsetIntoImmediateData);
        }

        public long GetImmediateLong()
        {
            return GetImmediateLong(0);
        }

        public ulong GetImmediateULong(int offsetIntoImmediateData)
        {
            return BitConverter.ToUInt64(mIoBuffer, 24 + offsetIntoImmediateData);
        }

        public ulong GetImmediateULong()
        {
            return GetImmediateULong(0);
        }

        public float GetImmediateFloat(int offsetIntoImmediateData)
        {
            return BitConverter.ToSingle(mIoBuffer, 24 + offsetIntoImmediateData);
        }

        public float GetImmediateFloat()
        {
            return GetImmediateFloat(0);
        }

        public double GetImmediateDouble(int offsetIntoImmediateData)
        {
            return BitConverter.ToDouble(mIoBuffer, 24 + offsetIntoImmediateData);
        }

        public double GetImmediateDouble()
        {
            return GetImmediateDouble(0);
        }



        //----- Payload Helpers -----

        public int PayloadLen
        {
            get
            {
                int bytesRemaining = (int)BytesRemaining;
                return (bytesRemaining < 20) ? 0 : bytesRemaining - 20;
            }
            set
            {
                BytesRemaining = (value < 0) ? 20 : (uint)(value + 20);
            }
        }

        public void SetPayload(byte[] data, int dataOffset, int offset, int numBytes)
        {
            int iEnd = 64 + offset + numBytes;
            int maxPayload = mIoBuffer.Length - 64;
            if (iEnd > maxPayload)
            {
                iEnd = mIoBuffer.Length;
                numBytes = iEnd - offset;
                if (numBytes < 0) { numBytes = 0; }
            }
            // Note: dataOffset is not bound checked
            Array.Copy(data, dataOffset, mIoBuffer, 44 + offset, numBytes);
            PayloadLen = offset + numBytes;
        }

        public void SetPayload(byte[] data, int offset, int numBytes)
        {
            SetPayload(data, 0, offset, numBytes);
        }

        public void SetPayloadAtOffset(byte[] data, int offset)
        {
            SetPayload(data, 0, offset, data.Length);
        }

        // Copy the specified data buffer into the payload
        public void SetPayload(byte[] data, int numBytes)
        {
            if (numBytes > (mIoBuffer.Length - 64)) {
                throw new System.ArgumentException("SetPayload() parameter 'numBytes' is too large for IO buffer");
            }
            Array.Copy(data, 0, mIoBuffer, 44, numBytes);
            BytesRemaining = (uint)(numBytes + 20);
            SetFooter();
        }

        public void SetPayload(byte[] data)
        {
            SetPayload(data, data.Length);
        }

        public void SetPayload(byte data, int offset)
        {
            mIoBuffer[44 + offset] = (byte)data;
            PayloadLen = offset + 1;
        }

        public void SetPayload(char data, int offset)
        {
            mIoBuffer[44 + offset] = (byte)data;
            PayloadLen = offset + 1;
        }

        public void SetPayload(ushort data, int offset)
        {
            mIoBuffer[44 + offset] = (byte)(data & 0xff);
            mIoBuffer[45 + offset] = (byte)(data >> 8);
            PayloadLen = offset + 2;
        }

        public void SetPayload(short data, int offset)
        {
            mIoBuffer[44 + offset] = (byte)(data & 0xff);
            mIoBuffer[45 + offset] = (byte)(data >> 8);
            PayloadLen = offset + 2;
        }

        public void SetPayload(uint data, int offset)
        {
            mIoBuffer[44 + offset] = (byte)(data & 0xff);
            mIoBuffer[45 + offset] = (byte)(data >> 8);
            mIoBuffer[46 + offset] = (byte)(data >> 16);
            mIoBuffer[47 + offset] = (byte)(data >> 24);
            PayloadLen = offset + 4;
        }

        public void SetPayload(int data, int offset)
        {
            mIoBuffer[44 + offset] = (byte)(data & 0xff);
            mIoBuffer[45 + offset] = (byte)(data >> 8);
            mIoBuffer[46 + offset] = (byte)(data >> 16);
            mIoBuffer[47 + offset] = (byte)(data >> 24);
            PayloadLen = offset + 4;
        }

        public void SetPayload(ulong data, int offset)
        {
            mIoBuffer[44 + offset] = (byte)(data & 0xff);
            mIoBuffer[45 + offset] = (byte)(data >> 8);
            mIoBuffer[46 + offset] = (byte)(data >> 16);
            mIoBuffer[47 + offset] = (byte)(data >> 24);
            mIoBuffer[48 + offset] = (byte)(data >> 32);
            mIoBuffer[49 + offset] = (byte)(data >> 40);
            mIoBuffer[50 + offset] = (byte)(data >> 48);
            mIoBuffer[51 + offset] = (byte)(data >> 56);
            PayloadLen = offset + 8;
        }

        public void SetPayload(long data, int offset)
        {
            mIoBuffer[44 + offset] = (byte)(data & 0xff);
            mIoBuffer[45 + offset] = (byte)(data >> 8);
            mIoBuffer[46 + offset] = (byte)(data >> 16);
            mIoBuffer[47 + offset] = (byte)(data >> 24);
            mIoBuffer[48 + offset] = (byte)(data >> 32);
            mIoBuffer[49 + offset] = (byte)(data >> 40);
            mIoBuffer[50 + offset] = (byte)(data >> 48);
            mIoBuffer[51 + offset] = (byte)(data >> 56);
            PayloadLen = offset + 8;
        }

        public void SetPayload(float data, int offset)
        {
            SetPayloadAtOffset(BitConverter.GetBytes(data), offset);
        }

        public void SetPayload(double data, int offset)
        {
            SetPayloadAtOffset(BitConverter.GetBytes(data), offset);
        }


        // Return the payload data into a new buffer
        public byte[] GetPayload()
        {
            int payloadLen = PayloadLen;
            byte[] payload = new byte[payloadLen];
            Array.Copy(mIoBuffer, 44, payload, 0, payloadLen);

            return payload;
        }

        // Return the payload data at the specified offset into a new buffer
        public byte[] GetPayload(int offset, int numBytes)
        {
            int iEnd = offset + numBytes;
            if (iEnd > (mIoBuffer.Length - 64))
            {
                throw new System.ArgumentException("SetPayload() parameters 'offset + numBytes' is too large for IO buffer");
            }
            byte[] payload = new byte[numBytes];
            Array.Copy(mIoBuffer, 44 + offset, payload, 0, numBytes);

            return payload;
        }

        // Return the remaining payload data at the specified offset into a new buffer
        public byte[] GetPayload(int offset)
        {
            int numBytes = PayloadLen - offset;
            if (numBytes < 0) { numBytes = 0; }
            return GetPayload(offset, numBytes);
        }

        // Return the remaining payload data at the specified offset into a new ushort array
        public ushort[] GetPayloadUShortArray(int offset)
        {
            int numVals = (PayloadLen - offset) / 2;
            if (numVals < 0) { numVals = 0; }
            ushort[] payloadA = new ushort[numVals];

            int startOffset = 44 + offset;
            Buffer.BlockCopy(mIoBuffer, startOffset, payloadA, 0, numVals * 2);

            return payloadA;
        }

        // Return the remaining payload data at the specified offset into a new uint array
        public uint[] GetPayloadUIntArray(int offset)
        {
            int numVals = (PayloadLen - offset) / 4;
            if (numVals < 0) { numVals = 0; }
            uint[] payloadA = new uint[numVals];

            int startOffset = 44 + offset;
            Buffer.BlockCopy(mIoBuffer, startOffset, payloadA, 0, numVals * 4);

            return payloadA;
        }

        // Return the remaining payload data at the specified offset into a new float array
        public float[] GetPayloadFloatArray(int offset)
        {
            int numVals = (PayloadLen - offset) / 4;
            if (numVals < 0) { numVals = 0; }
            float[] payloadA = new float[numVals];

            int startOffset = 44 + offset;
            Buffer.BlockCopy(mIoBuffer, startOffset, payloadA, 0, numVals * 4);

            return payloadA;
        }

        public byte GetPayloadByte(int offset)
        {
            return mIoBuffer[44 + offset];
        }

        public char GetPayloadChar(int offset)
        {
            return (char)mIoBuffer[44 + offset];
        }

        public short GetPayloadShort(int offset)
        {
            return (short)(mIoBuffer[45 + offset] << 8 | mIoBuffer[44 + offset]);
        }

        public ushort GetPayloadUShort(int offset)
        {
            return (ushort)(mIoBuffer[45 + offset] << 8 | mIoBuffer[44 + offset]);
        }

        public int GetPayloadInt(int offset)
        {
            return (int)(mIoBuffer[47 + offset] << 24 | mIoBuffer[46 + offset] << 16 | mIoBuffer[45 + offset] << 8 | mIoBuffer[44 + offset]);
        }

        public uint GetPayloadUInt(int offset)
        {
            return (uint)(mIoBuffer[47 + offset] << 24 | mIoBuffer[46 + offset] << 16 | mIoBuffer[45 + offset] << 8 | mIoBuffer[44 + offset]);
        }

        public long GetPayloadLong(int offset)
        {
            return BitConverter.ToInt64(mIoBuffer, 44 + offset);
        }

        public ulong GetPayloadULong(int offset)
        {
            return BitConverter.ToUInt64(mIoBuffer, 44 + offset);
        }

        public float GetPayloadFloat(int offset)
        {
            return BitConverter.ToSingle(mIoBuffer, 44 + offset);
        }

        public double GetPayloadDouble(int offset)
        {
            return BitConverter.ToDouble(mIoBuffer, 44 + offset);
        }

    }
}
