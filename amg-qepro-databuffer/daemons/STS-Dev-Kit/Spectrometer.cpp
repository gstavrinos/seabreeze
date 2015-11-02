/***************************************************//**
 * @file    Spectrometer.cpp
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * Provides the functionality to accept a command/request for a spectrometer and return a response to the
 * response handler. An Active Object is used to queue requests on a separate thread to ensure we don't block
 * execution during e.g. long acquisitions caused by large integraiton times and/or scans to average.
 *
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

#include "Spectrometer.h"
#include "IResponseHandler.h"
#include <api/SeaBreezeWrapper.h>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <limits>

/* Submit a message (task) to the active object to be added to the execution queue.
*/
void Spectrometer::Submit(ActiveObject::MessagePtr p) {
    m_activeObject.Send(p);
}

/* Accept a command/request and if it is one this class can handle submit the task to be executed, otherwise handle it as an error.
*/
bool Spectrometer::Accept(boost::shared_ptr<Connection> connection, const CommandType command, const std::string &arguments) {
    bool result = false;
    if (IsValidCommand(command)) {
        m_connection = connection;
        ActiveObject::MessagePtr p(new CommandMessage(m_connection, this, command, arguments));
        Submit(p);
        result = true;
    }
    else {
        HandleError(connection);
    }
    return result;
}

/* Return true if the command is one this class can handle i.e. we have a member function mapped to the command value, false otherwise.
*/
bool Spectrometer::IsValidCommand(const CommandType command) {
    return m_command.find(command) != m_command.end();
}

/* If the command is one this class can handle then invoke the mapped member function and return the reponse,
* otherwise return an empty response.
*/
Spectrometer::Response Spectrometer::HandleCommand(const CommandType command, const std::string &arguments) {
    Response result = IsValidCommand(command) ? (this->*m_command.at(command))(arguments) : Response(0, "");
    return result;
}

/* Handle the response from a command/request and return the result on the same connection it was received on.
* This method is called by the active object on completion of a task.
*/
void Spectrometer::HandleResponse(boost::shared_ptr<Connection> connection, const int resultCode, const std::string &response) {
    m_responseHandler->NotifyResponse(connection, resultCode, response);
}

/* Handle an error in accepting/executing a request.
*/
void Spectrometer::HandleError(boost::shared_ptr<Connection> connection) {
    m_responseHandler->NotifyError(connection);
}

/* Return a spectrum from this spectrometer.
*/
std::vector<double> Spectrometer::GetSpectrum() {

    std::vector<double> spectrum(m_binnedPixelCount, 0.0);
    int error = 0;
    int actual = m_wrapper->getFormattedSpectrum(m_index, &error, spectrum.data(), m_binnedPixelCount);
    return spectrum;
}

/* Constructor. Map the command codes to member function handlers.
*/
Spectrometer::Spectrometer(IResponseHandler *responseHandler, const int index, SeaBreezeWrapper *wrapper) :
    m_responseHandler(responseHandler),
    m_index(index),
    m_wrapper(wrapper),
    m_minIntegrationTime(0),
    m_maxIntegrationTime(std::numeric_limits<long>::max()),
    m_boxcarWidth(0),
    m_scansToAverage(0) {

    InitializeParameters();

    m_command.insert(Handler(SET_INTEGRATION_TIME, &Spectrometer::SetIntegrationTime));
    m_command.insert(Handler(GET_INTEGRATION_TIME, &Spectrometer::GetIntegrationTime));
    m_command.insert(Handler(SET_BOXCAR_WIDTH, &Spectrometer::SetBoxcarWidth));
    m_command.insert(Handler(GET_BOXCAR_WIDTH, &Spectrometer::GetBoxcarWidth));
    m_command.insert(Handler(SET_SCANS_TO_AVERAGE, &Spectrometer::SetScansToAverage));
    m_command.insert(Handler(GET_SCANS_TO_AVERAGE, &Spectrometer::GetScansToAverage));
    m_command.insert(Handler(GET_SPECTRUM, &Spectrometer::GetSpectrum));
    m_command.insert(Handler(GET_WAVELENGTHS, &Spectrometer::GetWavelengths));
    m_command.insert(Handler(GET_SERIAL_NUMBER, &Spectrometer::GetSerialNumber));
    m_command.insert(Handler(GET_NAME, &Spectrometer::GetName));
    m_command.insert(Handler(GET_CALIBRATION_COEFFICIENTS_FROM_BUFFER, &Spectrometer::GetBufferCalibrationCoefficients));
    m_command.insert(Handler(SET_CALIBRATION_COEFFICIENTS_TO_BUFFER, &Spectrometer::SetBufferCalibrationCoefficients));
    m_command.insert(Handler(GET_CALIBRATION_COEFFICIENTS_FROM_EEPROM, &Spectrometer::GetEepromCalibrationCoefficients));
    m_command.insert(Handler(SET_CALIBRATION_COEFFICIENTS_TO_EEPROM, &Spectrometer::SetEepromCalibrationCoefficients));
    m_command.insert(Handler(GET_PIXEL_BINNING_FACTOR, &Spectrometer::GetPixelBinningfactor));
    m_command.insert(Handler(SET_PIXEL_BINNING_FACTOR, &Spectrometer::SetPixelBinningfactor));
    m_command.insert(Handler(GET_INTEGRATION_TIME_MINIMUM, &Spectrometer::GetIntegrationTimeMinimum));
    m_command.insert(Handler(GET_INTEGRATION_TIME_MAXIMUM, &Spectrometer::GetIntegrationTimeMaximum));
    m_command.insert(Handler(GET_INTENSITY_MAXIMUM, &Spectrometer::GetIntensityMaximum));
    m_command.insert(Handler(GET_ELECTRIC_DARK_CORRECTION, &Spectrometer::GetElectricDarkCorrection));
    m_command.insert(Handler(SET_ELECTRIC_DARK_CORRECTION, &Spectrometer::SetElectricDarkCorrection));
    m_command.insert(Handler(GET_SPECTROMETER_STATUS, &Spectrometer::GetCurrentStatus));
}

