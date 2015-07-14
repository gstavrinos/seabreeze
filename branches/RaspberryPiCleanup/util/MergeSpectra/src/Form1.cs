using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.Reflection;

namespace MergeSpectra
{
    public partial class Form1 : Form
    {
        ////////////////////////////////////////////////////////////////////////
        // Attributes
        ////////////////////////////////////////////////////////////////////////

        int pixels;

        List<double> wavelengths;
        SortedDictionary<string, List<double>> spectra;

        bool enableInterpolation;
        bool enableGraph = true;

        SortedDictionary<string, SortedDictionary<int, Tuple<double, double, double>>> peaks; // filename to pixel to (wavelength, resolution, height)

        Logger logger = Logger.getInstance();

        ////////////////////////////////////////////////////////////////////////
        // Lifecycle
        ////////////////////////////////////////////////////////////////////////

        public Form1()
        {
            InitializeComponent();

            logger.setTextBox(textBoxEventLog);
            checkBoxInterpolate_CheckedChanged(null, null);
            Text = String.Format("MergeSpectra v{0}", Assembly.GetExecutingAssembly().GetName().Version.ToString());
        }

        ////////////////////////////////////////////////////////////////////////
        // Callbacks
        ////////////////////////////////////////////////////////////////////////

        private void buttonSelectInputFiles_Click(object sender, EventArgs e)
        {
            DialogResult result = openFileDialogSelectInputFiles.ShowDialog();
            if (result != DialogResult.OK)
                return;

            listBoxInputFiles.Items.Clear();
            foreach (string pathname in openFileDialogSelectInputFiles.FileNames)
                listBoxInputFiles.Items.Add(pathname);

            labelFileCount.Text = String.Format("{0} files", listBoxInputFiles.Items.Count);
        }

        private void buttonMerge_Click(object sender, EventArgs e)
        {
            DialogResult result = saveFileDialogMergeResult.ShowDialog();
            if (result != DialogResult.OK)
                return;

            // init merge
            pixels = 0;
            spectra = new SortedDictionary<string, List<double>>();
            wavelengths = null;
            peaks = null;
            foreach (Series s in chartMerge.Series)
                s.Points.Clear();
            Refresh();

            // load and optionally graph each file
            foreach (string pathname in openFileDialogSelectInputFiles.FileNames)
            {
                string filename = Path.GetFileName(pathname);

                // read the raw file as-is
                SortedDictionary<double, double> spectrum = readFile(pathname);

                // generate the wavelengths (for ALL files) from the first file read
                if (wavelengths == null)
                {
                    wavelengths = generateWavelengths(ref spectrum);
                    pixels = wavelengths.Count;
                }

                // use the (possibly-interpolated) wavelengths to generate the actual intensities
                List<double> intensities = generateIntensities(ref wavelengths, ref spectrum);
                spectra.Add(filename, intensities);

                // peak-finding if requested
                if (enablePeakFinding)
                    generatePeaks(filename);
    
                // graph if requested
                if (enableGraph)
                    graphFile(filename);
            }

            // save result
            saveAll();
        }

        void generatePeaks(string filename)
        {
            if (peaks == null)
                peaks = new SortedDictionary<string, SortedDictionary<int, Tuple<double, double, double>>>();

            // initialize PeakFinder
            com.oceanoptics.spam.advancedprocessing.AdvancedPeakFinding peakFinder = 
                new com.oceanoptics.spam.advancedprocessing.AdvancedPeakFinding();

            com.oceanoptics.spam.advancedprocessing.SpectrumPeak[] results = 
                peakFinder.getAllPeaks(wavelengths.ToArray(), spectra[filename].ToArray(), minIndicesBetweenPeaks, baseline);

            SortedDictionary<int, Tuple<double, double, double>> thesePeaks = new SortedDictionary<int, Tuple<double, double, double>>();
            foreach (com.oceanoptics.spam.advancedprocessing.SpectrumPeak peak in results)
            {
                int index = peak.PixelNumber;
                double wavelength = peak.Centroid;
                double resolution = peak.WavelengthFWHM;
                double intensity = spectra[filename][index];

                thesePeaks.Add(index, new Tuple<double, double, double>(wavelength, resolution, intensity));
            }

            peaks.Add(filename, thesePeaks);
        }

