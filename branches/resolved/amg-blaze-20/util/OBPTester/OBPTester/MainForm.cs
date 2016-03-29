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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.IO.Ports;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Globalization;
using System.Windows.Forms.DataVisualization.Charting;
using OceanUtil;
using MadWizard.WinUSBNet;
using OBP_Library;

namespace OBP_RS232
{
    public partial class MainForm : Form
    {

        //----- Static Data -----

        protected static Color SUCCESS_COLOR = Color.Green;
        protected static Color FAIL_COLOR = Color.Red;
        protected static Color NORMAL_COLOR = SystemColors.ControlText;

        protected static int TAB_RS232 = 0;
        protected static int TAB_TCPIP = 1;
        protected static int TAB_USB = 2;

        protected static string[] NO_COM_PORTS = {
            "none",
        };

        protected static string[] BAUDRATE_STRINGS = {
            "300",
            "600",
            "1200",
            "2400",
            "9600",
            "14400",
            "19200",
            "38400",
            "57600",
            "115200",
            "230400",
            "460800",
            "921600",
        };

        protected static string[] DATABIT_STRINGS = {
            "8",
            "7",
            "6",
            "5",
        };

        protected static string[] PARITY_STRINGS = {
            "None",
            "Odd",
            "Even",
            "Mark",
            "Space",
        };

        protected static string[] STOPBIT_STRINGS = {
            "0",
            "1",
            "2",
            "1.5",
        };

        protected static string[] HANDSHAKE_STRINGS = {
            "None",
            "XOnXOff (SW)",
            "RTS (HW)",
            "RTS-XOnXOff (SW & HW)",
        };


        //----- Data Members -----

        // The IO object
        protected RS232IO mRS232IO = null;
        protected ISendReceive mActiveIO = null;
        protected TcpIpIO mTCPIO = null;  // TODO: Implement TCP IO
        protected USBIO mUSBIO = null;

        protected int mSendTimeoutMS = 2000;
        protected int mReceiveTimeoutMS = 5000;

        // The IO buffers
        protected OBPBuffer mRequest = new OBPBuffer();
        protected OBPBuffer mResponse = new OBPBuffer();

        // The list of available COM ports
        protected string[] mComPorts;

        // Assorted flags
        protected bool mTestSuccess = false;
        protected bool mTestRequired = true;
        protected bool mFwSyncRequired = false;

        // The OBP Protocol Version
        protected ushort mProtocolVersion = 0x1100;  // QEPro - 0x1100, Some STS = 0x1000

        protected uint mRegarding = 0x00000000;  // Regarding (optional identifier for use by host)

        // The active button (to single thread IO without disabling every button)
        protected Button mActiveButton = null;

        // The list of available user strings
        protected string[] mUserStringIndexes;

        // Available coefficient indexes
        protected string[] mNonLinearityIndexes;
        protected string[] mWavecalIndexes;
        protected string[] mStrayLightIndexes;

        // The list of available temperature sensors
        protected string[] mTempSensorIndexes;

        // The list of available I2C buses
        protected string[] mI2CBusIndexes;


        //----- Constructor -----

        public MainForm()
        {
            InitializeComponent();

            // App title... include the version
            string appName = Assembly.GetEntryAssembly().GetName().Name;
            string appVersion = Assembly.GetEntryAssembly().GetName().Version.ToString();
            this.Text = String.Format("{0} v{1}", appName, appVersion);


            //----------------------------------------------------------------------------
            //          Set up RS232
            //----------------------------------------------------------------------------
            //--- Get the list of COM ports ---

            try
            {
                mComPorts = RS232IO.GetPortNames();
            }
            catch (Exception) {
                // Nothing to do
            }

            if (mComPorts == null || mComPorts.Length == 0)
            {
                mComPorts = NO_COM_PORTS;
            }

            int defaultComPortIndex = 0;
            for (int i = 0; i < mComPorts.Length; i++)
            {
                if (mComPorts[i] == "COM1")
                {
                    defaultComPortIndex = i;  // COM1 by default (preferred)
                    break;
                }
                else if (mComPorts[i] == "COM2")
                {
                    defaultComPortIndex = i;  // keep checking for COM1
                }
            }


            //--- Initialize the configuration combo boxes ---

            comboBoxComPort.DataSource = mComPorts;
            comboBoxComPort.SelectedIndex = defaultComPortIndex;  // Default to COM1 or COM2 if present
            comboBoxBaudRate.DataSource = BAUDRATE_STRINGS;
            comboBoxBaudRate.SelectedIndex = 4;  // 9600 default
            comboBoxDataBits.DataSource = DATABIT_STRINGS;
            comboBoxParity.DataSource = PARITY_STRINGS;
            comboBoxStopBits.DataSource = STOPBIT_STRINGS;
            comboBoxStopBits.SelectedIndex = 1;  // 1 default
            comboBoxHandshaking.DataSource = HANDSHAKE_STRINGS;

            textBoxProtocolVersion.Text = String.Format("0x{0:X4}", (int)mProtocolVersion);
            textBoxRegarding.Text = String.Format("0x{0:X8}", (int)mRegarding);

            labelPortNum.Text = "---";
            labelIPAddress.Text = "---";

        }

        //----- Helper Methods -----

        protected void setTestRequired()
        {
            mTestRequired = true;

            setFwSyncRequired(false);

            // TODO: Indicate test required
            // TODO: Disable all OBP Messages & FW Sync button

        }

        protected void setTestSuccess()
        {
            mTestRequired = false;
            mTestSuccess = true;

            setFwSyncRequired(false);

            // TODO: Enable all OBP Messages & FW Sync button

        }

        protected void setTestFailed(string failMsg)
        {
            mTestRequired = true;
            mTestSuccess = false;

            setStatusFailed(failMsg);
        }

        protected void setStatusFailed(string failMsg)
        {
            // if this is called from a thread there is a cross thread exception thrown.
            //  use a static variable then and pick the value up after the thread is done.

            labelIOStatus.Text = failMsg;
            labelIOStatus.ForeColor = FAIL_COLOR;

            labelOBPStatus.Text = "n/a";
            labelOBPStatus.ForeColor = NORMAL_COLOR;
        }

        protected void setFwSyncRequired()
        {
            mFwSyncRequired = true;
        }

        protected void setFwSyncRequired(bool bRequired)
        {
            if (!mTestSuccess)
            {
                bRequired = false;
            }
            mFwSyncRequired = bRequired;
            buttonSyncFW.Enabled = bRequired;
        }

        protected void doTest()
        {
            tabControlContent.Enabled = false;

            if (tabControlProtocol.SelectedIndex == TAB_RS232)
            {
                doTestRS232();
            }
            else if (tabControlProtocol.SelectedIndex == TAB_TCPIP)
            {
                // TODO: implement TCP test
                doTestTCP();
            }
            else if (tabControlProtocol.SelectedIndex == TAB_USB)
            {
                doTestUSB();
            }
        }

        protected void doTestRS232()
        {
            buttonTest.Enabled = false;

            clearMessageStatus();

            try
            {
                // Create a new COM port if the port name changes...
                string portName = mComPorts[comboBoxComPort.SelectedIndex];
                if (mRS232IO == null || portName != mRS232IO.PortName)
                {
                    if (mRS232IO != null)
                    {
                        mRS232IO.Close();
                    }
                    mRS232IO = new RS232IO(portName);
                }

                mActiveIO = mRS232IO;

                mRS232IO.setReceiveTimeout(mReceiveTimeoutMS);
                mRS232IO.setReceiveTimeout(mSendTimeoutMS);

                // Initialize the COM port
                mRS232IO.BaudRate = Int32.Parse(BAUDRATE_STRINGS[comboBoxBaudRate.SelectedIndex]);
                mRS232IO.DataBits = Int32.Parse(DATABIT_STRINGS[comboBoxDataBits.SelectedIndex]);
                mRS232IO.Parity = (Parity)comboBoxParity.SelectedIndex;
                mRS232IO.StopBits = (StopBits)comboBoxStopBits.SelectedIndex;
                mRS232IO.Handshake = (Handshake)comboBoxHandshaking.SelectedIndex;

                // Sync the current settings labels
                labelPortName.Text = mRS232IO.PortName;
                labelBaudRate.Text = mRS232IO.BaudRate.ToString();
                labelDataBits.Text = mRS232IO.DataBits.ToString();
                labelParity.Text = PARITY_STRINGS[comboBoxParity.SelectedIndex];
                labelStopBits.Text = STOPBIT_STRINGS[comboBoxStopBits.SelectedIndex];
                labelHandshaking.Text = HANDSHAKE_STRINGS[comboBoxHandshaking.SelectedIndex];

                try
                {
                    if (!mRS232IO.IsOpen)
                    {
                        mRS232IO.Open();
                    }

                    if (!mRS232IO.IsOpen)
                    {
                        buttonTest.Enabled = true;
                        setTestFailed("Failed to open RS232 port");
                    }
                    else
                    {
                        Thread testThread = new Thread(doTestIO);
                        testThread.Start();
                        tabControlContent.Enabled = true;

                    }
                }
                catch (Exception)
                {
                    buttonTest.Enabled = true;
                    setTestFailed("Failed to open RS232 port");
                }

            }
            catch (Exception)
            {
                buttonTest.Enabled = true;
                setTestFailed("Error initializing RS232 port settings");
            }
        }

        protected void doTestTCP()
        {
            buttonTest.Enabled = false;

            clearMessageStatus();

            try
            {
                // Create a new COM port if the port name changes...
                IPAddress ipAddress = null;
                try {
                    ipAddress = IPAddress.Parse(textBoxIPAddress.Text);
                }
                catch (Exception) {
                    // nothing to do
                }

                if (ipAddress == null) {
                    buttonTest.Enabled = true;
                    setTestFailed("Invalid IP Address");
                    return;
                }

                int portNum = -1;
                try {
                    portNum = int.Parse(textBoxPortNum.Text);
                }
                catch (Exception) {
                    // nothing to do
                }

                if (portNum < 0) {
                    buttonTest.Enabled = true;
                    setTestFailed("Invalid Port #");
                    return;
                }

                if (mTCPIO == null || !ipAddress.Equals(mTCPIO.ipAddress) || portNum != mTCPIO.PortNum)
                {
                    if (mTCPIO != null)
                    {
                        mTCPIO.Close(1000);
                    }
                    mTCPIO = new TcpIpIO(ipAddress, portNum);

                }

                mActiveIO = mTCPIO;

                try
                {
                    if (!mTCPIO.Connected)
                    {
                        mTCPIO.Connect();
                    }

                    if (!mTCPIO.Connected)
                    {
                        buttonTest.Enabled = true;
                        setTestFailed("Failed to open TCPIP socket");
                    }
                    else
                    {
                        // Attempt to connect
                        Thread tcpConnectThread = new Thread(doTestIO);
                        tcpConnectThread.Start();
                        tabControlContent.Enabled = true;
                        buttonTest.Enabled = false;
                    }
                }
                catch (Exception)
                {
                    buttonTest.Enabled = true;
                    setTestFailed("Failed to open RS232 port");
                }

            }
            catch (Exception)
            {
                buttonTest.Enabled = true;
                setTestFailed("Error initializing RS232 port settings");
            }

        }

        protected void InitializeUSBTab()
        {
            USBDeviceInfo[] availableDevices = USBDevice.GetDevices("DBBAD306-1786-4f2e-A8AB-340D45F0653F");
            dataGridViewUSBDeviceList.Rows.Clear();
            if(availableDevices.Count()>0)
            {
                dataGridViewUSBDeviceList.SelectionChanged -= dataGridViewUSBDeviceList_SelectionChanged;

                foreach(USBDeviceInfo di in availableDevices)
                {
                    dataGridViewUSBDeviceList.Rows.Add(di.DeviceDescription, di.PID.ToString("X4"));
                    dataGridViewUSBDeviceList.Rows[dataGridViewUSBDeviceList.Rows.Count - 1].Selected = false;
                    dataGridViewUSBDeviceList.Rows[dataGridViewUSBDeviceList.Rows.Count - 1].Tag = di;

                    if ((di.PID >= 0x4000) && (di.PID <= 0x5000))
                        dataGridViewUSBDeviceList.Rows[dataGridViewUSBDeviceList.Rows.Count-1].DefaultCellStyle.Font = new Font(dataGridViewUSBDeviceList.DefaultCellStyle.Font, FontStyle.Bold);
                    else
                        dataGridViewUSBDeviceList.Rows[dataGridViewUSBDeviceList.Rows.Count - 1].DefaultCellStyle.Font = new Font(dataGridViewUSBDeviceList.DefaultCellStyle.Font, FontStyle.Regular);
                }
                dataGridViewUSBDeviceList.EndEdit();
                dataGridViewUSBDeviceList.CommitEdit(DataGridViewDataErrorContexts.Commit);

                dataGridViewUSBDeviceList.SelectionChanged += dataGridViewUSBDeviceList_SelectionChanged;

                // get first OBP spectrometer if there is one
                foreach (DataGridViewRow r in dataGridViewUSBDeviceList.Rows)
                {
                    if (r.InheritedStyle.Font.Bold)
                    {
                        r.Selected = true;
                        break;
                    }
                }
            }
        }
        protected void doTestUSB()
        {
            USBPipePolicy inPipePolicy = null;
            USBPipePolicy outPipePolicy = null;
           
            try
            {
                if (USBIO.aSpectrometer != null)
                {
                    mActiveIO = mUSBIO;
                    // what kind of spectrometer is it? only FX2 protocol can be tested
                    String name = USBIO.aSpectrometer.Descriptor.FullName;
                    int productId = USBIO.aSpectrometer.Descriptor.PID;
                    String product = USBIO.aSpectrometer.Descriptor.Product;
                    Byte protocol = USBIO.aSpectrometer.Descriptor.Protocol;
                    String serialNumber = USBIO.aSpectrometer.Descriptor.SerialNumber;
                    int vendorID = USBIO.aSpectrometer.Descriptor.VID;

                    //         sts                       qe pro               pyreos                     spark
                    if ((productId == 0x4000) || (productId == 0x4004) || (productId == 0x4005) || (productId == 0x4200))
                    {
                        // assume only one interface. This is normally a reasonable simplification. For Ocean Optics spectrometers
                        USBIO.inPipe = (USBPipe)(dataGridViewInPipes.Rows[dataGridViewInPipes.SelectedRows[0].Index].Tag);
                        inPipePolicy = USBIO.inPipe.Policy;
                        inPipePolicy.AllowPartialReads = true; // Reads do not fail when device returns more data than expected
                        inPipePolicy.AutoFlush = false; // when more data than expected is returned, do not discard remaining data
                        inPipePolicy.IgnoreShortPackets = true; // read operations are completed only when the requested number of bytes is received
                        inPipePolicy.AutoClearStall = true; // driver clears failed transfers
                        
                        //inPipePolicy.PipeTransferTimeout = 2000; // two second timout
                        mUSBIO.setReceiveTimeout(mReceiveTimeoutMS);

                        USBIO.outPipe = (USBPipe)(dataGridViewOutPipes.Rows[dataGridViewOutPipes.SelectedRows[0].Index].Tag);
                        outPipePolicy = USBIO.outPipe.Policy;
                        outPipePolicy.ShortPacketTerminate = false; // write requests that are multiples of the max packet size are no null terminated
                        outPipePolicy.AutoClearStall = true;
                        
                        //outPipePolicy.PipeTransferTimeout = 2000;
                        mUSBIO.setSendTimeout(mSendTimeoutMS);

                        Thread testThread = new Thread(doTestIO);
                        testThread.Start();
                        tabControlContent.Enabled = true;
                        buttonTest.Enabled = false;


                    }
                    else
                    {
                        buttonTest.Enabled = true;
                        setTestFailed("The chosen spectrometer does not use OBP.");
                    }
                }
                else
                {
                    buttonTest.Enabled = true;
                    setTestFailed("Failed to find a spectrometer on the USB bus.");
                }

            }
            catch (USBException usbEX)
            {
                buttonTest.Enabled = true;
                setTestFailed("USB Error: " + usbEX.Message);

            }
            catch (Exception ex)
            {
                buttonTest.Enabled = true;
                setTestFailed("Error initializing USB: "+ex.Message);
            }
    }

