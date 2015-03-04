/*******************************************************
 * File:    heliospectraSTSdaemon_support.c
 * Date:    January 2015
 * Author:  Heliospectra AB, Kirk C.
 *
 * Provides support for running the Ocean Optics STS
 * spectromteter from the Raspberry Pi
 *
 * c. 2015 Heliospectra AB
 *
 *******************************************************/
 

#include <stdlib.h>		//calloc()
#include <stdio.h>		// printf() etc
#include <string.h>		// strcmp() etc.
#include <errno.h>		// standard linux errors
#include <math.h>		// round()

#include "api/seabreezeapi/SeaBreezeAPI.h"
#include "api/seabreezeapi/SeaBreezeAPIConstants.h"
#include "heliospectraSTSdaemon_support.h"
#include "heliospectraSTSdaemon_eventLoop.h"  // define  noError and success as 0

static struct seabreezeStorage sbAPI; // shared structure for most functions on this page
static struct waveband *myWavebands;

/****************************************************************************************/
// STS preparation functions check for errors in the previous action. If there was an error
//  the function does nothing and exits. If there was no error it performs its preparation
//  task. The int pointer argument is status, if that is noError, preparation is performed.
//  the char pointer pointer is an error description. 
//  This way a long line of things can get done and the code is easy to read
/****************************************************************************************/
int preparation_finish(int *status, char *errorMessage)
{
	if(*status!=noError)
	{
		printf("heliospectraSTSdaemon - %s\n\t%s\n", errorMessage, sbapi_get_error_string(*status));
		fflush(stdout);
	}
	return(*status);
}

void preparation_STS_allocateMemory(int *status, char *errorMessage)
{

	if(*status==noError)
	{
		sbAPI.stsCalibrationArray=malloc(sizeof(float)*STS_PIXEL_COUNT);
		if(sbAPI.stsCalibrationArray!=NULL)
		{
			sbAPI.stsWavelengthArray=malloc(sizeof(double)*STS_PIXEL_COUNT);
			if(sbAPI.stsWavelengthArray!=NULL)
			{
				sbAPI.stsFormattedSpectrumArray=malloc(sizeof(double)*STS_PIXEL_COUNT);
				if(sbAPI.stsFormattedSpectrumArray!=NULL)
				{
					sbAPI.stsFormattedDarkSpectrumArray=malloc(sizeof(double)*STS_PIXEL_COUNT);
					if(sbAPI.stsFormattedDarkSpectrumArray!=NULL)
					{
						sbAPI.stsRawSpectrumArray=malloc(sizeof(double)*STS_PIXEL_COUNT*2);
						if(sbAPI.stsRawSpectrumArray!=NULL)
						{
							sbAPI.stsRawDarkSpectrumArray=malloc(sizeof(double)*STS_PIXEL_COUNT*2);
							if(sbAPI.stsRawDarkSpectrumArray!=NULL)
								errno=noError; // errno is undefined after a successful malloc call
						}
					}
				}
			}
		}			
		if(errno!=noError)
		{
			*status=errno;
			strncpy(errorMessage, "Memory allocation failed.", STRING_BUFFER_LENGTH-1);
		}
	}
}

void preparation_STS_search(int *status, char *errorMessage)
{

	if (*status==noError)
	{
		if(sbapi_probe_devices()<=0)
		{
			*status=ENXIO; // no such device or address
			strncpy(errorMessage, "Could not find the STS.", STRING_BUFFER_LENGTH-1);
		}
	}
}

void preparation_STS_deviceID(int *status, char *errorMessage)
{
	if (*status==noError)
	{
		if (sbapi_get_device_ids(&(sbAPI.stsDeviceID), 1) < 1 )
		{
			*status=ENXIO; // no such device or address
			strncpy(errorMessage, "Could not get the STS device id.", STRING_BUFFER_LENGTH-1);
		}
	}
}

void preparation_STS_openDevice(int *status, char *errorMessage)
{
	if (*status==noError)
	{
		if (sbapi_open_device(sbAPI.stsDeviceID, status) != noError)
			strncpy(errorMessage, "Could not open the STS.", STRING_BUFFER_LENGTH-1);
	}
}

void preparation_STS_confirmDeviceTypeIsSTS(int *status, char *errorMessage)
{
	if (*status==noError)
	{
		char typeBuffer[16];
				
		if (sbapi_get_device_type(sbAPI.stsDeviceID, status, typeBuffer, sizeof(typeBuffer))>0) // number of bytes returned
		{
			if (strcmp("STS", typeBuffer)!=0)
			{
				*status=-1;
				strncpy(errorMessage, "The spectrometer found was not an STS", STRING_BUFFER_LENGTH-1);
			}
		}
		else
		{
			*status=-1;
			strncpy(errorMessage, "Could not get a spectrometer device type.", STRING_BUFFER_LENGTH-1);
		}
	}
}

