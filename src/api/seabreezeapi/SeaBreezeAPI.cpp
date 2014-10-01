/***************************************************//**
 * @file    SeaBreezeAPI.cpp
 * @date    February 2012
 * @author  Ocean Optics, Inc.
 *
 * This is a wrapper around the SeaBreeze driver.
 * Both C and C++ language interfaces are provided.  Please
 * note that this wrapper should try very hard to recover
 * from errors -- like the user trying to read data before
 * opening the device -- and set an appropriate error code.
 * Even if a method here is called grossly out of order,
 * it should not be possible to crash anything.  The only
 * case where it may be reasonable to crash is when trying
 * to fill in a buffer that the user has not properly
 * allocated first.  All other cases should recover.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2014, Ocean Optics Inc
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

/* Constants and macros */
#define SET_ERROR_CODE(code) do { if(NULL != errorCode) { *errorCode = code; }  } while(0)

#include "common/globals.h"
#include "api/seabreezeapi/SeaBreezeAPI.h"
#include "api/seabreezeapi/SeaBreezeAPIConstants.h"
#include "api/DeviceFactory.h"
#include "common/buses/rs232/RS232DeviceLocator.h"
#include "common/buses/DeviceLocationProberInterface.h"

#include <ctype.h>
#include <vector>
#include <string.h>

using namespace seabreeze;
using namespace seabreeze::api;
using namespace std;

// MZ: added "Error" to all error strings to simplify test output analysis
static const char *error_msgs[] = {
    "Success",
    "Error: Undefined error",
    "Error: No device found",
    "Error: Could not close device",
    "Error: Feature not implemented",
    "Error: No such feature on device",
    "Error: Data transfer error",
    "Error: Invalid user buffer provided"
};

static int number_error_msgs = sizeof (error_msgs) / sizeof (char *);
static int __deviceID = 1;

SeaBreezeAPI *SeaBreezeAPI::instance = NULL;

SeaBreezeAPI::SeaBreezeAPI() {

}

SeaBreezeAPI::~SeaBreezeAPI() {
    vector<DeviceAdapter *>::iterator dIter;

    for(dIter = this->specifiedDevices.begin(); dIter != this->specifiedDevices.end(); dIter++) {
        delete *dIter;
    }

    for(dIter = this->probedDevices.begin(); dIter != this->probedDevices.end(); dIter++) {
        delete *dIter;
    }
}

SeaBreezeAPI *SeaBreezeAPI::getInstance() {
    if(NULL == instance) {
        instance = new SeaBreezeAPI();
    }
    return instance;
}

void SeaBreezeAPI::shutdown() {
    if(NULL != instance) {
        delete instance;
        instance = NULL;
    }
    DeviceFactory::shutdown();
}

