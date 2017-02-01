namespace FlameX_SeaBreeze_Command_Test
{
    partial class FlameX_SeaBreeze_Test
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
            this.buttonFindFlameX = new System.Windows.Forms.Button();
            this.listBoxTestLog = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // buttonFindFlameX
            // 
            this.buttonFindFlameX.Location = new System.Drawing.Point(12, 12);
            this.buttonFindFlameX.Name = "buttonFindFlameX";
            this.buttonFindFlameX.Size = new System.Drawing.Size(78, 23);
            this.buttonFindFlameX.TabIndex = 1;
            this.buttonFindFlameX.Text = "Connect";
            this.buttonFindFlameX.UseVisualStyleBackColor = true;
            this.buttonFindFlameX.Click += new System.EventHandler(this.buttonFindFlameX_Click);
            // 
            // listBoxTestLog
            // 
            this.listBoxTestLog.FormattingEnabled = true;
            this.listBoxTestLog.Location = new System.Drawing.Point(12, 41);
            this.listBoxTestLog.Name = "listBoxTestLog";
            this.listBoxTestLog.Size = new System.Drawing.Size(467, 316);
            this.listBoxTestLog.TabIndex = 4;
            // 
            // FlameX_SeaBreeze_Test
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(955, 360);
            this.Controls.Add(this.listBoxTestLog);
            this.Controls.Add(this.buttonFindFlameX);
            this.Name = "FlameX_SeaBreeze_Test";
            this.Text = "Flame X SeaBreeze Test";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FlameX_SeaBreeze_Test_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Button buttonFindFlameX;
        private System.Windows.Forms.ListBox listBoxTestLog;
    }
}