void preparation_STS_serialNumber(int *status, char *errorMessage)
{
	long serialNumberID;
	
	if (*status==noError)
	{
		if(sbapi_get_serial_number_features(sbAPI.stsDeviceID, status, &serialNumberID, 1)>=1) 
		{
					
			/***********************  TOFIX *****************************/
			// do this twice as a workaround against an STS hang when 
			// sbapi_get_serial_number is called an even number of times
			sbapi_get_serial_number(sbAPI.stsDeviceID, serialNumberID, status, 
				sbAPI.stsSerialNumber, sizeof(sbAPI.stsSerialNumber)); 
			
			// returns the number of bytes in the serial number
			if(sbapi_get_serial_number(sbAPI.stsDeviceID, serialNumberID, status, 
				sbAPI.stsSerialNumber, sizeof(sbAPI.stsSerialNumber))<=0)
				strncpy(errorMessage, "Could not get an STS serial number.", STRING_BUFFER_LENGTH);
		}
		else
			strncpy(errorMessage, "Could not get the STS serial number feature.", STRING_BUFFER_LENGTH);		
	}
}

void preparation_STS_spectrometerID(int *status, char *errorMessage)
{
	if (*status==noError)
	{
		if(sbapi_get_spectrometer_features(sbAPI.stsDeviceID, 
					status, &(sbAPI.stsSpectrometerID), 1)!=1)
			strncpy(errorMessage, "Could not get an STS spectrometer ID.", STRING_BUFFER_LENGTH);
	}
}

void preparation_STS_shutterID(int *status, char *errorMessage)
{
	if (*status==noError)
	{
		if(sbapi_get_shutter_features(sbAPI.stsDeviceID, 
						status, &(sbAPI.stsShutterID), 1)!=1)
			strncpy(errorMessage, "Could not get an STS shutter ID.", STRING_BUFFER_LENGTH);
	}
}


void preparation_STS_temperatureID(int *status, char *errorMessage)
{
	if (*status==noError)
	{
		if(sbapi_get_temperature_features(sbAPI.stsDeviceID, 
						status, &(sbAPI.stsTemperatureID), 1)!=1)
			strncpy(errorMessage, "Could not get an STS temperature ID.", STRING_BUFFER_LENGTH);
	}
}

void preparation_STS_opticalBenchID(int *status, char *errorMessage)
{
	if (*status==noError)
	{
		if(sbapi_get_optical_bench_features(sbAPI.stsDeviceID, 
						status, &(sbAPI.stsOpticalBenchID), 1)!=1)
			strncpy(errorMessage, "Could not get an STS Optical Bench ID.", STRING_BUFFER_LENGTH);
	}
}

void preparation_STS_revisionID(int *status, char *errorMessage)
{
	if (*status==noError)
	{
		if(sbapi_get_revision_features(sbAPI.stsDeviceID, 
						status, &(sbAPI.stsRevisionID), 1)!=1)
			strncpy(errorMessage, "Could not get an STS Revision ID.", STRING_BUFFER_LENGTH);
	}
}

void preparation_STS_spectrumProcessingID(int *status, char *errorMessage)
{
	if (*status==noError)
	{
		if(sbapi_get_spectrum_processing_features(sbAPI.stsDeviceID, 
						status, &(sbAPI.stsSpectrumProcessingID), 1)!=1)
			strncpy(errorMessage, "Could not get an STS Spectrum Processing ID.", STRING_BUFFER_LENGTH);
	}
}

void preparation_STS_calibrationTable(int *status, char *errorMessage)
{
	if (*status==noError)
	{
		if(sbapi_get_irrad_cal_features(sbAPI.stsDeviceID, status, 
			&(sbAPI.stsIrradCalID), 1)==1)
		{
			if(sbapi_irrad_calibration_read(sbAPI.stsDeviceID, 
					sbAPI.stsIrradCalID, status, 
					sbAPI.stsCalibrationArray, STS_PIXEL_COUNT)==STS_PIXEL_COUNT)
			{
				sbAPI.stsCollectionArea_cm2 = 
					sbapi_irrad_calibration_read_collection_area(sbAPI.stsDeviceID, 
						sbAPI.stsIrradCalID, status);
				// result of collection area read is in status
				if(*status!=noError)
					strncpy(errorMessage, "Could not get the STS Collection Area.", STRING_BUFFER_LENGTH);
			}
			else
				strncpy(errorMessage, "Could not read the STS calibration table.", STRING_BUFFER_LENGTH);		
		}
		else
			strncpy(errorMessage, "Could not get an STS absolute irradiance calibration ID.", STRING_BUFFER_LENGTH);
	}
}