int SeaBreezeAPI::probeDevices() {
    /* This function is a little ugly because it tries to find hardware
     * associated with every device type, but without allowing multiple
     * Device instances to be created for a particular Bus location.
     * This requires a bit of searching which translates to nested loops.
     * Fortunately, none of these loops has to iterate very long.
     */
    Device *dev;
    vector<DeviceAdapter *>::iterator devIter;
    vector<DeviceAdapter *>::iterator validIter;
    int i;
    vector<DeviceAdapter *> validDevices;

    DeviceFactory* deviceFactory = DeviceFactory::getInstance();

    for(i = 0; i < deviceFactory->getNumberOfDeviceTypes(); i++) {
        /* Try to create a device by its type index.  This does not require
         * knowing what type of device is actually being created.  This instance
         * will be an exemplar and will not actually be tied to real hardware.
         */
        dev = deviceFactory->create(i);

        vector<Bus *>::iterator iter;
        vector<Bus *> buses = dev->getBuses();

        for(iter = buses.begin(); iter != buses.end(); iter++) {
            DeviceLocationProberInterface *prober = dynamic_cast<DeviceLocationProberInterface *>(*iter);
            if(NULL != prober) {
                /* Found a type of Bus that can probe for
                 * real hardware to associate with this Device
                 */
                vector<DeviceLocatorInterface *> *locations;
                locations = prober->probeDevices();
                vector<DeviceLocatorInterface *>::iterator locIter;
                for(    locIter = locations->begin();
                        locIter != locations->end();
                        locIter++) {
                    /* For each device location, check whether it is already
                     * known.  If not, add it.  If so, skip over it.
                     */
                    bool locationKnown = false;
                    for(    devIter = this->probedDevices.begin();
                            devIter != this->probedDevices.end();
                            devIter++) {
                        /* For each known device, compare to the newly probed
                         * location and see if they match.
                         */
                        DeviceLocatorInterface *knownLoc = (*devIter)->getLocation();
                        if(true == (*locIter)->equals(*knownLoc)) {
                            /* This device location is already tracked. */
                            locationKnown = true;
                            /* Note that it has just been seen */
                            validDevices.push_back(*devIter);
                            break;
                        }
                    }
                    if(false == locationKnown) {
                        /* The location is not already known.  Create a new
                         * instance of the type of device in question and
                         * assign the new instance to this location.  This also
                         * effectively marks the new instance as being valid.
                         */
                        Device *newdev = deviceFactory->create(i);
                        newdev->setLocation(**locIter);
                        /* Note that this pre-increments the device ID to
                         * mitigate any race conditions
                         */
                        DeviceAdapter *da = new DeviceAdapter(newdev, ++__deviceID);
                        this->probedDevices.push_back(da);
                        validDevices.push_back(da);
                    }
                }
                for(vector<DeviceLocatorInterface *>::iterator iter = locations->begin();
                        iter != locations->end(); iter++) {
                    delete *iter;
                }
                locations->clear();
                delete locations;
            }
        }

        delete dev;
    }

    /* Now go through the set of all probed devices and ensure that each of
     * them is still around.
     */
    for(    devIter = this->probedDevices.begin();
            devIter != this->probedDevices.end();
            ) {
        bool verified = false;
        DeviceLocatorInterface *knownLoc = (*devIter)->getLocation();
        for(    validIter = validDevices.begin();
                validIter != validDevices.end();
                validIter++) {
            DeviceLocatorInterface *validLoc = (*validIter)->getLocation();
            if(knownLoc->equals(*validLoc)) {
                verified = true;
                break;
            }
        }
        if(false == verified) {
            /* The device has disappeared since it was first probed.  Get rid
             * of the instance that was tracking it.
             */
            delete *devIter;
            devIter = this->probedDevices.erase(devIter);
        } else {
            devIter++;
        }
    }

    return (int) probedDevices.size();
}


int SeaBreezeAPI::addRS232DeviceLocation(char *deviceTypeName,
        char *deviceBusPath, unsigned int baud) {
    string path(deviceBusPath);

    Device *dev = DeviceFactory::getInstance()->create(deviceTypeName);
    if(NULL == dev) {
        /* Failed to identify that type of device. */
        return 1;
    }

    RS232DeviceLocator locator(path, baud);
    dev->setLocation(locator);

    /* Note that this pre-increments the device ID to mitigate any race conditions */
    this->specifiedDevices.push_back(new DeviceAdapter(dev, ++__deviceID));

    return 0;
}


int SeaBreezeAPI::getNumberOfDeviceIDs() {
    return (int) (this->specifiedDevices.size() + this->probedDevices.size());
}


int SeaBreezeAPI::getDeviceIDs(long *ids, unsigned long maxLength) {

    vector<DeviceAdapter *>::iterator iter;
    unsigned int i = 0;

    for(    iter = specifiedDevices.begin();
            iter != specifiedDevices.end() && i < maxLength;
            iter++, i++) {
        ids[i] = (*iter)->getID();
    }

    /* Note: this does not reset i and this is deliberate. */

    for(    iter = probedDevices.begin();
            iter != probedDevices.end() && i < maxLength;
            iter++, i++) {
        ids[i] = (*iter)->getID();
    }

    return i;
}

DeviceAdapter *SeaBreezeAPI::getDeviceByID(unsigned long id) {
    vector<DeviceAdapter *>::iterator iter;

    /* This gives priority to specified devices since they require more specific
     * information to set up.
     */
    for(iter = specifiedDevices.begin(); iter != specifiedDevices.end(); iter++) {
        if((*iter)->getID() == id) {
            return *iter;
        }
    }

    for(iter = probedDevices.begin(); iter != probedDevices.end(); iter++) {
        if((*iter)->getID() == id) {
            return *iter;
        }
    }

    return NULL;
}

