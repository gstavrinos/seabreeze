/***************************************************//**
 * @file    SeaBreezeAPI.h
 * @date    February 2015
 * @author  Ocean Optics, Inc., Kirk Clendinning, Heliospectra
 *
 * This is an interface to SeaBreeze that allows
 * the user to connect to devices over USB and other buses.
 * This is intended as a usable and extensible API.
 *
 * This provides a C interface to help with linkage.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2014, Ocean Optics Inc
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

#ifndef SEABREEZEAPI_H
#define SEABREEZEAPI_H

#include "api/DllDecl.h"

#ifdef __cplusplus

#include "api/seabreezeapi/DeviceAdapter.h"

/*!
    @brief  This is an interface to SeaBreeze that allows
            the user to connect to devices over USB and
            other buses.  This is intended as a usable and
            extensible API.

    @note   Detailed method documentation is available in
            the analogous C functions in SeaBreezeAPI.h
*/
class DLL_DECL SeaBreezeAPI {
public:
    /**
     * No public constructor.  To use this class in C++,
     * you must first call getInstance().  This provides
     * a singleton: it is the same on every call.
     */
    static SeaBreezeAPI *getInstance();

    /**
     * No public destructor.  Call this to force memory cleanup.
     */
    static void shutdown();

    /**
     * Use the probeDevices() method to force the driver to look for any
     * device that can be found automatically.  If this is not called then
     * such devices will not be available for use.  This should be used when
     * attempting to find USB devices.
     */
    int probeDevices();
    
    /**
     * Use the addIPv4DeviceLocation() method to specify that a device may be
     * found on a TCP/IPv4 network on a given port.  Once specified,
     * the typical openDevice() function can be used to access it.
     */
    int addTCPIPv4DeviceLocation(char *deviceTypeName, char *ipAddr, int port);

    /**
     * Use the addRS232DeviceLocation() method to specify that a device may be
     * found on a particular serial bus with a given baud rate.  Once specified,
     * the typical openDevice() function can be used to access it.
     */
    int addRS232DeviceLocation(char *deviceTypeName, char *deviceBusPath, unsigned int baud);

    /**
     * This provides the number of devices that have either been probed or
     * manually specified.  Devices are not opened automatically, but this can
     * provide a bound for getDeviceIDs().
     */
    int getNumberOfDeviceIDs();

    /**
     * This provides a unique ID of each device that is detected or specified.
     * The IDs are copied into the user-provided buffer.  These IDs are weak
     * references: attempting to access a device that no longer exists will cause
     * an error value to be returned but should not cause any instability.
     * The IDs may be entirely random, but a given ID will always refer to the
     * same device for as long as the program is running.  This will return the
     * number of device IDs actually copied into the array or 0 on error.
     */
    int getDeviceIDs(long *ids, unsigned long maxLength);

    /**
     * This will attempt to open the bus connetion to the device with the given ID.
     * Returns 0 on success, other value on error.
     */
    int openDevice(long id, int *errorCode);

    /**
     * This will attempt to close the bus connection to the device with the given ID.
     */
    void closeDevice(long id, int *errorCode);

    /* Get a string that describes the type of device */
    int getDeviceType(long id, int *errorCode, char *buffer, unsigned int length);
    
    /* Get the usb endpoint address for a specified type of endpoint */
   	unsigned char getDeviceEndpoint(long id, int *error_code, usbEndpointType endpointType);

    /* Get raw usb access capabilities */
    int getNumberOfRawUSBBusAccessFeatures(long deviceID, int *errorCode);
    int getRawUSBBusAccessFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    int rawUSBBusAccessRead(long deviceID, long featureID, int *errorCode, unsigned char *buffer, unsigned int bufferLength, unsigned char endpoint);
    int rawUSBBusAccessWrite(long deviceID, long featureID, int *errorCode, unsigned char *buffer, unsigned int bufferLength, unsigned char endpoint);
    
    /* Serial number capabilities */
    int getNumberOfSerialNumberFeatures(long deviceID, int *errorCode);
    int getSerialNumberFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    int getSerialNumber(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);
    unsigned char getSerialNumberMaximumLength(long deviceID, long featureID, int *errorCode);

    /* Spectrometer capabilities */
    int getNumberOfSpectrometerFeatures(long id, int *errorCode);
    int getSpectrometerFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    void spectrometerSetTriggerMode(long deviceID, long spectrometerFeatureID, int *errorCode, int mode);
    void spectrometerSetIntegrationTimeMicros(long deviceID, long spectrometerFeatureID, int *errorCode, unsigned long integrationTimeMicros);
    unsigned long spectrometerGetMinimumIntegrationTimeMicros(long deviceID, long spectrometerFeatureID, int *errorCode);
    unsigned long spectrometerGetMaximumIntegrationTimeMicros(long deviceID, long spectrometerFeatureID, int *errorCode);
    double spectrometerGetMaximumIntensity(long deviceID, long spectrometerFeatureID, int *errorCode);
    int spectrometerGetUnformattedSpectrumLength(long deviceID, long spectrometerFeatureID, int *errorCode);
    int spectrometerGetUnformattedSpectrum(long deviceID, long spectrometerFeatureID, int *errorCode, unsigned char *buffer, int bufferLength);
    int spectrometerGetFormattedSpectrumLength(long deviceID, long spectrometerFeatureID, int *errorCode);
    int spectrometerGetFormattedSpectrum(long deviceID, long spectrometerFeatureID, int *errorCode, double *buffer, int bufferLength);
    int spectrometerGetWavelengths(long deviceID, long spectrometerFeatureID, int *errorCode, double *wavelengths, int length);
    int spectrometerGetElectricDarkPixelCount(long deviceID, long spectrometerFeatureID, int *errorCode);
    int spectrometerGetElectricDarkPixelIndices(long deviceID, long spectrometerFeatureID, int *errorCode, int *indices, int length);

    /* Pixel binning capabilities */
    int getNumberOfPixelBinningFeatures(long id, int *errorCode);
    int getPixelBinningFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    void binningSetPixelBinningFactor(long deviceID, long spectrometerFeatureID, int *errorCode, const unsigned char binningFactor);
    unsigned char binningGetPixelBinningFactor(long deviceID, long spectrometerFeatureID, int *errorCode);
    void binningSetDefaultPixelBinningFactor(long deviceID, long spectrometerFeatureID, int *errorCode, const unsigned char binningFactor);
    void binningSetDefaultPixelBinningFactor(long deviceID, long spectrometerFeatureID, int *errorCode);
    unsigned char binningGetDefaultPixelBinningFactor(long deviceID, long spectrometerFeatureID, int *errorCode);
    unsigned char binningGetMaxPixelBinningFactor(long deviceID, long spectrometerFeatureID, int *errorCode);
        
    /* TEC capabilities */
    int getNumberOfThermoElectricFeatures(long deviceID, int *errorCode);
    int getThermoElectricFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    double tecReadTemperatureDegreesC(long deviceID, long featureID, int *errorCode);
    void tecSetTemperatureSetpointDegreesC(long deviceID, long featureID, int *errorCode, double temperatureDegreesCelsius);
    void tecSetEnable(long deviceID, long featureID, int *errorCode, unsigned char tecEnable);

    /* Irradiance calibration features */
    int getNumberOfIrradCalFeatures(long deviceID, int *errorCode);
    int getIrradCalFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    int irradCalibrationRead(long deviceID, long featureID, int *errorCode, float *buffer, int bufferLength);
    int irradCalibrationWrite(long deviceID, long featureID, int *errorCode, float *buffer, int bufferLength);
    int irradCalibrationHasCollectionArea(long deviceID, long featureID, int *errorCode);
    float irradCalibrationReadCollectionArea(long deviceID, long featureID, int *errorCode);
    void irradCalibrationWriteCollectionArea(long deviceID, long featureID, int *errorCode, float area);

    /* EEPROM capabilities */
    int getNumberOfEEPROMFeatures(long deviceID, int *errorCode);
    int getEEPROMFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    int eepromReadSlot(long deviceID, long featureID, int *errorCode, int slotNumber, unsigned char *buffer, int bufferLength);

    /* Light source capabilities */
    int getNumberOfLightSourceFeatures(long deviceID, int *errorCode);
    int getLightSourceFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    int lightSourceGetCount(long deviceID, long featureID, int *errorCode);
    bool lightSourceHasEnable(long deviceID, long featureID, int *errorCode, int lightSourceIndex);
    bool lightSourceIsEnabled(long deviceID, long featureID, int *errorCode, int lightSourceIndex);
    void lightSourceSetEnable(long deviceID, long featureID, int *errorCode, int lightSourceIndex, bool enable);
    bool lightSourceHasVariableIntensity(long deviceID, long featureID, int *errorCode, int lightSourceIndex);
    double lightSourceGetIntensity(long deviceID, long featureID, int *errorCode, int lightSourceIndex);
    void lightSourceSetIntensity(long deviceID, long featureID, int *errorCode, int lightSourceIndex, double intensity);

    /* Lamp capabilities */
    int getNumberOfLampFeatures(long deviceID, int *errorCode);
    int getLampFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    void lampSetLampEnable(long deviceID, long featureID, int *errorCode, bool strobeEnable);

