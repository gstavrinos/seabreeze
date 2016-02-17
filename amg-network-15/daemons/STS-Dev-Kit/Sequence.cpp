/***************************************************//**
 * @file    Sequence.cpp
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * Provides the functionality to save a sequence of spectra at specified intervals.
 * Optionally the spectra can be saved into multiple files or a single file with
 * header information about e.g. the integration time.
 * A sequence is associated with a spectrometer so multiple sequences can be saved
 * concurrently if there is more than one spectrometer conencted.
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

#include "Sequence.h"
#include "IResponseHandler.h"
#include "Spectrometer.h"
#include "Common.h"
#include "OceanHandlerConfiguration.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/assign.hpp>
#include <iomanip>

/* Construct a message that defines a save spectrum "task" to be submitted to the active object held by the
 * spectrometer associated with this sequence. This is actually a one-off save that is not part of a sequence
 * and is intended for the dark/reference/immediate spectrum functionality. Its presence here is due to the
 * commonality of code and reducing the complexity of the design,
 */
Sequence::SaveSpectrumMessage::SaveSpectrumMessage(boost::shared_ptr<Connection> connection, Sequence *sequence,
    CommandType command, const std::string &arguments) :

    m_connection(connection),
    m_sequence(sequence),
    m_command(command),
    m_arguments(arguments) {
}

/* The method that is executed in the active object thread. This calls the command handler
* associated with the command code and then returns the response on the same connection that
* the request/comman was received on.
*/
void Sequence::SaveSpectrumMessage::Execute() {
    Sequence::Response response = m_sequence->HandleCommand(m_command, m_arguments);
    m_sequence->HandleResponse(m_connection, response.first, response.second);
}

/* Construct a message the defines a save sequence "task". This message causes one acquisition in the sequence
* to be saved.
*/
Sequence::SaveSequenceMessage::SaveSequenceMessage(Sequence *sequence) :

    m_sequence(sequence) {
}

/* The method that is executed in the active object thread. This calls the method that performs the actual save.
*/
void Sequence::SaveSequenceMessage::Execute() {
    m_sequence->SaveSequenceEntry();
}

/* Accept a command on the specified connection with the associated arguments. Check to see if it is a one-off save and handle
* this as a special case, otherwise pass the command to the handler.
*/
bool Sequence::Accept(boost::shared_ptr<Connection> connection, const CommandType command, const std::string &arguments) {
    bool result = false;
    if (IsValidCommand(command)) {
        switch (command) {
        case SAVE_SPECTRUM: {
                ActiveObject::MessagePtr p(new SaveSpectrumMessage(connection, this, command, arguments));
                m_spectrometer.Submit(p);
            }
            break;
        default:
            Response response = HandleCommand(command, arguments);
            HandleResponse(connection, response.first, response.second);
            break;
        }
        result = true;
    }
    return result;
}

/* return true if the command is one we know about i.e. it is mapped here to a hanlder method.
*/
bool Sequence::IsValidCommand(const CommandType command) {
    return m_command.find(command) != m_command.end();
}

/* if this is a known command, execute the mapped handler method and return the response, otherwise return an "empty" response.
*/
Sequence::Response Sequence::HandleCommand(const CommandType command, const std::string &arguments) {
    Response result = IsValidCommand(command) ? (this->*m_command.at(command))(arguments) : Response(0, "");
    return result;
}

/* Handle the response from the execution of the command. Pass the response to the response handler notify callback.
*/
void Sequence::HandleResponse(boost::shared_ptr<Connection> connection, const int resultCode, const std::string &response) {
    m_responseHandler->NotifyResponse(connection, resultCode, response);
}