void preparation_STS_wavelengthTable(int *status, char *errorMessage)
{
	if (*status==noError)
	{
		sbAPI.stsWavelengthArray=malloc(sizeof(double)*STS_PIXEL_COUNT);
		//sbapi_spectrometer_get_wavelengths should handle a NULL pointer error
		if(sbapi_spectrometer_get_wavelengths(sbAPI.stsDeviceID, sbAPI.stsSpectrometerID, status, 
				sbAPI.stsWavelengthArray, STS_PIXEL_COUNT)==STS_PIXEL_COUNT)
			strncpy(errorMessage, "Could not get the STS wavelength table.", STRING_BUFFER_LENGTH);
	}
}
	
								
/****************************************************************************************/
//  Go through the seabreeze defined procedure to open a USB connection with an STS
//    spectrometer. 
/****************************************************************************************/
int OpenSTS(void)
{
	int sb_error_code=noError;
	char errorMessage[STRING_BUFFER_LENGTH]="No Error";	
	
	sbapi_initialize(); // prepare the Ocean Optics spectrometer interface
	preparation_STS_search(&sb_error_code, errorMessage);
	preparation_STS_deviceID(&sb_error_code, errorMessage);
	preparation_STS_openDevice(&sb_error_code, errorMessage);
	preparation_STS_confirmDeviceTypeIsSTS(&sb_error_code, errorMessage);
	preparation_STS_serialNumber(&sb_error_code, errorMessage);
	preparation_STS_spectrometerID(&sb_error_code, errorMessage);
	preparation_STS_spectrumProcessingID(&sb_error_code, errorMessage);
	preparation_STS_opticalBenchID(&sb_error_code, errorMessage);
	preparation_STS_revisionID(&sb_error_code, errorMessage);
	preparation_STS_shutterID(&sb_error_code, errorMessage);
	preparation_STS_temperatureID(&sb_error_code, errorMessage);
	preparation_STS_allocateMemory(&sb_error_code, errorMessage);
	preparation_STS_calibrationTable(&sb_error_code, errorMessage);
	preparation_STS_wavelengthTable(&sb_error_code, errorMessage);
	preparation_finish(&sb_error_code, errorMessage);

	return(sb_error_code); 
}


/****************************************************************************************/
// get the STS serial number
/****************************************************************************************/
const char *GetSerialNumber_STS(void)
{
	return(sbAPI.stsSerialNumber);
}

/****************************************************************************************/
// Release waveband memory
/****************************************************************************************/
void releaseWavebandMemory(void)
{
	struct waveband *nextFree;
	
	if (myWavebands!=NULL)
	do 
	{	
		nextFree=myWavebands->next;
		free(myWavebands);
		myWavebands=nextFree; //myWavebands = NULL; on last iteration
	} while (nextFree!=NULL);
}

/****************************************************************************************/
//  Close the STS connection and release memory for the seabreeze api
/****************************************************************************************/
int CloseSTS(void)
{
	int returnValue=-1;
	
	sbapi_close_device(sbAPI.stsDeviceID, &returnValue);
	sbapi_shutdown(); // free memory used by the seabreeze api
	free(sbAPI.stsCalibrationArray);
	free(sbAPI.stsWavelengthArray);
	free(sbAPI.stsFormattedSpectrumArray);
	free(sbAPI.stsFormattedDarkSpectrumArray);
	free(sbAPI.stsRawSpectrumArray);
	free(sbAPI.stsRawDarkSpectrumArray);
	releaseWavebandMemory();
	
	return(returnValue);
}  

/****************************************************************************************/
//  Prepare the STS for taking measurements, set trigger mode and integration time
/****************************************************************************************/				
int SetSampleParametersSTS(int triggerMode, int integrationTime_us)
{
	int returnValue=ERROR_INPUT_OUT_OF_BOUNDS;
	
	if ((integrationTime_us<=STS_MAX_INTEGRATION) && (integrationTime_us>=STS_MIN_INTEGRATION))
	{
		sbapi_spectrometer_set_trigger_mode	(sbAPI.stsDeviceID, sbAPI.stsSpectrometerID, &returnValue, triggerMode);
		if (returnValue==noError)
			sbapi_spectrometer_set_integration_time_micros	(sbAPI.stsDeviceID, sbAPI.stsSpectrometerID, &returnValue, integrationTime_us);
		if(returnValue==noError)
			sbAPI.integrationTime_us=integrationTime_us;
	}
	
	return(returnValue);
}

