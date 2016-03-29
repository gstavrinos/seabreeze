/***************************************************//**
 * @file    OceanHandlerConfiguration.cpp
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * Read/Save the part of the configuration file associated with the OceanHandler.
 * The configuration is stored in INI format with a section per spectrometer serial number.
 * This part of the configuration deals with the sequences of acquisitions. The acquisitions
 * themselves are configured via RequestHandlerConfiguration.
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

#include "OceanHandlerConfiguration.h"
#include "FileManager.h"

/* Set the file extension for the sequence associated with the spectrometer with the specified serial number.
*/
std::string OceanHandlerConfiguration::SaveFileExtension(const std::string &serialNumber) const {

    std::string key(serialNumber);
    key.append(ms_propertyFileExtension);

    std::string result = GetValue(key, ms_defaultFileExtension);

    return result;
}

/* Set the file prefix for the sequence associated with the spectrometer with the specified serial number.
*/
std::string OceanHandlerConfiguration::SaveFilePrefix(const std::string &serialNumber) const {

    std::string key(serialNumber);
    key.append(ms_propertyFilePrefix);

    std::string result = GetValue(key, ms_defaultFilePrefix);

    return result;
}

/* Set the save to single/multiple file option for the sequence associated with the spectrometer with the
* specified serial number.
*/
bool OceanHandlerConfiguration::SaveMultipleFiles(const std::string &serialNumber) const {

    std::string key(serialNumber);
    key.append(ms_propertySaveMultipleFiles);

    bool result = GetValue(key, ms_defaultSaveMultipleFiles);

    return result;
}

/* Return the maximum number of acquisitions in the sequence associated with the spectrometer with the
* specified serial number.
*/
int OceanHandlerConfiguration::MaxAcquisitions(const std::string &serialNumber) const {

    std::string key(serialNumber);
    key.append(ms_propertyMaxAcquisitions);

    int result = GetValue(key, ms_defaultMaxAcquisitions);

    return result;
}

/* Return the interval between acquisitions to be saved in the sequence associated with the spectrometer with
* the specified serial number.
*/
long OceanHandlerConfiguration::SaveInterval(const std::string &serialNumber) const {
    
    std::string key(serialNumber);
    key.append(ms_propertySaveInterval);

    long result = GetValue(key, ms_defaultSaveInterval);

    return result;
}

/* Return the scope mode setting (on/off) for the sequence associated with the spectrometer with
* the specified serial number.
*/
bool OceanHandlerConfiguration::ShowScopeMode(const std::string &serialNumber) const {
    
    std::string key(serialNumber);
    key.append(ms_propertyShowScopeMode);

    bool result = GetValue(key, ms_defaultShowScopeMode);

    return result;
}

/* Return the scope mode refresh interval in seconds for the sequence associated with the spectrometer with
* the specified serial number.
*/
int OceanHandlerConfiguration::RefreshInterval(const std::string &serialNumber) const {
    
    std::string key(serialNumber);
    key.append(ms_propertyRefreshInterval);

    int result = GetValue(key, ms_defaultRefreshInterval);

    return result;
}

/* Return the save format for the sequence associated with the spectrometer with
* the specified serial number. This is either default i.e. with header information
* or OV_PLAIN which consists of two columns containing wavelengths and intensities
* without header information.
*/
std::string OceanHandlerConfiguration::SaveFormat(const std::string &serialNumber) const {

    std::string key(serialNumber);
    key.append(ms_propertySaveFormat);

    std::string result = GetValue(key, DefaultSaveFormat());

    return result;
}

/* Return the precision (digits) we will save floating point numbers to in the sequence associated
* with the spectrometer with the specified serial number.
*/
int OceanHandlerConfiguration::SavePrecision(const std::string &serialNumber) const {

    std::string key(serialNumber);
    key.append(ms_propertySavePrecision);

    int result = GetValue(key, ms_defaultSavePrecision);

    return result;
}

/* Return the default file save format.
*/
std::string OceanHandlerConfiguration::DefaultSaveFormat() {
    if (ms_defaultSaveFormat.empty()) {
        ms_defaultSaveFormat = FileManager::DefaultFormatKey();
    }
    
    return ms_defaultSaveFormat;
}

/* Cosntructor. Initialise to default values.
*/
OceanHandlerConfiguration::OceanHandlerConfiguration() :

    RequestHandlerConfiguration(),
    m_fileExtension(ms_defaultFileExtension),
    m_saveMultipleFiles(ms_defaultSaveMultipleFiles),
    m_maxAcquisitions(ms_defaultMaxAcquisitions),
    m_saveInterval(ms_defaultSaveInterval),
    m_showScopeMode(ms_defaultShowScopeMode),
    m_refreshInterval(ms_defaultRefreshInterval),
    m_saveFormat(DefaultSaveFormat()) {
}

OceanHandlerConfiguration::~OceanHandlerConfiguration() {
}

// static keys for the properties in the configuration file.
const std::string OceanHandlerConfiguration::ms_propertyFileExtension(".FILE_EXTENSION");
const std::string OceanHandlerConfiguration::ms_propertyFilePrefix(".FILE_PREFIX");
const std::string OceanHandlerConfiguration::ms_propertySaveMultipleFiles(".SAVE_MULTIPLE");
const std::string OceanHandlerConfiguration::ms_propertyMaxAcquisitions(".SAVE_MAX");
const std::string OceanHandlerConfiguration::ms_propertySaveInterval(".SAVE_INTERVAL");
const std::string OceanHandlerConfiguration::ms_propertyShowScopeMode(".SHOW_SCOPE");
const std::string OceanHandlerConfiguration::ms_propertyRefreshInterval(".REFRESH_INTERVAL");
const std::string OceanHandlerConfiguration::ms_propertySaveFormat(".SAVE_FORMAT");
const std::string OceanHandlerConfiguration::ms_propertySavePrecision(".SAVE_PRECISION");

// static default values.
const std::string OceanHandlerConfiguration::ms_defaultFileExtension(".txt");
const std::string OceanHandlerConfiguration::ms_defaultFilePrefix("");
const bool OceanHandlerConfiguration::ms_defaultSaveMultipleFiles(true);
const bool OceanHandlerConfiguration::ms_defaultShowScopeMode(true);
std::string OceanHandlerConfiguration::ms_defaultSaveFormat;