/* Return the last (cached) acquired spectrum in this sequence. This functionality allows the scope mode to query for a spectrum
* without the need to interrupt any running sequene of acquisition.
*/
Sequence::Response Sequence::GetCurrentSpectrum(const std::string &arguments) {
    m_resultCode = SUCCESS;
    std::string spectrum;
    {
        boost::mutex::scoped_lock lock(m_mutex);
        spectrum = ToString(m_cachedSpectrum);
    }
    // put the length of the string into the first 4 characters of the reponse MSB first
    const int sizeCount = 4;
    unsigned char size[sizeCount];
    size_t s = spectrum.size();
    size[0] = (s >> 24) & 0xFF;
    size[1] = (s >> 16) & 0xFF;
    size[2] = (s >> 8) & 0xFF;
    size[3] = s & 0xFF;
    std::string result;
    result.resize(spectrum.size() + sizeCount);
    std::copy(size, size + sizeCount, result.begin());
    std::copy(spectrum.begin(), spectrum.end(), result.begin() + sizeCount);
    return Response(m_resultCode, result);
}

/* Parse the arguments for a valid maximum number of acquisitions for this sequence. Set the width of the number
* sequence in the file name to allow for this. If this is not known we cannot put leading zeros
* in the numeric part and the files will not be listed in order.
*/
Sequence::Response Sequence::SetMaximumAcquisitions(const std::string &arguments) {
    m_resultCode = SUCCESS;
    std::string maxAcquisitions(arguments.begin(), arguments.end());
    try {
        int m = boost::lexical_cast<int>(maxAcquisitions);
        if (m < 0) {
            m_resultCode = INVALID_MAX_ACQUISITIONS;
        }
        else {
            m_maxAcquisitions = m;
            double dWidth = std::ceil(log10(static_cast<double>(m_maxAcquisitions))) + 1;
            int width = static_cast<int>(dWidth);
            m_fileManager.SetFileSequenceWidth(width);
        }
    }
    catch (boost::bad_lexical_cast) {
        m_resultCode = UNABLE_TO_PARSE_PARAMETER;
    }

    return Response(m_resultCode, maxAcquisitions);
}

/* Return the maximum number of acquisitions for this sequence.
*/
Sequence::Response Sequence::GetMaximumAcquisitions(const std::string &arguments) {
    m_resultCode = SUCCESS;
    std::string maxAcquisitions;
    try {
        maxAcquisitions = boost::lexical_cast<std::string>(m_maxAcquisitions);
    }
    catch (boost::bad_lexical_cast) {
        m_resultCode = UNEXPECTED_MAX_ACQUISITIONS;
    }
    return Response(m_resultCode, maxAcquisitions);
}

/* Return the current (numeric) position in this sequence of acquisitions
*/
Sequence::Response Sequence::GetCurrentSequenceNumber(const std::string &arguments) {
    m_resultCode = SUCCESS;
    std::string sequenceNumber;
    try {
        sequenceNumber = boost::lexical_cast<std::string>(m_acquisitionCount);
    }
    catch (boost::bad_lexical_cast) {
        m_resultCode = UNEXPECTED_ACQUISITION_COUNT;
    }
    return Sequence::Response(m_resultCode, sequenceNumber);
}

/* Set the file save mode to multiple files or a single file for this sequence.
*/
Sequence::Response Sequence::SetFileSaveMode(const std::string &arguments) {
    m_resultCode = SUCCESS;

    std::string saveMode(arguments.begin(), arguments.end());
    boost::to_lower(saveMode);

    /* Check to find the file save mode value in the map. If it is not there we have an invalid argument.
    */
    auto mode = std::find_if(ms_saveModes.begin(), ms_saveModes.end(), [&saveMode](const std::pair<SaveMode, std::string> &m) {
        return m.second == saveMode;
    });

    if (mode != ms_saveModes.end()) {
        m_saveMode = mode->first;
        m_fileManager.SetSaveMode(m_saveMode == MULTI);
    }
    else {
        m_resultCode = INVALID_SAVE_MODE;
    }

    return Response(m_resultCode, saveMode);
}

/* Return the current file save mode for this sequence.
*/
Sequence::Response Sequence::GetFileSaveMode(const std::string &arguments) {
    m_resultCode = SUCCESS;
    return Response(m_resultCode, ms_saveModes.at(m_saveMode));
}

