namespace MergeSpectra
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea3 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend3 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            this.splitContainerControlVsGraph = new System.Windows.Forms.SplitContainer();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.numericUpDownBaseline = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownMinIndicesBetweenPeaks = new System.Windows.Forms.NumericUpDown();
            this.checkBoxEnablePeakFinding = new System.Windows.Forms.CheckBox();
            this.checkBoxGraph = new System.Windows.Forms.CheckBox();
            this.labelFileCount = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.checkBoxExcelTabs = new System.Windows.Forms.CheckBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.numericUpDownInterpolateEnd = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.numericUpDownInterpolateIncrement = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownInterpolateStart = new System.Windows.Forms.NumericUpDown();
            this.checkBoxInterpolate = new System.Windows.Forms.CheckBox();
            this.buttonMerge = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.checkBoxDelimComma = new System.Windows.Forms.CheckBox();
            this.checkBoxDelimTabs = new System.Windows.Forms.CheckBox();
            this.listBoxInputFiles = new System.Windows.Forms.ListBox();
            this.buttonSelectInputFiles = new System.Windows.Forms.Button();
            this.chartMerge = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.openFileDialogSelectInputFiles = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialogMergeResult = new System.Windows.Forms.SaveFileDialog();
            this.splitContainerControlsGraphVsLog = new System.Windows.Forms.SplitContainer();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.textBoxEventLog = new System.Windows.Forms.TextBox();
            this.numericUpDownYAxisColumn = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerControlVsGraph)).BeginInit();
            this.splitContainerControlVsGraph.Panel1.SuspendLayout();
            this.splitContainerControlVsGraph.Panel2.SuspendLayout();
            this.splitContainerControlVsGraph.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownBaseline)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMinIndicesBetweenPeaks)).BeginInit();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownInterpolateEnd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownInterpolateIncrement)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownInterpolateStart)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chartMerge)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerControlsGraphVsLog)).BeginInit();
            this.splitContainerControlsGraphVsLog.Panel1.SuspendLayout();
            this.splitContainerControlsGraphVsLog.Panel2.SuspendLayout();
            this.splitContainerControlsGraphVsLog.SuspendLayout();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownYAxisColumn)).BeginInit();
            this.SuspendLayout();
            // 
            // splitContainerControlVsGraph
            // 
            this.splitContainerControlVsGraph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerControlVsGraph.Location = new System.Drawing.Point(0, 0);
            this.splitContainerControlVsGraph.Name = "splitContainerControlVsGraph";
            // 
            // splitContainerControlVsGraph.Panel1
            // 
            this.splitContainerControlVsGraph.Panel1.Controls.Add(this.groupBox2);
            this.splitContainerControlVsGraph.Panel1.Controls.Add(this.groupBox1);
            // 
            // splitContainerControlVsGraph.Panel2
            // 
            this.splitContainerControlVsGraph.Panel2.Controls.Add(this.chartMerge);
            this.splitContainerControlVsGraph.Size = new System.Drawing.Size(1094, 372);
            this.splitContainerControlVsGraph.SplitterDistance = 507;
            this.splitContainerControlVsGraph.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.groupBox6);
            this.groupBox2.Controls.Add(this.checkBoxGraph);
            this.groupBox2.Controls.Add(this.labelFileCount);
            this.groupBox2.Controls.Add(this.groupBox4);
            this.groupBox2.Controls.Add(this.groupBox3);
            this.groupBox2.Controls.Add(this.buttonMerge);
            this.groupBox2.Location = new System.Drawing.Point(6, 180);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(493, 180);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Transforms";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.label5);
            this.groupBox6.Controls.Add(this.label4);
            this.groupBox6.Controls.Add(this.numericUpDownBaseline);
            this.groupBox6.Controls.Add(this.numericUpDownMinIndicesBetweenPeaks);
            this.groupBox6.Controls.Add(this.checkBoxEnablePeakFinding);
            this.groupBox6.Location = new System.Drawing.Point(165, 73);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(263, 69);
            this.groupBox6.TabIndex = 5;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "SPAM";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(166, 43);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(46, 13);
            this.label5.TabIndex = 5;
            this.label5.Text = "baseline";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(166, 20);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(96, 13);
            this.label4.TabIndex = 4;
            this.label4.Text = "min pixels between";
            // 
            // numericUpDownBaseline
            // 
            this.numericUpDownBaseline.Location = new System.Drawing.Point(107, 40);
            this.numericUpDownBaseline.Maximum = new decimal(new int[] {
            50000,
            0,
            0,
            0});
            this.numericUpDownBaseline.Name = "numericUpDownBaseline";
            this.numericUpDownBaseline.Size = new System.Drawing.Size(53, 20);
            this.numericUpDownBaseline.TabIndex = 3;
            this.numericUpDownBaseline.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDownBaseline.Value = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numericUpDownBaseline.ValueChanged += new System.EventHandler(this.numericUpDownBaseline_ValueChanged);
            // 
            // numericUpDownMinIndicesBetweenPeaks
            // 
            this.numericUpDownMinIndicesBetweenPeaks.Location = new System.Drawing.Point(107, 18);
            this.numericUpDownMinIndicesBetweenPeaks.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownMinIndicesBetweenPeaks.Name = "numericUpDownMinIndicesBetweenPeaks";
            this.numericUpDownMinIndicesBetweenPeaks.Size = new System.Drawing.Size(53, 20);
            this.numericUpDownMinIndicesBetweenPeaks.TabIndex = 2;
            this.numericUpDownMinIndicesBetweenPeaks.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDownMinIndicesBetweenPeaks.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericUpDownMinIndicesBetweenPeaks.ValueChanged += new System.EventHandler(this.numericUpDownMinIndicesBetweenPeaks_ValueChanged);
            // 
            // checkBoxEnablePeakFinding
            // 
            this.checkBoxEnablePeakFinding.AutoSize = true;
            this.checkBoxEnablePeakFinding.Location = new System.Drawing.Point(6, 19);
            this.checkBoxEnablePeakFinding.Name = "checkBoxEnablePeakFinding";
            this.checkBoxEnablePeakFinding.Size = new System.Drawing.Size(85, 17);
            this.checkBoxEnablePeakFinding.TabIndex = 0;
            this.checkBoxEnablePeakFinding.Text = "PeakFinding";
            this.checkBoxEnablePeakFinding.UseVisualStyleBackColor = true;
            this.checkBoxEnablePeakFinding.CheckedChanged += new System.EventHandler(this.checkBoxEnablePeakFinding_CheckedChanged);
            // 
            // checkBoxGraph
            // 
            this.checkBoxGraph.AutoSize = true;
            this.checkBoxGraph.Checked = true;
            this.checkBoxGraph.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxGraph.Location = new System.Drawing.Point(165, 152);
            this.checkBoxGraph.Name = "checkBoxGraph";
            this.checkBoxGraph.Size = new System.Drawing.Size(55, 17);
            this.checkBoxGraph.TabIndex = 4;
            this.checkBoxGraph.Text = "Graph";
            this.checkBoxGraph.UseVisualStyleBackColor = true;
            this.checkBoxGraph.CheckedChanged += new System.EventHandler(this.checkBoxGraph_CheckedChanged);
            // 
            // labelFileCount
            // 
            this.labelFileCount.AutoSize = true;
            this.labelFileCount.Location = new System.Drawing.Point(88, 153);
            this.labelFileCount.Name = "labelFileCount";
            this.labelFileCount.Size = new System.Drawing.Size(25, 13);
            this.labelFileCount.TabIndex = 4;
            this.labelFileCount.Text = "files";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.checkBoxExcelTabs);
            this.groupBox4.Enabled = false;
            this.groupBox4.Location = new System.Drawing.Point(165, 19);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(104, 48);
            this.groupBox4.TabIndex = 3;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Excel";
            // 
            // checkBoxExcelTabs
            // 
            this.checkBoxExcelTabs.AutoSize = true;
            this.checkBoxExcelTabs.Location = new System.Drawing.Point(6, 19);
            this.checkBoxExcelTabs.Name = "checkBoxExcelTabs";
            this.checkBoxExcelTabs.Size = new System.Drawing.Size(86, 17);
            this.checkBoxExcelTabs.TabIndex = 0;
            this.checkBoxExcelTabs.Text = "Files to Tabs";
            this.checkBoxExcelTabs.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.numericUpDownInterpolateEnd);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.numericUpDownInterpolateIncrement);
            this.groupBox3.Controls.Add(this.numericUpDownInterpolateStart);
            this.groupBox3.Controls.Add(this.checkBoxInterpolate);
            this.groupBox3.Location = new System.Drawing.Point(6, 19);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(153, 123);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Interpolation";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(81, 70);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(26, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "End";
            // 
            // numericUpDownInterpolateEnd
            // 
            this.numericUpDownInterpolateEnd.Location = new System.Drawing.Point(28, 68);
            this.numericUpDownInterpolateEnd.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numericUpDownInterpolateEnd.Name = "numericUpDownInterpolateEnd";
            this.numericUpDownInterpolateEnd.Size = new System.Drawing.Size(47, 20);
            this.numericUpDownInterpolateEnd.TabIndex = 2;
            this.numericUpDownInterpolateEnd.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDownInterpolateEnd.Value = new decimal(new int[] {
            2400,
            0,
            0,
            0});
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(81, 96);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(54, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Increment";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(81, 44);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Start";
            // 
            // numericUpDownInterpolateIncrement
            // 
            this.numericUpDownInterpolateIncrement.DecimalPlaces = 1;
            this.numericUpDownInterpolateIncrement.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.numericUpDownInterpolateIncrement.Location = new System.Drawing.Point(28, 94);
            this.numericUpDownInterpolateIncrement.Name = "numericUpDownInterpolateIncrement";
            this.numericUpDownInterpolateIncrement.Size = new System.Drawing.Size(47, 20);
            this.numericUpDownInterpolateIncrement.TabIndex = 3;
            this.numericUpDownInterpolateIncrement.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDownInterpolateIncrement.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // numericUpDownInterpolateStart
            // 
            this.numericUpDownInterpolateStart.Location = new System.Drawing.Point(28, 42);
            this.numericUpDownInterpolateStart.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numericUpDownInterpolateStart.Name = "numericUpDownInterpolateStart";
            this.numericUpDownInterpolateStart.Size = new System.Drawing.Size(47, 20);
            this.numericUpDownInterpolateStart.TabIndex = 1;
            this.numericUpDownInterpolateStart.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDownInterpolateStart.Value = new decimal(new int[] {
            400,
            0,
            0,
            0});
            // 
            // checkBoxInterpolate
            // 
            this.checkBoxInterpolate.AutoSize = true;
            this.checkBoxInterpolate.Location = new System.Drawing.Point(6, 19);
            this.checkBoxInterpolate.Name = "checkBoxInterpolate";
            this.checkBoxInterpolate.Size = new System.Drawing.Size(76, 17);
            this.checkBoxInterpolate.TabIndex = 30;
            this.checkBoxInterpolate.Text = "Interpolate";
            this.checkBoxInterpolate.UseVisualStyleBackColor = true;
            this.checkBoxInterpolate.CheckedChanged += new System.EventHandler(this.checkBoxInterpolate_CheckedChanged);
            // 
            // buttonMerge
            // 
            this.buttonMerge.Location = new System.Drawing.Point(6, 148);
            this.buttonMerge.Name = "buttonMerge";
            this.buttonMerge.Size = new System.Drawing.Size(75, 23);
            this.buttonMerge.TabIndex = 0;
            this.buttonMerge.Text = "Merge";
            this.buttonMerge.UseVisualStyleBackColor = true;
            this.buttonMerge.Click += new System.EventHandler(this.buttonMerge_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.numericUpDownYAxisColumn);
            this.groupBox1.Controls.Add(this.checkBoxDelimComma);
            this.groupBox1.Controls.Add(this.checkBoxDelimTabs);
            this.groupBox1.Controls.Add(this.listBoxInputFiles);
            this.groupBox1.Controls.Add(this.buttonSelectInputFiles);
            this.groupBox1.Location = new System.Drawing.Point(6, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(493, 171);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Inputs";
            // 
            // checkBoxDelimComma
            // 
            this.checkBoxDelimComma.AutoSize = true;
            this.checkBoxDelimComma.Checked = true;
            this.checkBoxDelimComma.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxDelimComma.Location = new System.Drawing.Point(212, 23);
            this.checkBoxDelimComma.Name = "checkBoxDelimComma";
            this.checkBoxDelimComma.Size = new System.Drawing.Size(66, 17);
            this.checkBoxDelimComma.TabIndex = 3;
            this.checkBoxDelimComma.Text = "Commas";
            this.checkBoxDelimComma.UseVisualStyleBackColor = true;
            // 
            // checkBoxDelimTabs
            // 
            this.checkBoxDelimTabs.AutoSize = true;
            this.checkBoxDelimTabs.Checked = true;
            this.checkBoxDelimTabs.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxDelimTabs.Location = new System.Drawing.Point(156, 23);
            this.checkBoxDelimTabs.Name = "checkBoxDelimTabs";
            this.checkBoxDelimTabs.Size = new System.Drawing.Size(50, 17);
            this.checkBoxDelimTabs.TabIndex = 2;
            this.checkBoxDelimTabs.Text = "Tabs";
            this.checkBoxDelimTabs.UseVisualStyleBackColor = true;
            // 
            // listBoxInputFiles
            // 
            this.listBoxInputFiles.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.listBoxInputFiles.FormattingEnabled = true;
            this.listBoxInputFiles.Location = new System.Drawing.Point(6, 48);
            this.listBoxInputFiles.Name = "listBoxInputFiles";
            this.listBoxInputFiles.Size = new System.Drawing.Size(481, 95);
            this.listBoxInputFiles.TabIndex = 1;
            // 
            // buttonSelectInputFiles
            // 
            this.buttonSelectInputFiles.Location = new System.Drawing.Point(6, 19);
            this.buttonSelectInputFiles.Name = "buttonSelectInputFiles";
            this.buttonSelectInputFiles.Size = new System.Drawing.Size(117, 23);
            this.buttonSelectInputFiles.TabIndex = 0;
            this.buttonSelectInputFiles.Text = "Select Input Files";
            this.buttonSelectInputFiles.UseVisualStyleBackColor = true;
            this.buttonSelectInputFiles.Click += new System.EventHandler(this.buttonSelectInputFiles_Click);
            // 
            // chartMerge
            // 
            chartArea3.CursorX.IsUserEnabled = true;
            chartArea3.CursorX.IsUserSelectionEnabled = true;
            chartArea3.CursorY.IsUserEnabled = true;
            chartArea3.CursorY.IsUserSelectionEnabled = true;
            chartArea3.Name = "ChartArea1";
            this.chartMerge.ChartAreas.Add(chartArea3);
            this.chartMerge.Dock = System.Windows.Forms.DockStyle.Fill;
            legend3.Name = "Legend1";
            this.chartMerge.Legends.Add(legend3);
            this.chartMerge.Location = new System.Drawing.Point(0, 0);
            this.chartMerge.Name = "chartMerge";
            this.chartMerge.Size = new System.Drawing.Size(583, 372);
            this.chartMerge.TabIndex = 0;
            this.chartMerge.Text = "chart1";
            // 
            // openFileDialogSelectInputFiles
            // 
            this.openFileDialogSelectInputFiles.AddExtension = false;
            this.openFileDialogSelectInputFiles.Multiselect = true;
            this.openFileDialogSelectInputFiles.ReadOnlyChecked = true;
            this.openFileDialogSelectInputFiles.ShowReadOnly = true;
            this.openFileDialogSelectInputFiles.SupportMultiDottedExtensions = true;
            // 
            // saveFileDialogMergeResult
            // 
            this.saveFileDialogMergeResult.AddExtension = false;
            this.saveFileDialogMergeResult.DefaultExt = "csv";
            this.saveFileDialogMergeResult.FileName = "merged.csv";
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
            this.splitContainerControlsGraphVsLog.Panel1.Controls.Add(this.splitContainerControlVsGraph);
            // 
            // splitContainerControlsGraphVsLog.Panel2
            // 
            this.splitContainerControlsGraphVsLog.Panel2.Controls.Add(this.groupBox5);
            this.splitContainerControlsGraphVsLog.Size = new System.Drawing.Size(1094, 484);
            this.splitContainerControlsGraphVsLog.SplitterDistance = 372;
            this.splitContainerControlsGraphVsLog.TabIndex = 1;
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.textBoxEventLog);
            this.groupBox5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox5.Location = new System.Drawing.Point(0, 0);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(1094, 108);
            this.groupBox5.TabIndex = 1;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Event Log";
            // 
            // textBoxEventLog
            // 
            this.textBoxEventLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxEventLog.Location = new System.Drawing.Point(3, 16);
            this.textBoxEventLog.Multiline = true;
            this.textBoxEventLog.Name = "textBoxEventLog";
            this.textBoxEventLog.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxEventLog.Size = new System.Drawing.Size(1088, 89);
            this.textBoxEventLog.TabIndex = 0;
            this.textBoxEventLog.WordWrap = false;
            // 
            // numericUpDownYAxisColumn
            // 
            this.numericUpDownYAxisColumn.Location = new System.Drawing.Point(294, 22);
            this.numericUpDownYAxisColumn.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownYAxisColumn.Name = "numericUpDownYAxisColumn";
            this.numericUpDownYAxisColumn.Size = new System.Drawing.Size(49, 20);
            this.numericUpDownYAxisColumn.TabIndex = 4;
            this.numericUpDownYAxisColumn.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDownYAxisColumn.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numericUpDownYAxisColumn.ValueChanged += new System.EventHandler(this.numericUpDownYAxisColumn_ValueChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(349, 24);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(107, 13);
            this.label6.TabIndex = 5;
            this.label6.Text = "Merge Y-Axis Column";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1094, 484);
            this.Controls.Add(this.splitContainerControlsGraphVsLog);
            this.Name = "Form1";
            this.Text = "Merge Spectra";
            this.splitContainerControlVsGraph.Panel1.ResumeLayout(false);
            this.splitContainerControlVsGraph.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerControlVsGraph)).EndInit();
            this.splitContainerControlVsGraph.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownBaseline)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMinIndicesBetweenPeaks)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownInterpolateEnd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownInterpolateIncrement)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownInterpolateStart)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chartMerge)).EndInit();
            this.splitContainerControlsGraphVsLog.Panel1.ResumeLayout(false);
            this.splitContainerControlsGraphVsLog.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerControlsGraphVsLog)).EndInit();
            this.splitContainerControlsGraphVsLog.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownYAxisColumn)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainerControlVsGraph;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.CheckBox checkBoxExcelTabs;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numericUpDownInterpolateEnd;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDownInterpolateIncrement;
        private System.Windows.Forms.NumericUpDown numericUpDownInterpolateStart;
        private System.Windows.Forms.CheckBox checkBoxInterpolate;
        private System.Windows.Forms.Button buttonMerge;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ListBox listBoxInputFiles;
        private System.Windows.Forms.Button buttonSelectInputFiles;
        private System.Windows.Forms.OpenFileDialog openFileDialogSelectInputFiles;
        private System.Windows.Forms.SaveFileDialog saveFileDialogMergeResult;
        private System.Windows.Forms.Label labelFileCount;
        private System.Windows.Forms.CheckBox checkBoxDelimComma;
        private System.Windows.Forms.CheckBox checkBoxDelimTabs;
        private System.Windows.Forms.SplitContainer splitContainerControlsGraphVsLog;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.TextBox textBoxEventLog;
        private System.Windows.Forms.CheckBox checkBoxGraph;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartMerge;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.CheckBox checkBoxEnablePeakFinding;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown numericUpDownBaseline;
        private System.Windows.Forms.NumericUpDown numericUpDownMinIndicesBetweenPeaks;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.NumericUpDown numericUpDownYAxisColumn;
    }
}

