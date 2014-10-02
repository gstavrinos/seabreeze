using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// simplifies switching between SeaBreeze and OmniDriver
interface Spectrometer
{
    bool open();
    bool close();

    void setLogger(Logger log);
    bool isOk();

    int      getPixelCount();
    bool     setIntegrationTimeMilliseconds(int ms);
    double[] getWavelengths();
    double[] getSpectrum();
    double[] getIrradianceCalibration();
    double   getCollectionArea();
    string   getVersion();
    string   getSerialNumber();
    string   getSpectrometerType();
    int[]    getEDCIndices();
    double[] getNLCCoeffs();
}
