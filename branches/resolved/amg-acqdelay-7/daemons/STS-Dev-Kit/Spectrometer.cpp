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
#include "RequestHandlerConfiguration.h"
#include "IResponseHandler.h"
#include <api/seabreezeapi/SeaBreezeAPI.h>
#include <api/seabreezeapi/SeaBreezeAPIConstants.h>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/assign.hpp>
#include <limits>
#include <numeric>

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

/* Constructor. Map the command codes to member function handlers.
*/
Spectrometer::Spectrometer(IResponseHandler *responseHandler, const long handle, SeaBreezeAPI *seabreezeApi, RequestHandlerConfiguration &config) :
    m_responseHandler(responseHandler),
    m_handle(handle),
    m_seabreezeApi(seabreezeApi),
    m_configuration(config),
    m_minIntegrationTime(0),
    m_maxIntegrationTime(std::numeric_limits<long>::max()) {

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
    m_command.insert(Handler(GET_PIXEL_BINNING_FACTOR, &Spectrometer::GetPixelBinningFactor));
    m_command.insert(Handler(SET_PIXEL_BINNING_FACTOR, &Spectrometer::SetPixelBinningFactor));
    m_command.insert(Handler(GET_INTEGRATION_TIME_MINIMUM, &Spectrometer::GetIntegrationTimeMinimum));
    m_command.insert(Handler(GET_INTEGRATION_TIME_MAXIMUM, &Spectrometer::GetIntegrationTimeMaximum));
    m_command.insert(Handler(GET_INTENSITY_MAXIMUM, &Spectrometer::GetIntensityMaximum));
    m_command.insert(Handler(GET_ELECTRIC_DARK_CORRECTION, &Spectrometer::GetElectricDarkCorrection));
    m_command.insert(Handler(SET_ELECTRIC_DARK_CORRECTION, &Spectrometer::SetElectricDarkCorrection));
    m_command.insert(Handler(GET_SPECTROMETER_STATUS, &Spectrometer::GetCurrentStatus));
    m_command.insert(Handler(SET_TEC_ENABLE, &Spectrometer::SetTecEnable));
    m_command.insert(Handler(SET_TEC_TEMPERATURE, &Spectrometer::SetTecTemperature));
    m_command.insert(Handler(GET_TEC_TEMPERATURE, &Spectrometer::GetTecTemperature));
    m_command.insert(Handler(SET_LAMP_ENABLE, &Spectrometer::SetLampEnable));
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

/* Return the current integration time in microseconds for this spectrometer.
*/
Spectrometer::Response Spectrometer::GetIntegrationTime(const std::string &arguments) {
    Response result;
    try {
        // we can't query the spectrometer for this so return the cached value
        result.second = boost::lexical_cast<std::string>(IntegrationTime());
        result.first = SUCCESS;
    }
    catch (boost::bad_lexical_cast) {
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
        else if (m_spectrometerFeatures.size() == 0) {
            result.first = SPECTROMETER_ERROR;
        }
        else {
            m_integrationTime = time;
            int error = 0;
            m_seabreezeApi->spectrometerSetIntegrationTimeMicros(m_handle, m_spectrometerFeatures[0], &error, m_integrationTime);
            if (error != 0) {
                result.first = SPECTROMETER_ERROR;
            }
            else {
                m_integrationTime = time;
                result.first = SUCCESS;
            }
        }
    }
    catch (boost::bad_lexical_cast) {
        result.first = UNABLE_TO_PARSE_PARAMETER;
    }

    return result;
}

/* Return the boxcar width for this spectrometer.
*/
Spectrometer::Response Spectrometer::GetBoxcarWidth(const std::string &arguments) {
    Response result;
    try {
        // Intercept this functionality if it exists...we will do it here in the handler
        if (false/*m_spectrumProcessingFeatures.size() == 0*/) {
            result.first = SPECTROMETER_ERROR;
        }
        else {
            int error = 0;
            //unsigned char width = m_seabreezeApi->spectrumProcessingBoxcarWidthGet(m_handle, m_spectrumProcessingFeatures[0], &error);
            if (error != 0) {
                result.first = SPECTROMETER_ERROR;
            }
            else {
                result.second = boost::lexical_cast<std::string>(static_cast<unsigned int>(m_boxcarWidth));
                result.first = SUCCESS;
            }
        }
    }
    catch (boost::bad_lexical_cast) {
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
        // Intercept this functionality if it exists...we will do it here in the handler
        // convert to unsigned int first otherwise we get the character value
        unsigned int width = boost::lexical_cast<unsigned int>(arguments);
        if (width > std::numeric_limits<unsigned char>::max()) {
            result.first = INVALID_BOXCAR_WIDTH;
        }
        //else if (m_spectrumProcessingFeatures.size() == 0) {
        //    result.first = SPECTROMETER_ERROR;
        //}
        else {
            m_boxcarWidth = static_cast<unsigned char>(width);
            int error = 0;
            //m_seabreezeApi->spectrumProcessingBoxcarWidthSet(m_handle, m_spectrumProcessingFeatures[0], &error, m_boxcarWidth);
            result.first = (error == 0) ? SUCCESS : SPECTROMETER_ERROR;
        }
    }
    catch (boost::bad_lexical_cast) {
        result.first = UNABLE_TO_PARSE_PARAMETER;
    }

    return result;
}

/* Return the scans to average for this spectrometer.
*/
Spectrometer::Response Spectrometer::GetScansToAverage(const std::string &arguments) {
    Response result;
    try {
        // Intercept this functionality if it exists...we will do it here in the handler
        if (false/*m_spectrumProcessingFeatures.size() == 0*/) {
            result.first = SPECTROMETER_ERROR;
        }
        else {
            int error = 0;
            //unsigned short scans = m_seabreezeApi->spectrumProcessingScansToAverageGet(m_handle, m_spectrumProcessingFeatures[0], &error);
            if (error != 0) {
                result.first = SPECTROMETER_ERROR;
            }
            else {
                result.second = boost::lexical_cast<std::string>(m_scansToAverage);
                result.first = SUCCESS;
            }
        }
    }
    catch (boost::bad_lexical_cast) {
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
        // Intercept this functionality if it exists...we will do it here in the handler
        if (false/*m_spectrumProcessingFeatures.size() == 0*/) {
            result.first = SPECTROMETER_ERROR;
        }
        else {
            m_scansToAverage = boost::lexical_cast<unsigned short>(arguments);
            int error = 0;
            //m_seabreezeApi->spectrumProcessingScansToAverageSet(m_handle, m_spectrumProcessingFeatures[0], &error, m_scansToAverage);
            result.first = (error == 0) ? SUCCESS : SPECTROMETER_ERROR;
        }
    }
    catch (boost::bad_lexical_cast) {
        result.first = INVALID_SCANS_TO_AVERAGE;
    }

    return result;
}

/* Apply any electric dark correction.
*/
void Spectrometer::CorrectForElectricDark(std::vector<double> &spectrum, const size_t numPixels) {
    if (m_applyElectricDark) {
        const size_t numDark = m_electricDarkIndex.size();
        if (numDark > 0) {
            double correction = 0.0;
            for (size_t i = 0; i < numDark; ++i) {
                correction += spectrum[m_electricDarkIndex[i]];
            }
            correction /= numDark;

            for (size_t i = 0; i < numPixels; ++i) {
                spectrum[i] -= correction;
            }
        }
    }
}

/* Apply any boxcar smoothing
*/
void Spectrometer::ApplyBoxcar(std::vector<double> &spectrum, const size_t numPixels) {
    if (m_boxcarWidth > 0) {

        const size_t windowSize = 2 * m_boxcarWidth + 1;
        if (numPixels > windowSize) {
            std::vector<double> window(spectrum.begin(), spectrum.begin() + windowSize);

            size_t windowEndPos = windowSize - 1;
            const size_t windowUpperLimit = numPixels - m_boxcarWidth;

            for (size_t i = m_boxcarWidth; i < windowUpperLimit; ++i) {
                window[windowEndPos % windowSize] = spectrum[windowEndPos];
                spectrum[i] = std::accumulate(window.begin(), window.end(), 0.0) / windowSize;
                ++windowEndPos;
            }
        }
    }
}

/* Return a spectrum from this spectrometer.
*/
std::vector<double> Spectrometer::GetSpectrum() {

    int error = 0;
    std::vector<double> spectrum(m_binnedPixelCount, 0.0);
    GetSpectrum(spectrum, error);

    return spectrum;
}

/* Return a spectrum from this spectrometer.
*/
void Spectrometer::GetSpectrum(std::vector<double> &spectrum, int &error) {

    if (m_spectrometerFeatures.size() > 0) {
        const size_t numPixels = spectrum.size();
        error = 0;

        if (m_scansToAverage < 2) {
            int actual = m_seabreezeApi->spectrometerGetFormattedSpectrum(m_handle, m_spectrometerFeatures[0], &error, spectrum.data(), numPixels);
            if (error == 0) {
                CorrectForElectricDark(spectrum, actual);
            }
        }
        else {
            std::vector<double> sum(numPixels, 0.0);
            for (unsigned short scan = 0; scan < m_scansToAverage; ++scan) {
                int actual = m_seabreezeApi->spectrometerGetFormattedSpectrum(m_handle, m_spectrometerFeatures[0], &error, spectrum.data(), numPixels);
                if (error == 0) {
                    CorrectForElectricDark(spectrum, actual);
                    // accumulate the current spectrum into the sum
                    transform(sum.begin(), sum.begin() + actual, spectrum.begin(), sum.begin(), std::plus<double>());
                }
                else {
                    break;
                }
            }
            if (error == 0) {
                std::swap(spectrum, sum);
                for (size_t i = 0; i < numPixels; ++i) {
                    spectrum[i] /= m_scansToAverage;
                }
            }
        }

        if (error == 0) {
            ApplyBoxcar(spectrum, numPixels);
        }
    }
    else {
        // TODO avoid this duplication of the test...for now just skip the error
    }
}

/* Get a spectrum and return it as a space separated string.
*/
Spectrometer::Response Spectrometer::GetSpectrum(const std::string &arguments) {

    using boost::adaptors::transformed;
    //using boost::adaptors::transform;
    using boost::algorithm::join;
    using boost::lexical_cast;
    using boost::make_iterator_range;
    using std::string;
    //using std::vector;

    Response result(SUCCESS, "");

    if (m_spectrometerFeatures.size() == 0) {
        result.first = SPECTROMETER_ERROR;
    }
    else {
        int error = 0;
        GetSpectrum(m_spectrum, error);
        if (error != 0) {
            result.first = UNABLE_TO_GET_SPECTRUM;
        }
        else {
            try {
                // put the length of the string into the first 4 characters of the reponse
                const size_t sizeCount = 4;
                unsigned char size[sizeCount] = {0};
                result.second = string(size, size + sizeCount);
                /*
                 join concatenates a range of values, make_iterator_range selects the correct number of pixels according to the binning
                 and transformed uses lexical_cast to convert double to string. The alternative is a loop with push_back and logic
                 to prevent adding the space at the end
                 */
                result.second.append(
                    join(make_iterator_range(m_spectrum.begin(), m_spectrum.begin() + m_binnedPixelCount) |
                    transformed(lexical_cast<string, double>), " "));

                size_t s = result.second.size() - sizeCount;
                size[0] = (s >> 24) & 0xFF;
                size[1] = (s >> 16) & 0xFF;
                size[2] = (s >> 8) & 0xFF;
                size[3] = s & 0xFF;

                std::copy(size, size + sizeCount, result.second.begin());

                result.first = SUCCESS;
            }
            catch (boost::bad_lexical_cast) {
                result.first = UNABLE_TO_ENCODE_SPECTRUM;
            }
        }
    }
    return result;
}

/* Get the wavelengths for this spectrometer and return them as as space separated string.
*/
Spectrometer::Response Spectrometer::GetWavelengths(const std::string &arguments) {

    using boost::adaptors::transformed;
    using boost::algorithm::join;
    using boost::lexical_cast;
    using boost::make_iterator_range;
    using std::string;

    Response result;

    try {
        // put the length of the string into the first 4 characters of the reponse
        const size_t sizeCount = 4;
        unsigned char size[sizeCount] = {0};
        result.second = string(size, size + sizeCount);
        int error = 0;
        int actual = m_seabreezeApi->spectrometerGetWavelengths(m_handle, m_spectrometerFeatures[0], &error, m_wavelengths.data(), m_binnedPixelCount);
        if (error == 0) {
            /*
                join concatenates a range of values, make_iterator_range selects the correct number of pixels according to the binning
                and transformed uses lexical_cast to convert double to string. The alternative is a loop with push_back and logic
                to prevent adding the space at the end
                */
            result.second.append(
                join(make_iterator_range(m_wavelengths.begin(), m_wavelengths.begin() + m_binnedPixelCount) |
                transformed(lexical_cast<string, double>), " "));

            size_t s = result.second.size() - sizeCount;
            size[0] = (s >> 24) & 0xFF;
            size[1] = (s >> 16) & 0xFF;
            size[2] = (s >> 8) & 0xFF;
            size[3] = s & 0xFF;

            std::copy(size, size + sizeCount, result.second.begin());

            result.first = SUCCESS;
        }
        else {
            result.first = UNABLE_TO_GET_WAVELENGTHS;
        }
    }
    catch (boost::bad_lexical_cast) {
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

/* Return the pixel binning factor (0 to 3 for the STS) for this spectrometer (where supported).
*/
Spectrometer::Response Spectrometer::GetPixelBinningFactor(const std::string &arguments) {
    Response result;
    if (m_pixelBinningFeatures.size() == 0) {
        result.first = SPECTROMETER_ERROR;
    }
    else {
        try {
            int error = 0;
            unsigned char binning = m_seabreezeApi->binningGetPixelBinningFactor(m_handle, m_pixelBinningFeatures[0], &error);
            if (error != 0) {
                result.first = INVALID_PIXEL_BINNING_FACTOR;
            }
            else {
                result.second = boost::lexical_cast<std::string>(binning);
                result.first = SUCCESS;
            }
        }
        catch (boost::bad_lexical_cast) {
            result.first = INVALID_PIXEL_BINNING_FACTOR;
        }
    }
    return result;
}

/* Set the pixel binning factor for this spectrometer (where supported).
*/
Spectrometer::Response Spectrometer::SetPixelBinningFactor(const std::string &arguments) {
    Response result;
    result.second = arguments;

    if (m_pixelBinningFeatures.size() == 0) {
        result.first = SPECTROMETER_ERROR;
    }
    else {
        try {
            unsigned int binning = boost::lexical_cast<unsigned int>(arguments);
            if (binning > std::numeric_limits<unsigned char>::max()) {
                result.first = INVALID_PIXEL_BINNING_FACTOR;
            }
            else {
                unsigned char binningFactor = static_cast<unsigned char>(binning);
                int error = 0;
                m_seabreezeApi->binningSetPixelBinningFactor(m_handle, m_pixelBinningFeatures[0], &error, binningFactor);
                if (error != 0) {
                    result.first = INVALID_PIXEL_BINNING_FACTOR;
                }
                else {
                    m_binningFactor = binningFactor;
                    m_binnedPixelCount = m_pixelCount >> m_binningFactor;
                    m_wavelengths.resize(m_binnedPixelCount);
                    int actual = m_seabreezeApi->spectrometerGetWavelengths(m_handle, m_spectrometerFeatures[0], &error, m_wavelengths.data(), m_binnedPixelCount);
                    if (error != 0) {
                        result.first = SPECTROMETER_ERROR;
                    }
                    else {
                        result.first = SUCCESS;
                    }
                }
            }
        }
        catch (boost::bad_lexical_cast) {
            result.first = INVALID_PIXEL_BINNING_FACTOR;
        }
    }

    return result;
}

/* Return the default pixel binning factor (0 to 3 for the STS) for this spectrometer (where supported).
*/
Spectrometer::Response Spectrometer::GetDefaultPixelBinningFactor(const std::string &arguments) {
    Response result;
    if (m_pixelBinningFeatures.size() == 0) {
        result.first = SPECTROMETER_ERROR;
    }
    else {
        try {
            int error = 0;
            unsigned char binning = m_seabreezeApi->binningGetDefaultPixelBinningFactor(m_handle, m_pixelBinningFeatures[0], &error);
            if (error != 0) {
                result.first = INVALID_PIXEL_BINNING_FACTOR;
            }
            else {
                result.second = boost::lexical_cast<std::string>(binning);
                result.first = SUCCESS;
            }
        }
        catch (boost::bad_lexical_cast) {
            result.first = INVALID_PIXEL_BINNING_FACTOR;
        }
    }
    return result;
}

/* Set the pixel binning factor for this spectrometer (where supported).
*/
Spectrometer::Response Spectrometer::SetDefaultPixelBinningFactor(const std::string &arguments) {
    Response result;
    result.second = arguments;

    if (m_pixelBinningFeatures.size() == 0) {
        result.first = SPECTROMETER_ERROR;
    }
    else {
        try {
            unsigned int binning = boost::lexical_cast<unsigned int>(arguments);
            if (binning > std::numeric_limits<unsigned char>::max()) {
                result.first = INVALID_PIXEL_BINNING_FACTOR;
            }
            else {
                unsigned char binningFactor = static_cast<unsigned char>(binning);
                int error = 0;
                m_seabreezeApi->binningSetDefaultPixelBinningFactor(m_handle, m_pixelBinningFeatures[0], &error, binningFactor);
                if (error != 0) {
                    result.first = INVALID_PIXEL_BINNING_FACTOR;
                }
                else {
                    result.first = SUCCESS;
                }
            }
        }
        catch (boost::bad_lexical_cast) {
            result.first = INVALID_PIXEL_BINNING_FACTOR;
        }
    }

    return result;
}

/* Return the default pixel binning factor (0 to 3 for the STS) for this spectrometer (where supported).
*/
Spectrometer::Response Spectrometer::GetMaxPixelBinningFactor(const std::string &arguments) {
    Response result;
    if (m_pixelBinningFeatures.size() == 0) {
        result.first = SPECTROMETER_ERROR;
    }
    else {
        try {
            int error = 0;
            unsigned char binning = m_seabreezeApi->binningGetMaxPixelBinningFactor(m_handle, m_pixelBinningFeatures[0], &error);
            if (error != 0) {
                result.first = INVALID_PIXEL_BINNING_FACTOR;
            }
            else {
                result.second = boost::lexical_cast<std::string>(binning);
                result.first = SUCCESS;
            }
        }
        catch (boost::bad_lexical_cast) {
            result.first = INVALID_PIXEL_BINNING_FACTOR;
        }
    }
    return result;
}

/* Return the minimum integration time for this spectrometer.
*/
Spectrometer::Response Spectrometer::GetIntegrationTimeMinimum(const std::string &arguments) {
    Response result;

    try {
        result.second = boost::lexical_cast<std::string>(MinIntegrationTime());
        result.first = SUCCESS;
    }
    catch (boost::bad_lexical_cast) {
        result.first = INVALID_INTEGRATION_TIME;
    }

    return result;
}

/* Return the maximum integration time for this spectrometer.
*/
Spectrometer::Response Spectrometer::GetIntegrationTimeMaximum(const std::string &arguments) {
    Response result;

    try {
        result.second = boost::lexical_cast<std::string>(MaxIntegrationTime());
        result.first = SUCCESS;
    }
    catch (boost::bad_lexical_cast) {
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
    catch (boost::bad_lexical_cast) {
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
        catch (boost::bad_lexical_cast) {
            result.first = UNABLE_TO_PARSE_PARAMETER;
        }
    }

    return result;
}

/* Turn the TEC on/off
*/
Spectrometer::Response Spectrometer::SetTecEnable(const std::string &arguments) {
    Response result;
    result.second = arguments;

    if (m_thermoElectricFeatures.size() == 0) {
        result.first = SPECTROMETER_ERROR;
    }
    else {
        try {
            unsigned int intEnable = boost::lexical_cast<unsigned int>(arguments);
            
            if (intEnable > 1) {
                result.first = INVALID_TEC_ENABLE;
            }
            else {
                unsigned char enable = static_cast<unsigned char>(intEnable);
                int error = 0;
                m_seabreezeApi->tecSetEnable(m_handle, m_thermoElectricFeatures[0], &error, enable);
                if (error != 0) {
                    result.first = SPECTROMETER_ERROR;
                }
                else {
                    result.first = SUCCESS;
                }
            }
        }
        catch (boost::bad_lexical_cast) {
            result.first = INVALID_TEC_ENABLE;
        }
    }

    return result;
}

/* Set the TEC temperature
*/
Spectrometer::Response Spectrometer::SetTecTemperature(const std::string &arguments) {
    Response result;
    result.second = arguments;

    if (m_thermoElectricFeatures.size() == 0) {
        result.first = SPECTROMETER_ERROR;
    }
    else {
        try {
            double temperature = boost::lexical_cast<double>(arguments);
            
            int error = 0;
            m_seabreezeApi->tecSetTemperatureSetpointDegreesC(m_handle, m_thermoElectricFeatures[0], &error, temperature);

            if (error != 0) {
                result.first = SPECTROMETER_ERROR;
            }
            else {
                result.first = SUCCESS;
            }
        }
        catch (boost::bad_lexical_cast) {
            result.first = UNABLE_TO_PARSE_PARAMETER;
        }
    }

    return result;
}

/* Return the TEC temperature.
*/
Spectrometer::Response Spectrometer::GetTecTemperature(const std::string &arguments) {
    Response result(SUCCESS, "");

    if (m_thermoElectricFeatures.size() == 0) {
        result.first = SPECTROMETER_ERROR;
    }
    else {
        try {
            int error = 0;
            double temperature = m_seabreezeApi->tecReadTemperatureDegreesC(m_handle, m_thermoElectricFeatures[0], &error);

            if (error != 0) {
                result.first = SPECTROMETER_ERROR;
            }
            else {
                result.first = SUCCESS;
                result.second = boost::lexical_cast<std::string>(temperature);
            }
        }
        catch (boost::bad_lexical_cast) {
            result.first = UNABLE_TO_PARSE_PARAMETER;
        }
    }

    return result;
}

/* Turn on/off any lamp
*/
Spectrometer::Response Spectrometer::SetLampEnable(const std::string &arguments) {
    Response result;
    result.second = arguments;

    if (m_lampFeatures.size() == 0) {
        result.first = SPECTROMETER_ERROR;
    }
    else {
        try {
            unsigned int intEnable = boost::lexical_cast<unsigned int>(arguments);
            
            if (intEnable > 1) {
                result.first = INVALID_LAMP_ENABLE;
            }
            else {
                const bool enable = (intEnable == 1);
                int error = 0;
                m_seabreezeApi->lampSetLampEnable(m_handle, m_lampFeatures[0], &error, enable);
                if (error != 0) {
                    result.first = SPECTROMETER_ERROR;
                }
                else {
                    result.first = SUCCESS;
                }
            }
        }
        catch (boost::bad_lexical_cast) {
            result.first = INVALID_LAMP_ENABLE;
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
    if (m_seabreezeApi != 0) {
        int error = 0;

        int serialNumberFeatureCount = m_seabreezeApi->getNumberOfSerialNumberFeatures(m_handle, &error);
        if (serialNumberFeatureCount > 0) {
            m_serialNumberFeatures.resize(serialNumberFeatureCount);
            m_seabreezeApi->getSerialNumberFeatures(m_handle, &error, m_serialNumberFeatures.data(), serialNumberFeatureCount);
        }

        int spectrumProcessingFeatureCount = m_seabreezeApi->getNumberOfSpectrumProcessingFeatures(m_handle, &error);
        if (spectrumProcessingFeatureCount > 0) {
            m_spectrumProcessingFeatures.resize(spectrumProcessingFeatureCount);
            m_seabreezeApi->getSpectrumProcessingFeatures(m_handle, &error, m_spectrumProcessingFeatures.data(), spectrumProcessingFeatureCount);
        }

        int spectrometerFeatureCount = m_seabreezeApi->getNumberOfSpectrometerFeatures(m_handle, &error);
        if (spectrometerFeatureCount > 0) {
            m_spectrometerFeatures.resize(spectrometerFeatureCount);
            m_seabreezeApi->getSpectrometerFeatures(m_handle, &error, m_spectrometerFeatures.data(), spectrometerFeatureCount);

            m_pixelCount = m_seabreezeApi->spectrometerGetFormattedSpectrumLength(m_handle, m_spectrometerFeatures[0], &error);
            m_binnedPixelCount = m_pixelCount;

            int electricDarkPixelCount = m_seabreezeApi->spectrometerGetElectricDarkPixelCount(m_handle, m_spectrometerFeatures[0], &error);
            m_electricDarkIndex.resize(electricDarkPixelCount);

            int actualElectricDark = m_seabreezeApi->spectrometerGetElectricDarkPixelIndices(m_handle, m_spectrometerFeatures[0], &error, m_electricDarkIndex.data(), electricDarkPixelCount);
            m_electricDarkIndex.resize(actualElectricDark);

            m_spectrum.resize(m_pixelCount, 0.0);
            m_wavelengths.resize(m_pixelCount, 0.0);
            m_seabreezeApi->spectrometerGetWavelengths(m_handle, m_spectrometerFeatures[0], &error, m_wavelengths.data(), m_pixelCount);
        }

        if (serialNumberFeatureCount > 0) {
            unsigned char snSize = m_seabreezeApi->getSerialNumberMaximumLength(m_handle, m_serialNumberFeatures[0], &error);
            snSize = (snSize > 0) ? snSize : ms_maxSerialNumberCount;
            m_serialNumber.resize(snSize);
            int actual = m_seabreezeApi->getSerialNumber(m_handle, m_serialNumberFeatures[0], &error, const_cast<char *>(m_serialNumber.data()), snSize);
            if (actual <= 0) {
            }
            // inconsistent behaviour between spectrometers...
            else if (m_serialNumber[actual - 1] == '\0') {
                m_serialNumber.resize(actual - 1);
            }
            else {
                m_serialNumber.resize(actual);
            }
        }

        int pixelBinningFeatureCount = m_seabreezeApi->getNumberOfPixelBinningFeatures(m_handle, &error);
        if (pixelBinningFeatureCount > 0) {
            m_pixelBinningFeatures.resize(pixelBinningFeatureCount);
            m_seabreezeApi->getPixelBinningFeatures(m_handle, &error, m_pixelBinningFeatures.data(), pixelBinningFeatureCount);
        }

        int thermoElectricFeatureCount = m_seabreezeApi->getNumberOfThermoElectricFeatures(m_handle, &error);
        if (thermoElectricFeatureCount > 0) {
            m_thermoElectricFeatures.resize(thermoElectricFeatureCount);
            m_seabreezeApi->getThermoElectricFeatures(m_handle, &error, m_thermoElectricFeatures.data(), thermoElectricFeatureCount);
        }

        int lampFeatureCount = m_seabreezeApi->getNumberOfLampFeatures(m_handle, &error);
        if (lampFeatureCount > 0) {
            m_lampFeatures.resize(lampFeatureCount);
            m_seabreezeApi->getLampFeatures(m_handle, &error, m_lampFeatures.data(), lampFeatureCount);
        }

        m_integrationTime = m_configuration.IntegrationTime(m_serialNumber);
        if (spectrometerFeatureCount > 0) {
            long minIntegrationTime = m_seabreezeApi->spectrometerGetMinimumIntegrationTimeMicros(m_handle, m_spectrometerFeatures[0], &error);
            m_maxIntegrationTime = m_seabreezeApi->spectrometerGetMaximumIntegrationTimeMicros(m_handle, m_spectrometerFeatures[0], &error);

            m_integrationTime = std::max(m_minIntegrationTime, minIntegrationTime);
            m_seabreezeApi->spectrometerSetIntegrationTimeMicros(m_handle, m_spectrometerFeatures[0], &error, m_integrationTime);

            m_maxIntensity = m_seabreezeApi->spectrometerGetMaximumIntensity(m_handle, m_spectrometerFeatures[0], &error);
        }

        m_boxcarWidth = m_configuration.BoxcarWidth(m_serialNumber);
        m_scansToAverage = m_configuration.ScansToAverage(m_serialNumber);
        if (spectrumProcessingFeatureCount > 0) {
            // We are intercepting smoothing and averaging and handling them

            /*m_seabreezeApi->spectrumProcessingBoxcarWidthSet(m_handle, m_spectrumProcessingFeatures[0], &error, m_boxcarWidth);

            m_seabreezeApi->spectrumProcessingScansToAverageSet(m_handle, m_spectrumProcessingFeatures[0], &error, m_scansToAverage);*/
        }

        m_binningFactor = 0;
        m_applyElectricDark = false;
        char typeBuffer[ms_maxSpectrometerNameCount] = {0};
        m_seabreezeApi->getDeviceType(m_handle, &error, typeBuffer, ms_maxSpectrometerNameCount);
        m_name = std::string(typeBuffer);
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
    (SPECTROMETER_INDEX_DOES_NOT_EXIST, std::string("Invalid spectrometer number"))
    (INVALID_TEC_ENABLE, std::string("Invalid TEC enable value"))
    (INVALID_LAMP_ENABLE, std::string("Invalid lamp enable value"));
