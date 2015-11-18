/***************************************************//**
 * @file    RequestHandlerConfiguration.cpp
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * Read/Save the part of the configuration file associated with the RequestHandler.
 * The configuration is stored in INI format with a section per spectrometer serial number.
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

#include "RequestHandlerConfiguration.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>

/* Return the port we are listening on.
*/
short RequestHandlerConfiguration::Port() const {

    std::string key(ms_global);
    key.append(ms_propertyPort);

    short result = GetValue(key, ms_defaultPort);
    return result;
}

/* Return the integration time for the spectrometer with the specified serial number if
* it is in the configuration file or the default value otherwise.
*/
long RequestHandlerConfiguration::IntegrationTime(const std::string &serialNumber) const {
    std::string key(serialNumber);
    key.append(ms_propertyIntegrationTime);

    long result = GetValue(key, ms_defaultIntegrationTime);

    return result;
}

/* Return the scans to average for the spectrometer with the specified serial number if
* it is in the configuration file or the default value otherwise.
*/
int RequestHandlerConfiguration::ScansToAverage(const std::string &serialNumber) const {
    
    std::string key(serialNumber);
    key.append(ms_propertyScansToAverage);

    int result = GetValue(key, ms_defaultScansToAverage);

    return result;
}

/* Return the boxcar width for the spectrometer with the specified serial number if
* it is in the configuration file or the default value otherwise.
*/
int RequestHandlerConfiguration::BoxcarWidth(const std::string &serialNumber) const {
    
    std::string key(serialNumber);
    key.append(ms_propertyBoxcarWidth);

    int result = GetValue(key, ms_defaultBoxcarWidth);

    return result;
}

/* Do an update of the configuration file. Save any changes or add an new spectrometer.
*/
void RequestHandlerConfiguration::DoUpdate() {
    // m_updateRequired may be set true by the (template) method GetValue
    if (m_updateRequired && boost::filesystem::exists(ms_configFilename)) {
        boost::property_tree::write_ini(ms_configFilename, m_pTree);
    }
}

/* Constructor. Initialise default values and parse the configuration file.
*/
RequestHandlerConfiguration::RequestHandlerConfiguration() :

    m_updateRequired(false),
    m_port(ms_defaultPort),
    m_integrationTime(ms_defaultIntegrationTime),
    m_scansToAverage(ms_defaultScansToAverage),
    m_boxcarWidth(ms_defaultBoxcarWidth) {

    // We use a Boost property tree to parse in the configuration file (.ini format).
    if (boost::filesystem::exists(ms_configFilename)) {
        boost::property_tree::read_ini(ms_configFilename, m_pTree);
    }
}

RequestHandlerConfiguration::~RequestHandlerConfiguration() {
}

// static values
#ifndef WIN32
const std::string RequestHandlerConfiguration::ms_configFilename("/etc/ocean/ocean-daemon.conf");
#else
const std::string RequestHandlerConfiguration::ms_configFilename("c:\\temp\\ocean-daemon.conf");
#endif

// the keys for the values in the configuration file
const std::string RequestHandlerConfiguration::ms_global("GLOBAL");
const std::string RequestHandlerConfiguration::ms_propertyPort(".PORT");
const std::string RequestHandlerConfiguration::ms_propertyIntegrationTime(".INTEGRATION_TIME");
const std::string RequestHandlerConfiguration::ms_propertyScansToAverage(".SCANS_TO_AVERAGE");
const std::string RequestHandlerConfiguration::ms_propertyBoxcarWidth(".BOXCAR");