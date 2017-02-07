using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace FlameX_SeaBreeze_Command_Test
{
    class SeaBreezeSpectrometerClass
    {
        ListBox logListBox;
        TestClass myTests;
        string myName;
        Boolean isActive = false;
        public const int SEABREEZE_ID = 0;

        // opens the first spectrometer found. It will have a SeaBreeze ID of 0
        public SeaBreezeSpectrometerClass(ListBox listBoxForLogging, ref Boolean isConnected)
        {
            int zero_on_success = 0;
            int errorCode = 0;

            logListBox = listBoxForLogging;
            if (File.Exists(SeaBreezeWrapper.DLL))
            {
                zero_on_success = SeaBreezeWrapper.seabreeze_open_spectrometer(SEABREEZE_ID, ref errorCode);
                if ((zero_on_success == 0) && (errorCode == 0))
                {
                    isActive = true;
                    myName = GetName();
                    logListBox.Items.Add("Connected to: " + myName);
                    isConnected = true;

                    if (myName.StartsWith("FLAMEX"))
                    {
                        myTests = new FlameTestClass(logListBox, SEABREEZE_ID);
                    }
                    else if (myName.StartsWith("STS"))
                    {
                        myTests = new STSTestClass(logListBox);
                    }
                    else if (myName.StartsWith("QEPRO"))
                    {
                        myTests = new QEPROTestClass(logListBox);
                    }
                    else
                        logListBox.Items.Add(string.Format("A spectrometer test class was not found for {0}.", myName));
                }
                isConnected = isActive;
            }
            else
                throw (new Exception("The DLL library link was incorrect in the SeaBreezeWrapper.cs file."));
           
        }

        ~SeaBreezeSpectrometerClass()
        {
            Close();
        }

        public void Close()
        {
            if(isActive)
            {
                Int32 myErrorCode = 0;
                Int32 aLength = SeaBreezeWrapper.seabreeze_get_error_string_maximum_length();
                byte[] myBuffer = new byte[aLength];

                int totallyBogus = SeaBreezeWrapper.seabreeze_close_spectrometer(SEABREEZE_ID, ref myErrorCode);
                int bytesInBuffer = SeaBreezeWrapper.seabreeze_get_error_string(myErrorCode, ref myBuffer[0], aLength);
                string myErrorString = "Exception closing spectrometer: " + System.Text.Encoding.Default.GetString(myBuffer);


                if (myErrorCode == 0)
                    logListBox.Items.Add("Disconnected from spectrometer.");
                else
                    logListBox.Items.Add("Error disconnecting: "+myErrorString);
            }
        }

        public string GetName()
        {
            string result = "Not Connected";
            if(isActive)
            {
                Int32 myErrorCode = 0;

                Int32 errorStringLength = SeaBreezeWrapper.seabreeze_get_error_string_maximum_length();
                byte[] errorStringBuffer = new byte[errorStringLength];

                Int32 modelStringLength = SeaBreezeWrapper.seabreeze_get_model_string_maximum_length();
                byte[] myModelTypeBuffer = new byte[modelStringLength];

                int bytesInBuffer = SeaBreezeWrapper.seabreeze_get_model(SEABREEZE_ID, ref myErrorCode, ref myModelTypeBuffer[0], modelStringLength);
                if (myErrorCode != 0)
                {
                    bytesInBuffer = SeaBreezeWrapper.seabreeze_get_error_string(myErrorCode, ref errorStringBuffer[0], errorStringLength);
                    string myErrorString = "Exception getting model description: " + System.Text.Encoding.Default.GetString(errorStringBuffer);
                    throw (new Exception(myErrorString));
                }

                result =  System.Text.Encoding.UTF8.GetString(myModelTypeBuffer).TrimEnd((char)0);
            }
            return result;
        }

       public void RunTests()
        {
            if (myTests != null)
            {
                myTests.RunCommonTests();
                myTests.RunSpectrometerSpecificTests();
            }

        }
    }
}
