using System;
using System.Collections.Generic;
using System.Text;

namespace MergeSpectra
{
    class Interpolator
    {
        SortedDictionary<double, double> spectrum;
        List<double> wavelengths;
        double firstWavelength;
        double lastWavelength;
        int pos = 0; // where the interpolation state currently sits in 'wavelengths' (lower bound)
        int pixels = 0;
        Logger logger = Logger.getInstance();

        public Interpolator(SortedDictionary<double, double> spectrumIn)
        {
            spectrum = spectrumIn;

            wavelengths = new List<double>(spectrum.Keys);
            wavelengths.Sort();
            pixels = wavelengths.Count;

            firstWavelength = wavelengths[0];
            lastWavelength = wavelengths[pixels - 1];
        }

        // NOTE: "sequential" means that it should be called with INCREASING wavelength values, which
        // allows faster processing; random-access interpolation should be achieved through 
        // interpolateRandom() (not currently implemented)
        public double interpolateSequential(double wavelength)
        {
            // handle corner-cases fast
            if (wavelength <= firstWavelength)
                return spectrum[firstWavelength];
            if (wavelength >= lastWavelength)
                return spectrum[lastWavelength];
            if (spectrum.ContainsKey(wavelength))
                return spectrum[wavelength];

            // this shouldn't happen if caller follows the "sequential" requirement
            if (wavelengths[pos] > wavelength)
            {
                // just reset and let the caller suffer the runtime penalty
                logger.log("WARNING: sequential calling error: wavelengths[{0}] ({1:f2}) > wavelength ({3:f2})", pos, wavelengths[pos], wavelength);
                pos = 0;
            }

            // pos is an index SOMEWHERE below the requested wavelength
            // advance until it is the index DIRECTLY below the requested wavelength
            while (pos < pixels - 2)
            {
                if (wavelengths[pos + 1] > wavelength)
                    break;
                pos++;
            }

            // pos should now be directly below requested wavelength

            // lookup bracketing wavelength and intensity values
            double wl0 = wavelengths[pos];
            double wl1 = wavelengths[pos + 1];
        
            double a0 = spectrum[wl0];
            double a1 = spectrum[wl1];
        
            // interpolate
            return (((wavelength - wl0) / (wl1 - wl0)) * (a1 - a0)) + a0;
        }
    }
}
