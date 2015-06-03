using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Windows.Forms.DataVisualization.Charting;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Reflection;

namespace PulseGenerator
{
    public partial class Form1 : Form
    {
        ////////////////////////////////////////////////////////////////////////
        // Constants
        ////////////////////////////////////////////////////////////////////////

        const int MAX_CHANNELS = 8;                     // TODO: lookup from model

        const int DEFAULT_PULSE_WIDTH_MS = 100;
        const int DEFAULT_PULSE_INTERVAL_MS = 150;

        // convenience offsets into the DataGridView
        const int PULSE_ID_COL = 0;
        const int PULSE_ENABLE_COL = 1;
        const int PULSE_OFFSET_COL = 2;
        const int PULSE_WIDTH_COL = 3;

        // these really don't matter
        const int GRAPH_CHANNEL_HEIGHT = 10;
        const int GRAPH_CHANNEL_GAP = 5;

        ////////////////////////////////////////////////////////////////////////
        // Attributes
        ////////////////////////////////////////////////////////////////////////

        // local (non-GUI) storage of channel settings
        Dictionary<int, bool> pulseEnabled;
        Dictionary<int, int>  pulseWidth;
        Dictionary<int, int>  pulseOffset;

        Logger logger = Logger.getInstance();
        SpectrometerSeaBreeze spectrometer = SpectrometerSeaBreeze.getInstance();

        bool running;           // is the generator currently running?
        bool keepLooping;       // should it re-loop after completing a sequence?

        ////////////////////////////////////////////////////////////////////////
        // Lifecycle
        ////////////////////////////////////////////////////////////////////////

        public Form1()
        {
            InitializeComponent();

            Text = String.Format("Pulse Generator v{0}", Assembly.GetExecutingAssembly().GetName().Version.ToString());

            logger.setTextBox(textBoxEventLog);
        }

        void initChannels()
        {
            pulseEnabled = new Dictionary<int, bool>();
            pulseWidth = new Dictionary<int, int>();
            pulseOffset = new Dictionary<int, int>();

            for (int i = 0; i < MAX_CHANNELS; i++)
            {
                int offset = i * (DEFAULT_PULSE_INTERVAL_MS + DEFAULT_PULSE_WIDTH_MS);
                dataGridViewPulses.Rows.Add(
                    i + 1,
                    true,
                    String.Format("{0}", offset),
                    String.Format("{0}", DEFAULT_PULSE_WIDTH_MS));
            }

            updatePulseData();
            updateGraph();
        }

        ////////////////////////////////////////////////////////////////////////
        // Callbacks
        ////////////////////////////////////////////////////////////////////////

        private void buttonInit_Click(object sender, EventArgs e)
        {
            if (spectrometer.init())
            {
                logger.flush();
                initChannels();

                logger.flush();
                buttonStart.Enabled = true;
            }
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            if (!running)
            {
                buttonStart.Text = "Stop";
                backgroundWorkerSequence.RunWorkerAsync();
            }
            else
            {
                running = false;
                backgroundWorkerSequence.CancelAsync();
            }
        }

        private void dataGridViewPulses_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            updatePulseData();
            updateGraph();
        }

        private void checkBoxLoop_CheckedChanged(object sender, EventArgs e)
        {
            keepLooping = checkBoxLoop.Checked;
        }

        ////////////////////////////////////////////////////////////////////////
        // Graphing
        ////////////////////////////////////////////////////////////////////////

