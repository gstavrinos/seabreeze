using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Windows.Forms.DataVisualization.Charting;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace PulseGenerator
{
    public partial class Form1 : Form
    {
        const int MAX_CHANNELS = 8;

        const int DEFAULT_PULSE_WIDTH_MS = 10;
        const int DEFAULT_PULSE_INTERVAL_MS = 10;

        const int PULSE_ID_COL = 0;
        const int PULSE_ENABLE_COL = 1;
        const int PULSE_OFFSET_COL = 2;
        const int PULSE_WIDTH_COL = 3;

        const int GRAPH_CHANNEL_HEIGHT = 10;
        const int GRAPH_CHANNEL_GAP = 5;

        Dictionary<int, List<int>> graphData;
        Logger logger = Logger.getInstance();
        SpectrometerSeaBreeze spectrometer = SpectrometerSeaBreeze.getInstance();
        public static Thread uiThread;

        bool running;
        bool keepLooping;

        public Form1()
        {
            InitializeComponent();

            uiThread = Thread.CurrentThread;

            logger.setTextBox(textBoxEventLog);
        }

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
                backgroundWorkerSequence.CancelAsync();
            }

        }

        void initChannels()
        {
            graphData = new Dictionary<int, List<int>>();

            for (int i = 0; i < MAX_CHANNELS; i++)
            {
                int offset = i * (DEFAULT_PULSE_INTERVAL_MS + DEFAULT_PULSE_WIDTH_MS);
                dataGridViewPulses.Rows.Add(
                    i + 1,
                    true,
                    String.Format("{0}", offset),
                    String.Format("{0}", DEFAULT_PULSE_WIDTH_MS));

            }

            updateGraph();
        }

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

        int getPulseWidth(int i)
        {
            if (dataGridViewPulses.RowCount == 0 || i + 1 >= dataGridViewPulses.RowCount)
                return 0;
            if (dataGridViewPulses.ColumnCount == 0 || PULSE_WIDTH_COL + 1 > dataGridViewPulses.ColumnCount)
                return 0;
            string s = (string) dataGridViewPulses[PULSE_WIDTH_COL, i].Value;
            return Convert.ToInt16(s);
        }

        int getPulseOffset(int i)
        {
            if (dataGridViewPulses.RowCount == 0 || i + 1 >= dataGridViewPulses.RowCount)
                return 0;
            if (dataGridViewPulses.ColumnCount == 0 || PULSE_OFFSET_COL + 1 > dataGridViewPulses.ColumnCount)
                return 0;
            string s = (string) dataGridViewPulses[PULSE_OFFSET_COL, i].Value;
            return Convert.ToInt16(s);
        }

        bool getPulseEnabled(int i)
        {
            if (dataGridViewPulses.RowCount == 0 || i + 1 >= dataGridViewPulses.RowCount)
                return false;
            if (dataGridViewPulses.ColumnCount == 0 || PULSE_ENABLE_COL + 1 > dataGridViewPulses.ColumnCount)
                return false;
            bool flag = (bool) dataGridViewPulses[PULSE_ENABLE_COL, i].Value;
            return flag;
        }

        private void dataGridViewPulses_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            updateGraph();
        }

        ////////////////////////////////////////////////////////////////////////
        // BackgroundWorkerSequence
        ////////////////////////////////////////////////////////////////////////

        private void backgroundWorkerSequence_DoWork(object sender, DoWorkEventArgs e)
        {
            logger.queue("BackgroundWorkerSequence: starting");

            BackgroundWorker worker = sender as BackgroundWorker;

            running = true;

            int t = 0;      // TODO: use actual DateTime
            int loopCount = 0;
            int progressCount = 0;
            do
            {
                t = 0;

                // FOR NOW, JUST RUN EACH CHANNEL ONCE IN TURN...FINAL VERSION WILL NEED MUCH MORE CONTROL
                logger.queue("________________________________________________________");
                logger.queue("Loop #{0}", loopCount++);
                for (int i = 0; i < MAX_CHANNELS; i++)
                {
                    if (!getPulseEnabled(i))
                        continue;

                    logger.queue(" ");
                    logger.queue("Channel #{0}", i + 1);
                    
                    int offset = getPulseOffset(i);

                    if (offset > t)
                    {
                        int sleepMS = offset - t;
                        logger.queue("sleeping {0} ms", sleepMS);
                        Thread.Sleep(sleepMS);

                        if (worker.CancellationPending)
                        {
                            e.Cancel = true;
                            logger.queue("BackgroundWorkerSequence: cancelled");
                            break;
                        }

                        worker.ReportProgress(progressCount++);

                        t += offset;
                    }

                    logger.queue("raising GPIO {0}", i);
                    bool ok = spectrometer.setGPIO(i, true);
                    if (!ok)
                    {
                        logger.queue("Error raising GPIO on loop {0}, channel {1}", loopCount, i);
                        break;
                    }
                    Thread.Sleep(5);
                    worker.ReportProgress(progressCount++);

                    int width = getPulseWidth(i);
                    Thread.Sleep(width);
                    t += width;

                    if (worker.CancellationPending)
                    {
                        e.Cancel = true;
                        logger.queue("BackgroundWorkerSequence: cancelled");
                        break;
                    }

                    logger.queue("lowering GPIO {0}", i);
                    ok = spectrometer.setGPIO(i, false);
                    Thread.Sleep(5);
                    if (!ok)
                    {
                        logger.queue("Error lowering GPIO on loop {0}, channel {1}", loopCount, i);
                        break;
                    }
                    worker.ReportProgress(progressCount++);
                }
            } while (keepLooping);

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

        private void checkBoxLoop_CheckedChanged(object sender, EventArgs e)
        {
            keepLooping = checkBoxLoop.Checked;
        }

    }
}
