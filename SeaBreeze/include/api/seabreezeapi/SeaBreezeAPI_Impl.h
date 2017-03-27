/***************************************************//**
 * @file    SeaBreezeAPI_Impl.h
 * @date    February 2017
 * @author  Ocean Optics, Inc.
 *
 * This is an implementation of the SeaBreezeAPI interface.
 * This has been split off of SeaBreezeAPI.h so that the
 * #include tree for that file can be kept to a minimum.  This
 * is important for non-C languages to more easily integrate
 * with SeaBreeze.
 *
 * This provides a C interface to help with linkage.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2017, Ocean Optics Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *******************************************************/

#ifndef SEABREEZEAPI_IMPL_H
#define SEABREEZEAPI_IMPL_H

#include "api/seabreezeapi/SeaBreezeAPI.h"
#include "api/seabreezeapi/DeviceAdapter.h"

class SeaBreezeAPI_Impl : SeaBreezeAPI {
public:
    virtual ~SeaBreezeAPI_Impl();
    
    virtual int probeDevices();
    virtual int addTCPIPv4DeviceLocation(char *deviceTypeName, char *ipAddr, int port);
    virtual int addRS232DeviceLocation(char *deviceTypeName, char *deviceBusPath,
        unsigned int baud);

    virtual int getNumberOfDeviceIDs();
    virtual int getDeviceIDs(long *ids, unsigned long maxLength);
    virtual int openDevice(long id, int *errorCode);
    virtual void closeDevice(long id, int *errorCode);

    virtual int getDeviceType(long id, int *errorCode, char *buffer, unsigned int length);
    
    virtual unsigned char getDeviceEndpoint(long id, int *error_code, usbEndpointType endpointType);

    /* Get raw usb access capabilities */
    virtual int getNumberOfRawUSBBusAccessFeatures(long deviceID, int *errorCode);
    virtual int getRawUSBBusAccessFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual int rawUSBBusAccessRead(long deviceID, long featureID, int *errorCode, unsigned char *buffer, unsigned int bufferLength, unsigned char endpoint);
    virtual int rawUSBBusAccessWrite(long deviceID, long featureID, int *errorCode, unsigned char *buffer, unsigned int bufferLength, unsigned char endpoint);
    
    /* Serial number capabilities */
    virtual int getNumberOfSerialNumberFeatures(long deviceID, int *errorCode);
    virtual int getSerialNumberFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual int getSerialNumber(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);
    virtual unsigned char getSerialNumberMaximumLength(long deviceID, long featureID, int *errorCode);

    /* Spectrometer capabilities */
    virtual int getNumberOfSpectrometerFeatures(long id, int *errorCode);
    virtual int getSpectrometerFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual void spectrometerSetTriggerMode(long deviceID, long spectrometerFeatureID, int *errorCode, int mode);
    virtual void spectrometerSetIntegrationTimeMicros(long deviceID, long spectrometerFeatureID, int *errorCode, unsigned long integrationTimeMicros);
    virtual unsigned long spectrometerGetMinimumIntegrationTimeMicros(long deviceID, long spectrometerFeatureID, int *errorCode);
    virtual unsigned long spectrometerGetMaximumIntegrationTimeMicros(long deviceID, long spectrometerFeatureID, int *errorCode);
    virtual double spectrometerGetMaximumIntensity(long deviceID, long spectrometerFeatureID, int *errorCode);
    virtual int spectrometerGetUnformattedSpectrumLength(long deviceID, long spectrometerFeatureID, int *errorCode);
    virtual int spectrometerGetUnformattedSpectrum(long deviceID, long spectrometerFeatureID, int *errorCode, unsigned char *buffer, int bufferLength);
	virtual int spectrometerGetFastBufferSpectrum(long deviceID, long spectrometerFeatureID, int *errorCode, unsigned char *buffer, int bufferLength, unsigned int numberOfSamplesToRetrieve);
	virtual int spectrometerGetFormattedSpectrumLength(long deviceID, long spectrometerFeatureID, int *errorCode);
    virtual int spectrometerGetFormattedSpectrum(long deviceID, long spectrometerFeatureID, int *errorCode, double *buffer, int bufferLength);
    virtual int spectrometerGetWavelengths(long deviceID, long spectrometerFeatureID, int *errorCode, double *wavelengths, int length);
    virtual int spectrometerGetElectricDarkPixelCount(long deviceID, long spectrometerFeatureID, int *errorCode);
    virtual int spectrometerGetElectricDarkPixelIndices(long deviceID, long spectrometerFeatureID, int *errorCode, int *indices, int length);