    /* Continuous strobe capabilities */
    int getNumberOfContinuousStrobeFeatures(long deviceID, int *errorCode);
    int getContinuousStrobeFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    void continuousStrobeSetContinuousStrobeEnable(long deviceID, long featureID, int *errorCode, bool strobeEnable);
    void continuousStrobeSetContinuousStrobePeriodMicroseconds(long deviceID, long featureID, int *errorCode, unsigned long strobePeriodMicroseconds);

    /* Shutter capabilities */
    int getNumberOfShutterFeatures(long deviceID, int *errorCode);
    int getShutterFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    void shutterSetShutterOpen(long deviceID, long featureID, int *errorCode, bool opened);

    /* Nonlinearity coefficient capabilities */
    int getNumberOfNonlinearityCoeffsFeatures(long deviceID, int *errorCode);
    int getNonlinearityCoeffsFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    int nonlinearityCoeffsGet(long deviceID, long featureID, int *errorCode, double *buffer, int maxLength);

    /* Temperature capabilities */
    int getNumberOfTemperatureFeatures(long deviceID, int *errorCode);
    int getTemperatureFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    unsigned char temperatureCountGet(long deviceID, long featureID, int *errorCode);
    double temperatureGet(long deviceID, long featureID, int *errorCode, int index);
    int temperatureGetAll(long deviceID, long featureID, int *errorCode, double *buffer, int maxLength);

    /* Spectrum processing capabilities */
    int getNumberOfSpectrumProcessingFeatures(long deviceID, int *errorCode);
    int getSpectrumProcessingFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    unsigned char spectrumProcessingBoxcarWidthGet(long deviceID, long featureID, int *errorCode);
    unsigned short int spectrumProcessingScansToAverageGet(long deviceID, long featureID, int *errorCode);
    void spectrumProcessingBoxcarWidthSet(long deviceID, long featureID, int *errorCode, unsigned char boxcarWidth);
    void spectrumProcessingScansToAverageSet(long deviceID, long featureID, int *errorCode, unsigned short int scansToAverage);
 
    /* Revision capabilities */
    int getNumberOfRevisionFeatures(long deviceID, int *errorCode);
    int getRevisionFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    unsigned char revisionHardwareGet(long deviceID, long featureID, int *errorCode);
    unsigned short int revisionFirmwareGet(long deviceID, long featureID, int *errorCode);

    /* Optical Bench capabilities */
    int getNumberOfOpticalBenchFeatures(long deviceID, int *errorCode);
    int getOpticalBenchFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    unsigned short int opticalBenchGetFiberDiameterMicrons(long deviceID, long featureID, int *errorCode);
    unsigned short int opticalBenchGetSlitWidthMicrons(long deviceID, long featureID, int *errorCode);
    int opticalBenchGetID(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);
    int opticalBenchGetSerialNumber(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);
    int opticalBenchGetCoating(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);
    int opticalBenchGetFilter(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);
    int opticalBenchGetGrating(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);

    /* Stray light coefficient capabilities */
    int getNumberOfStrayLightCoeffsFeatures(long deviceID, int *errorCode);
    int getStrayLightCoeffsFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    int strayLightCoeffsGet(long deviceID, long featureID, int *errorCode, double *buffer, int maxLength);

    /* Data buffer capabilities */
    int getNumberOfDataBufferFeatures(long deviceID, int *errorCode);
    int getDataBufferFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    void dataBufferClear(long deviceID, long featureID, int *errorCode);
    unsigned long dataBufferGetNumberOfElements(long deviceID, long featureID, int *errorCode);
    unsigned long dataBufferGetBufferCapacity(long deviceID, long featureID, int *errorCode);
    unsigned long dataBufferGetBufferCapacityMaximum(long deviceID, long featureID, int *errorCode);
    unsigned long dataBufferGetBufferCapacityMinimum(long deviceID, long featureID, int *errorCode);
    void dataBufferSetBufferCapacity(long deviceID, long featureID, int *errorCode, unsigned long capacity);

    /* Acquisition delay capabilities */
    int getNumberOfAcquisitionDelayFeatures(long deviceID, int *errorCode);
    int getAcquisitionDelayFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    void acquisitionDelaySetDelayMicroseconds(long deviceID, long featureID,
            int *errorCode, unsigned long delay_usec);
    unsigned long acquisitionDelayGetDelayMicroseconds(long deviceID,
            long featureID, int *errorCode);
    unsigned long acquisitionDelayGetDelayIncrementMicroseconds(long deviceID,
            long featureID, int *errorCode);
    unsigned long acquisitionDelayGetDelayMaximumMicroseconds(long deviceID,
            long featureID, int *errorCode);
    unsigned long acquisitionDelayGetDelayMinimumMicroseconds(long deviceID,
            long featureID, int *errorCode);

private:
    SeaBreezeAPI();
    virtual ~SeaBreezeAPI();

    seabreeze::api::DeviceAdapter *getDeviceByID(unsigned long id);

    static SeaBreezeAPI *instance;

    std::vector<seabreeze::api::DeviceAdapter *> probedDevices;
    std::vector<seabreeze::api::DeviceAdapter *> specifiedDevices;
};

