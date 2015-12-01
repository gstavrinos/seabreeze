namespace MergeSpectra
{
    partial class PreviewForm
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend2 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.textBoxSummary = new System.Windows.Forms.TextBox();
            this.splitContainerSummaryVsGraph = new System.Windows.Forms.SplitContainer();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.chartPeaks = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerSummaryVsGraph)).BeginInit();
            this.splitContainerSummaryVsGraph.Panel1.SuspendLayout();
            this.splitContainerSummaryVsGraph.Panel2.SuspendLayout();
            this.splitContainerSummaryVsGraph.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chartPeaks)).BeginInit();
            this.SuspendLayout();
            // 
            // textBoxSummary
            // 
            this.textBoxSummary.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxSummary.Location = new System.Drawing.Point(3, 16);
            this.textBoxSummary.Multiline = true;
            this.textBoxSummary.Name = "textBoxSummary";
            this.textBoxSummary.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxSummary.Size = new System.Drawing.Size(459, 426);
            this.textBoxSummary.TabIndex = 0;
            // 
            // splitContainerSummaryVsGraph
            // 
            this.splitContainerSummaryVsGraph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerSummaryVsGraph.Location = new System.Drawing.Point(0, 0);
            this.splitContainerSummaryVsGraph.Name = "splitContainerSummaryVsGraph";
            // 
            // splitContainerSummaryVsGraph.Panel1
            // 
            this.splitContainerSummaryVsGraph.Panel1.Controls.Add(this.groupBox1);
            // 
            // splitContainerSummaryVsGraph.Panel2
            // 
            this.splitContainerSummaryVsGraph.Panel2.Controls.Add(this.chartPeaks);
            this.splitContainerSummaryVsGraph.Size = new System.Drawing.Size(855, 445);
            this.splitContainerSummaryVsGraph.SplitterDistance = 465;
            this.splitContainerSummaryVsGraph.TabIndex = 1;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBoxSummary);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(465, 445);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Summary";
            // 
            // chartPeaks
            // 
            chartArea2.Name = "ChartArea1";
            this.chartPeaks.ChartAreas.Add(chartArea2);
            this.chartPeaks.Dock = System.Windows.Forms.DockStyle.Fill;
            legend2.Name = "Legend1";
            this.chartPeaks.Legends.Add(legend2);
            this.chartPeaks.Location = new System.Drawing.Point(0, 0);
            this.chartPeaks.Name = "chartPeaks";
            series2.ChartArea = "ChartArea1";
            series2.Legend = "Legend1";
            series2.Name = "Series1";
            this.chartPeaks.Series.Add(series2);
            this.chartPeaks.Size = new System.Drawing.Size(386, 445);
            this.chartPeaks.TabIndex = 0;
            this.chartPeaks.Text = "chart1";
            // 
            // PreviewForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(855, 445);
            this.Controls.Add(this.splitContainerSummaryVsGraph);
            this.Name = "PreviewForm";
            this.Text = "PreviewForm";
            this.splitContainerSummaryVsGraph.Panel1.ResumeLayout(false);
            this.splitContainerSummaryVsGraph.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerSummaryVsGraph)).EndInit();
            this.splitContainerSummaryVsGraph.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chartPeaks)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainerSummaryVsGraph;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartPeaks;
        internal System.Windows.Forms.TextBox textBoxSummary;
    }
}