int SeaBreezeAPI::openDevice(long id, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(id);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return -1;
    }

    return adapter->open(errorCode);
}

void SeaBreezeAPI::closeDevice(long deviceID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return;
    }

    adapter->close();
    SET_ERROR_CODE(ERROR_SUCCESS);
}

int SeaBreezeAPI::getDeviceType(long id, int *errorCode,
            char *buffer, unsigned int length) {
    DeviceAdapter *adapter = getDeviceByID(id);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->getDeviceType(errorCode, buffer, length);
}

int SeaBreezeAPI::getNumberOfSerialNumberFeatures(long deviceID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getNumberOfSerialNumberFeatures();
}

int SeaBreezeAPI::getSerialNumberFeatures(long deviceID, int *errorCode,
            long *buffer, unsigned int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getSerialNumberFeatures(buffer, maxLength);
}

int SeaBreezeAPI::getNumberOfSpectrometerFeatures(long deviceID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getNumberOfSpectrometerFeatures();
}

int SeaBreezeAPI::getSpectrometerFeatures(long deviceID, int *errorCode,
            long *buffer, unsigned int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getSpectrometerFeatures(buffer, maxLength);
}

int SeaBreezeAPI::getNumberOfThermoElectricFeatures(long deviceID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getNumberOfThermoElectricFeatures();
}

int SeaBreezeAPI::getThermoElectricFeatures(long deviceID, int *errorCode,
            long *buffer, unsigned int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getThermoElectricFeatures(buffer, maxLength);
}

int SeaBreezeAPI::getNumberOfIrradCalFeatures(long deviceID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getNumberOfIrradCalFeatures();
}

int SeaBreezeAPI::getIrradCalFeatures(long deviceID, int *errorCode,
            long *buffer, unsigned int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getIrradCalFeatures(buffer, maxLength);
}

int SeaBreezeAPI::getNumberOfEEPROMFeatures(long deviceID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getNumberOfEEPROMFeatures();
}

int SeaBreezeAPI::getEEPROMFeatures(long deviceID, int *errorCode,
            long *buffer, unsigned int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getEEPROMFeatures(buffer, maxLength);
}

int SeaBreezeAPI::getNumberOfLampFeatures(long deviceID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getNumberOfStrobeLampFeatures();
}

int SeaBreezeAPI::getLampFeatures(long deviceID, int *errorCode,
            long *buffer, unsigned int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getStrobeLampFeatures(buffer, maxLength);
}

int SeaBreezeAPI::getNumberOfShutterFeatures(long deviceID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getNumberOfShutterFeatures();
}

int SeaBreezeAPI::getShutterFeatures(long deviceID, int *errorCode,
            long *buffer, unsigned int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getShutterFeatures(buffer, maxLength);
}

int SeaBreezeAPI::getNumberOfLightSourceFeatures(long deviceID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getNumberOfLightSourceFeatures();
}

int SeaBreezeAPI::getLightSourceFeatures(long deviceID, int *errorCode,
            long *buffer, unsigned int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getLightSourceFeatures(buffer, maxLength);
}

int SeaBreezeAPI::getNumberOfNonlinearityCoeffsFeatures(long deviceID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getNumberOfNonlinearityCoeffsFeatures();
}


int SeaBreezeAPI::getNonlinearityCoeffsFeatures(long deviceID, int *errorCode,
        long *buffer, unsigned int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getNonlinearityCoeffsFeatures(buffer, maxLength);
}


int SeaBreezeAPI::getNumberOfStrayLightCoeffsFeatures(long deviceID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getNumberOfStrayLightCoeffsFeatures();
}


int SeaBreezeAPI::getStrayLightCoeffsFeatures(long deviceID, int *errorCode,
        long *buffer, unsigned int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getStrayLightCoeffsFeatures(buffer, maxLength);
}

int SeaBreezeAPI::getSerialNumber(long deviceID, long featureID, int *errorCode,
            char *buffer, int bufferLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->getSerialNumber(featureID, errorCode, buffer, bufferLength);
}


