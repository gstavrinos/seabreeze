/***************************************************//**
 * @file    Spectrometer.h
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * This class provides the functionality associated with a connected spectrometer. It accepts commands
 * and arguments from a request handler (class RequestHandler or a class derived from RequestHandler)
 * and uses an active object (see e.g. http://www.drdobbs.com/parallel/prefer-using-active-objects-instead-of-n/225700095)
 * to run the associated member methods in a separate thread.
 *
 * LICENSE:
 *
 * Dev Kit Copyright (C) 2015, Ocean Optics Inc
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

#ifndef SPECTROMETER_H
#define SPECTROMETER_H

// For the benefit of VC++ which doesn't like throw specifications
#pragma warning (disable : 4290)

#include "Connection.h"
#include "ActiveObject.h"
#include <vector>
#include <map>
#include <utility>
#include <boost/asio.hpp>

class SeaBreezeAPI;
class IResponseHandler;
class RequestHandlerConfiguration;

class Spectrometer {

public:
    // we map the received commands to the member methods using CommandValue
    enum CommandValue {
        // RequestHandler commands
        SET_INTEGRATION_TIME = 0x01,
        GET_INTEGRATION_TIME,
        SET_BOXCAR_WIDTH,
        GET_BOXCAR_WIDTH,
        SET_SCANS_TO_AVERAGE,
        GET_SCANS_TO_AVERAGE,
        //SET_TARGET_URL, -- now deprecated
        //GET_TARGET_URL, -- now deprecated
        GET_SPECTRUM = 0x09,
        GET_WAVELENGTHS,
        GET_SERIAL_NUMBER,
        GET_NAME,
        //GET_VERSION, // now intercepted by OceanHandler
        GET_CALIBRATION_COEFFICIENTS_FROM_BUFFER = 0x0E,
        SET_CALIBRATION_COEFFICIENTS_TO_BUFFER,
        GET_CALIBRATION_COEFFICIENTS_FROM_EEPROM,
        SET_CALIBRATION_COEFFICIENTS_TO_EEPROM,
        GET_PIXEL_BINNING_FACTOR,
        SET_PIXEL_BINNING_FACTOR,
        GET_INTEGRATION_TIME_MINIMUM,
        GET_INTEGRATION_TIME_MAXIMUM,
        GET_INTENSITY_MAXIMUM,
        GET_ELECTRIC_DARK_CORRECTION,
        SET_ELECTRIC_DARK_CORRECTION,
        GET_SPECTROMETER_STATUS,
        SET_TEC_ENABLE,
        SET_TEC_TEMPERATURE,
        GET_TEC_TEMPERATURE,
        SET_LAMP_ENABLE,
        GET_DEFAULT_PIXEL_BINNING_FACTOR,
        SET_DEFAULT_PIXEL_BINNING_FACTOR,
        GET_MAX_PIXEL_BINNING_FACTOR,

        LAST_COMMAND_VALUE
    };
        
    // The result of the command(s)
    enum ResultCode {
        SUCCESS = 1,
        SPECTROMETER_INTERFACE_NOT_ESTABLISHED,
        UNABLE_TO_OPEN_SPECTROMETER,
        UNABLE_TO_CLOSE_SPECTROMETER,
        SPECTROMETER_ERROR,
        UNABLE_TO_PARSE_PARAMETER,
        UNABLE_TO_DETERMINE_SPECTRUM_LENGTH,
        INTEGRATION_TIME_TOO_LOW,
        INVALID_INTEGRATION_TIME,
        INVALID_BOXCAR_WIDTH,
        INVALID_SCANS_TO_AVERAGE,
        INVALID_CALIBRATION_COEFFICIENTS,
        INVALID_PIXEL_BINNING_FACTOR,
        INVALID_ELECTRIC_DARK,
        UNABLE_TO_GET_SPECTRUM,
        UNABLE_TO_ENCODE_SPECTRUM,
        UNABLE_TO_GET_WAVELENGTHS,
        UNABLE_TO_ENCODE_WAVELENGTHS,
        UNABLE_TO_SET_SHUTTER_POSITION,
        SPECTROMETER_INDEX_DOES_NOT_EXIST,
        INVALID_TEC_ENABLE,
        INVALID_LAMP_ENABLE,

        LAST_RESULT_CODE
    };

    // Public convenience typedefs
    typedef unsigned short CommandType;
    typedef std::pair<int, std::string> Response;

    // The message class we will use to pass commands to the active object thread
    class CommandMessage : public ActiveObject::Message {
    public:
        void Execute();
        CommandMessage(boost::shared_ptr<Connection> connection,
            Spectrometer *spectrometer,
            CommandType command,
            const std::string &arguments);
    private:
        boost::shared_ptr<Connection> m_connection;
        CommandType m_command;
        Spectrometer *m_spectrometer;
        std::string m_arguments;
    };

    /* Get a spectrum. This method is a response handler.
    */
    Response GetSpectrum(const std::string &arguments);

    /* Submit a message (i.e. a task) to the active object
    */
    void Submit(ActiveObject::MessagePtr p);

    /* Accept a command with optional arguments. The connection is also supplied in order to reply with the response value.
    *  Return true if the command is successfully handled, false otherwise.
    */
    virtual bool Accept(boost::shared_ptr<Connection> connection, const CommandType command, const std::string &arguments);
    
    /* Return true if the command is a known (i.e.. mapped) command, false otherwise
    */
    bool IsValidCommand(const CommandType command);

    /* Execute the handler associated with the command, supplied with the spcified arguments.
    */
    Response HandleCommand(const CommandType command, const std::string &arguments);

    /* Return the response to the command on the same connection
    */
    void HandleResponse(boost::shared_ptr<Connection> connection, const int resultCode, const std::string &response);

    /* Handle an error condition
    */
    void HandleError(boost::shared_ptr<Connection> connection);

    /* Return the serial number of hte spectrometer
    */
    std::string SerialNumber() {return m_serialNumber;}

    /* Return the spectrometer type (model)
    */
    std::string Type() {return m_name;}

    /* Return the cached minimum integration time
    */
    long MinIntegrationTime() {return m_minIntegrationTime;}

    /* Return the cached maximum integration time
    */
    long MaxIntegrationTime() {return m_maxIntegrationTime;}

    /* Return the wavelengths associated with each pixel
    */
    std::vector<double> GetWavelengths() {return m_wavelengths;}

    /* Return the currently acquired spectrum
    */
    std::vector<double> GetSpectrum();

    /* Get the currently acquired spectrum into an existing vector
    */
    void GetSpectrum(std::vector<double>& spectrum, int &error);

    /* Return the cached integration time.
    */
    long IntegrationTime() {return m_integrationTime;}

    /* Return the cached current scans to average
    */
    unsigned short ScansToAverage() {return m_scansToAverage;}

    /* Return the cached boxcar width
    */
    unsigned char BoxcarWidth() {return m_boxcarWidth;}

    /* return the number of pixels in a spectrum
    */
    int PixelCount() {return m_pixelCount;}

    /* Return the number of binned pixels
    */
    unsigned int BinnedPixelCount() {return m_binnedPixelCount;}

    long Handle() const {return m_handle;}

    /* Constructor with the response handler, index of the spectrometer assigned by the SeaBreeze wrapper
    *  and a pointer to the SeaBreeze wrapper to execute the commands
    */
    Spectrometer(IResponseHandler *reponseHandler, const long handle, SeaBreezeAPI *seabreezeApi, RequestHandlerConfiguration &config);
    virtual ~Spectrometer();

