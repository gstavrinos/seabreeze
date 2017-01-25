using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FlameX_SeaBreeze_Command_Test
{
    class SeaBreezeSpectrometerClass
    {
        private Int32 seaBreezeIndex = -1;

        public SeaBreezeSpectrometerClass(int spectrometerIndex)
        {
            seaBreezeIndex = spectrometerIndex;
        }

        ~SeaBreezeSpectrometerClass()
        {
            Close();
        }

        public void Close()
        {
            if(seaBreezeIndex > 0)
            {
                Int32 myErrorCode = 0;
                Int32 aLength = SeaBreezeWrapper.seabreeze_get_error_string_maximum_length();
                byte[] myBuffer = new byte[aLength];

                int totallyBogus = SeaBreezeWrapper.seabreeze_close_spectrometer(seaBreezeIndex, ref myErrorCode);
                int bytesInBuffer = SeaBreezeWrapper.seabreeze_get_error_string(myErrorCode, ref myBuffer[0], aLength);
                string myErrorString = "Exception closing spectrometer: " + System.Text.Encoding.Default.GetString(myBuffer);
                seaBreezeIndex = -1;
                if (myErrorCode != 0)
                    throw (new Exception(myErrorString));
            }
        }

        public string GetNameAndSerialNumber()
        {
            Int32 myErrorCode = 0;

            Int32 errorStringLength = SeaBreezeWrapper.seabreeze_get_error_string_maximum_length();
            byte[] errorStringBuffer = new byte[errorStringLength];

            Int32 serialNumberStringLength = SeaBreezeWrapper.seabreeze_get_serial_number_max_length(seaBreezeIndex, ref myErrorCode);
            byte[] mySerialNumberBuffer = new byte[serialNumberStringLength];

            int bytesInBuffer = SeaBreezeWrapper.seabreeze_get_serial_number(seaBreezeIndex, ref myErrorCode, ref mySerialNumberBuffer[0], serialNumberStringLength);
            if (myErrorCode != 0)
            {
                bytesInBuffer = SeaBreezeWrapper.seabreeze_get_error_string(myErrorCode, ref errorStringBuffer[0], errorStringLength);
                string myErrorString = "Exception getting model description: " + System.Text.Encoding.Default.GetString(errorStringBuffer);
                throw (new Exception(myErrorString));
            }

            Int32 modelStringLength = SeaBreezeWrapper.seabreeze_get_model_string_maximum_length();
            byte[] myModelTypeBuffer = new byte[modelStringLength];

            bytesInBuffer = SeaBreezeWrapper.seabreeze_get_model(seaBreezeIndex, ref myErrorCode, ref myModelTypeBuffer[0], modelStringLength);
            if (myErrorCode != 0)
            {
                bytesInBuffer = SeaBreezeWrapper.seabreeze_get_error_string(myErrorCode, ref errorStringBuffer[0], errorStringLength);
                string myErrorString = "Exception getting model description: " + System.Text.Encoding.Default.GetString(errorStringBuffer);
                throw (new Exception(myErrorString));
            }

            string name = System.Text.Encoding.UTF8.GetString(myModelTypeBuffer).TrimEnd((char)0);
            string serialNumber = System.Text.Encoding.UTF8.GetString(mySerialNumberBuffer).TrimEnd((char)0);
            return name + " " + serialNumber;
        }

    }
}