void SeaBreezeAPI::spectrometerSetTriggerMode(long deviceID, long featureID,
            int *errorCode, int mode) {

    SpectrometerTriggerMode triggerMode(mode);

    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return;
    }

    adapter->spectrometerSetTriggerMode(featureID, errorCode, mode);
}


void SeaBreezeAPI::spectrometerSetIntegrationTimeMicros(long deviceID,
        long featureID, int *errorCode,
        unsigned long integrationTimeMicros) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return;
    }

    adapter->spectrometerSetIntegrationTimeMicros(featureID,
        errorCode, integrationTimeMicros);
}

unsigned long SeaBreezeAPI::spectrometerGetMinimumIntegrationTimeMicros(
        long deviceID, long featureID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->spectrometerGetMinimumIntegrationTimeMicros(featureID, errorCode);
}

int SeaBreezeAPI::spectrometerGetUnformattedSpectrum(long deviceID,
        long featureID, int *errorCode, unsigned char *buffer, int bufferLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->spectrometerGetUnformattedSpectrum(featureID, errorCode,
            buffer, bufferLength);
}

int SeaBreezeAPI::spectrometerGetFormattedSpectrum(long deviceID,
        long featureID, int *errorCode, double *buffer, int bufferLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->spectrometerGetFormattedSpectrum(featureID, errorCode,
            buffer, bufferLength);
}

int SeaBreezeAPI::spectrometerGetUnformattedSpectrumLength(long deviceID,
        long featureID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->spectrometerGetUnformattedSpectrumLength(featureID, errorCode);
}

int SeaBreezeAPI::spectrometerGetFormattedSpectrumLength(long deviceID,
        long featureID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->spectrometerGetFormattedSpectrumLength(featureID, errorCode);
}

int SeaBreezeAPI::spectrometerGetWavelengths(long deviceID,
        long featureID, int *errorCode, double *wavelengths, int length) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->spectrometerGetWavelengths(featureID, errorCode,
                wavelengths, length);
}

int SeaBreezeAPI::spectrometerGetElectricDarkPixelCount(long deviceID,
        long featureID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->spectrometerGetElectricDarkPixelCount(featureID, errorCode);
}

int SeaBreezeAPI::spectrometerGetElectricDarkPixelIndices(long deviceID,
        long featureID, int *errorCode, int *indices, int length) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->spectrometerGetElectricDarkPixelIndices(featureID, errorCode,
                indices, length);
}


int SeaBreezeAPI::eepromReadSlot(long deviceID, long featureID, int *errorCode,
        int slotNumber, unsigned char *buffer, int bufferLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->eepromReadSlot(featureID, errorCode, slotNumber,
            buffer, bufferLength);
}


int SeaBreezeAPI::lightSourceGetCount(long deviceID, long featureID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->lightSourceGetCount(featureID, errorCode);
}

bool SeaBreezeAPI::lightSourceHasEnable(long deviceID, long featureID, int *errorCode,
        int lightSourceIndex) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return false;
    }

    return adapter->lightSourceHasEnable(featureID, errorCode, lightSourceIndex);
}

bool SeaBreezeAPI::lightSourceIsEnabled(long deviceID, long featureID, int *errorCode,
        int lightSourceIndex) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return false;
    }

    return adapter->lightSourceIsEnabled(featureID, errorCode, lightSourceIndex);
}

void SeaBreezeAPI::lightSourceSetEnable(long deviceID, long featureID, int *errorCode,
        int lightSourceIndex, bool enable) {

    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return;
    }

    adapter->lightSourceSetEnable(featureID, errorCode, lightSourceIndex, enable);
}

bool SeaBreezeAPI::lightSourceHasVariableIntensity(long deviceID, long featureID,
        int *errorCode, int lightSourceIndex) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return false;
    }

    return adapter->lightSourceHasVariableIntensity(featureID, errorCode,
            lightSourceIndex);
}

double SeaBreezeAPI::lightSourceGetIntensity(long deviceID, long featureID, int *errorCode,
        int lightSourceIndex) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return false;
    }

    return adapter->lightSourceGetIntensity(featureID, errorCode, lightSourceIndex);
}

void SeaBreezeAPI::lightSourceSetIntensity(long deviceID, long featureID, int *errorCode,
        int lightSourceIndex, double intensity) {

    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return;
    }

    adapter->lightSourceSetIntensity(featureID, errorCode,
            lightSourceIndex, intensity);
}


