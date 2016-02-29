/***************************************************//**
 * @file    QEProThermoElectricFeature.h
 * @date    September 2013
 * @author  Ocean Optics, Inc.
 *
 * This feature provides an interface to the thermo-
 * electric unit (TEC) on the QE-PRO.
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
#ifndef QEPROTHERMOELECTRICFEATURE_H
#define QEPROTHERMOELECTRICFEATURE_H

#include "vendors/OceanOptics/features/thermoelectric/ThermoElectricFeatureBase.h"

namespace seabreeze {
    class QEProThermoElectricFeature : public ThermoElectricFeatureBase {
    public:
        QEProThermoElectricFeature();
        virtual ~QEProThermoElectricFeature();

        /* Inherited from ThermoElectricFeatureBase where they are pure virtual */
        virtual double getDefaultSetPointCelsius(const Protocol &protocol,
                const Bus &bus) throw (FeatureException);
        virtual bool getDefaultThermoElectricEnable(const Protocol &protocol,
                const Bus &bus) throw (FeatureException);

        /* Override from Feature */
        virtual bool initialize(const Protocol &protocol, const Bus &bus)
            throw (FeatureException);
    };
}

#endif /* QEPROTHERMOELECTRICFEATURE_H */
