namespace OBP_RS232
{
    partial class MainForm
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
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.comboBoxComPort = new System.Windows.Forms.ComboBox();
            this.comboBoxDataBits = new System.Windows.Forms.ComboBox();
            this.comboBoxParity = new System.Windows.Forms.ComboBox();
            this.comboBoxStopBits = new System.Windows.Forms.ComboBox();
            this.comboBoxHandshaking = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.comboBoxBaudRate = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.buttonTest = new System.Windows.Forms.Button();
            this.buttonSyncFW = new System.Windows.Forms.Button();
            this.labelPortName = new System.Windows.Forms.Label();
            this.labelBaudRate = new System.Windows.Forms.Label();
            this.labelDataBits = new System.Windows.Forms.Label();
            this.labelParity = new System.Windows.Forms.Label();
            this.labelStopBits = new System.Windows.Forms.Label();
            this.labelHandshaking = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.labelIOStatus = new System.Windows.Forms.Label();
            this.labelOBPStatus = new System.Windows.Forms.Label();
            this.checkBoxAckAlways = new System.Windows.Forms.CheckBox();
            this.label10 = new System.Windows.Forms.Label();
            this.textBoxProtocolVersion = new System.Windows.Forms.TextBox();
            this.tabControlContent = new System.Windows.Forms.TabControl();
            this.tabPageIOLog = new System.Windows.Forms.TabPage();
            this.buttonClearLog = new System.Windows.Forms.Button();
            this.textBoxIOLog = new System.Windows.Forms.TextBox();
            this.tabPageInfo = new System.Windows.Forms.TabPage();
            this.textBoxDetectorSerialNum = new System.Windows.Forms.TextBox();
            this.buttonReadDetectorSerialNum = new System.Windows.Forms.Button();
            this.label33 = new System.Windows.Forms.Label();
            this.textBoxCoating = new System.Windows.Forms.TextBox();
            this.buttonReadCoating = new System.Windows.Forms.Button();
            this.label32 = new System.Windows.Forms.Label();
            this.textBoxFilter = new System.Windows.Forms.TextBox();
            this.buttonReadFilter = new System.Windows.Forms.Button();
            this.label31 = new System.Windows.Forms.Label();
            this.textBoxGrating = new System.Windows.Forms.TextBox();
            this.buttonReadGrating = new System.Windows.Forms.Button();
            this.label30 = new System.Windows.Forms.Label();
            this.textBoxFiberDiameter = new System.Windows.Forms.TextBox();
            this.buttonReadFiberDiameter = new System.Windows.Forms.Button();
            this.label29 = new System.Windows.Forms.Label();
            this.textBoxSlitWidth = new System.Windows.Forms.TextBox();
            this.buttonReadSlitWidth = new System.Windows.Forms.Button();
            this.label28 = new System.Windows.Forms.Label();
            this.textBoxBenchSerialNum = new System.Windows.Forms.TextBox();
            this.buttonReadBenchSerialNum = new System.Windows.Forms.Button();
            this.label27 = new System.Windows.Forms.Label();
            this.textBoxBenchId = new System.Windows.Forms.TextBox();
            this.buttonReadBenchId = new System.Windows.Forms.Button();
            this.label26 = new System.Windows.Forms.Label();
            this.buttonSaveRS232Settings = new System.Windows.Forms.Button();
            this.buttonResetDefaults = new System.Windows.Forms.Button();
            this.buttonResetSpectrometer = new System.Windows.Forms.Button();
            this.textBoxStatusLED = new System.Windows.Forms.TextBox();
            this.buttonWriteStatusLED = new System.Windows.Forms.Button();
            this.label25 = new System.Windows.Forms.Label();
            this.textBoxSerialNumLen = new System.Windows.Forms.TextBox();
            this.buttonReadSerialNumLen = new System.Windows.Forms.Button();
            this.label24 = new System.Windows.Forms.Label();
            this.textBoxFPGARevision = new System.Windows.Forms.TextBox();
            this.buttonReadFPGARevision = new System.Windows.Forms.Button();
            this.label23 = new System.Windows.Forms.Label();
            this.comboBoxUserStringNum = new System.Windows.Forms.ComboBox();
            this.textBoxUserStringLen = new System.Windows.Forms.TextBox();
            this.textBoxNumUserStrings = new System.Windows.Forms.TextBox();
            this.textBoxUserString = new System.Windows.Forms.TextBox();
            this.buttonWriteUserString = new System.Windows.Forms.Button();
            this.buttonReadUserString = new System.Windows.Forms.Button();
            this.buttonReadUserStringLen = new System.Windows.Forms.Button();
            this.buttonReadNumUserStrings = new System.Windows.Forms.Button();
            this.textBoxDeviceAlias = new System.Windows.Forms.TextBox();
            this.buttonWriteDeviceAlias = new System.Windows.Forms.Button();
            this.buttonReadDeviceAlias = new System.Windows.Forms.Button();
            this.textBoxDeviceAliasLen = new System.Windows.Forms.TextBox();
            this.buttonReadAliasLen = new System.Windows.Forms.Button();
            this.textBoxFwRevision = new System.Windows.Forms.TextBox();
            this.textBoxHwRevision = new System.Windows.Forms.TextBox();
            this.buttonReadFWRevision = new System.Windows.Forms.Button();
            this.textBoxSerialNum = new System.Windows.Forms.TextBox();
            this.buttonReadHWRevision = new System.Windows.Forms.Button();
            this.buttonReadSerialNum = new System.Windows.Forms.Button();
            this.label22 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.tabPageAcquisitionParams = new System.Windows.Forms.TabPage();
            this.checkBoxDefaultBinningFactor = new System.Windows.Forms.CheckBox();
            this.label49 = new System.Windows.Forms.Label();
            this.buttonWritePartialSpectrumMode = new System.Windows.Forms.Button();
            this.buttonReadPartialSpectrumMode = new System.Windows.Forms.Button();
            this.buttonSimTriggerPulse = new System.Windows.Forms.Button();
            this.textBoxPartialSpectrumMode = new System.Windows.Forms.TextBox();
            this.textBoxMaxBinningFactor = new System.Windows.Forms.TextBox();
            this.buttonReadMaxBinningFactor = new System.Windows.Forms.Button();
            this.label48 = new System.Windows.Forms.Label();
            this.textBoxDefaultBinningFactor = new System.Windows.Forms.TextBox();
            this.buttonWriteDefaultBinningFactor = new System.Windows.Forms.Button();
            this.buttonReadDefaultBinningFactor = new System.Windows.Forms.Button();
            this.label47 = new System.Windows.Forms.Label();
            this.textBoxBinningFactor = new System.Windows.Forms.TextBox();
            this.buttonWriteBinningFactor = new System.Windows.Forms.Button();
            this.buttonReadBinningFactor = new System.Windows.Forms.Button();
            this.label46 = new System.Windows.Forms.Label();
            this.textBoxBoxcarWidth = new System.Windows.Forms.TextBox();
            this.buttonWriteBoxcarWidth = new System.Windows.Forms.Button();
            this.buttonReadBoxcarWidth = new System.Windows.Forms.Button();
            this.label45 = new System.Windows.Forms.Label();
            this.textBoxScansToAvg = new System.Windows.Forms.TextBox();
            this.buttonWriteScansToAvg = new System.Windows.Forms.Button();
            this.buttonReadScansToAvg = new System.Windows.Forms.Button();
            this.label44 = new System.Windows.Forms.Label();
            this.textBoxAcquisitionDelayIncrement = new System.Windows.Forms.TextBox();
            this.buttonReadAcquisitionDelayIncrement = new System.Windows.Forms.Button();
            this.label41 = new System.Windows.Forms.Label();
            this.textBoxMinAcquisitionDelay = new System.Windows.Forms.TextBox();
            this.textBoxMaxAcquisitionDelay = new System.Windows.Forms.TextBox();
            this.buttonReadMinAcquisitionDelay = new System.Windows.Forms.Button();
            this.buttonReadMaxAcquisitionDelay = new System.Windows.Forms.Button();
            this.label42 = new System.Windows.Forms.Label();
            this.label43 = new System.Windows.Forms.Label();
            this.textBoxAcquisitionDelay = new System.Windows.Forms.TextBox();
            this.buttonWriteAcquisitionDelay = new System.Windows.Forms.Button();
            this.buttonReadAcquisitionDelay = new System.Windows.Forms.Button();
            this.label40 = new System.Windows.Forms.Label();
            this.textBoxTriggerMode = new System.Windows.Forms.TextBox();
            this.buttonWriteTriggerMode = new System.Windows.Forms.Button();
            this.buttonReadTriggerMode = new System.Windows.Forms.Button();
            this.label39 = new System.Windows.Forms.Label();
            this.textBoxLampEnable = new System.Windows.Forms.TextBox();
            this.buttonWriteLampEnable = new System.Windows.Forms.Button();
            this.buttonReadLampEnable = new System.Windows.Forms.Button();
            this.label38 = new System.Windows.Forms.Label();
            this.textBoxIntegTimeIncrement = new System.Windows.Forms.TextBox();
            this.buttonReadIntegTimeIncrement = new System.Windows.Forms.Button();
            this.label37 = new System.Windows.Forms.Label();
            this.textBoxMinIntegTime = new System.Windows.Forms.TextBox();
            this.textBoxMaxIntegTime = new System.Windows.Forms.TextBox();
            this.buttonReadMinIntegTime = new System.Windows.Forms.Button();
            this.buttonReadMaxIntegTime = new System.Windows.Forms.Button();
            this.textBoxIntegTime = new System.Windows.Forms.TextBox();
            this.buttonWriteIntegTime = new System.Windows.Forms.Button();
            this.buttonReadIntegTime = new System.Windows.Forms.Button();
            this.label34 = new System.Windows.Forms.Label();
            this.label35 = new System.Windows.Forms.Label();
            this.label36 = new System.Windows.Forms.Label();
            this.tabPageSpectrum = new System.Windows.Forms.TabPage();
            this.tabControlSpectrumInfo = new System.Windows.Forms.TabControl();
            this.tabPageChart = new System.Windows.Forms.TabPage();
            this.chartSpectrum = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.tabPageData = new System.Windows.Forms.TabPage();
            this.textBoxMaxValuePixelNum = new System.Windows.Forms.TextBox();
            this.labelMaxValuePixelNum = new System.Windows.Forms.Label();
            this.textBoxMaxSpectrumValue = new System.Windows.Forms.TextBox();
            this.label61 = new System.Windows.Forms.Label();
            this.textBoxNumPixelsReturned = new System.Windows.Forms.TextBox();
            this.groupBoxMetadata = new System.Windows.Forms.GroupBox();
            this.textBoxMetadataTriggerMode = new System.Windows.Forms.TextBox();
            this.textBoxMetadataTimeUS = new System.Windows.Forms.TextBox();
            this.textBoxMetadataTickCount = new System.Windows.Forms.TextBox();
            this.textBoxMetadataSpectrumCount = new System.Windows.Forms.TextBox();
            this.label60 = new System.Windows.Forms.Label();
            this.label59 = new System.Windows.Forms.Label();
            this.label58 = new System.Windows.Forms.Label();
            this.label57 = new System.Windows.Forms.Label();
            this.label56 = new System.Windows.Forms.Label();
            this.textBoxSpectrumData = new System.Windows.Forms.TextBox();
            this.tabPageBuffer = new System.Windows.Forms.TabPage();
            this.buttonAbortAcquisition = new System.Windows.Forms.Button();
            this.textBoxIsIdle = new System.Windows.Forms.TextBox();
            this.buttonReadIsIdle = new System.Windows.Forms.Button();
            this.label66 = new System.Windows.Forms.Label();
            this.buttonAcquireSpectraIntoBuffer = new System.Windows.Forms.Button();
            this.buttonClearBufferedSpectra = new System.Windows.Forms.Button();
            this.textBoxRemoveOldestSpectra = new System.Windows.Forms.TextBox();
            this.buttonRemoveOldestSpectra = new System.Windows.Forms.Button();
            this.label65 = new System.Windows.Forms.Label();
            this.textBoxNumSpectraInBuffer = new System.Windows.Forms.TextBox();
            this.buttonReadNumSpectraInBuffer = new System.Windows.Forms.Button();
            this.label64 = new System.Windows.Forms.Label();
            this.textBoxBufferSize = new System.Windows.Forms.TextBox();
            this.buttonWriteBufferSize = new System.Windows.Forms.Button();
            this.buttonReadBufferSize = new System.Windows.Forms.Button();
            this.label63 = new System.Windows.Forms.Label();
            this.textBoxMaxBufferSize = new System.Windows.Forms.TextBox();
            this.buttonReadMaxBufferSize = new System.Windows.Forms.Button();
            this.label62 = new System.Windows.Forms.Label();
            this.buttonReadBufferedSpectrum = new System.Windows.Forms.Button();
            this.buttonReadPartialCorrectedSpectrum = new System.Windows.Forms.Button();
            this.buttonReadRawSpectrum = new System.Windows.Forms.Button();
            this.buttonReadCorrectedSpectrum = new System.Windows.Forms.Button();
            this.tabPageCoefficients = new System.Windows.Forms.TabPage();
            this.comboBoxStrayLightCoeffIndex = new System.Windows.Forms.ComboBox();
            this.textBoxNumStrayLightCoeffs = new System.Windows.Forms.TextBox();
            this.textBoxStrayLightCoeff = new System.Windows.Forms.TextBox();
            this.buttonWriteStrayLightCoeff = new System.Windows.Forms.Button();
            this.buttonReadStrayLightCoeff = new System.Windows.Forms.Button();
            this.buttonReadNumStrayLightCoeffs = new System.Windows.Forms.Button();
            this.label54 = new System.Windows.Forms.Label();
            this.label55 = new System.Windows.Forms.Label();
            this.comboBoxWavecalCoeffIndex = new System.Windows.Forms.ComboBox();
            this.textBoxNumWavecalCoeffs = new System.Windows.Forms.TextBox();
            this.textBoxWavecalCoeff = new System.Windows.Forms.TextBox();
            this.buttonWriteWavecalCoeff = new System.Windows.Forms.Button();
            this.buttonReadWavecalCoeff = new System.Windows.Forms.Button();
            this.buttonReadNumWavecalCoeffs = new System.Windows.Forms.Button();
            this.label52 = new System.Windows.Forms.Label();
            this.label53 = new System.Windows.Forms.Label();
            this.comboBoxNonLinearityCoeffIndex = new System.Windows.Forms.ComboBox();
            this.textBoxNumNonLinearityCoeffs = new System.Windows.Forms.TextBox();
            this.textBoxNonLinearityCoeff = new System.Windows.Forms.TextBox();
            this.buttonWriteNonLinearityCoeff = new System.Windows.Forms.Button();
            this.buttonReadNonLinearityCoeff = new System.Windows.Forms.Button();
            this.buttonReadNumNonLinearityCoeffs = new System.Windows.Forms.Button();
            this.label50 = new System.Windows.Forms.Label();
            this.label51 = new System.Windows.Forms.Label();
            this.tabPageTecTemp = new System.Windows.Forms.TabPage();
            this.groupBoxTec = new System.Windows.Forms.GroupBox();
            this.buttonReadTecSetpoint = new System.Windows.Forms.Button();
            this.textBoxTecTemperature = new System.Windows.Forms.TextBox();
            this.label70 = new System.Windows.Forms.Label();
            this.buttonReadTecTemperature = new System.Windows.Forms.Button();
            this.buttonReadTecEnable = new System.Windows.Forms.Button();
            this.label73 = new System.Windows.Forms.Label();
            this.buttonWriteTecEnable = new System.Windows.Forms.Button();
            this.textBoxTecStable = new System.Windows.Forms.TextBox();
            this.textBoxTecEnable = new System.Windows.Forms.TextBox();
            this.buttonReadTecStable = new System.Windows.Forms.Button();
            this.label71 = new System.Windows.Forms.Label();
            this.label72 = new System.Windows.Forms.Label();
            this.buttonWriteTecSetpoint = new System.Windows.Forms.Button();
            this.textBoxTecSetpoint = new System.Windows.Forms.TextBox();
            this.label69 = new System.Windows.Forms.Label();
            this.buttonReadAllTempSensors = new System.Windows.Forms.Button();
            this.textBoxAllTempSensors = new System.Windows.Forms.TextBox();
            this.comboBoxTempSensorIndex = new System.Windows.Forms.ComboBox();
            this.textBoxNumTempSensors = new System.Windows.Forms.TextBox();
            this.textBoxTempSensorValue = new System.Windows.Forms.TextBox();
            this.buttonReadTempSensor = new System.Windows.Forms.Button();
            this.buttonReadNumTempSensors = new System.Windows.Forms.Button();
            this.label67 = new System.Windows.Forms.Label();
            this.label68 = new System.Windows.Forms.Label();
            this.tabPageGPIO = new System.Windows.Forms.TabPage();
            this.label78 = new System.Windows.Forms.Label();
            this.textBoxGPIOOutputEnableMask = new System.Windows.Forms.TextBox();
            this.label77 = new System.Windows.Forms.Label();
            this.textBoxGPIOValueMask = new System.Windows.Forms.TextBox();
            this.textBoxGPIOValue = new System.Windows.Forms.TextBox();
            this.buttonWriteGPIOValue = new System.Windows.Forms.Button();
            this.buttonReadGPIOValue = new System.Windows.Forms.Button();
            this.label76 = new System.Windows.Forms.Label();
            this.textBoxGPIOOutputEnable = new System.Windows.Forms.TextBox();
            this.buttonWriteGPIOOutputEnable = new System.Windows.Forms.Button();
            this.buttonReadGPIOOutputEnable = new System.Windows.Forms.Button();
            this.label75 = new System.Windows.Forms.Label();
            this.textBoxNumGPIOPins = new System.Windows.Forms.TextBox();
            this.buttonReadNumGPIOPins = new System.Windows.Forms.Button();
            this.label74 = new System.Windows.Forms.Label();
            this.tabPageI2C = new System.Windows.Forms.TabPage();
            this.label87 = new System.Windows.Forms.Label();
            this.label86 = new System.Windows.Forms.Label();
            this.label85 = new System.Windows.Forms.Label();
            this.comboBoxI2CClockBusNum = new System.Windows.Forms.ComboBox();
            this.textBoxI2CClockLimit = new System.Windows.Forms.TextBox();
            this.buttonWriteI2CClockLimit = new System.Windows.Forms.Button();
            this.label84 = new System.Windows.Forms.Label();
            this.textBoxI2CData = new System.Windows.Forms.TextBox();
            this.label83 = new System.Windows.Forms.Label();
            this.textBoxI2CNumBytes = new System.Windows.Forms.TextBox();
            this.label82 = new System.Windows.Forms.Label();
            this.label81 = new System.Windows.Forms.Label();
            this.textBoxI2CNumBuses = new System.Windows.Forms.TextBox();
            this.buttonReadI2CNumBuses = new System.Windows.Forms.Button();
            this.label80 = new System.Windows.Forms.Label();
            this.comboBoxI2CDataBusNum = new System.Windows.Forms.ComboBox();
            this.textBoxI2CAddress = new System.Windows.Forms.TextBox();
            this.buttonWriteI2CBytes = new System.Windows.Forms.Button();
            this.buttonReadI2CBytes = new System.Windows.Forms.Button();
            this.label79 = new System.Windows.Forms.Label();
            this.checkBoxMD5Checksum = new System.Windows.Forms.CheckBox();
            this.textBoxRegarding = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.tabControlProtocol = new System.Windows.Forms.TabControl();
            this.tabPageRS232 = new System.Windows.Forms.TabPage();
            this.tabPageTCPIP = new System.Windows.Forms.TabPage();
            this.label14 = new System.Windows.Forms.Label();
            this.buttonDisconnect = new System.Windows.Forms.Button();
            this.textBoxPortNum = new System.Windows.Forms.TextBox();
            this.textBoxIPAddress = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.labelIPAddress = new System.Windows.Forms.Label();
            this.labelPortNum = new System.Windows.Forms.Label();
            this.tabPageWinUSBNet = new System.Windows.Forms.TabPage();
            this.dataGridViewOutPipes = new System.Windows.Forms.DataGridView();
            this.dataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewInPipes = new System.Windows.Forms.DataGridView();
            this.ColumnPipes = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.label89 = new System.Windows.Forms.Label();
            this.label88 = new System.Windows.Forms.Label();
            this.dataGridViewUSBDeviceList = new System.Windows.Forms.DataGridView();
            this.ColumnDeviceDescription = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColumnProductID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tabControlContent.SuspendLayout();
            this.tabPageIOLog.SuspendLayout();
            this.tabPageInfo.SuspendLayout();
            this.tabPageAcquisitionParams.SuspendLayout();
            this.tabPageSpectrum.SuspendLayout();
            this.tabControlSpectrumInfo.SuspendLayout();
            this.tabPageChart.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chartSpectrum)).BeginInit();
            this.tabPageData.SuspendLayout();
            this.groupBoxMetadata.SuspendLayout();
            this.tabPageBuffer.SuspendLayout();
            this.tabPageCoefficients.SuspendLayout();
            this.tabPageTecTemp.SuspendLayout();
            this.groupBoxTec.SuspendLayout();
            this.tabPageGPIO.SuspendLayout();
            this.tabPageI2C.SuspendLayout();
            this.tabControlProtocol.SuspendLayout();
            this.tabPageRS232.SuspendLayout();
            this.tabPageTCPIP.SuspendLayout();
            this.tabPageWinUSBNet.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewOutPipes)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewInPipes)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewUSBDeviceList)).BeginInit();
            this.SuspendLayout();
            // 
            // comboBoxComPort
            // 
            this.comboBoxComPort.FormattingEnabled = true;
            this.comboBoxComPort.Location = new System.Drawing.Point(6, 27);
            this.comboBoxComPort.Name = "comboBoxComPort";
            this.comboBoxComPort.Size = new System.Drawing.Size(80, 21);
            this.comboBoxComPort.TabIndex = 0;
            this.comboBoxComPort.SelectedIndexChanged += new System.EventHandler(this.comboBoxComPort_SelectedIndexChanged);
            // 
            // comboBoxDataBits
            // 
            this.comboBoxDataBits.FormattingEnabled = true;
            this.comboBoxDataBits.Location = new System.Drawing.Point(195, 26);
            this.comboBoxDataBits.Name = "comboBoxDataBits";
            this.comboBoxDataBits.Size = new System.Drawing.Size(63, 21);
            this.comboBoxDataBits.TabIndex = 1;
            this.comboBoxDataBits.SelectedIndexChanged += new System.EventHandler(this.comboBoxDataBits_SelectedIndexChanged);
            // 
            // comboBoxParity
            // 
            this.comboBoxParity.FormattingEnabled = true;
            this.comboBoxParity.Location = new System.Drawing.Point(276, 26);
            this.comboBoxParity.Name = "comboBoxParity";
            this.comboBoxParity.Size = new System.Drawing.Size(61, 21);
            this.comboBoxParity.TabIndex = 2;
            this.comboBoxParity.SelectedIndexChanged += new System.EventHandler(this.comboBoxParity_SelectedIndexChanged);
            // 
            // comboBoxStopBits
            // 
            this.comboBoxStopBits.FormattingEnabled = true;
            this.comboBoxStopBits.Location = new System.Drawing.Point(356, 25);
            this.comboBoxStopBits.Name = "comboBoxStopBits";
            this.comboBoxStopBits.Size = new System.Drawing.Size(50, 21);
            this.comboBoxStopBits.TabIndex = 3;
            this.comboBoxStopBits.SelectedIndexChanged += new System.EventHandler(this.comboBoxStopBits_SelectedIndexChanged);
            // 
            // comboBoxHandshaking
            // 
            this.comboBoxHandshaking.FormattingEnabled = true;
            this.comboBoxHandshaking.Location = new System.Drawing.Point(422, 25);
            this.comboBoxHandshaking.Name = "comboBoxHandshaking";
            this.comboBoxHandshaking.Size = new System.Drawing.Size(133, 21);
            this.comboBoxHandshaking.TabIndex = 4;
            this.comboBoxHandshaking.SelectedIndexChanged += new System.EventHandler(this.comboBoxHandshaking_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 8);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "COM Port";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(201, 8);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(50, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Data Bits";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(289, 7);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(33, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Parity";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(357, 7);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(49, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Stop Bits";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(447, 7);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(70, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "Handshaking";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(3)))), ((int)(((byte)(103)))), ((int)(((byte)(177)))));
            this.label6.Location = new System.Drawing.Point(253, 6);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(327, 24);
            this.label6.TabIndex = 10;
            this.label6.Text = "Test Ocean Binary Protocol (OBP)";
            // 
            // comboBoxBaudRate
            // 
            this.comboBoxBaudRate.FormattingEnabled = true;
            this.comboBoxBaudRate.Location = new System.Drawing.Point(103, 27);
            this.comboBoxBaudRate.Name = "comboBoxBaudRate";
            this.comboBoxBaudRate.Size = new System.Drawing.Size(74, 21);
            this.comboBoxBaudRate.TabIndex = 11;
            this.comboBoxBaudRate.SelectedIndexChanged += new System.EventHandler(this.comboBoxBaudRate_SelectedIndexChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(110, 7);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(58, 13);
            this.label7.TabIndex = 12;
            this.label7.Text = "Baud Rate";
            // 
            // buttonTest
            // 
            this.buttonTest.Location = new System.Drawing.Point(25, 215);
            this.buttonTest.Name = "buttonTest";
            this.buttonTest.Size = new System.Drawing.Size(121, 23);
            this.buttonTest.TabIndex = 13;
            this.buttonTest.Text = "Connect and Test";
            this.buttonTest.UseVisualStyleBackColor = true;
            this.buttonTest.Click += new System.EventHandler(this.buttonTest_Click);
            // 
            // buttonSyncFW
            // 
            this.buttonSyncFW.Location = new System.Drawing.Point(152, 215);
            this.buttonSyncFW.Name = "buttonSyncFW";
            this.buttonSyncFW.Size = new System.Drawing.Size(111, 23);
            this.buttonSyncFW.TabIndex = 14;
            this.buttonSyncFW.Text = "Sync FW Settings";
            this.buttonSyncFW.UseVisualStyleBackColor = true;
            this.buttonSyncFW.Click += new System.EventHandler(this.buttonSyncFW_Click);
            // 
            // labelPortName
            // 
            this.labelPortName.AutoSize = true;
            this.labelPortName.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelPortName.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.labelPortName.Location = new System.Drawing.Point(23, 49);
            this.labelPortName.Name = "labelPortName";
            this.labelPortName.Size = new System.Drawing.Size(33, 12);
            this.labelPortName.TabIndex = 15;
            this.labelPortName.Text = "COM1";
            // 
            // labelBaudRate
            // 
            this.labelBaudRate.AutoSize = true;
            this.labelBaudRate.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelBaudRate.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.labelBaudRate.Location = new System.Drawing.Point(111, 49);
            this.labelBaudRate.Name = "labelBaudRate";
            this.labelBaudRate.Size = new System.Drawing.Size(25, 12);
            this.labelBaudRate.TabIndex = 16;
            this.labelBaudRate.Text = "9600";
            // 
            // labelDataBits
            // 
            this.labelDataBits.AutoSize = true;
            this.labelDataBits.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelDataBits.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.labelDataBits.Location = new System.Drawing.Point(218, 49);
            this.labelDataBits.Name = "labelDataBits";
            this.labelDataBits.Size = new System.Drawing.Size(10, 12);
            this.labelDataBits.TabIndex = 17;
            this.labelDataBits.Text = "8";
            // 
            // labelParity
            // 
            this.labelParity.AutoSize = true;
            this.labelParity.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelParity.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.labelParity.Location = new System.Drawing.Point(286, 49);
            this.labelParity.Name = "labelParity";
            this.labelParity.Size = new System.Drawing.Size(27, 12);
            this.labelParity.TabIndex = 18;
            this.labelParity.Text = "None";
            // 
            // labelStopBits
            // 
            this.labelStopBits.AutoSize = true;
            this.labelStopBits.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelStopBits.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.labelStopBits.Location = new System.Drawing.Point(372, 49);
            this.labelStopBits.Name = "labelStopBits";
            this.labelStopBits.Size = new System.Drawing.Size(10, 12);
            this.labelStopBits.TabIndex = 19;
            this.labelStopBits.Text = "1";
            // 
            // labelHandshaking
            // 
            this.labelHandshaking.AutoSize = true;
            this.labelHandshaking.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelHandshaking.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.labelHandshaking.Location = new System.Drawing.Point(434, 49);
            this.labelHandshaking.Name = "labelHandshaking";
            this.labelHandshaking.Size = new System.Drawing.Size(27, 12);
            this.labelHandshaking.TabIndex = 20;
            this.labelHandshaking.Text = "None";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(46, 260);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(54, 13);
            this.label8.TabIndex = 21;
            this.label8.Text = "IO Status:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(35, 277);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(65, 13);
            this.label9.TabIndex = 22;
            this.label9.Text = "OBP Status:";
            // 
            // labelIOStatus
            // 
            this.labelIOStatus.AutoSize = true;
            this.labelIOStatus.Location = new System.Drawing.Point(107, 260);
            this.labelIOStatus.Name = "labelIOStatus";
            this.labelIOStatus.Size = new System.Drawing.Size(16, 13);
            this.labelIOStatus.TabIndex = 23;
            this.labelIOStatus.Text = "---";
            // 
            // labelOBPStatus
            // 
            this.labelOBPStatus.AutoSize = true;
            this.labelOBPStatus.Location = new System.Drawing.Point(107, 277);
            this.labelOBPStatus.Name = "labelOBPStatus";
            this.labelOBPStatus.Size = new System.Drawing.Size(16, 13);
            this.labelOBPStatus.TabIndex = 24;
            this.labelOBPStatus.Text = "---";
            // 
            // checkBoxAckAlways
            // 
            this.checkBoxAckAlways.AutoSize = true;
            this.checkBoxAckAlways.Location = new System.Drawing.Point(269, 208);
            this.checkBoxAckAlways.Name = "checkBoxAckAlways";
            this.checkBoxAckAlways.Size = new System.Drawing.Size(81, 17);
            this.checkBoxAckAlways.TabIndex = 25;
            this.checkBoxAckAlways.Text = "Ack Always";
            this.checkBoxAckAlways.UseVisualStyleBackColor = true;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(376, 209);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(84, 13);
            this.label10.TabIndex = 26;
            this.label10.Text = "Protocol Version";
            // 
            // textBoxProtocolVersion
            // 
            this.textBoxProtocolVersion.Location = new System.Drawing.Point(463, 206);
            this.textBoxProtocolVersion.Name = "textBoxProtocolVersion";
            this.textBoxProtocolVersion.Size = new System.Drawing.Size(105, 20);
            this.textBoxProtocolVersion.TabIndex = 27;
            this.textBoxProtocolVersion.TextChanged += new System.EventHandler(this.textBoxProtocolVersion_TextChanged);
            // 
            // tabControlContent
            // 
            this.tabControlContent.Controls.Add(this.tabPageIOLog);
            this.tabControlContent.Controls.Add(this.tabPageInfo);
            this.tabControlContent.Controls.Add(this.tabPageAcquisitionParams);
            this.tabControlContent.Controls.Add(this.tabPageSpectrum);
            this.tabControlContent.Controls.Add(this.tabPageCoefficients);
            this.tabControlContent.Controls.Add(this.tabPageTecTemp);
            this.tabControlContent.Controls.Add(this.tabPageGPIO);
            this.tabControlContent.Controls.Add(this.tabPageI2C);
            this.tabControlContent.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.tabControlContent.Enabled = false;
            this.tabControlContent.Location = new System.Drawing.Point(0, 309);
            this.tabControlContent.Name = "tabControlContent";
            this.tabControlContent.SelectedIndex = 0;
            this.tabControlContent.Size = new System.Drawing.Size(734, 403);
            this.tabControlContent.TabIndex = 28;
            // 
            // tabPageIOLog
            // 
            this.tabPageIOLog.BackColor = System.Drawing.SystemColors.Control;
            this.tabPageIOLog.Controls.Add(this.buttonClearLog);
            this.tabPageIOLog.Controls.Add(this.textBoxIOLog);
            this.tabPageIOLog.Location = new System.Drawing.Point(4, 22);
            this.tabPageIOLog.Name = "tabPageIOLog";
            this.tabPageIOLog.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageIOLog.Size = new System.Drawing.Size(726, 377);
            this.tabPageIOLog.TabIndex = 0;
            this.tabPageIOLog.Text = "IO Log";
            // 
            // buttonClearLog
            // 
            this.buttonClearLog.Location = new System.Drawing.Point(606, 16);
            this.buttonClearLog.Name = "buttonClearLog";
            this.buttonClearLog.Size = new System.Drawing.Size(75, 23);
            this.buttonClearLog.TabIndex = 1;
            this.buttonClearLog.Text = "Clear Log";
            this.buttonClearLog.UseVisualStyleBackColor = true;
            this.buttonClearLog.Click += new System.EventHandler(this.buttonClearLog_Click);
            // 
            // textBoxIOLog
            // 
            this.textBoxIOLog.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBoxIOLog.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxIOLog.Location = new System.Drawing.Point(3, 3);
            this.textBoxIOLog.Multiline = true;
            this.textBoxIOLog.Name = "textBoxIOLog";
            this.textBoxIOLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxIOLog.Size = new System.Drawing.Size(570, 371);
            this.textBoxIOLog.TabIndex = 0;
            // 
            // tabPageInfo
            // 
            this.tabPageInfo.BackColor = System.Drawing.SystemColors.Control;
            this.tabPageInfo.Controls.Add(this.textBoxDetectorSerialNum);
            this.tabPageInfo.Controls.Add(this.buttonReadDetectorSerialNum);
            this.tabPageInfo.Controls.Add(this.label33);
            this.tabPageInfo.Controls.Add(this.textBoxCoating);
            this.tabPageInfo.Controls.Add(this.buttonReadCoating);
            this.tabPageInfo.Controls.Add(this.label32);
            this.tabPageInfo.Controls.Add(this.textBoxFilter);
            this.tabPageInfo.Controls.Add(this.buttonReadFilter);
            this.tabPageInfo.Controls.Add(this.label31);
            this.tabPageInfo.Controls.Add(this.textBoxGrating);
            this.tabPageInfo.Controls.Add(this.buttonReadGrating);
            this.tabPageInfo.Controls.Add(this.label30);
            this.tabPageInfo.Controls.Add(this.textBoxFiberDiameter);
            this.tabPageInfo.Controls.Add(this.buttonReadFiberDiameter);
            this.tabPageInfo.Controls.Add(this.label29);
            this.tabPageInfo.Controls.Add(this.textBoxSlitWidth);
            this.tabPageInfo.Controls.Add(this.buttonReadSlitWidth);
            this.tabPageInfo.Controls.Add(this.label28);
            this.tabPageInfo.Controls.Add(this.textBoxBenchSerialNum);
            this.tabPageInfo.Controls.Add(this.buttonReadBenchSerialNum);
            this.tabPageInfo.Controls.Add(this.label27);
            this.tabPageInfo.Controls.Add(this.textBoxBenchId);
            this.tabPageInfo.Controls.Add(this.buttonReadBenchId);
            this.tabPageInfo.Controls.Add(this.label26);
            this.tabPageInfo.Controls.Add(this.buttonSaveRS232Settings);
            this.tabPageInfo.Controls.Add(this.buttonResetDefaults);
            this.tabPageInfo.Controls.Add(this.buttonResetSpectrometer);
            this.tabPageInfo.Controls.Add(this.textBoxStatusLED);
            this.tabPageInfo.Controls.Add(this.buttonWriteStatusLED);
            this.tabPageInfo.Controls.Add(this.label25);
            this.tabPageInfo.Controls.Add(this.textBoxSerialNumLen);
            this.tabPageInfo.Controls.Add(this.buttonReadSerialNumLen);
            this.tabPageInfo.Controls.Add(this.label24);
            this.tabPageInfo.Controls.Add(this.textBoxFPGARevision);
            this.tabPageInfo.Controls.Add(this.buttonReadFPGARevision);
            this.tabPageInfo.Controls.Add(this.label23);
            this.tabPageInfo.Controls.Add(this.comboBoxUserStringNum);
            this.tabPageInfo.Controls.Add(this.textBoxUserStringLen);
            this.tabPageInfo.Controls.Add(this.textBoxNumUserStrings);
            this.tabPageInfo.Controls.Add(this.textBoxUserString);
            this.tabPageInfo.Controls.Add(this.buttonWriteUserString);
            this.tabPageInfo.Controls.Add(this.buttonReadUserString);
            this.tabPageInfo.Controls.Add(this.buttonReadUserStringLen);
            this.tabPageInfo.Controls.Add(this.buttonReadNumUserStrings);
            this.tabPageInfo.Controls.Add(this.textBoxDeviceAlias);
            this.tabPageInfo.Controls.Add(this.buttonWriteDeviceAlias);
            this.tabPageInfo.Controls.Add(this.buttonReadDeviceAlias);
            this.tabPageInfo.Controls.Add(this.textBoxDeviceAliasLen);
            this.tabPageInfo.Controls.Add(this.buttonReadAliasLen);
            this.tabPageInfo.Controls.Add(this.textBoxFwRevision);
            this.tabPageInfo.Controls.Add(this.textBoxHwRevision);
            this.tabPageInfo.Controls.Add(this.buttonReadFWRevision);
            this.tabPageInfo.Controls.Add(this.textBoxSerialNum);
            this.tabPageInfo.Controls.Add(this.buttonReadHWRevision);
            this.tabPageInfo.Controls.Add(this.buttonReadSerialNum);
            this.tabPageInfo.Controls.Add(this.label22);
            this.tabPageInfo.Controls.Add(this.label21);
            this.tabPageInfo.Controls.Add(this.label20);
            this.tabPageInfo.Controls.Add(this.label19);
            this.tabPageInfo.Controls.Add(this.label18);
            this.tabPageInfo.Controls.Add(this.label17);
            this.tabPageInfo.Controls.Add(this.label16);
            this.tabPageInfo.Controls.Add(this.label15);
            this.tabPageInfo.Location = new System.Drawing.Point(4, 22);
            this.tabPageInfo.Name = "tabPageInfo";
            this.tabPageInfo.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageInfo.Size = new System.Drawing.Size(726, 377);
            this.tabPageInfo.TabIndex = 1;
            this.tabPageInfo.Text = "Basic Info";
            // 
            // textBoxDetectorSerialNum
            // 
            this.textBoxDetectorSerialNum.Location = new System.Drawing.Point(554, 178);
            this.textBoxDetectorSerialNum.Name = "textBoxDetectorSerialNum";
            this.textBoxDetectorSerialNum.ReadOnly = true;
            this.textBoxDetectorSerialNum.Size = new System.Drawing.Size(116, 20);
            this.textBoxDetectorSerialNum.TabIndex = 64;
            // 
            // buttonReadDetectorSerialNum
            // 
            this.buttonReadDetectorSerialNum.Location = new System.Drawing.Point(482, 177);
            this.buttonReadDetectorSerialNum.Name = "buttonReadDetectorSerialNum";
            this.buttonReadDetectorSerialNum.Size = new System.Drawing.Size(29, 21);
            this.buttonReadDetectorSerialNum.TabIndex = 63;
            this.buttonReadDetectorSerialNum.Text = "R";
            this.buttonReadDetectorSerialNum.UseVisualStyleBackColor = true;
            this.buttonReadDetectorSerialNum.Click += new System.EventHandler(this.buttonReadDetectorSerialNum_Click);
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.Location = new System.Drawing.Point(377, 181);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(102, 13);
            this.label33.TabIndex = 62;
            this.label33.Text = "Detector Serial Num";
            // 
            // textBoxCoating
            // 
            this.textBoxCoating.Location = new System.Drawing.Point(554, 155);
            this.textBoxCoating.Name = "textBoxCoating";
            this.textBoxCoating.ReadOnly = true;
            this.textBoxCoating.Size = new System.Drawing.Size(116, 20);
            this.textBoxCoating.TabIndex = 61;
            // 
            // buttonReadCoating
            // 
            this.buttonReadCoating.Location = new System.Drawing.Point(482, 154);
            this.buttonReadCoating.Name = "buttonReadCoating";
            this.buttonReadCoating.Size = new System.Drawing.Size(29, 21);
            this.buttonReadCoating.TabIndex = 60;
            this.buttonReadCoating.Text = "R";
            this.buttonReadCoating.UseVisualStyleBackColor = true;
            this.buttonReadCoating.Click += new System.EventHandler(this.buttonReadCoating_Click);
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.Location = new System.Drawing.Point(436, 158);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(43, 13);
            this.label32.TabIndex = 59;
            this.label32.Text = "Coating";
            // 
            // textBoxFilter
            // 
            this.textBoxFilter.Location = new System.Drawing.Point(554, 132);
            this.textBoxFilter.Name = "textBoxFilter";
            this.textBoxFilter.ReadOnly = true;
            this.textBoxFilter.Size = new System.Drawing.Size(116, 20);
            this.textBoxFilter.TabIndex = 58;
            // 
            // buttonReadFilter
            // 
            this.buttonReadFilter.Location = new System.Drawing.Point(482, 131);
            this.buttonReadFilter.Name = "buttonReadFilter";
            this.buttonReadFilter.Size = new System.Drawing.Size(29, 21);
            this.buttonReadFilter.TabIndex = 57;
            this.buttonReadFilter.Text = "R";
            this.buttonReadFilter.UseVisualStyleBackColor = true;
            this.buttonReadFilter.Click += new System.EventHandler(this.buttonReadFilter_Click);
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.Location = new System.Drawing.Point(450, 135);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(29, 13);
            this.label31.TabIndex = 56;
            this.label31.Text = "Filter";
            // 
            // textBoxGrating
            // 
            this.textBoxGrating.Location = new System.Drawing.Point(554, 109);
            this.textBoxGrating.Name = "textBoxGrating";
            this.textBoxGrating.ReadOnly = true;
            this.textBoxGrating.Size = new System.Drawing.Size(116, 20);
            this.textBoxGrating.TabIndex = 55;
            // 
            // buttonReadGrating
            // 
            this.buttonReadGrating.Location = new System.Drawing.Point(482, 108);
            this.buttonReadGrating.Name = "buttonReadGrating";
            this.buttonReadGrating.Size = new System.Drawing.Size(29, 21);
            this.buttonReadGrating.TabIndex = 54;
            this.buttonReadGrating.Text = "R";
            this.buttonReadGrating.UseVisualStyleBackColor = true;
            this.buttonReadGrating.Click += new System.EventHandler(this.buttonReadGrating_Click);
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.Location = new System.Drawing.Point(438, 112);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(41, 13);
            this.label30.TabIndex = 53;
            this.label30.Text = "Grating";
            // 
            // textBoxFiberDiameter
            // 
            this.textBoxFiberDiameter.Location = new System.Drawing.Point(554, 85);
            this.textBoxFiberDiameter.Name = "textBoxFiberDiameter";
            this.textBoxFiberDiameter.ReadOnly = true;
            this.textBoxFiberDiameter.Size = new System.Drawing.Size(116, 20);
            this.textBoxFiberDiameter.TabIndex = 52;
            // 
            // buttonReadFiberDiameter
            // 
            this.buttonReadFiberDiameter.Location = new System.Drawing.Point(482, 84);
            this.buttonReadFiberDiameter.Name = "buttonReadFiberDiameter";
            this.buttonReadFiberDiameter.Size = new System.Drawing.Size(29, 21);
            this.buttonReadFiberDiameter.TabIndex = 51;
            this.buttonReadFiberDiameter.Text = "R";
            this.buttonReadFiberDiameter.UseVisualStyleBackColor = true;
            this.buttonReadFiberDiameter.Click += new System.EventHandler(this.buttonReadFiberDiameter_Click);
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Location = new System.Drawing.Point(404, 88);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(75, 13);
            this.label29.TabIndex = 50;
            this.label29.Text = "Fiber Diameter";
            // 
            // textBoxSlitWidth
            // 
            this.textBoxSlitWidth.Location = new System.Drawing.Point(554, 62);
            this.textBoxSlitWidth.Name = "textBoxSlitWidth";
            this.textBoxSlitWidth.ReadOnly = true;
            this.textBoxSlitWidth.Size = new System.Drawing.Size(116, 20);
            this.textBoxSlitWidth.TabIndex = 49;
            // 
            // buttonReadSlitWidth
            // 
            this.buttonReadSlitWidth.Location = new System.Drawing.Point(482, 61);
            this.buttonReadSlitWidth.Name = "buttonReadSlitWidth";
            this.buttonReadSlitWidth.Size = new System.Drawing.Size(29, 21);
            this.buttonReadSlitWidth.TabIndex = 48;
            this.buttonReadSlitWidth.Text = "R";
            this.buttonReadSlitWidth.UseVisualStyleBackColor = true;
            this.buttonReadSlitWidth.Click += new System.EventHandler(this.buttonReadSlitWidth_Click);
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Location = new System.Drawing.Point(427, 65);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(52, 13);
            this.label28.TabIndex = 47;
            this.label28.Text = "Slit Width";
            // 
            // textBoxBenchSerialNum
            // 
            this.textBoxBenchSerialNum.Location = new System.Drawing.Point(554, 39);
            this.textBoxBenchSerialNum.Name = "textBoxBenchSerialNum";
            this.textBoxBenchSerialNum.ReadOnly = true;
            this.textBoxBenchSerialNum.Size = new System.Drawing.Size(116, 20);
            this.textBoxBenchSerialNum.TabIndex = 46;
            // 
            // buttonReadBenchSerialNum
            // 
            this.buttonReadBenchSerialNum.Location = new System.Drawing.Point(482, 38);
            this.buttonReadBenchSerialNum.Name = "buttonReadBenchSerialNum";
            this.buttonReadBenchSerialNum.Size = new System.Drawing.Size(29, 21);
            this.buttonReadBenchSerialNum.TabIndex = 45;
            this.buttonReadBenchSerialNum.Text = "R";
            this.buttonReadBenchSerialNum.UseVisualStyleBackColor = true;
            this.buttonReadBenchSerialNum.Click += new System.EventHandler(this.buttonReadBenchSerialNum_Click);
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.Location = new System.Drawing.Point(387, 42);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(92, 13);
            this.label27.TabIndex = 44;
            this.label27.Text = "Bench Serial Num";
            // 
            // textBoxBenchId
            // 
            this.textBoxBenchId.Location = new System.Drawing.Point(554, 16);
            this.textBoxBenchId.Name = "textBoxBenchId";
            this.textBoxBenchId.ReadOnly = true;
            this.textBoxBenchId.Size = new System.Drawing.Size(116, 20);
            this.textBoxBenchId.TabIndex = 43;
            // 
            // buttonReadBenchId
            // 
            this.buttonReadBenchId.Location = new System.Drawing.Point(482, 15);
            this.buttonReadBenchId.Name = "buttonReadBenchId";
            this.buttonReadBenchId.Size = new System.Drawing.Size(29, 21);
            this.buttonReadBenchId.TabIndex = 42;
            this.buttonReadBenchId.Text = "R";
            this.buttonReadBenchId.UseVisualStyleBackColor = true;
            this.buttonReadBenchId.Click += new System.EventHandler(this.buttonReadBenchId_Click);
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(427, 19);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(52, 13);
            this.label26.TabIndex = 41;
            this.label26.Text = "Bench ID";
            // 
            // buttonSaveRS232Settings
            // 
            this.buttonSaveRS232Settings.Location = new System.Drawing.Point(310, 334);
            this.buttonSaveRS232Settings.Name = "buttonSaveRS232Settings";
            this.buttonSaveRS232Settings.Size = new System.Drawing.Size(123, 23);
            this.buttonSaveRS232Settings.TabIndex = 40;
            this.buttonSaveRS232Settings.Text = "Save RS232 Settings";
            this.buttonSaveRS232Settings.UseVisualStyleBackColor = true;
            this.buttonSaveRS232Settings.Click += new System.EventHandler(this.buttonSaveRS232Settings_Click);
            // 
            // buttonResetDefaults
            // 
            this.buttonResetDefaults.Location = new System.Drawing.Point(184, 334);
            this.buttonResetDefaults.Name = "buttonResetDefaults";
            this.buttonResetDefaults.Size = new System.Drawing.Size(108, 23);
            this.buttonResetDefaults.TabIndex = 39;
            this.buttonResetDefaults.Text = "Reset Defaults";
            this.buttonResetDefaults.UseVisualStyleBackColor = true;
            this.buttonResetDefaults.Click += new System.EventHandler(this.buttonResetDefaults_Click);
            // 
            // buttonResetSpectrometer
            // 
            this.buttonResetSpectrometer.Location = new System.Drawing.Point(54, 334);
            this.buttonResetSpectrometer.Name = "buttonResetSpectrometer";
            this.buttonResetSpectrometer.Size = new System.Drawing.Size(114, 23);
            this.buttonResetSpectrometer.TabIndex = 38;
            this.buttonResetSpectrometer.Text = "Reset Spectrometer";
            this.buttonResetSpectrometer.UseVisualStyleBackColor = true;
            this.buttonResetSpectrometer.Click += new System.EventHandler(this.buttonResetSpectrometer_Click);
            // 
            // textBoxStatusLED
            // 
            this.textBoxStatusLED.Location = new System.Drawing.Point(176, 243);
            this.textBoxStatusLED.Name = "textBoxStatusLED";
            this.textBoxStatusLED.Size = new System.Drawing.Size(116, 20);
            this.textBoxStatusLED.TabIndex = 37;
            // 
            // buttonWriteStatusLED
            // 
            this.buttonWriteStatusLED.Location = new System.Drawing.Point(139, 242);
            this.buttonWriteStatusLED.Name = "buttonWriteStatusLED";
            this.buttonWriteStatusLED.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteStatusLED.TabIndex = 36;
            this.buttonWriteStatusLED.Text = "W";
            this.buttonWriteStatusLED.UseVisualStyleBackColor = true;
            this.buttonWriteStatusLED.Click += new System.EventHandler(this.buttonWriteStatusLED_Click);
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(40, 246);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(61, 13);
            this.label25.TabIndex = 34;
            this.label25.Text = "Status LED";
            // 
            // textBoxSerialNumLen
            // 
            this.textBoxSerialNumLen.Location = new System.Drawing.Point(176, 12);
            this.textBoxSerialNumLen.Name = "textBoxSerialNumLen";
            this.textBoxSerialNumLen.ReadOnly = true;
            this.textBoxSerialNumLen.Size = new System.Drawing.Size(116, 20);
            this.textBoxSerialNumLen.TabIndex = 33;
            // 
            // buttonReadSerialNumLen
            // 
            this.buttonReadSerialNumLen.Location = new System.Drawing.Point(104, 11);
            this.buttonReadSerialNumLen.Name = "buttonReadSerialNumLen";
            this.buttonReadSerialNumLen.Size = new System.Drawing.Size(29, 21);
            this.buttonReadSerialNumLen.TabIndex = 32;
            this.buttonReadSerialNumLen.Text = "R";
            this.buttonReadSerialNumLen.UseVisualStyleBackColor = true;
            this.buttonReadSerialNumLen.Click += new System.EventHandler(this.buttonReadSerialNumLen_Click);
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(22, 15);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(79, 13);
            this.label24.TabIndex = 31;
            this.label24.Text = "Serial Num Len";
            // 
            // textBoxFPGARevision
            // 
            this.textBoxFPGARevision.Location = new System.Drawing.Point(176, 105);
            this.textBoxFPGARevision.Name = "textBoxFPGARevision";
            this.textBoxFPGARevision.ReadOnly = true;
            this.textBoxFPGARevision.Size = new System.Drawing.Size(116, 20);
            this.textBoxFPGARevision.TabIndex = 30;
            // 
            // buttonReadFPGARevision
            // 
            this.buttonReadFPGARevision.Location = new System.Drawing.Point(104, 104);
            this.buttonReadFPGARevision.Name = "buttonReadFPGARevision";
            this.buttonReadFPGARevision.Size = new System.Drawing.Size(29, 21);
            this.buttonReadFPGARevision.TabIndex = 29;
            this.buttonReadFPGARevision.Text = "R";
            this.buttonReadFPGARevision.UseVisualStyleBackColor = true;
            this.buttonReadFPGARevision.Click += new System.EventHandler(this.buttonReadFPGARevision_Click);
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(22, 108);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(79, 13);
            this.label23.TabIndex = 28;
            this.label23.Text = "FPGA Revision";
            // 
            // comboBoxUserStringNum
            // 
            this.comboBoxUserStringNum.Enabled = false;
            this.comboBoxUserStringNum.FormattingEnabled = true;
            this.comboBoxUserStringNum.Location = new System.Drawing.Point(176, 220);
            this.comboBoxUserStringNum.Name = "comboBoxUserStringNum";
            this.comboBoxUserStringNum.Size = new System.Drawing.Size(44, 21);
            this.comboBoxUserStringNum.TabIndex = 27;
            // 
            // textBoxUserStringLen
            // 
            this.textBoxUserStringLen.Location = new System.Drawing.Point(176, 197);
            this.textBoxUserStringLen.Name = "textBoxUserStringLen";
            this.textBoxUserStringLen.ReadOnly = true;
            this.textBoxUserStringLen.Size = new System.Drawing.Size(116, 20);
            this.textBoxUserStringLen.TabIndex = 26;
            // 
            // textBoxNumUserStrings
            // 
            this.textBoxNumUserStrings.Location = new System.Drawing.Point(176, 174);
            this.textBoxNumUserStrings.Name = "textBoxNumUserStrings";
            this.textBoxNumUserStrings.ReadOnly = true;
            this.textBoxNumUserStrings.Size = new System.Drawing.Size(116, 20);
            this.textBoxNumUserStrings.TabIndex = 25;
            // 
            // textBoxUserString
            // 
            this.textBoxUserString.Enabled = false;
            this.textBoxUserString.Location = new System.Drawing.Point(226, 220);
            this.textBoxUserString.Name = "textBoxUserString";
            this.textBoxUserString.Size = new System.Drawing.Size(116, 20);
            this.textBoxUserString.TabIndex = 24;
            // 
            // buttonWriteUserString
            // 
            this.buttonWriteUserString.Enabled = false;
            this.buttonWriteUserString.Location = new System.Drawing.Point(139, 219);
            this.buttonWriteUserString.Name = "buttonWriteUserString";
            this.buttonWriteUserString.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteUserString.TabIndex = 23;
            this.buttonWriteUserString.Text = "W";
            this.buttonWriteUserString.UseVisualStyleBackColor = true;
            this.buttonWriteUserString.Click += new System.EventHandler(this.buttonWriteUserString_Click);
            // 
            // buttonReadUserString
            // 
            this.buttonReadUserString.Enabled = false;
            this.buttonReadUserString.Location = new System.Drawing.Point(104, 219);
            this.buttonReadUserString.Name = "buttonReadUserString";
            this.buttonReadUserString.Size = new System.Drawing.Size(29, 21);
            this.buttonReadUserString.TabIndex = 22;
            this.buttonReadUserString.Text = "R";
            this.buttonReadUserString.UseVisualStyleBackColor = true;
            this.buttonReadUserString.Click += new System.EventHandler(this.buttonReadUserString_Click);
            // 
            // buttonReadUserStringLen
            // 
            this.buttonReadUserStringLen.Location = new System.Drawing.Point(104, 196);
            this.buttonReadUserStringLen.Name = "buttonReadUserStringLen";
            this.buttonReadUserStringLen.Size = new System.Drawing.Size(29, 21);
            this.buttonReadUserStringLen.TabIndex = 21;
            this.buttonReadUserStringLen.Text = "R";
            this.buttonReadUserStringLen.UseVisualStyleBackColor = true;
            this.buttonReadUserStringLen.Click += new System.EventHandler(this.buttonReadUserStringLen_Click);
            // 
            // buttonReadNumUserStrings
            // 
            this.buttonReadNumUserStrings.Location = new System.Drawing.Point(104, 173);
            this.buttonReadNumUserStrings.Name = "buttonReadNumUserStrings";
            this.buttonReadNumUserStrings.Size = new System.Drawing.Size(29, 21);
            this.buttonReadNumUserStrings.TabIndex = 20;
            this.buttonReadNumUserStrings.Text = "R";
            this.buttonReadNumUserStrings.UseVisualStyleBackColor = true;
            this.buttonReadNumUserStrings.Click += new System.EventHandler(this.buttonReadNumUserStrings_Click);
            // 
            // textBoxDeviceAlias
            // 
            this.textBoxDeviceAlias.Location = new System.Drawing.Point(176, 151);
            this.textBoxDeviceAlias.Name = "textBoxDeviceAlias";
            this.textBoxDeviceAlias.Size = new System.Drawing.Size(116, 20);
            this.textBoxDeviceAlias.TabIndex = 19;
            // 
            // buttonWriteDeviceAlias
            // 
            this.buttonWriteDeviceAlias.Location = new System.Drawing.Point(139, 150);
            this.buttonWriteDeviceAlias.Name = "buttonWriteDeviceAlias";
            this.buttonWriteDeviceAlias.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteDeviceAlias.TabIndex = 18;
            this.buttonWriteDeviceAlias.Text = "W";
            this.buttonWriteDeviceAlias.UseVisualStyleBackColor = true;
            this.buttonWriteDeviceAlias.Click += new System.EventHandler(this.buttonWriteDeviceAlias_Click);
            // 
            // buttonReadDeviceAlias
            // 
            this.buttonReadDeviceAlias.Location = new System.Drawing.Point(104, 150);
            this.buttonReadDeviceAlias.Name = "buttonReadDeviceAlias";
            this.buttonReadDeviceAlias.Size = new System.Drawing.Size(29, 21);
            this.buttonReadDeviceAlias.TabIndex = 17;
            this.buttonReadDeviceAlias.Text = "R";
            this.buttonReadDeviceAlias.UseVisualStyleBackColor = true;
            this.buttonReadDeviceAlias.Click += new System.EventHandler(this.buttonReadDeviceAlias_Click);
            // 
            // textBoxDeviceAliasLen
            // 
            this.textBoxDeviceAliasLen.Location = new System.Drawing.Point(176, 128);
            this.textBoxDeviceAliasLen.Name = "textBoxDeviceAliasLen";
            this.textBoxDeviceAliasLen.ReadOnly = true;
            this.textBoxDeviceAliasLen.Size = new System.Drawing.Size(116, 20);
            this.textBoxDeviceAliasLen.TabIndex = 16;
            // 
            // buttonReadAliasLen
            // 
            this.buttonReadAliasLen.Location = new System.Drawing.Point(104, 127);
            this.buttonReadAliasLen.Name = "buttonReadAliasLen";
            this.buttonReadAliasLen.Size = new System.Drawing.Size(29, 21);
            this.buttonReadAliasLen.TabIndex = 15;
            this.buttonReadAliasLen.Text = "R";
            this.buttonReadAliasLen.UseVisualStyleBackColor = true;
            this.buttonReadAliasLen.Click += new System.EventHandler(this.buttonReadAliasLen_Click);
            // 
            // textBoxFwRevision
            // 
            this.textBoxFwRevision.Location = new System.Drawing.Point(176, 81);
            this.textBoxFwRevision.Name = "textBoxFwRevision";
            this.textBoxFwRevision.ReadOnly = true;
            this.textBoxFwRevision.Size = new System.Drawing.Size(116, 20);
            this.textBoxFwRevision.TabIndex = 14;
            // 
            // textBoxHwRevision
            // 
            this.textBoxHwRevision.Location = new System.Drawing.Point(176, 58);
            this.textBoxHwRevision.Name = "textBoxHwRevision";
            this.textBoxHwRevision.ReadOnly = true;
            this.textBoxHwRevision.Size = new System.Drawing.Size(116, 20);
            this.textBoxHwRevision.TabIndex = 13;
            // 
            // buttonReadFWRevision
            // 
            this.buttonReadFWRevision.Location = new System.Drawing.Point(104, 80);
            this.buttonReadFWRevision.Name = "buttonReadFWRevision";
            this.buttonReadFWRevision.Size = new System.Drawing.Size(29, 21);
            this.buttonReadFWRevision.TabIndex = 12;
            this.buttonReadFWRevision.Text = "R";
            this.buttonReadFWRevision.UseVisualStyleBackColor = true;
            this.buttonReadFWRevision.Click += new System.EventHandler(this.buttonReadFWRevision_Click);
            // 
            // textBoxSerialNum
            // 
            this.textBoxSerialNum.Location = new System.Drawing.Point(176, 35);
            this.textBoxSerialNum.Name = "textBoxSerialNum";
            this.textBoxSerialNum.ReadOnly = true;
            this.textBoxSerialNum.Size = new System.Drawing.Size(116, 20);
            this.textBoxSerialNum.TabIndex = 11;
            // 
            // buttonReadHWRevision
            // 
            this.buttonReadHWRevision.Location = new System.Drawing.Point(104, 57);
            this.buttonReadHWRevision.Name = "buttonReadHWRevision";
            this.buttonReadHWRevision.Size = new System.Drawing.Size(29, 21);
            this.buttonReadHWRevision.TabIndex = 9;
            this.buttonReadHWRevision.Text = "R";
            this.buttonReadHWRevision.UseVisualStyleBackColor = true;
            this.buttonReadHWRevision.Click += new System.EventHandler(this.buttonReadHWRevision_Click);
            // 
            // buttonReadSerialNum
            // 
            this.buttonReadSerialNum.Location = new System.Drawing.Point(104, 34);
            this.buttonReadSerialNum.Name = "buttonReadSerialNum";
            this.buttonReadSerialNum.Size = new System.Drawing.Size(29, 21);
            this.buttonReadSerialNum.TabIndex = 8;
            this.buttonReadSerialNum.Text = "R";
            this.buttonReadSerialNum.UseVisualStyleBackColor = true;
            this.buttonReadSerialNum.Click += new System.EventHandler(this.buttonReadSerialNum_Click);
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(42, 223);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(59, 13);
            this.label22.TabIndex = 7;
            this.label22.Text = "User String";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(21, 200);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(80, 13);
            this.label21.TabIndex = 6;
            this.label21.Text = "User String Len";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(12, 177);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(89, 13);
            this.label20.TabIndex = 5;
            this.label20.Text = "Num User Strings";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(35, 154);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(66, 13);
            this.label19.TabIndex = 4;
            this.label19.Text = "Device Alias";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(14, 131);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(87, 13);
            this.label18.TabIndex = 3;
            this.label18.Text = "Device Alias Len";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(33, 84);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(68, 13);
            this.label17.TabIndex = 2;
            this.label17.Text = "FW Revision";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(31, 61);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(70, 13);
            this.label16.TabIndex = 1;
            this.label16.Text = "HW Revision";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(43, 38);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(58, 13);
            this.label15.TabIndex = 0;
            this.label15.Text = "Serial Num";
            // 
            // tabPageAcquisitionParams
            // 
            this.tabPageAcquisitionParams.BackColor = System.Drawing.SystemColors.Control;
            this.tabPageAcquisitionParams.Controls.Add(this.checkBoxDefaultBinningFactor);
            this.tabPageAcquisitionParams.Controls.Add(this.label49);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonWritePartialSpectrumMode);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadPartialSpectrumMode);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonSimTriggerPulse);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxPartialSpectrumMode);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxMaxBinningFactor);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadMaxBinningFactor);
            this.tabPageAcquisitionParams.Controls.Add(this.label48);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxDefaultBinningFactor);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonWriteDefaultBinningFactor);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadDefaultBinningFactor);
            this.tabPageAcquisitionParams.Controls.Add(this.label47);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxBinningFactor);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonWriteBinningFactor);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadBinningFactor);
            this.tabPageAcquisitionParams.Controls.Add(this.label46);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxBoxcarWidth);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonWriteBoxcarWidth);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadBoxcarWidth);
            this.tabPageAcquisitionParams.Controls.Add(this.label45);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxScansToAvg);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonWriteScansToAvg);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadScansToAvg);
            this.tabPageAcquisitionParams.Controls.Add(this.label44);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxAcquisitionDelayIncrement);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadAcquisitionDelayIncrement);
            this.tabPageAcquisitionParams.Controls.Add(this.label41);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxMinAcquisitionDelay);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxMaxAcquisitionDelay);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadMinAcquisitionDelay);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadMaxAcquisitionDelay);
            this.tabPageAcquisitionParams.Controls.Add(this.label42);
            this.tabPageAcquisitionParams.Controls.Add(this.label43);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxAcquisitionDelay);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonWriteAcquisitionDelay);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadAcquisitionDelay);
            this.tabPageAcquisitionParams.Controls.Add(this.label40);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxTriggerMode);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonWriteTriggerMode);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadTriggerMode);
            this.tabPageAcquisitionParams.Controls.Add(this.label39);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxLampEnable);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonWriteLampEnable);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadLampEnable);
            this.tabPageAcquisitionParams.Controls.Add(this.label38);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxIntegTimeIncrement);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadIntegTimeIncrement);
            this.tabPageAcquisitionParams.Controls.Add(this.label37);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxMinIntegTime);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxMaxIntegTime);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadMinIntegTime);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadMaxIntegTime);
            this.tabPageAcquisitionParams.Controls.Add(this.textBoxIntegTime);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonWriteIntegTime);
            this.tabPageAcquisitionParams.Controls.Add(this.buttonReadIntegTime);
            this.tabPageAcquisitionParams.Controls.Add(this.label34);
            this.tabPageAcquisitionParams.Controls.Add(this.label35);
            this.tabPageAcquisitionParams.Controls.Add(this.label36);
            this.tabPageAcquisitionParams.Location = new System.Drawing.Point(4, 22);
            this.tabPageAcquisitionParams.Name = "tabPageAcquisitionParams";
            this.tabPageAcquisitionParams.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageAcquisitionParams.Size = new System.Drawing.Size(726, 377);
            this.tabPageAcquisitionParams.TabIndex = 2;
            this.tabPageAcquisitionParams.Text = "Acquisition Params";
            // 
            // checkBoxDefaultBinningFactor
            // 
            this.checkBoxDefaultBinningFactor.AutoSize = true;
            this.checkBoxDefaultBinningFactor.Location = new System.Drawing.Point(310, 191);
            this.checkBoxDefaultBinningFactor.Name = "checkBoxDefaultBinningFactor";
            this.checkBoxDefaultBinningFactor.Size = new System.Drawing.Size(187, 17);
            this.checkBoxDefaultBinningFactor.TabIndex = 86;
            this.checkBoxDefaultBinningFactor.Text = "reset to factory default (with Write)";
            this.checkBoxDefaultBinningFactor.UseVisualStyleBackColor = true;
            // 
            // label49
            // 
            this.label49.AutoSize = true;
            this.label49.Location = new System.Drawing.Point(90, 253);
            this.label49.Name = "label49";
            this.label49.Size = new System.Drawing.Size(353, 13);
            this.label49.TabIndex = 85;
            this.label49.Text = "Partial Spectrum Mode (space separated hex bytes such as: 01 00 04 00)";
            // 
            // buttonWritePartialSpectrumMode
            // 
            this.buttonWritePartialSpectrumMode.Location = new System.Drawing.Point(55, 249);
            this.buttonWritePartialSpectrumMode.Name = "buttonWritePartialSpectrumMode";
            this.buttonWritePartialSpectrumMode.Size = new System.Drawing.Size(29, 21);
            this.buttonWritePartialSpectrumMode.TabIndex = 84;
            this.buttonWritePartialSpectrumMode.Text = "W";
            this.buttonWritePartialSpectrumMode.UseVisualStyleBackColor = true;
            this.buttonWritePartialSpectrumMode.Click += new System.EventHandler(this.buttonWritePartialSpectrumMode_Click);
            // 
            // buttonReadPartialSpectrumMode
            // 
            this.buttonReadPartialSpectrumMode.Location = new System.Drawing.Point(20, 249);
            this.buttonReadPartialSpectrumMode.Name = "buttonReadPartialSpectrumMode";
            this.buttonReadPartialSpectrumMode.Size = new System.Drawing.Size(29, 21);
            this.buttonReadPartialSpectrumMode.TabIndex = 83;
            this.buttonReadPartialSpectrumMode.Text = "R";
            this.buttonReadPartialSpectrumMode.UseVisualStyleBackColor = true;
            this.buttonReadPartialSpectrumMode.Click += new System.EventHandler(this.buttonReadPartialSpectrumMode_Click);
            // 
            // buttonSimTriggerPulse
            // 
            this.buttonSimTriggerPulse.Location = new System.Drawing.Point(20, 324);
            this.buttonSimTriggerPulse.Name = "buttonSimTriggerPulse";
            this.buttonSimTriggerPulse.Size = new System.Drawing.Size(131, 23);
            this.buttonSimTriggerPulse.TabIndex = 81;
            this.buttonSimTriggerPulse.Text = "Simulate Trigger Pulse";
            this.buttonSimTriggerPulse.UseVisualStyleBackColor = true;
            this.buttonSimTriggerPulse.Click += new System.EventHandler(this.buttonSimTriggerPulse_Click);
            // 
            // textBoxPartialSpectrumMode
            // 
            this.textBoxPartialSpectrumMode.Location = new System.Drawing.Point(20, 276);
            this.textBoxPartialSpectrumMode.Name = "textBoxPartialSpectrumMode";
            this.textBoxPartialSpectrumMode.Size = new System.Drawing.Size(515, 20);
            this.textBoxPartialSpectrumMode.TabIndex = 82;
            // 
            // textBoxMaxBinningFactor
            // 
            this.textBoxMaxBinningFactor.Location = new System.Drawing.Point(187, 212);
            this.textBoxMaxBinningFactor.Name = "textBoxMaxBinningFactor";
            this.textBoxMaxBinningFactor.Size = new System.Drawing.Size(116, 20);
            this.textBoxMaxBinningFactor.TabIndex = 80;
            // 
            // buttonReadMaxBinningFactor
            // 
            this.buttonReadMaxBinningFactor.Location = new System.Drawing.Point(115, 211);
            this.buttonReadMaxBinningFactor.Name = "buttonReadMaxBinningFactor";
            this.buttonReadMaxBinningFactor.Size = new System.Drawing.Size(29, 21);
            this.buttonReadMaxBinningFactor.TabIndex = 78;
            this.buttonReadMaxBinningFactor.Text = "R";
            this.buttonReadMaxBinningFactor.UseVisualStyleBackColor = true;
            this.buttonReadMaxBinningFactor.Click += new System.EventHandler(this.buttonReadMaxBinningFactor_Click);
            // 
            // label48
            // 
            this.label48.AutoSize = true;
            this.label48.Location = new System.Drawing.Point(17, 215);
            this.label48.Name = "label48";
            this.label48.Size = new System.Drawing.Size(98, 13);
            this.label48.TabIndex = 77;
            this.label48.Text = "Max Binning Factor";
            // 
            // textBoxDefaultBinningFactor
            // 
            this.textBoxDefaultBinningFactor.Location = new System.Drawing.Point(187, 189);
            this.textBoxDefaultBinningFactor.Name = "textBoxDefaultBinningFactor";
            this.textBoxDefaultBinningFactor.Size = new System.Drawing.Size(116, 20);
            this.textBoxDefaultBinningFactor.TabIndex = 76;
            // 
            // buttonWriteDefaultBinningFactor
            // 
            this.buttonWriteDefaultBinningFactor.Location = new System.Drawing.Point(150, 188);
            this.buttonWriteDefaultBinningFactor.Name = "buttonWriteDefaultBinningFactor";
            this.buttonWriteDefaultBinningFactor.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteDefaultBinningFactor.TabIndex = 75;
            this.buttonWriteDefaultBinningFactor.Text = "W";
            this.buttonWriteDefaultBinningFactor.UseVisualStyleBackColor = true;
            this.buttonWriteDefaultBinningFactor.Click += new System.EventHandler(this.buttonWriteDefaultBinningFactor_Click);
            // 
            // buttonReadDefaultBinningFactor
            // 
            this.buttonReadDefaultBinningFactor.Location = new System.Drawing.Point(115, 188);
            this.buttonReadDefaultBinningFactor.Name = "buttonReadDefaultBinningFactor";
            this.buttonReadDefaultBinningFactor.Size = new System.Drawing.Size(29, 21);
            this.buttonReadDefaultBinningFactor.TabIndex = 74;
            this.buttonReadDefaultBinningFactor.Text = "R";
            this.buttonReadDefaultBinningFactor.UseVisualStyleBackColor = true;
            this.buttonReadDefaultBinningFactor.Click += new System.EventHandler(this.buttonReadDefaultBinningFactor_Click);
            // 
            // label47
            // 
            this.label47.AutoSize = true;
            this.label47.Location = new System.Drawing.Point(3, 192);
            this.label47.Name = "label47";
            this.label47.Size = new System.Drawing.Size(112, 13);
            this.label47.TabIndex = 73;
            this.label47.Text = "Default Binning Factor";
            // 
            // textBoxBinningFactor
            // 
            this.textBoxBinningFactor.Location = new System.Drawing.Point(187, 166);
            this.textBoxBinningFactor.Name = "textBoxBinningFactor";
            this.textBoxBinningFactor.Size = new System.Drawing.Size(116, 20);
            this.textBoxBinningFactor.TabIndex = 72;
            // 
            // buttonWriteBinningFactor
            // 
            this.buttonWriteBinningFactor.Location = new System.Drawing.Point(150, 165);
            this.buttonWriteBinningFactor.Name = "buttonWriteBinningFactor";
            this.buttonWriteBinningFactor.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteBinningFactor.TabIndex = 71;
            this.buttonWriteBinningFactor.Text = "W";
            this.buttonWriteBinningFactor.UseVisualStyleBackColor = true;
            this.buttonWriteBinningFactor.Click += new System.EventHandler(this.buttonWriteBinningFactor_Click);
            // 
            // buttonReadBinningFactor
            // 
            this.buttonReadBinningFactor.Location = new System.Drawing.Point(115, 165);
            this.buttonReadBinningFactor.Name = "buttonReadBinningFactor";
            this.buttonReadBinningFactor.Size = new System.Drawing.Size(29, 21);
            this.buttonReadBinningFactor.TabIndex = 70;
            this.buttonReadBinningFactor.Text = "R";
            this.buttonReadBinningFactor.UseVisualStyleBackColor = true;
            this.buttonReadBinningFactor.Click += new System.EventHandler(this.buttonReadBinningFactor_Click);
            // 
            // label46
            // 
            this.label46.AutoSize = true;
            this.label46.Location = new System.Drawing.Point(15, 169);
            this.label46.Name = "label46";
            this.label46.Size = new System.Drawing.Size(100, 13);
            this.label46.TabIndex = 69;
            this.label46.Text = "Pixel Binning Factor";
            // 
            // textBoxBoxcarWidth
            // 
            this.textBoxBoxcarWidth.Location = new System.Drawing.Point(187, 143);
            this.textBoxBoxcarWidth.Name = "textBoxBoxcarWidth";
            this.textBoxBoxcarWidth.Size = new System.Drawing.Size(116, 20);
            this.textBoxBoxcarWidth.TabIndex = 68;
            // 
            // buttonWriteBoxcarWidth
            // 
            this.buttonWriteBoxcarWidth.Location = new System.Drawing.Point(150, 142);
            this.buttonWriteBoxcarWidth.Name = "buttonWriteBoxcarWidth";
            this.buttonWriteBoxcarWidth.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteBoxcarWidth.TabIndex = 67;
            this.buttonWriteBoxcarWidth.Text = "W";
            this.buttonWriteBoxcarWidth.UseVisualStyleBackColor = true;
            this.buttonWriteBoxcarWidth.Click += new System.EventHandler(this.buttonWriteBoxcarWidth_Click);
            // 
            // buttonReadBoxcarWidth
            // 
            this.buttonReadBoxcarWidth.Location = new System.Drawing.Point(115, 142);
            this.buttonReadBoxcarWidth.Name = "buttonReadBoxcarWidth";
            this.buttonReadBoxcarWidth.Size = new System.Drawing.Size(29, 21);
            this.buttonReadBoxcarWidth.TabIndex = 66;
            this.buttonReadBoxcarWidth.Text = "R";
            this.buttonReadBoxcarWidth.UseVisualStyleBackColor = true;
            this.buttonReadBoxcarWidth.Click += new System.EventHandler(this.buttonReadBoxcarWidth_Click);
            // 
            // label45
            // 
            this.label45.AutoSize = true;
            this.label45.Location = new System.Drawing.Point(44, 146);
            this.label45.Name = "label45";
            this.label45.Size = new System.Drawing.Size(71, 13);
            this.label45.TabIndex = 65;
            this.label45.Text = "Boxcar Width";
            // 
            // textBoxScansToAvg
            // 
            this.textBoxScansToAvg.Location = new System.Drawing.Point(187, 120);
            this.textBoxScansToAvg.Name = "textBoxScansToAvg";
            this.textBoxScansToAvg.Size = new System.Drawing.Size(116, 20);
            this.textBoxScansToAvg.TabIndex = 64;
            // 
            // buttonWriteScansToAvg
            // 
            this.buttonWriteScansToAvg.Location = new System.Drawing.Point(150, 119);
            this.buttonWriteScansToAvg.Name = "buttonWriteScansToAvg";
            this.buttonWriteScansToAvg.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteScansToAvg.TabIndex = 63;
            this.buttonWriteScansToAvg.Text = "W";
            this.buttonWriteScansToAvg.UseVisualStyleBackColor = true;
            this.buttonWriteScansToAvg.Click += new System.EventHandler(this.buttonWriteScansToAvg_Click);
            // 
            // buttonReadScansToAvg
            // 
            this.buttonReadScansToAvg.Location = new System.Drawing.Point(115, 119);
            this.buttonReadScansToAvg.Name = "buttonReadScansToAvg";
            this.buttonReadScansToAvg.Size = new System.Drawing.Size(29, 21);
            this.buttonReadScansToAvg.TabIndex = 62;
            this.buttonReadScansToAvg.Text = "R";
            this.buttonReadScansToAvg.UseVisualStyleBackColor = true;
            this.buttonReadScansToAvg.Click += new System.EventHandler(this.buttonReadScansToAvg_Click);
            // 
            // label44
            // 
            this.label44.AutoSize = true;
            this.label44.Location = new System.Drawing.Point(44, 123);
            this.label44.Name = "label44";
            this.label44.Size = new System.Drawing.Size(71, 13);
            this.label44.TabIndex = 61;
            this.label44.Text = "Scans to Avg";
            // 
            // textBoxAcquisitionDelayIncrement
            // 
            this.textBoxAcquisitionDelayIncrement.Location = new System.Drawing.Point(540, 143);
            this.textBoxAcquisitionDelayIncrement.Name = "textBoxAcquisitionDelayIncrement";
            this.textBoxAcquisitionDelayIncrement.ReadOnly = true;
            this.textBoxAcquisitionDelayIncrement.Size = new System.Drawing.Size(116, 20);
            this.textBoxAcquisitionDelayIncrement.TabIndex = 60;
            // 
            // buttonReadAcquisitionDelayIncrement
            // 
            this.buttonReadAcquisitionDelayIncrement.Location = new System.Drawing.Point(468, 142);
            this.buttonReadAcquisitionDelayIncrement.Name = "buttonReadAcquisitionDelayIncrement";
            this.buttonReadAcquisitionDelayIncrement.Size = new System.Drawing.Size(29, 21);
            this.buttonReadAcquisitionDelayIncrement.TabIndex = 59;
            this.buttonReadAcquisitionDelayIncrement.Text = "R";
            this.buttonReadAcquisitionDelayIncrement.UseVisualStyleBackColor = true;
            this.buttonReadAcquisitionDelayIncrement.Click += new System.EventHandler(this.buttonReadAcquisitionDelayIncrement_Click);
            // 
            // label41
            // 
            this.label41.AutoSize = true;
            this.label41.Location = new System.Drawing.Point(362, 146);
            this.label41.Name = "label41";
            this.label41.Size = new System.Drawing.Size(106, 13);
            this.label41.TabIndex = 58;
            this.label41.Text = "Acq Delay Increment";
            // 
            // textBoxMinAcquisitionDelay
            // 
            this.textBoxMinAcquisitionDelay.Location = new System.Drawing.Point(540, 120);
            this.textBoxMinAcquisitionDelay.Name = "textBoxMinAcquisitionDelay";
            this.textBoxMinAcquisitionDelay.ReadOnly = true;
            this.textBoxMinAcquisitionDelay.Size = new System.Drawing.Size(116, 20);
            this.textBoxMinAcquisitionDelay.TabIndex = 57;
            // 
            // textBoxMaxAcquisitionDelay
            // 
            this.textBoxMaxAcquisitionDelay.Location = new System.Drawing.Point(540, 97);
            this.textBoxMaxAcquisitionDelay.Name = "textBoxMaxAcquisitionDelay";
            this.textBoxMaxAcquisitionDelay.ReadOnly = true;
            this.textBoxMaxAcquisitionDelay.Size = new System.Drawing.Size(116, 20);
            this.textBoxMaxAcquisitionDelay.TabIndex = 56;
            // 
            // buttonReadMinAcquisitionDelay
            // 
            this.buttonReadMinAcquisitionDelay.Location = new System.Drawing.Point(468, 119);
            this.buttonReadMinAcquisitionDelay.Name = "buttonReadMinAcquisitionDelay";
            this.buttonReadMinAcquisitionDelay.Size = new System.Drawing.Size(29, 21);
            this.buttonReadMinAcquisitionDelay.TabIndex = 55;
            this.buttonReadMinAcquisitionDelay.Text = "R";
            this.buttonReadMinAcquisitionDelay.UseVisualStyleBackColor = true;
            this.buttonReadMinAcquisitionDelay.Click += new System.EventHandler(this.buttonReadMinAcquisitionDelay_Click);
            // 
            // buttonReadMaxAcquisitionDelay
            // 
            this.buttonReadMaxAcquisitionDelay.Location = new System.Drawing.Point(468, 96);
            this.buttonReadMaxAcquisitionDelay.Name = "buttonReadMaxAcquisitionDelay";
            this.buttonReadMaxAcquisitionDelay.Size = new System.Drawing.Size(29, 21);
            this.buttonReadMaxAcquisitionDelay.TabIndex = 54;
            this.buttonReadMaxAcquisitionDelay.Text = "R";
            this.buttonReadMaxAcquisitionDelay.UseVisualStyleBackColor = true;
            this.buttonReadMaxAcquisitionDelay.Click += new System.EventHandler(this.buttonReadMaxAcquisitionDelay_Click);
            // 
            // label42
            // 
            this.label42.AutoSize = true;
            this.label42.Location = new System.Drawing.Point(392, 123);
            this.label42.Name = "label42";
            this.label42.Size = new System.Drawing.Size(76, 13);
            this.label42.TabIndex = 53;
            this.label42.Text = "Min Acq Delay";
            // 
            // label43
            // 
            this.label43.AutoSize = true;
            this.label43.Location = new System.Drawing.Point(389, 100);
            this.label43.Name = "label43";
            this.label43.Size = new System.Drawing.Size(79, 13);
            this.label43.TabIndex = 52;
            this.label43.Text = "Max Acq Delay";
            // 
            // textBoxAcquisitionDelay
            // 
            this.textBoxAcquisitionDelay.Location = new System.Drawing.Point(540, 70);
            this.textBoxAcquisitionDelay.Name = "textBoxAcquisitionDelay";
            this.textBoxAcquisitionDelay.Size = new System.Drawing.Size(116, 20);
            this.textBoxAcquisitionDelay.TabIndex = 51;
            // 
            // buttonWriteAcquisitionDelay
            // 
            this.buttonWriteAcquisitionDelay.Location = new System.Drawing.Point(503, 69);
            this.buttonWriteAcquisitionDelay.Name = "buttonWriteAcquisitionDelay";
            this.buttonWriteAcquisitionDelay.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteAcquisitionDelay.TabIndex = 50;
            this.buttonWriteAcquisitionDelay.Text = "W";
            this.buttonWriteAcquisitionDelay.UseVisualStyleBackColor = true;
            this.buttonWriteAcquisitionDelay.Click += new System.EventHandler(this.buttonWriteAcquisitionDelay_Click);
            // 
            // buttonReadAcquisitionDelay
            // 
            this.buttonReadAcquisitionDelay.Location = new System.Drawing.Point(468, 69);
            this.buttonReadAcquisitionDelay.Name = "buttonReadAcquisitionDelay";
            this.buttonReadAcquisitionDelay.Size = new System.Drawing.Size(29, 21);
            this.buttonReadAcquisitionDelay.TabIndex = 49;
            this.buttonReadAcquisitionDelay.Text = "R";
            this.buttonReadAcquisitionDelay.UseVisualStyleBackColor = true;
            this.buttonReadAcquisitionDelay.Click += new System.EventHandler(this.buttonReadAcquisitionDelay_Click);
            // 
            // label40
            // 
            this.label40.AutoSize = true;
            this.label40.Location = new System.Drawing.Point(380, 73);
            this.label40.Name = "label40";
            this.label40.Size = new System.Drawing.Size(88, 13);
            this.label40.TabIndex = 48;
            this.label40.Text = "Acquisition Delay";
            // 
            // textBoxTriggerMode
            // 
            this.textBoxTriggerMode.Location = new System.Drawing.Point(540, 43);
            this.textBoxTriggerMode.Name = "textBoxTriggerMode";
            this.textBoxTriggerMode.Size = new System.Drawing.Size(116, 20);
            this.textBoxTriggerMode.TabIndex = 47;
            // 
            // buttonWriteTriggerMode
            // 
            this.buttonWriteTriggerMode.Location = new System.Drawing.Point(503, 42);
            this.buttonWriteTriggerMode.Name = "buttonWriteTriggerMode";
            this.buttonWriteTriggerMode.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteTriggerMode.TabIndex = 46;
            this.buttonWriteTriggerMode.Text = "W";
            this.buttonWriteTriggerMode.UseVisualStyleBackColor = true;
            this.buttonWriteTriggerMode.Click += new System.EventHandler(this.buttonWriteTriggerMode_Click);
            // 
            // buttonReadTriggerMode
            // 
            this.buttonReadTriggerMode.Location = new System.Drawing.Point(468, 42);
            this.buttonReadTriggerMode.Name = "buttonReadTriggerMode";
            this.buttonReadTriggerMode.Size = new System.Drawing.Size(29, 21);
            this.buttonReadTriggerMode.TabIndex = 45;
            this.buttonReadTriggerMode.Text = "R";
            this.buttonReadTriggerMode.UseVisualStyleBackColor = true;
            this.buttonReadTriggerMode.Click += new System.EventHandler(this.buttonReadTriggerMode_Click);
            // 
            // label39
            // 
            this.label39.AutoSize = true;
            this.label39.Location = new System.Drawing.Point(398, 46);
            this.label39.Name = "label39";
            this.label39.Size = new System.Drawing.Size(70, 13);
            this.label39.TabIndex = 44;
            this.label39.Text = "Trigger Mode";
            // 
            // textBoxLampEnable
            // 
            this.textBoxLampEnable.Location = new System.Drawing.Point(540, 16);
            this.textBoxLampEnable.Name = "textBoxLampEnable";
            this.textBoxLampEnable.Size = new System.Drawing.Size(116, 20);
            this.textBoxLampEnable.TabIndex = 43;
            // 
            // buttonWriteLampEnable
            // 
            this.buttonWriteLampEnable.Location = new System.Drawing.Point(503, 15);
            this.buttonWriteLampEnable.Name = "buttonWriteLampEnable";
            this.buttonWriteLampEnable.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteLampEnable.TabIndex = 42;
            this.buttonWriteLampEnable.Text = "W";
            this.buttonWriteLampEnable.UseVisualStyleBackColor = true;
            this.buttonWriteLampEnable.Click += new System.EventHandler(this.buttonWriteLampEnable_Click);
            // 
            // buttonReadLampEnable
            // 
            this.buttonReadLampEnable.Location = new System.Drawing.Point(468, 15);
            this.buttonReadLampEnable.Name = "buttonReadLampEnable";
            this.buttonReadLampEnable.Size = new System.Drawing.Size(29, 21);
            this.buttonReadLampEnable.TabIndex = 41;
            this.buttonReadLampEnable.Text = "R";
            this.buttonReadLampEnable.UseVisualStyleBackColor = true;
            this.buttonReadLampEnable.Click += new System.EventHandler(this.buttonReadLampEnable_Click);
            // 
            // label38
            // 
            this.label38.AutoSize = true;
            this.label38.Location = new System.Drawing.Point(399, 19);
            this.label38.Name = "label38";
            this.label38.Size = new System.Drawing.Size(69, 13);
            this.label38.TabIndex = 40;
            this.label38.Text = "Lamp Enable";
            // 
            // textBoxIntegTimeIncrement
            // 
            this.textBoxIntegTimeIncrement.Location = new System.Drawing.Point(187, 85);
            this.textBoxIntegTimeIncrement.Name = "textBoxIntegTimeIncrement";
            this.textBoxIntegTimeIncrement.ReadOnly = true;
            this.textBoxIntegTimeIncrement.Size = new System.Drawing.Size(116, 20);
            this.textBoxIntegTimeIncrement.TabIndex = 39;
            // 
            // buttonReadIntegTimeIncrement
            // 
            this.buttonReadIntegTimeIncrement.Location = new System.Drawing.Point(115, 84);
            this.buttonReadIntegTimeIncrement.Name = "buttonReadIntegTimeIncrement";
            this.buttonReadIntegTimeIncrement.Size = new System.Drawing.Size(29, 21);
            this.buttonReadIntegTimeIncrement.TabIndex = 38;
            this.buttonReadIntegTimeIncrement.Text = "R";
            this.buttonReadIntegTimeIncrement.UseVisualStyleBackColor = true;
            this.buttonReadIntegTimeIncrement.Click += new System.EventHandler(this.buttonReadIntegTimeIncrement_Click);
            // 
            // label37
            // 
            this.label37.AutoSize = true;
            this.label37.Location = new System.Drawing.Point(8, 88);
            this.label37.Name = "label37";
            this.label37.Size = new System.Drawing.Size(107, 13);
            this.label37.TabIndex = 37;
            this.label37.Text = "Integ Time Increment";
            // 
            // textBoxMinIntegTime
            // 
            this.textBoxMinIntegTime.Location = new System.Drawing.Point(187, 62);
            this.textBoxMinIntegTime.Name = "textBoxMinIntegTime";
            this.textBoxMinIntegTime.ReadOnly = true;
            this.textBoxMinIntegTime.Size = new System.Drawing.Size(116, 20);
            this.textBoxMinIntegTime.TabIndex = 36;
            // 
            // textBoxMaxIntegTime
            // 
            this.textBoxMaxIntegTime.Location = new System.Drawing.Point(187, 39);
            this.textBoxMaxIntegTime.Name = "textBoxMaxIntegTime";
            this.textBoxMaxIntegTime.ReadOnly = true;
            this.textBoxMaxIntegTime.Size = new System.Drawing.Size(116, 20);
            this.textBoxMaxIntegTime.TabIndex = 35;
            // 
            // buttonReadMinIntegTime
            // 
            this.buttonReadMinIntegTime.Location = new System.Drawing.Point(115, 61);
            this.buttonReadMinIntegTime.Name = "buttonReadMinIntegTime";
            this.buttonReadMinIntegTime.Size = new System.Drawing.Size(29, 21);
            this.buttonReadMinIntegTime.TabIndex = 34;
            this.buttonReadMinIntegTime.Text = "R";
            this.buttonReadMinIntegTime.UseVisualStyleBackColor = true;
            this.buttonReadMinIntegTime.Click += new System.EventHandler(this.buttonReadMinIntegTime_Click);
            // 
            // buttonReadMaxIntegTime
            // 
            this.buttonReadMaxIntegTime.Location = new System.Drawing.Point(115, 38);
            this.buttonReadMaxIntegTime.Name = "buttonReadMaxIntegTime";
            this.buttonReadMaxIntegTime.Size = new System.Drawing.Size(29, 21);
            this.buttonReadMaxIntegTime.TabIndex = 33;
            this.buttonReadMaxIntegTime.Text = "R";
            this.buttonReadMaxIntegTime.UseVisualStyleBackColor = true;
            this.buttonReadMaxIntegTime.Click += new System.EventHandler(this.buttonReadMaxIntegTime_Click);
            // 
            // textBoxIntegTime
            // 
            this.textBoxIntegTime.Location = new System.Drawing.Point(187, 16);
            this.textBoxIntegTime.Name = "textBoxIntegTime";
            this.textBoxIntegTime.Size = new System.Drawing.Size(116, 20);
            this.textBoxIntegTime.TabIndex = 32;
            // 
            // buttonWriteIntegTime
            // 
            this.buttonWriteIntegTime.Location = new System.Drawing.Point(150, 15);
            this.buttonWriteIntegTime.Name = "buttonWriteIntegTime";
            this.buttonWriteIntegTime.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteIntegTime.TabIndex = 31;
            this.buttonWriteIntegTime.Text = "W";
            this.buttonWriteIntegTime.UseVisualStyleBackColor = true;
            this.buttonWriteIntegTime.Click += new System.EventHandler(this.buttonWriteIntegTime_Click);
            // 
            // buttonReadIntegTime
            // 
            this.buttonReadIntegTime.Location = new System.Drawing.Point(115, 15);
            this.buttonReadIntegTime.Name = "buttonReadIntegTime";
            this.buttonReadIntegTime.Size = new System.Drawing.Size(29, 21);
            this.buttonReadIntegTime.TabIndex = 30;
            this.buttonReadIntegTime.Text = "R";
            this.buttonReadIntegTime.UseVisualStyleBackColor = true;
            this.buttonReadIntegTime.Click += new System.EventHandler(this.buttonReadIntegTime_Click);
            // 
            // label34
            // 
            this.label34.AutoSize = true;
            this.label34.Location = new System.Drawing.Point(38, 65);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(77, 13);
            this.label34.TabIndex = 29;
            this.label34.Text = "Min Integ Time";
            // 
            // label35
            // 
            this.label35.AutoSize = true;
            this.label35.Location = new System.Drawing.Point(35, 42);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(80, 13);
            this.label35.TabIndex = 28;
            this.label35.Text = "Max Integ Time";
            // 
            // label36
            // 
            this.label36.AutoSize = true;
            this.label36.Location = new System.Drawing.Point(32, 19);
            this.label36.Name = "label36";
            this.label36.Size = new System.Drawing.Size(83, 13);
            this.label36.TabIndex = 27;
            this.label36.Text = "Integration Time";
            // 
            // tabPageSpectrum
            // 
            this.tabPageSpectrum.BackColor = System.Drawing.SystemColors.Control;
            this.tabPageSpectrum.Controls.Add(this.tabControlSpectrumInfo);
            this.tabPageSpectrum.Controls.Add(this.buttonReadBufferedSpectrum);
            this.tabPageSpectrum.Controls.Add(this.buttonReadPartialCorrectedSpectrum);
            this.tabPageSpectrum.Controls.Add(this.buttonReadRawSpectrum);
            this.tabPageSpectrum.Controls.Add(this.buttonReadCorrectedSpectrum);
            this.tabPageSpectrum.Location = new System.Drawing.Point(4, 22);
            this.tabPageSpectrum.Name = "tabPageSpectrum";
            this.tabPageSpectrum.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageSpectrum.Size = new System.Drawing.Size(726, 377);
            this.tabPageSpectrum.TabIndex = 3;
            this.tabPageSpectrum.Text = "Spectrum";
            // 
            // tabControlSpectrumInfo
            // 
            this.tabControlSpectrumInfo.Controls.Add(this.tabPageChart);
            this.tabControlSpectrumInfo.Controls.Add(this.tabPageData);
            this.tabControlSpectrumInfo.Controls.Add(this.tabPageBuffer);
            this.tabControlSpectrumInfo.Location = new System.Drawing.Point(15, 53);
            this.tabControlSpectrumInfo.Name = "tabControlSpectrumInfo";
            this.tabControlSpectrumInfo.SelectedIndex = 0;
            this.tabControlSpectrumInfo.Size = new System.Drawing.Size(689, 328);
            this.tabControlSpectrumInfo.TabIndex = 83;
            // 
            // tabPageChart
            // 
            this.tabPageChart.Controls.Add(this.chartSpectrum);
            this.tabPageChart.Location = new System.Drawing.Point(4, 22);
            this.tabPageChart.Name = "tabPageChart";
            this.tabPageChart.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageChart.Size = new System.Drawing.Size(681, 302);
            this.tabPageChart.TabIndex = 0;
            this.tabPageChart.Text = "Chart";
            this.tabPageChart.UseVisualStyleBackColor = true;
            // 
            // chartSpectrum
            // 
            chartArea1.AxisX.MinorGrid.Enabled = true;
            chartArea1.AxisX.MinorGrid.LineColor = System.Drawing.Color.LightGray;
            chartArea1.AxisX.Title = "Pixel";
            chartArea1.AxisY.MinorGrid.Enabled = true;
            chartArea1.AxisY.MinorGrid.LineColor = System.Drawing.Color.LightGray;
            chartArea1.AxisY.Title = "Counts";
            chartArea1.Name = "ChartArea1";
            this.chartSpectrum.ChartAreas.Add(chartArea1);
            this.chartSpectrum.Dock = System.Windows.Forms.DockStyle.Fill;
            legend1.Enabled = false;
            legend1.Name = "Legend1";
            this.chartSpectrum.Legends.Add(legend1);
            this.chartSpectrum.Location = new System.Drawing.Point(3, 3);
            this.chartSpectrum.Name = "chartSpectrum";
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series1.Legend = "Legend1";
            series1.Name = "Spectrum";
            this.chartSpectrum.Series.Add(series1);
            this.chartSpectrum.Size = new System.Drawing.Size(675, 296);
            this.chartSpectrum.TabIndex = 0;
            this.chartSpectrum.Text = "Spectrum";
            // 
            // tabPageData
            // 
            this.tabPageData.BackColor = System.Drawing.SystemColors.Control;
            this.tabPageData.Controls.Add(this.textBoxMaxValuePixelNum);
            this.tabPageData.Controls.Add(this.labelMaxValuePixelNum);
            this.tabPageData.Controls.Add(this.textBoxMaxSpectrumValue);
            this.tabPageData.Controls.Add(this.label61);
            this.tabPageData.Controls.Add(this.textBoxNumPixelsReturned);
            this.tabPageData.Controls.Add(this.groupBoxMetadata);
            this.tabPageData.Controls.Add(this.label56);
            this.tabPageData.Controls.Add(this.textBoxSpectrumData);
            this.tabPageData.Location = new System.Drawing.Point(4, 22);
            this.tabPageData.Name = "tabPageData";
            this.tabPageData.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageData.Size = new System.Drawing.Size(681, 302);
            this.tabPageData.TabIndex = 2;
            this.tabPageData.Text = "Data";
            // 
            // textBoxMaxValuePixelNum
            // 
            this.textBoxMaxValuePixelNum.Location = new System.Drawing.Point(547, 43);
            this.textBoxMaxValuePixelNum.Name = "textBoxMaxValuePixelNum";
            this.textBoxMaxValuePixelNum.ReadOnly = true;
            this.textBoxMaxValuePixelNum.Size = new System.Drawing.Size(100, 20);
            this.textBoxMaxValuePixelNum.TabIndex = 12;
            // 
            // labelMaxValuePixelNum
            // 
            this.labelMaxValuePixelNum.AutoSize = true;
            this.labelMaxValuePixelNum.Location = new System.Drawing.Point(499, 46);
            this.labelMaxValuePixelNum.Name = "labelMaxValuePixelNum";
            this.labelMaxValuePixelNum.Size = new System.Drawing.Size(42, 13);
            this.labelMaxValuePixelNum.TabIndex = 11;
            this.labelMaxValuePixelNum.Text = "@ pixel";
            // 
            // textBoxMaxSpectrumValue
            // 
            this.textBoxMaxSpectrumValue.Location = new System.Drawing.Point(393, 43);
            this.textBoxMaxSpectrumValue.Name = "textBoxMaxSpectrumValue";
            this.textBoxMaxSpectrumValue.ReadOnly = true;
            this.textBoxMaxSpectrumValue.Size = new System.Drawing.Size(100, 20);
            this.textBoxMaxSpectrumValue.TabIndex = 10;
            // 
            // label61
            // 
            this.label61.AutoSize = true;
            this.label61.Location = new System.Drawing.Point(330, 46);
            this.label61.Name = "label61";
            this.label61.Size = new System.Drawing.Size(63, 13);
            this.label61.TabIndex = 9;
            this.label61.Text = "Max Value: ";
            // 
            // textBoxNumPixelsReturned
            // 
            this.textBoxNumPixelsReturned.Location = new System.Drawing.Point(393, 14);
            this.textBoxNumPixelsReturned.Name = "textBoxNumPixelsReturned";
            this.textBoxNumPixelsReturned.ReadOnly = true;
            this.textBoxNumPixelsReturned.Size = new System.Drawing.Size(100, 20);
            this.textBoxNumPixelsReturned.TabIndex = 8;
            // 
            // groupBoxMetadata
            // 
            this.groupBoxMetadata.Controls.Add(this.textBoxMetadataTriggerMode);
            this.groupBoxMetadata.Controls.Add(this.textBoxMetadataTimeUS);
            this.groupBoxMetadata.Controls.Add(this.textBoxMetadataTickCount);
            this.groupBoxMetadata.Controls.Add(this.textBoxMetadataSpectrumCount);
            this.groupBoxMetadata.Controls.Add(this.label60);
            this.groupBoxMetadata.Controls.Add(this.label59);
            this.groupBoxMetadata.Controls.Add(this.label58);
            this.groupBoxMetadata.Controls.Add(this.label57);
            this.groupBoxMetadata.Location = new System.Drawing.Point(291, 89);
            this.groupBoxMetadata.Name = "groupBoxMetadata";
            this.groupBoxMetadata.Size = new System.Drawing.Size(356, 193);
            this.groupBoxMetadata.TabIndex = 2;
            this.groupBoxMetadata.TabStop = false;
            this.groupBoxMetadata.Text = "Buffered Spectrum Metadata";
            // 
            // textBoxMetadataTriggerMode
            // 
            this.textBoxMetadataTriggerMode.Location = new System.Drawing.Point(102, 113);
            this.textBoxMetadataTriggerMode.Name = "textBoxMetadataTriggerMode";
            this.textBoxMetadataTriggerMode.ReadOnly = true;
            this.textBoxMetadataTriggerMode.Size = new System.Drawing.Size(100, 20);
            this.textBoxMetadataTriggerMode.TabIndex = 7;
            // 
            // textBoxMetadataTimeUS
            // 
            this.textBoxMetadataTimeUS.Location = new System.Drawing.Point(102, 85);
            this.textBoxMetadataTimeUS.Name = "textBoxMetadataTimeUS";
            this.textBoxMetadataTimeUS.ReadOnly = true;
            this.textBoxMetadataTimeUS.Size = new System.Drawing.Size(100, 20);
            this.textBoxMetadataTimeUS.TabIndex = 6;
            // 
            // textBoxMetadataTickCount
            // 
            this.textBoxMetadataTickCount.Location = new System.Drawing.Point(102, 57);
            this.textBoxMetadataTickCount.Name = "textBoxMetadataTickCount";
            this.textBoxMetadataTickCount.ReadOnly = true;
            this.textBoxMetadataTickCount.Size = new System.Drawing.Size(100, 20);
            this.textBoxMetadataTickCount.TabIndex = 5;
            // 
            // textBoxMetadataSpectrumCount
            // 
            this.textBoxMetadataSpectrumCount.Location = new System.Drawing.Point(102, 29);
            this.textBoxMetadataSpectrumCount.Name = "textBoxMetadataSpectrumCount";
            this.textBoxMetadataSpectrumCount.ReadOnly = true;
            this.textBoxMetadataSpectrumCount.Size = new System.Drawing.Size(100, 20);
            this.textBoxMetadataSpectrumCount.TabIndex = 4;
            // 
            // label60
            // 
            this.label60.AutoSize = true;
            this.label60.Location = new System.Drawing.Point(29, 116);
            this.label60.Name = "label60";
            this.label60.Size = new System.Drawing.Size(73, 13);
            this.label60.TabIndex = 3;
            this.label60.Text = "Trigger Mode:";
            // 
            // label59
            // 
            this.label59.AutoSize = true;
            this.label59.Location = new System.Drawing.Point(16, 88);
            this.label59.Name = "label59";
            this.label59.Size = new System.Drawing.Size(86, 13);
            this.label59.TabIndex = 2;
            this.label59.Text = "Integration Time:";
            // 
            // label58
            // 
            this.label58.AutoSize = true;
            this.label58.Location = new System.Drawing.Point(40, 60);
            this.label58.Name = "label58";
            this.label58.Size = new System.Drawing.Size(62, 13);
            this.label58.TabIndex = 1;
            this.label58.Text = "Tick Count:";
            // 
            // label57
            // 
            this.label57.AutoSize = true;
            this.label57.Location = new System.Drawing.Point(16, 32);
            this.label57.Name = "label57";
            this.label57.Size = new System.Drawing.Size(86, 13);
            this.label57.TabIndex = 0;
            this.label57.Text = "Spectrum Count:";
            // 
            // label56
            // 
            this.label56.AutoSize = true;
            this.label56.Location = new System.Drawing.Point(311, 17);
            this.label56.Name = "label56";
            this.label56.Size = new System.Drawing.Size(82, 13);
            this.label56.TabIndex = 1;
            this.label56.Text = "Num Returned: ";
            // 
            // textBoxSpectrumData
            // 
            this.textBoxSpectrumData.Dock = System.Windows.Forms.DockStyle.Left;
            this.textBoxSpectrumData.Location = new System.Drawing.Point(3, 3);
            this.textBoxSpectrumData.Multiline = true;
            this.textBoxSpectrumData.Name = "textBoxSpectrumData";
            this.textBoxSpectrumData.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxSpectrumData.Size = new System.Drawing.Size(266, 296);
            this.textBoxSpectrumData.TabIndex = 0;
            // 
            // tabPageBuffer
            // 
            this.tabPageBuffer.BackColor = System.Drawing.SystemColors.Control;
            this.tabPageBuffer.Controls.Add(this.buttonAbortAcquisition);
            this.tabPageBuffer.Controls.Add(this.textBoxIsIdle);
            this.tabPageBuffer.Controls.Add(this.buttonReadIsIdle);
            this.tabPageBuffer.Controls.Add(this.label66);
            this.tabPageBuffer.Controls.Add(this.buttonAcquireSpectraIntoBuffer);
            this.tabPageBuffer.Controls.Add(this.buttonClearBufferedSpectra);
            this.tabPageBuffer.Controls.Add(this.textBoxRemoveOldestSpectra);
            this.tabPageBuffer.Controls.Add(this.buttonRemoveOldestSpectra);
            this.tabPageBuffer.Controls.Add(this.label65);
            this.tabPageBuffer.Controls.Add(this.textBoxNumSpectraInBuffer);
            this.tabPageBuffer.Controls.Add(this.buttonReadNumSpectraInBuffer);
            this.tabPageBuffer.Controls.Add(this.label64);
            this.tabPageBuffer.Controls.Add(this.textBoxBufferSize);
            this.tabPageBuffer.Controls.Add(this.buttonWriteBufferSize);
            this.tabPageBuffer.Controls.Add(this.buttonReadBufferSize);
            this.tabPageBuffer.Controls.Add(this.label63);
            this.tabPageBuffer.Controls.Add(this.textBoxMaxBufferSize);
            this.tabPageBuffer.Controls.Add(this.buttonReadMaxBufferSize);
            this.tabPageBuffer.Controls.Add(this.label62);
            this.tabPageBuffer.Location = new System.Drawing.Point(4, 22);
            this.tabPageBuffer.Name = "tabPageBuffer";
            this.tabPageBuffer.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageBuffer.Size = new System.Drawing.Size(681, 302);
            this.tabPageBuffer.TabIndex = 1;
            this.tabPageBuffer.Text = "Buffer";
            // 
            // buttonAbortAcquisition
            // 
            this.buttonAbortAcquisition.Location = new System.Drawing.Point(275, 172);
            this.buttonAbortAcquisition.Name = "buttonAbortAcquisition";
            this.buttonAbortAcquisition.Size = new System.Drawing.Size(106, 23);
            this.buttonAbortAcquisition.TabIndex = 41;
            this.buttonAbortAcquisition.Text = "Abort Acquisition";
            this.buttonAbortAcquisition.UseVisualStyleBackColor = true;
            this.buttonAbortAcquisition.Click += new System.EventHandler(this.buttonAbortAcquisition_Click);
            // 
            // textBoxIsIdle
            // 
            this.textBoxIsIdle.Location = new System.Drawing.Point(205, 130);
            this.textBoxIsIdle.Name = "textBoxIsIdle";
            this.textBoxIsIdle.ReadOnly = true;
            this.textBoxIsIdle.Size = new System.Drawing.Size(116, 20);
            this.textBoxIsIdle.TabIndex = 40;
            // 
            // buttonReadIsIdle
            // 
            this.buttonReadIsIdle.Location = new System.Drawing.Point(133, 129);
            this.buttonReadIsIdle.Name = "buttonReadIsIdle";
            this.buttonReadIsIdle.Size = new System.Drawing.Size(29, 21);
            this.buttonReadIsIdle.TabIndex = 39;
            this.buttonReadIsIdle.Text = "R";
            this.buttonReadIsIdle.UseVisualStyleBackColor = true;
            this.buttonReadIsIdle.Click += new System.EventHandler(this.buttonReadIsIdle_Click);
            // 
            // label66
            // 
            this.label66.AutoSize = true;
            this.label66.Location = new System.Drawing.Point(95, 133);
            this.label66.Name = "label66";
            this.label66.Size = new System.Drawing.Size(35, 13);
            this.label66.TabIndex = 38;
            this.label66.Text = "Is Idle";
            // 
            // buttonAcquireSpectraIntoBuffer
            // 
            this.buttonAcquireSpectraIntoBuffer.Location = new System.Drawing.Point(133, 172);
            this.buttonAcquireSpectraIntoBuffer.Name = "buttonAcquireSpectraIntoBuffer";
            this.buttonAcquireSpectraIntoBuffer.Size = new System.Drawing.Size(117, 23);
            this.buttonAcquireSpectraIntoBuffer.TabIndex = 37;
            this.buttonAcquireSpectraIntoBuffer.Text = "Acquire Into Buffer";
            this.buttonAcquireSpectraIntoBuffer.UseVisualStyleBackColor = true;
            this.buttonAcquireSpectraIntoBuffer.Click += new System.EventHandler(this.buttonAcquireSpectraIntoBuffer_Click);
            // 
            // buttonClearBufferedSpectra
            // 
            this.buttonClearBufferedSpectra.Location = new System.Drawing.Point(15, 172);
            this.buttonClearBufferedSpectra.Name = "buttonClearBufferedSpectra";
            this.buttonClearBufferedSpectra.Size = new System.Drawing.Size(94, 23);
            this.buttonClearBufferedSpectra.TabIndex = 36;
            this.buttonClearBufferedSpectra.Text = "Clear Buffer";
            this.buttonClearBufferedSpectra.UseVisualStyleBackColor = true;
            this.buttonClearBufferedSpectra.Click += new System.EventHandler(this.buttonClearBufferedSpectra_Click);
            // 
            // textBoxRemoveOldestSpectra
            // 
            this.textBoxRemoveOldestSpectra.Location = new System.Drawing.Point(205, 101);
            this.textBoxRemoveOldestSpectra.Name = "textBoxRemoveOldestSpectra";
            this.textBoxRemoveOldestSpectra.Size = new System.Drawing.Size(116, 20);
            this.textBoxRemoveOldestSpectra.TabIndex = 35;
            this.textBoxRemoveOldestSpectra.Text = "1";
            // 
            // buttonRemoveOldestSpectra
            // 
            this.buttonRemoveOldestSpectra.Location = new System.Drawing.Point(168, 100);
            this.buttonRemoveOldestSpectra.Name = "buttonRemoveOldestSpectra";
            this.buttonRemoveOldestSpectra.Size = new System.Drawing.Size(29, 21);
            this.buttonRemoveOldestSpectra.TabIndex = 34;
            this.buttonRemoveOldestSpectra.Text = "W";
            this.buttonRemoveOldestSpectra.UseVisualStyleBackColor = true;
            this.buttonRemoveOldestSpectra.Click += new System.EventHandler(this.buttonRemoveOldestSpectra_Click);
            // 
            // label65
            // 
            this.label65.AutoSize = true;
            this.label65.Location = new System.Drawing.Point(12, 104);
            this.label65.Name = "label65";
            this.label65.Size = new System.Drawing.Size(120, 13);
            this.label65.TabIndex = 32;
            this.label65.Text = "Remove Oldest Spectra";
            // 
            // textBoxNumSpectraInBuffer
            // 
            this.textBoxNumSpectraInBuffer.Location = new System.Drawing.Point(205, 74);
            this.textBoxNumSpectraInBuffer.Name = "textBoxNumSpectraInBuffer";
            this.textBoxNumSpectraInBuffer.ReadOnly = true;
            this.textBoxNumSpectraInBuffer.Size = new System.Drawing.Size(116, 20);
            this.textBoxNumSpectraInBuffer.TabIndex = 31;
            // 
            // buttonReadNumSpectraInBuffer
            // 
            this.buttonReadNumSpectraInBuffer.Location = new System.Drawing.Point(133, 73);
            this.buttonReadNumSpectraInBuffer.Name = "buttonReadNumSpectraInBuffer";
            this.buttonReadNumSpectraInBuffer.Size = new System.Drawing.Size(29, 21);
            this.buttonReadNumSpectraInBuffer.TabIndex = 29;
            this.buttonReadNumSpectraInBuffer.Text = "R";
            this.buttonReadNumSpectraInBuffer.UseVisualStyleBackColor = true;
            this.buttonReadNumSpectraInBuffer.Click += new System.EventHandler(this.buttonReadNumSpectraInBuffer_Click);
            // 
            // label64
            // 
            this.label64.AutoSize = true;
            this.label64.Location = new System.Drawing.Point(21, 77);
            this.label64.Name = "label64";
            this.label64.Size = new System.Drawing.Size(111, 13);
            this.label64.TabIndex = 28;
            this.label64.Text = "Num Spectra in Buffer";
            // 
            // textBoxBufferSize
            // 
            this.textBoxBufferSize.Location = new System.Drawing.Point(205, 47);
            this.textBoxBufferSize.Name = "textBoxBufferSize";
            this.textBoxBufferSize.Size = new System.Drawing.Size(116, 20);
            this.textBoxBufferSize.TabIndex = 27;
            // 
            // buttonWriteBufferSize
            // 
            this.buttonWriteBufferSize.Location = new System.Drawing.Point(168, 46);
            this.buttonWriteBufferSize.Name = "buttonWriteBufferSize";
            this.buttonWriteBufferSize.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteBufferSize.TabIndex = 26;
            this.buttonWriteBufferSize.Text = "W";
            this.buttonWriteBufferSize.UseVisualStyleBackColor = true;
            this.buttonWriteBufferSize.Click += new System.EventHandler(this.buttonWriteBufferSize_Click);
            // 
            // buttonReadBufferSize
            // 
            this.buttonReadBufferSize.Location = new System.Drawing.Point(133, 46);
            this.buttonReadBufferSize.Name = "buttonReadBufferSize";
            this.buttonReadBufferSize.Size = new System.Drawing.Size(29, 21);
            this.buttonReadBufferSize.TabIndex = 25;
            this.buttonReadBufferSize.Text = "R";
            this.buttonReadBufferSize.UseVisualStyleBackColor = true;
            this.buttonReadBufferSize.Click += new System.EventHandler(this.buttonReadBufferSize_Click);
            // 
            // label63
            // 
            this.label63.AutoSize = true;
            this.label63.Location = new System.Drawing.Point(72, 50);
            this.label63.Name = "label63";
            this.label63.Size = new System.Drawing.Size(58, 13);
            this.label63.TabIndex = 24;
            this.label63.Text = "Buffer Size";
            // 
            // textBoxMaxBufferSize
            // 
            this.textBoxMaxBufferSize.Location = new System.Drawing.Point(205, 20);
            this.textBoxMaxBufferSize.Name = "textBoxMaxBufferSize";
            this.textBoxMaxBufferSize.ReadOnly = true;
            this.textBoxMaxBufferSize.Size = new System.Drawing.Size(116, 20);
            this.textBoxMaxBufferSize.TabIndex = 23;
            // 
            // buttonReadMaxBufferSize
            // 
            this.buttonReadMaxBufferSize.Location = new System.Drawing.Point(133, 19);
            this.buttonReadMaxBufferSize.Name = "buttonReadMaxBufferSize";
            this.buttonReadMaxBufferSize.Size = new System.Drawing.Size(29, 21);
            this.buttonReadMaxBufferSize.TabIndex = 21;
            this.buttonReadMaxBufferSize.Text = "R";
            this.buttonReadMaxBufferSize.UseVisualStyleBackColor = true;
            this.buttonReadMaxBufferSize.Click += new System.EventHandler(this.buttonReadMaxBufferSize_Click);
            // 
            // label62
            // 
            this.label62.AutoSize = true;
            this.label62.Location = new System.Drawing.Point(49, 23);
            this.label62.Name = "label62";
            this.label62.Size = new System.Drawing.Size(81, 13);
            this.label62.TabIndex = 20;
            this.label62.Text = "Max Buffer Size";
            // 
            // buttonReadBufferedSpectrum
            // 
            this.buttonReadBufferedSpectrum.Location = new System.Drawing.Point(519, 14);
            this.buttonReadBufferedSpectrum.Name = "buttonReadBufferedSpectrum";
            this.buttonReadBufferedSpectrum.Size = new System.Drawing.Size(138, 23);
            this.buttonReadBufferedSpectrum.TabIndex = 82;
            this.buttonReadBufferedSpectrum.Text = "Get Buffered Spectrum";
            this.buttonReadBufferedSpectrum.UseVisualStyleBackColor = true;
            this.buttonReadBufferedSpectrum.Click += new System.EventHandler(this.buttonReadBufferedSpectrum_Click);
            // 
            // buttonReadPartialCorrectedSpectrum
            // 
            this.buttonReadPartialCorrectedSpectrum.Location = new System.Drawing.Point(326, 14);
            this.buttonReadPartialCorrectedSpectrum.Name = "buttonReadPartialCorrectedSpectrum";
            this.buttonReadPartialCorrectedSpectrum.Size = new System.Drawing.Size(167, 23);
            this.buttonReadPartialCorrectedSpectrum.TabIndex = 2;
            this.buttonReadPartialCorrectedSpectrum.Text = "Get Partial Corrected Spectrum";
            this.buttonReadPartialCorrectedSpectrum.UseVisualStyleBackColor = true;
            this.buttonReadPartialCorrectedSpectrum.Click += new System.EventHandler(this.buttonReadPartialCorrectedSpectrum_Click);
            // 
            // buttonReadRawSpectrum
            // 
            this.buttonReadRawSpectrum.Location = new System.Drawing.Point(170, 14);
            this.buttonReadRawSpectrum.Name = "buttonReadRawSpectrum";
            this.buttonReadRawSpectrum.Size = new System.Drawing.Size(136, 23);
            this.buttonReadRawSpectrum.TabIndex = 1;
            this.buttonReadRawSpectrum.Text = "Get Raw Spectrum";
            this.buttonReadRawSpectrum.UseVisualStyleBackColor = true;
            this.buttonReadRawSpectrum.Click += new System.EventHandler(this.buttonReadRawSpectrum_Click);
            // 
            // buttonReadCorrectedSpectrum
            // 
            this.buttonReadCorrectedSpectrum.Location = new System.Drawing.Point(15, 14);
            this.buttonReadCorrectedSpectrum.Name = "buttonReadCorrectedSpectrum";
            this.buttonReadCorrectedSpectrum.Size = new System.Drawing.Size(136, 23);
            this.buttonReadCorrectedSpectrum.TabIndex = 0;
            this.buttonReadCorrectedSpectrum.Text = "Get Corrected Spectrum";
            this.buttonReadCorrectedSpectrum.UseVisualStyleBackColor = true;
            this.buttonReadCorrectedSpectrum.Click += new System.EventHandler(this.buttonReadCorrectedSpectrum_Click);
            // 
            // tabPageCoefficients
            // 
            this.tabPageCoefficients.BackColor = System.Drawing.SystemColors.Control;
            this.tabPageCoefficients.Controls.Add(this.comboBoxStrayLightCoeffIndex);
            this.tabPageCoefficients.Controls.Add(this.textBoxNumStrayLightCoeffs);
            this.tabPageCoefficients.Controls.Add(this.textBoxStrayLightCoeff);
            this.tabPageCoefficients.Controls.Add(this.buttonWriteStrayLightCoeff);
            this.tabPageCoefficients.Controls.Add(this.buttonReadStrayLightCoeff);
            this.tabPageCoefficients.Controls.Add(this.buttonReadNumStrayLightCoeffs);
            this.tabPageCoefficients.Controls.Add(this.label54);
            this.tabPageCoefficients.Controls.Add(this.label55);
            this.tabPageCoefficients.Controls.Add(this.comboBoxWavecalCoeffIndex);
            this.tabPageCoefficients.Controls.Add(this.textBoxNumWavecalCoeffs);
            this.tabPageCoefficients.Controls.Add(this.textBoxWavecalCoeff);
            this.tabPageCoefficients.Controls.Add(this.buttonWriteWavecalCoeff);
            this.tabPageCoefficients.Controls.Add(this.buttonReadWavecalCoeff);
            this.tabPageCoefficients.Controls.Add(this.buttonReadNumWavecalCoeffs);
            this.tabPageCoefficients.Controls.Add(this.label52);
            this.tabPageCoefficients.Controls.Add(this.label53);
            this.tabPageCoefficients.Controls.Add(this.comboBoxNonLinearityCoeffIndex);
            this.tabPageCoefficients.Controls.Add(this.textBoxNumNonLinearityCoeffs);
            this.tabPageCoefficients.Controls.Add(this.textBoxNonLinearityCoeff);
            this.tabPageCoefficients.Controls.Add(this.buttonWriteNonLinearityCoeff);
            this.tabPageCoefficients.Controls.Add(this.buttonReadNonLinearityCoeff);
            this.tabPageCoefficients.Controls.Add(this.buttonReadNumNonLinearityCoeffs);
            this.tabPageCoefficients.Controls.Add(this.label50);
            this.tabPageCoefficients.Controls.Add(this.label51);
            this.tabPageCoefficients.Location = new System.Drawing.Point(4, 22);
            this.tabPageCoefficients.Name = "tabPageCoefficients";
            this.tabPageCoefficients.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageCoefficients.Size = new System.Drawing.Size(726, 377);
            this.tabPageCoefficients.TabIndex = 5;
            this.tabPageCoefficients.Text = "Coefficients";
            // 
            // comboBoxStrayLightCoeffIndex
            // 
            this.comboBoxStrayLightCoeffIndex.Enabled = false;
            this.comboBoxStrayLightCoeffIndex.FormattingEnabled = true;
            this.comboBoxStrayLightCoeffIndex.Location = new System.Drawing.Point(215, 168);
            this.comboBoxStrayLightCoeffIndex.Name = "comboBoxStrayLightCoeffIndex";
            this.comboBoxStrayLightCoeffIndex.Size = new System.Drawing.Size(44, 21);
            this.comboBoxStrayLightCoeffIndex.TabIndex = 51;
            // 
            // textBoxNumStrayLightCoeffs
            // 
            this.textBoxNumStrayLightCoeffs.Location = new System.Drawing.Point(215, 145);
            this.textBoxNumStrayLightCoeffs.Name = "textBoxNumStrayLightCoeffs";
            this.textBoxNumStrayLightCoeffs.ReadOnly = true;
            this.textBoxNumStrayLightCoeffs.Size = new System.Drawing.Size(116, 20);
            this.textBoxNumStrayLightCoeffs.TabIndex = 50;
            // 
            // textBoxStrayLightCoeff
            // 
            this.textBoxStrayLightCoeff.Enabled = false;
            this.textBoxStrayLightCoeff.Location = new System.Drawing.Point(265, 168);
            this.textBoxStrayLightCoeff.Name = "textBoxStrayLightCoeff";
            this.textBoxStrayLightCoeff.Size = new System.Drawing.Size(116, 20);
            this.textBoxStrayLightCoeff.TabIndex = 49;
            // 
            // buttonWriteStrayLightCoeff
            // 
            this.buttonWriteStrayLightCoeff.Enabled = false;
            this.buttonWriteStrayLightCoeff.Location = new System.Drawing.Point(178, 167);
            this.buttonWriteStrayLightCoeff.Name = "buttonWriteStrayLightCoeff";
            this.buttonWriteStrayLightCoeff.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteStrayLightCoeff.TabIndex = 48;
            this.buttonWriteStrayLightCoeff.Text = "W";
            this.buttonWriteStrayLightCoeff.UseVisualStyleBackColor = true;
            this.buttonWriteStrayLightCoeff.Click += new System.EventHandler(this.buttonWriteStrayLightCoeff_Click);
            // 
            // buttonReadStrayLightCoeff
            // 
            this.buttonReadStrayLightCoeff.Enabled = false;
            this.buttonReadStrayLightCoeff.Location = new System.Drawing.Point(143, 167);
            this.buttonReadStrayLightCoeff.Name = "buttonReadStrayLightCoeff";
            this.buttonReadStrayLightCoeff.Size = new System.Drawing.Size(29, 21);
            this.buttonReadStrayLightCoeff.TabIndex = 47;
            this.buttonReadStrayLightCoeff.Text = "R";
            this.buttonReadStrayLightCoeff.UseVisualStyleBackColor = true;
            this.buttonReadStrayLightCoeff.Click += new System.EventHandler(this.buttonReadStrayLightCoeff_Click);
            // 
            // buttonReadNumStrayLightCoeffs
            // 
            this.buttonReadNumStrayLightCoeffs.Location = new System.Drawing.Point(143, 144);
            this.buttonReadNumStrayLightCoeffs.Name = "buttonReadNumStrayLightCoeffs";
            this.buttonReadNumStrayLightCoeffs.Size = new System.Drawing.Size(29, 21);
            this.buttonReadNumStrayLightCoeffs.TabIndex = 46;
            this.buttonReadNumStrayLightCoeffs.Text = "R";
            this.buttonReadNumStrayLightCoeffs.UseVisualStyleBackColor = true;
            this.buttonReadNumStrayLightCoeffs.Click += new System.EventHandler(this.buttonReadNumStrayLightCoeffs_Click);
            // 
            // label54
            // 
            this.label54.AutoSize = true;
            this.label54.Location = new System.Drawing.Point(55, 171);
            this.label54.Name = "label54";
            this.label54.Size = new System.Drawing.Size(85, 13);
            this.label54.TabIndex = 45;
            this.label54.Text = "Stray Light Coeff";
            // 
            // label55
            // 
            this.label55.AutoSize = true;
            this.label55.Location = new System.Drawing.Point(25, 148);
            this.label55.Name = "label55";
            this.label55.Size = new System.Drawing.Size(115, 13);
            this.label55.TabIndex = 44;
            this.label55.Text = "Num Stray Light Coeffs";
            // 
            // comboBoxWavecalCoeffIndex
            // 
            this.comboBoxWavecalCoeffIndex.Enabled = false;
            this.comboBoxWavecalCoeffIndex.FormattingEnabled = true;
            this.comboBoxWavecalCoeffIndex.Location = new System.Drawing.Point(215, 42);
            this.comboBoxWavecalCoeffIndex.Name = "comboBoxWavecalCoeffIndex";
            this.comboBoxWavecalCoeffIndex.Size = new System.Drawing.Size(44, 21);
            this.comboBoxWavecalCoeffIndex.TabIndex = 43;
            // 
            // textBoxNumWavecalCoeffs
            // 
            this.textBoxNumWavecalCoeffs.Location = new System.Drawing.Point(215, 19);
            this.textBoxNumWavecalCoeffs.Name = "textBoxNumWavecalCoeffs";
            this.textBoxNumWavecalCoeffs.ReadOnly = true;
            this.textBoxNumWavecalCoeffs.Size = new System.Drawing.Size(116, 20);
            this.textBoxNumWavecalCoeffs.TabIndex = 42;
            // 
            // textBoxWavecalCoeff
            // 
            this.textBoxWavecalCoeff.Enabled = false;
            this.textBoxWavecalCoeff.Location = new System.Drawing.Point(265, 42);
            this.textBoxWavecalCoeff.Name = "textBoxWavecalCoeff";
            this.textBoxWavecalCoeff.Size = new System.Drawing.Size(116, 20);
            this.textBoxWavecalCoeff.TabIndex = 41;
            // 
            // buttonWriteWavecalCoeff
            // 
            this.buttonWriteWavecalCoeff.Enabled = false;
            this.buttonWriteWavecalCoeff.Location = new System.Drawing.Point(178, 41);
            this.buttonWriteWavecalCoeff.Name = "buttonWriteWavecalCoeff";
            this.buttonWriteWavecalCoeff.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteWavecalCoeff.TabIndex = 40;
            this.buttonWriteWavecalCoeff.Text = "W";
            this.buttonWriteWavecalCoeff.UseVisualStyleBackColor = true;
            this.buttonWriteWavecalCoeff.Click += new System.EventHandler(this.buttonWriteWavecalCoeff_Click);
            // 
            // buttonReadWavecalCoeff
            // 
            this.buttonReadWavecalCoeff.Enabled = false;
            this.buttonReadWavecalCoeff.Location = new System.Drawing.Point(143, 41);
            this.buttonReadWavecalCoeff.Name = "buttonReadWavecalCoeff";
            this.buttonReadWavecalCoeff.Size = new System.Drawing.Size(29, 21);
            this.buttonReadWavecalCoeff.TabIndex = 39;
            this.buttonReadWavecalCoeff.Text = "R";
            this.buttonReadWavecalCoeff.UseVisualStyleBackColor = true;
            this.buttonReadWavecalCoeff.Click += new System.EventHandler(this.buttonReadWavecalCoeff_Click);
            // 
            // buttonReadNumWavecalCoeffs
            // 
            this.buttonReadNumWavecalCoeffs.Location = new System.Drawing.Point(143, 18);
            this.buttonReadNumWavecalCoeffs.Name = "buttonReadNumWavecalCoeffs";
            this.buttonReadNumWavecalCoeffs.Size = new System.Drawing.Size(29, 21);
            this.buttonReadNumWavecalCoeffs.TabIndex = 38;
            this.buttonReadNumWavecalCoeffs.Text = "R";
            this.buttonReadNumWavecalCoeffs.UseVisualStyleBackColor = true;
            this.buttonReadNumWavecalCoeffs.Click += new System.EventHandler(this.buttonReadNumWavecalCoeffs_Click);
            // 
            // label52
            // 
            this.label52.AutoSize = true;
            this.label52.Location = new System.Drawing.Point(62, 45);
            this.label52.Name = "label52";
            this.label52.Size = new System.Drawing.Size(78, 13);
            this.label52.TabIndex = 37;
            this.label52.Text = "Wavecal Coeff";
            // 
            // label53
            // 
            this.label53.AutoSize = true;
            this.label53.Location = new System.Drawing.Point(32, 22);
            this.label53.Name = "label53";
            this.label53.Size = new System.Drawing.Size(108, 13);
            this.label53.TabIndex = 36;
            this.label53.Text = "Num Wavecal Coeffs";
            // 
            // comboBoxNonLinearityCoeffIndex
            // 
            this.comboBoxNonLinearityCoeffIndex.Enabled = false;
            this.comboBoxNonLinearityCoeffIndex.FormattingEnabled = true;
            this.comboBoxNonLinearityCoeffIndex.Location = new System.Drawing.Point(215, 105);
            this.comboBoxNonLinearityCoeffIndex.Name = "comboBoxNonLinearityCoeffIndex";
            this.comboBoxNonLinearityCoeffIndex.Size = new System.Drawing.Size(44, 21);
            this.comboBoxNonLinearityCoeffIndex.TabIndex = 35;
            // 
            // textBoxNumNonLinearityCoeffs
            // 
            this.textBoxNumNonLinearityCoeffs.Location = new System.Drawing.Point(215, 82);
            this.textBoxNumNonLinearityCoeffs.Name = "textBoxNumNonLinearityCoeffs";
            this.textBoxNumNonLinearityCoeffs.ReadOnly = true;
            this.textBoxNumNonLinearityCoeffs.Size = new System.Drawing.Size(116, 20);
            this.textBoxNumNonLinearityCoeffs.TabIndex = 34;
            // 
            // textBoxNonLinearityCoeff
            // 
            this.textBoxNonLinearityCoeff.Enabled = false;
            this.textBoxNonLinearityCoeff.Location = new System.Drawing.Point(265, 105);
            this.textBoxNonLinearityCoeff.Name = "textBoxNonLinearityCoeff";
            this.textBoxNonLinearityCoeff.Size = new System.Drawing.Size(116, 20);
            this.textBoxNonLinearityCoeff.TabIndex = 33;
            // 
            // buttonWriteNonLinearityCoeff
            // 
            this.buttonWriteNonLinearityCoeff.Enabled = false;
            this.buttonWriteNonLinearityCoeff.Location = new System.Drawing.Point(178, 104);
            this.buttonWriteNonLinearityCoeff.Name = "buttonWriteNonLinearityCoeff";
            this.buttonWriteNonLinearityCoeff.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteNonLinearityCoeff.TabIndex = 32;
            this.buttonWriteNonLinearityCoeff.Text = "W";
            this.buttonWriteNonLinearityCoeff.UseVisualStyleBackColor = true;
            this.buttonWriteNonLinearityCoeff.Click += new System.EventHandler(this.buttonWriteNonLinearityCoeff_Click);
            // 
            // buttonReadNonLinearityCoeff
            // 
            this.buttonReadNonLinearityCoeff.Enabled = false;
            this.buttonReadNonLinearityCoeff.Location = new System.Drawing.Point(143, 104);
            this.buttonReadNonLinearityCoeff.Name = "buttonReadNonLinearityCoeff";
            this.buttonReadNonLinearityCoeff.Size = new System.Drawing.Size(29, 21);
            this.buttonReadNonLinearityCoeff.TabIndex = 31;
            this.buttonReadNonLinearityCoeff.Text = "R";
            this.buttonReadNonLinearityCoeff.UseVisualStyleBackColor = true;
            this.buttonReadNonLinearityCoeff.Click += new System.EventHandler(this.buttonReadNonLinearityCoeff_Click);
            // 
            // buttonReadNumNonLinearityCoeffs
            // 
            this.buttonReadNumNonLinearityCoeffs.Location = new System.Drawing.Point(143, 81);
            this.buttonReadNumNonLinearityCoeffs.Name = "buttonReadNumNonLinearityCoeffs";
            this.buttonReadNumNonLinearityCoeffs.Size = new System.Drawing.Size(29, 21);
            this.buttonReadNumNonLinearityCoeffs.TabIndex = 30;
            this.buttonReadNumNonLinearityCoeffs.Text = "R";
            this.buttonReadNumNonLinearityCoeffs.UseVisualStyleBackColor = true;
            this.buttonReadNumNonLinearityCoeffs.Click += new System.EventHandler(this.buttonReadNumNonLinearityCoeffs_Click);
            // 
            // label50
            // 
            this.label50.AutoSize = true;
            this.label50.Location = new System.Drawing.Point(43, 108);
            this.label50.Name = "label50";
            this.label50.Size = new System.Drawing.Size(97, 13);
            this.label50.TabIndex = 29;
            this.label50.Text = "Non-Linearity Coeff";
            // 
            // label51
            // 
            this.label51.AutoSize = true;
            this.label51.Location = new System.Drawing.Point(13, 85);
            this.label51.Name = "label51";
            this.label51.Size = new System.Drawing.Size(127, 13);
            this.label51.TabIndex = 28;
            this.label51.Text = "Num Non-Linearity Coeffs";
            // 
            // tabPageTecTemp
            // 
            this.tabPageTecTemp.BackColor = System.Drawing.SystemColors.Control;
            this.tabPageTecTemp.Controls.Add(this.groupBoxTec);
            this.tabPageTecTemp.Controls.Add(this.label69);
            this.tabPageTecTemp.Controls.Add(this.buttonReadAllTempSensors);
            this.tabPageTecTemp.Controls.Add(this.textBoxAllTempSensors);
            this.tabPageTecTemp.Controls.Add(this.comboBoxTempSensorIndex);
            this.tabPageTecTemp.Controls.Add(this.textBoxNumTempSensors);
            this.tabPageTecTemp.Controls.Add(this.textBoxTempSensorValue);
            this.tabPageTecTemp.Controls.Add(this.buttonReadTempSensor);
            this.tabPageTecTemp.Controls.Add(this.buttonReadNumTempSensors);
            this.tabPageTecTemp.Controls.Add(this.label67);
            this.tabPageTecTemp.Controls.Add(this.label68);
            this.tabPageTecTemp.Location = new System.Drawing.Point(4, 22);
            this.tabPageTecTemp.Name = "tabPageTecTemp";
            this.tabPageTecTemp.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTecTemp.Size = new System.Drawing.Size(726, 377);
            this.tabPageTecTemp.TabIndex = 6;
            this.tabPageTecTemp.Text = "TEC & Temp";
            // 
            // groupBoxTec
            // 
            this.groupBoxTec.Controls.Add(this.buttonReadTecSetpoint);
            this.groupBoxTec.Controls.Add(this.textBoxTecTemperature);
            this.groupBoxTec.Controls.Add(this.label70);
            this.groupBoxTec.Controls.Add(this.buttonReadTecTemperature);
            this.groupBoxTec.Controls.Add(this.buttonReadTecEnable);
            this.groupBoxTec.Controls.Add(this.label73);
            this.groupBoxTec.Controls.Add(this.buttonWriteTecEnable);
            this.groupBoxTec.Controls.Add(this.textBoxTecStable);
            this.groupBoxTec.Controls.Add(this.textBoxTecEnable);
            this.groupBoxTec.Controls.Add(this.buttonReadTecStable);
            this.groupBoxTec.Controls.Add(this.label71);
            this.groupBoxTec.Controls.Add(this.label72);
            this.groupBoxTec.Controls.Add(this.buttonWriteTecSetpoint);
            this.groupBoxTec.Controls.Add(this.textBoxTecSetpoint);
            this.groupBoxTec.Location = new System.Drawing.Point(21, 147);
            this.groupBoxTec.Name = "groupBoxTec";
            this.groupBoxTec.Size = new System.Drawing.Size(555, 210);
            this.groupBoxTec.TabIndex = 71;
            this.groupBoxTec.TabStop = false;
            this.groupBoxTec.Text = "TEC";
            // 
            // buttonReadTecSetpoint
            // 
            this.buttonReadTecSetpoint.Location = new System.Drawing.Point(104, 52);
            this.buttonReadTecSetpoint.Name = "buttonReadTecSetpoint";
            this.buttonReadTecSetpoint.Size = new System.Drawing.Size(29, 21);
            this.buttonReadTecSetpoint.TabIndex = 60;
            this.buttonReadTecSetpoint.Text = "R";
            this.buttonReadTecSetpoint.UseVisualStyleBackColor = true;
            this.buttonReadTecSetpoint.Click += new System.EventHandler(this.buttonReadTecSetpoint_Click);
            // 
            // textBoxTecTemperature
            // 
            this.textBoxTecTemperature.Location = new System.Drawing.Point(176, 107);
            this.textBoxTecTemperature.Name = "textBoxTecTemperature";
            this.textBoxTecTemperature.ReadOnly = true;
            this.textBoxTecTemperature.Size = new System.Drawing.Size(116, 20);
            this.textBoxTecTemperature.TabIndex = 70;
            // 
            // label70
            // 
            this.label70.AutoSize = true;
            this.label70.Location = new System.Drawing.Point(37, 29);
            this.label70.Name = "label70";
            this.label70.Size = new System.Drawing.Size(64, 13);
            this.label70.TabIndex = 55;
            this.label70.Text = "TEC Enable";
            // 
            // buttonReadTecTemperature
            // 
            this.buttonReadTecTemperature.Location = new System.Drawing.Point(104, 106);
            this.buttonReadTecTemperature.Name = "buttonReadTecTemperature";
            this.buttonReadTecTemperature.Size = new System.Drawing.Size(29, 21);
            this.buttonReadTecTemperature.TabIndex = 68;
            this.buttonReadTecTemperature.Text = "R";
            this.buttonReadTecTemperature.UseVisualStyleBackColor = true;
            this.buttonReadTecTemperature.Click += new System.EventHandler(this.buttonReadTecTemperature_Click);
            // 
            // buttonReadTecEnable
            // 
            this.buttonReadTecEnable.Location = new System.Drawing.Point(104, 25);
            this.buttonReadTecEnable.Name = "buttonReadTecEnable";
            this.buttonReadTecEnable.Size = new System.Drawing.Size(29, 21);
            this.buttonReadTecEnable.TabIndex = 56;
            this.buttonReadTecEnable.Text = "R";
            this.buttonReadTecEnable.UseVisualStyleBackColor = true;
            this.buttonReadTecEnable.Click += new System.EventHandler(this.buttonReadTecEnable_Click);
            // 
            // label73
            // 
            this.label73.AutoSize = true;
            this.label73.Location = new System.Drawing.Point(10, 110);
            this.label73.Name = "label73";
            this.label73.Size = new System.Drawing.Size(91, 13);
            this.label73.TabIndex = 67;
            this.label73.Text = "TEC Temperature";
            // 
            // buttonWriteTecEnable
            // 
            this.buttonWriteTecEnable.Location = new System.Drawing.Point(139, 25);
            this.buttonWriteTecEnable.Name = "buttonWriteTecEnable";
            this.buttonWriteTecEnable.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteTecEnable.TabIndex = 57;
            this.buttonWriteTecEnable.Text = "W";
            this.buttonWriteTecEnable.UseVisualStyleBackColor = true;
            this.buttonWriteTecEnable.Click += new System.EventHandler(this.buttonWriteTecEnable_Click);
            // 
            // textBoxTecStable
            // 
            this.textBoxTecStable.Location = new System.Drawing.Point(176, 80);
            this.textBoxTecStable.Name = "textBoxTecStable";
            this.textBoxTecStable.ReadOnly = true;
            this.textBoxTecStable.Size = new System.Drawing.Size(116, 20);
            this.textBoxTecStable.TabIndex = 66;
            // 
            // textBoxTecEnable
            // 
            this.textBoxTecEnable.Location = new System.Drawing.Point(176, 26);
            this.textBoxTecEnable.Name = "textBoxTecEnable";
            this.textBoxTecEnable.Size = new System.Drawing.Size(116, 20);
            this.textBoxTecEnable.TabIndex = 58;
            // 
            // buttonReadTecStable
            // 
            this.buttonReadTecStable.Location = new System.Drawing.Point(104, 79);
            this.buttonReadTecStable.Name = "buttonReadTecStable";
            this.buttonReadTecStable.Size = new System.Drawing.Size(29, 21);
            this.buttonReadTecStable.TabIndex = 64;
            this.buttonReadTecStable.Text = "R";
            this.buttonReadTecStable.UseVisualStyleBackColor = true;
            this.buttonReadTecStable.Click += new System.EventHandler(this.buttonReadTecStable_Click);
            // 
            // label71
            // 
            this.label71.AutoSize = true;
            this.label71.Location = new System.Drawing.Point(31, 56);
            this.label71.Name = "label71";
            this.label71.Size = new System.Drawing.Size(70, 13);
            this.label71.TabIndex = 59;
            this.label71.Text = "TEC Setpoint";
            // 
            // label72
            // 
            this.label72.AutoSize = true;
            this.label72.Location = new System.Drawing.Point(29, 83);
            this.label72.Name = "label72";
            this.label72.Size = new System.Drawing.Size(72, 13);
            this.label72.TabIndex = 63;
            this.label72.Text = "Is TEC Stable";
            // 
            // buttonWriteTecSetpoint
            // 
            this.buttonWriteTecSetpoint.Location = new System.Drawing.Point(139, 52);
            this.buttonWriteTecSetpoint.Name = "buttonWriteTecSetpoint";
            this.buttonWriteTecSetpoint.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteTecSetpoint.TabIndex = 61;
            this.buttonWriteTecSetpoint.Text = "W";
            this.buttonWriteTecSetpoint.UseVisualStyleBackColor = true;
            this.buttonWriteTecSetpoint.Click += new System.EventHandler(this.buttonWriteTecSetpoint_Click);
            // 
            // textBoxTecSetpoint
            // 
            this.textBoxTecSetpoint.Location = new System.Drawing.Point(176, 53);
            this.textBoxTecSetpoint.Name = "textBoxTecSetpoint";
            this.textBoxTecSetpoint.Size = new System.Drawing.Size(116, 20);
            this.textBoxTecSetpoint.TabIndex = 62;
            // 
            // label69
            // 
            this.label69.AutoSize = true;
            this.label69.Location = new System.Drawing.Point(59, 77);
            this.label69.Name = "label69";
            this.label69.Size = new System.Drawing.Size(122, 13);
            this.label69.TabIndex = 54;
            this.label69.Text = "All Temperature Sensors";
            // 
            // buttonReadAllTempSensors
            // 
            this.buttonReadAllTempSensors.Location = new System.Drawing.Point(24, 73);
            this.buttonReadAllTempSensors.Name = "buttonReadAllTempSensors";
            this.buttonReadAllTempSensors.Size = new System.Drawing.Size(29, 21);
            this.buttonReadAllTempSensors.TabIndex = 53;
            this.buttonReadAllTempSensors.Text = "R";
            this.buttonReadAllTempSensors.UseVisualStyleBackColor = true;
            this.buttonReadAllTempSensors.Click += new System.EventHandler(this.buttonReadAllTempSensors_Click);
            // 
            // textBoxAllTempSensors
            // 
            this.textBoxAllTempSensors.Location = new System.Drawing.Point(21, 100);
            this.textBoxAllTempSensors.Name = "textBoxAllTempSensors";
            this.textBoxAllTempSensors.ReadOnly = true;
            this.textBoxAllTempSensors.Size = new System.Drawing.Size(552, 20);
            this.textBoxAllTempSensors.TabIndex = 52;
            // 
            // comboBoxTempSensorIndex
            // 
            this.comboBoxTempSensorIndex.Enabled = false;
            this.comboBoxTempSensorIndex.FormattingEnabled = true;
            this.comboBoxTempSensorIndex.Location = new System.Drawing.Point(193, 41);
            this.comboBoxTempSensorIndex.Name = "comboBoxTempSensorIndex";
            this.comboBoxTempSensorIndex.Size = new System.Drawing.Size(44, 21);
            this.comboBoxTempSensorIndex.TabIndex = 51;
            // 
            // textBoxNumTempSensors
            // 
            this.textBoxNumTempSensors.Location = new System.Drawing.Point(193, 18);
            this.textBoxNumTempSensors.Name = "textBoxNumTempSensors";
            this.textBoxNumTempSensors.ReadOnly = true;
            this.textBoxNumTempSensors.Size = new System.Drawing.Size(116, 20);
            this.textBoxNumTempSensors.TabIndex = 50;
            // 
            // textBoxTempSensorValue
            // 
            this.textBoxTempSensorValue.Enabled = false;
            this.textBoxTempSensorValue.Location = new System.Drawing.Point(243, 41);
            this.textBoxTempSensorValue.Name = "textBoxTempSensorValue";
            this.textBoxTempSensorValue.Size = new System.Drawing.Size(116, 20);
            this.textBoxTempSensorValue.TabIndex = 49;
            // 
            // buttonReadTempSensor
            // 
            this.buttonReadTempSensor.Enabled = false;
            this.buttonReadTempSensor.Location = new System.Drawing.Point(121, 40);
            this.buttonReadTempSensor.Name = "buttonReadTempSensor";
            this.buttonReadTempSensor.Size = new System.Drawing.Size(29, 21);
            this.buttonReadTempSensor.TabIndex = 47;
            this.buttonReadTempSensor.Text = "R";
            this.buttonReadTempSensor.UseVisualStyleBackColor = true;
            this.buttonReadTempSensor.Click += new System.EventHandler(this.buttonReadTempSensor_Click);
            // 
            // buttonReadNumTempSensors
            // 
            this.buttonReadNumTempSensors.Location = new System.Drawing.Point(121, 17);
            this.buttonReadNumTempSensors.Name = "buttonReadNumTempSensors";
            this.buttonReadNumTempSensors.Size = new System.Drawing.Size(29, 21);
            this.buttonReadNumTempSensors.TabIndex = 46;
            this.buttonReadNumTempSensors.Text = "R";
            this.buttonReadNumTempSensors.UseVisualStyleBackColor = true;
            this.buttonReadNumTempSensors.Click += new System.EventHandler(this.buttonReadNumTempSensors_Click);
            // 
            // label67
            // 
            this.label67.AutoSize = true;
            this.label67.Location = new System.Drawing.Point(48, 44);
            this.label67.Name = "label67";
            this.label67.Size = new System.Drawing.Size(70, 13);
            this.label67.TabIndex = 45;
            this.label67.Text = "Temp Sensor";
            // 
            // label68
            // 
            this.label68.AutoSize = true;
            this.label68.Location = new System.Drawing.Point(18, 21);
            this.label68.Name = "label68";
            this.label68.Size = new System.Drawing.Size(100, 13);
            this.label68.TabIndex = 44;
            this.label68.Text = "Num Temp Sensors";
            // 
            // tabPageGPIO
            // 
            this.tabPageGPIO.BackColor = System.Drawing.SystemColors.Control;
            this.tabPageGPIO.Controls.Add(this.label78);
            this.tabPageGPIO.Controls.Add(this.textBoxGPIOOutputEnableMask);
            this.tabPageGPIO.Controls.Add(this.label77);
            this.tabPageGPIO.Controls.Add(this.textBoxGPIOValueMask);
            this.tabPageGPIO.Controls.Add(this.textBoxGPIOValue);
            this.tabPageGPIO.Controls.Add(this.buttonWriteGPIOValue);
            this.tabPageGPIO.Controls.Add(this.buttonReadGPIOValue);
            this.tabPageGPIO.Controls.Add(this.label76);
            this.tabPageGPIO.Controls.Add(this.textBoxGPIOOutputEnable);
            this.tabPageGPIO.Controls.Add(this.buttonWriteGPIOOutputEnable);
            this.tabPageGPIO.Controls.Add(this.buttonReadGPIOOutputEnable);
            this.tabPageGPIO.Controls.Add(this.label75);
            this.tabPageGPIO.Controls.Add(this.textBoxNumGPIOPins);
            this.tabPageGPIO.Controls.Add(this.buttonReadNumGPIOPins);
            this.tabPageGPIO.Controls.Add(this.label74);
            this.tabPageGPIO.Location = new System.Drawing.Point(4, 22);
            this.tabPageGPIO.Name = "tabPageGPIO";
            this.tabPageGPIO.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageGPIO.Size = new System.Drawing.Size(726, 377);
            this.tabPageGPIO.TabIndex = 7;
            this.tabPageGPIO.Text = "GPIO";
            // 
            // label78
            // 
            this.label78.AutoSize = true;
            this.label78.Location = new System.Drawing.Point(309, 52);
            this.label78.Name = "label78";
            this.label78.Size = new System.Drawing.Size(57, 13);
            this.label78.TabIndex = 48;
            this.label78.Text = "write mask";
            // 
            // textBoxGPIOOutputEnableMask
            // 
            this.textBoxGPIOOutputEnableMask.Location = new System.Drawing.Point(365, 49);
            this.textBoxGPIOOutputEnableMask.Name = "textBoxGPIOOutputEnableMask";
            this.textBoxGPIOOutputEnableMask.Size = new System.Drawing.Size(116, 20);
            this.textBoxGPIOOutputEnableMask.TabIndex = 47;
            this.textBoxGPIOOutputEnableMask.Text = "0xFFFFFFFF";
            // 
            // label77
            // 
            this.label77.AutoSize = true;
            this.label77.Location = new System.Drawing.Point(309, 79);
            this.label77.Name = "label77";
            this.label77.Size = new System.Drawing.Size(57, 13);
            this.label77.TabIndex = 46;
            this.label77.Text = "write mask";
            // 
            // textBoxGPIOValueMask
            // 
            this.textBoxGPIOValueMask.Location = new System.Drawing.Point(365, 76);
            this.textBoxGPIOValueMask.Name = "textBoxGPIOValueMask";
            this.textBoxGPIOValueMask.Size = new System.Drawing.Size(116, 20);
            this.textBoxGPIOValueMask.TabIndex = 45;
            this.textBoxGPIOValueMask.Text = "0xFFFFFFFF";
            // 
            // textBoxGPIOValue
            // 
            this.textBoxGPIOValue.Location = new System.Drawing.Point(187, 76);
            this.textBoxGPIOValue.Name = "textBoxGPIOValue";
            this.textBoxGPIOValue.Size = new System.Drawing.Size(116, 20);
            this.textBoxGPIOValue.TabIndex = 44;
            // 
            // buttonWriteGPIOValue
            // 
            this.buttonWriteGPIOValue.Location = new System.Drawing.Point(150, 75);
            this.buttonWriteGPIOValue.Name = "buttonWriteGPIOValue";
            this.buttonWriteGPIOValue.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteGPIOValue.TabIndex = 43;
            this.buttonWriteGPIOValue.Text = "W";
            this.buttonWriteGPIOValue.UseVisualStyleBackColor = true;
            this.buttonWriteGPIOValue.Click += new System.EventHandler(this.buttonWriteGPIOValue_Click);
            // 
            // buttonReadGPIOValue
            // 
            this.buttonReadGPIOValue.Location = new System.Drawing.Point(115, 75);
            this.buttonReadGPIOValue.Name = "buttonReadGPIOValue";
            this.buttonReadGPIOValue.Size = new System.Drawing.Size(29, 21);
            this.buttonReadGPIOValue.TabIndex = 42;
            this.buttonReadGPIOValue.Text = "R";
            this.buttonReadGPIOValue.UseVisualStyleBackColor = true;
            this.buttonReadGPIOValue.Click += new System.EventHandler(this.buttonReadGPIOValue_Click);
            // 
            // label76
            // 
            this.label76.AutoSize = true;
            this.label76.Location = new System.Drawing.Point(79, 79);
            this.label76.Name = "label76";
            this.label76.Size = new System.Drawing.Size(34, 13);
            this.label76.TabIndex = 41;
            this.label76.Text = "Value";
            // 
            // textBoxGPIOOutputEnable
            // 
            this.textBoxGPIOOutputEnable.Location = new System.Drawing.Point(187, 49);
            this.textBoxGPIOOutputEnable.Name = "textBoxGPIOOutputEnable";
            this.textBoxGPIOOutputEnable.Size = new System.Drawing.Size(116, 20);
            this.textBoxGPIOOutputEnable.TabIndex = 40;
            // 
            // buttonWriteGPIOOutputEnable
            // 
            this.buttonWriteGPIOOutputEnable.Location = new System.Drawing.Point(150, 48);
            this.buttonWriteGPIOOutputEnable.Name = "buttonWriteGPIOOutputEnable";
            this.buttonWriteGPIOOutputEnable.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteGPIOOutputEnable.TabIndex = 39;
            this.buttonWriteGPIOOutputEnable.Text = "W";
            this.buttonWriteGPIOOutputEnable.UseVisualStyleBackColor = true;
            this.buttonWriteGPIOOutputEnable.Click += new System.EventHandler(this.buttonWriteGPIOOutputEnable_Click);
            // 
            // buttonReadGPIOOutputEnable
            // 
            this.buttonReadGPIOOutputEnable.Location = new System.Drawing.Point(115, 48);
            this.buttonReadGPIOOutputEnable.Name = "buttonReadGPIOOutputEnable";
            this.buttonReadGPIOOutputEnable.Size = new System.Drawing.Size(29, 21);
            this.buttonReadGPIOOutputEnable.TabIndex = 38;
            this.buttonReadGPIOOutputEnable.Text = "R";
            this.buttonReadGPIOOutputEnable.UseVisualStyleBackColor = true;
            this.buttonReadGPIOOutputEnable.Click += new System.EventHandler(this.buttonReadGPIOOutputEnable_Click);
            // 
            // label75
            // 
            this.label75.AutoSize = true;
            this.label75.Location = new System.Drawing.Point(38, 52);
            this.label75.Name = "label75";
            this.label75.Size = new System.Drawing.Size(75, 13);
            this.label75.TabIndex = 37;
            this.label75.Text = "Output Enable";
            // 
            // textBoxNumGPIOPins
            // 
            this.textBoxNumGPIOPins.Location = new System.Drawing.Point(187, 22);
            this.textBoxNumGPIOPins.Name = "textBoxNumGPIOPins";
            this.textBoxNumGPIOPins.ReadOnly = true;
            this.textBoxNumGPIOPins.Size = new System.Drawing.Size(116, 20);
            this.textBoxNumGPIOPins.TabIndex = 36;
            // 
            // buttonReadNumGPIOPins
            // 
            this.buttonReadNumGPIOPins.Location = new System.Drawing.Point(115, 21);
            this.buttonReadNumGPIOPins.Name = "buttonReadNumGPIOPins";
            this.buttonReadNumGPIOPins.Size = new System.Drawing.Size(29, 21);
            this.buttonReadNumGPIOPins.TabIndex = 34;
            this.buttonReadNumGPIOPins.Text = "R";
            this.buttonReadNumGPIOPins.UseVisualStyleBackColor = true;
            this.buttonReadNumGPIOPins.Click += new System.EventHandler(this.buttonReadNumGPIOPins_Click);
            // 
            // label74
            // 
            this.label74.AutoSize = true;
            this.label74.Location = new System.Drawing.Point(32, 25);
            this.label74.Name = "label74";
            this.label74.Size = new System.Drawing.Size(81, 13);
            this.label74.TabIndex = 33;
            this.label74.Text = "Num GPIO Pins";
            // 
            // tabPageI2C
            // 
            this.tabPageI2C.BackColor = System.Drawing.SystemColors.Control;
            this.tabPageI2C.Controls.Add(this.label87);
            this.tabPageI2C.Controls.Add(this.label86);
            this.tabPageI2C.Controls.Add(this.label85);
            this.tabPageI2C.Controls.Add(this.comboBoxI2CClockBusNum);
            this.tabPageI2C.Controls.Add(this.textBoxI2CClockLimit);
            this.tabPageI2C.Controls.Add(this.buttonWriteI2CClockLimit);
            this.tabPageI2C.Controls.Add(this.label84);
            this.tabPageI2C.Controls.Add(this.textBoxI2CData);
            this.tabPageI2C.Controls.Add(this.label83);
            this.tabPageI2C.Controls.Add(this.textBoxI2CNumBytes);
            this.tabPageI2C.Controls.Add(this.label82);
            this.tabPageI2C.Controls.Add(this.label81);
            this.tabPageI2C.Controls.Add(this.textBoxI2CNumBuses);
            this.tabPageI2C.Controls.Add(this.buttonReadI2CNumBuses);
            this.tabPageI2C.Controls.Add(this.label80);
            this.tabPageI2C.Controls.Add(this.comboBoxI2CDataBusNum);
            this.tabPageI2C.Controls.Add(this.textBoxI2CAddress);
            this.tabPageI2C.Controls.Add(this.buttonWriteI2CBytes);
            this.tabPageI2C.Controls.Add(this.buttonReadI2CBytes);
            this.tabPageI2C.Controls.Add(this.label79);
            this.tabPageI2C.Location = new System.Drawing.Point(4, 22);
            this.tabPageI2C.Name = "tabPageI2C";
            this.tabPageI2C.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageI2C.Size = new System.Drawing.Size(726, 377);
            this.tabPageI2C.TabIndex = 8;
            this.tabPageI2C.Text = "I2C";
            // 
            // label87
            // 
            this.label87.AutoSize = true;
            this.label87.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.label87.Location = new System.Drawing.Point(371, 106);
            this.label87.Name = "label87";
            this.label87.Size = new System.Drawing.Size(253, 13);
            this.label87.TabIndex = 48;
            this.label87.Text = "(space separated hex bytes such as: 00 01 F0 A2...)";
            // 
            // label86
            // 
            this.label86.AutoSize = true;
            this.label86.Location = new System.Drawing.Point(235, 299);
            this.label86.Name = "label86";
            this.label86.Size = new System.Drawing.Size(20, 13);
            this.label86.TabIndex = 47;
            this.label86.Text = "Hz";
            // 
            // label85
            // 
            this.label85.AutoSize = true;
            this.label85.Location = new System.Drawing.Point(177, 299);
            this.label85.Name = "label85";
            this.label85.Size = new System.Drawing.Size(25, 13);
            this.label85.TabIndex = 46;
            this.label85.Text = "Bus";
            // 
            // comboBoxI2CClockBusNum
            // 
            this.comboBoxI2CClockBusNum.Enabled = false;
            this.comboBoxI2CClockBusNum.FormattingEnabled = true;
            this.comboBoxI2CClockBusNum.Location = new System.Drawing.Point(171, 315);
            this.comboBoxI2CClockBusNum.Name = "comboBoxI2CClockBusNum";
            this.comboBoxI2CClockBusNum.Size = new System.Drawing.Size(44, 21);
            this.comboBoxI2CClockBusNum.TabIndex = 45;
            // 
            // textBoxI2CClockLimit
            // 
            this.textBoxI2CClockLimit.Enabled = false;
            this.textBoxI2CClockLimit.Location = new System.Drawing.Point(221, 315);
            this.textBoxI2CClockLimit.Name = "textBoxI2CClockLimit";
            this.textBoxI2CClockLimit.Size = new System.Drawing.Size(66, 20);
            this.textBoxI2CClockLimit.TabIndex = 44;
            // 
            // buttonWriteI2CClockLimit
            // 
            this.buttonWriteI2CClockLimit.Enabled = false;
            this.buttonWriteI2CClockLimit.Location = new System.Drawing.Point(134, 314);
            this.buttonWriteI2CClockLimit.Name = "buttonWriteI2CClockLimit";
            this.buttonWriteI2CClockLimit.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteI2CClockLimit.TabIndex = 43;
            this.buttonWriteI2CClockLimit.Text = "W";
            this.buttonWriteI2CClockLimit.UseVisualStyleBackColor = true;
            this.buttonWriteI2CClockLimit.Click += new System.EventHandler(this.buttonWriteI2CClockLimit_Click);
            // 
            // label84
            // 
            this.label84.AutoSize = true;
            this.label84.Location = new System.Drawing.Point(37, 318);
            this.label84.Name = "label84";
            this.label84.Size = new System.Drawing.Size(58, 13);
            this.label84.TabIndex = 41;
            this.label84.Text = "Clock Limit";
            // 
            // textBoxI2CData
            // 
            this.textBoxI2CData.Enabled = false;
            this.textBoxI2CData.Location = new System.Drawing.Point(99, 103);
            this.textBoxI2CData.Multiline = true;
            this.textBoxI2CData.Name = "textBoxI2CData";
            this.textBoxI2CData.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxI2CData.Size = new System.Drawing.Size(261, 177);
            this.textBoxI2CData.TabIndex = 40;
            // 
            // label83
            // 
            this.label83.AutoSize = true;
            this.label83.Location = new System.Drawing.Point(297, 60);
            this.label83.Name = "label83";
            this.label83.Size = new System.Drawing.Size(58, 13);
            this.label83.TabIndex = 39;
            this.label83.Text = "Num Bytes";
            // 
            // textBoxI2CNumBytes
            // 
            this.textBoxI2CNumBytes.Enabled = false;
            this.textBoxI2CNumBytes.Location = new System.Drawing.Point(294, 77);
            this.textBoxI2CNumBytes.Name = "textBoxI2CNumBytes";
            this.textBoxI2CNumBytes.Size = new System.Drawing.Size(66, 20);
            this.textBoxI2CNumBytes.TabIndex = 38;
            // 
            // label82
            // 
            this.label82.AutoSize = true;
            this.label82.Location = new System.Drawing.Point(230, 60);
            this.label82.Name = "label82";
            this.label82.Size = new System.Drawing.Size(45, 13);
            this.label82.TabIndex = 37;
            this.label82.Text = "Address";
            // 
            // label81
            // 
            this.label81.AutoSize = true;
            this.label81.Location = new System.Drawing.Point(177, 60);
            this.label81.Name = "label81";
            this.label81.Size = new System.Drawing.Size(25, 13);
            this.label81.TabIndex = 36;
            this.label81.Text = "Bus";
            // 
            // textBoxI2CNumBuses
            // 
            this.textBoxI2CNumBuses.Location = new System.Drawing.Point(171, 21);
            this.textBoxI2CNumBuses.Name = "textBoxI2CNumBuses";
            this.textBoxI2CNumBuses.ReadOnly = true;
            this.textBoxI2CNumBuses.Size = new System.Drawing.Size(116, 20);
            this.textBoxI2CNumBuses.TabIndex = 35;
            // 
            // buttonReadI2CNumBuses
            // 
            this.buttonReadI2CNumBuses.Location = new System.Drawing.Point(99, 20);
            this.buttonReadI2CNumBuses.Name = "buttonReadI2CNumBuses";
            this.buttonReadI2CNumBuses.Size = new System.Drawing.Size(29, 21);
            this.buttonReadI2CNumBuses.TabIndex = 34;
            this.buttonReadI2CNumBuses.Text = "R";
            this.buttonReadI2CNumBuses.UseVisualStyleBackColor = true;
            this.buttonReadI2CNumBuses.Click += new System.EventHandler(this.buttonReadI2CNumBuses_Click);
            // 
            // label80
            // 
            this.label80.AutoSize = true;
            this.label80.Location = new System.Drawing.Point(15, 24);
            this.label80.Name = "label80";
            this.label80.Size = new System.Drawing.Size(80, 13);
            this.label80.TabIndex = 33;
            this.label80.Text = "Num I2C Buses";
            // 
            // comboBoxI2CDataBusNum
            // 
            this.comboBoxI2CDataBusNum.Enabled = false;
            this.comboBoxI2CDataBusNum.FormattingEnabled = true;
            this.comboBoxI2CDataBusNum.Location = new System.Drawing.Point(171, 76);
            this.comboBoxI2CDataBusNum.Name = "comboBoxI2CDataBusNum";
            this.comboBoxI2CDataBusNum.Size = new System.Drawing.Size(44, 21);
            this.comboBoxI2CDataBusNum.TabIndex = 32;
            // 
            // textBoxI2CAddress
            // 
            this.textBoxI2CAddress.Enabled = false;
            this.textBoxI2CAddress.Location = new System.Drawing.Point(221, 76);
            this.textBoxI2CAddress.Name = "textBoxI2CAddress";
            this.textBoxI2CAddress.Size = new System.Drawing.Size(66, 20);
            this.textBoxI2CAddress.TabIndex = 31;
            // 
            // buttonWriteI2CBytes
            // 
            this.buttonWriteI2CBytes.Enabled = false;
            this.buttonWriteI2CBytes.Location = new System.Drawing.Point(134, 75);
            this.buttonWriteI2CBytes.Name = "buttonWriteI2CBytes";
            this.buttonWriteI2CBytes.Size = new System.Drawing.Size(29, 21);
            this.buttonWriteI2CBytes.TabIndex = 30;
            this.buttonWriteI2CBytes.Text = "W";
            this.buttonWriteI2CBytes.UseVisualStyleBackColor = true;
            this.buttonWriteI2CBytes.Click += new System.EventHandler(this.buttonWriteI2CBytes_Click);
            // 
            // buttonReadI2CBytes
            // 
            this.buttonReadI2CBytes.Enabled = false;
            this.buttonReadI2CBytes.Location = new System.Drawing.Point(99, 75);
            this.buttonReadI2CBytes.Name = "buttonReadI2CBytes";
            this.buttonReadI2CBytes.Size = new System.Drawing.Size(29, 21);
            this.buttonReadI2CBytes.TabIndex = 29;
            this.buttonReadI2CBytes.Text = "R";
            this.buttonReadI2CBytes.UseVisualStyleBackColor = true;
            this.buttonReadI2CBytes.Click += new System.EventHandler(this.buttonReadI2CBytes_Click);
            // 
            // label79
            // 
            this.label79.AutoSize = true;
            this.label79.Location = new System.Drawing.Point(65, 106);
            this.label79.Name = "label79";
            this.label79.Size = new System.Drawing.Size(30, 13);
            this.label79.TabIndex = 28;
            this.label79.Text = "Data";
            // 
            // checkBoxMD5Checksum
            // 
            this.checkBoxMD5Checksum.AutoSize = true;
            this.checkBoxMD5Checksum.Location = new System.Drawing.Point(269, 234);
            this.checkBoxMD5Checksum.Name = "checkBoxMD5Checksum";
            this.checkBoxMD5Checksum.Size = new System.Drawing.Size(102, 17);
            this.checkBoxMD5Checksum.TabIndex = 29;
            this.checkBoxMD5Checksum.Text = "MD5 Checksum";
            this.checkBoxMD5Checksum.UseVisualStyleBackColor = true;
            // 
            // textBoxRegarding
            // 
            this.textBoxRegarding.Location = new System.Drawing.Point(463, 232);
            this.textBoxRegarding.Name = "textBoxRegarding";
            this.textBoxRegarding.Size = new System.Drawing.Size(105, 20);
            this.textBoxRegarding.TabIndex = 31;
            this.textBoxRegarding.TextChanged += new System.EventHandler(this.textBoxRegarding_TextChanged);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(404, 235);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(56, 13);
            this.label11.TabIndex = 30;
            this.label11.Text = "Regarding";
            // 
            // tabControlProtocol
            // 
            this.tabControlProtocol.Controls.Add(this.tabPageRS232);
            this.tabControlProtocol.Controls.Add(this.tabPageTCPIP);
            this.tabControlProtocol.Controls.Add(this.tabPageWinUSBNet);
            this.tabControlProtocol.Location = new System.Drawing.Point(18, 15);
            this.tabControlProtocol.Name = "tabControlProtocol";
            this.tabControlProtocol.SelectedIndex = 0;
            this.tabControlProtocol.Size = new System.Drawing.Size(589, 185);
            this.tabControlProtocol.TabIndex = 32;
            this.tabControlProtocol.SelectedIndexChanged += new System.EventHandler(this.tabControlProtocol_SelectedIndexChanged);
            // 
            // tabPageRS232
            // 
            this.tabPageRS232.BackColor = System.Drawing.SystemColors.Control;
            this.tabPageRS232.Controls.Add(this.comboBoxBaudRate);
            this.tabPageRS232.Controls.Add(this.comboBoxComPort);
            this.tabPageRS232.Controls.Add(this.comboBoxDataBits);
            this.tabPageRS232.Controls.Add(this.comboBoxParity);
            this.tabPageRS232.Controls.Add(this.comboBoxStopBits);
            this.tabPageRS232.Controls.Add(this.comboBoxHandshaking);
            this.tabPageRS232.Controls.Add(this.label1);
            this.tabPageRS232.Controls.Add(this.label2);
            this.tabPageRS232.Controls.Add(this.label3);
            this.tabPageRS232.Controls.Add(this.label4);
            this.tabPageRS232.Controls.Add(this.label5);
            this.tabPageRS232.Controls.Add(this.label7);
            this.tabPageRS232.Controls.Add(this.labelPortName);
            this.tabPageRS232.Controls.Add(this.labelHandshaking);
            this.tabPageRS232.Controls.Add(this.labelBaudRate);
            this.tabPageRS232.Controls.Add(this.labelStopBits);
            this.tabPageRS232.Controls.Add(this.labelDataBits);
            this.tabPageRS232.Controls.Add(this.labelParity);
            this.tabPageRS232.Location = new System.Drawing.Point(4, 22);
            this.tabPageRS232.Name = "tabPageRS232";
            this.tabPageRS232.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageRS232.Size = new System.Drawing.Size(581, 159);
            this.tabPageRS232.TabIndex = 0;
            this.tabPageRS232.Text = "RS232";
            // 
            // tabPageTCPIP
            // 
            this.tabPageTCPIP.BackColor = System.Drawing.SystemColors.Control;
            this.tabPageTCPIP.Controls.Add(this.label14);
            this.tabPageTCPIP.Controls.Add(this.buttonDisconnect);
            this.tabPageTCPIP.Controls.Add(this.textBoxPortNum);
            this.tabPageTCPIP.Controls.Add(this.textBoxIPAddress);
            this.tabPageTCPIP.Controls.Add(this.label12);
            this.tabPageTCPIP.Controls.Add(this.label13);
            this.tabPageTCPIP.Controls.Add(this.labelIPAddress);
            this.tabPageTCPIP.Controls.Add(this.labelPortNum);
            this.tabPageTCPIP.Location = new System.Drawing.Point(4, 22);
            this.tabPageTCPIP.Name = "tabPageTCPIP";
            this.tabPageTCPIP.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTCPIP.Size = new System.Drawing.Size(581, 159);
            this.tabPageTCPIP.TabIndex = 1;
            this.tabPageTCPIP.Text = "TCP-IP";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(434, 27);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(126, 13);
            this.label14.TabIndex = 26;
            this.label14.Text = "(press \"Test\" to connect)";
            // 
            // buttonDisconnect
            // 
            this.buttonDisconnect.Enabled = false;
            this.buttonDisconnect.Location = new System.Drawing.Point(344, 22);
            this.buttonDisconnect.Name = "buttonDisconnect";
            this.buttonDisconnect.Size = new System.Drawing.Size(75, 23);
            this.buttonDisconnect.TabIndex = 25;
            this.buttonDisconnect.Text = "Disconnect";
            this.buttonDisconnect.UseVisualStyleBackColor = true;
            this.buttonDisconnect.Click += new System.EventHandler(this.buttonDisconnect_Click);
            // 
            // textBoxPortNum
            // 
            this.textBoxPortNum.Location = new System.Drawing.Point(263, 25);
            this.textBoxPortNum.Name = "textBoxPortNum";
            this.textBoxPortNum.Size = new System.Drawing.Size(63, 20);
            this.textBoxPortNum.TabIndex = 24;
            // 
            // textBoxIPAddress
            // 
            this.textBoxIPAddress.Location = new System.Drawing.Point(25, 25);
            this.textBoxIPAddress.Name = "textBoxIPAddress";
            this.textBoxIPAddress.Size = new System.Drawing.Size(224, 20);
            this.textBoxIPAddress.TabIndex = 23;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(82, 8);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(106, 13);
            this.label12.TabIndex = 18;
            this.label12.Text = "IP Address (v4 or v6)";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(271, 8);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(36, 13);
            this.label13.TabIndex = 20;
            this.label13.Text = "Port #";
            // 
            // labelIPAddress
            // 
            this.labelIPAddress.AutoSize = true;
            this.labelIPAddress.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelIPAddress.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.labelIPAddress.Location = new System.Drawing.Point(45, 47);
            this.labelIPAddress.Name = "labelIPAddress";
            this.labelIPAddress.Size = new System.Drawing.Size(54, 12);
            this.labelIPAddress.TabIndex = 21;
            this.labelIPAddress.Text = "192.168.1.2";
            // 
            // labelPortNum
            // 
            this.labelPortNum.AutoSize = true;
            this.labelPortNum.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelPortNum.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.labelPortNum.Location = new System.Drawing.Point(280, 47);
            this.labelPortNum.Name = "labelPortNum";
            this.labelPortNum.Size = new System.Drawing.Size(25, 12);
            this.labelPortNum.TabIndex = 22;
            this.labelPortNum.Text = "8080";
            // 
            // tabPageWinUSBNet
            // 
            this.tabPageWinUSBNet.Controls.Add(this.dataGridViewOutPipes);
            this.tabPageWinUSBNet.Controls.Add(this.dataGridViewInPipes);
            this.tabPageWinUSBNet.Controls.Add(this.label89);
            this.tabPageWinUSBNet.Controls.Add(this.label88);
            this.tabPageWinUSBNet.Controls.Add(this.dataGridViewUSBDeviceList);
            this.tabPageWinUSBNet.Location = new System.Drawing.Point(4, 22);
            this.tabPageWinUSBNet.Name = "tabPageWinUSBNet";
            this.tabPageWinUSBNet.Size = new System.Drawing.Size(581, 159);
            this.tabPageWinUSBNet.TabIndex = 2;
            this.tabPageWinUSBNet.Text = "WinUSBNet";
            this.tabPageWinUSBNet.UseVisualStyleBackColor = true;
            // 
            // dataGridViewOutPipes
            // 
            this.dataGridViewOutPipes.AllowUserToAddRows = false;
            this.dataGridViewOutPipes.AllowUserToDeleteRows = false;
            this.dataGridViewOutPipes.AllowUserToResizeColumns = false;
            this.dataGridViewOutPipes.AllowUserToResizeRows = false;
            this.dataGridViewOutPipes.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridViewOutPipes.ColumnHeadersVisible = false;
            this.dataGridViewOutPipes.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.dataGridViewTextBoxColumn1});
            this.dataGridViewOutPipes.Location = new System.Drawing.Point(477, 57);
            this.dataGridViewOutPipes.MultiSelect = false;
            this.dataGridViewOutPipes.Name = "dataGridViewOutPipes";
            this.dataGridViewOutPipes.RowHeadersVisible = false;
            this.dataGridViewOutPipes.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.dataGridViewOutPipes.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridViewOutPipes.Size = new System.Drawing.Size(50, 75);
            this.dataGridViewOutPipes.TabIndex = 6;
            // 
            // dataGridViewTextBoxColumn1
            // 
            this.dataGridViewTextBoxColumn1.FillWeight = 50F;
            this.dataGridViewTextBoxColumn1.HeaderText = "Pipes";
            this.dataGridViewTextBoxColumn1.Name = "dataGridViewTextBoxColumn1";
            this.dataGridViewTextBoxColumn1.ReadOnly = true;
            this.dataGridViewTextBoxColumn1.Width = 50;
            // 
            // dataGridViewInPipes
            // 
            this.dataGridViewInPipes.AllowUserToAddRows = false;
            this.dataGridViewInPipes.AllowUserToDeleteRows = false;
            this.dataGridViewInPipes.AllowUserToResizeColumns = false;
            this.dataGridViewInPipes.AllowUserToResizeRows = false;
            this.dataGridViewInPipes.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridViewInPipes.ColumnHeadersVisible = false;
            this.dataGridViewInPipes.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ColumnPipes});
            this.dataGridViewInPipes.Location = new System.Drawing.Point(385, 57);
            this.dataGridViewInPipes.MultiSelect = false;
            this.dataGridViewInPipes.Name = "dataGridViewInPipes";
            this.dataGridViewInPipes.RowHeadersVisible = false;
            this.dataGridViewInPipes.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.dataGridViewInPipes.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridViewInPipes.Size = new System.Drawing.Size(50, 75);
            this.dataGridViewInPipes.TabIndex = 5;
            // 
            // ColumnPipes
            // 
            this.ColumnPipes.FillWeight = 50F;
            this.ColumnPipes.HeaderText = "Pipes";
            this.ColumnPipes.Name = "ColumnPipes";
            this.ColumnPipes.ReadOnly = true;
            this.ColumnPipes.Width = 50;
            // 
            // label89
            // 
            this.label89.AutoSize = true;
            this.label89.Location = new System.Drawing.Point(479, 41);
            this.label89.Name = "label89";
            this.label89.Size = new System.Drawing.Size(48, 13);
            this.label89.TabIndex = 4;
            this.label89.Text = "Out Pipe";
            // 
            // label88
            // 
            this.label88.AutoSize = true;
            this.label88.Location = new System.Drawing.Point(392, 41);
            this.label88.Name = "label88";
            this.label88.Size = new System.Drawing.Size(40, 13);
            this.label88.TabIndex = 3;
            this.label88.Text = "In Pipe";
            // 
            // dataGridViewUSBDeviceList
            // 
            this.dataGridViewUSBDeviceList.AllowUserToAddRows = false;
            this.dataGridViewUSBDeviceList.AllowUserToDeleteRows = false;
            this.dataGridViewUSBDeviceList.AllowUserToResizeRows = false;
            this.dataGridViewUSBDeviceList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridViewUSBDeviceList.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ColumnDeviceDescription,
            this.ColumnProductID});
            this.dataGridViewUSBDeviceList.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.dataGridViewUSBDeviceList.Location = new System.Drawing.Point(3, 3);
            this.dataGridViewUSBDeviceList.Name = "dataGridViewUSBDeviceList";
            this.dataGridViewUSBDeviceList.ReadOnly = true;
            this.dataGridViewUSBDeviceList.RowHeadersVisible = false;
            this.dataGridViewUSBDeviceList.RowTemplate.ReadOnly = true;
            this.dataGridViewUSBDeviceList.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.dataGridViewUSBDeviceList.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridViewUSBDeviceList.Size = new System.Drawing.Size(304, 153);
            this.dataGridViewUSBDeviceList.TabIndex = 0;
            this.dataGridViewUSBDeviceList.SelectionChanged += new System.EventHandler(this.dataGridViewUSBDeviceList_SelectionChanged);
            // 
            // ColumnDeviceDescription
            // 
            this.ColumnDeviceDescription.FillWeight = 225F;
            this.ColumnDeviceDescription.HeaderText = "Description";
            this.ColumnDeviceDescription.Name = "ColumnDeviceDescription";
            this.ColumnDeviceDescription.ReadOnly = true;
            this.ColumnDeviceDescription.Width = 225;
            // 
            // ColumnProductID
            // 
            this.ColumnProductID.FillWeight = 75F;
            this.ColumnProductID.HeaderText = "ProductID";
            this.ColumnProductID.Name = "ColumnProductID";
            this.ColumnProductID.ReadOnly = true;
            this.ColumnProductID.Width = 75;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(734, 712);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.tabControlProtocol);
            this.Controls.Add(this.textBoxRegarding);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.checkBoxMD5Checksum);
            this.Controls.Add(this.tabControlContent);
            this.Controls.Add(this.textBoxProtocolVersion);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.checkBoxAckAlways);
            this.Controls.Add(this.labelOBPStatus);
            this.Controls.Add(this.labelIOStatus);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.buttonSyncFW);
            this.Controls.Add(this.buttonTest);
            this.Name = "MainForm";
            this.Text = "MainForm";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.tabControlContent.ResumeLayout(false);
            this.tabPageIOLog.ResumeLayout(false);
            this.tabPageIOLog.PerformLayout();
            this.tabPageInfo.ResumeLayout(false);
            this.tabPageInfo.PerformLayout();
            this.tabPageAcquisitionParams.ResumeLayout(false);
            this.tabPageAcquisitionParams.PerformLayout();
            this.tabPageSpectrum.ResumeLayout(false);
            this.tabControlSpectrumInfo.ResumeLayout(false);
            this.tabPageChart.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.chartSpectrum)).EndInit();
            this.tabPageData.ResumeLayout(false);
            this.tabPageData.PerformLayout();
            this.groupBoxMetadata.ResumeLayout(false);
            this.groupBoxMetadata.PerformLayout();
            this.tabPageBuffer.ResumeLayout(false);
            this.tabPageBuffer.PerformLayout();
            this.tabPageCoefficients.ResumeLayout(false);
            this.tabPageCoefficients.PerformLayout();
            this.tabPageTecTemp.ResumeLayout(false);
            this.tabPageTecTemp.PerformLayout();
            this.groupBoxTec.ResumeLayout(false);
            this.groupBoxTec.PerformLayout();
            this.tabPageGPIO.ResumeLayout(false);
            this.tabPageGPIO.PerformLayout();
            this.tabPageI2C.ResumeLayout(false);
            this.tabPageI2C.PerformLayout();
            this.tabControlProtocol.ResumeLayout(false);
            this.tabPageRS232.ResumeLayout(false);
            this.tabPageRS232.PerformLayout();
            this.tabPageTCPIP.ResumeLayout(false);
            this.tabPageTCPIP.PerformLayout();
            this.tabPageWinUSBNet.ResumeLayout(false);
            this.tabPageWinUSBNet.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewOutPipes)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewInPipes)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewUSBDeviceList)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBoxComPort;
        private System.Windows.Forms.ComboBox comboBoxDataBits;
        private System.Windows.Forms.ComboBox comboBoxParity;
        private System.Windows.Forms.ComboBox comboBoxStopBits;
        private System.Windows.Forms.ComboBox comboBoxHandshaking;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox comboBoxBaudRate;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button buttonTest;
        private System.Windows.Forms.Button buttonSyncFW;
        private System.Windows.Forms.Label labelPortName;
        private System.Windows.Forms.Label labelBaudRate;
        private System.Windows.Forms.Label labelDataBits;
        private System.Windows.Forms.Label labelParity;
        private System.Windows.Forms.Label labelStopBits;
        private System.Windows.Forms.Label labelHandshaking;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label labelIOStatus;
        private System.Windows.Forms.Label labelOBPStatus;
        private System.Windows.Forms.CheckBox checkBoxAckAlways;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBoxProtocolVersion;
        private System.Windows.Forms.TabControl tabControlContent;
        private System.Windows.Forms.TabPage tabPageIOLog;
        private System.Windows.Forms.TabPage tabPageInfo;
        private System.Windows.Forms.CheckBox checkBoxMD5Checksum;
        private System.Windows.Forms.TextBox textBoxRegarding;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Button buttonClearLog;
        private System.Windows.Forms.TextBox textBoxIOLog;
        private System.Windows.Forms.TabControl tabControlProtocol;
        private System.Windows.Forms.TabPage tabPageRS232;
        private System.Windows.Forms.TabPage tabPageTCPIP;
        private System.Windows.Forms.TextBox textBoxPortNum;
        private System.Windows.Forms.TextBox textBoxIPAddress;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label labelIPAddress;
        private System.Windows.Forms.Label labelPortNum;
        private System.Windows.Forms.Button buttonDisconnect;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox textBoxFwRevision;
        private System.Windows.Forms.TextBox textBoxHwRevision;
        private System.Windows.Forms.Button buttonReadFWRevision;
        private System.Windows.Forms.TextBox textBoxSerialNum;
        private System.Windows.Forms.Button buttonReadHWRevision;
        private System.Windows.Forms.Button buttonReadSerialNum;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.ComboBox comboBoxUserStringNum;
        private System.Windows.Forms.TextBox textBoxUserStringLen;
        private System.Windows.Forms.TextBox textBoxNumUserStrings;
        private System.Windows.Forms.TextBox textBoxUserString;
        private System.Windows.Forms.Button buttonWriteUserString;
        private System.Windows.Forms.Button buttonReadUserString;
        private System.Windows.Forms.Button buttonReadUserStringLen;
        private System.Windows.Forms.Button buttonReadNumUserStrings;
        private System.Windows.Forms.TextBox textBoxDeviceAlias;
        private System.Windows.Forms.Button buttonWriteDeviceAlias;
        private System.Windows.Forms.Button buttonReadDeviceAlias;
        private System.Windows.Forms.TextBox textBoxDeviceAliasLen;
        private System.Windows.Forms.Button buttonReadAliasLen;
        private System.Windows.Forms.TextBox textBoxFPGARevision;
        private System.Windows.Forms.Button buttonReadFPGARevision;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.TextBox textBoxSerialNumLen;
        private System.Windows.Forms.Button buttonReadSerialNumLen;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.TextBox textBoxStatusLED;
        private System.Windows.Forms.Button buttonWriteStatusLED;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Button buttonSaveRS232Settings;
        private System.Windows.Forms.Button buttonResetDefaults;
        private System.Windows.Forms.Button buttonResetSpectrometer;
        private System.Windows.Forms.TextBox textBoxDetectorSerialNum;
        private System.Windows.Forms.Button buttonReadDetectorSerialNum;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.TextBox textBoxCoating;
        private System.Windows.Forms.Button buttonReadCoating;
        private System.Windows.Forms.Label label32;
        private System.Windows.Forms.TextBox textBoxFilter;
        private System.Windows.Forms.Button buttonReadFilter;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.TextBox textBoxGrating;
        private System.Windows.Forms.Button buttonReadGrating;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.TextBox textBoxFiberDiameter;
        private System.Windows.Forms.Button buttonReadFiberDiameter;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.TextBox textBoxSlitWidth;
        private System.Windows.Forms.Button buttonReadSlitWidth;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.TextBox textBoxBenchSerialNum;
        private System.Windows.Forms.Button buttonReadBenchSerialNum;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.TextBox textBoxBenchId;
        private System.Windows.Forms.Button buttonReadBenchId;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.TabPage tabPageAcquisitionParams;
        private System.Windows.Forms.TabPage tabPageSpectrum;
        private System.Windows.Forms.TextBox textBoxAcquisitionDelayIncrement;
        private System.Windows.Forms.Button buttonReadAcquisitionDelayIncrement;
        private System.Windows.Forms.Label label41;
        private System.Windows.Forms.TextBox textBoxMinAcquisitionDelay;
        private System.Windows.Forms.TextBox textBoxMaxAcquisitionDelay;
        private System.Windows.Forms.Button buttonReadMinAcquisitionDelay;
        private System.Windows.Forms.Button buttonReadMaxAcquisitionDelay;
        private System.Windows.Forms.Label label42;
        private System.Windows.Forms.Label label43;
        private System.Windows.Forms.TextBox textBoxAcquisitionDelay;
        private System.Windows.Forms.Button buttonWriteAcquisitionDelay;
        private System.Windows.Forms.Button buttonReadAcquisitionDelay;
        private System.Windows.Forms.Label label40;
        private System.Windows.Forms.TextBox textBoxTriggerMode;
        private System.Windows.Forms.Button buttonWriteTriggerMode;
        private System.Windows.Forms.Button buttonReadTriggerMode;
        private System.Windows.Forms.Label label39;
        private System.Windows.Forms.TextBox textBoxLampEnable;
        private System.Windows.Forms.Button buttonWriteLampEnable;
        private System.Windows.Forms.Button buttonReadLampEnable;
        private System.Windows.Forms.Label label38;
        private System.Windows.Forms.TextBox textBoxIntegTimeIncrement;
        private System.Windows.Forms.Button buttonReadIntegTimeIncrement;
        private System.Windows.Forms.Label label37;
        private System.Windows.Forms.TextBox textBoxMinIntegTime;
        private System.Windows.Forms.TextBox textBoxMaxIntegTime;
        private System.Windows.Forms.Button buttonReadMinIntegTime;
        private System.Windows.Forms.Button buttonReadMaxIntegTime;
        private System.Windows.Forms.TextBox textBoxIntegTime;
        private System.Windows.Forms.Button buttonWriteIntegTime;
        private System.Windows.Forms.Button buttonReadIntegTime;
        private System.Windows.Forms.Label label34;
        private System.Windows.Forms.Label label35;
        private System.Windows.Forms.Label label36;
        private System.Windows.Forms.TabPage tabPageCoefficients;
        private System.Windows.Forms.Label label49;
        private System.Windows.Forms.Button buttonWritePartialSpectrumMode;
        private System.Windows.Forms.Button buttonReadPartialSpectrumMode;
        private System.Windows.Forms.TextBox textBoxPartialSpectrumMode;
        private System.Windows.Forms.Button buttonSimTriggerPulse;
        private System.Windows.Forms.TextBox textBoxMaxBinningFactor;
        private System.Windows.Forms.Button buttonReadMaxBinningFactor;
        private System.Windows.Forms.Label label48;
        private System.Windows.Forms.TextBox textBoxDefaultBinningFactor;
        private System.Windows.Forms.Button buttonWriteDefaultBinningFactor;
        private System.Windows.Forms.Button buttonReadDefaultBinningFactor;
        private System.Windows.Forms.Label label47;
        private System.Windows.Forms.TextBox textBoxBinningFactor;
        private System.Windows.Forms.Button buttonWriteBinningFactor;
        private System.Windows.Forms.Button buttonReadBinningFactor;
        private System.Windows.Forms.Label label46;
        private System.Windows.Forms.TextBox textBoxBoxcarWidth;
        private System.Windows.Forms.Button buttonWriteBoxcarWidth;
        private System.Windows.Forms.Button buttonReadBoxcarWidth;
        private System.Windows.Forms.Label label45;
        private System.Windows.Forms.TextBox textBoxScansToAvg;
        private System.Windows.Forms.Button buttonWriteScansToAvg;
        private System.Windows.Forms.Button buttonReadScansToAvg;
        private System.Windows.Forms.Label label44;
        private System.Windows.Forms.Button buttonReadPartialCorrectedSpectrum;
        private System.Windows.Forms.Button buttonReadRawSpectrum;
        private System.Windows.Forms.Button buttonReadCorrectedSpectrum;
        private System.Windows.Forms.CheckBox checkBoxDefaultBinningFactor;
        private System.Windows.Forms.ComboBox comboBoxNonLinearityCoeffIndex;
        private System.Windows.Forms.TextBox textBoxNumNonLinearityCoeffs;
        private System.Windows.Forms.TextBox textBoxNonLinearityCoeff;
        private System.Windows.Forms.Button buttonWriteNonLinearityCoeff;
        private System.Windows.Forms.Button buttonReadNonLinearityCoeff;
        private System.Windows.Forms.Button buttonReadNumNonLinearityCoeffs;
        private System.Windows.Forms.Label label50;
        private System.Windows.Forms.Label label51;
        private System.Windows.Forms.ComboBox comboBoxStrayLightCoeffIndex;
        private System.Windows.Forms.TextBox textBoxNumStrayLightCoeffs;
        private System.Windows.Forms.TextBox textBoxStrayLightCoeff;
        private System.Windows.Forms.Button buttonWriteStrayLightCoeff;
        private System.Windows.Forms.Button buttonReadStrayLightCoeff;
        private System.Windows.Forms.Button buttonReadNumStrayLightCoeffs;
        private System.Windows.Forms.Label label54;
        private System.Windows.Forms.Label label55;
        private System.Windows.Forms.ComboBox comboBoxWavecalCoeffIndex;
        private System.Windows.Forms.TextBox textBoxNumWavecalCoeffs;
        private System.Windows.Forms.TextBox textBoxWavecalCoeff;
        private System.Windows.Forms.Button buttonWriteWavecalCoeff;
        private System.Windows.Forms.Button buttonReadWavecalCoeff;
        private System.Windows.Forms.Button buttonReadNumWavecalCoeffs;
        private System.Windows.Forms.Label label52;
        private System.Windows.Forms.Label label53;
        private System.Windows.Forms.TabControl tabControlSpectrumInfo;
        private System.Windows.Forms.TabPage tabPageChart;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartSpectrum;
        private System.Windows.Forms.TabPage tabPageBuffer;
        private System.Windows.Forms.TabPage tabPageData;
        private System.Windows.Forms.Button buttonReadBufferedSpectrum;
        private System.Windows.Forms.Label labelMaxValuePixelNum;
        private System.Windows.Forms.TextBox textBoxMaxSpectrumValue;
        private System.Windows.Forms.Label label61;
        private System.Windows.Forms.TextBox textBoxNumPixelsReturned;
        private System.Windows.Forms.GroupBox groupBoxMetadata;
        private System.Windows.Forms.TextBox textBoxMetadataTriggerMode;
        private System.Windows.Forms.TextBox textBoxMetadataTimeUS;
        private System.Windows.Forms.TextBox textBoxMetadataTickCount;
        private System.Windows.Forms.TextBox textBoxMetadataSpectrumCount;
        private System.Windows.Forms.Label label60;
        private System.Windows.Forms.Label label59;
        private System.Windows.Forms.Label label58;
        private System.Windows.Forms.Label label57;
        private System.Windows.Forms.Label label56;
        private System.Windows.Forms.TextBox textBoxSpectrumData;
        private System.Windows.Forms.TextBox textBoxMaxValuePixelNum;
        private System.Windows.Forms.TextBox textBoxRemoveOldestSpectra;
        private System.Windows.Forms.Button buttonRemoveOldestSpectra;
        private System.Windows.Forms.Label label65;
        private System.Windows.Forms.TextBox textBoxNumSpectraInBuffer;
        private System.Windows.Forms.Button buttonReadNumSpectraInBuffer;
        private System.Windows.Forms.Label label64;
        private System.Windows.Forms.TextBox textBoxBufferSize;
        private System.Windows.Forms.Button buttonWriteBufferSize;
        private System.Windows.Forms.Button buttonReadBufferSize;
        private System.Windows.Forms.Label label63;
        private System.Windows.Forms.TextBox textBoxMaxBufferSize;
        private System.Windows.Forms.Button buttonReadMaxBufferSize;
        private System.Windows.Forms.Label label62;
        private System.Windows.Forms.Button buttonAcquireSpectraIntoBuffer;
        private System.Windows.Forms.Button buttonClearBufferedSpectra;
        private System.Windows.Forms.Button buttonAbortAcquisition;
        private System.Windows.Forms.TextBox textBoxIsIdle;
        private System.Windows.Forms.Button buttonReadIsIdle;
        private System.Windows.Forms.Label label66;
        private System.Windows.Forms.TabPage tabPageTecTemp;
        private System.Windows.Forms.ComboBox comboBoxTempSensorIndex;
        private System.Windows.Forms.TextBox textBoxNumTempSensors;
        private System.Windows.Forms.TextBox textBoxTempSensorValue;
        private System.Windows.Forms.Button buttonReadTempSensor;
        private System.Windows.Forms.Button buttonReadNumTempSensors;
        private System.Windows.Forms.Label label67;
        private System.Windows.Forms.Label label68;
        private System.Windows.Forms.Label label69;
        private System.Windows.Forms.Button buttonReadAllTempSensors;
        private System.Windows.Forms.TextBox textBoxAllTempSensors;
        private System.Windows.Forms.TextBox textBoxTecTemperature;
        private System.Windows.Forms.Button buttonReadTecTemperature;
        private System.Windows.Forms.Label label73;
        private System.Windows.Forms.TextBox textBoxTecStable;
        private System.Windows.Forms.Button buttonReadTecStable;
        private System.Windows.Forms.Label label72;
        private System.Windows.Forms.TextBox textBoxTecSetpoint;
        private System.Windows.Forms.Button buttonWriteTecSetpoint;
        private System.Windows.Forms.Button buttonReadTecSetpoint;
        private System.Windows.Forms.Label label71;
        private System.Windows.Forms.TextBox textBoxTecEnable;
        private System.Windows.Forms.Button buttonWriteTecEnable;
        private System.Windows.Forms.Button buttonReadTecEnable;
        private System.Windows.Forms.Label label70;
        private System.Windows.Forms.GroupBox groupBoxTec;
        private System.Windows.Forms.TabPage tabPageGPIO;
        private System.Windows.Forms.Label label78;
        private System.Windows.Forms.TextBox textBoxGPIOOutputEnableMask;
        private System.Windows.Forms.Label label77;
        private System.Windows.Forms.TextBox textBoxGPIOValueMask;
        private System.Windows.Forms.TextBox textBoxGPIOValue;
        private System.Windows.Forms.Button buttonWriteGPIOValue;
        private System.Windows.Forms.Button buttonReadGPIOValue;
        private System.Windows.Forms.Label label76;
        private System.Windows.Forms.TextBox textBoxGPIOOutputEnable;
        private System.Windows.Forms.Button buttonWriteGPIOOutputEnable;
        private System.Windows.Forms.Button buttonReadGPIOOutputEnable;
        private System.Windows.Forms.Label label75;
        private System.Windows.Forms.TextBox textBoxNumGPIOPins;
        private System.Windows.Forms.Button buttonReadNumGPIOPins;
        private System.Windows.Forms.Label label74;
        private System.Windows.Forms.TabPage tabPageI2C;
        private System.Windows.Forms.TextBox textBoxI2CNumBuses;
        private System.Windows.Forms.Button buttonReadI2CNumBuses;
        private System.Windows.Forms.Label label80;
        private System.Windows.Forms.ComboBox comboBoxI2CDataBusNum;
        private System.Windows.Forms.TextBox textBoxI2CAddress;
        private System.Windows.Forms.Button buttonWriteI2CBytes;
        private System.Windows.Forms.Button buttonReadI2CBytes;
        private System.Windows.Forms.Label label79;
        private System.Windows.Forms.Label label87;
        private System.Windows.Forms.Label label86;
        private System.Windows.Forms.Label label85;
        private System.Windows.Forms.ComboBox comboBoxI2CClockBusNum;
        private System.Windows.Forms.TextBox textBoxI2CClockLimit;
        private System.Windows.Forms.Button buttonWriteI2CClockLimit;
        private System.Windows.Forms.Label label84;
        private System.Windows.Forms.TextBox textBoxI2CData;
        private System.Windows.Forms.Label label83;
        private System.Windows.Forms.TextBox textBoxI2CNumBytes;
        private System.Windows.Forms.Label label82;
        private System.Windows.Forms.Label label81;
        private System.Windows.Forms.TabPage tabPageWinUSBNet;
        private System.Windows.Forms.DataGridView dataGridViewUSBDeviceList;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnDeviceDescription;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnProductID;
        private System.Windows.Forms.Label label89;
        private System.Windows.Forms.Label label88;
        private System.Windows.Forms.DataGridView dataGridViewOutPipes;
        private System.Windows.Forms.DataGridView dataGridViewInPipes;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn1;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnPipes;
    }
}