/* Return the filename prefix for this sequence.
*/
Sequence::Response Sequence::GetFilePrefix(const std::string &arguments) {
    m_resultCode = SUCCESS;
    return Response(m_resultCode, m_filePrefix);
}

/* Set the filename prefix for this sequence.
*/
Sequence::Response Sequence::SetFilePrefix(const std::string &arguments) {
    m_resultCode = SUCCESS;
    m_filePrefix = std::string(arguments.begin(), arguments.end());
    m_fileManager.SetFilePrefix(m_filePrefix);
    return Response(m_resultCode, m_filePrefix);
}

/* Set the type of this sequence to timed or manual acquisition.
*/
Sequence::Response Sequence::SetSequenceType(const std::string &arguments) {
    m_resultCode = SUCCESS;

    std::string sequenceType(arguments.begin(), arguments.end());
    boost::to_lower(sequenceType);

    /* Check to find the sequence type in the map. If it is not there we have an invalid argument.
    */
    auto sequence = std::find_if(ms_sequenceTypes.begin(), ms_sequenceTypes.end(), [&sequenceType](const std::pair<SequenceType, std::string> &s) {
        return s.second == sequenceType;
    });

    if (sequence != ms_sequenceTypes.end()) {
        m_sequenceType = sequence->first;
    }
    else {
        m_resultCode = INVALID_SEQUENCE_TYPE;
    }

    return Response(m_resultCode, sequenceType);
}

/* Return the type (manual or timed) of this sequence.
*/
Sequence::Response Sequence::GetSequenceType(const std::string &arguments) {
    m_resultCode = SUCCESS;
    return Response(m_resultCode, ms_sequenceTypes.at(m_sequenceType));
}

/* Return the interval in miliseconds between acquisitions in this sequence.
*/
Sequence::Response Sequence::GetSequenceInterval(const std::string &arguments) {
    m_resultCode = SUCCESS;
    std::string interval = boost::lexical_cast<std::string>(m_sequenceInterval);
    return Sequence::Response(m_resultCode, interval);
}

/* Set the interval in milliseconds between acquisitions in this sequence.
*/
Sequence::Response Sequence::SetSequenceInterval(const std::string &arguments) {
    m_resultCode = SUCCESS;
    std::string interval(arguments.begin(), arguments.end());
    try {
        int sequenceInterval = boost::lexical_cast<int>(interval);
        if (sequenceInterval <= 0) {
            m_resultCode = INVALID_SEQUENCE_INTERVAL;
        }
        else {
            m_sequenceInterval = sequenceInterval;
        }
    }
    catch (boost::bad_lexical_cast) {
        m_resultCode = UNABLE_TO_PARSE_PARAMETER;
    }

    return Response(m_resultCode, interval);
}

/* Set the directory we will save files in sequence into.
*/
Sequence::Response Sequence::SetSaveDirectory(const std::string &arguments) {
    m_resultCode = SUCCESS;

    using namespace boost::filesystem;
    const perms writeable = owner_write | group_write | others_write;

    std::string directory(arguments.begin(), arguments.end());
    if (!exists(directory)) {
        m_resultCode = INVALID_PATH;
    }
    else if (!is_directory(directory)) {
        m_resultCode = NOT_A_DIRECTORY;
    }
    else if ((status(directory).permissions() & writeable) != writeable) {
        m_resultCode = DIRECTORY_NOT_WRITEABLE;
    }
    else {
        m_saveDirectory = directory;
        m_fileManager.SetSaveDirectory(m_saveDirectory);
    }

    return Response(m_resultCode, directory);
}

/* Return the directory that files in this sequence are being saved into.
*/
Sequence::Response Sequence::GetSaveDirectory(const std::string &arguments) {
    m_resultCode = SUCCESS;
    return Response(m_resultCode, m_saveDirectory);
}