void SeaBreezeAPI::lampSetLampEnable(long deviceID, long featureID,
        int *errorCode, bool strobeEnable) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return;
    }

    adapter->lampSetStrobeEnable(featureID, errorCode, strobeEnable);
}

int SeaBreezeAPI::getNumberOfContinuousStrobeFeatures(long deviceID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getNumberOfContinuousStrobeFeatures();
}

int SeaBreezeAPI::getContinuousStrobeFeatures(long deviceID, int *errorCode, long *buffer,
        unsigned int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    return adapter->getContinuousStrobeFeatures(buffer, maxLength);
}

void SeaBreezeAPI::continuousStrobeSetContinuousStrobeEnable(long deviceID, long featureID,
        int *errorCode, bool strobeEnable) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    adapter->continuousStrobeSetEnable(featureID, errorCode, strobeEnable);
}

void SeaBreezeAPI::continuousStrobeSetContinuousStrobePeriodMicroseconds(long deviceID,
        long featureID, int *errorCode, unsigned long strobePeriodMicroseconds) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return;
    }

    SET_ERROR_CODE(ERROR_SUCCESS);
    adapter->continuousStrobeSetPeriodMicroseconds(featureID, errorCode, strobePeriodMicroseconds);
}

void SeaBreezeAPI::shutterSetShutterOpen(long deviceID, long featureID,
        int *errorCode, bool opened) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return;
    }

    adapter->shutterSetShutterOpen(featureID, errorCode, opened);
}

int SeaBreezeAPI::irradCalibrationRead(long deviceID, long featureID,
        int *errorCode, float *buffer, int bufferLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->irradCalibrationRead(featureID, errorCode, buffer, bufferLength);
}

int SeaBreezeAPI::irradCalibrationWrite(long deviceID, long featureID,
        int *errorCode, float *buffer, int bufferLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->irradCalibrationWrite(featureID, errorCode, buffer, bufferLength);
}

int SeaBreezeAPI::irradCalibrationHasCollectionArea(long deviceID,
        long featureID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->irradCalibrationHasCollectionArea(featureID, errorCode);
}

float SeaBreezeAPI::irradCalibrationReadCollectionArea(long deviceID,
        long featureID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->irradCalibrationReadCollectionArea(featureID, errorCode);
}

void SeaBreezeAPI::irradCalibrationWriteCollectionArea(long deviceID, long featureID,
        int *errorCode, float area) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return;
    }

    adapter->irradCalibrationWriteCollectionArea(featureID, errorCode, area);
}

double SeaBreezeAPI::tecReadTemperatureDegreesC(long deviceID,
        long featureID, int *errorCode) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->tecReadTemperatureDegreesC(featureID, errorCode);
}

void SeaBreezeAPI::tecSetTemperatureSetpointDegreesC(long deviceID, long featureID,
        int *errorCode, double temperatureDegreesCelsius) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return;
    }

    adapter->tecSetTemperatureSetpointDegreesC(featureID, errorCode,
            temperatureDegreesCelsius);
}

void SeaBreezeAPI::tecSetEnable(long deviceID, long featureID, int *errorCode,
        unsigned char tecEnable) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return;
    }

    adapter->tecSetEnable(featureID, errorCode, 0 == tecEnable ? false : true);
}

int SeaBreezeAPI::nonlinearityCoeffsGet(long deviceID, long featureID,
        int *errorCode, double *buffer, int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->nonlinearityCoeffsGet(featureID, errorCode, buffer, maxLength);
}

int SeaBreezeAPI::strayLightCoeffsGet(long deviceID, long featureID,
        int *errorCode, double *buffer, int maxLength) {
    DeviceAdapter *adapter = getDeviceByID(deviceID);
    if(NULL == adapter) {
        SET_ERROR_CODE(ERROR_NO_DEVICE);
        return 0;
    }

    return adapter->strayLightCoeffsGet(featureID, errorCode, buffer, maxLength);
}


/*******************  C language wrappers ********************************/

void sbapi_initialize() {
    /* Force the API to allocate an instance if it has not already. */
    SeaBreezeAPI::getInstance();

    /* Nothing else is required (yet) */
}

