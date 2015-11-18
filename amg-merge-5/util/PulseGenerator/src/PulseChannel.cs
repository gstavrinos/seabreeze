using System;
using System.Collections.Generic;
using System.Windows.Forms.DataVisualization.Charting;
using System.Text;
using System.ComponentModel;

namespace PulseGenerator
{
    class PulseChannel
    {
        public enum ChannelState { OFFSET, LEVEL, DELAY };

        public int pulseID;

        public bool enabled;
        public int widthMS;
        public int initialOffsetMS;
        public int intraPulseDelayMS;
        public ChannelState state;

        // inelegant but simple
        public Series series;
        public int indexHigh;
        public int indexLow;

        public BackgroundWorker worker;

        public PulseChannel(int id, BackgroundWorker bw)
        {
            pulseID = id;
            worker = bw;
            enabled = true;
            state = ChannelState.OFFSET;
        }
    }
}