    /* Pixel binning capabilities */
    virtual int getNumberOfPixelBinningFeatures(long id, int *errorCode);
    virtual int getPixelBinningFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual void binningSetPixelBinningFactor(long deviceID, long spectrometerFeatureID, int *errorCode, const unsigned char binningFactor);
    virtual unsigned char binningGetPixelBinningFactor(long deviceID, long spectrometerFeatureID, int *errorCode);
    virtual void binningSetDefaultPixelBinningFactor(long deviceID, long spectrometerFeatureID, int *errorCode, const unsigned char binningFactor);
    virtual void binningSetDefaultPixelBinningFactor(long deviceID, long spectrometerFeatureID, int *errorCode);
    virtual unsigned char binningGetDefaultPixelBinningFactor(long deviceID, long spectrometerFeatureID, int *errorCode);
    virtual unsigned char binningGetMaxPixelBinningFactor(long deviceID, long spectrometerFeatureID, int *errorCode);
        
    /* TEC capabilities */
    virtual int getNumberOfThermoElectricFeatures(long deviceID, int *errorCode);
    virtual int getThermoElectricFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual double tecReadTemperatureDegreesC(long deviceID, long featureID, int *errorCode);
    virtual void tecSetTemperatureSetpointDegreesC(long deviceID, long featureID, int *errorCode, double temperatureDegreesCelsius);
    virtual void tecSetEnable(long deviceID, long featureID, int *errorCode, unsigned char tecEnable);

    /* Irradiance calibration features */
    virtual int getNumberOfIrradCalFeatures(long deviceID, int *errorCode);
    virtual int getIrradCalFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual int irradCalibrationRead(long deviceID, long featureID, int *errorCode, float *buffer, int bufferLength);
    virtual int irradCalibrationWrite(long deviceID, long featureID, int *errorCode, float *buffer, int bufferLength);
    virtual int irradCalibrationHasCollectionArea(long deviceID, long featureID, int *errorCode);
    virtual float irradCalibrationReadCollectionArea(long deviceID, long featureID, int *errorCode);
    virtual void irradCalibrationWriteCollectionArea(long deviceID, long featureID, int *errorCode, float area);

	/* Ethernet Configuration features */
	virtual int getNumberOfEthernetConfigurationFeatures(long deviceID, int *errorCode);
	virtual int getEthernetConfigurationFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
	virtual void ethernetConfiguration_Get_MAC_Address(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex, unsigned char (&macAddress)[6]);
	virtual void ethernetConfiguration_Set_MAC_Address(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex, const unsigned char macAddress[6]);
	virtual unsigned char ethernetConfiguration_Get_GbE_Enable_Status(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex);
	virtual void ethernetConfiguration_Set_GbE_Enable_Status(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex, unsigned char enableState);

	/* DHCP Server features */
	virtual int getNumberOfDHCPServerFeatures(long deviceID, int *errorCode);
	virtual int getDHCPServerFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
	virtual void dhcpServerGetAddress(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex, unsigned char(&serverAddress)[4], unsigned char &netMask);
	virtual void dhcpServerSetAddress(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex, const unsigned char serverAddress[4], unsigned char netMask);
	virtual unsigned char dhcpServerGetEnableState(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex);
	virtual void dhcpServerSetEnableState(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex, unsigned char enableState);


	/* Network Configuration features */
	virtual int getNumberOfNetworkConfigurationFeatures(long deviceID, int *errorCode);
	virtual int getNetworkConfigurationFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
	virtual unsigned char getNumberOfNetworkInterfaces(long deviceID, long featureID, int *errorCode);
	virtual unsigned char getNetworkInterfaceConnectionType(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex);
	virtual unsigned char runNetworkInterfaceSelfTest(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex);
	virtual unsigned char getNetworkInterfaceEnableState(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex);
	virtual void setNetworkInterfaceEnableState(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex, unsigned char enableState);
	virtual void saveNetworkInterfaceConnectionSettings(long deviceID, long featureID, int *errorCode, unsigned char interfaceIndex);


    /* EEPROM capabilities */
    virtual int getNumberOfEEPROMFeatures(long deviceID, int *errorCode);
    virtual int getEEPROMFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual int eepromReadSlot(long deviceID, long featureID, int *errorCode, int slotNumber, unsigned char *buffer, int bufferLength);