extern "C" {
#endif /* __cplusplus */

    /* All of these C functions start with sbapi_ to prevent namespace
     * collisions.
     */

    /**
     * This should be called prior to any other sbapi_call.  The API may
     * recover gracefully if this is not called, but future releases may assume
     * this is called first.  This should be called synchronously -- a single
     * thread should call this.
     */

    DLL_DECL void sbapi_initialize();

    /**
     * This may be called to free up any allocated memory being held by the
     * driver interface.  After this is called by any thread, sbapi_initialize
     * should be called again before any other sbapi_ functions are used.
     */
    DLL_DECL void sbapi_shutdown();
    
    /**
     * This specifies to the driver that a device of the given type might be
     * found on the network at a given address and port.  The driver will add
     * the device type and location to the set of those that can be opened.
     *
     * @param deviceTypeName (Input) The name of a type of device.  This can be
     *      one of the following: Jaz
     *
     * @param ipAddress (Input) The IPv4 address of the device.  This should be
     * in "dotted quads" notation, such as "192.168.1.100".
     *
     * @param port (Input) The network port to open on the device.  This will
     * depend on the device type; consult its datasheet.
     *
     * @return zero on success, non-zero on error
     */
    DLL_DECL int
    sbapi_add_TCPIPv4_device_location(char *deviceTypeName, char *ipAddress,
                unsigned int port);

    /**
     * This specifies to the driver that a device of the given type might be
     * found on a particular RS232 bus (e.g. a COM port).  The driver will add
     * the device type and location to the set of those that can be opened.
     *
     * @param deviceTypeName (Input) The name of a type of device.  This can be
     *      one of the following: QE-PRO, STS.
     *
     * @param deviceBusPath (Input) The location of the device on the RS232 bus.
     *      This will be a platform-specific location.  Under Windows, this may
     *      be COM1, COM2, etc.  Under Linux, this might be /dev/ttyS0, /dev/ttyS1,
     *      etc.
     *
     * @param baud (Input) Baud rate at which to open the device.  This should
     *      be specified as the rate itself, e.g. 9600, 57600, or 115200.
     *
     * @return zero on success, non-zero on error
     */
    DLL_DECL int
    sbapi_add_RS232_device_location(char *deviceTypeName, char *deviceBusPath,
                unsigned int baud);

    /**
     * This causes a search for known devices on all buses that support
     * autodetection.  This does NOT automatically open any device -- that must
     * still be done with the sbapi_open_device() function.  Note that this
     * should only be done by one thread at a time, and it is recommended that
     * other threads avoid calling sbapi_get_number_of_device_ids() or
     * sbapi_get_device_ids() while this is executing.  Ideally, a single thread
     * should be designated for all device discovery/get actions, and
     * separate worker threads can be used for each device discovered.
     *
     * @return the total number of devices that have been found
     *      automatically.  If called repeatedly, this will always return the
     *      number of devices most recently found, even if they have been
     *      found or opened previously.
     */
    DLL_DECL int
    sbapi_probe_devices();

    /**
     * This returns the total number of devices that are known either because
     * they have been specified with sbapi_add_RS232_device_location or
     * because they were probed on some bus.  This can be used to bound the
     * number of device references that can be gotten with
     * sbapi_get_device_ids().
     *
     * @return the total number of devices references that are available
     *      through sbapi_get_device_ids().
     */
    DLL_DECL int
    sbapi_get_number_of_device_ids();

    /**
     * This will populate the provided buffer with up to max_ids of device
     * references.  These references must be used as the first parameter to
     * most of the other sbapi_ calls.  Each uniquely identifies a single
     * device instance.
     *
     * @param ids (Output) an array of long integers that will be overwritten
     *           with the unique IDs of each known device.  Note that these
     *           devices will not be open by default.
     * @param max_ids (Input) the maximum number of IDs that may be written
     *           to the array
     *
     * @return The total number of device IDs that were written to the array.
     *      This may be zero on error.
     */
    DLL_DECL int
    sbapi_get_device_ids(long *ids, unsigned int max_ids);

    /**
     * This function opens a device attached to the system.  The device must
     * be provided as a location ID from the sbapi_get_device_ids()
     * function.  Such locations can either be specified or probed using the
     * other methods in this interface.
     *
     * @param id (Input) The location ID of a device to try to open.  Only IDs
     *      that have been returned by a previous call to seabreeze_get_device_ids()
     *      are valid.
     * @param error_code (Output) A pointer to an integer that can be used for
     *      storing error codes.
     *
     * @return 0 if it opened a device successfully, or 1 if no device was opened
     *      (in which case the error_code variable will be set).
     */
    DLL_DECL int
    sbapi_open_device(long id, int *error_code);

    /**
     * This function closes the spectrometer attached to the system.
     *
     * @param id (Input) The location ID of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     *
     */
    DLL_DECL void
    sbapi_close_device(long id, int *error_code);

    /**
     * This function returns a description of the error denoted by
     * error_code.
     *
     * @param error_code (Input) The integer error code to look up.  Error codes
     *      may not be zero, but can be any non-zero integer (positive or
     *      negative).
     *
     * @return char *: A description in the form of a string that describes
     *      what the error was.
     */
    DLL_DECL const char *
    sbapi_get_error_string(int error_code);

    /**
     * This function copies a string denoting the type of the device into the
     * provided buffer.
     *
     * @param id (Input) The location ID of a device previously opened with
     *      sbapi_get_device_locations().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.  This may be NULL.
     * @param buffer (Output) Pointer to a user buffer that the name will be
     *      stored into.  This may be one of the following:
     *      \li NONE: Used if no spectrometer is found (error_code will also be set)
     *      \li HR2000: Represents an HR2000 spectrometer
     *      \li HR2000PLUS: Represents an HR2000+ spectrometer
     *      \li HR4000: Represents an HR4000 spectrometer
     *      \li JAZ: Represents a Jaz spectrometer
     *      \li MAYA2000: Represents a MAYA2000 spectrometer
     *      \li MAYA2000PRO: Represents a MAYA2000PRO spectrometer
     *      \li MAYALSL: Represents a Maya-LSL spectrometer
     *      \li NIRQUEST256: Represents an NIRQUEST256 spectrometer
     *      \li NIRQUEST512: Represents an NIRQUEST512 spectrometer
     *      \li QE65000: Represents a QE65000 spectrometer
     *      \li STS: Represents an STS spectrometer
     *      \li Torus: Represents a Torus spectrometer
     *      \li USB2000: Represents a USB2000 spectrometer
     *      \li USB2000PLUS: Represents a USB2000+ spectrometer
     *      \li USB4000: Represents a USB4000 spectrometer
     *
     * @param length (Input) Maximum number of bytes that may be written to the
     *      buffer
     *
     * @return integral number of bytes actually written to the user buffer
     */
    DLL_DECL int
    sbapi_get_device_type(long id, int *error_code,
            char *buffer, unsigned int length);

    /**
     * This function returns the usb endpoint for the type specified.
     * If the type is not supported by the device, a zero is returned.
     * 0 is normally the control endpoint. That value is not valid in this context.
     *
     * @param deviceID (Input)  The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @return the usb endpoint address.
     */
    DLL_DECL unsigned char
    sbapi_get_device_usb_endpoint_primary_out(long id, int *error_code);

    /**
     * This function returns the usb endpoint for the type specified.
     * If the type is not supported by the device, a zero is returned.
     * 0 is normally the control endpoint. That value is not valid in this context.
     *
     * @param deviceID (Input)  The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @return the usb endpoint address.
     */
    DLL_DECL unsigned char
    sbapi_get_device_usb_endpoint_primary_in(long id, int *error_code);
    
    /**
     * This function returns the usb endpoint for the type specified.
     * If the type is not supported by the device, a zero is returned.
     * 0 is normally the control endpoint. That value is not valid in this context.
     *
     * @param deviceID (Input)  The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @return the usb endpoint address.
     */
    DLL_DECL unsigned char
    sbapi_get_device_usb_endpoint_secondary_out(long id, int *error_code);

    /**
     * This function returns the usb endpoint for the type specified.
     * If the type is not supported by the device, a zero is returned.
     * 0 is normally the control endpoint. That value is not valid in this context.
     *
     * @param deviceID (Input)  The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @return the usb endpoint address.
     */
    DLL_DECL unsigned char
    sbapi_get_device_usb_endpoint_secondary_in(long id, int *error_code);

    /**
     * This function returns the usb endpoint for the type specified.
     * If the type is not supported by the device, a zero is returned.
     * 0 is normally the control endpoint. That value is not valid in this context.
     *
     * @param deviceID (Input)  The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @return the usb endpoint address.
     */
    DLL_DECL unsigned char
    sbapi_get_device_usb_endpoint_secondary_in2(long id, int *error_code);


    /**
     * This function returns the total number of raw usb bus access feature
     * instances available in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of raw usb bus access features that will be
     *      returned by a call to sbapi_get_raw_usb_bus_access_features().
     */
    DLL_DECL int
    sbapi_get_number_of_raw_usb_bus_access_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each raw usb bus access feature
     * instance for this device.  The IDs are only valid when used with the
     * deviceID used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) a preallocated array to hold returned feature handles
     * @param max_features (Input) length of the preallocated buffer
     *
     * @return the number of raw usb bus access feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_raw_usb_bus_access_features(long deviceID, int *error_code, long *features,
            int max_features);

    /**
     * This function reads out a raw usb access from the spectrometer's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of an irradiance calibration
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_raw_usb_access_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param buffer (Output) preallocated array to hold an unsigned char buffer
     * @param buffer_length (Input) size of the preallocated buffer (should equal pixel count)
     * @param endpoint (Input) a USB endpoint gotten from one of the
     *         sbapi_get_device_usb_endpoint_xxx_xxx() type calls.
     *
     * @return the number of floats read from the device into the buffer
     */
    DLL_DECL int
    sbapi_raw_usb_bus_access_read(long deviceID, long featureID,
            int *error_code, unsigned char *buffer, int buffer_length, unsigned char endpoint);

    /**
     * This function writes a buffer of unsigned chars to the specified USB endpoint
     * if the feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of an raw usb bus access
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_raw_usb_bus_access_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param buffer (Output) a pointer to unsigned char values to send to the usb endpoint
     * @param buffer_length (Input) number of calibration factors to write
     * @param endpoint (Input) a USB endpoint gotten from one of the
     *         sbapi_get_device_usb_endpoint_xxx_xxx() type calls.
     *
     * @return the number of floats written from the buffer to the device
     */
    DLL_DECL int
    sbapi_raw_usb_bus_access_write(long deviceID, long featureID,
            int *error_code, unsigned char *buffer, int buffer_length, unsigned char endpoint);


    /**
     * This function returns the total number of serial number instances available
     * in the indicated device.  Each instance may refer to a different module.
     *
     * @param deviceID (Input)  The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @return the number of serial_number features that will be returned
     *  by a call to sbapi_get_serial_number_features().
     */
    DLL_DECL int
    sbapi_get_number_of_serial_number_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each serial number instance for this
     * device.  Each instance refers to a single serial number feature.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for
     *      storing error codes.
     * @param features (Output) a pre-allocated array to hold the list of
     *      supported serial number features
     * @param max_features (Input) size of the preallocated output array
     * @return the number of serial number feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_serial_number_features(long deviceID, int *error_code, long *features,
            int max_features);

    /**
     * This reads the device's serial number and fills the
     * provided array (up to the given length) with it.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a serial
     *      number feature.  Valid IDs can be found with the
     *      sbapi_get_serial_number_features() function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @param buffer (Output)  A pre-allocated array of characters that the
     *      serial number will be copied into
     * @param buffer_length (Input) The number of values to copy into the buffer
     *      (this should be no larger than the number of chars allocated in
     *      the buffer)
     *
     * @return the number of bytes written into the buffer
     */
    DLL_DECL int
    sbapi_get_serial_number(long deviceID, long featureID, int *error_code,
            char *buffer, int buffer_length);

    /**
     * This reads the possible maximum length of the device's serial number 
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a serial
     *      number feature.  Valid IDs can be found with the
     *      sbapi_get_serial_number_features() function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     *
     * @return the length of the serial number in an unsigned character byte
     */
    DLL_DECL unsigned char
    sbapi_get_serial_number_maximum_length(long deviceID, long featureID, int *error_code);
            
    /**
     * This function returns the total number of spectrometer instances available
     * in the indicated device.  Each instance refers to a single optical bench.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @return the number of spectrometer features that will be returned
     *  by a call to sbapi_get_spectrometer_features().
     */
    DLL_DECL int
    sbapi_get_number_of_spectrometer_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each spectrometer instance for this
     * device.  Each instance refers to a single optical bench.  The IDs are only
     * valid when used with the deviceID used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for
     *      storing error codes.
     * @param features (Output) a preallocated output array to hold the features
     * @param max_features (Input) size of the preallocated output array
     * @return Returns the number of spectrometer feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_spectrometer_features(long deviceID, int *error_code, long *features,
            int max_features);

    /**
     * This function sets the trigger mode for the specified device.
     * Note that requesting an unsupported mode will result in an error.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a
     *      spectrometer feature.  Valid IDs can be found with the
     *      sbapi_get_spectrometer_features() function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @param mode (Input) a trigger mode (0 = normal, 1 = software,
     *      2 = synchronization, 3 = external hardware, etc - check your
     *      particular spectrometer's Data Sheet)
     */
    DLL_DECL void
    sbapi_spectrometer_set_trigger_mode(long deviceID, long featureID,
        int *error_code, int mode);

    /**
     * This function sets the integration time for the specified device.
     * This function should not be responsible for performing stability
     * scans.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a
     *      spectrometer feature.  Valid IDs can be found with the
     *      sbapi_get_spectrometer_features() function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @param integration_time_micros (Input) The new integration time in
     *      units of microseconds
     */
    DLL_DECL void
    sbapi_spectrometer_set_integration_time_micros(long deviceID, long featureID,
            int *error_code, unsigned long integration_time_micros);

    /**
     * This function returns the smallest integration time setting,
     * in microseconds, that is valid for the spectrometer.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a
     *      spectrometer feature.  Valid IDs can be found with the
     *      sbapi_get_spectrometer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used
     *      for storing error codes.
     * @return Returns minimum legal integration time in microseconds if > 0.
     *      On error, returns -1 and error_code will be set accordingly.
     */
    DLL_DECL long
    sbapi_spectrometer_get_minimum_integration_time_micros(long deviceID,
            long featureID, int *error_code);

    /**
     * This function returns the maximum pixel intensity for the
     * spectrometer.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a
     *      spectrometer feature.  Valid IDs can be found with the
     *      sbapi_get_spectrometer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used
     *      for storing error codes.
     * @return Returns maximum pixel intensity if > 0.
     *      On error, returns -1 and error_code will be set accordingly.
     */
    DLL_DECL double
    sbapi_spectrometer_get_maximum_intensity(long deviceID,
            long featureID, int *error_code);

    /**
     * This returns an integer denoting the number of pixels in a
     * formatted spectrum (as returned by get_formatted_spectrum(...)).
     *
     * @param deviceID (Input)  The index of a device previously opened with
     *      sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a spectrometer
     *      feature.  Valid IDs can be found with the sbapi_get_spectrometer_features()
     *      function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     *
     * @return the length of a formatted spectrum.
     */
    DLL_DECL int
    sbapi_spectrometer_get_formatted_spectrum_length(long deviceID,
            long featureID, int *error_code);

    /**
     * This acquires a spectrum and returns the answer in formatted
     *     floats.  In this mode, auto-nulling should be automatically
     *     performed for devices that support it.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a
     *      spectrometer feature.  Valid IDs can be found with the
     *      sbapi_get_spectrometer_features() function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @param buffer (Output) A buffer (with memory already allocated) to
     *      hold the spectral data
     * @param buffer_length (Input) The length of the buffer
     *
     * @return the number of floats read into the buffer
     */
    DLL_DECL int
    sbapi_spectrometer_get_formatted_spectrum(long deviceID,
            long featureID, int *error_code,
            double* buffer, int buffer_length);

    /**
     * This returns an integer denoting the length of a raw spectrum
     * (as returned by get_unformatted_spectrum(...)).
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      open_spectrometer().
     * @param featureID (Input) The ID of a particular instance of a
     *      spectrometer feature.  Valid IDs can be found with the
     *      sbapi_get_spectrometer_features() function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     *
     * @return the length of an unformatted spectrum.
     */
    DLL_DECL int
    sbapi_spectrometer_get_unformatted_spectrum_length(long deviceID,
            long featureID, int *error_code);

    /**
     * This acquires a spectrum and returns the answer in raw,
     * unformatted bytes.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      open_spectrometer().
     * @param featureID (Input) The ID of a particular instance of a spectrometer
     *      feature.  Valid IDs can be found with the sbapi_get_spectrometer_features()
     *      function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @param buffer (Output) A buffer (with memory already allocated) to hold
     *      the spectral data
     * @param buffer_length (Input) The length of the buffer
     *
     * @return the number of bytes read into the buffer
     */
    DLL_DECL int
    sbapi_spectrometer_get_unformatted_spectrum(long deviceID,
            long featureID, int *error_code,
            unsigned char *buffer, int buffer_length);

    /**
     * This computes the wavelengths for the spectrometer and fills in the
     * provided array (up to the given length) with those values.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      open_spectrometer().
     * @param featureID (Input) The ID of a particular instance of a spectrometer
     *      feature.  Valid IDs can be found with the sbapi_get_spectrometer_features()
     *      function.
     * @param error_code (Ouput) pointer to an integer that can be used for storing
     *      error codes.
     * @param wavelengths (Output) A pre-allocated array of doubles that the wavelengths
     *      will be copied into
     * @param length (Input) The number of values to copy into the wavelength array
     *      (this should be no larger than the number of doubles allocated in the wavelengths
     *      array)
     *
     * @return the number of bytes written into the wavelength buffer
     */
    DLL_DECL int
    sbapi_spectrometer_get_wavelengths(long deviceID,
            long featureID, int *error_code, double *wavelengths, int length);

    /**
     * This returns the number of pixels that are electrically active but
     * optically masked (a.k.a. electric dark pixels).  Note that not all
     * detectors have optically masked pixels; in that case, this function
     * will return zero.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a spectrometer
     *      feature.  Valid IDs can be found with the sbapi_get_spectrometer_features()
     *      function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of pixels that can be retrieved by the
     *      sbapi_spectrometer_get_electric_dark_pixel_indices() function.
     */
    DLL_DECL int
    sbapi_spectrometer_get_electric_dark_pixel_count(long deviceID,
            long featureID, int *error_code);

    /**
     * This fills in the provided array (up to the given length) with the indices
     * of the pixels that are electrically active but optically masked
     * (a.k.a. electric dark pixels).  Note that not all detectors have optically
     * masked pixels; in that case, this function will return zero.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a spectrometer
     *      feature.  Valid IDs can be found with the sbapi_get_spectrometer_features()
     *      function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @param indices (Output) A pre-allocated array of ints that the pixel indices
     *      will be copied into
     * @param length (Input) The number of values to copy into the indices array
     *      (this should be no larger than the number of ints allocated in the indices
     *      array)
     *
     * @return the number of bytes written into the indices buffer
     */
    DLL_DECL int
    sbapi_spectrometer_get_electric_dark_pixel_indices(long deviceID,
            long featureID, int *error_code, int *indices, int length);

    /**
     * This function returns the total number of pixel binning instances available
     * in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     *
     * @return the number of pixel binning features that will be returned by a call
     *      to sbapi_get_pixel_binning_features().
     */
    DLL_DECL int
    sbapi_get_number_of_pixel_binning_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each pixel binning feature for this
     * device.  The IDs are only valid when used with the deviceID used to
     * obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for
     *      storing error codes.
     * @param features (Output) a pre-populated array to hold the returned
     *      feature handles
     * @param max_features (Input) size of the pre-allocated array
     *
     * @return the number of pixel binning feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_pixel_binning_features(long deviceID, int *error_code, long *features,
            int max_features);

    /**
     * This function sets the pixel binning factor on the device.
     *
     *  @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     *  @param featureID (Input) The ID of a particular instance of a pixel binning feature.
     *        Valid IDs can be found with the sbapi_get_pixel_binning_features() function.
     *  @param error_code (Output) A pointer to an integer that can be used for
     *      storing error codes.
     *  @param factor (Input) The desired pixel binning factor.
     */
    DLL_DECL void
    sbapi_binning_set_pixel_binning_factor(long deviceID, long featureID,
            int *error_code, unsigned char factor);

    /**
     * This function gets the pixel binning factor on the device.
     *
     *  @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     *  @param featureID (Input) The ID of a particular instance of a pixel binning feature.
     *        Valid IDs can be found with the sbapi_get_pixel_binning_features() function.
     *  @param error_code (Output) A pointer to an integer that can be used for
     *      storing error codes.
     *
     * @return the pixel binning factor for the specified feature.
     */
    DLL_DECL unsigned char
    sbapi_binning_get_pixel_binning_factor(long deviceID, long featureID, int *error_code);

    /**
     * This function sets the default pixel binning factor on the device.
     *
     *  @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     *  @param featureID (Input) The ID of a particular instance of a pixel binning feature.
     *        Valid IDs can be found with the sbapi_get_pixel_binning_features() function.
     *  @param error_code (Output)A pointer to an integer that can be used for
     *      storing error codes.
     *  @param factor (Input) The desired default pixel binning factor.
     */
    DLL_DECL void
    sbapi_binning_set_default_pixel_binning_factor(long deviceID, long featureID,
            int *error_code, unsigned char factor);

    /**
     * This function resets the default pixel binning factor on the device back to the factory default.
     *
     *  @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     *  @param featureID (Input) The ID of a particular instance of a pixel binning feature.
     *        Valid IDs can be found with the sbapi_get_pixel_binning_features() function.
     *  @param error_code (Output)A pointer to an integer that can be used for
     *      storing error codes.
     */
    DLL_DECL void
    sbapi_binning_reset_default_pixel_binning_factor(long deviceID, long featureID,
            int *error_code);

    /**
     * This function gets the default pixel binning factor on the device.
     *
     *  @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     *  @param featureID (Input) The ID of a particular instance of a pixel binning feature.
     *        Valid IDs can be found with the sbapi_get_pixel_binning_features() function.
     *  @param error_code (Output)A pointer to an integer that can be used for
     *      storing error codes.
     *
     * @return the default pixel binning factor for the specified feature.
     */
    DLL_DECL unsigned char
    sbapi_binning_get_default_pixel_binning_factor(long deviceID, long featureID, int *error_code);

    /**
     * This function gets the maximum pixel binning factor on the device.
     *
     *  @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     *  @param featureID (Input) The ID of a particular instance of a pixel binning feature.
     *        Valid IDs can be found with the sbapi_get_pixel_binning_features() function.
     *  @param error_code (Output)A pointer to an integer that can be used for
     *      storing error codes.
     *
     * @return the maximum pixel binning factor for the specified feature.
     */
    DLL_DECL unsigned char
    sbapi_binning_get_max_pixel_binning_factor(long deviceID, long featureID, int *error_code);

    /**
     * This function returns the total number of shutter instances available
     * in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     *
     * @return the number of shutter features that will be returned by a call
     *      to sbapi_get_shutter_features().
     */
    DLL_DECL int
    sbapi_get_number_of_shutter_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each shutter instance for this
     * device.  The IDs are only valid when used with the deviceID used to
     * obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for
     *      storing error codes.
     * @param features (Output) a pre-populated array to hold the returned
     *      feature handles
     * @param max_features (Input) size of the pre-allocated array
     *
     * @return the number of shutter feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_shutter_features(long deviceID, int *error_code, long *features,
            int max_features);

    /**
     * This function sets the shutter state on the device.
     *
     *  @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     *  @param featureID (Input) The ID of a particular instance of a shutter feature.
     *        Valid IDs can be found with the sbapi_get_shutter_features() function.
     *  @param error_code (Output)A pointer to an integer that can be used for
     *      storing error codes.
     *  @param opened (Input) A boolean used for denoting the desired state
     *      (opened/closed) of the shutter.   If the value of
     *      opened is non-zero, then the shutter will open.  If
     *      the value of opened is zero, then the shutter will close.
     */
    DLL_DECL void
    sbapi_shutter_set_shutter_open(long deviceID, long featureID,
            int *error_code, unsigned char opened);

    /**
     * This function returns the total number of light source instances available
     * in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     *
     * @return the number of light source features that will be returned
     *      by a call to sbapi_get_light_source_features().
     */
    DLL_DECL int
    sbapi_get_number_of_light_source_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each light source instance for this
     * device.  The IDs are only valid when used with the deviceID used to
     * obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @param features (Output) pre-allocated buffer that will receive the IDs of the
     *      feature instances
     * @param max_features (Input) the maximum number of elements that can be
     *      copied into the provided features array
     *
     * @return the number of light source feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_light_source_features(long deviceID, int *error_code,
            long *features, int max_features);

    /**
     * This function gets the number of light sources that are represented by
     * the given featureID.  Such light sources could be individual LEDs,
     * light bulbs, lasers, etc.  Each of these light sources may have different
     * capabilities, such as programmable intensities and enables, which should
     * be queried before they are used.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param featureID (Input)  The ID of a particular instance of a light source
     *      feature.  Valid IDs can be found with sbapi_get_light_source_features().
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     *
     * @return the number of light sources (e.g. bulbs) in the indicated feature
     */
    DLL_DECL int
    sbapi_light_source_get_count(long deviceID, long featureID, int *error_code);

    /**
     * Queries whether the indicated light source within the given feature
     * instance has a usable enable/disable control.  If this returns 0
     * (meaning no enable available) then calling sbapi_light_source_set_enable()
     * or sbapi_light_source_is_enabled() is likely to result in an error.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a light source
     *      feature.  Valid IDs can be found with the sbapi_get_light_source_features()
     *      function.
     * @param error_code (Ouput)  A pointer to an integer that can be used for
     *      storing error codes.
     * @param light_source_index (Input) Which of potentially many light sources
     *      (LEDs, lasers, light bulbs) within the indicated feature instance to query
     *
     * @return 0 to indicate specified light source cannot be enabled/disabled,
     *         1 to indicate specified light source can be enabled/disabled with
     *                       sbapi_light_source_set_enable()
     */
    DLL_DECL unsigned char
    sbapi_light_source_has_enable(long deviceID, long featureID, int *error_code,
            int light_source_index);

    /**
     * Queries whether the indicated light source within the given feature
     * instance is enabled (energized).
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a light source feature.  Valid
     *        IDs can be found with the sbapi_get_light_source_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param light_source_index (Input) Which of potentially many light sources (LEDs, lasers,
     *      light bulbs) within the indicated feature instance to query
     *
     * @return 0 to indicate specified light source is disabled (should emit no light),
     *         1 to indicate specified light source is enabled (should emit light depending
     *                       on configured intensity setting)
     */
    DLL_DECL unsigned char
    sbapi_light_source_is_enabled(long deviceID, long featureID, int *error_code,
            int light_source_index);

    /**
     * Attempts to enable or disable the indicated light source within the given
     * feature instance.  Not all light sources have an enable/disable control,
     * and this capability can be queried with sbapi_light_source_has_enable().
     * Note that an enabled light source should emit light according to its last
     * (or default) intensity setting which might be the minimum; in this case,
     * the light source might appear to remain off.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a light source feature.  Valid
     *        IDs can be found with the sbapi_get_light_source_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param light_source_index (Input) Which of potentially many light sources (LEDs, lasers,
     *      light bulbs) within the indicated feature instance to query
     * @param enable (Input) Whether to enable the light source.  A value of zero will attempt
     *      to disable the light source, and any other value will enable it.
     */
    DLL_DECL void
    sbapi_light_source_set_enable(long deviceID, long featureID, int *error_code,
            int light_source_index, unsigned char enable);

    /**
     * Queries whether the indicated light source within the given feature
     * instance has a usable intensity control.  If this returns 0
     * (meaning no control available) then calling sbapi_light_source_set_intensity()
     * or sbapi_light_source_get_intensity() is likely to result in an error.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a light source feature.  Valid
     *        IDs can be found with the sbapi_get_light_source_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param light_source_index (Input) Which of potentially many light sources (LEDs, lasers,
     *      light bulbs) within the indicated feature instance to query
     *
     * @return 0 to indicate specified light source cannot have its intensity changed,
     *         1 to indicate the specified light source can have its intensity controlled
     *                       with sbapi_light_source_set_intensity()
     */
    DLL_DECL unsigned char
    sbapi_light_source_has_variable_intensity(long deviceID, long featureID,
        int *error_code, int light_source_index);

    /**
     * Queries the intensity level of the indicated light source within the
     * given feature instance.  The intensity is normalized over the
     * range [0, 1], with 0 as the minimum and 1 as the maximum.
     *
     * SAFETY WARNING: a light source at its minimum intensity (0) might still
     * emit light, and in some cases, this may be harmful radiation.  A value
     * of 0 indicates the minimum of the programmable range for the light source,
     * and does not necessarily turn the light source off.  To disable a light
     * source completely, use sbapi_light_source_set_enable() if the device
     * supports this feature, or provide some other mechanism to allow the light
     * to be disabled or blocked by the operator.
     *
     * In some cases, the intensity may refer to the duty cycle of a pulsed
     * light source instead of a continuous power rating.  The actual power output
     * of the light source might not vary linearly with the reported intensity,
     * so independent measurement or calibration of the light source may be
     * necessary.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a light source feature.  Valid
     *        IDs can be found with the sbapi_get_light_source_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param light_source_index (Input) Which of potentially many light sources (LEDs, lasers,
     *      light bulbs) within the indicated feature instance to query
     *
     * @return Real-valued result (as a double-precision floating point number) over
     *  the range [0, 1] where 0 represents the minimum programmable intensity
     *  level and 1 indicates the maximum.  Note that the minimum intensity level
     *  might still allow the light source to produce light.
     */
    DLL_DECL double
    sbapi_light_source_get_intensity(long deviceID, long featureID,
        int *error_code, int light_source_index);

    /**
     * Sets the intensity level of the indicated light source within the
     * given feature instance.  The intensity is normalized over the
     * range [0, 1], with 0 as the minimum and 1 as the maximum.
     *
     * SAFETY WARNING: a light source at its minimum intensity (0) might still
     * emit light, and in some cases, this may be harmful radiation.  A value
     * of 0 indicates the minimum of the programmable range for the light source,
     * and does not necessarily turn the light source off.  To disable a light
     * source completely, use sbapi_light_source_set_enable() if the device
     * supports this feature, or provide some other mechanism to allow the light
     * to be disabled or blocked by the operator.
     *
     * In some cases, the intensity may refer to the duty cycle of a pulsed
     * light source instead of a continuous power rating.  The actual power output
     * of the light source might not vary linearly with the reported intensity,
     * so independent measurement or calibration of the light source may be
     * necessary.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a light source feature.  Valid
     *        IDs can be found with the sbapi_get_light_source_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param light_source_index (Input) Which of potentially many light sources (LEDs, lasers,
     *      light bulbs) within the indicated feature instance to query
     * @param intensity (Input) The target intensity of the light source in the range [0, 1]
     */
    DLL_DECL void
    sbapi_light_source_set_intensity(long deviceID, long featureID,
        int *error_code, int light_source_index, double intensity);

    /*
     * This function returns the total number of strobe/lamp instances available
     * in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of strobe/lamp features that will be returned
     *      by a call to sbapi_get_strobe_lamp_features().
     */
     
