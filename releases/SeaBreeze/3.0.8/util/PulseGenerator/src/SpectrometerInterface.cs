using System;
using System.Collections.Generic;
using System.Text;

namespace PulseGenerator
{
    interface SpectrometerInterface
    {
        bool init();
        bool getGPIO(int pin);
        bool setGPIO(int pin, bool flag);
    }
}