/****************************************************************************************/
//  Prepare the STS for taking measurements, set boxcar filter width and scans to average
/****************************************************************************************/				
int SetSpectrumProcessingSTS(unsigned char boxcarFilterWidth, unsigned short int scansToAverage)
{
	int returnValue;
	
	if((boxcarFilterWidth>=0) && (boxcarFilterWidth<=15) && (scansToAverage>0) && (scansToAverage<=5000))
	{
		sbapi_spectrum_processing_boxcar_width_set(sbAPI.stsDeviceID, sbAPI.stsSpectrumProcessingID, &returnValue, boxcarFilterWidth);
		if(returnValue==noError)
			sbAPI.boxcarFilterWidth=boxcarFilterWidth;
			sbapi_spectrum_processing_scans_to_average_set(sbAPI.stsDeviceID, sbAPI.stsSpectrumProcessingID, &returnValue, scansToAverage);
		if(returnValue==noError)
			sbAPI.scansToAverage=scansToAverage;
	}
	else
		returnValue=ERROR_INPUT_OUT_OF_BOUNDS;
		
	return(returnValue);
}

/****************************************************************************************/
// Get Temperature of an indexed temperature sensor
//  
/****************************************************************************************/				
int GetTemperature_STS(double *temperature, int whichTemperature)
{
	int returnValue=noError;
	
	if(whichTemperature==STS_TEMPERATURE_DETECTOR)
		*temperature=sbapi_temperature_get(sbAPI.stsDeviceID, sbAPI.stsTemperatureID, &returnValue, 0);
	else if(whichTemperature==STS_TEMPERATURE_CPU)
		*temperature=sbapi_temperature_get(sbAPI.stsDeviceID, sbAPI.stsTemperatureID, &returnValue, 2);
	else
	{
		returnValue=-1;
		*temperature=-275;
	}
		
	return(returnValue);	
}

/****************************************************************************************/
// Get the STS optical bench details

/****************************************************************************************/				
int GetOpticalBenchDetails_STS(char *opticalBenchDetails, int bufferLength)
{
	int returnValue=noError;
	char coating[8], filter[8], grating[8];
	
	sbapi_optical_bench_get_coating(sbAPI.stsDeviceID, sbAPI.stsOpticalBenchID, &returnValue, coating, sizeof(coating));
	if (returnValue==noError)
		sbapi_optical_bench_get_filter(sbAPI.stsDeviceID, sbAPI.stsOpticalBenchID, &returnValue, filter, sizeof(filter));
	if (returnValue==noError)
		sbapi_optical_bench_get_grating(sbAPI.stsDeviceID, sbAPI.stsOpticalBenchID, &returnValue, grating, sizeof(grating));
	
	if(returnValue==noError)
		snprintf(opticalBenchDetails, bufferLength, "coating: %s, filter: %s, grating: %s", coating, filter, grating);
	
	return(returnValue);	
}

/****************************************************************************************/
// Get the boxcar filter width

/****************************************************************************************/				
int GetBoxcarFilterWidth_STS(unsigned char *boxcarFilterWidth)
{
	int returnValue=noError;
	
	*boxcarFilterWidth=sbapi_spectrum_processing_boxcar_width_get(sbAPI.stsDeviceID, sbAPI.stsSpectrumProcessingID, &returnValue);
	
	return(returnValue);	
}

/****************************************************************************************/
// Get the scans to average 

/****************************************************************************************/				
int GetScansToAverage_STS(unsigned short int *scansToAverage)
{
	int returnValue=noError;
	
	*scansToAverage=sbapi_spectrum_processing_scans_to_average_get(sbAPI.stsDeviceID, sbAPI.stsSpectrumProcessingID, &returnValue);
	
	return(returnValue);	
}

/****************************************************************************************/
// Get the integration time in microseconds
//  note that the integration time cannot be gotten from the STS, only set
/****************************************************************************************/				
unsigned int GetIntegrationTime_us_STS(void)
{	
	return(sbAPI.integrationTime_us);	
}

/****************************************************************************************/
// Get the integration time in milliseconds
//  note that the integration time cannot be gotten from the STS, only set
/****************************************************************************************/				
unsigned int GetIntegrationTime_ms_STS(void)
{	
	return(round(sbAPI.integrationTime_us/1000.0));	
}

/****************************************************************************************/
// Get the wavelength for the specified pixel index STS
/****************************************************************************************/				
double GetWavelengthPixelValueSTS(int pixelNumber)
{
	double returnValue=-1.0;
	
	if ((pixelNumber>=0) && (pixelNumber<STS_PIXEL_COUNT))
			returnValue=sbAPI.stsWavelengthArray[pixelNumber];

	return returnValue;
}

/****************************************************************************************/
// Get the calibration number for the specified pixel index STS
/****************************************************************************************/				
double GetCalibrationPixelValueSTS(int pixelNumber)
{
	double returnValue=-1.0;
	
	if ((pixelNumber>=0) && (pixelNumber<STS_PIXEL_COUNT))
		returnValue=sbAPI.stsCalibrationArray[pixelNumber];

	return returnValue;
}

