/***************************************************//**
 * @file    OceanHandlerConfiguration.h
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * The class that parses the configuration file for the OceanHandler class.
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

#ifndef OCEAN_HANDLER_CONFIGURATION_H
#define OCEAN_HANDLER_CONFIGURATION_H

#include "RequestHandlerConfiguration.h"
#include <string>
#include <boost/property_tree/ptree.hpp>

class OceanHandlerConfiguration : public RequestHandlerConfiguration {
public:
    /* Return the default saved file extension.
    */
    std::string SaveFileExtension(const std::string &serialNumber) const;

    /* Return the default saved file prefix.
    */
    std::string SaveFilePrefix(const std::string &serialNumber) const;

    /* Return the default file save mode.
    */
    bool SaveMultipleFiles(const std::string &serialNumber) const;

    /* Return the default max acquisitions.
    */
    int MaxAcquisitions(const std::string &serialNumber) const;

    /* Return the default interval between acquisitions (milliseconds).
    */
    long SaveInterval(const std::string &serialNumber) const;

    /* Return the default scope mode selection
    */
    bool ShowScopeMode(const std::string &serialNumber) const;

    /* Return the default refresh interval for scope mode
    */
    int RefreshInterval(const std::string &serialNumber) const;

    /* Return the format the sequence is saved in. Currently this is with or without
    *  header information to provide compatibility with OceanView.
    */
    std::string SaveFormat(const std::string &serialNumber) const;

    /* Return the precision (digits) to save floating point numbers to.
    */
    int SavePrecision(const std::string &serialNumber) const;

    OceanHandlerConfiguration();
    virtual ~OceanHandlerConfiguration();
private:

    // Lazy initialization to get around the undefined order of static initialization
    static std::string DefaultSaveFormat();

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

    // local copies of the configuraition file values
    std::string m_fileExtension;
    bool m_saveMultipleFiles;
    int m_maxAcquisitions;
    long m_saveInterval;
    bool m_showScopeMode;
    int m_refreshInterval;

    std::string m_saveFormat;
    int m_savePrecision;

    // static constants for e.g. the config file name and "sensible" default values.
    static const std::string ms_configFilename;

    static const std::string ms_global;

    static const std::string ms_propertyFileExtension;
    static const std::string ms_propertyFilePrefix;
    static const std::string ms_propertySaveMultipleFiles;
    static const std::string ms_propertyMaxAcquisitions;
    static const std::string ms_propertySaveInterval;
    static const std::string ms_propertyShowScopeMode;
    static const std::string ms_propertyRefreshInterval;
    static const std::string ms_propertySaveFormat;
    static const std::string ms_propertySavePrecision;

    static const std::string ms_defaultFileExtension;
    static const std::string ms_defaultFilePrefix;
    static const bool ms_defaultSaveMultipleFiles;
    static const int ms_defaultMaxAcquisitions = 0;
    static const long ms_defaultSaveInterval = 1000;
    static const bool ms_defaultShowScopeMode;
    static const int ms_defaultRefreshInterval = 5;

    static std::string ms_defaultSaveFormat;
    static const int ms_defaultSavePrecision = 8;
};
#endif