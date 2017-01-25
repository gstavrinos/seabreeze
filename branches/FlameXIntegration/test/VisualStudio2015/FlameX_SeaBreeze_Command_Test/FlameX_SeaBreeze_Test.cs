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

        List<SeaBreezeSpectrometerClass> spectrometerList = new List<SeaBreezeSpectrometerClass>();

        public FlameX_SeaBreeze_Test()
        {
            InitializeComponent();

            this.Text = String.Format("Flame X SeaBreeze Test (Version: {0})", Assembly.GetExecutingAssembly().GetName().Version.ToString());
        }

        private void buttonFindFlameX_Click(object sender, EventArgs e)
        {
            int index = 0;
            int zero_on_success = 0;
            int errorCode = 0;

            do
            {
                zero_on_success = SeaBreezeWrapper.seabreeze_open_spectrometer(index, ref errorCode);
                if ((zero_on_success == 0) && (errorCode == 0))
                {
                    spectrometerList.Add(new SeaBreezeSpectrometerClass(index));
                    index++;
                }
            } while (zero_on_success == 0);
            
            foreach(SeaBreezeSpectrometerClass aSpectrometer in spectrometerList)
            {
                string spectrometerID = aSpectrometer.GetNameAndSerialNumber();
                if (spectrometerID.Contains("FLAMEX"))
                    listBoxFlameXFound.Items.Add(spectrometerID);
                else
                    aSpectrometer.Close();
            }
        }
    }
}
