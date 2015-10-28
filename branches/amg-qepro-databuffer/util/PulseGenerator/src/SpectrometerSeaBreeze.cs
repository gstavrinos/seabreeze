using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace PulseGenerator
{
    class SpectrometerSeaBreeze : SpectrometerInterface
    {
        ////////////////////////////////////////////////////////////////////////
        // Constants
        ////////////////////////////////////////////////////////////////////////

        const byte USB_TX_ENDPOINT         = 0x01;
        const byte USB_RX_ENDPOINT         = 0x81;

        int specIndex = -1;

        const int GPIO_DIRECTION_READ = 0;
        const int GPIO_DIRECTION_WRITE = 1;
        const int GPIO_MUX_NORMAL = 0;
        const int GPIO_MUX_ALT = 1;

        ////////////////////////////////////////////////////////////////////////
        // Singleton
        ////////////////////////////////////////////////////////////////////////

        static SpectrometerSeaBreeze instance = null;
        static Mutex mut = new Mutex();

        ////////////////////////////////////////////////////////////////////////
        // Attributes
        ////////////////////////////////////////////////////////////////////////

        FeatureControllerGPIO gpio;

        Logger logger = Logger.getInstance();

        int numberOfSpectrometers = 0;
        string serialNumber;
        string modelName;
        int pixels;
        double[] wavelengths;

        ////////////////////////////////////////////////////////////////////////
        // Lifecyle
        ////////////////////////////////////////////////////////////////////////

        public static SpectrometerSeaBreeze getInstance()
        {
            mut.WaitOne();
            if (instance == null)
                instance = new SpectrometerSeaBreeze();
            mut.ReleaseMutex();
            return instance;
        }

        SpectrometerSeaBreeze()
        {
        }

        public bool init()
        {
            int error = 0;
            int openErr = SeaBreezeWrapper.seabreeze_open_spectrometer(0, ref error);
            if (openErr != 0)
            {
                logger.queue("ERROR: SeaBreeze could not open first spectrometer");
                return false;
            }

            Thread.Sleep(100); // Testing...

            numberOfSpectrometers = 1;
            specIndex = 0;

            logger.queue("Found {0} spectrometers", numberOfSpectrometers);
            if (numberOfSpectrometers < 1)
            {
                logger.queue("ERROR: no spectrometers found");
                return false;
            }
            if (numberOfSpectrometers > 1)
                logger.queue("WARNING: defaulting to first-detected spectrometer");

            ////////////////////////////////////////////////////////////////////
            // initialize metadata for each spectrometer
            ////////////////////////////////////////////////////////////////////

            pixels = getPixels();
            wavelengths = readWavelengths();
            serialNumber = getSerialNumber();
            modelName = getModel();

            ////////////////////////////////////////////////////////////////////
            // init GPIO
            ////////////////////////////////////////////////////////////////////

            gpio = new FeatureControllerGPIO();
            gpio.init();
            gpio.setCacheEnabled(true);

            ////////////////////////////////////////////////////////////////////
            // finished
            ////////////////////////////////////////////////////////////////////

            logger.queue("Spectrometer at index {0} is a {1} with serial {2} and {3} pixels from {4:0.00} to {5:0.00}nm",
                specIndex, modelName, serialNumber, pixels, wavelengths[0], wavelengths[wavelengths.Length - 1]);

            return true;
        }

        ////////////////////////////////////////////////////////////////////////
        // Methods
        ////////////////////////////////////////////////////////////////////////

        public double[] getWavelengths()
        {
            return wavelengths;
        }

        string getSerialNumber()
        {
            int error = 0;

            if (specIndex < 0)
                return null;

            byte[] buf = new byte[16];

            bool ok = mut.WaitOne(2);
            if (!ok)
                return null;

            SeaBreezeWrapper.seabreeze_get_serial_number(specIndex, ref error, ref buf[0], buf.Length);
            mut.ReleaseMutex();

            if (error != 0)
            {
                logger.queue("error getting serial number (error = {0})", error);
                return null;
            }

            return byteToString(buf);
        }

        string getModel()
        {
            int error = 0;

            if (specIndex < 0)
                return null;

            byte[] buf = new byte[16];

            bool ok = mut.WaitOne(2);
            if (!ok)
                return null;

            SeaBreezeWrapper.seabreeze_get_model(specIndex, ref error, ref buf[0], buf.Length);
            mut.ReleaseMutex();

            if (error != 0)
            {
                logger.queue("error getting model (error = {0})", error);
                return null;
            }

            return byteToString(buf);
        }

        int getPixels()
        {
            int retval = 0;
            int error = 0;

            if (specIndex < 0)
                return retval;

            bool ok = mut.WaitOne(2);
            if (!ok)
                return 0;

            retval = SeaBreezeWrapper.seabreeze_get_formatted_spectrum_length(specIndex, ref error);
            mut.ReleaseMutex();

            if (error != 0)
            {
                logger.queue("error getting pixel count (error = {0})", error);
                return 0;
            }
            return retval;
        }

        double[] readWavelengths()
        {
            if (specIndex < 0)
                return null;

            double[] wl = new double[pixels];

            int error = 0;

            bool ok = mut.WaitOne(2);
            if (!ok)
                return null;

            SeaBreezeWrapper.seabreeze_get_wavelengths(specIndex, ref error, ref wl[0], pixels);
            mut.ReleaseMutex();

            if (error != 0)
            {
                logger.queue("Error calling readWavelengths(): errorCode = {0}", error);
                specIndex = -1;
                return null;
            }

            return wl;
        }

        // NOTE: these are zero-indexed GPIO bits, with low-level logic
        public bool getGPIO(int pin)
        {
            if (gpio == null)
                return false;

            bool ok = mut.WaitOne(2);
            if (!ok)
                return false;

            bool set = gpio.getValueBit(pin);
            mut.ReleaseMutex();

            return set;
        }

        public bool setGPIO(int pin, bool flag)
        {
            if (gpio == null)
                return false;

            bool ok = mut.WaitOne(5);
            if (!ok)
                return false;

            logger.log("[setGPIO] setting GPIO {0} {1}", pin, flag);
            ok = gpio.setValueBit(pin, flag ? 1 : 0);
            mut.ReleaseMutex();
            return ok;
        }

        // NO INTERNAL MUTEX! Caller must synchronize
        public bool writeUSB(byte[] data)
        {
            int error = 0;

            string debug = "";
            for (int i = 0; i < data.Length; i++)
                debug += String.Format(" 0x{0:x2}", data[i]);
            logger.log(">> {0}", debug);

            SeaBreezeWrapper.seabreeze_write_usb(specIndex, ref error, USB_TX_ENDPOINT, ref data[0], data.Length);
            return (error == 0);
        }

        // NO INTERNAL MUTEX! Caller must synchronize
        public bool readUSB(ref byte[] buffer)
        {
            int errorCode = 0;
            SeaBreezeWrapper.seabreeze_read_usb(specIndex, ref errorCode, USB_RX_ENDPOINT, ref buffer[0], buffer.Length);

            string debug = "";
            for (int i = 0; i < buffer.Length; i++)
                debug += String.Format(" 0x{0:x2}", buffer[i]);
            logger.log("<< {0}", debug);

            return (errorCode == 0);
        }

        string byteToString(byte[] buf)
        {
            int len = 0;
            while (buf[len] != 0 && len + 1 < buf.Length)
                len++;

            byte[] clean = Encoding.Convert(Encoding.GetEncoding("iso-8859-1"), Encoding.UTF8, buf);
            return Encoding.UTF8.GetString(clean, 0, len);
        }

    }

    ////////////////////////////////////////////////////////////////////////////
    //                                                                        //
    //                       FeatureControllerGPIO                            //
    //                                                                        //
    ////////////////////////////////////////////////////////////////////////////

    class FeatureControllerGPIO
    {
        const byte REGISTER_GPIO_MUX       = 0x48; // (0 use GPIO, 1 use alternate)
        const byte REGISTER_GPIO_DIRECTION = 0x50; // (0 is input, 1 is output)
        const byte REGISTER_GPIO_TRANSFER  = 0x54; // (reads or writes, depending on DIRECTION)

        const byte OP_WRITE_REGISTER       = 0x6A;
        const byte OP_READ_REGISTER        = 0x6B;

        Logger logger = Logger.getInstance();
        SpectrometerSeaBreeze wrapper = SpectrometerSeaBreeze.getInstance();

        public FeatureControllerGPIO() { }

        Mutex mut = new Mutex();

        bool cacheEnabled;
        uint cachedValue;

        public void setCacheEnabled(bool flag) 
        { 
            cacheEnabled = flag; 
        }

        bool readRegister(byte register, ref uint value)
        {
            bool ok = mut.WaitOne(2);
            if (!ok)
                return false;

            byte[] readRequest = new byte[2];

            readRequest[0] = OP_READ_REGISTER;
            readRequest[1] = register;

            ok = wrapper.writeUSB(readRequest);

            if (!ok)
            {
                logger.log("Error requesting read of GPIO register 0x{0:x4}", register);
                mut.ReleaseMutex();
                return false;
            }

            byte[] readResponse = new byte[3];
            ok = wrapper.readUSB(ref readResponse);
            mut.ReleaseMutex();

            if (!ok)
            {
                logger.log("Error reading GPIO register 0x{0:x4}", register);
                return false;
            }

            value = (uint)(readResponse[1] | (readResponse[2] << 8));
            logger.log("[GPIO] register 0x{0:x2} read  0x{1:x4}", register, value);
            return true;
        }

        bool writeRegister(byte register, uint value)
        {
            bool ok = mut.WaitOne(2);
            if (!ok)
                return false;

            byte[] request  = new byte[4];
            request[0] = OP_WRITE_REGISTER;
            request[1] = register;
            request[2] = (byte) (value & 0xff); // LSB
            request[3] = (byte) ((value >> 8) & 0xff); // MSB

            ok = wrapper.writeUSB(request);
            mut.ReleaseMutex();

            if (!ok)
            {
                logger.log("Error writing GPIO register 0x{0:x4} with value 0x{1:x4}", register, value);
                return false;
            }

            logger.log("[GPIO] register 0x{0:x2} wrote 0x{1:x4}", register, value);
            return true;
        }

        public bool init()
        {
            bool success = true;
            logger.log("Initializing GPIO");

            ////////////////////////////////////////////////////////////////////
            // set all GPIO pins to "GPIO" mode (not "alt")
            ////////////////////////////////////////////////////////////////////
            logger.log("setting GPIO pins 0-7 to GPIO mode");

            // read current MUX state
            uint value = 0;
            if (!readRegister(REGISTER_GPIO_MUX, ref value))
            {
                logger.queue("can't initialize GPIO (read MUX)");
                success = false;
            }

            // unset pins 0-7 (make all explicit control, none alt)
            value &= 0xff00;
            if (!writeRegister(REGISTER_GPIO_MUX, value))
            {
                logger.queue("can't initialize GPIO (write MUX)");
                success = false;
            }

            ////////////////////////////////////////////////////////////////////
            // set all GPIO pins to OUTPUT 
            ////////////////////////////////////////////////////////////////////
            logger.log("setting GPIO pins 0-7 to output");

            // read current DIRECTION state
            value = 0;
            if (!readRegister(REGISTER_GPIO_DIRECTION, ref value))
            {
                logger.queue("can't initialize GPIO (read DIRECTION)");
                success = false;
            }

            // set pins 0-7 to output
            value |= 0x00ff;
            if (!writeRegister(REGISTER_GPIO_DIRECTION, value))
            {
                logger.queue("can't initialize GPIO (write DIRECTION)");
                success = false;
            }

            ////////////////////////////////////////////////////////////////////
            // set all GPIO pins to OFF
            ////////////////////////////////////////////////////////////////////
            logger.log("setting GPIO pins 0-7 to low");

            // read current DIRECTION state
            value = 0;
            if (!readRegister(REGISTER_GPIO_TRANSFER, ref value))
            {
                logger.queue("can't initialize GPIO (read VALUE)");
                success = false;
            }

            // set pins 0-7 to low
            value &= 0xff00;
            if (!writeRegister(REGISTER_GPIO_TRANSFER, value))
            {
                logger.queue("can't initialize GPIO (write VALUE)");
                success = false;
            }

            ////////////////////////////////////////////////////////////////////
            // done
            ////////////////////////////////////////////////////////////////////

            logger.log("GPIO initialized");
            return success;
        }

        public bool getValueBit(int pin)
        {
            logger.log("[GPIO] reading pin {0}", pin);

            uint value = 0;
            if (!readRegister(REGISTER_GPIO_TRANSFER, ref value))
            {
                logger.log("[getValueBit] error reading transfer register");
                return false;
            }

            // create mask
            uint bit = (uint)(1 << pin);

            // read the bit
            return ((value & bit) != 0);
        }

        public bool setValueBit(int pin, int newValue)
        {
            logger.log("[GPIO] setting pin {0} to {1}", pin, newValue);

            uint value = 0;
            if (cacheEnabled)
                value = cachedValue;
            else
            {
                if (!readRegister(REGISTER_GPIO_TRANSFER, ref value))
                {
                    logger.log("[setValueBit] error reading transfer register");
                    return false;
                }
            }

            // mask in the new value
            uint bit = (uint) (1 << pin);
            if (newValue != 0)
            {
                // set the bit
                value |= bit;
            }
            else
            {
                // unset the bit
                uint mask = 0x3ff ^ bit;
                value &= mask;
            }

            if (!writeRegister(REGISTER_GPIO_TRANSFER, value))
            {
                logger.log("[setValueBit] error writing transfer register");
                return false;
            }

            return true;
        }
    }
}
