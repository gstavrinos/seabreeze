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
            this.listBoxFlameXFound = new System.Windows.Forms.ListBox();
            this.buttonFindFlameX = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // listBoxFlameXFound
            // 
            this.listBoxFlameXFound.FormattingEnabled = true;
            this.listBoxFlameXFound.Location = new System.Drawing.Point(12, 38);
            this.listBoxFlameXFound.Name = "listBoxFlameXFound";
            this.listBoxFlameXFound.Size = new System.Drawing.Size(216, 108);
            this.listBoxFlameXFound.TabIndex = 0;
            // 
            // buttonFindFlameX
            // 
            this.buttonFindFlameX.Location = new System.Drawing.Point(12, 9);
            this.buttonFindFlameX.Name = "buttonFindFlameX";
            this.buttonFindFlameX.Size = new System.Drawing.Size(75, 23);
            this.buttonFindFlameX.TabIndex = 1;
            this.buttonFindFlameX.Text = "Find FlameX";
            this.buttonFindFlameX.UseVisualStyleBackColor = true;
            this.buttonFindFlameX.Click += new System.EventHandler(this.buttonFindFlameX_Click);
            // 
            // FlameX_SeaBreeze_Test
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(647, 309);
            this.Controls.Add(this.buttonFindFlameX);
            this.Controls.Add(this.listBoxFlameXFound);
            this.Name = "FlameX_SeaBreeze_Test";
            this.Text = "Flame X SeaBreeze Test";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox listBoxFlameXFound;
        private System.Windows.Forms.Button buttonFindFlameX;
    }
}

