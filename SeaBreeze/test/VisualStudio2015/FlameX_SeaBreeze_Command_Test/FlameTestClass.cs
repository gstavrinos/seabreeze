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

        public FlameTestClass(ListBox lb) : base(lb)
        {

            Log("Executed Command: 0x00110220  Get number of pixels");
            Log("Executed Command: 0x00110221  Get active pixel ranges");
            Log("Executed Command: 0x00110222  Get optical dark pixel ranges");
            Log("Executed Command: 0x00110223  Get electrical dark pixel ranges");

        }

        public override void RunSpectrometerSpecificTests()
        {
            Log("flame test");
        }

    }
}