Spectrometer::~Spectrometer() {
}

/* Construct a command message to give to the active object for execution.
*/
Spectrometer::CommandMessage::CommandMessage(boost::shared_ptr<Connection> connection, Spectrometer *spectrometer,
    Spectrometer::CommandType command, const std::string &arguments) :

    m_connection(connection),
    m_spectrometer(spectrometer),
    m_command(command),
    m_arguments(arguments) {
}

/* The actual task being carried out by the active object. This will run in the active object's thread of
* execution. First it handles the command request and then returns the result to the response handler.
*/
void Spectrometer::CommandMessage::Execute() {
    Spectrometer::Response result = m_spectrometer->HandleCommand(m_command, m_arguments);
    m_spectrometer->HandleResponse(m_connection, result.first, result.second);
}

/* Return the current integraiton time in microseconds for this spectrometer.
*/
Spectrometer::Response Spectrometer::GetIntegrationTime(const std::string &arguments) {
    Response result;
    try {
        result.second = boost::lexical_cast<std::string>(m_integrationTime);
        result.first = SUCCESS;
    }
    catch (boost::bad_lexical_cast &blc) {
        result.first = INVALID_INTEGRATION_TIME;
    }
    return result;
}

/* Set the integration time in microseconds for this spectrometer.
*/
Spectrometer::Response Spectrometer::SetIntegrationTime(const std::string &arguments) {
    Response result;
    result.second = arguments;

    try {
        long time = boost::lexical_cast<long>(arguments);
        if (time < m_minIntegrationTime) {
            result.first = INVALID_INTEGRATION_TIME;
        }
        else if (time > m_maxIntegrationTime) {
            result.first = INVALID_INTEGRATION_TIME;
        }
        else {
            int error = 0;
            m_wrapper->setIntegrationTimeMicrosec(m_index, &error, time);
            if (error != 0) {
                result.first = SPECTROMETER_ERROR;
            }
            else {
                m_integrationTime = time;
                result.first = SUCCESS;
            }
        }
    }
    catch (boost::bad_lexical_cast &blc) {
        result.first = UNABLE_TO_PARSE_PARAMETER;
    }

    return result;
}

/* Return the boxcar width for this spectrometer.
*/
Spectrometer::Response Spectrometer::GetBoxcarWidth(const std::string &arguments) {
    Response result;
    try {
        result.second = boost::lexical_cast<std::string>(m_boxcarWidth);
        result.first = SUCCESS;
    }
    catch (boost::bad_lexical_cast &blc) {
        result.first = INVALID_BOXCAR_WIDTH;
    }
    return result;
}

