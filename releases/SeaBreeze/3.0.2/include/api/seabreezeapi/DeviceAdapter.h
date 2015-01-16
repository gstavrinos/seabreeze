/***************************************************//**
 * @file    DeviceAdapter.h
 * @date    February 2012
 * @author  Ocean Optics, Inc.
 *
 * This is a wrapper that allows
 * access to SeaBreeze Device instances.
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
#ifndef DEVICEADAPTER_H
#define DEVICEADAPTER_H

/* Includes */
#include "common/devices/Device.h"
#include "common/buses/DeviceLocatorInterface.h"
#include "api/seabreezeapi/EEPROMFeatureAdapter.h"
#include "api/seabreezeapi/IrradCalFeatureAdapter.h"
#include "api/seabreezeapi/NonlinearityCoeffsFeatureAdapter.h"
#include "api/seabreezeapi/SerialNumberFeatureAdapter.h"
#include "api/seabreezeapi/ShutterFeatureAdapter.h"
#include "api/seabreezeapi/SpectrometerFeatureAdapter.h"
#include "api/seabreezeapi/StrayLightCoeffsFeatureAdapter.h"
#include "api/seabreezeapi/StrobeLampFeatureAdapter.h"
#include "api/seabreezeapi/ContinuousStrobeFeatureAdapter.h"
#include "api/seabreezeapi/ThermoElectricCoolerFeatureAdapter.h"
#include "api/seabreezeapi/LightSourceFeatureAdapter.h"
#include <vector>

namespace seabreeze {
    namespace api {

        class DeviceAdapter {
        public:
            DeviceAdapter(Device *dev, unsigned long id);
            ~DeviceAdapter();

            int open(int *errorCode);
            void close();

            DeviceLocatorInterface *getLocation();

            /* An for weak association to this object */
            unsigned long getID();

            /* Get a string that describes the type of device */
            int getDeviceType(int *errorCode, char *buffer, unsigned int maxLength);

            /* Get one or more serial number features */
            int getNumberOfSerialNumberFeatures();
            int getSerialNumberFeatures(long *buffer, int maxFeatures);
            int getSerialNumber(long featureID, int *errorCode,
                    char *buffer, int bufferLength);

            /* Get one or more spectrometer acquisition features */
            int getNumberOfSpectrometerFeatures();
            int getSpectrometerFeatures(long *buffer, int maxFeatures);
            void spectrometerSetTriggerMode(long featureID, int *errorCode, int mode);
            void spectrometerSetIntegrationTimeMicros(long featureID, int *errorCode,
                    unsigned long integrationTimeMicros);
            unsigned long spectrometerGetMinimumIntegrationTimeMicros(
                    long featureID, int *errorCode);
            int spectrometerGetUnformattedSpectrumLength(
                    long featureID, int *errorCode);
            int spectrometerGetUnformattedSpectrum(long featureID,
                    int *errorCode, unsigned char *buffer, int bufferLength);
            int spectrometerGetFormattedSpectrumLength(
                    long featureID, int *errorCode);
            int spectrometerGetFormattedSpectrum(long featureID, int *errorCode,
                    double *buffer, int bufferLength);
            int spectrometerGetWavelengths(long featureID, int *errorCode,
                    double *wavelengths, int length);
            int spectrometerGetElectricDarkPixelCount(
                    long featureID, int *errorCode);
            int spectrometerGetElectricDarkPixelIndices(
                    long featureID, int *errorCode, int *indices, int length);

            /* Get one or more TEC features */
            int getNumberOfThermoElectricFeatures();
            int getThermoElectricFeatures(long *buffer, int maxFeatures);
            double tecReadTemperatureDegreesC(long featureID, int *errorCode);
            void tecSetTemperatureSetpointDegreesC(long featureID, int *errorCode,
                    double temperatureDegreesCelsius);
            void tecSetEnable(long featureID, int *errorCode, bool tecEnable);

            /* Get one or more irradiance calibration features */
            int getNumberOfIrradCalFeatures();
            int getIrradCalFeatures(long *buffer, int maxFeatures);
            int irradCalibrationRead(long featureID,
                    int *errorCode, float *buffer, int bufferLength);
            int irradCalibrationWrite(long featureID,
                    int *errorCode, float *buffer, int bufferLength);
            int irradCalibrationHasCollectionArea(long featureID, int *errorCode);
            float irradCalibrationReadCollectionArea(long featureID, int *errorCode);
            void irradCalibrationWriteCollectionArea(long featureID,
                    int *errorCode, float area);

