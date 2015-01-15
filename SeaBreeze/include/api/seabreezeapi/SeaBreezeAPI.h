/***************************************************//**
 * @file    SeaBreezeAPI.h
 * @date    February 2012
 * @author  Ocean Optics, Inc.
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

    /* Serial number capabilities */
    int getNumberOfSerialNumberFeatures(long deviceID, int *errorCode);
    int getSerialNumberFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    int getSerialNumber(long deviceID, long featureID, int *errorCode, char *buffer, int bufferLength);

    /* Spectrometer capabilities */
    int getNumberOfSpectrometerFeatures(long id, int *errorCode);
    int getSpectrometerFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    void spectrometerSetTriggerMode(long deviceID, long featureID, int *errorCode, int mode);
    void spectrometerSetIntegrationTimeMicros(long deviceID, long featureID, int *errorCode, unsigned long integrationTimeMicros);
    unsigned long spectrometerGetMinimumIntegrationTimeMicros(long deviceID, long featureID, int *errorCode);
    int spectrometerGetUnformattedSpectrumLength(long deviceID, long featureID, int *errorCode);
    int spectrometerGetUnformattedSpectrum(long deviceID, long featureID, int *errorCode, unsigned char *buffer, int bufferLength);
    int spectrometerGetFormattedSpectrumLength(long deviceID, long featureID, int *errorCode);
    int spectrometerGetFormattedSpectrum(long deviceID, long featureID, int *errorCode, double *buffer, int bufferLength);
    int spectrometerGetWavelengths(long deviceID, long featureID, int *errorCode, double *wavelengths, int length);
    int spectrometerGetElectricDarkPixelCount(long deviceID, long featureID, int *errorCode);
    int spectrometerGetElectricDarkPixelIndices(long deviceID, long featureID, int *errorCode, int *indices, int length);

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

    /* Stray light coefficient capabilities */
    int getNumberOfStrayLightCoeffsFeatures(long deviceID, int *errorCode);
    int getStrayLightCoeffsFeatures(long deviceID, int *errorCode, long *buffer, unsigned int maxLength);
    int strayLightCoeffsGet(long deviceID, long featureID, int *errorCode, double *buffer, int maxLength);

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
     * found on a particular RS232 bus (e.g. a COM port).  The driver will add
     * the device type and location to the set of those that can be opened.
     *
     * @param deviceTypeName (Input) The name of a type of device.  This can be
     *      one of the following:
     *          HR2000, HR2000Plus, HR4000, Jaz, Maya2000, Maya2000Pro,
     *          MayaLSL, NIRQuest256, NIRQuest512, QE65000, STS, Torus, USB2000,
     *          USB2000Plus, or USB4000.
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

    /**
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
    DLL_DECL int
    sbapi_get_number_of_strobe_lamp_features(long deviceID, int *error_code);

    /**
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
    DLL_DECL int
    sbapi_get_strobe_lamp_features(long deviceID, int *error_code, long *features,
            int max_features);

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


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SEABREEZEAPI_H */