/* Set the boxcar width for this spectrometer.
*/
Spectrometer::Response Spectrometer::SetBoxcarWidth(const std::string &arguments) {
    Response result;
    result.second = arguments;

    try {
        int boxcar = boost::lexical_cast<int>(arguments);
        if (boxcar < 0) {
            result.first = INVALID_BOXCAR_WIDTH;
        }
        else {
            m_boxcarWidth = boxcar;
            result.first = SUCCESS;
        }
    }
    catch (boost::bad_lexical_cast &blc) {
        result.first = UNABLE_TO_PARSE_PARAMETER;
    }

    return result;
}

/* Return the scans to average for this spectrometer.
*/
Spectrometer::Response Spectrometer::GetScansToAverage(const std::string &arguments) {
    Response result;
    try {
        result.second = boost::lexical_cast<std::string>(m_scansToAverage);
        result.first = SUCCESS;
    }
    catch (boost::bad_lexical_cast &blc) {
        result.first = INVALID_SCANS_TO_AVERAGE;
    }
    return result;
}

/* Set the scans to average for this spectrometer.
*/
Spectrometer::Response Spectrometer::SetScansToAverage(const std::string &arguments) {
    Response result;
    result.second = arguments;

    try {
        int scans = boost::lexical_cast<int>(arguments);
        if (scans < 0) {
            result.first = INVALID_SCANS_TO_AVERAGE;
        }
        else {
            m_scansToAverage = scans;
            result.first = SUCCESS;
        }
    }
    catch (boost::bad_lexical_cast &blc) {
        result.first = UNABLE_TO_PARSE_PARAMETER;
    }

    return result;
}

/* Get a spectrum and return it as a space separated string.
*/
Spectrometer::Response Spectrometer::GetSpectrum(const std::string &arguments) {
    Response result;

    int error = 0;
    int actual = m_wrapper->getFormattedSpectrum(m_index, &error, m_spectrum.data(), m_binnedPixelCount);
    if (error == 0) {
        try {
            std::string spectrum;
            const int lastSpaceMarker = m_binnedPixelCount - 1;
            for (int i = 0; i < m_binnedPixelCount; ++i) {
                std::string s = boost::lexical_cast<std::string>(m_spectrum[i]); 
                spectrum.append(s).append(i < lastSpaceMarker ? " " : "");
            }
            // put the length of the string into the first 4 characters of the reponse
            const int sizeCount = 4;
            unsigned char size[sizeCount];
            size_t s = spectrum.size();
            size[0] = (s >> 24) & 0xFF;
            size[1] = (s >> 16) & 0xFF;
            size[2] = (s >> 8) & 0xFF;
            size[3] = s & 0xFF;

            result.second.resize(s + sizeCount);
            std::copy(size, size + sizeCount, result.second.begin());
            std::copy(spectrum.begin(), spectrum.end(), result.second.begin() + sizeCount);

            result.first = SUCCESS;
        }
        catch (boost::bad_lexical_cast &blc) {
            result.first = UNABLE_TO_ENCODE_SPECTRUM;
        }
    }
    else {
        result.first = UNABLE_TO_GET_SPECTRUM;
    }
    return result;
}

/* get the wavelengths for this spectrometer and return them as as space separated string.
*/
Spectrometer::Response Spectrometer::GetWavelengths(const std::string &arguments) {
    Response result;
    try {
        std::string wavelengths;
        const int lastSpaceMarker = m_binnedPixelCount - 1;
        for (int i = 0; i < m_binnedPixelCount; ++i) {
            std::string w = boost::lexical_cast<std::string>(m_wavelengths[i]); 
            wavelengths.append(w).append(i < lastSpaceMarker ? " " : "");
        }
        // put the length of the string into the first 4 characters of the reponse
        const int sizeCount = 4;
        unsigned char size[sizeCount];
        size_t s = wavelengths.size();
        size[0] = (s >> 24) & 0xFF;
        size[1] = (s >> 16) & 0xFF;
        size[2] = (s >> 8) & 0xFF;
        size[3] = s & 0xFF;

        result.second.resize(s + sizeCount);
        std::copy(size, size + sizeCount, result.second.begin());
        std::copy(wavelengths.begin(), wavelengths.end(), result.second.begin() + sizeCount);

        result.first = SUCCESS;
    }
    catch (boost::bad_lexical_cast &blc) {
        result.first = UNABLE_TO_GET_WAVELENGTHS;
    }
    return result;
}

/* Return the serial number for this spectrometer.
*/
Spectrometer::Response Spectrometer::GetSerialNumber(const std::string &arguments) {
    Response result(SUCCESS, m_serialNumber);
    return result;
}

/* Return the name (model) of this spectrometer.
*/
Spectrometer::Response Spectrometer::GetName(const std::string &arguments) {
    Response result(SUCCESS, m_name);
    return result;
}