    /* Light source capabilities */
    virtual int getNumberOfLightSourceFeatures(long deviceID, int *errorCode);
    virtual int getLightSourceFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual int lightSourceGetCount(long deviceID, long featureID, int *errorCode);
    virtual bool lightSourceHasEnable(long deviceID, long featureID, int *errorCode, int lightSourceIndex);
    virtual bool lightSourceIsEnabled(long deviceID, long featureID, int *errorCode, int lightSourceIndex);
    virtual void lightSourceSetEnable(long deviceID, long featureID, int *errorCode, int lightSourceIndex, bool enable);
    virtual bool lightSourceHasVariableIntensity(long deviceID, long featureID, int *errorCode, int lightSourceIndex);
    virtual double lightSourceGetIntensity(long deviceID, long featureID, int *errorCode, int lightSourceIndex);
    virtual void lightSourceSetIntensity(long deviceID, long featureID, int *errorCode, int lightSourceIndex, double intensity);

    /* Lamp capabilities */
    virtual int getNumberOfLampFeatures(long deviceID, int *errorCode);
    virtual int getLampFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual void lampSetLampEnable(long deviceID, long featureID, int *errorCode, bool strobeEnable);

    /* Continuous strobe capabilities */
    virtual int getNumberOfContinuousStrobeFeatures(long deviceID, int *errorCode);
    virtual int getContinuousStrobeFeatures(long deviceID, int *errorCode, long *buffer,
        unsigned int maxLength);
    virtual void continuousStrobeSetContinuousStrobeEnable(long deviceID,
        long featureID, int *errorCode, bool strobeEnable);
    virtual void continuousStrobeSetContinuousStrobePeriodMicroseconds(long deviceID,
        long featureID, int *errorCode, unsigned long strobePeriodMicroseconds);

    /* Shutter capabilities */
    virtual int getNumberOfShutterFeatures(long deviceID, int *errorCode);
    virtual int getShutterFeatures(long deviceID, int *errorCode, long *buffer,
        unsigned int maxLength);
    virtual void shutterSetShutterOpen(long deviceID, long featureID, int *errorCode,
        bool opened);

    /* Nonlinearity coefficient capabilities */
    virtual int getNumberOfNonlinearityCoeffsFeatures(long deviceID, int *errorCode);
    virtual int getNonlinearityCoeffsFeatures(long deviceID, int *errorCode,
        long *buffer, unsigned int maxLength);
    virtual int nonlinearityCoeffsGet(long deviceID, long featureID, int *errorCode,
        double *buffer, int maxLength);

    /* Temperature capabilities */
    virtual int getNumberOfTemperatureFeatures(long deviceID, int *errorCode);
    virtual int getTemperatureFeatures(long deviceID, int *errorCode, long *buffer,
        unsigned int maxLength);
    virtual unsigned char temperatureCountGet(long deviceID, long featureID, int *errorCode);
    virtual double temperatureGet(long deviceID, long featureID, int *errorCode, int index);
    virtual int temperatureGetAll(long deviceID, long featureID, int *errorCode,
        double *buffer, int maxLength);

	/* Introspection capabilities */
	virtual int getNumberOfIntrospectionFeatures(long deviceID, int *errorCode);
	virtual int getIntrospectionFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
	virtual unsigned short int introspectionNumberOfPixelsGet(long deviceID, long featureID, int *errorCode);
	virtual int introspectionActivePixelRangesGet(long deviceID, long featureID, int *errorCode, unsigned int *pixelRangePairs, int maxLength);
	virtual int introspectionElectricDarkPixelRangesGet(long deviceID, long featureID, int *errorCode, unsigned int *pixelRangePairs, int maxLength);
	virtual int introspectionOpticalDarkPixelRangesGet(long deviceID, long featureID, int *errorCode, unsigned int *pixelRangePairs, int maxLength);

    /* Spectrum processing capabilities */
    virtual int getNumberOfSpectrumProcessingFeatures(long deviceID, int *errorCode);
    virtual int getSpectrumProcessingFeatures(long deviceID, int *errorCode,
        long *buffer, unsigned int maxLength);
    virtual unsigned char spectrumProcessingBoxcarWidthGet(long deviceID,
        long featureID, int *errorCode);
    virtual unsigned short int spectrumProcessingScansToAverageGet(long deviceID,
        long featureID, int *errorCode);
    virtual void spectrumProcessingBoxcarWidthSet(long deviceID, long featureID,
        int *errorCode, unsigned char boxcarWidth);
    virtual void spectrumProcessingScansToAverageSet(long deviceID, long featureID,
        int *errorCode, unsigned short int scansToAverage);
 