// There is not code in seabreezeAPI.cpp for this function. Perhaps it was not finished
     
    //DLL_DECL int
    //sbapi_get_number_of_strobe_lamp_features(long deviceID, int *error_code);

    /*
     * This function returns IDs for accessing each strobe/lamp instance for this
     * device.  The IDs are only valid when used with the deviceID used to
     * obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) a preallocated array to hold returned feature handles
     * @param max_features (Input) length of the preallocated buffer
     *
     * @return the number of strobe/lamp feature IDs that were copied.
     */
     
// There is not code in seabreezeAPI.cpp for this function. Perhaps it was not finished.
    
    //DLL_DECL int
    //sbapi_get_strobe_lamp_features(long deviceID, int *error_code, long *features,
    //        int max_features);

    /**
     * This function returns the total number of lamp instances available
     * in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of lamp features that will be returned
     *      by a call to sbapi_get_lamp_features().
     */
    DLL_DECL int
    sbapi_get_number_of_lamp_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each lamp instance for this
     * device.  The IDs are only valid when used with the deviceID used to
     * obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) a preallocated array to hold returned feature handles
     * @param max_features (Input) length of the preallocated buffer
     *
     * @return the number of lamp feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_lamp_features(long deviceID, int *error_code, long *features,
            int max_features);

    /**
     * This function sets the strobe enable on the spectrometer.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a lamp feature.  Valid
     *        IDs can be found with the sbapi_get_lamp_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param lamp_enable (Input) A character used for denoting the desired value
     *      (high/low) of the strobe-enable pin.   If the value of
     *      strobe_enable is zero, then the pin should be set low.  If
     *      the value of strobe_enable is non-zero, then the pin should be
     *      set high.
     */
    DLL_DECL void
    sbapi_lamp_set_lamp_enable(long deviceID, long featureID,
            int *error_code, unsigned char lamp_enable);

    /**
     * This function returns the total number of continuous strobe instances
     * available in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of continuous strobe features that will be returned
     *      by a call to sbapi_get_continuous_strobe_features().
     */
    DLL_DECL int
    sbapi_get_number_of_continuous_strobe_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each continuous strobe instance
     * for this device.  The IDs are only valid when used with the deviceID
     * used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) a preallocated array to hold returned feature handles
     * @param max_features (Input) length of the preallocated buffer
     *
     * @return the number of continuous strobe feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_continuous_strobe_features(long deviceID, int *error_code, long *features,
            int max_features);

    /**
     * This function sets the continuous strobe enable state on the device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a continuous strobe feature.
     *        Valid IDs can be found with the sbapi_get_continuous_strobe_features()
     *        function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *        error codes.
     * @param enable (Input) A boolean used for denoting the desired state
     *        (on/off) of the continuous strobe generator.   If the value of
     *        enable is nonzero, then the continuous strobe will operate.  If
     *        the value of enable is zero, then the continuous strobe will stop.
     *        Note that on some devices the continuous strobe enable is tied to other
     *        enables (such as lamp enable or single strobe enable) which may cause
     *        side effects.
     */
    DLL_DECL void
    sbapi_continuous_strobe_set_continuous_strobe_enable(long deviceID, long featureID,
            int *error_code, unsigned char enable);

    /**
     * This function sets the continuous strobe period on the device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a continuous strobe feature.
     *        Valid IDs can be found with the sbapi_get_continuous_strobe_features()
     *        function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param period_micros (Input) The new period of the continous strobe measured in microseconds
     */
    DLL_DECL void
    sbapi_continuous_strobe_set_continuous_strobe_period_micros(long deviceID, long featureID,
            int *error_code, unsigned long period_micros);

    /**
     * This function returns the total number of EEPROM instances available
     * in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of EEPROM features that will be returned
     *  by a call to sbapi_get_eeprom_features().
     */
    DLL_DECL int
    sbapi_get_number_of_eeprom_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each EEPROM instance for this
     * device.  The IDs are only valid when used with the deviceID used to
     * obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) a preallocated array to hold returned feature handles
     * @param max_features (Input) length of the preallocated buffer
     *
     * @return the number of EEPROM feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_eeprom_features(long deviceID, int *error_code, long *features,
            int max_features);

    /**
     * This function reads a string out of the device's EEPROM slot
     * and returns the result.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of an EEPROM feature.  Valid
     *        IDs can be found with the sbapi_get_eeprom_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param slot_number (Input) The number of the slot to read out.  Possible
     *      values are 0 through 17.
     * @param buffer (Output)  A buffer (with memory already allocated) to hold the
     *      value read out of the EEPROM slot
     * @param buffer_length (Input) The length of the buffer
     *
     * @return the number of bytes read from the EEPROM slot into the buffer
     */
    DLL_DECL int
    sbapi_eeprom_read_slot(long deviceID, long featureID,
            int *error_code, int slot_number,
            unsigned char *buffer, int buffer_length);

    /**
     * This function returns the total number of irradiance calibration
     * instances available in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of irradiance calibration features that will be
     *      returned by a call to sbapi_get_irrad_cal_features().
     */
    DLL_DECL int
    sbapi_get_number_of_irrad_cal_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each irradiance calibration
     * instance for this device.  The IDs are only valid when used with the
     * deviceID used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) a preallocated array to hold returned feature handles
     * @param max_features (Input) length of the preallocated buffer
     *
     * @return the number of irradiance calibration feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_irrad_cal_features(long deviceID, int *error_code, long *features,
            int max_features);

    /**
     * This function reads out an irradiance calibration from the spectrometer's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of an irradiance calibration
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_irrad_cal_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param buffer (Output) preallocated array to hold irradiance calibration scalars (one per pixel)
     * @param buffer_length (Input) size of the preallocated buffer (should equal pixel count)
     *
     * @return the number of floats read from the device into the buffer
     */
    DLL_DECL int
    sbapi_irrad_calibration_read(long deviceID, long featureID,
            int *error_code, float *buffer, int buffer_length);

    /**
     * This function writes an irradiance calibration to the device's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of an irradiance calibration
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_irrad_cal_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param buffer (Output) array of floating point values to store into the device
     * @param buffer_length (Input) number of calibration factors to write
     *
     * @return the number of floats written from the buffer to the device
     */
    DLL_DECL int
    sbapi_irrad_calibration_write(long deviceID, long featureID,
            int *error_code, float *buffer, int buffer_length);

    /**
     * This function checks for an irradiance collection area in the device's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of an irradiance calibration
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_irrad_cal_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return 0 if no collection area available, 1 if available.
     */
    DLL_DECL int
    sbapi_irrad_calibration_has_collection_area(long deviceID, long featureID,
            int *error_code);

    /**
     * This function reads an irradiance collection area from the device's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of an irradiance calibration
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_irrad_cal_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return float: collection area (usually in units of cm^2) read from device
     */
    DLL_DECL float
    sbapi_irrad_calibration_read_collection_area(long deviceID, long featureID,
            int *error_code);

    /**
     * This function writes an irradiance collection area to the spectrometer's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of an irradiance calibration
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_irrad_cal_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param area (Input) collection area to save to spectrometer, presumably in cm^2
     */
    DLL_DECL void
    sbapi_irrad_calibration_write_collection_area(long deviceID, long featureID,
            int *error_code, float area);

    /**
     * This function returns the total number of thermoelectric cooler (TEC)
     * instances available in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of TEC features that will be returned by a call to
     *      sbapi_get_thermoelectric_features().
     */
    DLL_DECL int
    sbapi_get_number_of_thermo_electric_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each thermoelectric cooler
     * (TEC) instance for this device.  The IDs are only valid when used with
     * the deviceID used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) preallocated array to hold feature handles
     * @param max_features (Input) size of the preallocated array
     *
     * @return the number of TEC feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_thermo_electric_features(long deviceID, int *error_code, long *features,
            int max_features);

    /**
     * This function reads the actual temperature of the TEC and returns the value in
     * degrees celsius.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of an thermoelectric cooler
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_thermo_electric_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return double: The TEC temperature in degrees celsius.
     */
    DLL_DECL double
    sbapi_tec_read_temperature_degrees_C(long deviceID, long featureID,
            int *error_code);

    /**
     * This function sets the target (setpoint) TEC temperature.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of an thermoelectric cooler
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_thermo_electric_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param temperature_degrees_celsius (Input) desired temperature,
     *      in degrees celsius.
     */
    DLL_DECL void
    sbapi_tec_set_temperature_setpoint_degrees_C(long deviceID, long featureID,
            int *error_code, double temperature_degrees_celsius);

    /**
     * This function enables the TEC feature on the device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of an thermoelectric cooler
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_thermo_electric_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param tec_enable (Input) A character that denotes the desired TEC enable
     *      state.  If the value of tec_enable is zero, the TEC should
     *      be disabled.  If the value of tec_enable is non-zero, the TEC
     *      should be enabled.
     */
    DLL_DECL void
    sbapi_tec_set_enable(long deviceID, long featureID,
            int *error_code, unsigned char tec_enable);

    /**
     * This function returns the total number of nonlinearity coefficient feature
     * instances available in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of features that will be returned by a call to
     *      sbapi_get_nonlinearity_coeffs_features().
     */
    DLL_DECL int
    sbapi_get_number_of_nonlinearity_coeffs_features(
            long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each nonlinearity coefficient
     * feature instance for this device.  The IDs are only valid when used with
     * the deviceID used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) preallocated array to hold returned feature handles
     * @param max_features (Input) size of preallocated array
     *
     * @return the number of nonlinearity coefficient feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_nonlinearity_coeffs_features(long deviceID, int *error_code,
            long *features, int max_features);

    /**
     * This function reads out nonlinearity coefficients from the device's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a nonlinearity coefficient
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_nonlinearity_coeffs_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param buffer (Output) preallocated buffer to store NLC coefficients
     * @param max_length (Input) size of preallocated buffer
     *
     * @return the number of doubles read from the device into the buffer
     */
    DLL_DECL int sbapi_nonlinearity_coeffs_get(long deviceID, long featureID,
            int *error_code, double *buffer, int max_length);