void sbapi_shutdown() {
    SeaBreezeAPI::shutdown();
}

int
sbapi_add_RS232_device_location(char *deviceTypeName, char *deviceBusPath,
            unsigned int baud) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->addRS232DeviceLocation(deviceTypeName, deviceBusPath, baud);
}

int
sbapi_probe_devices() {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->probeDevices();
}

int
sbapi_get_number_of_device_ids() {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNumberOfDeviceIDs();
}

int
sbapi_get_device_ids(long *ids, unsigned int max_ids) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getDeviceIDs(ids, max_ids);
}

int
sbapi_open_device(long deviceID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->openDevice(deviceID, error_code);
}

void
sbapi_close_device(long index, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    wrapper->closeDevice(index, error_code);
}

const char *
sbapi_get_error_string(int error_code) {
    if((error_code < 0) || (error_code >= number_error_msgs)) {
        return error_msgs[ERROR_INVALID_ERROR];
    }
    return error_msgs[error_code];
}

int
sbapi_get_device_type(long deviceID, int *error_code,
            char *buffer, unsigned int length) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getDeviceType(deviceID, error_code, buffer, length);
}

int
sbapi_get_number_of_serial_number_features(long deviceID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNumberOfSerialNumberFeatures(deviceID, error_code);
}

int
sbapi_get_serial_number_features(long deviceID, int *error_code, long *features,
        int max_features) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getSerialNumberFeatures(deviceID, error_code,
            features, max_features);
}

int
sbapi_get_serial_number(long deviceID, long featureID, int *error_code,
        char *buffer, int buffer_length) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getSerialNumber(deviceID, featureID,
            error_code, buffer, buffer_length);
}

int
sbapi_get_number_of_spectrometer_features(long deviceID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNumberOfSpectrometerFeatures(deviceID, error_code);
}

int
sbapi_get_spectrometer_features(long deviceID, int *error_code, long *features,
        int max_features) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getSpectrometerFeatures(deviceID, error_code,
            features, max_features);
}

void
sbapi_spectrometer_set_trigger_mode(long deviceID, long featureID,
        int *error_code, int mode) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    wrapper->spectrometerSetTriggerMode(deviceID, featureID, error_code, mode);
}

void
sbapi_spectrometer_set_integration_time_micros(long deviceID, long featureID,
        int *error_code, unsigned long integration_time_micros) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    wrapper->spectrometerSetIntegrationTimeMicros(deviceID, featureID, error_code,
        integration_time_micros);
}

long
sbapi_spectrometer_get_minimum_integration_time_micros(long deviceID,
        long featureID, int *error_code) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->spectrometerGetMinimumIntegrationTimeMicros(deviceID, featureID,
            error_code);
}

int
sbapi_spectrometer_get_unformatted_spectrum(long deviceID,
        long featureID, int *error_code,
        unsigned char *buffer, int buffer_length) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->spectrometerGetUnformattedSpectrum(deviceID,
            featureID, error_code, buffer, buffer_length);
}

int
sbapi_spectrometer_get_formatted_spectrum(long deviceID,
        long featureID, int *error_code,
        double* buffer, int buffer_length) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->spectrometerGetFormattedSpectrum(deviceID, featureID,
            error_code, buffer, buffer_length);
}

int
sbapi_spectrometer_get_unformatted_spectrum_length(long deviceID,
        long featureID, int *error_code) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->spectrometerGetUnformattedSpectrumLength(deviceID,
            featureID, error_code);
}

int
sbapi_spectrometer_get_formatted_spectrum_length(long deviceID,
        long featureID, int *error_code) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->spectrometerGetFormattedSpectrumLength(deviceID,
            featureID, error_code);
}

int
sbapi_spectrometer_get_wavelengths(long deviceID,
        long featureID, int *error_code, double *wavelengths,
        int length) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->spectrometerGetWavelengths(deviceID, featureID,
            error_code, wavelengths, length);
}

int
sbapi_spectrometer_get_electric_dark_pixel_count(long deviceID,
        long featureID, int *error_code) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->spectrometerGetElectricDarkPixelCount(deviceID, featureID,
            error_code);
}

