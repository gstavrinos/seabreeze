using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FlameX_SeaBreeze_Command_Test 
{
    class QEPROTestClass : TestClass
    {

        public QEPROTestClass(ListBox lb) : base(lb)
        {

            Log("QE Pro specific commands have not yet been implemented.");

        }

        public override void RunSpectrometerSpecificTests()
        {
            Log("QE PRO specific tests go here");
        }

    }
}