/**
     * This function returns the total number of temperature feature
     * instances available in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of features that will be returned by a call to
     *      sbapi_get_temperature_features().
     */
    DLL_DECL int
    sbapi_get_number_of_temperature_features(
            long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each temperature
     * feature instance for this device.  The IDs are only valid when used with
     * the deviceID used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) preallocated array to hold returned feature handles
     * @param max_features (Input) size of preallocated array
     *
     * @return the number of temperature feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_temperature_features(long deviceID, int *error_code,
            long *temperatureFeatures, int max_features);

    /**
     * This function reads out an the number of indexed temperatures available from the
     *  device's internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a temperature
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_temperature_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of temperatures available as an unsigned char 
     */
    DLL_DECL unsigned char sbapi_temperature_count_get(long deviceID, long temperatureFeatureID, int *error_code);

    /**
     * This function reads out an indexed temperature from the device's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a temperature
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_temperature_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param index (Input) An index for the device's temperature sensors
     *
     * @return the temperature as a double
     */
    DLL_DECL double sbapi_temperature_get(long deviceID, long temperatureFeatureID, int *error_code, int index);

    /**
     * This function reads out all temperatures from the device's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a temperature
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_temperature_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param buffer (Output) preallocated buffer to store temperatures
     * @param max_length (Input) size of preallocated buffer
     *
     * @return the number of doubles read from the device into the buffer
     */
    DLL_DECL int sbapi_temperature_get_all(long deviceID, long temperatureFeatureID, int *error_code, double *buffer, int max_length);



