/***************************************************//**
 * @file    Sequence.h
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * This class provides the functionality for generating a sequence of acquisitions
 * e.g. set the location of the saved files, file prefix, acquisition frequency and so on.
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

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Connection.h"
#include "ActiveObject.h"
#include "FileManager.h"
#include <map>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

class Spectrometer;
class OceanHandlerConfiguration;
class IResponseHandler;

class Sequence {
public:
    // we map the received commands to the member methods using CommandValue
    enum CommandValue {
        // Sequence commands
        GET_CURRENT_STATUS = 0x20,
        GET_CURRENT_SPECTRUM,
        SET_MAX_ACQUISITIONS,
        GET_MAX_ACQUISITIONS,
        SET_FILE_SAVE_MODE,
        GET_FILE_SAVE_MODE,
        SET_FILE_PREFIX,
        GET_FILE_PREFIX,
        SET_SEQUENCE_TYPE,
        GET_SEQUENCE_TYPE,
        SET_SEQUENCE_INTERVAL,
        GET_SEQUENCE_INTERVAL,
        SET_SAVE_DIRECTORY,
        GET_SAVE_DIRECTORY,
        SAVE_SPECTRUM,
        START_SEQUENCE,
        PAUSE_SEQUENCE,
        RESUME_SEQUENCE,
        STOP_SEQUENCE,
        GET_SEQUENCE_STATE,
        GET_CURRENT_SEQUENCE_NUMBER,
        SET_SCOPE_MODE,
        GET_SCOPE_MODE,
        SET_SCOPE_INTERVAL,
        GET_SCOPE_INTERVAL,
        GET_SEQUENCE_STATUS,
        LAST_COMMAND_VALUE
    };

    // The result of the command(s)
    enum ResultCode {
        SUCCESS = 1,
        UNABLE_TO_PARSE_PARAMETER,
        INVALID_MAX_ACQUISITIONS,
        UNEXPECTED_MAX_ACQUISITIONS,
        INVALID_SAVE_MODE,
        INVALID_SEQUENCE_TYPE,
        INVALID_SEQUENCE_INTERVAL,
        INVALID_PATH,
        NOT_A_DIRECTORY,
        DIRECTORY_NOT_WRITEABLE,
        UNABLE_TO_OPEN_FILE,
        INVALID_SCOPE_MODE,
        INVALID_SCOPE_INTERVAL,
        SEQUENCE_ACQUISITION_CONFLICT,
        UNEXPECTED_ACQUISITION_COUNT,

        LAST_RESULT_CODE
    };

    // Public convenience typedefs
    typedef unsigned short CommandType;
    typedef std::pair<int, std::string> Response;
    typedef Response (Sequence::*Command)(const std::string &arguments);
    typedef std::map<CommandType, Command> CommandMap;
    typedef CommandMap::value_type Handler;

    // The message class we will use to pass save spectrum commands to the active object thread
    class SaveSpectrumMessage : public ActiveObject::Message {
    public:
        void Execute();
        SaveSpectrumMessage(boost::shared_ptr<Connection> connection,
            Sequence *sequence,
            CommandType command,
            const std::string &arguments);
    private:
        boost::shared_ptr<Connection> m_connection;
        Sequence *m_sequence;
        CommandType m_command;
        std::string m_arguments;
    };

    // The message class we will use to pass save sequence commands to the active object thread
    class SaveSequenceMessage : public ActiveObject::Message {
    public:
        void Execute();
        SaveSequenceMessage(Sequence *sequence);
    private:
        Sequence *m_sequence;
    };

    /* Callback (from the active object thread) that does the actual saving of a spectrum to a file.
    */
    void SaveSequenceEntry();

    /* Accept a command with optional arguments. The connection is also supplied in order to reply with the response value.
    *  Return true if the command is successfully handled, false otherwise.
    */
    virtual bool Accept(boost::shared_ptr<Connection> connection, const CommandType command, const std::string &arguments);
    
    /* Return true if the command is one we can handle, false otherwise
    */
    bool IsValidCommand(const CommandType command);

    /* Execute the handler associated with the command, supplied with the spcified arguments.
    */
    Response HandleCommand(const CommandType command, const std::string &arguments);

    /* Return the response to the command on the same connection
    */
    void HandleResponse(boost::shared_ptr<Connection> connection, const int resultCode, const std::string &response);

    /* Handler to return the last acquired spectrum. Note this does not initiate an acqusition, it only returns the last spectrum acquired.
    * This will service requests to display the latest spectrum
    * without interrupting the programmed sequence of acquisitions.
    */
    Response GetCurrentSpectrum(const std::string &arguments);

    /* Handler for setting the maxiumum number of saved acquisitions to prevent us
    * accidentally filling the SD card.
    */
    Response SetMaximumAcquisitions(const std::string &arguments);

    /* Handler for getting the maxiumum number of saved acquisitions.
    */
    Response GetMaximumAcquisitions(const std::string &arguments);

    /* Handler for getting the current acquisition number
    */
    Response GetCurrentSequenceNumber(const std::string &arguments);

    /* Handler for setting the acquisitions to be saved to multiple files or a single file.
    */
    Response SetFileSaveMode(const std::string &arguments);

    /* Handler for getting the acquisitions to be saved  to multiple files or a single file.
    */
    Response GetFileSaveMode(const std::string &arguments);

    /* Handler for the set file prefix command.
    */
    Response SetFilePrefix(const std::string &arguments);

    /* Handler for the get file prefix command.
    */
    Response GetFilePrefix(const std::string &arguments);

    /* Handler for setting the sequence type i.e. manually initiating each acquisition
    * or periodic acquisition based on a timer.
    */
    Response SetSequenceType(const std::string &arguments);

    /* Handler for getting the sequence type: manual or timer.
    */
    Response GetSequenceType(const std::string &arguments);

    /* Handler for the set sequence interval command.
    */
    Response SetSequenceInterval(const std::string &arguments);

    /* Handler for the get sequence interval command.
    */
    Response GetSequenceInterval(const std::string &arguments);

    /* Handler for setting the directory we will save spectra into.
    * This assumes that the paths exists, is a directory and is writable by all,
    * otherwise a failure is reported.
    */
    Response SetSaveDirectory(const std::string &arguments);

    /* Handler for getting the directory we will save spectra into.
    */
    Response GetSaveDirectory(const std::string &arguments);

    /* Handler for the save spectrum command. This will be used for e.g. dark and reference spectra
    * which are expected to be triggered manually. It will also be used for manual acquisition
    * of normal spectra. If we are running a sequence of acquisitions on a timer the command will be
    * ignored and a warning status generated. The parameter should contain location to save the result.
    */
    Response SaveSpectrum(const std::string &arguments);

    /* Handler for the start sequence of acquisitions command. If we are not correctly configured it
    * will be ignored and generate a warning.
    */
    Response StartSequence(const std::string &arguments);

    /* Handler for the pause sequence command. If we are not running a sequence this will be ignored.
    */
    Response PauseSequence(const std::string &arguments);

    /* Handler for the resume sequence command.
    */
    Response ResumeSequence(const std::string &arguments);

    /* Handler for the stop sequence command. If we are not running a sequence this will be ignored.
    */
    Response StopSequence(const std::string &arguments);

    /* Handler for the get sequence state query.
    */
    Response GetSequenceState(const std::string &arguments);

    /* Handler for the set scope mode on/off setting
    */
    Response SetScopeMode(const std::string &arguments);

    /* Handler for the get scope mode on/off setting
    */
    Response GetScopeMode(const std::string &arguments);

    /* Handler for the set scope mode interval
    */
    Response SetScopeInterval(const std::string &arguments);

    /* Handler for the get scope mode interval
    */
    Response GetScopeInterval(const std::string &arguments);

    /* Return the current status of the sequence as a meaningful string
    */
    Response GetCurrentStatus(const std::string &arguments);

    /* Set the time that all acquisition timestamps are relative to.
    */
    void SetTimeZero(boost::posix_time::ptime t);

    /* Constructor. Set up the handler for responses, the IO service , spectrometer that we will be using to acquire
    * for this sequence and the configuration.
    */
    Sequence(IResponseHandler *responseHandler, boost::asio::io_service &ioService,
        Spectrometer &spectrometer, OceanHandlerConfiguration &configuration);
    virtual ~Sequence();

