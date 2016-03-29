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

        const int DEFAULT_PULSE_OFFSET_MS = 200;
        const int DEFAULT_PULSE_WIDTH_MS = 100;
        const int DEFAULT_PULSE_DELAY_MS = 1000;

        // convenience offsets into the DataGridView (I think there are string labels, oh well)
        const int CHANNEL_ID_COL = 0;
        const int CHANNEL_ENABLE_COL = 1;
        const int CHANNEL_OFFSET_COL = 2;
        const int CHANNEL_WIDTH_COL = 3;
        const int CHANNEL_INTRA_DELAY_COL = 4;

        const int GRAPH_UPDATE_DELAY_MS = 100;
        const int MAX_GRAPH_POINTS = 100;
        const int GRAPH_CHANNEL_HEIGHT = 10;
        const int GRAPH_CHANNEL_GAP = 5;

        ////////////////////////////////////////////////////////////////////////
        // Attributes
        ////////////////////////////////////////////////////////////////////////

        // channel settings
        List<PulseChannel> channels = new List<PulseChannel>();

        Logger logger = Logger.getInstance();
        SpectrometerSeaBreeze spectrometer = SpectrometerSeaBreeze.getInstance();

        bool running;               // is the generator currently running?
        bool keepLooping = true;    // should it re-loop after completing a sequence?

        ////////////////////////////////////////////////////////////////////////
        // Lifecycle
        ////////////////////////////////////////////////////////////////////////

        public Form1()
        {
            InitializeComponent();

            Text = String.Format("Pulse Generator v{0}", Assembly.GetExecutingAssembly().GetName().Version.ToString());

            logger.setTextBox(textBoxEventLog);
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
                buttonStart.Enabled = true;
            }
            logger.flush();
        }

        void initChannels()
        {
            for (int i = 0; i < MAX_CHANNELS; i++)
            {
                int offset = i * (DEFAULT_PULSE_OFFSET_MS + DEFAULT_PULSE_WIDTH_MS);

                dataGridViewPulses.Rows.Add(
                    i + 1,
                    true,
                    String.Format("{0}", offset),
                    String.Format("{0}", DEFAULT_PULSE_WIDTH_MS),
                    String.Format("{0}", DEFAULT_PULSE_DELAY_MS));

                BackgroundWorker worker = new BackgroundWorker();
                worker.WorkerReportsProgress = true;
                worker.WorkerSupportsCancellation = true;
                worker.DoWork += backgroundWorkerSequence_DoWork;
                worker.ProgressChanged += backgroundWorkerSequence_ProgressChanged;
                worker.RunWorkerCompleted += backgroundWorkerSequence_RunWorkerCompleted;

                channels.Add(new PulseChannel(i, worker));
            }

            initGraph();
            updatePulseData();
            updateGraphEditor();
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            if (!running)
            {
                buttonStart.Text = "Stop";
                initGraph();

                running = true;

                // tick graph update independent of actual channel pulses
                backgroundWorkerGraph.RunWorkerAsync();

                // spawn threads
                if (singleThreaded)
                    channels[0].worker.RunWorkerAsync(0);
                else
                    for (int i = 0; i < MAX_CHANNELS; i++)
                        if (channels[i].enabled)
                            channels[i].worker.RunWorkerAsync(i);
            }
            else
            {
                running = false;
                for (int i = 0; i < MAX_CHANNELS; i++)
                    channels[i].worker.CancelAsync();
                backgroundWorkerGraph.CancelAsync();
            }
        }

        private void dataGridViewPulses_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (channels.Count == 0)
                return;

            updatePulseData();
            updateGraphEditor();
        }

        private void checkBoxLoop_CheckedChanged(object sender, EventArgs e)
        {
            keepLooping = checkBoxLoop.Checked;
        }

        ////////////////////////////////////////////////////////////////////////
        // Graphing
        ////////////////////////////////////////////////////////////////////////

        void initGraph()
        {
            chartPulses.Series.Clear();

            for (int i = 0; i < MAX_CHANNELS; i++)
            {
                Series s = new Series(String.Format("Channel {0}", i + 1));
                s.ChartType = SeriesChartType.Line;
                chartPulses.Series.Add(s);

                channels[i].indexHigh = (MAX_CHANNELS - i) * (GRAPH_CHANNEL_HEIGHT + GRAPH_CHANNEL_GAP) + GRAPH_CHANNEL_HEIGHT;
                channels[i].indexLow = (MAX_CHANNELS - i) * (GRAPH_CHANNEL_HEIGHT + GRAPH_CHANNEL_GAP);
                channels[i].series = s;
            }
        }

        void updateGraphRunning()
        {
            for (int i = 0; i < MAX_CHANNELS; i++)
            {
                while (channels[i].series.Points.Count > MAX_GRAPH_POINTS)
                    channels[i].series.Points.RemoveAt(0);
                
                if (channels[i].state == PulseChannel.ChannelState.LEVEL)
                    channels[i].series.Points.AddY(channels[i].indexHigh);
                else
                    channels[i].series.Points.AddY(channels[i].indexLow);
            }
            Refresh();
        }

        void updateGraphEditor()
        {
            initGraph();

            // dynamically scale editor's x-axis to widest channel
            int maxMS = channels[0].initialOffsetMS + channels[0].widthMS;
            for (int i = 1; i < MAX_CHANNELS; i++)
            {
                int endOfPulse = channels[i].initialOffsetMS + channels[i].widthMS;
                if (maxMS < endOfPulse)
                    maxMS = endOfPulse;
            }

            // populate all series to the maximum width
            for (int i = 0; i < MAX_CHANNELS; i++)
            {
                if (channels[i].enabled && channels[i].series != null)
                {
                    // eat up the initial offset
                    int t = -10;
                    PulseChannel.ChannelState state = PulseChannel.ChannelState.OFFSET;
                    while (t < channels[i].initialOffsetMS)
                    {
                        channels[i].series.Points.AddY(channels[i].indexLow);
                        t++;
                    }

                    // process (pulse, delay) until we use up time
                    int stateTimerMS = 0;
                    state = PulseChannel.ChannelState.LEVEL;
                    while (t < maxMS + 10)
                    {
                        if (state == PulseChannel.ChannelState.LEVEL)
                        {
                            channels[i].series.Points.AddY(channels[i].indexHigh);
                            if (stateTimerMS >= channels[i].widthMS)
                            {
                                stateTimerMS = 0;
                                state = PulseChannel.ChannelState.DELAY;
                            }
                        }
                        else if (state == PulseChannel.ChannelState.DELAY)
                        {
                            channels[i].series.Points.AddY(channels[i].indexLow);
                            if (stateTimerMS >= channels[i].intraPulseDelayMS)
                            {
                                stateTimerMS = 0;
                                state = PulseChannel.ChannelState.LEVEL;
                            }
                        }
                        t++;
                        stateTimerMS++;
                    }
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        // DataGridView
        ////////////////////////////////////////////////////////////////////////

        // could simplify this with binding

        int getPulseWidthFromView(int i)
        {
            if (dataGridViewPulses.RowCount == 0 || i + 1 > dataGridViewPulses.RowCount) return 0;
            if (dataGridViewPulses.ColumnCount == 0 || CHANNEL_WIDTH_COL + 1 > dataGridViewPulses.ColumnCount) return 0;
            return Convert.ToInt16((string) dataGridViewPulses[CHANNEL_WIDTH_COL, i].Value);
        }

        int getPulseOffsetFromView(int i)
        {
            if (dataGridViewPulses.RowCount == 0 || i + 1 > dataGridViewPulses.RowCount) return 0;
            if (dataGridViewPulses.ColumnCount == 0 || CHANNEL_OFFSET_COL + 1 > dataGridViewPulses.ColumnCount) return 0;
            return Convert.ToInt16((string) dataGridViewPulses[CHANNEL_OFFSET_COL, i].Value);
        }

        int getIntraPulseDelayFromView(int i)
        {
            if (dataGridViewPulses.RowCount == 0 || i + 1 > dataGridViewPulses.RowCount) return 0;
            if (dataGridViewPulses.ColumnCount == 0 || CHANNEL_INTRA_DELAY_COL + 1 > dataGridViewPulses.ColumnCount) return 0;
            return Convert.ToInt16((string) dataGridViewPulses[CHANNEL_INTRA_DELAY_COL, i].Value);
        }

        bool getPulseEnabledFromView(int i)
        {
            if (dataGridViewPulses.RowCount == 0 || i + 1 > dataGridViewPulses.RowCount) return false;
            if (dataGridViewPulses.ColumnCount == 0 || CHANNEL_ENABLE_COL + 1 > dataGridViewPulses.ColumnCount) return false;
            return (bool) dataGridViewPulses[CHANNEL_ENABLE_COL, i].Value;
        }

        void updatePulseData()
        {
            if (channels.Count == 0)
                return;

            for (int i = 0; i < MAX_CHANNELS; i++)
            {
                channels[i].enabled = getPulseEnabledFromView(i);
                channels[i].widthMS = getPulseWidthFromView(i);
                channels[i].initialOffsetMS = getPulseOffsetFromView(i);
                channels[i].intraPulseDelayMS = getIntraPulseDelayFromView(i);
            }
        }

        ////////////////////////////////////////////////////////////////////////
        // BackgroundWorkerSequence
        ////////////////////////////////////////////////////////////////////////

        private void backgroundWorkerSequence_DoWork(object sender, DoWorkEventArgs e)
        {
            int channel = (int) e.Argument;
            BackgroundWorker worker = sender as BackgroundWorker;
            logger.queue("channel {0} starting", channel);

            ////////////////////////////////////////////////////////////
            // initial offset
            ////////////////////////////////////////////////////////////

            if (!singleThreaded)
            {
                channels[channel].state = PulseChannel.ChannelState.OFFSET;
                Thread.Sleep(channels[channel].initialOffsetMS);
            }

            ////////////////////////////////////////////////////////////
            // pulse loop
            ////////////////////////////////////////////////////////////

            uint pulseCount = 0;

            while (running)
            {
                if (singleThreaded)
                {
                    // advance to next active channel
                    channel = (int) (pulseCount++ % channels.Count);
                    if (!channels[channel].enabled)
                        continue;

                    // if we haven't done the initial offset yet, do it now
                    if (channel == pulseCount - 1)
                    {
                        channels[channel].state = PulseChannel.ChannelState.OFFSET;
                        Thread.Sleep(channels[channel].initialOffsetMS);
                    }
                }

                ////////////////////////////////////////////////////////////
                // start the pulse (rising edge)
                ////////////////////////////////////////////////////////////

                logger.queue("channel {0} -> high", channel);
                channels[channel].state = PulseChannel.ChannelState.LEVEL;
                bool ok = spectrometer.setGPIO(channel, true);
                if (!ok)
                    logger.queue("Error raising GPIO {0}", channel);

                ////////////////////////////////////////////////////////////
                // wait for pulse to complete (level)
                ////////////////////////////////////////////////////////////

                Thread.Sleep(channels[channel].widthMS);

                ////////////////////////////////////////////////////////////
                // end the pulse (falling edge)
                ////////////////////////////////////////////////////////////

                logger.queue("channel {0} -> low", channel);
                channels[channel].state = PulseChannel.ChannelState.DELAY;
                ok = spectrometer.setGPIO(channel, false);
                if (!ok)
                    logger.queue("Error lowering GPIO {0}", channel);

                if (worker.CancellationPending)
                {
                    e.Cancel = true;
                    break;
                }

                ////////////////////////////////////////////////////////////
                // wait for next pulse to start (intra-pulse delay)
                ////////////////////////////////////////////////////////////

                Thread.Sleep(channels[channel].intraPulseDelayMS);
                if (worker.CancellationPending)
                {
                    e.Cancel = true;
                    break;
                }

                if (!keepLooping)
                    break;
            }

            logger.queue("channel {0} done", channel);
        }

        private void backgroundWorkerSequence_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            int channel = (int) e.UserState;
        }

        private void backgroundWorkerSequence_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            logger.flush();
        }

        ////////////////////////////////////////////////////////////////////////
        // BackgroundWorkerGraph
        ////////////////////////////////////////////////////////////////////////

        private void backgroundWorkerGraph_DoWork(object sender, DoWorkEventArgs e)
        {
            logger.log("BackgroundWorkerGraph: starting");

            BackgroundWorker worker = sender as BackgroundWorker;
            int progressCount = 0;
            while (running)
            {
                worker.ReportProgress(progressCount++);
                Thread.Sleep(GRAPH_UPDATE_DELAY_MS);
                if (worker.CancellationPending)
                {
                    e.Cancel = true;
                    break;
                }

                bool oneStillRunning = false;
                for (int i = 0; i < MAX_CHANNELS; i++)
                {
                    if (channels[i].worker.IsBusy)
                    {
                        oneStillRunning = true;
                        break;
                    }
                }
                if (!oneStillRunning)
                    break;
            }
            logger.log("BackgroundWorkerGraph: done");
        }

        private void backgroundWorkerGraph_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            logger.flush();
            updateGraphRunning();
        }

        private void backgroundWorkerGraph_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            logger.flush();
            logger.log("BackgroundWorkerGraph: complete");
            buttonStart.Text = "Start";
            initGraph();
            updateGraphEditor();
        }

        bool singleThreaded;
        private void checkBoxSingleThread_CheckedChanged(object sender, EventArgs e)
        {
            singleThreaded = checkBoxSingleThread.Checked;
        }
    }
}
