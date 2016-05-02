/***************************************************//**
 * @file    FileManager.h
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * Provide the file handling for single or multiple result files, saving
 * a sequence of spectra. Checking of most of the parameters is done elsewhere
 * in this version - this should be moved in here in a later version.
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

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fstream>
#include <string>
#include <map>

class OceanHandlerConfiguration;

class FileManager {
public:
    // default file format includes header information, OV plain is two columns with wavelengths and intensities.
    enum SaveFormatType {DEFAULT, OV_PLAIN};

    static std::string DefaultFormatKey();

    /* Save a spectrum to a file.
    */
    void SaveToFile(std::ofstream &out, const double *wavelengths, const double *spectrum, const int pixels,
        const long integration, const int average, const int boxcar, const long long millisecs);

    /* Set the save mode i.e. to multiple files or a single file.
    *  multipleFiles On entry, true if we are saving to multiple files, false otherwise.
    */
    void SetSaveMode(const bool multipleFiles);

    /* Handle an acquisition. Save the spectrum to the file.
    */
    void OnAcquisition(const double *wavelengths, const double *spectrum, const int pixels,
        const long integration, const int average, const int boxcar,
        const long long millisecs, const int sequenceNumber);

    /* Handle the start sequence event.
    */
    void OnStart();

    /* Handle the stop sequence event.
    */
    void OnStop();

    /* Set the directory we will save to.
    */
    void SetSaveDirectory(const std::string &saveDirectory);

    /* Set the file prefix to be prepended to the file name.
    */
    void SetFilePrefix(const std::string &prefix);

    /* Set the width of the number sequence to be used in the filenames when we know
    *  the maximum number of acquisitions in advance.
    */
    void SetFileSequenceWidth(const int n);

    /* Set the file extension.
    */
    void SetFileExtension(const std::string &extension);

    /* Constructor. Set up the configuration, we need the serial number of the spectrometer to select the correct config values.
    */
    FileManager(OceanHandlerConfiguration &config, const std::string &serialNumber);
    ~FileManager();

private:
    /* Convenience method to open m_output as a sequence numbered file
    */
    void OpenSequenceNumber(const int sequenceNumber);

    /* Convenience method to outp[ut the sequence number to the file.
    */
    void OutputSequenceNumber(std::ofstream &out, const int sequenceNumber);

    std::ofstream m_output;
    bool m_multiple;
    bool m_running;
    std::string m_saveDirectory;
    std::string m_prefix;
    int m_width;
    std::string m_extension;
    SaveFormatType m_saveFormat;
    int m_precision;

    // Allow the formats to specified as human readable strings
    typedef std::map<SaveFormatType, std::string> FormatMap;
    static const FormatMap ms_saveFormats;
};
#endif