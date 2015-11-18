[README.TXT]

This is a simple Windows executable which merges and optionally interpolates 
column-based data (presumably spectra).  The idea is to merge a bunch of 
individual spectra into one "wide" CSV for easier charting and analysis in
Excel.  

By default, both the X-axis and Y-axis are from the FIRST file loaded, and then 
ONLY the Y-axis is merged in from subsequent files.

This is essentially a "horizontal concatenation," and Linux users will recognize 
it as something which is trivial to do with join(1), but sometimes tricky on
Windows (Cygwin is a hefty install).

You can also interpolate the X-axis (handy when comparing spectra in wavelength
or wavenumber space across different units, since pixel space will typically not
align).  Finally, peak-finding is available through an extract from SPAM 
(SpamSharp.dll in ./dist).

Assumptions:

- assumes that input files are in X, Y format and either comma- or tab-delimited
- ignores lines that don't start with a digit or sign character

History:

1.0.8 - added Gaussian peak fit using Microsoft Solver Foundation
        (https://msdn.microsoft.com/en-us/devlabs/hh145003)