/* Save an immediate spectrum i.e. not part of a sequence. This is intended for ad-hoc measurements
* such as re-referencing.
*/
Sequence::Response Sequence::SaveSpectrum(const std::string &arguments) {
    m_resultCode = SUCCESS;

    using namespace boost::filesystem;

    std::string pathName(arguments.begin(), arguments.end());
    boost::filesystem::path saveLocation(pathName);
    if (!exists(saveLocation.parent_path())) {
        m_resultCode = INVALID_PATH;
    }
    else {
        if (saveLocation.extension().string() != m_configuration.SaveFileExtension(m_serialNumber)) {
            // Append the extension this way for backward compatibility to older versions of Boost
            saveLocation = boost::filesystem::path(pathName + m_configuration.SaveFileExtension(m_serialNumber));
        }

        boost::posix_time::time_duration delta = boost::posix_time::microsec_clock::local_time() - m_timeStart;

        std::vector<double> w = m_spectrometer.GetWavelengths();
        std::vector<double> s = m_spectrometer.GetSpectrum();
        {
            boost::mutex::scoped_lock lock(m_mutex);
            m_cachedSpectrum = s;
        }

        std::ofstream out(saveLocation.string(), std::ios::app);
        if (out.is_open()) {
            SaveSpectrumToFile(out, w, s, m_spectrometer.BinnedPixelCount(), m_spectrometer.IntegrationTime(),
                m_spectrometer.ScansToAverage(), m_spectrometer.BoxcarWidth(), delta.total_milliseconds());
            out.close();
        }
        else {
            m_resultCode = UNABLE_TO_OPEN_FILE;
        }
    }

    return Response(m_resultCode, pathName);
}

/* Save a spectrum to a file with header information
*/
void Sequence::SaveSpectrumToFile(std::ofstream &out, const std::vector<double> &w, const std::vector<double> &s,
    const int length,  const long integration, const int average, const int boxcar, const long long millisecs) {

    m_fileManager.SaveToFile(out, w.data(), s.data(), length, integration, average, boxcar, millisecs);
}

/* Stop this sequence of acquisitions. This is called by the request handler StopSequence.
*/
void Sequence::StopSequence() {
    if (m_timer) {
        m_timer->cancel();
    }
    m_fileManager.OnStop();
    m_sequenceState = NOT_YET_STARTED;
}

/* Start this sequence of acquisitions.
*/
Sequence::Response Sequence::StartSequence(const std::string &arguments) {

    m_resultCode = SUCCESS;
    std::string response("not started");

    // compare the total acquisition time with the sequence interval to make
    // sure we don't request a spectrum too often
    long acquisitionInterval = m_spectrometer.IntegrationTime() * m_spectrometer.ScansToAverage();
    long sequenceInterval = m_sequenceInterval * 1000; // convert to microseconds for comparison
    if (sequenceInterval < acquisitionInterval) {
        m_resultCode = SEQUENCE_ACQUISITION_CONFLICT;
    }
    else {
        m_timer.reset(new boost::asio::deadline_timer(m_ioService));
        m_sequenceState = ACTIVE;
        m_acquisitionCount = 0;
        m_fileManager.OnStart();
        m_timer->expires_from_now(boost::posix_time::milliseconds(m_sequenceInterval));
        m_timer->async_wait(boost::bind(&Sequence::GetSpectrum, boost::ref(*this), boost::asio::placeholders::error, m_timer.get()));
        response = "started";
    }

    return Response(m_resultCode, response);
}

/* Pause this sequence of acquisitions.
*/
Sequence::Response Sequence::PauseSequence(const std::string &arguments) {
    m_resultCode = SUCCESS;
    m_sequenceState = PAUSED;
    if (m_timer) {
        m_timer->cancel();
    }
    return Response(m_resultCode, "paused");
}