        void updateGraph()
        {
            int maxMS = 0;
            for (int i = 0; i < MAX_CHANNELS; i++)
            {
                bool enabled = getPulseEnabled(i);
                int offset = getPulseOffset(i);
                int width = getPulseWidth(i);

                int endOfPulse = offset + width;
                if (maxMS < endOfPulse)
                    maxMS = endOfPulse;
            }

            chartPulses.Series.Clear();

            for (int i = 0; i < MAX_CHANNELS; i++)
            {
                bool enabled = getPulseEnabled(i);
                if (enabled)
                {
                    int offset = getPulseOffset(i);
                    int width = getPulseWidth(i);

                    Series s = new Series(String.Format("Channel {0}", i + 1));
                    s.ChartType = SeriesChartType.Line;

                    int indexHigh = (MAX_CHANNELS - i) * (GRAPH_CHANNEL_HEIGHT + GRAPH_CHANNEL_GAP) + GRAPH_CHANNEL_HEIGHT;
                    int indexLow = (MAX_CHANNELS - i) * (GRAPH_CHANNEL_HEIGHT + GRAPH_CHANNEL_GAP);

                    for (int t = -10; t < maxMS + 10; t++)
                    {
                        if (t < offset)
                            s.Points.AddY(indexLow);
                        else if (t < offset + width)
                            s.Points.AddY(indexHigh);
                        else
                            s.Points.AddY(indexLow);
                    }

                    chartPulses.Series.Add(s);
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        // DataGridView
        ////////////////////////////////////////////////////////////////////////

        int  getPulseWidth  (int i) { return pulseWidth  .ContainsKey(i) ? pulseWidth  [i] : 0; }
        int  getPulseOffset (int i) { return pulseOffset .ContainsKey(i) ? pulseOffset [i] : 0; }
        bool getPulseEnabled(int i) { return pulseEnabled.ContainsKey(i) ? pulseEnabled[i] : false; }

        int getPulseWidthFromView(int i)
        {
            if (dataGridViewPulses.RowCount == 0 || i + 1 > dataGridViewPulses.RowCount)
                return 0;
            if (dataGridViewPulses.ColumnCount == 0 || PULSE_WIDTH_COL + 1 > dataGridViewPulses.ColumnCount)
                return 0;
            string s = (string) dataGridViewPulses[PULSE_WIDTH_COL, i].Value;
            return Convert.ToInt16(s);
        }

        int getPulseOffsetFromView(int i)
        {
            if (dataGridViewPulses.RowCount == 0 || i + 1 > dataGridViewPulses.RowCount)
                return 0;
            if (dataGridViewPulses.ColumnCount == 0 || PULSE_OFFSET_COL + 1 > dataGridViewPulses.ColumnCount)
                return 0;
            string s = (string) dataGridViewPulses[PULSE_OFFSET_COL, i].Value;
            return Convert.ToInt16(s);
        }

        bool getPulseEnabledFromView(int i)
        {
            if (dataGridViewPulses.RowCount == 0 || i + 1 > dataGridViewPulses.RowCount)
                return false;
            if (dataGridViewPulses.ColumnCount == 0 || PULSE_ENABLE_COL + 1 > dataGridViewPulses.ColumnCount)
                return false;
            bool flag = (bool) dataGridViewPulses[PULSE_ENABLE_COL, i].Value;
            return flag;
        }

        void updatePulseData()
        {
            for (int i = 0; i < MAX_CHANNELS; i++)
            {
                if (pulseEnabled == null)
                    pulseEnabled = new Dictionary<int, bool>();
                if (pulseEnabled.ContainsKey(i))
                    pulseEnabled[i] = getPulseEnabledFromView(i);
                else
                    pulseEnabled.Add(i, getPulseEnabledFromView(i));

                if (pulseWidth == null)
                    pulseWidth = new Dictionary<int, int>();
                if (pulseWidth.ContainsKey(i))
                    pulseWidth[i] = getPulseWidthFromView(i);
                else
                    pulseWidth.Add(i, getPulseWidthFromView(i));

                if (pulseOffset == null)
                    pulseOffset = new Dictionary<int, int>();
                if (pulseOffset.ContainsKey(i))
                    pulseOffset[i] = getPulseOffsetFromView(i);
                else
                    pulseOffset.Add(i, getPulseOffsetFromView(i));
            }
        }

        ////////////////////////////////////////////////////////////////////////
        // BackgroundWorkerSequence
        ////////////////////////////////////////////////////////////////////////

        private void backgroundWorkerSequence_DoWork(object sender, DoWorkEventArgs e)
        {
            logger.queue("BackgroundWorkerSequence: starting");

            BackgroundWorker worker = sender as BackgroundWorker;

            running = true;

            int loopCount = 0;
            int progressCount = 0;
            do
            {
                // Okay, this all needs re-written for general use, but it's
                // enough for my current need.  Final version will need much 
                // better control, e.g. a separate BackgroundWorker for each 
                // channel, plus look into whatever event-triggered high-
                // resolution timing is available from .NET.

                logger.queue("Loop #{0}", loopCount++);
                for (int i = 0; i < MAX_CHANNELS; i++)
                {
                    if (!getPulseEnabled(i))
                    {
                        logger.queue("  Channel #{0} not enabled", i + 1);
                        continue;
                    }
                    else
                    {
                        logger.queue("  Channel #{0}", i + 1);
                    }

                    ////////////////////////////////////////////////////////////
                    // pre-pulse delay
                    ////////////////////////////////////////////////////////////

                    int offset = getPulseOffset(i);
                    if (i > 0)
                        offset -= getPulseOffset(i - 1);

                    // delay the computed offset
                    Thread.Sleep(offset);
                    if (worker.CancellationPending)
                    {
                        e.Cancel = true;
                        logger.queue("BackgroundWorkerSequence: cancelled");
                        break;
                    }
                    worker.ReportProgress(progressCount++);

                    ////////////////////////////////////////////////////////////
                    // start the pulse (rising edge)
                    ////////////////////////////////////////////////////////////

                    logger.queue("    raising GPIO {0}", i);
                    bool ok = spectrometer.setGPIO(i, true);
                    if (!ok)
                    {
                        logger.queue("Error raising GPIO on loop {0}, channel {1}", loopCount, i);
                        break;
                    }
                    worker.ReportProgress(progressCount++);

                    ////////////////////////////////////////////////////////////
                    // wait for pulse to complete (level)
                    ////////////////////////////////////////////////////////////

                    int width = getPulseWidth(i);
                    Thread.Sleep(width);
                    if (worker.CancellationPending)
                    {
                        e.Cancel = true;
                        logger.queue("BackgroundWorkerSequence: cancelled");
                        break;
                    }

                    ////////////////////////////////////////////////////////////
                    // end the pulse (falling edge)
                    ////////////////////////////////////////////////////////////

                    logger.queue("    lowering GPIO {0}", i);
                    ok = spectrometer.setGPIO(i, false);
                    if (!ok)
                    {
                        logger.queue("Error lowering GPIO on loop {0}, channel {1}", loopCount, i);
                        break;
                    }
                    worker.ReportProgress(progressCount++);
                }
            } while (keepLooping && running);

            logger.queue("BackgroundWorkerSequence: done");
        }

        private void backgroundWorkerSequence_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            logger.flush();
        }

        private void backgroundWorkerSequence_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            logger.flush();
            logger.display("Sequence ended");
            running = false;
            buttonStart.Text = "Start";
        }
    }
}