/****************************************************************************************/
// Get the formatted dark spectrum value for the specified pixel index STS
/****************************************************************************************/				
double GetFormattedDarkReferenceSpectrumPixelValueSTS(int pixelNumber)
{
	double returnValue=-1.0;
	
	if ((pixelNumber>=0) && (pixelNumber<STS_PIXEL_COUNT))
		returnValue=sbAPI.stsFormattedDarkSpectrumArray[pixelNumber];

	return returnValue;
}

/****************************************************************************************/
// Get the raw dark spectrum value for the specified pixel index STS
/****************************************************************************************/				
double GetRawDarkReferenceSpectrumPixelValueSTS(int pixelNumber)
{
	union
	{
		short int i;
		char c[2];
	} buffer;
	
	
	if ((pixelNumber>=0) && (pixelNumber<STS_PIXEL_COUNT))
	{
		buffer.c[0]=sbAPI.stsRawDarkSpectrumArray[pixelNumber*2];
		buffer.c[1]=sbAPI.stsRawDarkSpectrumArray[(pixelNumber*2)+1];
	}
	
	return (double)buffer.i;
}

/****************************************************************************************/
// Get the formatted spectrum value for the specified pixel index STS
/****************************************************************************************/				
double GetFormattedSampleSpectrumPixelValueSTS(int pixelNumber)
{
	double returnValue=-1.0;
	
	if ((pixelNumber>=0) && (pixelNumber<STS_PIXEL_COUNT))
			returnValue=sbAPI.stsFormattedSpectrumArray[pixelNumber];

	return returnValue;
}

/****************************************************************************************/
// Get the Raw spectrum value for the specified pixel index STS
/****************************************************************************************/				
double GetRawSampleSpectrumPixelValueSTS(int pixelNumber)
{
	union
	{
		short int i;
		char c[2];
	} buffer;
	
	
	if ((pixelNumber>=0) && (pixelNumber<STS_PIXEL_COUNT))
	{
		buffer.c[0]=sbAPI.stsRawSpectrumArray[pixelNumber*2];
		buffer.c[1]=sbAPI.stsRawSpectrumArray[(pixelNumber*2)+1];
	}
	
	return (double)buffer.i;
}

/****************************************************************************************/
// Take a formatted spectrum from the STS
/****************************************************************************************/				
int TakeFormattedSampleSpectrumSTS()
{
	int returnValue=noError;
	
	//sbapi_shutter_set_shutter_open(sbAPI.stsDeviceID, sbAPI.stsShutterID, &returnValue, 1);
	if (returnValue==noError)
		sbapi_spectrometer_get_formatted_spectrum(sbAPI.stsDeviceID, sbAPI.stsSpectrometerID, 
			&returnValue, sbAPI.stsFormattedSpectrumArray, STS_PIXEL_COUNT);

	for(int i=0;i<STS_PIXEL_COUNT;i++)
	{
		if(GetFormattedSampleSpectrumPixelValueSTS(i)>STS_SATURATION_VALUE)
		{
			returnValue=ERROR_SPECTROMETER_SATURATED;
			break;
		}
	}
		
	return(returnValue);	
}

/****************************************************************************************/
// Get a raw spectrum from the STS
/****************************************************************************************/				
int TakeRawSampleSpectrumSTS()
{
	int returnValue=noError;
	
	//sbapi_shutter_set_shutter_open(sbAPI.stsDeviceID, sbAPI.stsShutterID, &returnValue, 1);
	sbapi_spectrometer_get_unformatted_spectrum(sbAPI.stsDeviceID, sbAPI.stsSpectrometerID, 
			&returnValue, sbAPI.stsRawSpectrumArray, STS_PIXEL_COUNT*2);

	for(int i=0;i<STS_PIXEL_COUNT;i++)
	{
		if(GetRawSampleSpectrumPixelValueSTS(i)>STS_SATURATION_VALUE)
		{
			returnValue=ERROR_SPECTROMETER_SATURATED;
			break;
		}
	}
				
	return(returnValue);	
}

/****************************************************************************************/
// Get a formatted dark spectrum from the STS
/****************************************************************************************/				
int TakeFormattedDarkSpectrumSTS()
{
	int returnValue=noError;
	
	//Shutter in the STS does not work. This must be taken with lights off
	//sbapi_shutter_set_shutter_open(sbAPI.stsDeviceID, sbAPI.stsShutterID, &returnValue, 0);
	if (returnValue==noError)
		sbapi_spectrometer_get_formatted_spectrum(sbAPI.stsDeviceID, sbAPI.stsSpectrometerID, 
			&returnValue, sbAPI.stsFormattedDarkSpectrumArray, STS_PIXEL_COUNT);

	for(int i=0;i<STS_PIXEL_COUNT;i++)
	{
		if(GetFormattedDarkReferenceSpectrumPixelValueSTS(i)>STS_SATURATION_VALUE)
		{
			returnValue=ERROR_SPECTROMETER_SATURATED;
			break;
		}
	}
		
	return(returnValue);	
}