/**
     * This function returns the total number of spectrum processing feature
     * instances available in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of features that will be returned by a call to
     *      sbapi_get_spectrum_processing_features().
     */
    DLL_DECL int
    sbapi_get_number_of_spectrum_processing_features(
            long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each spectrum processing
     * feature instance for this device.  The IDs are only valid when used with
     * the deviceID used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) preallocated array to hold returned feature handles
     * @param max_features (Input) size of preallocated array
     *
     * @return the number of spectrum processing feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_spectrum_processing_features(long deviceID, int *error_code,
            long *spectrumProcessingFeatures, int max_features);

    /**
     * This function reads out an the number of scans to average from the
     *  device's internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a spectrum processing
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_spectrum_processing_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of scans to average as an unsigned short integer 
     */
    DLL_DECL unsigned short int sbapi_spectrum_processing_scans_to_average_get(long deviceID,
    	 long spectrumProcessingFeatureID, int *error_code);

    /**
     * This function sets the number of scans to average in the the device's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a spectrum processing
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_spectrum_processing_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param scansToAverage (Input) The number of spectrum scans used to generate a less
     *		noisy spectrum due to averaging
     *
     * @return void
     */
    DLL_DECL void sbapi_spectrum_processing_scans_to_average_set(long deviceID, 
    	long spectrumProcessingFeatureID, int *error_code, unsigned short int scansToAverage);

    /**
     * This function reads out an the width of the boxcar filter from the
     *  device's internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a spectrum processing
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_spectrum_processing_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the width of the boxcar filter an unsigned char (values typically 0-15)
     */
    DLL_DECL unsigned char sbapi_spectrum_processing_boxcar_width_get(long deviceID,
    	 long spectrumProcessingFeatureID, int *error_code);

    /**
     * This function sets width of the boxcar filter in the the device's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a spectrum processing
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_spectrum_processing_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param boxcarWidth (Input) The width of the boxcar smoothing function to be used.
     *			Values are typically 1 to 15.
     *
     * @return void
     */
    DLL_DECL void sbapi_spectrum_processing_boxcar_width_set(long deviceID, 
    		long spectrumProcessingFeatureID, int *error_code, unsigned char boxcarWidth);
   


