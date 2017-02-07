using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FlameX_SeaBreeze_Command_Test 
{
    class FlameTestClass : TestClass
    {
        private int mySpectrometerID = 0;

        public FlameTestClass(ListBox lb, int spectrometerID) : base(lb)
        {
            int errorCode = 0;
            const int maxIndiciesCount = 32;
            int [] pixelIndicies = new int[maxIndiciesCount];
            uint numberOfPixels = 0;

            // these commands are called when the spectrometer is initialized
            Log("The Flame X sends introspective commands during initialization.");

            mySpectrometerID = spectrometerID;
            // even though these were called in the initialization, call again to display values.
            numberOfPixels = SeaBreezeWrapper.seabreeze_get_number_of_pixels(spectrometerID, ref errorCode);
            if (errorCode == SeaBreezeWrapper.ERROR_SUCCESS)
            {
                Log("Executed Command: 0x00110220  Get number of pixels. "+numberOfPixels.ToString());
            }
            else
            {
                Log("Failure getting pixel count: " + GetErrorDescription(errorCode));
            }

            int numberOfIndicies = SeaBreezeWrapper.seabreeze_get_active_pixel_indices(spectrometerID, ref errorCode, ref pixelIndicies[0], 32);
            if (errorCode == SeaBreezeWrapper.ERROR_SUCCESS)
            {
                string indicies = "";
                for(int x = 0; x<numberOfIndicies; x=x+2)
                {
                    indicies = indicies + "(" + pixelIndicies[x].ToString() + ", " + pixelIndicies[x + 1].ToString() + ") ";
                }
                Log("Executed Command: 0x00110221  Get active pixel ranges:" + indicies);
            }
            else
            {
                Log("Failure getting pixel count: " + GetErrorDescription(errorCode));
            }

            int opticalDarkIndex = SeaBreezeWrapper.seabreeze_get_optical_dark_pixel_indices(spectrometerID, ref errorCode, ref pixelIndicies[0], 32);
            if (errorCode == SeaBreezeWrapper.ERROR_SUCCESS)
            {
                string indicies = "";
                for (int x = 0; x < opticalDarkIndex; x = x + 2)
                {
                    indicies = indicies + "(" + pixelIndicies[x].ToString() + ", " + pixelIndicies[x + 1].ToString() + ") ";
                }
                Log("Executed Command: 0x00110222  Get optical dark pixel ranges" + indicies);
            }
            else
            {
                Log("Failure getting pixel count: " + GetErrorDescription(errorCode));
            }

            int electricDarkPixelIndex = SeaBreezeWrapper.seabreeze_get_electric_dark_pixel_indices(spectrometerID, ref errorCode, ref pixelIndicies[0], 32);
            if (errorCode == SeaBreezeWrapper.ERROR_SUCCESS)
            {
                string indicies = "";
                for (int x = 0; x < electricDarkPixelIndex; x = x + 2)
                {
                    indicies = indicies + "(" + pixelIndicies[x].ToString() + ", " + pixelIndicies[x + 1].ToString() + ") ";
                }
                Log("Executed Command: 0x00110223  Get electrical dark pixel ranges" + indicies);

            }
            else
            {
                Log("Failure getting pixel count: " + GetErrorDescription(errorCode));
            }

            
        }

        public override void RunSpectrometerSpecificTests()
        {
            Log("flame test");
        }

    }
}