/****************************************************************************************/
// Take a raw dark spectrum from the STS
/****************************************************************************************/				
int TakeRawDarkSpectrumSTS()
{
	int returnValue=noError;
	
	//Shutter in the STS does not work. This must be taken with lights off
	//sbapi_shutter_set_shutter_open(sbAPI.stsDeviceID, sbAPI.stsShutterID, &returnValue, 0);
	if (returnValue==noError)
		sbapi_spectrometer_get_unformatted_spectrum(sbAPI.stsDeviceID, sbAPI.stsSpectrometerID, 
			&returnValue, sbAPI.stsRawDarkSpectrumArray, STS_PIXEL_COUNT*2);

	for(int i=0;i<STS_PIXEL_COUNT;i++)
	{
		if(GetRawDarkReferenceSpectrumPixelValueSTS(i)>STS_SATURATION_VALUE)
		{
			returnValue=ERROR_SPECTROMETER_SATURATED;
			break;
		}
	}
		
	return(returnValue);	
}

/****************************************************************************************/
// Get the width of the wavelength spread across the STS ccd pixel
/****************************************************************************************/
double GetBandwidth(int pixelNumber)
{
		double deltaWavelength=1; 
		
		// The wavelength is the center of the ccd pixel. The width of the ccd element is
		// defined as halfway in between the two centers, however this is not exactly the
		// same due to manufacturing and optical difference in the SST bench. So the 
		// borders of a pixel are halfway from the previous and halfway to the next.
		// On the edges there are only half pixels. So to estimate the actual wavelength
		//  size of the ccd by multiplying the known half by 2.
		
		if(pixelNumber==0) // bottom pixel
			deltaWavelength=(sbAPI.stsWavelengthArray[1]-sbAPI.stsWavelengthArray[0]); 
		else if(pixelNumber==STS_PIXEL_COUNT-1) // top pixel
			deltaWavelength=(sbAPI.stsWavelengthArray[STS_PIXEL_COUNT-1]-sbAPI.stsWavelengthArray[STS_PIXEL_COUNT-2]);
		else // all of the rest
			deltaWavelength=(sbAPI.stsWavelengthArray[pixelNumber+1]-sbAPI.stsWavelengthArray[pixelNumber-1])/2.0;
			
	return deltaWavelength;
}

/****************************************************************************************/
// Get the raw absolute spectral irradiance (uw/cm2/nm) for the specified pixel index.
/****************************************************************************************/				
double GetRawAbsoluteIrradianceSpectrumPixelValueSTS_uW_cm2_nm(int pixelNumber)
{
	double driftCorrectedPoint=0;
	double spectral_irradiance_uw_cm2_nm=0;
	
	if ((pixelNumber>=0) && (pixelNumber<STS_PIXEL_COUNT))
	{
	  // calculate irradiance in uWatts/cm2/nm
      // IP =(SP -DP)*CP /(T*A*dLP) sp=spectral counts, dp=dark counts, cp=calibration, t=integration time (s),
      // A=collection area (cm^2^, dLP=wavelength spread 	
	
		driftCorrectedPoint=GetRawSampleSpectrumPixelValueSTS(pixelNumber)-
			GetRawDarkReferenceSpectrumPixelValueSTS(pixelNumber);
		
		spectral_irradiance_uw_cm2_nm=(driftCorrectedPoint*sbAPI.stsCalibrationArray[pixelNumber])/
				((sbAPI.integrationTime_us/1000000.0)*sbAPI.stsCollectionArea_cm2*GetBandwidth(pixelNumber));
	}

	return(spectral_irradiance_uw_cm2_nm);
}

/****************************************************************************************/
// Get the Formatted absolute spectral irradiance (uw_cm2/nm) for the specified pixel index.
/****************************************************************************************/				
double GetFormattedAbsoluteIrradianceSpectrumPixelValueSTS_uW_cm2_nm(int pixelNumber)
{
	double driftCorrectedPoint=0;
	double spectral_irradiance_uw_cm2_nm=0;

	if ((pixelNumber>=0) && (pixelNumber<STS_PIXEL_COUNT))
	{
		driftCorrectedPoint=GetFormattedSampleSpectrumPixelValueSTS(pixelNumber)-
			GetFormattedDarkReferenceSpectrumPixelValueSTS(pixelNumber);
					
		spectral_irradiance_uw_cm2_nm=(driftCorrectedPoint*sbAPI.stsCalibrationArray[pixelNumber])/
			((sbAPI.integrationTime_us/1000000.0)*sbAPI.stsCollectionArea_cm2*GetBandwidth(pixelNumber));
	}

	return(spectral_irradiance_uw_cm2_nm);
}