/* Return the (buffered) calibraiton coefficients for this spectrometer.
*/
Spectrometer::Response Spectrometer::GetBufferCalibrationCoefficients(const std::string &arguments) {
    Response result;
    return result;
}

/* Set the (buffered) calibraiton coefficients for this spectrometer.
*/
Spectrometer::Response Spectrometer::SetBufferCalibrationCoefficients(const std::string &arguments) {
    Response result;
    return result;
}

/* Get the calibration coefficients from the EEPROM in this spectrometer.
*/
Spectrometer::Response Spectrometer::GetEepromCalibrationCoefficients(const std::string &arguments) {
    Response result;
    return result;
}

/* Set the calibration coefficients into the EEPROM in this spectrometer.
*/
Spectrometer::Response Spectrometer::SetEepromCalibrationCoefficients(const std::string &arguments) {
    Response result;
    return result;
}

/* Return the pixel binning factor (0 to 3) for this spectrometer (where supported).
*/
Spectrometer::Response Spectrometer::GetPixelBinningfactor(const std::string &arguments) {
    Response result;
    try {
        result.second = boost::lexical_cast<std::string>(m_binningFactor);
        result.first = SUCCESS;
    }
    catch (boost::bad_lexical_cast &blc) {
        result.first = INVALID_PIXEL_BINNING_FACTOR;
    }
    return result;
}

/* Set the pixel binning factor for this spectrometer (where supported).
*/
Spectrometer::Response Spectrometer::SetPixelBinningfactor(const std::string &arguments) {
    Response result;
    result.second = arguments;

    try {
        int binningFactor = boost::lexical_cast<int>(arguments);
        int error = 0;
        m_wrapper->setPixelBinningFactor(m_index, &error, binningFactor);
        if (error != 0) {
            result.first = INVALID_PIXEL_BINNING_FACTOR;
        }
        else {
            m_binningFactor = binningFactor;
            result.first = SUCCESS;
        }
    }
    catch (boost::bad_lexical_cast &blc) {
        result.first = INVALID_PIXEL_BINNING_FACTOR;
    }

    return result;
}

/* Return the minimum integration time for this spectrometer.
*/
Spectrometer::Response Spectrometer::GetIntegrationTimeMinimum(const std::string &arguments) {
    Response result;

    try {
        result.second = boost::lexical_cast<std::string>(m_minIntegrationTime);
        result.first = SUCCESS;
    }
    catch (boost::bad_lexical_cast &blc) {
        result.first = INVALID_INTEGRATION_TIME;
    }

    return result;
}

/* Return the maximum integration time for this spectromter.
*/
Spectrometer::Response Spectrometer::GetIntegrationTimeMaximum(const std::string &arguments) {
    Response result;

    try {
        result.second = boost::lexical_cast<std::string>(m_maxIntegrationTime);
        result.first = SUCCESS;
    }
    catch (boost::bad_lexical_cast &blc) {
        result.first = INVALID_INTEGRATION_TIME;
    }

    return result;
}

/* Return the maximum intensity for this spectrometer.
*/
Spectrometer::Response Spectrometer::GetIntensityMaximum(const std::string &arguments) {
    Response result;

    try {
        result.second = boost::lexical_cast<std::string>(m_maxIntensity);
        result.first = SUCCESS;
    }
    catch (boost::bad_lexical_cast &blc) {
        result.first = SPECTROMETER_ERROR;
    }

    return result;
}

/* Return 1 if electirc dark correction is turned on for this spectrometer, 0 otherwise.
*/
Spectrometer::Response Spectrometer::GetElectricDarkCorrection(const std::string &arguments) {
    Response result;
    result.second = m_applyElectricDark ? "1" : "0";
    result.first = SUCCESS;
    return result;
}

/* Set the electric dark correction on/off (1/0) for this spectrometer.
*/
Spectrometer::Response Spectrometer::SetElectricDarkCorrection(const std::string &arguments) {
    Response result;
    result.second = arguments;

    if (m_electricDarkIndex.size() == 0) {
        result.first = INVALID_ELECTRIC_DARK;
    }
    else {
        try {
            int ed = boost::lexical_cast<int>(arguments);
            m_applyElectricDark = ed != 0;
            result.first = SUCCESS;
        }
        catch (boost::bad_lexical_cast &blc) {
            result.first = UNABLE_TO_PARSE_PARAMETER;
        }
    }

    return result;
}

