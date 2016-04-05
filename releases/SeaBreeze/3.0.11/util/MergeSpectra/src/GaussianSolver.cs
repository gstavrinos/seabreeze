using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.SolverFoundation.Common;
using Microsoft.SolverFoundation.Solvers;

namespace MergeSpectra
{
    // Gaussian function definition from https://en.wikipedia.org/wiki/Gaussian_function:
    //
    //   f(x) = a * exp(-1 * (x - b)^2 / (2 * c^2)) + d
    //
    // where standard Gaussian variable names roughly map to the following "spectral peak" equivalents:
    //
    //   a = coeff[0] = peakIntensity
    //   b = coeff[1] = peak centroid wavelength
    //   c = coeff[2] = approximate peak width in pixels (non-zero)
    //   d = coeff[3] = baseline
    //
    class GaussianSolver
    {
        ////////////////////////////////////////////////////////////////////////
        // Data Types
        ////////////////////////////////////////////////////////////////////////

        public class Params
        {
            public double intensity;
            public double wavelength;
            public double width;
            public double baseline;

            public List<double> xvalues;
            public List<double> yvalues;

            // ctor
            public Params(double i, double wl, double w, double b)
            {
                intensity = i;
                wavelength = wl;
                width = w;
                baseline = b;

                xvalues = new List<double>();
                yvalues = new List<double>();
            }

            // copy-ctor
            public Params(Params rhs)
            {
                intensity = rhs.intensity;
                wavelength = rhs.wavelength;
                width = rhs.width;
                baseline = rhs.baseline;

                xvalues = new List<double>(rhs.xvalues);
                yvalues = new List<double>(rhs.yvalues);
            }

            // populate yvalues
            public void computeGaussian()
            {
                for (int i = 0; i < xvalues.Count; i++)
                    yvalues[i] = intensity * Math.Exp(-1 * (xvalues[i] - wavelength) * (xvalues[i] - wavelength) / (2 * width * width)) + baseline; 
            }
        };

        ////////////////////////////////////////////////////////////////////////
        // Attributes
        ////////////////////////////////////////////////////////////////////////

        public Params initialParams;
        public Params optimalParams;

        Logger logger = Logger.getInstance();

        ////////////////////////////////////////////////////////////////////////
        // Ctor
        ////////////////////////////////////////////////////////////////////////

        public GaussianSolver(double intensity, double wavelength, double width, double baseline)
        {
            initialParams = new Params(intensity, wavelength, width, baseline);
        }

        ////////////////////////////////////////////////////////////////////////
        // Methods
        ////////////////////////////////////////////////////////////////////////

        public void addPoint(double x, double y)
        {
            initialParams.xvalues.Add(x);
            initialParams.yvalues.Add(y);
        }

        public double computeFWHM()
        {
            // these all need to be arrays, because that's the way the NelderMeadSolver works
            double[] coeffs = new double[] 
            {   initialParams.intensity, 
                initialParams.wavelength, 
                initialParams.width, 
                initialParams.baseline 
            };

            ////////////////////////////////////////////////////////////////////
            // Bound the search
            ////////////////////////////////////////////////////////////////////

            // TODO: add a method to override these from the caller
            // TODO: find a way to "optimize" these based on initial conditions
            //       (an optimizer on the optimizer...yes I see the irony)
            //
            // (note we call the x-axis "wavelength," but sometimes it may be wavenumber or just pixel)
            //                              int     wl width    base
            double[] min = new double[] {     0, -1000,  0.1,      0 };
            double[] max = new double[] { 65535,  3500,   50,  65535 };

            // consider bounding things a bit closer than that, based on initial conditions:

            // min[0] = coeffs[0] * 0.5;   // intensity should be +/- 50% initial guess
            // max[0] = coeffs[0] * 1.5;
    
            // min[1] = coeffs[1] - 5 ;    // wavelength shouldn't vary by more than 20,
            // max[1] = coeffs[1] + 5 ;    // whether wavelength, wavenumber or pixel
    
            // max[3] = coeffs[0];         // baseline certainly shouldn't exceed peak height

            ////////////////////////////////////////////////////////////////////
            // run solver
            ////////////////////////////////////////////////////////////////////

            var solution = NelderMeadSolver.Solve(x => computeGaussianFitQuality(x[0], x[1], x[2], x[3]), coeffs, min, max);

            ////////////////////////////////////////////////////////////////////
            // post-process results
            ////////////////////////////////////////////////////////////////////

            optimalParams = new Params(initialParams);
            optimalParams.intensity  = solution.GetValue(1); // 1-indexed
            optimalParams.wavelength = solution.GetValue(2);
            optimalParams.width      = solution.GetValue(3);
            optimalParams.baseline   = solution.GetValue(4);
            optimalParams.computeGaussian();

            // assuming width has been properly optimized now
            double fwhm = 2.0 * Math.Sqrt((2 * Math.Log(2))) * optimalParams.width;

            // generate report
            logger.log("");
            logger.log("NelderMeadSolver:");
            logger.log("  Initial: intensity {0:f2}, wavelength {1:f2}, width {2:f2}, baseline {3:f2}", initialParams.intensity, initialParams.wavelength, initialParams.width, initialParams.baseline);
            logger.log("  Optimal: intensity {0:f2}, wavelength {1:f2}, width {2:f2}, baseline {3:f2}", optimalParams.intensity, optimalParams.wavelength, optimalParams.width, optimalParams.baseline);
            for (int i = 0; i < initialParams.xvalues.Count; i++)
                logger.log("  {0}, {1:f2}, {2:f2}, {3:f2}{4}", 
                    i, 
                    initialParams.xvalues[i], 
                    initialParams.yvalues[i], 
                    optimalParams.yvalues[i], 
                    i == ((initialParams.xvalues.Count - 1) / 2) ? ", peak" : "");

            return fwhm;
        }

        // returns positive indication of fitness, zero is best, higher is worse
        double computeGaussianFitQuality(double intensity, double wavelength, double width, double baseline)
        {
            double sumOfSquaredDeltas = 0;

            // make copy of initial params (holds x-axis)
            Params p = new Params(initialParams);

            // overwrite with this iteration's variables
            p.intensity = intensity;
            p.wavelength = wavelength;
            p.width = width;
            p.baseline = baseline;

            // generate the new curve
            p.computeGaussian();

            // calculate fitness
            for (int i = 0; i < initialParams.xvalues.Count; i++)
            {
                double delta = initialParams.yvalues[i] - p.yvalues[i];
                sumOfSquaredDeltas += delta * delta;
            }

            return sumOfSquaredDeltas;
        }
    }
}