/****************************************************************************************/
// Get the raw absolute irradiance (umol q/m2/s/nm) for the specified pixel index.
/****************************************************************************************/				
double GetRawAbsoluteIrradianceSpectrumPixelValueSTS_umolq_m2_s_nm(int pixelNumber)
{

  // Watts / m^2 to (µmol photons / meter^2 / second)

	double avogadroConstant=AVOGADRO;
	double eV=ELECTRON_VOLT;
	double eV_from_wavelength;
  	double spectralIrradiance_w_m2_nm;
  	double umolq_m2_s_nm;
  	
	// (uw/cm2/nm) / 100 = W/m2/nm
	spectralIrradiance_w_m2_nm=GetRawAbsoluteIrradianceSpectrumPixelValueSTS_uW_cm2_nm(pixelNumber)/100.0;
	eV_from_wavelength=(PLANCK_CONSTANT_eVS*SPEED_OF_LIGHT_M_S*1000000000.0)/GetWavelengthPixelValueSTS(pixelNumber);
	umolq_m2_s_nm= (1000000.0*spectralIrradiance_w_m2_nm)/(eV*avogadroConstant*eV_from_wavelength);
  	
  	return umolq_m2_s_nm;
}

/****************************************************************************************/
// Get the Formatted absolute irradiance (umol q/m2/s/nm) for the specified pixel index.
/****************************************************************************************/				
double GetFormattedAbsoluteIrradianceSpectrumPixelValueSTS_umolq_m2_s_nm(int pixelNumber)
{

  // Watts / m^2 to (µmol photons / meter^2 / second)

	double avogadroConstant=AVOGADRO;
	double eV=ELECTRON_VOLT;
	double eV_from_wavelength;
  	double spectralIrradiance_w_m2_nm;
  	double umolq_m2_s_nm;
  	
	// (uw/cm2/nm) / 100 = W/m2/nm
	spectralIrradiance_w_m2_nm=GetFormattedAbsoluteIrradianceSpectrumPixelValueSTS_uW_cm2_nm(pixelNumber)/100.0;
	eV_from_wavelength=(PLANCK_CONSTANT_eVS*SPEED_OF_LIGHT_M_S*1000000000.0)/GetWavelengthPixelValueSTS(pixelNumber);
	umolq_m2_s_nm= (1000000.0*spectralIrradiance_w_m2_nm)/(eV*avogadroConstant*eV_from_wavelength);

	return umolq_m2_s_nm;
}

void CalculateRawAbsoluteIrradianceForWaveband_uw_cm2(struct waveband *aWaveband)
{
	aWaveband->value=0;
	
	// waveband was already checked for nonNull
	for(int i=aWaveband->wavelengthLowIndex; i<=aWaveband->wavelengthHighIndex; i++)
		aWaveband->value=aWaveband->value+(GetRawAbsoluteIrradianceSpectrumPixelValueSTS_uW_cm2_nm(i)*GetBandwidth(i));
}

void CalculateRawAbsoluteIrradianceForWaveband_umolq_m2_s(struct waveband *aWaveband)
{
	aWaveband->value=0;
	// waveband was already checked for nonNull
	for(int i=aWaveband->wavelengthLowIndex; i<=aWaveband->wavelengthHighIndex; i++)
		aWaveband->value=aWaveband->value+(GetRawAbsoluteIrradianceSpectrumPixelValueSTS_umolq_m2_s_nm(i)*GetBandwidth(i));
}

void CalculateFormattedAbsoluteIrradianceForWaveband_uw_cm2(struct waveband *aWaveband)
{
	aWaveband->value=0;

	// waveband was already checked for nonNull
	for(int i=aWaveband->wavelengthLowIndex; i<=aWaveband->wavelengthHighIndex; i++)
		aWaveband->value=aWaveband->value+(GetFormattedAbsoluteIrradianceSpectrumPixelValueSTS_uW_cm2_nm(i)*GetBandwidth(i));
}

void CalculateFormattedAbsoluteIrradianceForWaveband_umolq_m2_s(struct waveband *aWaveband)
{
	aWaveband->value=0;

	// waveband was already checked for nonNull
	for(int i=aWaveband->wavelengthLowIndex; i<=aWaveband->wavelengthHighIndex; i++)
		aWaveband->value=aWaveband->value+(GetFormattedAbsoluteIrradianceSpectrumPixelValueSTS_umolq_m2_s_nm(i)*GetBandwidth(i));
}

