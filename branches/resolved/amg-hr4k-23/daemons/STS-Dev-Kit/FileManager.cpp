/***************************************************//**
 * @file    FileManager.cpp
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * Filesystem specific functionality for saving sequences of acquisitions.
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

#include "FileManager.h"
#include "OceanHandlerConfiguration.h"
#include <iomanip>
#include <sstream>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/assign/list_of.hpp>

/* Save the spectrum to a file in either the default or OV_PLAIN format.
*/
void FileManager::SaveToFile(std::ofstream &out, const double *wavelengths, const double *spectrum, const int pixels,
    const long integration, const int average, const int boxcar, const long long millisecs) {

    if (m_saveFormat == DEFAULT) {
        out << "Saved at time: " << millisecs << " milliseconds\n";
        out << "Integration time: " << integration << '\n';
        out << "Scans to average: " << average << '\n';
        out << "Boxcar smoothing: " << boxcar << '\n';
        out << "Wavelengths\tIntensities" << '\n';
    }

    out << std::fixed << std::setprecision(m_precision);
    for (int i = 0; i < pixels; ++i) {
        out << /*std::fixed << std::setprecision(m_precision) <<*/ *wavelengths++ << '\t' << *spectrum++ << '\n';
    }
    out << std::endl;
}

/* Set the file save mode to either a single file (append each acquisition) or multiple files (one per acquisition).
*/
void FileManager::SetSaveMode(const bool multipleFiles) {
    if (!m_running) {
        m_multiple = multipleFiles;
    }
}

/* Handler for the completion of an acquisition event.
*/
void FileManager::OnAcquisition(const double *wavelengths, const double *spectrum, const int pixels,
    const long integration, const int average, const int boxcar,
    const long long millisecs, const int sequenceNumber) {

    if (m_multiple) {
        OpenSequenceNumber(sequenceNumber);
    }

    if (m_saveFormat == DEFAULT) {
        OutputSequenceNumber(m_output, sequenceNumber);
    }
    SaveToFile(m_output, wavelengths, spectrum, pixels, integration, average, boxcar, millisecs);

    if (m_multiple) {
        m_output.close();
    }
}

/* Handler for the sequence start event.
*/
void FileManager::OnStart() {
    m_running = true;
    if (!m_multiple) {
        OpenSequenceNumber(0);
    }
}

/* Handler for the sequence stop event.
*/
void FileManager::OnStop() {
    if (!m_multiple) {
        if (m_output.is_open()) {
            m_output.close();
        }
    }
    m_running = false;
}

/* Set the location that the sequence files will be saved into.
*/
void FileManager::SetSaveDirectory(const std::string &saveDirectory) {
    if (!m_running) {
        m_saveDirectory = saveDirectory;
    }
}

/* Set the file prefix to be prepended to the sequence file names.
*/
void FileManager::SetFilePrefix(const std::string &prefix) {
    if (!m_running) {
        m_prefix = prefix;
    }
}

/* Set the width of the integer part of the saved file names so that we can zero pad it.
*/
void FileManager::SetFileSequenceWidth(const int n) {
    m_width = n;
}

/* Set the save file extension.
*/
void FileManager::SetFileExtension(const std::string &extension) {
    m_extension = extension;
}

/* Open a file withe the given (zero padded if possible) sequence number and specified prefix and extension.
*/
void FileManager::OpenSequenceNumber(const int sequenceNumber) {
    std::string filename(m_saveDirectory);

    std::stringstream s;
    if (m_width > 0) {
        s.width(m_width);
    }
    s.fill('0');
    s << sequenceNumber;
    std::string number = s.str();

    filename.append("/").append(m_prefix).append(number).append(m_extension);
    // make the file append even if we are saving to multiple files...it will make no difference
    m_output.open(filename, std::ios::app);
}

/* Put the sequence number into the sequence file.
*/
void FileManager::OutputSequenceNumber(std::ofstream &out, const int sequenceNumber) {
    out << "Spectrum sequence number " << sequenceNumber << '\n';
}

/* Constructor. Initialise from the configuration.
*/
FileManager::FileManager(OceanHandlerConfiguration &config, const std::string &serialNumber) :
    m_multiple(config.SaveMultipleFiles(serialNumber)),
    m_running(false),
    m_width(0) {

    std::string saveFormat = config.SaveFormat(serialNumber);
    FormatMap::const_iterator format = std::find_if(ms_saveFormats.begin(), ms_saveFormats.end(),
        [&saveFormat](const std::pair<SaveFormatType, std::string> &t) {
            return t.second == saveFormat;
    });
    m_saveFormat = format != ms_saveFormats.end() ? format->first : DEFAULT;

    m_precision = config.SavePrecision(serialNumber);
}

FileManager::~FileManager() {
}

// map the save types to human readable strings
const std::map<FileManager::SaveFormatType, std::string> FileManager::ms_saveFormats = boost::assign::map_list_of
    (FileManager::DEFAULT, std::string("default"))
    (FileManager::OV_PLAIN, std::string("ov-plain"));

// the default file save format
std::string FileManager::DefaultFormatKey()  {
    size_t count = ms_saveFormats.size();
    return ms_saveFormats.at(DEFAULT);
}