        protected void doTcpConnect()
        {
            mTCPIO.setReceiveTimeout(mReceiveTimeoutMS);
            mTCPIO.setReceiveTimeout(mSendTimeoutMS);

            if (!mTCPIO.Connect())
            {
                Invoke((MethodInvoker)delegate
                {
                    onTcpConnectFailed();  // Invoke on UI thread
                });
            }
            else
            {
                Invoke((MethodInvoker)delegate
                {
                    onTcpConnectSuccess();  // Invoke on UI thread
                });

                doTestTCP();
            }
        }

        protected void doTcpDisconnect()
        {
            try
            {
                mTCPIO.Close(1000);
            }
            catch (Exception)
            {
                // nothing to do
            }
            mTCPIO = null;
            mActiveIO = null;

            Invoke((MethodInvoker)delegate
            {
                onTcpDisconnect();  // Invoke on UI thread
            });

        }

        protected void doTestIO()
        {
            // Ensure the receive buffer is flushed
            doFlush();

            // Send the get serial # command
            OBPGetSerialNumber serialNum = new OBPGetSerialNumber(mActiveIO, mRequest, mResponse);
            sendMessage(serialNum);

            Invoke((MethodInvoker)delegate
            {
                onTestIOComplete(serialNum); // invoke on UI thread
            });
        }

        protected void clearMessageStatus()
        {
            labelIOStatus.Text = "---";
            labelIOStatus.ForeColor = NORMAL_COLOR;

            labelOBPStatus.Text = "---";
            labelOBPStatus.ForeColor = NORMAL_COLOR;
        }

        protected void updateMessageStatus(OBPMessage obpMessage)
        {
            if (mActiveButton != null)
            {
                mActiveButton.Enabled = true;
                mActiveButton = null;
            }

            // Update the IO Status
            if (obpMessage.SendSuccess)
            {
                labelIOStatus.Text = "Success";
                labelIOStatus.ForeColor = SUCCESS_COLOR;
            }
            else
            {
                if (obpMessage.Request.IOStatus != SendReceiveStatus.IO_SUCCESS)
                {
                    setIOStatus("Request", obpMessage.Request.IOStatus);
                }
                else
                {
                    setIOStatus("Response", obpMessage.Response.IOStatus);
                }
            }

            // Update the OBP Response Status
            if (obpMessage.Response.IOStatus == SendReceiveStatus.IO_SUCCESS)
            {
                if (obpMessage.Response.IsError)
                {
                    labelOBPStatus.Text = obpMessage.Response.GetErrorString();
                    labelOBPStatus.ForeColor = FAIL_COLOR;
                }
                else
                {
                    labelOBPStatus.Text = "Success";
                    labelOBPStatus.ForeColor = SUCCESS_COLOR;
                }
            }
            else
            {
                labelOBPStatus.Text = "n/a";
                labelOBPStatus.ForeColor = NORMAL_COLOR;
            }

            textBoxIOLog.AppendText("\r\nRequest:\r\n");
            textBoxIOLog.AppendText(hexDump(obpMessage.Request.IOBuffer, 0, obpMessage.Request.MessageSize));
            if (obpMessage.Response.IOStatus != SendReceiveStatus.IO_PENDING)
            {
                textBoxIOLog.AppendText("\r\nResponse:\r\n");
                textBoxIOLog.AppendText(hexDump(obpMessage.Response.IOBuffer, 0, obpMessage.Response.MessageSize));

                if (checkBoxMD5Checksum.Checked)
                {
                    if (obpMessage.Response.IsChecksumMatch())
                    {
                        checkBoxMD5Checksum.ForeColor = SUCCESS_COLOR;
                    }
                    else
                    {
                        checkBoxMD5Checksum.ForeColor = FAIL_COLOR;
                    }
                }
                else
                {
                    checkBoxMD5Checksum.ForeColor = NORMAL_COLOR;
                }
            }
            else
            {
                checkBoxMD5Checksum.ForeColor = NORMAL_COLOR;
            }
        }

        protected void setIOStatus(string ioType, SendReceiveStatus ioStatus)
        {
            switch (ioStatus)
            {
                case SendReceiveStatus.IO_SUCCESS:
                    labelIOStatus.Text = ioType + ": Success";
                    labelIOStatus.ForeColor = SUCCESS_COLOR;
                    break;

                case SendReceiveStatus.IO_PENDING:
                    labelIOStatus.Text = ioType + ": Did not send";
                    labelIOStatus.ForeColor = NORMAL_COLOR;
                    break;

                case SendReceiveStatus.IO_PORT_ERROR:
                    labelIOStatus.Text = ioType + ": RS232 Port Error";
                    labelIOStatus.ForeColor = FAIL_COLOR;
                    break;

                case SendReceiveStatus.IO_NULL_BUFFER:
                    labelIOStatus.Text = ioType + ": NULL Buffer Error";
                    labelIOStatus.ForeColor = FAIL_COLOR;
                    break;

                case SendReceiveStatus.IO_OUT_OF_RANGE:
                    labelIOStatus.Text = ioType + ": Buffer Size Error (too small)";
                    labelIOStatus.ForeColor = FAIL_COLOR;
                    break;

                case SendReceiveStatus.IO_TIMEOUT:
                    labelIOStatus.Text = ioType + ": IO Timeout";
                    labelIOStatus.ForeColor = FAIL_COLOR;
                    break;

                case SendReceiveStatus.IO_INSUFFICIENT_DATA:
                    labelIOStatus.Text = ioType + ": Insufficient Data (read or written)";
                    labelIOStatus.ForeColor = FAIL_COLOR;
                    break;

                case SendReceiveStatus.IO_PIPE_ERROR:
                    labelIOStatus.Text = ioType + ": winUSBNet Error (could not read or write pipe)";
                    labelIOStatus.ForeColor = FAIL_COLOR;
                    break;

                default:
                    labelIOStatus.Text = ioType + ": Unknown Error";
                    labelIOStatus.ForeColor = FAIL_COLOR;
                    break;
            }

        }

        // Attemptsto flush the receive buffer
        protected void doFlush()
        {
            byte[] data = new byte[64];
            SendReceiveStatus ioStatus;
            mActiveIO.setReceiveTimeout(1000);
            int count = 0;

            if(mActiveIO == mRS232IO)
            {
                do
                {
                    ioStatus = mActiveIO.receiveBytes(data, 0, data.Length);
                    count++;
                } while (ioStatus == SendReceiveStatus.IO_SUCCESS && count < 100);
            }
            else if(mActiveIO == mTCPIO)
            {
                //TODO: fix the tcp flush
                do
                {
                    ioStatus = mActiveIO.receiveBytes(data, 0, data.Length);
                    count++;
                } while (ioStatus == SendReceiveStatus.IO_SUCCESS && count < 100);
            }
            else if(mActiveIO == mUSBIO)
            {
                mUSBIO.Interfaces[0].InPipe.Flush();
            }


            mActiveIO.setReceiveTimeout(mReceiveTimeoutMS);
        }

        private void doRS232SyncFW(uint baudRate, Handshake handshaking)
        {
            bool bBaudrateSuccess = true;
            bool bFlowControlSuccess = true;

            if (mRS232IO.BaudRate != baudRate)
            {
                OBPSetBaudRate setBaudRate = new OBPSetBaudRate(mRS232IO, mRequest, mResponse);
                setBaudRate.BaudRate = (uint)baudRate;

                bBaudrateSuccess = sendMessage(setBaudRate);

                // If no error occurred... update the baud rate
                if (bBaudrateSuccess && !setBaudRate.Response.IsError)
                {
                    try
                    {
                        mRS232IO.BaudRate = (int)baudRate;
                    }
                    catch (Exception)
                    {
                        bBaudrateSuccess = false;
                    }

                    if (bBaudrateSuccess)
                    {
                        // Delay to allow the change to take effect
                        Thread.Sleep(3000);
                    }
                }
                else
                {
                    bBaudrateSuccess = false;
                }

            }

            if (mRS232IO.Handshake != handshaking)
            {
                OBPSetFlowControl setFlowControl = new OBPSetFlowControl(mRS232IO, mRequest, mResponse);
                if (handshaking == Handshake.RequestToSend || handshaking == Handshake.RequestToSendXOnXOff)
                {
                    setFlowControl.FlowControl = 1;
                }
                else
                {
                    setFlowControl.FlowControl = 0;
                }

                bFlowControlSuccess = sendMessage(setFlowControl);

                // If no error occurred... update the handshaking
                if (bFlowControlSuccess && !setFlowControl.Response.IsError)
                {
                    try
                    {
                        mRS232IO.Handshake = handshaking;
                    }
                    catch (Exception)
                    {
                        bFlowControlSuccess = false;
                    }

                    // Delay to allow the change to take effect
                    Thread.Sleep(3000);
                }
                else
                {
                    bFlowControlSuccess = false;
                }

            }

            Invoke((MethodInvoker)delegate
            {
                onRS232SyncFWComplete(bBaudrateSuccess && bFlowControlSuccess); // invoke on UI thread
            });
        }

        private bool sendMessage(OBPMessage msg)
        {
            msg.Request.AckRequest = checkBoxAckAlways.Checked;
            msg.Request.ProtocolVersion = mProtocolVersion;
            msg.Request.ChecksumType = checkBoxMD5Checksum.Checked ? (byte)1 : (byte)0;
            msg.Request.Regarding = mRegarding;

            bool bSuccess = msg.Send();

            Invoke((MethodInvoker)delegate
            {
                onMessageSent(msg); // invoke on UI thread
            });

            return bSuccess;
        }

        private string hexDump(byte[] data, int offset, int length)
        {
            if (data == null || offset < 0 || length < 0 || offset >= data.Length)
            {
                return "";
            }

            StringBuilder sb = new StringBuilder();

            int iStart = offset;
            int asciiOffset = offset;
            int iEnd = iStart + length;
            if (iEnd > data.Length)
            {
                iEnd = data.Length;
            }
            int lineEnd;

            do
            {
                sb.Append((offset - iStart).ToString("X4")).Append(": ");

                lineEnd = offset + 16;
                if (lineEnd > iEnd)
                {
                    lineEnd = iEnd;
                }

                do
                {
                    sb.Append(data[offset++].ToString("X2")).Append(" ");
                } while (offset < lineEnd);

                sb.Append("    ");

                do
                {
                    char c = (char) data[asciiOffset++];
                    if (c < 32 || c > 126) { c = '.';  }
                    sb.Append(c);
                } while (asciiOffset < lineEnd);

                sb.Append("\r\n");

            } while (offset < iEnd);

            return sb.ToString();
        }

        private string hexDump(byte[] data)
        {
            return hexDump(data, 0, data.Length);
        }


        //----- Non-Control Event Handlers -----

        public void onTestIOComplete(OBPGetSerialNumber serialNum)
        {
            buttonTest.Enabled = true;

            if (serialNum.IsSuccess)
            {
                setTestSuccess();
            }
        }

        private void onMessageSent(OBPMessage msg)
        {
            updateMessageStatus(msg);
        }

        private void onRS232SyncFWComplete(bool bSuccess)
        {
            doTest();
        }

        protected void onTcpConnectFailed()
        {
            buttonTest.Enabled = true;
            buttonDisconnect.Enabled = false;

            setTestFailed("Failed to connect to specified IP address and port #");

            labelPortNum.Text = "---";
            labelIPAddress.Text = "---";
        }

        protected void onTcpConnectSuccess()
        {
            buttonDisconnect.Enabled = true;

            labelPortNum.Text = mTCPIO.PortNum.ToString();
            labelIPAddress.Text = mTCPIO.ipAddress.ToString();
        }

        protected void onTcpDisconnect()
        {
            buttonTest.Enabled = true;
            buttonDisconnect.Enabled = false;

            labelPortNum.Text = "---";
            labelIPAddress.Text = "---";
        }


        //----- UI Control Event Handlers -----

        private void buttonTest_Click(object sender, EventArgs e)
        {
            labelIOStatus.Text = "";
            labelOBPStatus.Text = "";
            doTest();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (mRS232IO != null)
            {
                // Close the port
                mRS232IO.Close();
                mRS232IO = null;
            }

            if (mTCPIO != null)
            {
                // Close the socket
                mTCPIO.Close();
                mTCPIO = null;
            }

            if(mUSBIO != null)
            {
                // mUSBIO.Close();
                mUSBIO = null;
            }
        }

        private void comboBoxComPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            setTestRequired();
        }

        private void comboBoxBaudRate_SelectedIndexChanged(object sender, EventArgs e)
        {
            setFwSyncRequired(true);
        }

        private void comboBoxDataBits_SelectedIndexChanged(object sender, EventArgs e)
        {
            setFwSyncRequired(true);
        }

        private void comboBoxParity_SelectedIndexChanged(object sender, EventArgs e)
        {
            setFwSyncRequired(true);
        }

        private void comboBoxStopBits_SelectedIndexChanged(object sender, EventArgs e)
        {
            setFwSyncRequired(true);
        }

        private void comboBoxHandshaking_SelectedIndexChanged(object sender, EventArgs e)
        {
            setFwSyncRequired(true);
        }

        private void buttonSyncFW_Click(object sender, EventArgs e)
        {
            buttonTest.Enabled = false;
            buttonSyncFW.Enabled = false;

            clearMessageStatus();

            uint baudRate = UInt32.Parse(BAUDRATE_STRINGS[comboBoxBaudRate.SelectedIndex]);
            Handshake handshaking = (Handshake)comboBoxHandshaking.SelectedIndex;

            Thread syncThread = new Thread(() => doRS232SyncFW(baudRate, handshaking));
            syncThread.Start();
        }

        private void textBoxProtocolVersion_TextChanged(object sender, EventArgs e)
        {
            try
            {
                mProtocolVersion = ushort.Parse(textBoxProtocolVersion.Text.Replace("0x",""), NumberStyles.HexNumber);
            }
            catch (Exception)
            {
                // Nothing to do
            }
        }

        private void buttonClearLog_Click(object sender, EventArgs e)
        {
            textBoxIOLog.Clear();
        }

        private void textBoxRegarding_TextChanged(object sender, EventArgs e)
        {
            try
            {
                mRegarding = uint.Parse(textBoxRegarding.Text.Replace("0x", ""), NumberStyles.HexNumber);
            }
            catch (Exception)
            {
                // Nothing to do
            }
        }

        private void tabControlProtocol_SelectedIndexChanged(object sender, EventArgs e)
        {
            tabControlContent.Enabled = false;
            if (tabControlProtocol.SelectedIndex == TAB_RS232)
            {
                mActiveIO = mRS232IO;
                buttonSyncFW.Enabled = mFwSyncRequired;
            }
            else if (tabControlProtocol.SelectedIndex == TAB_TCPIP)
            {
                mActiveIO = mTCPIO;
                buttonSyncFW.Enabled = false;
            }
            else if(tabControlProtocol.SelectedIndex == TAB_USB)
            {
                mActiveIO = mUSBIO; ;
                buttonSyncFW.Enabled = false;
                InitializeUSBTab();
            }
            else
            {
                MessageBox.Show("Programming Error: This tab was not represented in tabControlProtocol_SelectedIndexChanged().");
            }
        }

        private void buttonDisconnect_Click(object sender, EventArgs e)
        {
            buttonDisconnect.Enabled = false;

            Thread tcpDisconnectThread = new Thread(doTcpDisconnect);
            tcpDisconnectThread.Start();
        }

        private bool okToSend()
        {
            bool bOk = (mActiveButton == null && mActiveIO != null);

            if (mActiveIO == null)
            {
                labelIOStatus.Text = "IO Port Not Successfully Opened";
                labelIOStatus.ForeColor = FAIL_COLOR;

                labelOBPStatus.Text = "n/a";
                labelOBPStatus.ForeColor = NORMAL_COLOR;
            }
            else {
                clearMessageStatus();
            }

            return bOk;
        }


        //---------- IO Button Handlers, IO Threads, and Completion Methods ----------


        //--- ReadSerialNumLen ---

        private void buttonReadSerialNumLen_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadSerialNumLen;
            mActiveButton.Enabled = false;

            textBoxSerialNumLen.Text = "";