/* Get the status of the last executed command/request. Note this command resets the status to SUCCESS.
*/
Spectrometer::Response Spectrometer::GetCurrentStatus(const std::string &arguments) {
    int resultCode = m_resultCode;
    std::string resultMessage = ms_results.find(resultCode) != ms_results.end() ? ms_results.at(resultCode) : "";
    m_resultCode = SUCCESS;

    return Response(resultCode, resultMessage);
}

/* Convenience method to initialise the spectrometer.
*/
void Spectrometer::InitializeParameters() {
    if (m_wrapper != 0) {
        int error = 0;
        m_minIntegrationTime = m_wrapper->getMinIntegrationTimeMicrosec(m_index, &error);
        m_maxIntegrationTime = m_wrapper->getMaxIntegrationTimeMicrosec(m_index, &error);
        m_integrationTime = m_minIntegrationTime;
        m_wrapper->setIntegrationTimeMicrosec(m_index, &error, m_integrationTime);
        m_boxcarWidth = 0;
        m_scansToAverage = 0;
        m_pixelCount = m_wrapper->getFormattedSpectrumLength(m_index, &error);
        m_binnedPixelCount = m_pixelCount;
        m_electricDarkIndex.resize(ms_maxElectricDarkPixels);
        int actualElectricDark = m_wrapper->getElectricDarkPixelIndices(m_index, &error, m_electricDarkIndex.data(), ms_maxElectricDarkPixels);
        m_electricDarkIndex.resize(actualElectricDark);
        m_binningFactor = 0;
        m_applyElectricDark = false;
        m_maxIntensity = m_wrapper->getMaximumIntensity(m_index, &error);
        m_spectrum.resize(m_pixelCount, 0.0);
        m_wavelengths.resize(m_pixelCount, 0.0);
        m_wrapper->getWavelengths(m_index, &error, m_wavelengths.data(), m_pixelCount);
        char typeBuffer[ms_maxSpectrometerNameCount] = {0};
        m_wrapper->getModel(m_index, &error, typeBuffer, ms_maxSpectrometerNameCount);
        m_name = std::string(typeBuffer);
        char serialBuffer[ms_maxSerialNumberCount] = {0};
        m_wrapper->getSerialNumber(m_index, &error, serialBuffer, ms_maxSerialNumberCount);
        m_serialNumber = std::string(serialBuffer);
    }
}

// static map of return codes to human readable strings.
const std::map<int, std::string> Spectrometer::ms_results = boost::assign::map_list_of
    (SUCCESS, std::string("Success"))
    (SPECTROMETER_INTERFACE_NOT_ESTABLISHED, std::string("Unable to initialize the library for accessing the spectrometer"))
    (UNABLE_TO_OPEN_SPECTROMETER, std::string("Unable to find a spectrometer"))
    (UNABLE_TO_CLOSE_SPECTROMETER, std::string("Unable to stop the spectrometer"))
    (SPECTROMETER_ERROR, "Spectrometer returned an error")
    (UNABLE_TO_PARSE_PARAMETER, std::string("Unable to decode the parameters sent to the API"))
    (UNABLE_TO_DETERMINE_SPECTRUM_LENGTH, std::string("Unable to determine the length of the spectrum"))
    (INTEGRATION_TIME_TOO_LOW, "Integration time is too low")
    (INVALID_INTEGRATION_TIME, std::string("Unable to set the integration time"))
    (INVALID_BOXCAR_WIDTH, "Invalid boxcar width")
    (INVALID_SCANS_TO_AVERAGE, "Invalid scans to average")
    (INVALID_CALIBRATION_COEFFICIENTS, "Invalid calibration coefficients")
    (INVALID_PIXEL_BINNING_FACTOR, "Invalid binning factor")
    (INVALID_ELECTRIC_DARK, "Invalid electric dark")
    (UNABLE_TO_GET_SPECTRUM, std::string("Unable to get a spectrum"))
    (UNABLE_TO_ENCODE_SPECTRUM, std::string("Unable to encode the spectrum for returning to the API"))
    (UNABLE_TO_GET_WAVELENGTHS, std::string("Unable to get the spectrometer wavelengths"))
    (UNABLE_TO_ENCODE_WAVELENGTHS, std::string("Unable to encode the wavelengths for returning to the API"))
    (UNABLE_TO_SET_SHUTTER_POSITION, std::string("Unable to set the shutter position"))
    (SPECTROMETER_INDEX_DOES_NOT_EXIST, std::string("Invalid spectrometer number"));