/**
     * This function returns the total number of revision feature
     * instances available in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of features that will be returned by a call to
     *      sbapi_get_revision_features().
     */
    DLL_DECL int
    sbapi_get_number_of_revision_features(
            long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each revision
     * feature instance for this device.  The IDs are only valid when used with
     * the deviceID used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) preallocated array to hold returned feature handles
     * @param max_features (Input) size of preallocated array
     *
     * @return the number of revision feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_revision_features(long deviceID, int *error_code,
            long *revisionFeatures, int max_features);

    /**
     * This function reads out the hardware revision from the device's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a temperature
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_revision_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the hardware revision as one unsigned char byte. (Note that both Ocean View and SpectraSuite display the hex value.)
     */
    DLL_DECL unsigned char sbapi_revision_hardware_get(long deviceID, long revisionFeatureID, int *error_code);

    /**
     * This function reads out the firmware revision from the device's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a temperature
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_revision_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the firmware revision as two unsigned short int bytes (Note that both Ocean View and SpectraSuite display the hex value.)
     */
    DLL_DECL unsigned short int sbapi_revision_firmware_get(long deviceID, long revisionFeatureID, int *error_code);

/**
     * This function returns the total number of optical bench feature
     * instances available in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of features that will be returned by a call to
     *      sbapi_get_optical_bench_features().
     */
    DLL_DECL int
    sbapi_get_number_of_optical_bench_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each optical bench
     * feature instance for this device.  The IDs are only valid when used with
     * the deviceID used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) preallocated array to hold returned feature handles
     * @param max_features (Input) size of preallocated array
     *
     * @return the number of optical bench feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_optical_bench_features(long deviceID, int *error_code, long *opticalBenchFeatures, int max_features);

    /**
     * This function reads out the optical bench fiber diameter in microns
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param opticalBenchFeatureID (Input) The ID of a particular instance of a optical
     *        bench feature.  Valid IDs can be found with the
     *        sbapi_get_optical_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the fiber diameter in microns
     */
    DLL_DECL unsigned short int sbapi_optical_bench_get_fiber_diameter_microns(long deviceID, long opticalBenchFeatureID, int *error_code);

    /**
     * This function reads out the optical bench slit width in microns
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param opticalBenchFeatureID (Input) The ID of a particular instance of a optical
     *        bench feature.  Valid IDs can be found with the
     *        sbapi_get_optical_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the slit width in microns
     */
    DLL_DECL unsigned short int sbapi_optical_bench_get_slit_width_microns(long deviceID, long opticalBenchFeatureID, int *error_code);

    /**
     * This reads the optical bench ID and fills the
     * provided array (up to the given length) with it.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param opticalBenchFeatureID (Input) The ID of a particular instance of a serial
     *      number feature.  Valid IDs can be found with the
     *      sbapi_get_optical_bench_features() function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @param buffer (Output)  A pre-allocated array of characters that the
     *      serial number will be copied into
     * @param buffer_length (Input) The number of values to copy into the buffer
     *      (this should be no larger than the number of chars allocated in
     *      the buffer)
     *
     * @return the number of bytes written into the buffer
     */
    DLL_DECL int
    sbapi_optical_bench_get_id(long deviceID, long featureID, int *error_code, char *buffer, int buffer_length);
 
     /**
     * This reads the optical bench Serial Number and fills the
     * provided array (up to the given length) with it.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param opticalBenchFeatureID (Input) The ID of a particular instance of a serial
     *      number feature.  Valid IDs can be found with the
     *      sbapi_get_optical_bench_features() function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @param buffer (Output)  A pre-allocated array of characters that the
     *      serial number will be copied into
     * @param buffer_length (Input) The number of values to copy into the buffer
     *      (this should be no larger than the number of chars allocated in
     *      the buffer)
     *
     * @return the number of bytes written into the buffer
     */
    DLL_DECL int
    sbapi_optical_bench_get_serial_number(long deviceID, long featureID, int *error_code, char *buffer, int buffer_length);


    /**
     * This reads the optical bench Coating and fills the
     * provided array (up to the given length) with it.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param opticalBenchFeatureID (Input) The ID of a particular instance of a serial
     *      number feature.  Valid IDs can be found with the
     *      sbapi_get_optical_bench_features() function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @param buffer (Output)  A pre-allocated array of characters that the
     *      serial number will be copied into
     * @param buffer_length (Input) The number of values to copy into the buffer
     *      (this should be no larger than the number of chars allocated in
     *      the buffer)
     *
     * @return the number of bytes written into the buffer
     */
    DLL_DECL int
    sbapi_optical_bench_get_coating(long deviceID, long featureID, int *error_code, char *buffer, int buffer_length);
    
     /**
     * This reads the optical bench filter and fills the
     * provided array (up to the given length) with it.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param opticalBenchFeatureID (Input) The ID of a particular instance of a serial
     *      number feature.  Valid IDs can be found with the
     *      sbapi_get_optical_bench_features() function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @param buffer (Output)  A pre-allocated array of characters that the
     *      serial number will be copied into
     * @param buffer_length (Input) The number of values to copy into the buffer
     *      (this should be no larger than the number of chars allocated in
     *      the buffer)
     *
     * @return the number of bytes written into the buffer
     */
    DLL_DECL int
    sbapi_optical_bench_get_filter(long deviceID, long featureID, int *error_code, char *buffer, int buffer_length);
    
    /**
     * This reads the optical bench grating and fills the
     * provided array (up to the given length) with it.
     *
     * @param deviceID (Input) The index of a device previously opened with
     *      sbapi_open_device().
     * @param opticalBenchFeatureID (Input) The ID of a particular instance of a serial
     *      number feature.  Valid IDs can be found with the
     *      sbapi_get_optical_bench_features() function.
     * @param error_code (Output) pointer to an integer that can be used for
     *      storing error codes.
     * @param buffer (Output)  A pre-allocated array of characters that the
     *      serial number will be copied into
     * @param buffer_length (Input) The number of values to copy into the buffer
     *      (this should be no larger than the number of chars allocated in
     *      the buffer)
     *
     * @return the number of bytes written into the buffer
     */
    DLL_DECL int
    sbapi_optical_bench_get_grating(long deviceID, long featureID, int *error_code, char *buffer, int buffer_length);
    
                 
    /**
     * This function returns the total number of stray light coefficient feature
     * instances available in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of features that will be returned by a call to
     *      sbapi_get_stray_light_coeffs_features().
     */
    DLL_DECL int
    sbapi_get_number_of_stray_light_coeffs_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each stray light coefficient
     * feature instance for this device.  The IDs are only valid when used with
     * the deviceID used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) preallocated buffer to hold returned feature handles
     * @param max_features (Input) size of preallocated buffer
     *
     * @return the number of stray light coefficient feature IDs that were copied.
     */
    DLL_DECL int
    sbapi_get_stray_light_coeffs_features(long deviceID, int *error_code,
            long *features, int max_features);

    /**
     * This function reads out stray light coefficients from the device's
     * internal memory if that feature is supported.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a stray light coefficient
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_stray_light_coeffs_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param buffer (Output) preallocated buffer to store stray light coefficients
     * @param max_length (Input) size of preallocated buffer
     *
     * @return the number of doubles read from the device into the buffer
     */
    DLL_DECL int sbapi_stray_light_coeffs_get(long deviceID, long featureID,
            int *error_code, double *buffer, int max_length);


    /**
     * This function returns the total number of data buffer feature
     * instances available in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of features that will be returned by a call to
     *      sbapi_get_data_buffer_features().
     */
    DLL_DECL int sbapi_get_number_of_data_buffer_features(long deviceID, int *error_code);

    /**
     * This function returns IDs for accessing each data buffer
     * feature instance for this device.  The IDs are only valid when used with
     * the deviceID used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) preallocated buffer to hold returned feature handles
     * @param max_features (Input) size of preallocated buffer
     *
     * @return the number of data buffer feature IDs that were copied.
     */
    DLL_DECL int sbapi_get_data_buffer_features(long deviceID, int *error_code,
            long *buffer, unsigned int maxLength);

    /**
     * @brief Clear the data buffer
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a data buffer
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_data_buffer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *        error codes.
     */
    DLL_DECL void sbapi_data_buffer_clear(long deviceID, long featureID, int *error_code);

    /**
     * @brief Get the number of data elements currently in the buffer
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a data buffer
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_data_buffer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *        error codes.
     * @return A count of how many items are available for retrieval from the buffer
     */
    DLL_DECL unsigned long sbapi_data_buffer_get_number_of_elements(long deviceID,
            long featureID, int *error_code);

    /**
     * @brief Get the present limit of how many data elements will be retained by the buffer.
     *        This value can be changed with sbapi_data_buffer_set_buffer_capacity().
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a data buffer
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_data_buffer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *        error codes.
     * @return A count of how many items the buffer will store before data may be lost
     */
    DLL_DECL unsigned long sbapi_data_buffer_get_buffer_capacity(long deviceID,
            long featureID, int *error_code);

    /**
     * @brief Get the maximum possible configurable size for the data buffer
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a data buffer
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_data_buffer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *        error codes.
     * @return The largest value that may be set with sbapi_data_buffer_set_buffer_capacity().
     */
    DLL_DECL unsigned long sbapi_data_buffer_get_buffer_capacity_maximum(
            long deviceID, long featureID, int *error_code);

    /**
     * @brief Get the minimum possible configurable size for the data buffer
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a data buffer
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_data_buffer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *        error codes.
     * @return The smallest value that may be set with sbapi_data_buffer_set_buffer_capacity().
     */
    DLL_DECL unsigned long sbapi_data_buffer_get_buffer_capacity_minimum(
            long deviceID, long featureID, int *error_code);

    /**
     * @brief Set the number of data elements that the buffer should retain
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a data buffer
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_data_buffer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *        error codes.
     * @param capacity (Input) Limit on the number of data elements to store.  This is
     *        bounded by what is returned by sbapi_data_buffer_get_buffer_capacity_minimum() and
     *        sbapi_data_buffer_get_buffer_capacity_maximum().
     */
    DLL_DECL void sbapi_data_buffer_set_buffer_capacity(long deviceID,
            long featureID, int *error_code, unsigned long capacity);

    /**
     * This function returns the total number of acquisition delay feature
     * instances available in the indicated device.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     *
     * @return the number of features that will be returned by a call to
     *      sbapi_get_data_buffer_features().
     */
    DLL_DECL int sbapi_get_number_of_acquisition_delay_features(long deviceID, int *errorCode);

    /**
     * This function returns IDs for accessing each data buffer
     * feature instance for this device.  The IDs are only valid when used with
     * the deviceID used to obtain them.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *      error codes.
     * @param features (Output) preallocated buffer to hold returned feature handles
     * @param max_features (Input) size of preallocated buffer
     *
     * @return the number of data buffer feature IDs that were copied.
     */
    DLL_DECL int sbapi_get_acquisition_delay_features(long deviceID, int *errorCode,
            long *buffer, unsigned int maxLength);

    /**
     * Set the acquisition delay in microseconds.  This may also be referred to as the
     * trigger delay.  In any event, it is the time between some event (such as a request
     * for data, or an external trigger pulse) and when data acquisition begins.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a data buffer
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_data_buffer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *        error codes.
     * @param delay_usec (Input) The new delay to use in microseconds
     */
    DLL_DECL void sbapi_acquisition_delay_set_delay_microseconds(long deviceID, long featureID,
            int *errorCode, unsigned long delay_usec);

    /**
     * Get the acquisition delay in microseconds.  This may also be referred to as the
     * trigger delay.  In any event, it is the time between some event (such as a request
     * for data, or an external trigger pulse) and when data acquisition begins.
     *
     * Note that not all devices support reading this value back.  In these cases, the
     * returned value will be the last value sent to sbapi_acquisition_delay_set_delay_microseconds().
     * If no value has been set and the value cannot be read back, this function will
     * indicate an error.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a data buffer
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_data_buffer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *        error codes.
     * @return The acquisition delay in microseconds
     */
    DLL_DECL unsigned long sbapi_acquisition_delay_get_delay_microseconds(long deviceID,
            long featureID, int *errorCode);

    /**
     * Get the allowed step size for the acquisition delay in microseconds.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a data buffer
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_data_buffer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *        error codes.
     * @return The acquisition delay step size in microseconds
     */
    DLL_DECL unsigned long sbapi_acquisition_delay_get_delay_increment_microseconds(long deviceID,
            long featureID, int *errorCode);

    /**
     * Get the maximum allowed acquisition delay in microseconds.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a data buffer
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_data_buffer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *        error codes.
     * @return The maximum acquisition delay in microseconds
     */
    DLL_DECL unsigned long sbapi_acquisition_delay_get_delay_maximum_microseconds(long deviceID,
            long featureID, int *errorCode);

    /**
     * Get the minimum allowed acquisition delay in microseconds.
     *
     * @param deviceID (Input) The index of a device previously opened with sbapi_open_device().
     * @param featureID (Input) The ID of a particular instance of a data buffer
     *        feature.  Valid IDs can be found with the
     *        sbapi_get_data_buffer_features() function.
     * @param error_code (Output) A pointer to an integer that can be used for storing
     *        error codes.
     * @return The minimum acquisition delay in microseconds
     */
    DLL_DECL unsigned long sbapi_acquisition_delay_get_delay_minimum_microseconds(long deviceID,
            long featureID, int *errorCode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SEABREEZEAPI_H */