private:

    // Private convenience typedefs
    typedef Response (Spectrometer::*Command)(const std::string &arguments);
    typedef std::map<CommandType, Command> CommandMap;
    typedef CommandMap::value_type Handler;

    // The handlers for the received commands

    // Get/set the current integration time
    Response GetIntegrationTime(const std::string &arguments);
    Response SetIntegrationTime(const std::string &arguments);

    // Get/set the current boxcar width
    Response GetBoxcarWidth(const std::string &arguments);
    Response SetBoxcarWidth(const std::string &arguments);

    // Get/set the current scans to average
    Response GetScansToAverage(const std::string &arguments);
    Response SetScansToAverage(const std::string &arguments);

    // Get the (possibly binned) wavelengths
    Response GetWavelengths(const std::string &arguments);

    // Apply electric dark correction
    void CorrectForElectricDark(std::vector<double> &spectrum, const size_t numPixels);

    // Apply boxcar smoothing
    void ApplyBoxcar(std::vector<double> &spectrum, const size_t numPixels);

    // Get the serial number
    Response GetSerialNumber(const std::string &arguments);

    // Get the spectrometer type name
    Response GetName(const std::string &arguments);

    // Get/set the buffered calibration coefficients
    Response GetBufferCalibrationCoefficients(const std::string &arguments);
    Response SetBufferCalibrationCoefficients(const std::string &arguments);

    // Get/set the calibration coefficients in the EEPROM
    Response GetEepromCalibrationCoefficients(const std::string &arguments);
    Response SetEepromCalibrationCoefficients(const std::string &arguments);

    // Get/set the pixel binning factor where supported (default to zero otherwise)
    Response GetPixelBinningFactor(const std::string &arguments);
    Response SetPixelBinningFactor(const std::string &arguments);
    Response GetDefaultPixelBinningFactor(const std::string &arguments);
    Response SetDefaultPixelBinningFactor(const std::string &arguments);
    Response GetMaxPixelBinningFactor(const std::string &arguments);

    // Get the minimum integration time
    Response GetIntegrationTimeMinimum(const std::string &arguments);

    // Get the maximum integration time
    Response GetIntegrationTimeMaximum(const std::string &arguments);

    // Get the maximum intensity
    Response GetIntensityMaximum(const std::string &arguments);

    // Get/set the electric dark correction (where supported)
    Response GetElectricDarkCorrection(const std::string &arguments);
    Response SetElectricDarkCorrection(const std::string &arguments);

    Response SetTecEnable(const std::string &arguments);
    Response SetTecTemperature(const std::string &arguments);
    Response GetTecTemperature(const std::string &arguments);

    Response SetLampEnable(const std::string &arguments);

    // Get the current spectrometer status
    Response GetCurrentStatus(const std::string &arguments);

    // Convenience method to set up all of the spectrometer parameters on construction
    void InitializeParameters();

    // The map of integer commands we receive to the method that will handle the command
    CommandMap m_command;

    // Pointer to the object that will handle responses
    IResponseHandler *m_responseHandler;
    // The handle of the spectrometer as assigned by the SeaBreeze API
    const long m_handle;
    // Pointer to the SeaBreeze wrapper singleton
    SeaBreezeAPI *m_seabreezeApi;

    // The connection (socket) for the current connection we are receiving commands/parameters on - we will
    // send the response on this before closing it down
    boost::shared_ptr<Connection> m_connection;

    // Configuration for default values
    RequestHandlerConfiguration &m_configuration;

    // Integration time for this spectrometer
    long m_integrationTime;
    // Minimum integration time for this spectrometer
    long m_minIntegrationTime;
    // Maximum integration time for this spectrometer
    long m_maxIntegrationTime;
    // Cached boxcar smoothing width for this spectrometer
    unsigned char m_boxcarWidth;
    // Cached scans to average for this spectrometer
    unsigned short m_scansToAverage;
    // Binning factor for this spectrometer (not all spectrometers will use this)
    int m_binningFactor;
    // Flag for applying electric dark correction for this spectrometer (not all spectrometers will use this)
    bool m_applyElectricDark;
    // Indices of electric dark pixels for this spectrometer (not all spectrometers will use this)
    std::vector<int> m_electricDarkIndex;
    // Number of pixels for this spectrometer
    int m_pixelCount;
    // Binned pixel count for this spectrometer (not all spectrometers will use this)
    unsigned int m_binnedPixelCount;
    // The max intensity for this spectrometer
    int m_maxIntensity;
    // Current spectrum for this spectrometer
    std::vector<double> m_spectrum;
    // Wavelengths for this spectrometer
    std::vector<double> m_wavelengths;
    // The spectrometer type name
    std::string m_name;
    // The spectrometer serial number
    std::string m_serialNumber;
    
    // The maximum size of a spectrometer name
    static const int ms_maxSpectrometerNameCount = 64;
    // The maximumm size of a spectrometer serial number
    static const int ms_maxSerialNumberCount = 64;
    // The maximum number of electric dark pixels
    //static const int ms_maxElectricDarkPixels = 128;
    // The number of calibration coefficients
    static const int ms_coefficientCount = 14;
    // Calibration coefficients for this spectrometer
    double m_calibCoefficient[ms_coefficientCount];

    std::vector<long> m_spectrometerFeatures;
    std::vector<long> m_spectrumProcessingFeatures;
    //std::vector<long> m_opticalBenchFeatures;
    std::vector<long> m_serialNumberFeatures;
    std::vector<long> m_pixelBinningFeatures;
    std::vector<long> m_thermoElectricFeatures;
    std::vector<long> m_lampFeatures;

    int m_resultCode;
    // Map the result codes to meaningful strings
    static const std::map<int, std::string> ms_results;

    // not implemented
    Spectrometer(const Spectrometer&);
    Spectrometer& operator=(const Spectrometer&);

    // Declare the active object last to ensure correct destruction order
    // The active object that will give us the thread of execution for this spectrometer.
    ActiveObject m_activeObject;
};
#endif