private:
    /* Member function that is queued to perform an acquisition.
    */
    void GetSpectrum(const boost::system::error_code &code, boost::asio::deadline_timer *t);

    /* Save a spectrum to a file with some header information e.g. integration time, scans to average and so on.
    */
    void SaveSpectrumToFile(std::ofstream &out, const std::vector<double> &w, const std::vector<double> &s,
        const int length,  const long integration, const int average, const int boxcar, const long long millisecs);

    /* Stop the sequence of acquisitions.
    */
    void StopSequence();

    IResponseHandler *m_responseHandler;
    boost::asio::io_service &m_ioService;

    // Reference to the spectrometer associated with this acquisition sequence
    Spectrometer &m_spectrometer;
    // Put the spectrometer serial number in here so we don't have to keep calling a function
    std::string m_serialNumber;

    // Reference to the configuration
    OceanHandlerConfiguration &m_configuration;

    // The map of command values to handlers
    CommandMap m_command;

    enum SequenceState {NOT_YET_CONFIGURED, NOT_YET_STARTED, PAUSED, ACTIVE} m_sequenceState;

    static const std::map<SequenceState, std::string> ms_sequenceStates;

    // Allow the acquisition sequence to be either on a timer or manual.
    enum SequenceType {MANUAL, TIMER} m_sequenceType;

    static const std::map<SequenceType, std::string> ms_sequenceTypes;

    // The time between acquisitions in milliseconds
    int m_sequenceInterval;

    // Allow the acquisition to be saved to separate files or a single file
    enum SaveMode {MULTI, SINGLE} m_saveMode;

    static const std::map<SaveMode, std::string> ms_saveModes;

    // Allow an upper limit of acquisitions to be set to prevent us filling the SD card
    int m_maxAcquisitions;

    // The current count of saved acquisitions
    int m_acquisitionCount;

    // Store the scope mode on/off setting.
    bool m_scopeMode;

    // Store the scope mode refresh interval
    int m_scopeInterval;

    // The directory we will save spectra into, supplied by the user. If the supplied directory does
    // not exist or is not writable an error condition is set.
    std::string m_saveDirectory;

    // Filename prefix for saved spectra
    std::string m_filePrefix;

    std::vector<double> m_cachedSpectrum;

    // Keep track of the time at construction. All times will be relative to this.
    boost::posix_time::ptime m_timeStart;
    // On acquisition start we calculate the time since m_timeStart for the timestamp
    boost::posix_time::time_duration m_timeDelta;

    FileManager m_fileManager;

    int m_resultCode;
    // Map the result codes to meaningful strings
    static const std::map<int, std::string> ms_results;

    std::unique_ptr<boost::asio::deadline_timer> m_timer;

    boost::mutex m_mutex;
};
#endif