            Thread ioThread = new Thread(doReadSerialNumLen);
            ioThread.Start();
        }

        private void doReadSerialNumLen()
        {
            // Send the command
            OBPGetSerialNumberLen serialNumLen = new OBPGetSerialNumberLen(mActiveIO, mRequest, mResponse);
            sendMessage(serialNumLen);

            Invoke((MethodInvoker)delegate
            {
                onReadSerialNumLenComplete(serialNumLen); // invoke on UI thread
            });
        }

        private void onReadSerialNumLenComplete(OBPGetSerialNumberLen serialNumLen)
        {
            textBoxSerialNumLen.Text = serialNumLen.SerialNumberLen.ToString();
        }


        //--- ReadSerialNum ---

        private void buttonReadSerialNum_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return;  }

            mActiveButton = buttonReadSerialNum;
            mActiveButton.Enabled = false;

            textBoxSerialNum.Text = "";

            Thread ioThread = new Thread(doReadSerialNum);
            ioThread.Start();
        }

        private void doReadSerialNum()
        {
            // Send the command
            OBPGetSerialNumber serialNum = new OBPGetSerialNumber(mActiveIO, mRequest, mResponse);
            sendMessage(serialNum);

            Invoke((MethodInvoker)delegate
            {
                onReadSerialNumComplete(serialNum); // invoke on UI thread
            });
        }

        private void onReadSerialNumComplete(OBPGetSerialNumber serialNum)
        {
            textBoxSerialNum.Text = serialNum.SerialNum;
        }


        //--- ReadHwRevision ---

        private void buttonReadHWRevision_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadHWRevision;
            mActiveButton.Enabled = false;

            textBoxHwRevision.Text = "";

            Thread ioThread = new Thread(doReadHwRevision);
            ioThread.Start();
        }

        private void doReadHwRevision()
        {
            // Send the command
            OBPGetHWRevision hwRevision = new OBPGetHWRevision(mActiveIO, mRequest, mResponse);
            sendMessage(hwRevision);

            Invoke((MethodInvoker)delegate
            {
                onReadHwRevision(hwRevision); // invoke on UI thread
            });
        }

        private void onReadHwRevision(OBPGetHWRevision hwRevision)
        {
            textBoxHwRevision.Text = hwRevision.HwRevision.ToString();
        }


        //--- ReadFwRevision ---

        private void buttonReadFWRevision_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadFWRevision;
            mActiveButton.Enabled = false;

            textBoxFwRevision.Text = "";

            Thread ioThread = new Thread(doReadFwRevision);
            ioThread.Start();
        }

        private void doReadFwRevision()
        {
            // Send the command
            OBPGetFWRevision fwRevision = new OBPGetFWRevision(mActiveIO, mRequest, mResponse);
            sendMessage(fwRevision);

            Invoke((MethodInvoker)delegate
            {
                onReadFwRevision(fwRevision); // invoke on UI thread
            });
        }

        private void onReadFwRevision(OBPGetFWRevision fwRevision)
        {
            textBoxFwRevision.Text = fwRevision.FwRevision.ToString("X4");
        }


        //--- ReadFPGARevision ---

        private void buttonReadFPGARevision_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadFPGARevision;
            mActiveButton.Enabled = false;

            textBoxFPGARevision.Text = "";

            Thread ioThread = new Thread(doReadPGARevision);
            ioThread.Start();
        }

        private void doReadPGARevision()
        {
            // Send the command
            OBPGetFPGARevision fpgaRevision = new OBPGetFPGARevision(mActiveIO, mRequest, mResponse);
            sendMessage(fpgaRevision);

            Invoke((MethodInvoker)delegate
            {
                onReadFPGARevision(fpgaRevision); // invoke on UI thread
            });
        }

        private void onReadFPGARevision(OBPGetFPGARevision fpgaRevision)
        {
            textBoxFPGARevision.Text = fpgaRevision.FPGARevision.ToString("X4");
        }


        //--- ReadDeviceAliasLen ---

        private void buttonReadAliasLen_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadAliasLen;
            mActiveButton.Enabled = false;

            textBoxDeviceAliasLen.Text = "";

            Thread ioThread = new Thread(doReadDeviceAliasLen);
            ioThread.Start();
        }

        private void doReadDeviceAliasLen()
        {
            // Send the command
            OBPGetDeviceAliasLen deviceAliasLen = new OBPGetDeviceAliasLen(mActiveIO, mRequest, mResponse);
            sendMessage(deviceAliasLen);

            Invoke((MethodInvoker)delegate
            {
                onReadDeviceAliasLen(deviceAliasLen); // invoke on UI thread
            });
        }

        private void onReadDeviceAliasLen(OBPGetDeviceAliasLen deviceAliasLen)
        {
            textBoxDeviceAliasLen.Text = deviceAliasLen.DeviceAliasLen.ToString();
        }


        //--- ReadDeviceAlias ---

        private void buttonReadDeviceAlias_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadAliasLen;
            mActiveButton.Enabled = false;

            textBoxDeviceAlias.Text = "";

            Thread ioThread = new Thread(doReadDeviceAlias);
            ioThread.Start();
        }

        private void doReadDeviceAlias()
        {
            // Send the command
            OBPGetDeviceAlias deviceAlias = new OBPGetDeviceAlias(mActiveIO, mRequest, mResponse);
            sendMessage(deviceAlias);

            Invoke((MethodInvoker)delegate
            {
                onReadDeviceAlias(deviceAlias); // invoke on UI thread
            });
        }

        private void onReadDeviceAlias(OBPGetDeviceAlias deviceAlias)
        {
            textBoxDeviceAlias.Text = deviceAlias.DeviceAlias;
        }


        //--- WriteDeviceAlias ---

        private void buttonWriteDeviceAlias_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadAliasLen;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteDeviceAlias(textBoxDeviceAlias.Text));
            ioThread.Start();
        }

        private void doWriteDeviceAlias(string aliasStr)
        {
            // Send the command
            OBPSetDeviceAlias deviceAlias = new OBPSetDeviceAlias(mActiveIO, mRequest, mResponse);
            deviceAlias.DeviceAlias = aliasStr;
            sendMessage(deviceAlias);

            Invoke((MethodInvoker)delegate
            {
                onWriteDeviceAlias(deviceAlias); // invoke on UI thread
            });
        }

        private void onWriteDeviceAlias(OBPSetDeviceAlias deviceAlias)
        {
            // nothing to do
        }


        //--- ReadNumUserStrings ---

        private void buttonReadNumUserStrings_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadNumUserStrings;
            mActiveButton.Enabled = false;

            textBoxNumUserStrings.Text = "";

            Thread ioThread = new Thread(doReadNumUserStrings);
            ioThread.Start();
        }

        private void doReadNumUserStrings()
        {
            // Send the command
            OBPGetNumUserStrings numUserStrings = new OBPGetNumUserStrings(mActiveIO, mRequest, mResponse);
            sendMessage(numUserStrings);

            Invoke((MethodInvoker)delegate
            {
                onReadNumUserStrings(numUserStrings); // invoke on UI thread
            });
        }

        private void onReadNumUserStrings(OBPGetNumUserStrings numUserStrings)
        {
            if (numUserStrings.NumUserStrings > 0)
            {
                buttonReadUserString.Enabled = true;
                buttonWriteUserString.Enabled = true;
                comboBoxUserStringNum.Enabled = true;
                textBoxUserString.Enabled = true;

                mUserStringIndexes = new string[numUserStrings.NumUserStrings];
                for (int i = 0; i < mUserStringIndexes.Length; i++)
                {
                    mUserStringIndexes[i] = i.ToString();
                }
                comboBoxUserStringNum.DataSource = mUserStringIndexes;
            }
            else
            {
                buttonReadUserString.Enabled = false;
                buttonWriteUserString.Enabled = false;
                comboBoxUserStringNum.Enabled = false;
                textBoxUserString.Enabled = false;

            }

            textBoxNumUserStrings.Text = numUserStrings.NumUserStrings.ToString();
        }

        //--- ReadUserStringLen ---

        private void buttonReadUserStringLen_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadUserStringLen;
            mActiveButton.Enabled = false;

            textBoxUserStringLen.Text = "";

            Thread ioThread = new Thread(doReadUserStringLen);
            ioThread.Start();
        }

        private void doReadUserStringLen()
        {
            // Send the command
            OBPGetUserStringLen userStringLen = new OBPGetUserStringLen(mActiveIO, mRequest, mResponse);
            sendMessage(userStringLen);

            Invoke((MethodInvoker)delegate
            {
                onReadUserStringLen(userStringLen); // invoke on UI thread
            });
        }

        private void onReadUserStringLen(OBPGetUserStringLen userStringLen)
        {
            textBoxUserStringLen.Text = userStringLen.UserStringLen.ToString();
        }

        //--- ReadUserString ---

        private void buttonReadUserString_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadUserString;
            mActiveButton.Enabled = false;

            textBoxUserString.Text = "";
            byte userStringIndex = (byte)comboBoxUserStringNum.SelectedIndex;

            Thread ioThread = new Thread(() => doReadUserString(userStringIndex));
            ioThread.Start();
        }

        private void doReadUserString(byte userStringIndex)
        {
            // Send the command
            OBPGetUserString userString = new OBPGetUserString(mActiveIO, mRequest, mResponse);
            userString.UserStringIndex = userStringIndex;
            sendMessage(userString);

            Invoke((MethodInvoker)delegate
            {
                onReadUserString(userString); // invoke on UI thread
            });
        }

        private void onReadUserString(OBPGetUserString userString)
        {
            textBoxUserString.Text = userString.UserString;
        }


        //--- WriteUserString ---

        private void buttonWriteUserString_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonWriteUserString;
            mActiveButton.Enabled = false;

            byte userStringIndex = (byte)comboBoxUserStringNum.SelectedIndex;
            string userStr = textBoxUserString.Text;

            Thread ioThread = new Thread(() => doWriteUserString(userStringIndex, userStr)); ;
            ioThread.Start();
        }

        private void doWriteUserString(byte userStringIndex, string userStr)
        {
            // Send the command
            OBPSetUserString userString = new OBPSetUserString(mActiveIO, mRequest, mResponse);
            userString.UserStringIndex = userStringIndex;
            userString.UserString = userStr;
            sendMessage(userString);

            Invoke((MethodInvoker)delegate
            {
                onWriteUserString(userString); // invoke on UI thread
            });
        }

        private void onWriteUserString(OBPSetUserString userString)
        {
            // nothing to do
        }


        //--- WriteStatusLED ---

        private void buttonWriteStatusLED_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte ledIndex = 0;
            byte ledStatus = 0;
            try
            {
                ledStatus = byte.Parse(textBoxStatusLED.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid status LED value (0, 1, 2)");
                return;
            }

            mActiveButton = buttonWriteUserString;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteStatusLED(ledIndex, ledStatus)); ;
            ioThread.Start();
        }

        private void doWriteStatusLED(byte ledIndex, byte ledStatus)
        {
            // Send the command
            OBPSetStatusLED statusLED = new OBPSetStatusLED(mActiveIO, mRequest, mResponse);
            statusLED.LedIndex = ledIndex;
            statusLED.LedStatus = ledStatus;
            sendMessage(statusLED);

            Invoke((MethodInvoker)delegate
            {
                onWriteStatusLED(statusLED); // invoke on UI thread
            });
        }

        private void onWriteStatusLED(OBPSetStatusLED statusLED)
        {
            // nothing to do
        }


        //--- ResetSpectrometer ---

        private void buttonResetSpectrometer_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonResetSpectrometer;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(doResetSpectrometer); ;
            ioThread.Start();
        }

        private void doResetSpectrometer()
        {
            // Send the command
            OBPResetSpectrometer resetSpectrometer = new OBPResetSpectrometer(mActiveIO, mRequest, mResponse);
            sendMessage(resetSpectrometer);

            Invoke((MethodInvoker)delegate
            {
                onResetSpectrometer(resetSpectrometer); // invoke on UI thread
            });
        }

        private void onResetSpectrometer(OBPResetSpectrometer resetSpectrometer)
        {
            // nothing to do
        }


        //--- ResetDefaults ---

        private void buttonResetDefaults_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonResetDefaults;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(doResetDefaults); ;
            ioThread.Start();
        }

        private void doResetDefaults()
        {
            // Send the command
            OBPResetDefaults resetDefaults = new OBPResetDefaults(mActiveIO, mRequest, mResponse);
            sendMessage(resetDefaults);

            Invoke((MethodInvoker)delegate
            {
                onResetDefaults(resetDefaults); // invoke on UI thread
            });
        }

        private void onResetDefaults(OBPResetDefaults resetDefaults)
        {
            // nothing to do
        }


        //--- SaveRS232Settings ---

        private void buttonSaveRS232Settings_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonResetDefaults;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(doSaveRS232Settings); ;
            ioThread.Start();
        }

        private void doSaveRS232Settings()
        {
            // Send the command
            OBPSaveRS232Settings saveR232 = new OBPSaveRS232Settings(mActiveIO, mRequest, mResponse);
            sendMessage(saveR232);

            Invoke((MethodInvoker)delegate
            {
                onSaveRS232Settings(saveR232); // invoke on UI thread
            });
        }

        private void onSaveRS232Settings(OBPSaveRS232Settings saveR232)
        {
            // nothing to do
        }


        //--- ReadBenchId ---

        private void buttonReadBenchId_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonResetDefaults;
            mActiveButton.Enabled = false;

            textBoxBenchId.Text = "";

            Thread ioThread = new Thread(doReadBenchId); ;
            ioThread.Start();
        }

        private void doReadBenchId()
        {
            // Send the command
            OBPGetBenchID benchId = new OBPGetBenchID(mActiveIO, mRequest, mResponse);
            sendMessage(benchId);

            Invoke((MethodInvoker)delegate
            {
                onReadBenchId(benchId); // invoke on UI thread
            });
        }

        private void onReadBenchId(OBPGetBenchID benchId)
        {
            textBoxBenchId.Text = benchId.BenchID;
        }


        //--- ReadBenchSerialNum ---

        private void buttonReadBenchSerialNum_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadBenchSerialNum;
            mActiveButton.Enabled = false;

            textBoxBenchSerialNum.Text = "";

            Thread ioThread = new Thread(doReadBenchSerialNum); ;
            ioThread.Start();
        }

        private void doReadBenchSerialNum()
        {
            // Send the command
            OBPGetBenchSerialNum benchSerialNum = new OBPGetBenchSerialNum(mActiveIO, mRequest, mResponse);
            sendMessage(benchSerialNum);

            Invoke((MethodInvoker)delegate
            {
                onReadBenchSerialNum(benchSerialNum); // invoke on UI thread
            });
        }

        private void onReadBenchSerialNum(OBPGetBenchSerialNum benchSerialNum)
        {
            textBoxBenchSerialNum.Text = benchSerialNum.BenchSerialNum;
        }


        //--- ReadBenchSlitWidth ---

        private void buttonReadSlitWidth_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadSlitWidth;
            mActiveButton.Enabled = false;

            textBoxSlitWidth.Text = "";

            Thread ioThread = new Thread(doReadBenchSlitWidth); ;
            ioThread.Start();
        }

        private void doReadBenchSlitWidth()
        {
            // Send the command
            OBPGetBenchSlitWidth benchSlitWidth = new OBPGetBenchSlitWidth(mActiveIO, mRequest, mResponse);
            sendMessage(benchSlitWidth);

            Invoke((MethodInvoker)delegate
            {
                onReadBenchSlitWidth(benchSlitWidth); // invoke on UI thread
            });
        }

        private void onReadBenchSlitWidth(OBPGetBenchSlitWidth benchSlitWidth)
        {
            textBoxSlitWidth.Text = benchSlitWidth.BenchSlitWidth.ToString();
        }


        //--- ReadBenchFiberDiameter ---

        private void buttonReadFiberDiameter_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadFiberDiameter;
            mActiveButton.Enabled = false;

            textBoxFiberDiameter.Text = "";

            Thread ioThread = new Thread(doReadBenchFiberDiameter); ;
            ioThread.Start();
        }

        private void doReadBenchFiberDiameter()
        {
            // Send the command
            OBPGetBenchFiberDiameter benchFiberDiameter = new OBPGetBenchFiberDiameter(mActiveIO, mRequest, mResponse);
            sendMessage(benchFiberDiameter);

            Invoke((MethodInvoker)delegate
            {
                onReadBenchFiberDiameter(benchFiberDiameter); // invoke on UI thread
            });
        }

        private void onReadBenchFiberDiameter(OBPGetBenchFiberDiameter benchFiberDiameter)
        {
            textBoxFiberDiameter.Text = benchFiberDiameter.BenchFiberDiameter.ToString();
        }


        //--- ReadBenchGrating ---

        private void buttonReadGrating_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadGrating;
            mActiveButton.Enabled = false;

            textBoxGrating.Text = "";

            Thread ioThread = new Thread(doReadBenchGrating); ;
            ioThread.Start();
        }

        private void doReadBenchGrating()
        {
            // Send the command
            OBPGetBenchGrating benchGrating = new OBPGetBenchGrating(mActiveIO, mRequest, mResponse);
            sendMessage(benchGrating);

            Invoke((MethodInvoker)delegate
            {
                onReadBenchGrating(benchGrating); // invoke on UI thread
            });
        }

        private void onReadBenchGrating(OBPGetBenchGrating benchGrating)
        {
            textBoxGrating.Text = benchGrating.BenchGrating;
        }


        //--- ReadBenchFilter ---

        private void buttonReadFilter_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadFilter;
            mActiveButton.Enabled = false;

            textBoxFilter.Text = "";

            Thread ioThread = new Thread(doReadBenchFilter); ;
            ioThread.Start();
        }

        private void doReadBenchFilter()
        {
            // Send the command
            OBPGetBenchFilter benchFilter = new OBPGetBenchFilter(mActiveIO, mRequest, mResponse);
            sendMessage(benchFilter);

            Invoke((MethodInvoker)delegate
            {
                onReadBenchFilter(benchFilter); // invoke on UI thread
            });
        }

        private void onReadBenchFilter(OBPGetBenchFilter benchFilter)
        {
            textBoxFilter.Text = benchFilter.BenchFilter;
        }


        //--- ReadBenchCoating ---

        private void buttonReadCoating_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadCoating;
            mActiveButton.Enabled = false;

            textBoxCoating.Text = "";

            Thread ioThread = new Thread(doReadBenchCoating); ;
            ioThread.Start();
        }

        private void doReadBenchCoating()
        {
            // Send the command
            OBPGetBenchCoating benchCoating = new OBPGetBenchCoating(mActiveIO, mRequest, mResponse);
            sendMessage(benchCoating);

            Invoke((MethodInvoker)delegate
            {
                onReadBenchCoating(benchCoating); // invoke on UI thread
            });
        }

        private void onReadBenchCoating(OBPGetBenchCoating benchCoating)
        {
            textBoxCoating.Text = benchCoating.BenchCoating;
        }

        //--- ReadDetectorSerialNum ---

        private void buttonReadDetectorSerialNum_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadDetectorSerialNum;
            mActiveButton.Enabled = false;

            textBoxDetectorSerialNum.Text = "";

            Thread ioThread = new Thread(doReadDetectorSerialNum); ;
            ioThread.Start();
        }

        private void doReadDetectorSerialNum()
        {
            // Send the command
            OBPGetDetectorSerialNum detectorSerialNum = new OBPGetDetectorSerialNum(mActiveIO, mRequest, mResponse);
            sendMessage(detectorSerialNum);

            Invoke((MethodInvoker)delegate
            {
                onReadDetectorSerialNum(detectorSerialNum); // invoke on UI thread
            });
        }

        private void onReadDetectorSerialNum(OBPGetDetectorSerialNum detectorSerialNum)
        {
            textBoxDetectorSerialNum.Text = detectorSerialNum.DetectorSerialNum;
        }


        //--- ReadIntegrationTime ---

        private void buttonReadIntegTime_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadIntegTime;
            mActiveButton.Enabled = false;

            textBoxIntegTime.Text = "";

            Thread ioThread = new Thread(doReadIntegTime); ;
            ioThread.Start();
        }

        private void doReadIntegTime()
        {
            // Send the command
            OBPGetIntegrationTime integTime = new OBPGetIntegrationTime(mActiveIO, mRequest, mResponse);
            sendMessage(integTime);

            Invoke((MethodInvoker)delegate
            {
                onReadIntegTime(integTime); // invoke on UI thread
            });
        }

        private void onReadIntegTime(OBPGetIntegrationTime integTime)
        {
            textBoxIntegTime.Text = integTime.IntegrationTime.ToString();
        }


        //--- WriteIntegrationTime ---

        private void buttonWriteIntegTime_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            uint integTime = 0;
            try
            {
                integTime = uint.Parse(textBoxIntegTime.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid integration time (must be integer)");
                return;
            }

            mActiveButton = buttonWriteIntegTime;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteIntegTime(integTime));
            ioThread.Start();
        }

        private void doWriteIntegTime(uint integTimeUS)
        {
            // Send the command
            OBPSetIntegrationTime integTime = new OBPSetIntegrationTime(mActiveIO, mRequest, mResponse);
            integTime.IntegrationTime = integTimeUS;
            sendMessage(integTime);

            Invoke((MethodInvoker)delegate
            {
                onWriteIntegTime(integTime); // invoke on UI thread
            });
        }

        private void onWriteIntegTime(OBPSetIntegrationTime integTime)
        {
            // nothing to do
        }


        //--- ReadMaxIntegrationTime ---

        private void buttonReadMaxIntegTime_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadMaxIntegTime;
            mActiveButton.Enabled = false;

            textBoxMaxIntegTime.Text = "";

            Thread ioThread = new Thread(doReadMaxIntegTime); ;
            ioThread.Start();
        }

        private void doReadMaxIntegTime()
        {
            // Send the command
            OBPGetMaxIntegrationTime maxIntegTime = new OBPGetMaxIntegrationTime(mActiveIO, mRequest, mResponse);
            sendMessage(maxIntegTime);

            Invoke((MethodInvoker)delegate
            {
                onReadMaxIntegTime(maxIntegTime); // invoke on UI thread
            });
        }

        private void onReadMaxIntegTime(OBPGetMaxIntegrationTime maxIntegTime)
        {
            textBoxMaxIntegTime.Text = maxIntegTime.MaxIntegrationTime.ToString();
        }


        //--- ReadMinIntegrationTime ---

        private void buttonReadMinIntegTime_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadMinIntegTime;
            mActiveButton.Enabled = false;

            textBoxMinIntegTime.Text = "";

            Thread ioThread = new Thread(doReadMinIntegTime); ;
            ioThread.Start();
        }

        private void doReadMinIntegTime()
        {
            // Send the command
            OBPGetMinIntegrationTime minIntegTime = new OBPGetMinIntegrationTime(mActiveIO, mRequest, mResponse);
            sendMessage(minIntegTime);

            Invoke((MethodInvoker)delegate
            {
                onReadMinIntegTime(minIntegTime); // invoke on UI thread
            });
        }

        private void onReadMinIntegTime(OBPGetMinIntegrationTime minIntegTime)
        {
            textBoxMinIntegTime.Text = minIntegTime.MinIntegrationTime.ToString();
        }


        //--- ReadIntegrationTimeIncrement ---

        private void buttonReadIntegTimeIncrement_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadIntegTimeIncrement;
            mActiveButton.Enabled = false;

            textBoxIntegTimeIncrement.Text = "";

            Thread ioThread = new Thread(doReadIntegTimeIncrement);
            ioThread.Start();
        }

        private void doReadIntegTimeIncrement()
        {
            // Send the command
            OBPGetIntegrationTimeIncrement integTimeIncrement = new OBPGetIntegrationTimeIncrement(mActiveIO, mRequest, mResponse);
            sendMessage(integTimeIncrement);

            Invoke((MethodInvoker)delegate
            {
                onReadMinIntegTime(integTimeIncrement); // invoke on UI thread
            });
        }

        private void onReadMinIntegTime(OBPGetIntegrationTimeIncrement integTimeIncrement)
        {
            textBoxIntegTimeIncrement.Text = integTimeIncrement.IntegrationTimeIncrement.ToString();
        }

        //--- ReadLampEnable ---

        private void buttonReadLampEnable_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadLampEnable;
            mActiveButton.Enabled = false;

            textBoxLampEnable.Text = "";

            Thread ioThread = new Thread(doReadLampEnable);
            ioThread.Start();
        }

        private void doReadLampEnable()
        {
            // Send the command
            OBPGetLampEnable lampEnable = new OBPGetLampEnable(mActiveIO, mRequest, mResponse);
            sendMessage(lampEnable);

            Invoke((MethodInvoker)delegate
            {
                onReadLampEnable(lampEnable); // invoke on UI thread
            });
        }

        private void onReadLampEnable(OBPGetLampEnable lampEnable)
        {
            textBoxLampEnable.Text = lampEnable.LampEnable.ToString();
        }


        //--- WriteLampEnable ---

        private void buttonWriteLampEnable_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte lampEnable = 0;
            try
            {
                lampEnable = byte.Parse(textBoxLampEnable.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid lamp enable (0 or 1)");
                return;
            }

            mActiveButton = buttonWriteLampEnable;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteLampEnable(lampEnable));
            ioThread.Start();
        }

        private void doWriteLampEnable(byte lampEnableVal)
        {
            // Send the command
            OBPSetLampEnable lampEnable = new OBPSetLampEnable(mActiveIO, mRequest, mResponse);
            lampEnable.LampEnable = lampEnableVal;
            sendMessage(lampEnable);

            Invoke((MethodInvoker)delegate
            {
                onWriteLampEnable(lampEnable); // invoke on UI thread
            });
        }

        private void onWriteLampEnable(OBPSetLampEnable lampEnable)
        {
            // nothing to do
        }


        //--- ReadTriggerMode ---

        private void buttonReadTriggerMode_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadTriggerMode;
            mActiveButton.Enabled = false;

            textBoxTriggerMode.Text = "";

            Thread ioThread = new Thread(doReadTriggerMode);
            ioThread.Start();
        }

        private void doReadTriggerMode()
        {
            // Send the command
            OBPGetTriggerMode triggerMode = new OBPGetTriggerMode(mActiveIO, mRequest, mResponse);
            sendMessage(triggerMode);

            Invoke((MethodInvoker)delegate
            {
                onReadTriggerMode(triggerMode); // invoke on UI thread
            });
        }

        private void onReadTriggerMode(OBPGetTriggerMode triggerMode)
        {
            textBoxTriggerMode.Text = triggerMode.TriggerMode.ToString();
        }


        //--- WriteTriggerMode ---

        private void buttonWriteTriggerMode_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte triggerMode = 0;
            try
            {
                triggerMode = byte.Parse(textBoxTriggerMode.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid trigger mode (0-3 typically)");
                return;
            }

            mActiveButton = buttonWriteTriggerMode;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteTriggerMode(triggerMode));
            ioThread.Start();
        }

        private void doWriteTriggerMode(byte triggerModeVal)
        {
            // Send the command
            OBPSetTriggerMode triggerMode = new OBPSetTriggerMode(mActiveIO, mRequest, mResponse);
            triggerMode.TriggerMode = triggerModeVal;
            sendMessage(triggerMode);

            Invoke((MethodInvoker)delegate
            {
                onWriteTriggerMode(triggerMode); // invoke on UI thread
            });
        }

        private void onWriteTriggerMode(OBPSetTriggerMode triggerMode)
        {
            // nothing to do
        }


        //--- ReadAcquisitionDelay ---

        private void buttonReadAcquisitionDelay_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadAcquisitionDelay;
            mActiveButton.Enabled = false;

            textBoxAcquisitionDelay.Text = "";

            Thread ioThread = new Thread(doReadAcquisitionDelay);
            ioThread.Start();
        }

        private void doReadAcquisitionDelay()
        {
            // Send the command
            OBPGetAcquisitionDelay acquisitionDelay = new OBPGetAcquisitionDelay(mActiveIO, mRequest, mResponse);
            sendMessage(acquisitionDelay);

            Invoke((MethodInvoker)delegate
            {
                onReadAcquisitionDelay(acquisitionDelay); // invoke on UI thread
            });
        }

        private void onReadAcquisitionDelay(OBPGetAcquisitionDelay acquisitionDelay)
        {
            textBoxAcquisitionDelay.Text = acquisitionDelay.AcquisitionDelay.ToString();
        }


        //--- WriteAcquistionDelay ---

        private void buttonWriteAcquisitionDelay_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            uint acquisitionDelay = 0;
            try
            {
                acquisitionDelay = uint.Parse(textBoxAcquisitionDelay.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid acquisition delay (unsigned 32 bit integer)");
                return;
            }

            mActiveButton = buttonWriteAcquisitionDelay;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteAcquisitionDelay(acquisitionDelay));
            ioThread.Start();
        }

        private void doWriteAcquisitionDelay(uint acquisitionDelayVal)
        {
            // Send the command
            OBPSetAcquisitionDelay acquisitionDelay = new OBPSetAcquisitionDelay(mActiveIO, mRequest, mResponse);
            acquisitionDelay.AcquisitionDelay = acquisitionDelayVal;
            sendMessage(acquisitionDelay);

            Invoke((MethodInvoker)delegate
            {
                onWriteAcquisitionDelay(acquisitionDelay); // invoke on UI thread
            });
        }

        private void onWriteAcquisitionDelay(OBPSetAcquisitionDelay acquisitionDelay)
        {
            // nothing to do
        }


        //--- ReadMaxAcquisitionDelay ---

        private void buttonReadMaxAcquisitionDelay_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadMaxAcquisitionDelay;
            mActiveButton.Enabled = false;

            textBoxMaxAcquisitionDelay.Text = "";

            Thread ioThread = new Thread(doReadMaxAcquisitionDelay);
            ioThread.Start();
        }

        private void doReadMaxAcquisitionDelay()
        {
            // Send the command
            OBPGetMaxAcquisitionDelay maxAcquisitionDelay = new OBPGetMaxAcquisitionDelay(mActiveIO, mRequest, mResponse);
            sendMessage(maxAcquisitionDelay);

            Invoke((MethodInvoker)delegate
            {
                onReadMaxAcquisitionDelay(maxAcquisitionDelay); // invoke on UI thread
            });
        }

        private void onReadMaxAcquisitionDelay(OBPGetMaxAcquisitionDelay maxAcquisitionDelay)
        {
            textBoxMaxAcquisitionDelay.Text = maxAcquisitionDelay.MaxAcquisitionDelay.ToString();
        }


        //--- ReadMinAcquisitionDelay ---

        private void buttonReadMinAcquisitionDelay_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadMinAcquisitionDelay;
            mActiveButton.Enabled = false;

            textBoxMinAcquisitionDelay.Text = "";

            Thread ioThread = new Thread(doReadMinAcquisitionDelay);
            ioThread.Start();
        }

        private void doReadMinAcquisitionDelay()
        {
            // Send the command
            OBPGetMinAcquisitionDelay minAcquisitionDelay = new OBPGetMinAcquisitionDelay(mActiveIO, mRequest, mResponse);
            sendMessage(minAcquisitionDelay);

            Invoke((MethodInvoker)delegate
            {
                onReadMaxAcquisitionDelay(minAcquisitionDelay); // invoke on UI thread
            });
        }

        private void onReadMaxAcquisitionDelay(OBPGetMinAcquisitionDelay minAcquisitionDelay)
        {
            textBoxMinAcquisitionDelay.Text = minAcquisitionDelay.MinAcquisitionDelay.ToString();
        }


        //--- ReadAcquisitionDelayIncrement ---

        private void buttonReadAcquisitionDelayIncrement_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadAcquisitionDelayIncrement;
            mActiveButton.Enabled = false;

            textBoxAcquisitionDelayIncrement.Text = "";

            Thread ioThread = new Thread(doReadAcquisitionDelayIncrement);
            ioThread.Start();
        }

        private void doReadAcquisitionDelayIncrement()
        {
            // Send the command
            OBPGetAcquisitionDelayIncrement acquisitionDelayIncrement = new OBPGetAcquisitionDelayIncrement(mActiveIO, mRequest, mResponse);
            sendMessage(acquisitionDelayIncrement);

            Invoke((MethodInvoker)delegate
            {
                onReadAcquisitionDelayIncrement(acquisitionDelayIncrement); // invoke on UI thread
            });
        }

        private void onReadAcquisitionDelayIncrement(OBPGetAcquisitionDelayIncrement acquisitionDelayIncrement)
        {
            textBoxAcquisitionDelayIncrement.Text = acquisitionDelayIncrement.AcquisitionDelayIncrement.ToString();
        }


        //--- SimulateTriggerPulse ---

        private void buttonSimTriggerPulse_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonSimTriggerPulse;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(doSimTriggerPulse);
            ioThread.Start();
        }

        private void doSimTriggerPulse()
        {
            // Send the command
            OBPSimulateTriggerPulse simTriggerPulse = new OBPSimulateTriggerPulse(mActiveIO, mRequest, mResponse);

            sendMessage(simTriggerPulse);

            Invoke((MethodInvoker)delegate
            {
                onSimTriggerPulse(simTriggerPulse); // invoke on UI thread
            });
        }

        private void onSimTriggerPulse(OBPSimulateTriggerPulse simTriggerPulse)
        {
            // nothing to do
        }


        //--- ReadScansToAvg ---

        private void buttonReadScansToAvg_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadScansToAvg;
            mActiveButton.Enabled = false;

            textBoxScansToAvg.Text = "";

            Thread ioThread = new Thread(doReadScansToAvg);
            ioThread.Start();
        }

        private void doReadScansToAvg()
        {
            // Send the command
            OBPGetScansToAvg scansToAvg = new OBPGetScansToAvg(mActiveIO, mRequest, mResponse);
            sendMessage(scansToAvg);

            Invoke((MethodInvoker)delegate
            {
                onReadScansToAvg(scansToAvg); // invoke on UI thread
            });
        }

        private void onReadScansToAvg(OBPGetScansToAvg scansToAvg)
        {
            textBoxScansToAvg.Text = scansToAvg.ScansToAvg.ToString();
        }


        //--- WriteScansToAvg ---

        private void buttonWriteScansToAvg_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            ushort scansToAvg = 0;
            try
            {
                scansToAvg = ushort.Parse(textBoxScansToAvg.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid scans to avg value (0-65535)");
                return;
            }

            mActiveButton = buttonWriteScansToAvg;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteScansToAvg(scansToAvg));
            ioThread.Start();
        }

        private void doWriteScansToAvg(ushort scansToAvgVal)
        {
            // Send the command
            OBPSetScansToAvg scansToAvg = new OBPSetScansToAvg(mActiveIO, mRequest, mResponse);
            scansToAvg.ScansToAvg = scansToAvgVal;
            sendMessage(scansToAvg);

            Invoke((MethodInvoker)delegate
            {
                onWriteScansToAvg(scansToAvg); // invoke on UI thread
            });
        }

        private void onWriteScansToAvg(OBPSetScansToAvg scansToAvg)
        {
            // nothing to do
        }


        //--- ReadBoxcarWidth ---

        private void buttonReadBoxcarWidth_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadBoxcarWidth;
            mActiveButton.Enabled = false;

            textBoxBoxcarWidth.Text = "";

            Thread ioThread = new Thread(doReadBoxcarWidth);
            ioThread.Start();
        }

        private void doReadBoxcarWidth()
        {
            // Send the command
            OBPGetBoxcarWidth boxcarWidth = new OBPGetBoxcarWidth(mActiveIO, mRequest, mResponse);
            sendMessage(boxcarWidth);

            Invoke((MethodInvoker)delegate
            {
                onReadScansToAvg(boxcarWidth); // invoke on UI thread
            });
        }

        private void onReadScansToAvg(OBPGetBoxcarWidth boxcarWidth)
        {
            textBoxBoxcarWidth.Text = boxcarWidth.BoxcarWidth.ToString();
        }


        //--- WriteBoxcarWidth ---

        private void buttonWriteBoxcarWidth_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte boxcarWidth = 0;
            try
            {
                boxcarWidth = byte.Parse(textBoxBoxcarWidth.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid boxcar width (0-255)");
                return;
            }

            mActiveButton = buttonWriteBoxcarWidth;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteBoxcarWidth(boxcarWidth));
            ioThread.Start();
        }

        private void doWriteBoxcarWidth(byte boxcarWidthVal)
        {
            // Send the command
            OBPSetBoxcarWidth boxcarWidth = new OBPSetBoxcarWidth(mActiveIO, mRequest, mResponse);
            boxcarWidth.BoxcarWidth = boxcarWidthVal;
            sendMessage(boxcarWidth);

            Invoke((MethodInvoker)delegate
            {
                onWriteBoxcarWidth(boxcarWidth); // invoke on UI thread
            });
        }

        private void onWriteBoxcarWidth(OBPSetBoxcarWidth boxcarWidth)
        {
            // nothing to do
        }


        //--- ReadPixelBinningFactor ---

        private void buttonReadBinningFactor_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadBinningFactor;
            mActiveButton.Enabled = false;

            textBoxBinningFactor.Text = "";

            Thread ioThread = new Thread(doReadBinningFactor);
            ioThread.Start();
        }

        private void doReadBinningFactor()
        {
            // Send the command
            OBPGetPixelBinningFactor binningFactor = new OBPGetPixelBinningFactor(mActiveIO, mRequest, mResponse);
            sendMessage(binningFactor);

            Invoke((MethodInvoker)delegate
            {
                onReadBinningFactor(binningFactor); // invoke on UI thread
            });
        }

        private void onReadBinningFactor(OBPGetPixelBinningFactor binningFactor)
        {
            textBoxBinningFactor.Text = binningFactor.BinningFactor.ToString();
        }


        //--- WritePixelBinningFactor ---

        private void buttonWriteBinningFactor_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte binningFactor = 0;
            try
            {
                binningFactor = byte.Parse(textBoxBinningFactor.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid binning factor (0-255)");
                return;
            }

            mActiveButton = buttonWriteBinningFactor;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteBinningFactor(binningFactor));
            ioThread.Start();
        }

        private void doWriteBinningFactor(byte binningFactorVal)
        {
            // Send the command
            OBPSetPixelBinningFactor binningFactor = new OBPSetPixelBinningFactor(mActiveIO, mRequest, mResponse);
            binningFactor.BinningFactor = binningFactorVal;
            sendMessage(binningFactor);

            Invoke((MethodInvoker)delegate
            {
                onWriteBinningFactor(binningFactor); // invoke on UI thread
            });
        }

        private void onWriteBinningFactor(OBPSetPixelBinningFactor binningFactor)
        {
            // nothing to do
        }


        //--- ReadDefaultPixelBinningFactor ---

        private void buttonReadDefaultBinningFactor_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadDefaultBinningFactor;
            mActiveButton.Enabled = false;

            textBoxDefaultBinningFactor.Text = "";

            Thread ioThread = new Thread(doReadDefaultBinningFactor);
            ioThread.Start();
        }

        private void doReadDefaultBinningFactor()
        {
            // Send the command
            OBPGetDefaultPixelBinningFactor defaultBinningFactor = new OBPGetDefaultPixelBinningFactor(mActiveIO, mRequest, mResponse);
            sendMessage(defaultBinningFactor);

            Invoke((MethodInvoker)delegate
            {
                onReadDefaultBinningFactor(defaultBinningFactor); // invoke on UI thread
            });
        }

        private void onReadDefaultBinningFactor(OBPGetDefaultPixelBinningFactor defaultBinningFactor)
        {
            textBoxDefaultBinningFactor.Text = defaultBinningFactor.DefaultBinningFactor.ToString();
        }


        //--- WriteDefaultPixelBinningFactor ---

        private void buttonWriteDefaultBinningFactor_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte defaultBinningFactor = 0;
            bool resetDefault = checkBoxDefaultBinningFactor.Checked;
            try
            {
                defaultBinningFactor = byte.Parse(textBoxDefaultBinningFactor.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid default binning factor (0-255)");
                return;
            }

            mActiveButton = buttonWriteDefaultBinningFactor;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteDefaultBinningFactor(defaultBinningFactor, resetDefault));
            ioThread.Start();
        }

        private void doWriteDefaultBinningFactor(byte defaultBinningFactorVal, bool resetDefault)
        {
            // Send the command
            OBPSetDefaultPixelBinningFactor defaultBinningFactor = new OBPSetDefaultPixelBinningFactor(mActiveIO, mRequest, mResponse);
            defaultBinningFactor.DefaultBinningFactor = defaultBinningFactorVal;
            defaultBinningFactor.ResetDefault = resetDefault;
            sendMessage(defaultBinningFactor);

            Invoke((MethodInvoker)delegate
            {
                onWriteDefaultBinningFactor(defaultBinningFactor); // invoke on UI thread
            });
        }

        private void onWriteDefaultBinningFactor(OBPSetDefaultPixelBinningFactor defaultBinningFactor)
        {
            // nothing to do
        }


        //--- ReadMaxPixelBinningFactor ---

        private void buttonReadMaxBinningFactor_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadMaxBinningFactor;
            mActiveButton.Enabled = false;

            textBoxMaxBinningFactor.Text = "";

            Thread ioThread = new Thread(doReadMaxBinningFactor);
            ioThread.Start();
        }

        private void doReadMaxBinningFactor()
        {
            // Send the command
            OBPGetMaxPixelBinningFactor maxBinningFactor = new OBPGetMaxPixelBinningFactor(mActiveIO, mRequest, mResponse);
            sendMessage(maxBinningFactor);

            Invoke((MethodInvoker)delegate
            {
                onReadMaxBinningFactor(maxBinningFactor); // invoke on UI thread
            });
        }

        private void onReadMaxBinningFactor(OBPGetMaxPixelBinningFactor maxBinningFactor)
        {
            textBoxMaxBinningFactor.Text = maxBinningFactor.MaxBinningFactor.ToString();
        }


        //--- ReadMaxPixelBinningFactor ---

        private void buttonReadPartialSpectrumMode_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadPartialSpectrumMode;
            mActiveButton.Enabled = false;

            textBoxPartialSpectrumMode.Text = "";

            Thread ioThread = new Thread(doReadPartialSpectrumMode);
            ioThread.Start();
        }

        private void doReadPartialSpectrumMode()
        {
            // Send the command
            OBPGetPartialSpectrumMode partialSpectrumMode = new OBPGetPartialSpectrumMode(mActiveIO, mRequest, mResponse);
            sendMessage(partialSpectrumMode);

            Invoke((MethodInvoker)delegate
            {
                onReadPartialSpectrumMode(partialSpectrumMode); // invoke on UI thread
            });
        }

        private void onReadPartialSpectrumMode(OBPGetPartialSpectrumMode partialSpectrumMode)
        {
            textBoxPartialSpectrumMode.Text = OBPMessage.ToHexByteString(partialSpectrumMode.PartialSpectrumMode);
        }


        //--- WritePartialSpectrumMode ---

        private void buttonWritePartialSpectrumMode_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte[] partialSpectrumMode = null;
            try
            {
                partialSpectrumMode = OBPMessage.FromHexByteString(textBoxPartialSpectrumMode.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid partial spectrum mode (must be space separated hex bytes such as: 01 00 04 00)");
                return;
            }

            mActiveButton = buttonWritePartialSpectrumMode;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWritePartialSpectrumMode(partialSpectrumMode));
            ioThread.Start();
        }

        private void doWritePartialSpectrumMode(byte[] partialSpectrumModeA)
        {
            // Send the command
            OBPSetPartialSpectrumMode partialSpectrumMode = new OBPSetPartialSpectrumMode(mActiveIO, mRequest, mResponse);
            partialSpectrumMode.PartialSpectrumMode = partialSpectrumModeA;
            sendMessage(partialSpectrumMode);

            Invoke((MethodInvoker)delegate
            {
                onWritePartialSpectrumMode(partialSpectrumMode); // invoke on UI thread
            });
        }

        private void onWritePartialSpectrumMode(OBPSetPartialSpectrumMode partialSpectrumMode)
        {
            // nothing to do
        }


        //--- ReadNumWavecalCoeffs ---

        private void buttonReadNumWavecalCoeffs_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadNumWavecalCoeffs;
            mActiveButton.Enabled = false;

            textBoxNumWavecalCoeffs.Text = "";

            Thread ioThread = new Thread(doReadNumWavecalCoeffs);
            ioThread.Start();
        }

        private void doReadNumWavecalCoeffs()
        {
            // Send the command
            OBPGetNumWavecalCoefficients numWavecalCoeffs = new OBPGetNumWavecalCoefficients(mActiveIO, mRequest, mResponse);
            sendMessage(numWavecalCoeffs);

            Invoke((MethodInvoker)delegate
            {
                onReadNumWavecalCoeffs(numWavecalCoeffs); // invoke on UI thread
            });
        }

        private void onReadNumWavecalCoeffs(OBPGetNumWavecalCoefficients numWavecalCoeffs)
        {
            if (numWavecalCoeffs.NumWavecalCoeffs > 0)
            {
                buttonReadWavecalCoeff.Enabled = true;
                buttonWriteWavecalCoeff.Enabled = true;
                comboBoxWavecalCoeffIndex.Enabled = true;
                textBoxWavecalCoeff.Enabled = true;

                mWavecalIndexes = new string[numWavecalCoeffs.NumWavecalCoeffs];
                for (int i = 0; i < mWavecalIndexes.Length; i++)
                {
                    mWavecalIndexes[i] = i.ToString();
                }
                comboBoxWavecalCoeffIndex.DataSource = mWavecalIndexes;
            }
            else
            {
                buttonReadWavecalCoeff.Enabled = false;
                buttonWriteWavecalCoeff.Enabled = false;
                comboBoxWavecalCoeffIndex.Enabled = false;
                textBoxWavecalCoeff.Enabled = false;

            }

            textBoxNumWavecalCoeffs.Text = numWavecalCoeffs.NumWavecalCoeffs.ToString();
        }

        //--- ReadWavecalCoefficient ---

        private void buttonReadWavecalCoeff_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadWavecalCoeff;
            mActiveButton.Enabled = false;

            textBoxWavecalCoeff.Text = "";
            byte coeffIndex = (byte)comboBoxWavecalCoeffIndex.SelectedIndex;

            Thread ioThread = new Thread(() => doReadWavecalCoeff(coeffIndex));
            ioThread.Start();
        }

        private void doReadWavecalCoeff(byte coeffIndex)
        {
            // Send the command
            OBPGetWavecalCoefficient wavecalCoeff = new OBPGetWavecalCoefficient(mActiveIO, mRequest, mResponse);
            wavecalCoeff.WavecalIndex = coeffIndex;
            sendMessage(wavecalCoeff);

            Invoke((MethodInvoker)delegate
            {
                onReadWavecalCoeff(wavecalCoeff); // invoke on UI thread
            });
        }

        private void onReadWavecalCoeff(OBPGetWavecalCoefficient wavecalCoeff)
        {
            textBoxWavecalCoeff.Text = wavecalCoeff.WavecalCoeff.ToString();
        }


        //--- WriteWavecalCoefficient ---

        private void buttonWriteWavecalCoeff_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte coeffIndex = (byte)comboBoxWavecalCoeffIndex.SelectedIndex;
            float wavecalCoeff = 0;

            try
            {
                wavecalCoeff = float.Parse(textBoxWavecalCoeff.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid wavecal coefficient (must be float)");
                return;
            }

            mActiveButton = buttonWriteWavecalCoeff;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteWavecalCoeff(coeffIndex, wavecalCoeff)); ;
            ioThread.Start();
        }

        private void doWriteWavecalCoeff(byte coeffIndex, float coeffVal)
        {
            // Send the command
            OBPSetWavecalCoefficient wavecalCoeff = new OBPSetWavecalCoefficient(mActiveIO, mRequest, mResponse);
            wavecalCoeff.WavecalIndex = coeffIndex;
            wavecalCoeff.WavecalCoeff = coeffVal;
            sendMessage(wavecalCoeff);

            Invoke((MethodInvoker)delegate
            {
                onWriteWavecalCoeff(wavecalCoeff); // invoke on UI thread
            });
        }

        private void onWriteWavecalCoeff(OBPSetWavecalCoefficient wavecalCoeff)
        {
            // nothing to do
        }


        //--- ReadNumNonLinearityCoeffs ---

        private void buttonReadNumNonLinearityCoeffs_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadNumNonLinearityCoeffs;
            mActiveButton.Enabled = false;

            textBoxNumNonLinearityCoeffs.Text = "";

            Thread ioThread = new Thread(doReadNumNonLinearityCoeffs);
            ioThread.Start();
        }

        private void doReadNumNonLinearityCoeffs()
        {
            // Send the command
            OBPGetNumNonLinearityCoefficients numNonLinearityCoeffs = new OBPGetNumNonLinearityCoefficients(mActiveIO, mRequest, mResponse);
            sendMessage(numNonLinearityCoeffs);

            Invoke((MethodInvoker)delegate
            {
                onReadNumNonLinearityCoeffs(numNonLinearityCoeffs); // invoke on UI thread
            });
        }

        private void onReadNumNonLinearityCoeffs(OBPGetNumNonLinearityCoefficients numNonLinearityCoeffs)
        {
            if (numNonLinearityCoeffs.NumNonLinearityCoeffs > 0)
            {
                buttonReadNonLinearityCoeff.Enabled = true;
                buttonWriteNonLinearityCoeff.Enabled = true;
                comboBoxNonLinearityCoeffIndex.Enabled = true;
                textBoxNonLinearityCoeff.Enabled = true;

                mNonLinearityIndexes = new string[numNonLinearityCoeffs.NumNonLinearityCoeffs];
                for (int i = 0; i < mNonLinearityIndexes.Length; i++)
                {
                    mNonLinearityIndexes[i] = i.ToString();
                }
                comboBoxNonLinearityCoeffIndex.DataSource = mNonLinearityIndexes;
            }
            else
            {
                buttonReadNonLinearityCoeff.Enabled = false;
                buttonWriteNonLinearityCoeff.Enabled = false;
                comboBoxNonLinearityCoeffIndex.Enabled = false;
                textBoxNonLinearityCoeff.Enabled = false;

            }

            textBoxNumNonLinearityCoeffs.Text = numNonLinearityCoeffs.NumNonLinearityCoeffs.ToString();
        }

        //--- ReadNonLinearityCoefficient ---

        private void buttonReadNonLinearityCoeff_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadNonLinearityCoeff;
            mActiveButton.Enabled = false;

            textBoxNonLinearityCoeff.Text = "";
            byte coeffIndex = (byte)comboBoxNonLinearityCoeffIndex.SelectedIndex;

            Thread ioThread = new Thread(() => doReadNonLinearityCoeff(coeffIndex));
            ioThread.Start();
        }

        private void doReadNonLinearityCoeff(byte coeffIndex)
        {
            // Send the command
            OBPGetNonLinearityCoefficient nonLinearityCoeff = new OBPGetNonLinearityCoefficient(mActiveIO, mRequest, mResponse);
            nonLinearityCoeff.NonLinearityIndex = coeffIndex;
            sendMessage(nonLinearityCoeff);

            Invoke((MethodInvoker)delegate
            {
                onReadNonLinearityCoeff(nonLinearityCoeff); // invoke on UI thread
            });
        }

        private void onReadNonLinearityCoeff(OBPGetNonLinearityCoefficient nonLinearityCoeff)
        {
            textBoxNonLinearityCoeff.Text = nonLinearityCoeff.NonLinearityCoeff.ToString();
        }


        //--- WriteNonLinearityCoefficient ---

        private void buttonWriteNonLinearityCoeff_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte coeffIndex = (byte)comboBoxNonLinearityCoeffIndex.SelectedIndex;
            float nonLinearityCoeff = 0;

            try
            {
                nonLinearityCoeff = float.Parse(textBoxNonLinearityCoeff.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid non-linearity coefficient (must be float)");
                return;
            }

            mActiveButton = buttonWriteNonLinearityCoeff;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteNonLinearityCoeff(coeffIndex, nonLinearityCoeff)); ;
            ioThread.Start();
        }

        private void doWriteNonLinearityCoeff(byte coeffIndex, float coeffVal)
        {
            // Send the command
            OBPSetNonLinearityCoefficient nonLinearityCoeff = new OBPSetNonLinearityCoefficient(mActiveIO, mRequest, mResponse);
            nonLinearityCoeff.NonLinearityIndex = coeffIndex;
            nonLinearityCoeff.NonLinearityCoeff = coeffVal;
            sendMessage(nonLinearityCoeff);

            Invoke((MethodInvoker)delegate
            {
                onWriteNonLinearityCoeff(nonLinearityCoeff); // invoke on UI thread
            });
        }

        private void onWriteNonLinearityCoeff(OBPSetNonLinearityCoefficient nonLinearityCoeff)
        {
            // nothing to do
        }



        //--- ReadNumStrayLightCoeffs ---

        private void buttonReadNumStrayLightCoeffs_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadNumStrayLightCoeffs;
            mActiveButton.Enabled = false;

            textBoxNumStrayLightCoeffs.Text = "";

            Thread ioThread = new Thread(doReadNumStrayLightCoeffs);
            ioThread.Start();
        }

        private void doReadNumStrayLightCoeffs()
        {
            // Send the command
            OBPGetNumStrayLightCoefficients numStrayLightCoeffs = new OBPGetNumStrayLightCoefficients(mActiveIO, mRequest, mResponse);
            sendMessage(numStrayLightCoeffs);

            Invoke((MethodInvoker)delegate
            {
                onReadNumStrayLightCoeffs(numStrayLightCoeffs); // invoke on UI thread
            });
        }

        private void onReadNumStrayLightCoeffs(OBPGetNumStrayLightCoefficients numStrayLightCoeffs)
        {
            if (numStrayLightCoeffs.NumStrayLightCoeffs > 0)
            {
                buttonReadStrayLightCoeff.Enabled = true;
                buttonWriteStrayLightCoeff.Enabled = true;
                comboBoxStrayLightCoeffIndex.Enabled = true;
                textBoxStrayLightCoeff.Enabled = true;

                mStrayLightIndexes = new string[numStrayLightCoeffs.NumStrayLightCoeffs];
                for (int i = 0; i < mStrayLightIndexes.Length; i++)
                {
                    mStrayLightIndexes[i] = i.ToString();
                }
                comboBoxStrayLightCoeffIndex.DataSource = mStrayLightIndexes;
            }
            else
            {
                buttonReadStrayLightCoeff.Enabled = false;
                buttonWriteStrayLightCoeff.Enabled = false;
                comboBoxStrayLightCoeffIndex.Enabled = false;
                textBoxStrayLightCoeff.Enabled = false;

            }

            textBoxNumStrayLightCoeffs.Text = numStrayLightCoeffs.NumStrayLightCoeffs.ToString();
        }

        //--- ReadStrayLightCoefficient ---

        private void buttonReadStrayLightCoeff_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadStrayLightCoeff;
            mActiveButton.Enabled = false;

            textBoxStrayLightCoeff.Text = "";
            byte coeffIndex = (byte)comboBoxStrayLightCoeffIndex.SelectedIndex;

            Thread ioThread = new Thread(() => doReadStrayLightCoeff(coeffIndex));
            ioThread.Start();
        }

        private void doReadStrayLightCoeff(byte coeffIndex)
        {
            // Send the command
            OBPGetStrayLightCoefficient strayLightCoeff = new OBPGetStrayLightCoefficient(mActiveIO, mRequest, mResponse);
            strayLightCoeff.StrayLightIndex = coeffIndex;
            sendMessage(strayLightCoeff);

            Invoke((MethodInvoker)delegate
            {
                onReadStrayLightCoeff(strayLightCoeff); // invoke on UI thread
            });
        }

        private void onReadStrayLightCoeff(OBPGetStrayLightCoefficient strayLightCoeff)
        {
            textBoxStrayLightCoeff.Text = strayLightCoeff.StrayLightCoeff.ToString();
        }


        //--- WriteStrayLightCoefficient ---

        private void buttonWriteStrayLightCoeff_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte coeffIndex = (byte)comboBoxStrayLightCoeffIndex.SelectedIndex;
            float strayLightCoeff = 0;

            try
            {
                strayLightCoeff = float.Parse(textBoxStrayLightCoeff.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid stray light coefficient (must be float)");
                return;
            }

            mActiveButton = buttonWriteStrayLightCoeff;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteStrayLightCoeff(coeffIndex, strayLightCoeff)); ;
            ioThread.Start();
        }

        private void doWriteStrayLightCoeff(byte coeffIndex, float coeffVal)
        {
            // Send the command
            OBPSetStrayLightCoefficient strayLightCoeff = new OBPSetStrayLightCoefficient(mActiveIO, mRequest, mResponse);
            strayLightCoeff.StrayLightIndex = coeffIndex;
            strayLightCoeff.StrayLightCoeff = coeffVal;
            sendMessage(strayLightCoeff);

            Invoke((MethodInvoker)delegate
            {
                onWriteStrayLightCoeff(strayLightCoeff); // invoke on UI thread
            });
        }

        private void onWriteStrayLightCoeff(OBPSetStrayLightCoefficient strayLightCoeff)
        {
            // nothing to do
        }


        //--- ReadCorrectedSpectrum ---

        private void buttonReadCorrectedSpectrum_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadCorrectedSpectrum;
            mActiveButton.Enabled = false;

            textBoxSpectrumData.Text = "";
            textBoxNumPixelsReturned.Text = "";
            textBoxMaxSpectrumValue.Text = "";
            textBoxMaxValuePixelNum.Text = "";
            textBoxMetadataSpectrumCount.Text = "";
            textBoxMetadataTickCount.Text = "";
            textBoxMetadataTimeUS.Text = "";
            textBoxMetadataTriggerMode.Text = "";

            Thread ioThread = new Thread(doReadCorrectedSpectrum);
            ioThread.Start();
        }

        private void doReadCorrectedSpectrum()
        {
            // Send the command
            OBPGetCorrectedSpectrum correctedSpectrum = new OBPGetCorrectedSpectrum(mActiveIO, mRequest, mResponse);
            sendMessage(correctedSpectrum);

            Invoke((MethodInvoker)delegate
            {
                onReadCorrectedSpectrum(correctedSpectrum); // invoke on UI thread
            });
        }

        private void onReadCorrectedSpectrum(OBPGetCorrectedSpectrum correctedSpectrum)
        {
            Series s = chartSpectrum.Series[0];

            ushort[] yData = correctedSpectrum.CorrectedSpectrum;
            ushort[] xData = correctedSpectrum.Pixels;

            s.Points.DataBindXY(xData, yData);

            //chartSpectrum.ChartAreas[0].AxisY.IsStartedFromZero = true;
            //chartSpectrum.ChartAreas[0].AxisY.Maximum = yMax;

            chartSpectrum.ChartAreas[0].RecalculateAxesScale();

            StringBuilder dataStr = new StringBuilder();
            dataStr.Append("Pixel,CorrectedSpectrum\r\n");
            for (int i = 0; i < xData.Length; i++)
            {
                dataStr.Append(i).Append(',').Append(yData[i]).Append("\r\n");
            }
            textBoxSpectrumData.Text = dataStr.ToString();

            textBoxNumPixelsReturned.Text = xData.Length.ToString();
            textBoxMaxSpectrumValue.Text = correctedSpectrum.MaxValue.ToString();
            textBoxMaxValuePixelNum.Text = correctedSpectrum.MaxValuePixelNum.ToString();
            textBoxMetadataSpectrumCount.Text = "n/a";
            textBoxMetadataTickCount.Text = "n/a";
            textBoxMetadataTimeUS.Text = "n/a";
            textBoxMetadataTriggerMode.Text = "n/a";
        }


        //--- ReadRawSpectrum ---

        private void buttonReadRawSpectrum_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadRawSpectrum;
            mActiveButton.Enabled = false;

            textBoxSpectrumData.Text = "";
            textBoxNumPixelsReturned.Text = "";
            textBoxMaxSpectrumValue.Text = "";
            textBoxMaxValuePixelNum.Text = "";
            textBoxMetadataSpectrumCount.Text = "";
            textBoxMetadataTickCount.Text = "";
            textBoxMetadataTimeUS.Text = "";
            textBoxMetadataTriggerMode.Text = "";

            Thread ioThread = new Thread(doReadRawSpectrum);
            ioThread.Start();
        }

        private void doReadRawSpectrum()
        {
            // Send the command
            OBPGetRawSpectrum rawSpectrum = new OBPGetRawSpectrum(mActiveIO, mRequest, mResponse);
            sendMessage(rawSpectrum);

            Invoke((MethodInvoker)delegate
            {
                onReadRawSpectrum(rawSpectrum); // invoke on UI thread
            });
        }

        private void onReadRawSpectrum(OBPGetRawSpectrum rawSpectrum)
        {
            Series s = chartSpectrum.Series[0];

            ushort[] yData = rawSpectrum.RawSpectrum;
            ushort[] xData = rawSpectrum.Pixels;

            s.Points.DataBindXY(xData, yData);

            //chartSpectrum.ChartAreas[0].AxisY.IsStartedFromZero = true;
            //chartSpectrum.ChartAreas[0].AxisY.Maximum = yMax;

            chartSpectrum.ChartAreas[0].RecalculateAxesScale();

            StringBuilder dataStr = new StringBuilder();
            dataStr.Append("Pixel,RawSpectrum\r\n");
            for (int i = 0; i < xData.Length; i++)
            {
                dataStr.Append(i).Append(',').Append(yData[i]).Append("\r\n");
            }
            textBoxSpectrumData.Text = dataStr.ToString();

            textBoxNumPixelsReturned.Text = xData.Length.ToString();
            textBoxMaxSpectrumValue.Text = rawSpectrum.MaxValue.ToString();
            textBoxMaxValuePixelNum.Text = rawSpectrum.MaxValuePixelNum.ToString();
            textBoxMetadataSpectrumCount.Text = "n/a";
            textBoxMetadataTickCount.Text = "n/a";
            textBoxMetadataTimeUS.Text = "n/a";
            textBoxMetadataTriggerMode.Text = "n/a";
        }


        //--- ReadPartialCorrectedSpectrum ---

        private void buttonReadPartialCorrectedSpectrum_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadPartialCorrectedSpectrum;
            mActiveButton.Enabled = false;

            textBoxSpectrumData.Text = "";
            textBoxNumPixelsReturned.Text = "";
            textBoxMaxSpectrumValue.Text = "";
            textBoxMaxValuePixelNum.Text = "";
            textBoxMetadataSpectrumCount.Text = "";
            textBoxMetadataTickCount.Text = "";
            textBoxMetadataTimeUS.Text = "";
            textBoxMetadataTriggerMode.Text = "";

            Thread ioThread = new Thread(doReadPartialCorrectedSpectrum);
            ioThread.Start();
        }

        private void doReadPartialCorrectedSpectrum()
        {
            // Send the command
            OBPGetPartialCorrectedSpectrum partialCorrectedSpectrum = new OBPGetPartialCorrectedSpectrum(mActiveIO, mRequest, mResponse);
            sendMessage(partialCorrectedSpectrum);

            Invoke((MethodInvoker)delegate
            {
                onReadPartialCorrectedSpectrum(partialCorrectedSpectrum); // invoke on UI thread
            });
        }

        private void onReadPartialCorrectedSpectrum(OBPGetPartialCorrectedSpectrum partialCorrectedSpectrum)
        {
            Series s = chartSpectrum.Series[0];

            ushort[] yData = partialCorrectedSpectrum.PartialCorrectedSpectrum;
            ushort[] xData = partialCorrectedSpectrum.Pixels;

            s.Points.DataBindXY(xData, yData);

            //chartSpectrum.ChartAreas[0].AxisY.IsStartedFromZero = true;
            //chartSpectrum.ChartAreas[0].AxisY.Maximum = yMax;

            chartSpectrum.ChartAreas[0].RecalculateAxesScale();

            StringBuilder dataStr = new StringBuilder();
            dataStr.Append("Pixel,PartialCorrectedSpectrum\r\n");
            for (int i = 0; i < xData.Length; i++)
            {
                dataStr.Append(i).Append(',').Append(yData[i]).Append("\r\n");
            }
            textBoxSpectrumData.Text = dataStr.ToString();

            textBoxNumPixelsReturned.Text = xData.Length.ToString();
            textBoxMaxSpectrumValue.Text = partialCorrectedSpectrum.MaxValue.ToString();
            textBoxMaxValuePixelNum.Text = partialCorrectedSpectrum.MaxValuePixelNum.ToString();
            textBoxMetadataSpectrumCount.Text = "n/a";
            textBoxMetadataTickCount.Text = "n/a";
            textBoxMetadataTimeUS.Text = "n/a";
            textBoxMetadataTriggerMode.Text = "n/a";
        }


        //--- ReadBufferedSpectrum ---

        private void buttonReadBufferedSpectrum_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadBufferedSpectrum;
            mActiveButton.Enabled = false;

            textBoxSpectrumData.Text = "";
            textBoxNumPixelsReturned.Text = "";
            textBoxMaxSpectrumValue.Text = "";
            textBoxMaxValuePixelNum.Text = "";
            textBoxMetadataSpectrumCount.Text = "";
            textBoxMetadataTickCount.Text = "";
            textBoxMetadataTimeUS.Text = "";
            textBoxMetadataTriggerMode.Text = "";

            Thread ioThread = new Thread(doReadBufferedSpectrum);
            ioThread.Start();
        }

        private void doReadBufferedSpectrum()
        {
            // Send the command
            OBPGetBufferedSpectrum bufferedSpectrum = new OBPGetBufferedSpectrum(mActiveIO, mRequest, mResponse);
            sendMessage(bufferedSpectrum);

            Invoke((MethodInvoker)delegate
            {
                onReadBufferedSpectrum(bufferedSpectrum); // invoke on UI thread
            });
        }

        private void onReadBufferedSpectrum(OBPGetBufferedSpectrum bufferedSpectrum)
        {
            Series s = chartSpectrum.Series[0];

            uint[] yData = bufferedSpectrum.BufferedSpectrum;
            uint[] xData = bufferedSpectrum.Pixels;

            s.Points.DataBindXY(xData, yData);

            //chartSpectrum.ChartAreas[0].AxisY.IsStartedFromZero = true;
            //chartSpectrum.ChartAreas[0].AxisY.Maximum = yMax;

            chartSpectrum.ChartAreas[0].RecalculateAxesScale();

            StringBuilder dataStr = new StringBuilder();
            dataStr.Append("Pixel,BufferedSpectrum\r\n");
            for (int i = 0; i < xData.Length; i++)
            {
                dataStr.Append(i).Append(',').Append(yData[i]).Append("\r\n");
            }
            textBoxSpectrumData.Text = dataStr.ToString();

            textBoxNumPixelsReturned.Text = xData.Length.ToString();
            textBoxMaxSpectrumValue.Text = bufferedSpectrum.MaxValue.ToString();
            textBoxMaxValuePixelNum.Text = bufferedSpectrum.MaxValuePixelNum.ToString();
            textBoxMetadataSpectrumCount.Text = bufferedSpectrum.SpectrumCount.ToString();
            textBoxMetadataTickCount.Text = bufferedSpectrum.TickCount.ToString();
            textBoxMetadataTimeUS.Text = bufferedSpectrum.IntegrationTime.ToString();
            textBoxMetadataTriggerMode.Text = bufferedSpectrum.TriggerMode.ToString();
        }


        //--- ReadMaxBufferSize ---

        private void buttonReadMaxBufferSize_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadMaxBufferSize;
            mActiveButton.Enabled = false;

            textBoxMaxBufferSize.Text = "";

            Thread ioThread = new Thread(() => doReadMaxBufferSize());
            ioThread.Start();
        }

        private void doReadMaxBufferSize()
        {
            // Send the command
            OBPGetMaxBufferSize maxBufferSize = new OBPGetMaxBufferSize(mActiveIO, mRequest, mResponse);
            sendMessage(maxBufferSize);

            Invoke((MethodInvoker)delegate
            {
                onReadMaxBufferSize(maxBufferSize); // invoke on UI thread
            });
        }

        private void onReadMaxBufferSize(OBPGetMaxBufferSize maxBufferSize)
        {
            textBoxMaxBufferSize.Text = maxBufferSize.MaxBufferSize.ToString();
        }


        //--- ReadMaxBufferSize ---

        private void buttonReadBufferSize_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadBufferSize;
            mActiveButton.Enabled = false;

            textBoxBufferSize.Text = "";

            Thread ioThread = new Thread(() => doReadBufferSize());
            ioThread.Start();
        }

        private void doReadBufferSize()
        {
            // Send the command
            OBPGetBufferSize bufferSize = new OBPGetBufferSize(mActiveIO, mRequest, mResponse);
            sendMessage(bufferSize);

            Invoke((MethodInvoker)delegate
            {
                onReadBufferSize(bufferSize); // invoke on UI thread
            });
        }

        private void onReadBufferSize(OBPGetBufferSize bufferSize)
        {
            textBoxBufferSize.Text = bufferSize.BufferSize.ToString();
        }


        //--- WriteBufferSize ---

        private void buttonWriteBufferSize_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            uint bufferSize = 0;
            try
            {
                bufferSize = uint.Parse(textBoxBufferSize.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid buffer size (uint)");
                return;
            }

            mActiveButton = buttonWriteBufferSize;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteBufferSize(bufferSize));
            ioThread.Start();
        }

        private void doWriteBufferSize(uint bufferSizeVal)
        {
            // Send the command
            OBPSetBufferSize bufferSize = new OBPSetBufferSize(mActiveIO, mRequest, mResponse);
            bufferSize.BufferSize = bufferSizeVal;
            sendMessage(bufferSize);

            Invoke((MethodInvoker)delegate
            {
                onWriteBufferSize(bufferSize); // invoke on UI thread
            });
        }

        private void onWriteBufferSize(OBPSetBufferSize bufferSize)
        {
            // nothing to do
        }


        //--- ReadNumSpectraInBuffer ---

        private void buttonReadNumSpectraInBuffer_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadNumSpectraInBuffer;
            mActiveButton.Enabled = false;

            textBoxNumSpectraInBuffer.Text = "";

            Thread ioThread = new Thread(() => doReadNumSpectraInBuffer());
            ioThread.Start();
        }

        private void doReadNumSpectraInBuffer()
        {
            // Send the command
            OBPGetNumSpectraInBuffer numSpectraInBuffer = new OBPGetNumSpectraInBuffer(mActiveIO, mRequest, mResponse);
            sendMessage(numSpectraInBuffer);

            Invoke((MethodInvoker)delegate
            {
                onReadNumSpectraInBuffer(numSpectraInBuffer); // invoke on UI thread
            });
        }

        private void onReadNumSpectraInBuffer(OBPGetNumSpectraInBuffer numSpectraInBuffer)
        {
            textBoxNumSpectraInBuffer.Text = numSpectraInBuffer.NumSpectraInBuffer.ToString();
        }


        //--- RemoveOldestBufferedSpectra ---

        private void buttonRemoveOldestSpectra_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            uint numSpectra = 0;
            try
            {
                numSpectra = uint.Parse(textBoxRemoveOldestSpectra.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid number of spectra to remove (uint)");
                return;
            }

            mActiveButton = buttonRemoveOldestSpectra;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doRemoveOldestBufferedSpectra(numSpectra));
            ioThread.Start();
        }

        private void doRemoveOldestBufferedSpectra(uint numSpectraVal)
        {
            // Send the command
            OBPRemoveOldestBufferedSpectra removeOldestSpectra = new OBPRemoveOldestBufferedSpectra(mActiveIO, mRequest, mResponse);
            removeOldestSpectra.NumSpectra = numSpectraVal;
            sendMessage(removeOldestSpectra);

            Invoke((MethodInvoker)delegate
            {
                onRemoveOldestBufferedSpectra(removeOldestSpectra); // invoke on UI thread
            });
        }

        private void onRemoveOldestBufferedSpectra(OBPRemoveOldestBufferedSpectra removeOldestSpectra)
        {
            // nothing to do
        }


        //--- ReadIsIdle ---

        private void buttonReadIsIdle_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadIsIdle;
            mActiveButton.Enabled = false;

            textBoxIsIdle.Text = "";

            Thread ioThread = new Thread(() => doReadIsIdle());
            ioThread.Start();
        }

        private void doReadIsIdle()
        {
            // Send the command
            OBPQueryIsIdle isIdle = new OBPQueryIsIdle(mActiveIO, mRequest, mResponse);
            sendMessage(isIdle);

            Invoke((MethodInvoker)delegate
            {
                onReadIsIdle(isIdle); // invoke on UI thread
            });
        }

        private void onReadIsIdle(OBPQueryIsIdle isIdle)
        {
            textBoxIsIdle.Text = isIdle.IsIdle.ToString();
        }


        //--- ClearBufferedSpectra ---

        private void buttonClearBufferedSpectra_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonClearBufferedSpectra;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doClearBufferedSpectra());
            ioThread.Start();
        }

        private void doClearBufferedSpectra()
        {
            // Send the command
            OBPClearBufferedSpectra clearBufferedSpectra = new OBPClearBufferedSpectra(mActiveIO, mRequest, mResponse);
            sendMessage(clearBufferedSpectra);

            Invoke((MethodInvoker)delegate
            {
                onClearBufferedSpectra(clearBufferedSpectra); // invoke on UI thread
            });
        }

        private void onClearBufferedSpectra(OBPClearBufferedSpectra clearBufferedSpectra)
        {
            // nothing to do
        }


        //--- AbortAcquisition ---

        private void buttonAbortAcquisition_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonAbortAcquisition;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doAbortAcquisition());
            ioThread.Start();
        }

        private void doAbortAcquisition()
        {
            // Send the command
            OBPAbortAcquisition abortAcquisition = new OBPAbortAcquisition(mActiveIO, mRequest, mResponse);
            sendMessage(abortAcquisition);

            Invoke((MethodInvoker)delegate
            {
                onAbortAcquisition(abortAcquisition); // invoke on UI thread
            });
        }

        private void onAbortAcquisition(OBPAbortAcquisition abortAcquisition)
        {
            // nothing to do
        }


        //--- AcquireSpectraIntoBuffer ---

        private void buttonAcquireSpectraIntoBuffer_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonAcquireSpectraIntoBuffer;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doAcquireSpectraIntoBuffer());
            ioThread.Start();
        }

        private void doAcquireSpectraIntoBuffer()
        {
            // Send the command
            OBPAcquireSpectraIntoBuffer acquireSpectraIntoBuffer = new OBPAcquireSpectraIntoBuffer(mActiveIO, mRequest, mResponse);
            sendMessage(acquireSpectraIntoBuffer);

            Invoke((MethodInvoker)delegate
            {
                onAcquireSpectraIntoBuffer(acquireSpectraIntoBuffer); // invoke on UI thread
            });
        }

        private void onAcquireSpectraIntoBuffer(OBPAcquireSpectraIntoBuffer acquireSpectraIntoBuffer)
        {
            // nothing to do
        }


        //--- ReadNumTempSensors ---

        private void buttonReadNumTempSensors_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadNumTempSensors;
            mActiveButton.Enabled = false;

            textBoxNumTempSensors.Text = "";

            Thread ioThread = new Thread(() => doReadNumTempSensors());
            ioThread.Start();
        }

        private void doReadNumTempSensors()
        {
            // Send the command
            OBPGetNumTemperatureSensors numTempSensors = new OBPGetNumTemperatureSensors(mActiveIO, mRequest, mResponse);
            sendMessage(numTempSensors);

            Invoke((MethodInvoker)delegate
            {
                onReadNumTempSensors(numTempSensors); // invoke on UI thread
            });
        }

        private void onReadNumTempSensors(OBPGetNumTemperatureSensors numTempSensors)
        {
            if (numTempSensors.NumTempSensors > 0)
            {
                buttonReadTempSensor.Enabled = true;
                comboBoxTempSensorIndex.Enabled = true;
                textBoxTempSensorValue.Enabled = true;

                mTempSensorIndexes = new string[numTempSensors.NumTempSensors];
                for (int i = 0; i < mTempSensorIndexes.Length; i++)
                {
                    mTempSensorIndexes[i] = i.ToString();
                }
                comboBoxTempSensorIndex.DataSource = mTempSensorIndexes;
            }
            else
            {
                buttonReadTempSensor.Enabled = false;
                comboBoxTempSensorIndex.Enabled = false;
                textBoxTempSensorValue.Enabled = false;
            }
            textBoxNumTempSensors.Text = numTempSensors.NumTempSensors.ToString();
        }


        //--- ReadTempSensor ---

        private void buttonReadTempSensor_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadTempSensor;
            mActiveButton.Enabled = false;

            textBoxTempSensorValue.Text = "";
            byte tempSensorIndex = (byte)comboBoxTempSensorIndex.SelectedIndex;

            Thread ioThread = new Thread(() => doReadTempSensor(tempSensorIndex));
            ioThread.Start();
        }

        private void doReadTempSensor(byte tempSensorIndex)
        {
            // Send the command
            OBPGetTemperatureSensor tempSensor = new OBPGetTemperatureSensor(mActiveIO, mRequest, mResponse);
            tempSensor.TempSensorIndex = tempSensorIndex;
            sendMessage(tempSensor);

            Invoke((MethodInvoker)delegate
            {
                onReadTempSensor(tempSensor); // invoke on UI thread
            });
        }

        private void onReadTempSensor(OBPGetTemperatureSensor tempSensor)
        {
            textBoxTempSensorValue.Text = tempSensor.TempSensor.ToString();
        }


        //--- ReadAllTempSensors ---

        private void buttonReadAllTempSensors_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadAllTempSensors;
            mActiveButton.Enabled = false;

            textBoxAllTempSensors.Text = "";
            byte tempSensorIndex = (byte)comboBoxTempSensorIndex.SelectedIndex;

            Thread ioThread = new Thread(() => doReadAllTempSensors(tempSensorIndex));
            ioThread.Start();
        }

        private void doReadAllTempSensors(byte tempSensorIndex)
        {
            // Send the command
            OBPGetAllTemperatureSensors allTempSensors = new OBPGetAllTemperatureSensors(mActiveIO, mRequest, mResponse);
            sendMessage(allTempSensors);

            Invoke((MethodInvoker)delegate
            {
                onReadTempSensor(allTempSensors); // invoke on UI thread
            });
        }

        private void onReadTempSensor(OBPGetAllTemperatureSensors allTempSensors)
        {
            StringBuilder tempStr = new StringBuilder();
            for (int i = 0; i < allTempSensors.TempSensors.Length; i++)
            {
                if (i > 0)
                {
                    tempStr.Append("   ");
                }
                tempStr.Append(allTempSensors.TempSensors[i]);
            }
            textBoxAllTempSensors.Text = tempStr.ToString();
        }


        //--- ReadTecEnable ---

        private void buttonReadTecEnable_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadTecEnable;
            mActiveButton.Enabled = false;

            textBoxTecEnable.Text = "";

            Thread ioThread = new Thread(() => doReadTecEnable());
            ioThread.Start();
        }

        private void doReadTecEnable()
        {
            // Send the command
            OBPGetTecEnable tecEnable = new OBPGetTecEnable(mActiveIO, mRequest, mResponse);
            sendMessage(tecEnable);

            Invoke((MethodInvoker)delegate
            {
                onReadTecEnable(tecEnable); // invoke on UI thread
            });
        }

        private void onReadTecEnable(OBPGetTecEnable tecEnable)
        {
            textBoxTecEnable.Text = tecEnable.TecEnable.ToString();
        }


        //--- WriteTecEnable ---

        private void buttonWriteTecEnable_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte tecEnable = 0;
            try
            {
                tecEnable = byte.Parse(textBoxTecEnable.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid TEC enable value (0, 1)");
                return;
            }

            mActiveButton = buttonWriteTecEnable;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteTecEnable(tecEnable));
            ioThread.Start();
        }

        private void doWriteTecEnable(byte tecEnableVal)
        {
            // Send the command
            OBPSetTecEnable tecEnable = new OBPSetTecEnable(mActiveIO, mRequest, mResponse);
            tecEnable.TecEnable = tecEnableVal;
            sendMessage(tecEnable);

            Invoke((MethodInvoker)delegate
            {
                onWriteTecEnable(tecEnable); // invoke on UI thread
            });
        }

        private void onWriteTecEnable(OBPSetTecEnable tecEnable)
        {
            // nothing to do
        }


        //--- ReadTecSetpoint ---

        private void buttonReadTecSetpoint_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadTecSetpoint;
            mActiveButton.Enabled = false;

            textBoxTecSetpoint.Text = "";

            Thread ioThread = new Thread(() => doReadTecSetpoint());
            ioThread.Start();
        }

        private void doReadTecSetpoint()
        {
            // Send the command
            OBPGetTecSetpoint tecSetpoint = new OBPGetTecSetpoint(mActiveIO, mRequest, mResponse);
            sendMessage(tecSetpoint);

            Invoke((MethodInvoker)delegate
            {
                onReadTecTecSetpoint(tecSetpoint); // invoke on UI thread
            });
        }

        private void onReadTecTecSetpoint(OBPGetTecSetpoint tecSetpoint)
        {
            textBoxTecSetpoint.Text = tecSetpoint.TecSetpoint.ToString();
        }


        //--- WriteTecSetpoint ---

        private void buttonWriteTecSetpoint_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            float tecSetpoint = 0;
            try
            {
                tecSetpoint = float.Parse(textBoxTecSetpoint.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid TEC setpoint value (float)");
                return;
            }

            mActiveButton = buttonWriteTecSetpoint;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteTecSetpoint(tecSetpoint));
            ioThread.Start();
        }

        private void doWriteTecSetpoint(float tecSetpointVal)
        {
            // Send the command
            OBPSetTecSetpoint tecSetpoint = new OBPSetTecSetpoint(mActiveIO, mRequest, mResponse);
            tecSetpoint.TecSetpoint = tecSetpointVal;
            sendMessage(tecSetpoint);

            Invoke((MethodInvoker)delegate
            {
                onWriteTecSetpoint(tecSetpoint); // invoke on UI thread
            });
        }

        private void onWriteTecSetpoint(OBPSetTecSetpoint tecSetpoint)
        {
            // nothing to do
        }


        //--- ReadTecStable ---

        private void buttonReadTecStable_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadTecStable;
            mActiveButton.Enabled = false;

            textBoxTecStable.Text = "";

            Thread ioThread = new Thread(() => doReadTecStable());
            ioThread.Start();
        }

        private void doReadTecStable()
        {
            // Send the command
            OBPGetTecStable tecStable = new OBPGetTecStable(mActiveIO, mRequest, mResponse);
            sendMessage(tecStable);

            Invoke((MethodInvoker)delegate
            {
                onReadTecStable(tecStable); // invoke on UI thread
            });
        }

        private void onReadTecStable(OBPGetTecStable tecStable)
        {
            textBoxTecStable.Text = tecStable.TecStable.ToString();
        }


        //--- ReadTecTemperature ---

        private void buttonReadTecTemperature_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadTecTemperature;
            mActiveButton.Enabled = false;

            textBoxTecTemperature.Text = "";

            Thread ioThread = new Thread(() => doReadTecTemperature());
            ioThread.Start();
        }

        private void doReadTecTemperature()
        {
            // Send the command
            OBPGetTecTemperature tecTemperature = new OBPGetTecTemperature(mActiveIO, mRequest, mResponse);
            sendMessage(tecTemperature);

            Invoke((MethodInvoker)delegate
            {
                onReadTecTemperature(tecTemperature); // invoke on UI thread
            });
        }

        private void onReadTecTemperature(OBPGetTecTemperature tecTemperature)
        {
            textBoxTecTemperature.Text = tecTemperature.TecTemperature.ToString();
        }


        //--- ReadNumGPIOPins ---

        private void buttonReadNumGPIOPins_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadNumGPIOPins;
            mActiveButton.Enabled = false;

            textBoxNumGPIOPins.Text = "";

            Thread ioThread = new Thread(() => doReadNumGPIOPins());
            ioThread.Start();
        }

        private void doReadNumGPIOPins()
        {
            // Send the command
            OBPGetNumGPIOPins numGPIOPins = new OBPGetNumGPIOPins(mActiveIO, mRequest, mResponse);
            sendMessage(numGPIOPins);

            Invoke((MethodInvoker)delegate
            {
                onReadNumGPIOPins(numGPIOPins); // invoke on UI thread
            });
        }

        private void onReadNumGPIOPins(OBPGetNumGPIOPins numGPIOPins)
        {
            textBoxNumGPIOPins.Text = numGPIOPins.NumPins.ToString();
        }


        //--- ReadGPIOOutputEnable ---

        private void buttonReadGPIOOutputEnable_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadGPIOOutputEnable;
            mActiveButton.Enabled = false;

            textBoxGPIOOutputEnable.Text = "";

            Thread ioThread = new Thread(() => doReadGPIOOutputEnable());
            ioThread.Start();
        }

        private void doReadGPIOOutputEnable()
        {
            // Send the command
            OBPGetGPIOOutputEnable gpioOutputEnable = new OBPGetGPIOOutputEnable(mActiveIO, mRequest, mResponse);
            sendMessage(gpioOutputEnable);

            Invoke((MethodInvoker)delegate
            {
                onReadGPIOOutputEnable(gpioOutputEnable); // invoke on UI thread
            });
        }

        private void onReadGPIOOutputEnable(OBPGetGPIOOutputEnable gpioOutputEnable)
        {
            textBoxGPIOOutputEnable.Text = String.Format("0x{0:X8}", gpioOutputEnable.OutputEnable);
        }


        //--- WriteGPIOOutputEnable ---

        private void buttonWriteGPIOOutputEnable_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            uint outputEnable = 0;
            uint outputEnableMask = 0;
            try
            {
                outputEnable = uint.Parse(textBoxGPIOOutputEnable.Text.Replace("0x", ""), NumberStyles.HexNumber);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid output enable value (uint)");
                return;
            }

            try
            {
                outputEnableMask = uint.Parse(textBoxGPIOOutputEnableMask.Text.Replace("0x", ""), NumberStyles.HexNumber);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid output enable write mask (uint)");
                return;
            }

            mActiveButton = buttonWriteGPIOOutputEnable;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteGPIOOutputEnable(outputEnable, outputEnableMask));
            ioThread.Start();
        }

        private void doWriteGPIOOutputEnable(uint outputEnableVal, uint outputEnableMask)
        {
            // Send the command
            OBPSetGPIOOutputEnable gpioOutputEnable = new OBPSetGPIOOutputEnable(mActiveIO, mRequest, mResponse);
            gpioOutputEnable.OutputEnable = outputEnableVal;
            gpioOutputEnable.OutputEnableMask = outputEnableMask;
            sendMessage(gpioOutputEnable);

            Invoke((MethodInvoker)delegate
            {
                onWriteGPIOOutputEnable(gpioOutputEnable); // invoke on UI thread
            });
        }

        private void onWriteGPIOOutputEnable(OBPSetGPIOOutputEnable gpioOutputEnable)
        {
            // nothing to do
        }


        //--- ReadGPIOValue ---

        private void buttonReadGPIOValue_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadGPIOValue;
            mActiveButton.Enabled = false;

            textBoxGPIOValue.Text = "";

            Thread ioThread = new Thread(() => doReadGPIOValue());
            ioThread.Start();
        }

        private void doReadGPIOValue()
        {
            // Send the command
            OBPGetGPIOValue gpioValue = new OBPGetGPIOValue(mActiveIO, mRequest, mResponse);
            sendMessage(gpioValue);

            Invoke((MethodInvoker)delegate
            {
                onReadGPIOValue(gpioValue); // invoke on UI thread
            });
        }

        private void onReadGPIOValue(OBPGetGPIOValue gpioValue)
        {
            textBoxGPIOValue.Text = String.Format("0x{0:X8}", gpioValue.GPIOValue);
        }


        //--- WriteGPIOValue ---

        private void buttonWriteGPIOValue_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            uint gpioValue = 0;
            uint gpioValueMask = 0;
            try
            {
                gpioValue = uint.Parse(textBoxGPIOValue.Text.Replace("0x", ""), NumberStyles.HexNumber);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid GPIO value (uint)");
                return;
            }

            try
            {
                gpioValueMask = uint.Parse(textBoxGPIOValueMask.Text.Replace("0x", ""), NumberStyles.HexNumber);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid GPIO value write mask (uint)");
                return;
            }

            mActiveButton = buttonWriteGPIOOutputEnable;
            mActiveButton.Enabled = false;

            Thread ioThread = new Thread(() => doWriteGPIOValue(gpioValue, gpioValueMask));
            ioThread.Start();
        }

        private void doWriteGPIOValue(uint gpioVal, uint gpioValueMask)
        {
            // Send the command
            OBPSetGPIOValue gpioValue = new OBPSetGPIOValue(mActiveIO, mRequest, mResponse);
            gpioValue.GPIOValue = gpioVal;
            gpioValue.GPIOValueMask = gpioValueMask;
            sendMessage(gpioValue);

            Invoke((MethodInvoker)delegate
            {
                onWriteGPIOValue(gpioValue); // invoke on UI thread
            });
        }

        private void onWriteGPIOValue(OBPSetGPIOValue gpioValue)
        {
            // nothing to do
        }


        //--- ReadNumI2CBuses ---

        private void buttonReadI2CNumBuses_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            mActiveButton = buttonReadI2CNumBuses;
            mActiveButton.Enabled = false;

            textBoxI2CNumBuses.Text = "";

            Thread ioThread = new Thread(() => doReadNumI2CBuses());
            ioThread.Start();
        }

        private void doReadNumI2CBuses()
        {
            // Send the command
            OBPGetNumI2CBuses numI2CBuses = new OBPGetNumI2CBuses(mActiveIO, mRequest, mResponse);
            sendMessage(numI2CBuses);

            Invoke((MethodInvoker)delegate
            {
                onReadNumI2CBuses(numI2CBuses); // invoke on UI thread
            });
        }

        private void onReadNumI2CBuses(OBPGetNumI2CBuses numI2CBuses)
        {
            if (numI2CBuses.NumI2CBuses > 0)
            {
                buttonReadI2CBytes.Enabled = true;
                buttonWriteI2CBytes.Enabled = true;
                comboBoxI2CDataBusNum.Enabled = true;
                textBoxI2CAddress.Enabled = true;
                textBoxI2CNumBytes.Enabled = true;
                textBoxI2CData.Enabled = true;
                buttonWriteI2CClockLimit.Enabled = true;
                comboBoxI2CClockBusNum.Enabled = true;
                textBoxI2CClockLimit.Enabled = true;

                mI2CBusIndexes = new string[numI2CBuses.NumI2CBuses];
                for (int i = 0; i < mI2CBusIndexes.Length; i++)
                {
                    mI2CBusIndexes[i] = i.ToString();
                }
                comboBoxI2CDataBusNum.DataSource = mI2CBusIndexes;
                comboBoxI2CClockBusNum.DataSource = mI2CBusIndexes;
            }
            else
            {
                buttonReadI2CBytes.Enabled = false;
                buttonWriteI2CBytes.Enabled = false;
                comboBoxI2CDataBusNum.Enabled = false;
                textBoxI2CAddress.Enabled = false;
                textBoxI2CNumBytes.Enabled = false;
                textBoxI2CData.Enabled = false;
                buttonWriteI2CClockLimit.Enabled = false;
                comboBoxI2CClockBusNum.Enabled = false;
                textBoxI2CClockLimit.Enabled = false;
            }
            textBoxI2CNumBuses.Text = numI2CBuses.NumI2CBuses.ToString();
        }


        //--- ReadI2CBus ---

        private void buttonReadI2CBytes_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte busNum = (byte)comboBoxI2CDataBusNum.SelectedIndex;
            byte address = 0;
            ushort numBytes = 0;

            try
            {
                address = byte.Parse(textBoxI2CAddress.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid I2C address (0 - 127)");
                return;
            }

            try
            {
                numBytes = byte.Parse(textBoxI2CNumBytes.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid I2C Num Bytes (1 - 65535)");
                return;
            }

            mActiveButton = buttonReadI2CBytes;
            mActiveButton.Enabled = false;

            textBoxI2CData.Text = "";
            textBoxI2CNumBytes.Text = "";

            Thread ioThread = new Thread(() => doReadI2CBus(busNum, address, numBytes));
            ioThread.Start();
        }

        private void doReadI2CBus(byte busNum, byte address, ushort numBytes)
        {
            // Send the command
            OBPReadI2CBus readI2CBus = new OBPReadI2CBus(mActiveIO, mRequest, mResponse);
            readI2CBus.I2CBusNum = busNum;
            readI2CBus.I2CAddress = address;
            readI2CBus.NumBytes = numBytes;
            sendMessage(readI2CBus);

            Invoke((MethodInvoker)delegate
            {
                onReadI2CBus(readI2CBus); // invoke on UI thread
            });
        }

        private void onReadI2CBus(OBPReadI2CBus readI2CBus)
        {
            textBoxI2CNumBytes.Text = readI2CBus.I2CData.Length.ToString();
            textBoxI2CData.Text = OBPMessage.ToHexByteString(readI2CBus.I2CData);
        }


        //--- WriteI2CBus ---

        private void buttonWriteI2CBytes_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte busNum = (byte)comboBoxI2CDataBusNum.SelectedIndex;
            byte address = 0;
            byte[] dataA;

            try
            {
                address = byte.Parse(textBoxI2CAddress.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid I2C address (0 - 127)");
                return;
            }

            try
            {
                dataA = OBPMessage.FromHexByteString(textBoxI2CData.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid data bytes (must be space separated hex byte string such as: 00 01 F0 A2...");
                return;
            }

            mActiveButton = buttonWriteI2CBytes;
            mActiveButton.Enabled = false;

            textBoxI2CNumBytes.Text = "";

            Thread ioThread = new Thread(() => doWriteI2CBus(busNum, address, dataA));
            ioThread.Start();
        }

        private void doWriteI2CBus(byte busNum, byte address, byte[] dataA)
        {
            // Send the command
            OBPWriteI2CBus writeI2CBus = new OBPWriteI2CBus(mActiveIO, mRequest, mResponse);
            writeI2CBus.I2CBusNum = busNum;
            writeI2CBus.I2CAddress = address;
            writeI2CBus.I2CData = dataA;
            sendMessage(writeI2CBus);

            Invoke((MethodInvoker)delegate
            {
                onWriteI2CBus(writeI2CBus); // invoke on UI thread
            });
        }

        private void onWriteI2CBus(OBPWriteI2CBus writeI2CBus)
        {
            textBoxI2CNumBytes.Text = writeI2CBus.NumBytes.ToString();
        }


        //--- WriteI2CClockLimit ---

        private void buttonWriteI2CClockLimit_Click(object sender, EventArgs e)
        {
            if (!okToSend()) { return; }

            byte busNum = (byte)comboBoxI2CClockBusNum.SelectedIndex;
            uint clockLimit = 0;

            try
            {
                clockLimit = uint.Parse(textBoxI2CClockLimit.Text);
            }
            catch (Exception)
            {
                setStatusFailed("Invalid clock limit (uint)");
                return;
            }

            mActiveButton = buttonWriteI2CClockLimit;
            mActiveButton.Enabled = false;

            textBoxI2CData.Text = "";
            textBoxI2CNumBytes.Text = "";

            Thread ioThread = new Thread(() => doWriteI2CBus(busNum, clockLimit));
            ioThread.Start();
        }

        private void doWriteI2CBus(byte busNum, uint clockLimitVal)
        {
            // Send the command
            OBPSetI2CClockLimit clockLimit = new OBPSetI2CClockLimit(mActiveIO, mRequest, mResponse);
            clockLimit.I2CBusNum = busNum;
            clockLimit.ClockLimit = clockLimitVal;
            sendMessage(clockLimit);

            Invoke((MethodInvoker)delegate
            {
                onWriteI2CClockLimit(clockLimit); // invoke on UI thread
            });
        }

        private void onWriteI2CClockLimit(OBPSetI2CClockLimit clockLimit)
        {
            // nothing to do
        }

        private void dataGridViewUSBDeviceList_SelectionChanged(object sender, EventArgs e)
        {

            if(dataGridViewUSBDeviceList.SelectedRows.Count>0)
            {
                // find the row that's selected, there is only one
                if (dataGridViewUSBDeviceList.SelectedRows[0].Tag != null)
                {
                    // repair the devicePath
                    USBDeviceInfo di = (USBDeviceInfo)(dataGridViewUSBDeviceList.SelectedRows[0].Tag);
                    
                    if(mUSBIO== null)
                        mUSBIO = new USBIO(di);

                    if (mUSBIO != null)
                    {
                        dataGridViewInPipes.Rows.Clear();
                        dataGridViewOutPipes.Rows.Clear();

                        // Ocean Optics Spectrometers only have one interface
                        foreach (USBPipe aPipe in mUSBIO.Interfaces[0].Pipes)
                        {
                            if (aPipe.IsIn)
                            {
                                dataGridViewInPipes.Rows.Add(aPipe.Address.ToString("X2"));
                                dataGridViewInPipes.Rows[dataGridViewInPipes.Rows.Count - 1].Tag = aPipe;
                            }
                            if (aPipe.IsOut)
                            {
                                dataGridViewOutPipes.Rows.Add(aPipe.Address.ToString("X2"));
                                dataGridViewOutPipes.Rows[dataGridViewOutPipes.Rows.Count - 1].Tag = aPipe;
                            }
                        }
                    }
                }
            }
        }
    }
}
