using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FlameX_SeaBreeze_Command_Test
{
    class TestClass
    {
        const int SEABREEZE_ID = 0;
        ListBox loggingListBox;
        private static int errorStringLength = SeaBreezeWrapper.seabreeze_get_error_string_maximum_length();
        private byte[] errorMessage = new byte[errorStringLength];

        public TestClass(ListBox listBoxForLogging)
        {
            loggingListBox = listBoxForLogging;
            GetSerialNumber();
        }

        ~TestClass()
        {

        }

        public virtual void RunSpectrometerSpecificTests()
        {
            Log("No more tests are implemented.");
        }

        public void RunCommonTests()
        {
            Log("Running Common Tests.");
        }

        public void Log(string message)
        {
            if (loggingListBox != null)
            {
                loggingListBox.Items.Add(message);
            }
        }

        public string GetErrorDescription(int errorCode)
        {
            string description = "Could not retrieve the error description";
            int bogus = 0;



            int stringlength = SeaBreezeWrapper.seabreeze_get_error_string(errorCode, ref errorMessage[0], errorStringLength);

            return errorMessage.ToString();
        }

        public void GetSerialNumber()
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
            string mySerialNumber = System.Text.Encoding.UTF8.GetString(mySerialNumberBuffer).TrimEnd((char)0);
            Log("Serial Number: " + mySerialNumber);
        }

    }
}
