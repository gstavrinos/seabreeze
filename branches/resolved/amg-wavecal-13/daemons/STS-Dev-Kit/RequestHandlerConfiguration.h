/***************************************************//**
 * @file    RequestHandlerConfiguration.h
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * The class that parses the configuration file for the RequestHandler base class.
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

#ifndef REQUEST_HANDLER_CONFIGURATION_H
#define REQUEST_HANDLER_CONFIGURATION_H


#include <string>
#include <boost/property_tree/ptree.hpp>

class RequestHandlerConfiguration {
public:
    /* Return the configured port for the daemon to listen on.
    */
    short Port() const;

    /* Return the configured integration time.
    */
    long IntegrationTime(const std::string &serialNumber) const;

    /* Return the configured scans to average.
    */
    int ScansToAverage(const std::string &serialNumber) const;

    /* Return the configured boxcar width.
    */
    int BoxcarWidth(const std::string &serialNumber) const;

    /* Update the RequestHandlerConfiguration file if we have found an unknown spectrometer or value.
    *  The configuration uses the spectrometer serial number to recognize "known" spectrometers and
    *  save default configuration values. Unknown spectrometers are added with "sensible" default values
    *  that can be changed.
    */
    void DoUpdate();

    RequestHandlerConfiguration();
    virtual ~RequestHandlerConfiguration();

protected:
    // Do this once here as a template. Get a value with a specified key which could be one of several different types.
    template <class T>
    T GetValue(const std::string &key, const T valueDefault) const {
        using boost::property_tree::ptree;
        using boost::property_tree::ptree_bad_data;
        using boost::property_tree::ptree_bad_path;

        T result = valueDefault;
        try {
            result = m_pTree.get_child(key).get_value<T>();
        }
        catch (ptree_bad_path) {
            m_pTree.add(key, valueDefault);
            m_updateRequired = true;
        }
        catch (ptree_bad_data) {
            m_pTree.add(key, valueDefault);
            m_updateRequired = true;
        }
        return result;
    }

    // The configuration is implemented via a Boost poperty tree
    mutable boost::property_tree::ptree m_pTree;

    // If we come across a spectrometer we don't know then update the RequestHandlerConfiguration file with defaults.
    // Use this flag to keep track of unknowns
    mutable bool m_updateRequired;

    // local copies of the configuration values
    short m_port;
    long m_integrationTime;
    long m_scansToAverage;
    long m_boxcarWidth;

    // static constants for e.g. the config file name and "sensible" default values.
    static const std::string ms_configFilename;

    static const std::string ms_global;

    static const std::string ms_propertyPort;
    static const std::string ms_propertyIntegrationTime;
    static const std::string ms_propertyScansToAverage;
    static const std::string ms_propertyBoxcarWidth;

    static const short ms_defaultPort = 1865;
    static const long ms_defaultIntegrationTime = 100000;
    static const int ms_defaultScansToAverage = 0;
    static const int ms_defaultBoxcarWidth = 0;
};
#endif