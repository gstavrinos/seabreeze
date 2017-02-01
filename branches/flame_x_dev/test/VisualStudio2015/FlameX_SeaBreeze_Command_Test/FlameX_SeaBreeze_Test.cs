using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reflection;



namespace FlameX_SeaBreeze_Command_Test
{
    public partial class FlameX_SeaBreeze_Test : Form
    {

        SeaBreezeSpectrometerClass spectrometerUnderTest;
        Boolean isConnected = false;

        public FlameX_SeaBreeze_Test()
        {
            InitializeComponent();

            this.Text = String.Format("Flame X SeaBreeze Test (Version: {0})", Assembly.GetExecutingAssembly().GetName().Version.ToString());
        }

        private void buttonFindFlameX_Click(object sender, EventArgs e)
        {
            if (isConnected)
            {
                spectrometerUnderTest.Close();
                spectrometerUnderTest = null;
                buttonFindFlameX.Text = "Connect";
                isConnected = false;
            }
            else
            {
                listBoxTestLog.Items.Clear();
                spectrometerUnderTest = new SeaBreezeSpectrometerClass(listBoxTestLog, ref isConnected);
                if(isConnected)
                {
                    buttonFindFlameX.Text = "Disconnect";
                    spectrometerUnderTest.RunTests();
                }
            }

        }

        private void FlameX_SeaBreeze_Test_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (spectrometerUnderTest != null)
                spectrometerUnderTest.Close();
        }
    }
}