/* Resume this sequence of acquisitions after a pause.
*/
Sequence::Response Sequence::ResumeSequence(const std::string &arguments) {
    m_resultCode = SUCCESS;
    m_sequenceState = ACTIVE;
    m_timer->expires_from_now(boost::posix_time::milliseconds(m_sequenceInterval));
    m_timer->async_wait(boost::bind(&Sequence::GetSpectrum, boost::ref(*this), boost::asio::placeholders::error, m_timer.get()));
    return Sequence::Response(m_resultCode, "resumed");
}

/* Stop the sequence of acquisitions
*/
Sequence::Response Sequence::StopSequence(const std::string &arguments) {
    m_resultCode = SUCCESS;
    StopSequence();
    return Response(m_resultCode, "stopped");
}

/* Return the current state of the acquisition sequence e.g not yet started, paused etc.
*/
Sequence::Response Sequence::GetSequenceState(const std::string &arguments) {
    m_resultCode = SUCCESS;
    return Response(m_resultCode, ms_sequenceStates.at(m_sequenceState));
}

/* Set the pseudo scope mode on/off
*/
Sequence::Response Sequence::SetScopeMode(const std::string &arguments) {
    m_resultCode = SUCCESS;
    std::string mode(arguments.begin(), arguments.end());
    boost::to_lower(mode);
    if (mode == "on") {
        m_scopeMode = true;
    }
    else if (mode == "off") {
        m_scopeMode = false;
    }
    else {
        m_resultCode = INVALID_SCOPE_MODE;
    }
    return Response(m_resultCode, mode);
}

/* Return the scope mode state, on or off.
*/
Sequence::Response Sequence::GetScopeMode(const std::string &arguments) {
    m_resultCode = SUCCESS;
    std::string response = m_scopeMode ? "on" : "off";
    return Response(m_resultCode, response);
}

/* Set the interval between scope mode refreshes in seconds.
*/
Sequence::Response Sequence::SetScopeInterval(const std::string &arguments) {
    m_resultCode = SUCCESS;
    std::string number(arguments.begin(), arguments.end());
    try {
        int scopeInterval = boost::lexical_cast<int>(number);
        if (scopeInterval <= 0) {
            m_resultCode = INVALID_SCOPE_INTERVAL;
        }
        else {
            m_scopeInterval = scopeInterval;
        }
    }
    catch (boost::bad_lexical_cast) {
        m_resultCode = UNABLE_TO_PARSE_PARAMETER;
    }
    return Sequence::Response(m_resultCode, number);
}

/* Return the scope mode refresh interval in seconds.
*/
Sequence::Response Sequence::GetScopeInterval(const std::string &arguments) {
    m_resultCode = SUCCESS;
    std::string number;
    try {
        number = boost::lexical_cast<std::string>(m_scopeInterval);
    }
    catch (boost::bad_lexical_cast) {
        ;// really shouldn't get here
    }
    return Sequence::Response(m_resultCode, number);
}

/* Return the status of the last command/request. This handler resets the status to SUCCESS on exit.
*/
Sequence::Response Sequence::GetCurrentStatus(const std::string &arguments) {
    int resultCode = m_resultCode;
    std::string resultMessage = ms_results.find(resultCode) != ms_results.end() ? ms_results.at(resultCode) : "";
    m_resultCode = SUCCESS;

    return Sequence::Response(resultCode, resultMessage);
}

/* Set the time we will use to take as the zero reference for acquisition timestamps
*/
void Sequence::SetTimeZero(boost::posix_time::ptime t) {
    m_timeStart = t;
}

