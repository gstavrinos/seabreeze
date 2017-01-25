using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FlameX_SeaBreeze_Command_Test
{
    class SeaBreezeSpectrometerClass
    {
        ListBox logListBox;
        string myName;
        string mySerialNumber;
        Boolean isActive = false;
        const int SEABREEZE_ID = 0;

        // opens the first spectrometer found. It will have a SeaBreeze ID of 0
        public SeaBreezeSpectrometerClass(ListBox listBoxForLogging, ref Boolean isConnected)
        {
            int zero_on_success = 0;
            int errorCode = 0;

            logListBox = listBoxForLogging;
            zero_on_success = SeaBreezeWrapper.seabreeze_open_spectrometer(SEABREEZE_ID, ref errorCode);
            if ((zero_on_success == 0) && (errorCode == 0))
            {
                isActive = true;
                myName = GetName();

                if (myName.StartsWith("FLAMEX"))
                {
                    isConnected = true;
                    mySerialNumber = GetSerialNumber();

                    Log("Connected to:" + myName + " " + mySerialNumber);

                    Log("Executed Command: 0x00110220  Get number of pixels");
                    Log("Executed Command: 0x00110221  Get active pixel ranges");
                    Log("Executed Command: 0x00110222  Get optical dark pixel ranges");
                    Log("Executed Command: 0x00110223  Get electrical dark pixel ranges");
                }
                else
                    Log(string.Format("The spectrometer found was a {0}, not a Flame X.", myName));
            }
            isConnected = isActive;
        }

        ~SeaBreezeSpectrometerClass()
        {
            Close();
        }

        public void RunTests()
        {
            if (isActive)
            {

            }
            else
                Log("Could not run any tests because there was no active Flame X.");
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
                    Log("Disconnected from spectrometer.");
                else
                    Log("Error disconnecting: "+myErrorString);
            }
        }

        public void Log(string message)
        {
            if(logListBox != null)
            {
                logListBox.Items.Add(message);
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

        public string GetSerialNumber()
        {
            string result = "Not Connected";

            if(isActive)
            {
                Int32 myErrorCode = 0;

                Int32 errorStringLength = SeaBreezeWrapper.seabreeze_get_error_string_maximum_length();
                byte[] errorStringBuffer = new byte[errorStringLength];

                Int32 serialNumberStringLength = SeaBreezeWrapper.seabreeze_get_serial_number_max_length(SEABREEZE_ID, ref myErrorCode);
                byte[] mySerialNumberBuffer = new byte[serialNumberStringLength];

                int bytesInBuffer = SeaBreezeWrapper.seabreeze_get_serial_number(SEABREEZE_ID, ref myErrorCode, ref mySerialNumberBuffer[0], serialNumberStringLength);
                if (myErrorCode != 0)
                {
                    bytesInBuffer = SeaBreezeWrapper.seabreeze_get_error_string(myErrorCode, ref errorStringBuffer[0], errorStringLength);
                    string myErrorString = "Exception getting model description: " + System.Text.Encoding.Default.GetString(errorStringBuffer);
                    throw (new Exception(myErrorString));
                }
                result =  System.Text.Encoding.UTF8.GetString(mySerialNumberBuffer).TrimEnd((char)0);
            }
            return result;
        }
    }
}
