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

        // We construct an Interpolator from a passed spectrum, then pre-compute
        // anything we might want to re-use on each call to interpolate().
        // Note that "pos" (position within the spectrum, presumed to be sequentially
        // keyed in increasing order) is initialized to zero.
        public Interpolator(SortedDictionary<double, double> spectrumIn)
        {
            spectrum = spectrumIn;

            wavelengths = new List<double>(spectrum.Keys);
            wavelengths.Sort();     // just being thorough
            pixels = wavelengths.Count;

            firstWavelength = wavelengths[0];
            lastWavelength = wavelengths[pixels - 1];
        }

        // NOTE: this method will execute much more efficiently if called with 
        // sequentially increasing wavelength values (though it should still work
        // if used as "random access" into the spectrum, just more slowly)
        public double interpolate(double wavelength)
        {
            // handle corner-cases fast (leave 'pos' where it is) - no misleadingly cute extrapolation, 
            // just treat first and last values as infinite series
            if (wavelength <= firstWavelength)    return spectrum[firstWavelength]; 
            if (wavelength >= lastWavelength)     return spectrum[lastWavelength]; 
            if (spectrum.ContainsKey(wavelength)) return spectrum[wavelength];

            // this shouldn't happen if caller follows the "sequential" guidance (i.e.
            // each subsequent method call is invoked with an increasing wavelength value)
            if (wavelengths[pos] > wavelength)
                pos = 0;

            // pos should now be an index SOMEWHERE BELOW the requested wavelength;
            // advance until it is the index DIRECTLY BELOW the requested wavelength
            while (pos < pixels - 2)
            {
                if (wavelengths[pos + 1] > wavelength)
                    break;
                pos++;
            }

            // pos should now be DIRECTLY BELOW requested wavelength (don't worry
            // about running off the end...handled by initial corner-case)

            // lookup bracketing wavelengths
            double wl0 = wavelengths[pos];
            double wl1 = wavelengths[pos + 1];
        
            // lookup associated bracketing intensity values
            double a0 = spectrum[wl0];
            double a1 = spectrum[wl1];
        
            // perform linear interpolation between those two points
            return (((wavelength - wl0) / (wl1 - wl0)) * (a1 - a0)) + a0;
        }
    }
}
