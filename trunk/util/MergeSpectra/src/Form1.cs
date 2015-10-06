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
        // Data Types
        ////////////////////////////////////////////////////////////////////////

        class PeakInfo
        {
            public double wavelength;
            public double intensity;
            public double fwhm;

            public PeakInfo(double wavelength_in, double intensity_in, double fwhm_in)
            {
                wavelength = wavelength_in;
                intensity = intensity_in;
                fwhm = fwhm_in;
            }
        };

        enum PeakFindingAlgos { SPAM_ALGO, SOLVER_ALGO };

        ////////////////////////////////////////////////////////////////////////
        // Attributes
        ////////////////////////////////////////////////////////////////////////

        // merging
        int pixels;
        List<double> wavelengths;
        SortedDictionary<string, List<double>> spectra;
        bool enableGraph = true;
        bool enableInterpolation;

        uint xaxisColumn = 0;
        uint yaxisColumn = 1;  // zero-indexed, so zero is x-axis

        // peakfinding
        bool enablePeakFinding = false;
        int minIndicesBetweenPeaks = 1;
        double baseline = 0;
        int gaussianHalfWidth = 5;
        PeakFindingAlgos peakFindingAlgo = PeakFindingAlgos.SPAM_ALGO;
        SortedDictionary<string, SortedDictionary<int, PeakInfo>> peaks; 

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

            // update defaults from GUI
            numericUpDownBaseline_ValueChanged(null, null);
            numericUpDownGaussianHalfWidth_ValueChanged(null, null);
            numericUpDownXAxisColumn_ValueChanged(null, null);
            numericUpDownYAxisColumn_ValueChanged(null, null);
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

            updateMergeAllowed();
        }

        private void buttonMerge_Click(object sender, EventArgs e)
        {
            DialogResult result = saveFileDialogMergeResult.ShowDialog();
            if (result != DialogResult.OK)
                return;

            doMerge();

            // save result
            saveAll();
        }

        // need to increase code re-use with buttonMerge
        private void buttonPreview_Click(object sender, EventArgs e)
        {
            doMerge();

            PreviewForm preview = new PreviewForm();
            preview.Show();
            preview.textBoxSummary.Text = generatePeakSummary();
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

        private void checkBoxEnablePeakFinding_CheckedChanged(object sender, EventArgs e)
        {
            enablePeakFinding = checkBoxEnablePeakFinding.Checked;

            groupBoxSPAM.Enabled =
               groupBoxPeakFindingAlgo.Enabled = 
                enablePeakFinding;

            groupBoxSolver.Enabled = enablePeakFinding && peakFindingAlgo == PeakFindingAlgos.SOLVER_ALGO;
        }

        private void numericUpDownMinIndicesBetweenPeaks_ValueChanged(object sender, EventArgs e)
        {
            minIndicesBetweenPeaks = (int) numericUpDownMinIndicesBetweenPeaks.Value;
        }

        private void numericUpDownBaseline_ValueChanged(object sender, EventArgs e)
        {
            baseline = (double) numericUpDownBaseline.Value;
        }

        private void numericUpDownXAxisColumn_ValueChanged(object sender, EventArgs e)
        {
            xaxisColumn = (uint)(numericUpDownXAxisColumn.Value - 1);
            updateMergeAllowed();
        }

        private void numericUpDownYAxisColumn_ValueChanged(object sender, EventArgs e)
        {
            yaxisColumn = (uint)(numericUpDownYAxisColumn.Value - 1);
        }

        private void radioButtonLibrarySPAM_CheckedChanged(object sender, EventArgs e)
        {
            peakFindingAlgo = PeakFindingAlgos.SPAM_ALGO;
            groupBoxSolver.Enabled = false;
        }

        private void radioButtonLibrarySolver_CheckedChanged(object sender, EventArgs e)
        {
            peakFindingAlgo = PeakFindingAlgos.SOLVER_ALGO;
            groupBoxSolver.Enabled = true;
        }

        private void numericUpDownGaussianHalfWidth_ValueChanged(object sender, EventArgs e)
        {
            gaussianHalfWidth = (int) numericUpDownGaussianHalfWidth.Value;
        }

        ////////////////////////////////////////////////////////////////////////
        // Merging
        ////////////////////////////////////////////////////////////////////////

        void updateMergeAllowed()
        {
            buttonMerge.Enabled = xaxisColumn != yaxisColumn && listBoxInputFiles.Items.Count > 0;
        }

        string generatePeakSummary()
        {
            string s = "";
            if (peaks != null)
            {
                foreach (string filename in spectra.Keys)
                {
                    if (peaks.ContainsKey(filename))
                    {
                        s += filename + Environment.NewLine;
                        s += "pixel";
                        foreach (KeyValuePair<int, PeakInfo> peak in peaks[filename])
                            s += String.Format(", {0}", peak.Key);
                        s += Environment.NewLine;

                        s += String.Format("x-value({0})", comboBoxXUnit.Text);
                        foreach (KeyValuePair<int, PeakInfo> peak in peaks[filename])
                            s += String.Format(", {0:f2}", peak.Value.wavelength);
                        s += Environment.NewLine;

                        s += String.Format("FWHM({0})", comboBoxXUnit.Text);
                        foreach (KeyValuePair<int, PeakInfo> peak in peaks[filename])
                            s += String.Format(", {0:f2}", peak.Value.fwhm);
                        s += Environment.NewLine;

                        s += "intensity";
                        foreach (KeyValuePair<int, PeakInfo> peak in peaks[filename])
                            s += String.Format(", {0:f2}", peak.Value.intensity);
                        s += Environment.NewLine;
                        s += Environment.NewLine;
                    }
                }
            }
            return s;
        }

        void doMerge()
        {
            // init merge
            pixels = 0;
            spectra = new SortedDictionary<string, List<double>>();
            wavelengths = null;
            peaks = null;
            chartMerge.Series.Clear();
            chartMerge.Legends.Clear();
            chartMerge.Legends.Add(new Legend("Default"));
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
        }

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
                        // second is value (assumed intensity, could be absorbance etc).  
                        if (tokens.Length < xaxisColumn + 1 ||
                            tokens.Length < yaxisColumn + 1)
                            continue;

                        double wavelength = Convert.ToDouble(tokens[xaxisColumn]);
                        double value = Convert.ToDouble(tokens[yaxisColumn]);

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

        // This method is passed a spectrum (implemented as a SortedDictionary of (wavelength, intensity) pairs)
        // and a list of DESIRED wavelengths.  It returns a list of interpolated intensities, one per requested
        // wavelength.
        List<double> generateIntensities(ref List<double> wavelengths, ref SortedDictionary<double, double> spectrum)
        {
            List<double> values = new List<double>();

            // has the user indicated that they wish to interpolate the spectrom?
            if (enableInterpolation)
            {
                // yes, we've been asked to interpolate new intensities from the passed spectrum,
                // generating one per requested wavelength
                Interpolator interp = new Interpolator(spectrum);
                foreach (double wavelength in wavelengths)
                {
                    double interpolatedValue = interp.interpolate(wavelength);
                    values.Add(interpolatedValue);
                }
            }
            else
            {
                // no-op...just copy over the raw y-values (note: we're not even looking at
                // the "wavelengths" argument in this use-case)
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
                    string summary = generatePeakSummary();
                    file.WriteLine();
                    file.WriteLine("Peaks (files by row rather than column)");
                    file.WriteLine(summary);
                }
            }

            logger.display("done");
        }

        ////////////////////////////////////////////////////////////////////////
        // Peakfinding
        ////////////////////////////////////////////////////////////////////////

        void generatePeaks(string filename)
        {
            if (peaks == null)
                peaks = new SortedDictionary<string, SortedDictionary<int, PeakInfo>>();
            
            // yes, this should be subclassed
            if (peakFindingAlgo == PeakFindingAlgos.SPAM_ALGO)
                generatePeaksSPAM(filename);
            else if (peakFindingAlgo == PeakFindingAlgos.SOLVER_ALGO)
                generatePeaksSolver(filename);
        }

        // both SPAM and Solver resolution-calculation algorithms use SPAM to find the peaks,
        // so break this out separately
        SortedDictionary<int, PeakInfo> findPeaksSPAM(string filename)
        {
            com.oceanoptics.spam.advancedprocessing.AdvancedPeakFinding peakFinder = 
                new com.oceanoptics.spam.advancedprocessing.AdvancedPeakFinding();

            com.oceanoptics.spam.advancedprocessing.SpectrumPeak[] results = 
                peakFinder.getAllPeaks(wavelengths.ToArray(), spectra[filename].ToArray(), minIndicesBetweenPeaks, baseline);

            SortedDictionary<int, PeakInfo> thesePeaks = new SortedDictionary<int, PeakInfo>();
            foreach (com.oceanoptics.spam.advancedprocessing.SpectrumPeak peak in results)
            {
                int pixel = peak.PixelNumber;

                // Centroid is problematic...tail must drop below 5% of peak height INCLUDING baseline, hard to achieve on closely-spaced peaks.
                double wavelength = peak.CenterWavelength; 
                double intensity = spectra[filename][pixel];
                double fwhm = peak.WavelengthFWHM;

                thesePeaks.Add(pixel, new PeakInfo(wavelength, intensity, fwhm));
            }
            return thesePeaks;
        }

        void generatePeaksSPAM(string filename)
        {
            SortedDictionary<int, PeakInfo> thesePeaks = findPeaksSPAM(filename);
            peaks.Add(filename, thesePeaks);
        }

        void generatePeaksSolver(string filename)
        {
            SortedDictionary<int, PeakInfo> thesePeaks = findPeaksSPAM(filename);

            foreach (KeyValuePair<int, PeakInfo> peak in thesePeaks)
            {
                int centerPixel = peak.Key;
                PeakInfo pi = peak.Value;

                // grab one half-width's worth pixels to either side of centroid 
                if (centerPixel < gaussianHalfWidth || centerPixel + gaussianHalfWidth >= pixels)
                {
                    logger.display("Warning: can't create {0}-pixel gaussian on peak centered at {1}", gaussianHalfWidth * 2 + 1, centerPixel);
                    continue; // leave SPAM resolution
                }

                // determine initial guesses
                double guessIntensity = spectra[filename][centerPixel];
                double guessWavelength = pi.wavelength;
                double guessWidth = 1.0;    // MZ: in Excel this converged fine (don't use zero)
                double guessBaseline = 0.0; // MZ: in Excel this worked fine

                GaussianSolver solver = new GaussianSolver(guessIntensity, guessWavelength, guessWidth, guessBaseline);

                // Load the peak's local spectrum into the Solver.
                //
                // Note: the choice of which peaks to load into the Solver is significant in 
                // the Guassian curve generated, and therefore the calculated FWHM.  We could:
                // 
                // 1. Simply hard-code a half-width of 'n' pixels (fast, deterministic, simple, but may
                //    not reach all the way to the true baseline and therefore understates resolution).
                // 2. Do something clever like stop where slope crosses 0.5 or 0.
                //
                // For now, just let user pick a half-width.
                //
                for (int i = 0; i < gaussianHalfWidth * 2 + 1; i++)
                {
                    int pixel = centerPixel - gaussianHalfWidth + i;
                    solver.addPoint(wavelengths[pixel], spectra[filename][pixel]);
                }

                double resolution = solver.computeFWHM();
                if (resolution < pi.fwhm * 2)
                {
                    logger.display("replacing SPAM fwhm {0:f4} with gaussian {1:f4}", pi.fwhm, resolution);
                    pi.fwhm = resolution;
                }
                else
                {
                    logger.display("NOT replacing SPAM fwhm {0:f4} with gaussian {1:f4} (something seems wonky)", pi.fwhm, resolution);
                }
            }

            peaks.Add(filename, thesePeaks);
        }

    }
}