    /* Revision capabilities */
    virtual int getNumberOfRevisionFeatures(long deviceID, int *errorCode);
    virtual int getRevisionFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual unsigned char revisionHardwareGet(long deviceID, long featureID, int *errorCode);
    virtual unsigned short int revisionFirmwareGet(long deviceID, long featureID, int *errorCode);

    /* Optical Bench capabilities */
    virtual int getNumberOfOpticalBenchFeatures(long deviceID, int *errorCode);
    virtual int getOpticalBenchFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual unsigned short int opticalBenchGetFiberDiameterMicrons(long deviceID, long featureID, int *errorCode);
    virtual unsigned short int opticalBenchGetSlitWidthMicrons(long deviceID, long featureID, int *errorCode);
    virtual int opticalBenchGetID(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);
    virtual int opticalBenchGetSerialNumber(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);
    virtual int opticalBenchGetCoating(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);
    virtual int opticalBenchGetFilter(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);
    virtual int opticalBenchGetGrating(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);

    /* Stray light coefficient capabilities */
    virtual int getNumberOfStrayLightCoeffsFeatures(long deviceID, int *errorCode);
    virtual int getStrayLightCoeffsFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual int strayLightCoeffsGet(long deviceID, long featureID, int *errorCode, double *buffer, int maxLength);

    /* Data buffer capabilities */
    virtual int getNumberOfDataBufferFeatures(long deviceID, int *errorCode);
    virtual int getDataBufferFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual void dataBufferClear(long deviceID, long featureID, int *errorCode);
    virtual void dataBufferRemoveOldestSpectra(long deviceID, long featureID, int *errorCode, unsigned int numberOfSpectra);
    virtual unsigned long dataBufferGetNumberOfElements(long deviceID, long featureID, int *errorCode);
    virtual unsigned long dataBufferGetBufferCapacity(long deviceID, long featureID, int *errorCode);
    virtual unsigned long dataBufferGetBufferCapacityMaximum(long deviceID, long featureID, int *errorCode);
    virtual unsigned long dataBufferGetBufferCapacityMinimum(long deviceID, long featureID, int *errorCode);
    virtual void dataBufferSetBufferCapacity(long deviceID, long featureID, int *errorCode, unsigned long capacity);

	/* Fast Buffer capabilities*/
	virtual int getNumberOfFastBufferFeatures(long deviceID, int *errorCode);
	virtual int getFastBufferFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
	virtual unsigned char fastBufferGetBufferingEnable(long deviceID, long featureID, int *errorCode);
	virtual void fastBufferSetBufferingEnable(long deviceID, long featureID, int *errorCode, unsigned char isEnabled);
	virtual unsigned int fastBufferGetConsecutiveSampleCount(long deviceID, long featureID, int *errorCode);
	virtual void fastBufferSetConsecutiveSampleCount(long deviceID, long featureID, int *errorCode, unsigned int consecutiveSampleCount);

    /* Acquisition delay capabilities */
    virtual int getNumberOfAcquisitionDelayFeatures(long deviceID, int *errorCode);
    virtual int getAcquisitionDelayFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    virtual void acquisitionDelaySetDelayMicroseconds(long deviceID, long featureID,
            int *errorCode, unsigned long delay_usec);
    virtual unsigned long acquisitionDelayGetDelayMicroseconds(long deviceID,
            long featureID, int *errorCode);
    virtual unsigned long acquisitionDelayGetDelayIncrementMicroseconds(long deviceID,
            long featureID, int *errorCode);
    virtual unsigned long acquisitionDelayGetDelayMaximumMicroseconds(long deviceID,
            long featureID, int *errorCode);
    virtual unsigned long acquisitionDelayGetDelayMinimumMicroseconds(long deviceID,
            long featureID, int *errorCode);

private:
    SeaBreezeAPI_Impl();

    seabreeze::api::DeviceAdapter *getDeviceByID(unsigned long id);

    std::vector<seabreeze::api::DeviceAdapter *> probedDevices;
    std::vector<seabreeze::api::DeviceAdapter *> specifiedDevices;
    
friend class SeaBreezeAPI;

};

#endif /* SEABREEZEAPI_IMPL_H */