/****************************************************************************************/
// Get a printable error description
/****************************************************************************************/
const char *GetErrorDescriptionSTS(int sb_error_code)
{
	return(sbapi_get_error_string(sb_error_code));
}

/****************************************************************************************/
// Get the Last waveband index
/****************************************************************************************/
int GetLastWavebandIndex(void)
{
	int returnValue=-1;
	struct waveband *nextPtr;
	
	nextPtr=myWavebands;
	while(nextPtr!=NULL)
	{
		if((nextPtr->index)>returnValue)
			returnValue=nextPtr->index;
		
		if(nextPtr->next!=NULL)
			nextPtr=nextPtr->next;
		else
			break;
	}

	return(returnValue);
}

/****************************************************************************************/
// Get a pointer to the last waveband 
/****************************************************************************************/
struct waveband *GetLastWaveband(void)
{
	struct waveband *nextPtr;
	
	nextPtr=myWavebands;
	while(nextPtr!=NULL)
	{
		if(nextPtr->next!=NULL)
			nextPtr=nextPtr->next;
		else
			break;
	}

	return(nextPtr);
}

/****************************************************************************************/
// Get the STS pixel index below or equal to the wavelength entered
/****************************************************************************************/
int GetSTSPixelBelowWavelength(int wavelength)
{
	int i=0;
	// 
	for(i=STS_PIXEL_COUNT-1; i>=0; i--)
	{
		if(sbAPI.stsWavelengthArray[i]<=wavelength)
			break;
	}
	// this should never be used because the waveband should be valid
	if(i<0)
		i=0;
	return i;
}

/****************************************************************************************/
// Get the STS pixel index above or equal to the wavelength entered
/****************************************************************************************/
int GetSTSPixelAboveWavelength(int wavelength)
{
	int i=0;
	
	for(i=0; i<=STS_PIXEL_COUNT-1; i++)
	{
		if(sbAPI.stsWavelengthArray[i]>=wavelength)
			break;
	}
	// this should never be used because the waveband should be valid
	if(i>STS_PIXEL_COUNT-1)
		i=STS_PIXEL_COUNT-1;
	return i;
}

/****************************************************************************************/
// Add a waveband to the spectrum calculation
/****************************************************************************************/
int AddWaveband(int lowWavelength, int highWavelength)
{
	struct waveband *newWaveband;
	int returnValue=-1;

	if((lowWavelength<highWavelength) && (lowWavelength>=GetWavelengthPixelValueSTS(0)) && 
			(highWavelength<=GetWavelengthPixelValueSTS(STS_PIXEL_COUNT-1)))
	{
		newWaveband=(struct waveband *)calloc(1, sizeof(struct waveband));
		
		if (newWaveband!=NULL)
		{
			newWaveband->previous=GetLastWaveband();
			newWaveband->next=NULL;
			newWaveband->index=GetLastWavebandIndex()+1;
			newWaveband->wavelengthLow=lowWavelength;
			newWaveband->wavelengthLowIndex=GetSTSPixelBelowWavelength(lowWavelength);
			newWaveband->wavelengthHigh=highWavelength;
			newWaveband->wavelengthHighIndex=GetSTSPixelAboveWavelength(highWavelength);
			newWaveband->value=0;
			
			if (newWaveband->previous==NULL)
				myWavebands=newWaveband;
			else
				(newWaveband->previous)->next=newWaveband;
				
			returnValue=(newWaveband->index)+1;
		}
	}

	return(returnValue);
}

/****************************************************************************************/
// Get a pointer to a waveband from the index
/****************************************************************************************/
struct waveband *GetWavebandWithIndex(int index)
{
	struct waveband *returnValue=NULL;
	
	returnValue=myWavebands;
	while(returnValue!=NULL)
	{
		if((returnValue->index)!=index)
				returnValue=returnValue->next;
		else
			break;
	}

	return(returnValue);
}

/****************************************************************************************/
// Remove a waveband using its index
/****************************************************************************************/
int RemoveWavebandWithIndex(int index)
{
	struct waveband *target;
	int returnValue=-1;
	int counter=0;
	
	target=GetWavebandWithIndex(index); //if myWaveband == NULL this returns NULL
	if (target!=NULL)
	{
		if(target->previous==NULL)
		{
			myWavebands=target->next;
			if(myWavebands!=NULL)
				myWavebands->previous=NULL;
		}
		else
		{
			(target->previous)->next=target->next;
			if (target->next!=NULL)
				(target->next)->previous=target->previous;
		}
		free(target);
	
		// renumber the wavebands so they are sequencial
		target=myWavebands;
		while(target!=NULL)
		{
			target->index=counter++;
			target=target->next;
		}

		returnValue=counter;
	}

	return(returnValue);
}