        private void checkBoxInterpolate_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxInterpolate.Checked)
            {
                enableInterpolation = true;
                numericUpDownInterpolateStart.Enabled =
                    numericUpDownInterpolateEnd.Enabled =
                    numericUpDownInterpolateIncrement.Enabled = true;
            }
            else
            {
                enableInterpolation = false;
                numericUpDownInterpolateStart.Enabled =
                    numericUpDownInterpolateEnd.Enabled =
                    numericUpDownInterpolateIncrement.Enabled = false;
            }
        }

        private void checkBoxGraph_CheckedChanged(object sender, EventArgs e)
        {
            enableGraph = checkBoxGraph.Checked;
        }

        ////////////////////////////////////////////////////////////////////////
        // Methods
        ////////////////////////////////////////////////////////////////////////

        void graphFile(string filename)
        {
            if (filename == null || !spectra.ContainsKey(filename))
                return;

            Series s = chartMerge.Series.FindByName(filename);
            if (s == null)
            {
                s = new Series(filename);
                s.ChartType = SeriesChartType.Line;
                chartMerge.Series.Add(s);
            }

            s.Points.Clear();

            for (int i = 0; i < pixels; i++)
                s.Points.AddXY(wavelengths[i], spectra[filename][i]);

            chartMerge.ChartAreas[0].RecalculateAxesScale();
            Refresh();
        }

        SortedDictionary<double, double> readFile(string pathname)
        {
            SortedDictionary<double, double> spectrum = new SortedDictionary<double,double>();

            ////////////////////////////////////////////////////////////////////
            // Init delimiters
            ////////////////////////////////////////////////////////////////////

            List<char> delimiters = new List<char>();
            if (checkBoxDelimTabs.Checked)
                delimiters.Add('\t');
            if (checkBoxDelimComma.Checked)
                delimiters.Add(',');
            if (delimiters.Count < 1)
            {
                MessageBox.Show("No delimiters selected");
                return null;
            }
            char[] delimArray = delimiters.ToArray();

            ////////////////////////////////////////////////////////////////////
            // Read spectrum
            ////////////////////////////////////////////////////////////////////

            using (StreamReader sr = new StreamReader(pathname))
            {
                logger.display("reading {0}", pathname);

                string line;
                while ((line = sr.ReadLine()) != null)
                {
                    // skip blanks
                    if (line.Length < 1)
                        continue;

                    // only process lines starting with a digit or sign
                    if (Char.IsDigit(line[0]) || line[0] == '+' || line[0] == '-')
                    {
                        string[] tokens = line.Split(delimArray);

                        // typically first value is key (here called wavelength, could be wavenumber), 
                        // second is value (assumed intensity, could be absorbance etc).  Assume x-axis
                        // is column 0, and use mergeColumn for the Y-axis
                        if (tokens.Length < mergeColumn + 1)
                            continue;

                        double wavelength = Convert.ToDouble(tokens[0]);
                        double value = Convert.ToDouble(tokens[mergeColumn]);

                        spectrum.Add(wavelength, value);
                    }
                }
            }

            return spectrum;
        }

        List<double> generateWavelengths(ref SortedDictionary<double, double> spectrum)
        {
            wavelengths = new List<double>();
            if (enableInterpolation)
            {
                double wavelength = (double)numericUpDownInterpolateStart.Value;
                while (wavelength <= (double)numericUpDownInterpolateEnd.Value)
                {
                    wavelengths.Add(wavelength);
                    wavelength += (double)numericUpDownInterpolateIncrement.Value;
                }
            }
            else
            {
                foreach (KeyValuePair<double, double> pair in spectrum)
                    wavelengths.Add(pair.Key);
            }

            // just in case
            wavelengths.Sort();

            return wavelengths;
        }

        List<double> generateIntensities(ref List<double> wavelengths, ref SortedDictionary<double, double> spectrum)
        {
            List<double> values = new List<double>();
            if (enableInterpolation)
            {
                Interpolator interp = new Interpolator(spectrum);
                foreach (double wavelength in wavelengths)
                {
                    double interpolatedValue = interp.interpolateSequential(wavelength);
                    values.Add(interpolatedValue);
                }
            }
            else
            {
                // no-op...just copy over the raw y-values
                foreach (KeyValuePair<double, double> pair in spectrum)
                    values.Add(pair.Value);
            }

            return values;
        }

        void saveAll()
        {
            string pathname = saveFileDialogMergeResult.FileName;
            logger.display("Saving {0}", pathname);

            List<string> filenames = new List<string>(spectra.Keys);

            using (System.IO.StreamWriter file = new System.IO.StreamWriter(pathname))
            {
                // header row
                foreach (string filename in filenames)
                    file.Write(",{0}", filename);
                file.WriteLine();

                for (int pixel = 0; pixel < pixels; pixel++)
                {
                    if (filenames.Count > 50)
                        logger.display("saving pixel {0}", pixel);
                    file.Write("{0:f2}", wavelengths[pixel]);
                    foreach (string filename in filenames)
                        file.Write(",{0:f4}", spectra[filename][pixel]);
                    file.WriteLine();
                }

                if (peaks != null)
                {
                    file.WriteLine();
                    file.WriteLine("Peaks (files by row rather than column)");
                    foreach (string filename in filenames)
                    {
                        if (peaks.ContainsKey(filename))
                        {
                            file.WriteLine("{0}", filename);

                            file.Write("pixel");
                            foreach (KeyValuePair<int, Tuple<double, double, double>> peak in peaks[filename])
                                file.Write(",{0}", peak.Key);
                            file.WriteLine();

                            file.Write("wavelength");
                            foreach (KeyValuePair<int, Tuple<double, double, double>> peak in peaks[filename])
                                file.Write(",{0:f2}", peak.Value.Item1);
                            file.WriteLine();

                            file.Write("fwhm");
                            foreach (KeyValuePair<int, Tuple<double, double, double>> peak in peaks[filename])
                                file.Write(",{0:f2}", peak.Value.Item2);
                            file.WriteLine();

                            file.Write("intensity");
                            foreach (KeyValuePair<int, Tuple<double, double, double>> peak in peaks[filename])
                                file.Write(",{0:f2}", peak.Value.Item3);
                            file.WriteLine();

                            file.WriteLine();
                        }
                    }
                }
            }

            logger.display("done");
        }

        bool enablePeakFinding = false;
        private void checkBoxEnablePeakFinding_CheckedChanged(object sender, EventArgs e)
        {
            enablePeakFinding = checkBoxEnablePeakFinding.Checked;
        }

        int minIndicesBetweenPeaks = 1;
        private void numericUpDownMinIndicesBetweenPeaks_ValueChanged(object sender, EventArgs e)
        {
            minIndicesBetweenPeaks = (int) numericUpDownMinIndicesBetweenPeaks.Value;
        }

        double baseline = 0;
        private void numericUpDownBaseline_ValueChanged(object sender, EventArgs e)
        {
            baseline = (double) numericUpDownBaseline.Value;
        }

        uint mergeColumn = 1;  // zero-indexed, so zero is x-axis
        private void numericUpDownYAxisColumn_ValueChanged(object sender, EventArgs e)
        {
            mergeColumn = (uint)(numericUpDownYAxisColumn.Value - 1);
        }
    }
}
