using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FlameX_SeaBreeze_Command_Test 
{
    class STSTestClass : TestClass
    {

        public STSTestClass(ListBox lb) : base(lb)
        {
        	Log("STS specific commands have not yet been implemented");
        }

        public override void RunSpectrometerSpecificTests()
        {
            Log("STS specific tests go here");
        }

    }
}