/* Save one entry in the sequence. Stop if there is a maximum number of acquisitions and we have reached it.
*/
void Sequence::SaveSequenceEntry() {

    boost::posix_time::time_duration delta = boost::posix_time::microsec_clock::local_time() - m_timeStart;
    std::vector<double> spectrum(m_spectrometer.BinnedPixelCount());
    int error = 0;
    m_spectrometer.GetSpectrum(spectrum, error);

    if (m_maxAcquisitions == 0 || m_acquisitionCount < m_maxAcquisitions) {
        m_fileManager.OnAcquisition(m_spectrometer.GetWavelengths().data(), spectrum.data(), m_spectrometer.BinnedPixelCount(),
            m_spectrometer.IntegrationTime(), m_spectrometer.ScansToAverage(), m_spectrometer.BoxcarWidth(),
            delta.total_milliseconds(), m_acquisitionCount);
    }

    {
        boost::mutex::scoped_lock lock(m_mutex);
        std::swap(m_cachedSpectrum, spectrum);
    }

    ++m_acquisitionCount;

    if (m_maxAcquisitions && m_acquisitionCount >= m_maxAcquisitions) {
        StopSequence();
    }
}

/* Queue a task with the spectrometer to acquire a spectrum.
*/
void Sequence::GetSpectrum(const boost::system::error_code &code, boost::asio::deadline_timer *t) {
    if (m_sequenceState == ACTIVE) {
        t->expires_at(t->expires_at() + boost::posix_time::milliseconds(m_sequenceInterval));
        t->async_wait(boost::bind(&Sequence::GetSpectrum, boost::ref(*this), boost::asio::placeholders::error, t));
        ActiveObject::MessagePtr p(new SaveSequenceMessage(this));
        m_spectrometer.Submit(p);
    }
}

/* Constructor. Iniitalise sequence parameters from the configuration and map the command handlers.
*/
Sequence::Sequence(IResponseHandler *responseHandler,  boost::asio::io_service &ioService,
    Spectrometer &spectrometer, OceanHandlerConfiguration &configuration) :
    
    m_responseHandler(responseHandler),
    m_ioService(ioService),
    m_spectrometer(spectrometer),
    m_serialNumber(m_spectrometer.SerialNumber()),
    m_configuration(configuration),
    m_sequenceState(NOT_YET_CONFIGURED),
    m_sequenceType(TIMER),
    m_sequenceInterval(m_configuration.SaveInterval(m_serialNumber)),
    m_saveMode(m_configuration.SaveMultipleFiles(m_serialNumber) ? MULTI : SINGLE),
    m_maxAcquisitions(m_configuration.MaxAcquisitions(m_serialNumber)),
    m_scopeMode(m_configuration.ShowScopeMode(m_serialNumber)),
    m_scopeInterval(m_configuration.RefreshInterval(m_serialNumber)),
    m_filePrefix(m_configuration.SaveFilePrefix(m_serialNumber)),
    m_fileManager(m_configuration, m_serialNumber) {

    m_fileManager.SetFileExtension(m_configuration.SaveFileExtension(m_serialNumber));

    m_configuration.DoUpdate();

    m_cachedSpectrum = std::vector<double>(m_spectrometer.PixelCount(), 0.0);

    m_command.insert(Handler(GET_CURRENT_STATUS, &Sequence::GetCurrentStatus));
    m_command.insert(Handler(GET_CURRENT_SPECTRUM, &Sequence::GetCurrentSpectrum));

    m_command.insert(Handler(SET_MAX_ACQUISITIONS, &Sequence::SetMaximumAcquisitions));
    m_command.insert(Handler(GET_MAX_ACQUISITIONS, &Sequence::GetMaximumAcquisitions));

    m_command.insert(Handler(SET_FILE_SAVE_MODE, &Sequence::SetFileSaveMode));
    m_command.insert(Handler(GET_FILE_SAVE_MODE, &Sequence::GetFileSaveMode));

    m_command.insert(Handler(SET_FILE_PREFIX, &Sequence::SetFilePrefix));
    m_command.insert(Handler(GET_FILE_PREFIX, &Sequence::GetFilePrefix));

    m_command.insert(Handler(SET_SEQUENCE_TYPE, &Sequence::SetSequenceType));
    m_command.insert(Handler(GET_SEQUENCE_TYPE, &Sequence::GetSequenceType));

    m_command.insert(Handler(SET_SEQUENCE_INTERVAL, &Sequence::SetSequenceInterval));
    m_command.insert(Handler(GET_SEQUENCE_INTERVAL, &Sequence::GetSequenceInterval));

    m_command.insert(Handler(SET_SAVE_DIRECTORY, &Sequence::SetSaveDirectory));
    m_command.insert(Handler(GET_SAVE_DIRECTORY, &Sequence::GetSaveDirectory));

    m_command.insert(Handler(SAVE_SPECTRUM, &Sequence::SaveSpectrum));
    m_command.insert(Handler(START_SEQUENCE, &Sequence::StartSequence));
    m_command.insert(Handler(PAUSE_SEQUENCE, &Sequence::PauseSequence));
    m_command.insert(Handler(RESUME_SEQUENCE, &Sequence::ResumeSequence));
    m_command.insert(Handler(STOP_SEQUENCE, &Sequence::StopSequence));
    m_command.insert(Handler(GET_SEQUENCE_STATE, &Sequence::GetSequenceState));

    m_command.insert(Handler(GET_CURRENT_SEQUENCE_NUMBER, &Sequence::GetCurrentSequenceNumber));
    m_command.insert(Handler(SET_SCOPE_MODE, &Sequence::SetScopeMode));
    m_command.insert(Handler(GET_SCOPE_MODE, &Sequence::GetScopeMode));
    m_command.insert(Handler(SET_SCOPE_INTERVAL, &Sequence::SetScopeInterval));
    m_command.insert(Handler(GET_SCOPE_INTERVAL, &Sequence::GetScopeInterval));

    m_command.insert(Handler(GET_SEQUENCE_STATUS, &Sequence::GetCurrentStatus));
}

