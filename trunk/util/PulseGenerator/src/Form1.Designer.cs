namespace PulseGenerator
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            this.splitContainerControlsVsGraph = new System.Windows.Forms.SplitContainer();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.splitContainerSequenceVsChannels = new System.Windows.Forms.SplitContainer();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.buttonInit = new System.Windows.Forms.Button();
            this.checkBoxLoop = new System.Windows.Forms.CheckBox();
            this.buttonStart = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.dataGridViewPulses = new System.Windows.Forms.DataGridView();
            this.ChannelID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ChannelEnable = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.ChannelOffsetMS = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ChannelPulseWidth = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.intraPulseDelayMS = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.chartPulses = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.splitContainerControlsGraphVsLog = new System.Windows.Forms.SplitContainer();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.textBoxEventLog = new System.Windows.Forms.TextBox();
            this.backgroundWorkerSequence = new System.ComponentModel.BackgroundWorker();
            this.backgroundWorkerGraph = new System.ComponentModel.BackgroundWorker();
            this.checkBoxSingleThread = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerControlsVsGraph)).BeginInit();
            this.splitContainerControlsVsGraph.Panel1.SuspendLayout();
            this.splitContainerControlsVsGraph.Panel2.SuspendLayout();
            this.splitContainerControlsVsGraph.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerSequenceVsChannels)).BeginInit();
            this.splitContainerSequenceVsChannels.Panel1.SuspendLayout();
            this.splitContainerSequenceVsChannels.Panel2.SuspendLayout();
            this.splitContainerSequenceVsChannels.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewPulses)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartPulses)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerControlsGraphVsLog)).BeginInit();
            this.splitContainerControlsGraphVsLog.Panel1.SuspendLayout();
            this.splitContainerControlsGraphVsLog.Panel2.SuspendLayout();
            this.splitContainerControlsGraphVsLog.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainerControlsVsGraph
            // 
            this.splitContainerControlsVsGraph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerControlsVsGraph.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainerControlsVsGraph.Location = new System.Drawing.Point(0, 0);
            this.splitContainerControlsVsGraph.Name = "splitContainerControlsVsGraph";
            // 
            // splitContainerControlsVsGraph.Panel1
            // 
            this.splitContainerControlsVsGraph.Panel1.Controls.Add(this.groupBox2);
            // 
            // splitContainerControlsVsGraph.Panel2
            // 
            this.splitContainerControlsVsGraph.Panel2.Controls.Add(this.chartPulses);
            this.splitContainerControlsVsGraph.Size = new System.Drawing.Size(1005, 388);
            this.splitContainerControlsVsGraph.SplitterDistance = 277;
            this.splitContainerControlsVsGraph.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.splitContainerSequenceVsChannels);
            this.groupBox2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox2.Location = new System.Drawing.Point(0, 0);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(277, 388);
            this.groupBox2.TabIndex = 0;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Controls";
            // 
            // splitContainerSequenceVsChannels
            // 
            this.splitContainerSequenceVsChannels.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerSequenceVsChannels.Location = new System.Drawing.Point(3, 16);
            this.splitContainerSequenceVsChannels.Name = "splitContainerSequenceVsChannels";
            this.splitContainerSequenceVsChannels.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainerSequenceVsChannels.Panel1
            // 
            this.splitContainerSequenceVsChannels.Panel1.Controls.Add(this.groupBox4);
            // 
            // splitContainerSequenceVsChannels.Panel2
            // 
            this.splitContainerSequenceVsChannels.Panel2.Controls.Add(this.groupBox3);
            this.splitContainerSequenceVsChannels.Size = new System.Drawing.Size(271, 369);
            this.splitContainerSequenceVsChannels.SplitterDistance = 100;
            this.splitContainerSequenceVsChannels.TabIndex = 2;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.checkBoxSingleThread);
            this.groupBox4.Controls.Add(this.buttonInit);
            this.groupBox4.Controls.Add(this.checkBoxLoop);
            this.groupBox4.Controls.Add(this.buttonStart);
            this.groupBox4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox4.Location = new System.Drawing.Point(0, 0);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(271, 100);
            this.groupBox4.TabIndex = 1;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Sequence";
            // 
            // buttonInit
            // 
            this.buttonInit.Location = new System.Drawing.Point(9, 19);
            this.buttonInit.Name = "buttonInit";
            this.buttonInit.Size = new System.Drawing.Size(75, 23);
            this.buttonInit.TabIndex = 2;
            this.buttonInit.Text = "Init";
            this.buttonInit.UseVisualStyleBackColor = true;
            this.buttonInit.Click += new System.EventHandler(this.buttonInit_Click);
            // 
            // checkBoxLoop
            // 
            this.checkBoxLoop.AutoSize = true;
            this.checkBoxLoop.Checked = true;
            this.checkBoxLoop.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxLoop.Location = new System.Drawing.Point(90, 52);
            this.checkBoxLoop.Name = "checkBoxLoop";
            this.checkBoxLoop.Size = new System.Drawing.Size(50, 17);
            this.checkBoxLoop.TabIndex = 1;
            this.checkBoxLoop.Text = "Loop";
            this.checkBoxLoop.UseVisualStyleBackColor = true;
            this.checkBoxLoop.CheckedChanged += new System.EventHandler(this.checkBoxLoop_CheckedChanged);
            // 
            // buttonStart
            // 
            this.buttonStart.Enabled = false;
            this.buttonStart.Location = new System.Drawing.Point(9, 48);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(75, 23);
            this.buttonStart.TabIndex = 0;
            this.buttonStart.Text = "Start";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.buttonStart_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.dataGridViewPulses);
            this.groupBox3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox3.Location = new System.Drawing.Point(0, 0);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(271, 265);
            this.groupBox3.TabIndex = 0;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Channels";
            // 
            // dataGridViewPulses
            // 
            this.dataGridViewPulses.AllowUserToAddRows = false;
            this.dataGridViewPulses.AllowUserToDeleteRows = false;
            this.dataGridViewPulses.AllowUserToResizeColumns = false;
            this.dataGridViewPulses.AllowUserToResizeRows = false;
            this.dataGridViewPulses.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridViewPulses.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ChannelID,
            this.ChannelEnable,
            this.ChannelOffsetMS,
            this.ChannelPulseWidth,
            this.intraPulseDelayMS});
            this.dataGridViewPulses.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridViewPulses.Location = new System.Drawing.Point(3, 16);
            this.dataGridViewPulses.Name = "dataGridViewPulses";
            this.dataGridViewPulses.RowHeadersVisible = false;
            this.dataGridViewPulses.Size = new System.Drawing.Size(265, 246);
            this.dataGridViewPulses.TabIndex = 2;
            this.dataGridViewPulses.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridViewPulses_CellValueChanged);
            // 
            // ChannelID
            // 
            this.ChannelID.HeaderText = "ID";
            this.ChannelID.Name = "ChannelID";
            this.ChannelID.ReadOnly = true;
            this.ChannelID.Width = 30;
            // 
            // ChannelEnable
            // 
            this.ChannelEnable.HeaderText = "Enable";
            this.ChannelEnable.Name = "ChannelEnable";
            this.ChannelEnable.Width = 50;
            // 
            // ChannelOffsetMS
            // 
            this.ChannelOffsetMS.HeaderText = "Initial Offset (ms)";
            this.ChannelOffsetMS.Name = "ChannelOffsetMS";
            this.ChannelOffsetMS.Width = 60;
            // 
            // ChannelPulseWidth
            // 
            this.ChannelPulseWidth.HeaderText = "Pulse Width (ms)";
            this.ChannelPulseWidth.Name = "ChannelPulseWidth";
            this.ChannelPulseWidth.Width = 60;
            // 
            // intraPulseDelayMS
            // 
            this.intraPulseDelayMS.HeaderText = "Intra Pulse Delay (ms)";
            this.intraPulseDelayMS.Name = "intraPulseDelayMS";
            this.intraPulseDelayMS.Width = 60;
            // 
            // chartPulses
            // 
            chartArea1.AxisX.Enabled = System.Windows.Forms.DataVisualization.Charting.AxisEnabled.False;
            chartArea1.AxisY.Enabled = System.Windows.Forms.DataVisualization.Charting.AxisEnabled.False;
            chartArea1.AxisY.MajorGrid.Enabled = false;
            chartArea1.Name = "ChartArea1";
            this.chartPulses.ChartAreas.Add(chartArea1);
            this.chartPulses.Dock = System.Windows.Forms.DockStyle.Fill;
            legend1.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom;
            legend1.Name = "Legend1";
            this.chartPulses.Legends.Add(legend1);
            this.chartPulses.Location = new System.Drawing.Point(0, 0);
            this.chartPulses.Name = "chartPulses";
            this.chartPulses.Size = new System.Drawing.Size(724, 388);
            this.chartPulses.TabIndex = 0;
            this.chartPulses.Text = "chart1";
            // 
            // splitContainerControlsGraphVsLog
            // 
            this.splitContainerControlsGraphVsLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerControlsGraphVsLog.Location = new System.Drawing.Point(0, 0);
            this.splitContainerControlsGraphVsLog.Name = "splitContainerControlsGraphVsLog";
            this.splitContainerControlsGraphVsLog.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainerControlsGraphVsLog.Panel1
            // 
            this.splitContainerControlsGraphVsLog.Panel1.Controls.Add(this.splitContainerControlsVsGraph);
            // 
            // splitContainerControlsGraphVsLog.Panel2
            // 
            this.splitContainerControlsGraphVsLog.Panel2.Controls.Add(this.groupBox1);
            this.splitContainerControlsGraphVsLog.Size = new System.Drawing.Size(1005, 536);
            this.splitContainerControlsGraphVsLog.SplitterDistance = 388;
            this.splitContainerControlsGraphVsLog.TabIndex = 1;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBoxEventLog);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(1005, 144);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Event Log";
            // 
            // textBoxEventLog
            // 
            this.textBoxEventLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxEventLog.Location = new System.Drawing.Point(3, 16);
            this.textBoxEventLog.Multiline = true;
            this.textBoxEventLog.Name = "textBoxEventLog";
            this.textBoxEventLog.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxEventLog.Size = new System.Drawing.Size(999, 125);
            this.textBoxEventLog.TabIndex = 0;
            // 
            // backgroundWorkerSequence
            // 
            this.backgroundWorkerSequence.WorkerSupportsCancellation = true;
            this.backgroundWorkerSequence.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorkerSequence_DoWork);
            this.backgroundWorkerSequence.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorkerSequence_ProgressChanged);
            this.backgroundWorkerSequence.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.backgroundWorkerSequence_RunWorkerCompleted);
            // 
            // backgroundWorkerGraph
            // 
            this.backgroundWorkerGraph.WorkerReportsProgress = true;
            this.backgroundWorkerGraph.WorkerSupportsCancellation = true;
            this.backgroundWorkerGraph.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorkerGraph_DoWork);
            this.backgroundWorkerGraph.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorkerGraph_ProgressChanged);
            this.backgroundWorkerGraph.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.backgroundWorkerGraph_RunWorkerCompleted);
            // 
            // checkBoxSingleThread
            // 
            this.checkBoxSingleThread.AutoSize = true;
            this.checkBoxSingleThread.Location = new System.Drawing.Point(146, 52);
            this.checkBoxSingleThread.Name = "checkBoxSingleThread";
            this.checkBoxSingleThread.Size = new System.Drawing.Size(104, 17);
            this.checkBoxSingleThread.TabIndex = 3;
            this.checkBoxSingleThread.Text = "Single Threaded";
            this.checkBoxSingleThread.UseVisualStyleBackColor = true;
            this.checkBoxSingleThread.CheckedChanged += new System.EventHandler(this.checkBoxSingleThread_CheckedChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1005, 536);
            this.Controls.Add(this.splitContainerControlsGraphVsLog);
            this.Name = "Form1";
            this.Text = "Form1";
            this.splitContainerControlsVsGraph.Panel1.ResumeLayout(false);
            this.splitContainerControlsVsGraph.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerControlsVsGraph)).EndInit();
            this.splitContainerControlsVsGraph.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.splitContainerSequenceVsChannels.Panel1.ResumeLayout(false);
            this.splitContainerSequenceVsChannels.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerSequenceVsChannels)).EndInit();
            this.splitContainerSequenceVsChannels.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewPulses)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartPulses)).EndInit();
            this.splitContainerControlsGraphVsLog.Panel1.ResumeLayout(false);
            this.splitContainerControlsGraphVsLog.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerControlsGraphVsLog)).EndInit();
            this.splitContainerControlsGraphVsLog.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainerControlsVsGraph;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.SplitContainer splitContainerSequenceVsChannels;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button buttonStart;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.DataGridView dataGridViewPulses;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartPulses;
        private System.Windows.Forms.SplitContainer splitContainerControlsGraphVsLog;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox textBoxEventLog;
        private System.Windows.Forms.CheckBox checkBoxLoop;
        private System.ComponentModel.BackgroundWorker backgroundWorkerSequence;
        private System.Windows.Forms.Button buttonInit;
        private System.Windows.Forms.DataGridViewTextBoxColumn ChannelID;
        private System.Windows.Forms.DataGridViewCheckBoxColumn ChannelEnable;
        private System.Windows.Forms.DataGridViewTextBoxColumn ChannelOffsetMS;
        private System.Windows.Forms.DataGridViewTextBoxColumn ChannelPulseWidth;
        private System.Windows.Forms.DataGridViewTextBoxColumn intraPulseDelayMS;
        private System.ComponentModel.BackgroundWorker backgroundWorkerGraph;
        private System.Windows.Forms.CheckBox checkBoxSingleThread;
    }
}