int
sbapi_spectrometer_get_electric_dark_pixel_indices(long deviceID,
        long featureID, int *error_code, int *indices, int length) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->spectrometerGetElectricDarkPixelIndices(deviceID, featureID,
            error_code, indices, length);
}

int
sbapi_get_number_of_shutter_features(long deviceID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNumberOfShutterFeatures(deviceID, error_code);
}

int
sbapi_get_shutter_features(long deviceID, int *error_code, long *features,
        int max_features) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getShutterFeatures(deviceID, error_code,
            features, max_features);
}

void
sbapi_shutter_set_shutter_open(long deviceID, long featureID,
        int *error_code, unsigned char opened) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    wrapper->shutterSetShutterOpen(deviceID, featureID, error_code, opened != 0);
}

int
sbapi_get_number_of_light_source_features(long deviceID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNumberOfLightSourceFeatures(deviceID, error_code);
}

int
sbapi_get_light_source_features(long deviceID, int *error_code,
        long *features, int max_features) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getLightSourceFeatures(deviceID, error_code,
            features, max_features);
}

int
sbapi_light_source_get_count(long deviceID, long featureID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->lightSourceGetCount(deviceID, featureID, error_code);
}

unsigned char
sbapi_light_source_has_enable(long deviceID, long featureID, int *error_code,
        int light_source_index) {
    bool retval;
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    retval = wrapper->lightSourceHasEnable(deviceID, featureID, error_code,
            light_source_index);
    return (retval == false ? 0 : 1);
}

unsigned char
sbapi_light_source_is_enabled(long deviceID, long featureID, int *error_code,
        int light_source_index) {
    bool retval;
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    retval = wrapper->lightSourceIsEnabled(deviceID, featureID, error_code,
            light_source_index);
    return (false == retval ? 0 : 1);
}

void
sbapi_light_source_set_enable(long deviceID, long featureID, int *error_code,
        int light_source_index, unsigned char enable) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    wrapper->lightSourceSetEnable(deviceID, featureID, error_code,
            light_source_index, (0 == enable ? false : true));
}

unsigned char
sbapi_light_source_has_variable_intensity(long deviceID, long featureID,
        int *error_code, int light_source_index) {
    bool retval;
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    retval = wrapper->lightSourceHasVariableIntensity(deviceID, featureID,
            error_code, light_source_index);
    return (retval == false ? 0 : 1);
}

double
sbapi_light_source_get_intensity(long deviceID, long featureID,
        int *error_code, int light_source_index) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->lightSourceGetIntensity(deviceID, featureID, error_code,
        light_source_index);
}

void
sbapi_light_source_set_intensity(long deviceID, long featureID,
        int *error_code, int light_source_index, double intensity) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    wrapper->lightSourceSetIntensity(deviceID, featureID, error_code,
        light_source_index, intensity);
}

int
sbapi_get_number_of_lamp_features(long deviceID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNumberOfLampFeatures(deviceID, error_code);
}

int
sbapi_get_lamp_features(long deviceID, int *error_code, long *features,
        int max_features) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getLampFeatures(deviceID, error_code,
            features, max_features);
}

void
sbapi_lamp_set_lamp_enable(long deviceID, long featureID,
        int *error_code, unsigned char lamp_enable) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    wrapper->lampSetLampEnable(deviceID, featureID, error_code, lamp_enable != 0);
}

int
sbapi_get_number_of_continuous_strobe_features(long deviceID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNumberOfContinuousStrobeFeatures(deviceID, error_code);
}

int
sbapi_get_continuous_strobe_features(long deviceID, int *error_code, long *features,
        int max_features) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getContinuousStrobeFeatures(deviceID, error_code,
            features, max_features);
}

void
sbapi_continuous_strobe_set_continuous_strobe_enable(long deviceID, long featureID,
        int *error_code, unsigned char lamp_enable) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    wrapper->continuousStrobeSetContinuousStrobeEnable(deviceID, featureID, error_code,
        lamp_enable != 0);
}

void
sbapi_continuous_strobe_set_continuous_strobe_period_micros(long deviceID, long featureID,
        int *error_code, unsigned long period_usec) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    wrapper->continuousStrobeSetContinuousStrobePeriodMicroseconds(deviceID, featureID,
        error_code, period_usec);
}