Sequence::~Sequence() {
}

// map the sequence type values to human readable strings
const std::map<Sequence::SequenceType, std::string> Sequence::ms_sequenceTypes = boost::assign::map_list_of
    (MANUAL, std::string("manual"))
    (TIMER, std::string("timer"));

// map the sequence state values to human readable strings
const std::map<Sequence::SequenceState, std::string> Sequence::ms_sequenceStates = boost::assign::map_list_of
    (NOT_YET_CONFIGURED, std::string("not_yet_configured"))
    (NOT_YET_STARTED, std::string("not_yet_started"))
    (PAUSED, std::string("paused"))
    (ACTIVE, std::string("active"));

// map the sequence type values to human readable strings
const std::map<Sequence::SaveMode, std::string> Sequence::ms_saveModes = boost::assign::map_list_of
    (MULTI, std::string("multi"))
    (SINGLE, std::string("single"));

// map the error codes to human readable strings
const std::map<int, std::string> Sequence::ms_results = boost::assign::map_list_of
    (SUCCESS, std::string("Success"))
    (UNABLE_TO_PARSE_PARAMETER, std::string("Unable to decode the parameters sent to the API"))
    (INVALID_MAX_ACQUISITIONS, std::string("Invalid maximum acquisitions"))
    (UNEXPECTED_MAX_ACQUISITIONS, std::string("Unexpected maximum acquisitions"))
    (INVALID_SAVE_MODE, std::string("Invalid save mode"))
    (INVALID_SEQUENCE_TYPE, std::string("Invalid sequence type"))
    (INVALID_SEQUENCE_INTERVAL, std::string("Invalid sequence interval"))
    (INVALID_PATH, std::string("Invalid file path"))
    (NOT_A_DIRECTORY, std::string("Path is not a directory"))
    (DIRECTORY_NOT_WRITEABLE, std::string("Directory is not writeable"))
    (UNABLE_TO_OPEN_FILE, std::string("Unable to open file"))
    (INVALID_SCOPE_MODE, std::string("Invalid scope mode"))
    (INVALID_SCOPE_INTERVAL, std::string("Invalid scope interval"))
    (SEQUENCE_ACQUISITION_CONFLICT, std::string("Sequence interval shorter than total acquisition interval"))
    (UNEXPECTED_ACQUISITION_COUNT, std::string("Unexpected sequence number"));