            /* Get one or more EEPROM features */
            int getNumberOfEEPROMFeatures();
            int getEEPROMFeatures(long *buffer, int maxFeatures);
            int eepromReadSlot(long featureID, int *errorCode, int slotNumber,
                    unsigned char *buffer, int length);

            /* Get one or more light source features */
            int getNumberOfLightSourceFeatures();
            int getLightSourceFeatures(long *buffer, int maxFeatures);
            int lightSourceGetCount(long featureID, int *errorCode);
            bool lightSourceHasEnable(long featureID, int *errorCode,
                int lightSourceIndex);
            bool lightSourceIsEnabled(long featureID, int *errorCode,
                int lightSourceIndex);
            void lightSourceSetEnable(long featureID, int *errorCode,
                int lightSourceIndex, bool enable);
            bool lightSourceHasVariableIntensity(long featureID, int *errorCode,
                int lightSourceIndex);
            double lightSourceGetIntensity(long featureID, int *errorCode,
                int lightSourceIndex);
            void lightSourceSetIntensity(long featureID, int *errorCode,
                int lightSourceIndex, double intensity);

            /* Get one or more strobe lamp enable features */
            int getNumberOfStrobeLampFeatures();
            int getStrobeLampFeatures(long *buffer, int maxFeatures);
            void lampSetStrobeEnable(long featureID, int *errorCode, bool strobeEnable);

            /* Get one or more continuous strobe features */
            int getNumberOfContinuousStrobeFeatures();
            int getContinuousStrobeFeatures(long *buffer, int maxFeatures);
            void continuousStrobeSetPeriodMicroseconds(long featureID, int *errorCode,
                unsigned long period_usec);
            void continuousStrobeSetEnable(long featureID, int *errorCode, bool enable);

            /* Get one or more shutter features */
            int getNumberOfShutterFeatures();
            int getShutterFeatures(long *buffer, int maxFeatures);
            void shutterSetShutterOpen(long featureID, int *errorCode, bool opened);

            /* Get one or more nonlinearity coefficients features */
            int getNumberOfNonlinearityCoeffsFeatures();
            int getNonlinearityCoeffsFeatures(long *buffer, int maxFeatures);
            int nonlinearityCoeffsGet(long featureID, int *errorCode,
                    double *buffer, int bufferLength);

            /* Get one or more stray light coefficients features */
            int getNumberOfStrayLightCoeffsFeatures();
            int getStrayLightCoeffsFeatures(long *buffer, int maxFeatures);
            int strayLightCoeffsGet(long featureID, int *errorCode,
                    double *buffer, int bufferLength);

        protected:
            unsigned long instanceID;
            seabreeze::Device *device;
            std::vector<SerialNumberFeatureAdapter *> serialNumberFeatures;
            std::vector<SpectrometerFeatureAdapter *> spectrometerFeatures;
            std::vector<ThermoElectricCoolerFeatureAdapter *> tecFeatures;
            std::vector<IrradCalFeatureAdapter *> irradCalFeatures;
            std::vector<EEPROMFeatureAdapter *> eepromFeatures;
            std::vector<LightSourceFeatureAdapter *> lightSourceFeatures;
            std::vector<StrobeLampFeatureAdapter *> strobeLampFeatures;
            std::vector<ContinuousStrobeFeatureAdapter *> continuousStrobeFeatures;
            std::vector<ShutterFeatureAdapter *> shutterFeatures;
            std::vector<NonlinearityCoeffsFeatureAdapter *> nonlinearityFeatures;
            std::vector<StrayLightCoeffsFeatureAdapter *> strayLightFeatures;

            SerialNumberFeatureAdapter *getSerialNumberFeatureByID(long featureID);
            SpectrometerFeatureAdapter *getSpectrometerFeatureByID(long featureID);
            ThermoElectricCoolerFeatureAdapter *getTECFeatureByID(long featureID);
            IrradCalFeatureAdapter *getIrradCalFeatureByID(long featureID);
            EEPROMFeatureAdapter *getEEPROMFeatureByID(long featureID);
            LightSourceFeatureAdapter *getLightSourceFeatureByID(long featureID);
            StrobeLampFeatureAdapter *getStrobeLampFeatureByID(long featureID);
            ContinuousStrobeFeatureAdapter *getContinuousStrobeFeatureByID(long featureID);
            ShutterFeatureAdapter *getShutterFeatureByID(long featureID);
            NonlinearityCoeffsFeatureAdapter *getNonlinearityCoeffsFeatureByID(long featureID);
            StrayLightCoeffsFeatureAdapter *getStrayLightCoeffsFeatureByID(long featureID);
        };
    }
}

#endif