int
sbapi_get_number_of_eeprom_features(long deviceID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNumberOfEEPROMFeatures(deviceID, error_code);
}

int
sbapi_get_eeprom_features(long deviceID, int *error_code, long *features,
        int max_features) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getEEPROMFeatures(deviceID, error_code,
            features, max_features);
}

int
sbapi_eeprom_read_slot(long deviceID, long featureID,
        int *error_code, int slot_number,
        unsigned char *buffer, int buffer_length) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->eepromReadSlot(deviceID, featureID, error_code, slot_number,
            buffer, buffer_length);
}


int
sbapi_get_number_of_irrad_cal_features(long deviceID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNumberOfIrradCalFeatures(deviceID, error_code);
}

int
sbapi_get_irrad_cal_features(long deviceID, int *error_code, long *features,
        int max_features) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getIrradCalFeatures(deviceID, error_code,
            features, max_features);
}

int
sbapi_irrad_calibration_read(long deviceID, long featureID,
        int *error_code, float *buffer, int buffer_length) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->irradCalibrationRead(deviceID, featureID, error_code, buffer,
        buffer_length);
}

int
sbapi_irrad_calibration_write(long deviceID, long featureID,
        int *error_code, float *buffer, int buffer_length) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->irradCalibrationWrite(deviceID, featureID, error_code, buffer,
                buffer_length);
}

int
sbapi_irrad_calibration_has_collection_area(long deviceID, long featureID,
        int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->irradCalibrationHasCollectionArea(deviceID, featureID, error_code);
}

float
sbapi_irrad_calibration_read_collection_area(long deviceID, long featureID,
        int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->irradCalibrationReadCollectionArea(deviceID, featureID, error_code);
}

void
sbapi_irrad_calibration_write_collection_area(long deviceID, long featureID,
        int *error_code, float area) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->irradCalibrationWriteCollectionArea(deviceID, featureID, error_code, area);
}


int
sbapi_get_number_of_thermo_electric_features(long deviceID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNumberOfThermoElectricFeatures(deviceID, error_code);
}

int
sbapi_get_thermo_electric_features(long deviceID, int *error_code, long *features,
        int max_features) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getThermoElectricFeatures(deviceID, error_code,
            features, max_features);
}

double
sbapi_tec_read_temperature_degrees_C(long deviceID, long featureID,
        int *error_code) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->tecReadTemperatureDegreesC(deviceID, featureID, error_code);
}

void
sbapi_tec_set_temperature_setpoint_degrees_C(long deviceID, long featureID,
        int *error_code, double temperature_degrees_celsius) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    wrapper->tecSetTemperatureSetpointDegreesC(deviceID, featureID, error_code,
            temperature_degrees_celsius);
}

void
sbapi_tec_set_enable(long deviceID, long featureID,
        int *error_code, unsigned char tec_enable) {

    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    wrapper->tecSetEnable(deviceID, featureID, error_code, tec_enable);
}


int
sbapi_get_number_of_nonlinearity_coeffs_features(long deviceID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNumberOfNonlinearityCoeffsFeatures(deviceID, error_code);
}

int
sbapi_get_nonlinearity_coeffs_features(long deviceID, int *error_code, long *features,
        int max_features) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNonlinearityCoeffsFeatures(deviceID, error_code,
            features, max_features);
}

int sbapi_nonlinearity_coeffs_get(long deviceID, long featureID, int *error_code,
        double *buffer, int max_length) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->nonlinearityCoeffsGet(deviceID, featureID, error_code,
            buffer, max_length);
}


int
sbapi_get_number_of_stray_light_coeffs_features(long deviceID, int *error_code) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getNumberOfStrayLightCoeffsFeatures(deviceID, error_code);
}

int
sbapi_get_stray_light_coeffs_features(long deviceID, int *error_code, long *features,
        int max_features) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->getStrayLightCoeffsFeatures(deviceID, error_code,
            features, max_features);
}

int sbapi_stray_light_coeffs_get(long deviceID, long featureID, int *error_code,
        double *buffer, int max_length) {
    SeaBreezeAPI *wrapper = SeaBreezeAPI::getInstance();

    return wrapper->strayLightCoeffsGet(deviceID, featureID, error_code,
            buffer, max_length);
}