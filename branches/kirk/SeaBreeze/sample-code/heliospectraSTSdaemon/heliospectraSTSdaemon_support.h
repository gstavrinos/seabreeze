/*******************************************************
 * File:    heliospectraSTSdaemon_support.h
 * Date:    January 2015
 * Author:  Heliospectra AB, Kirk C.
 *
 * Provides support for running the Ocean Optics STS
 * spectromteter from the Raspberry Pi
 *
 * c. 2015 Heliospectra AB
 *
 *******************************************************/
 
#ifndef HELIOSPECTRA_STS_DAEMON_SUPPORT_H
#define HELIOSPECTRA_STS_DAEMON_SUPPORT_H

#define MAX_LABEL_SIZE  31  // largest length of any spectrometer type or serial number
#define STRING_BUFFER_LENGTH 64  
#define STS_TRIGGER_MODE_NORMAL 0
#define STS_SHUTTER_CLOSED	0
#define STS_SHUTTER_OPENED	1
#define STS_PIXEL_COUNT 1024
#define STS_MIN_INTEGRATION 10  //10 us
#define STS_MAX_INTEGRATION 10000000 // 10s or 10000000 us
#define DATA_BUFFER_SIZE 80 // buffer size for writing out lines of data
#define STS_TEMPERATURE_DETECTOR 0 
#define STS_TEMPERATURE_RESERVED 1
#define STS_TEMPERATURE_CPU		 2
#define STS_SATURATION_VALUE	15564 //95% of 16384, full range for the AtoD

#define AVOGADRO 602214141070409084099072.0
#define ELECTRON_VOLT 1.602176565E-19
#define PLANCK_CONSTANT_eVS 4.135667516E-15
#define SPEED_OF_LIGHT_M_S 299792458.0
// temperature has been added to SeaBreeze
//DLL_DECL int sbapi_get_number_of_temperature_features (long deviceID, int *error_code)
 //DLL_DECL int 	sbapi_get_temperature_features (long deviceID, int *error_code, long *features, int max_features)
 //DLL_DECL int 	sbapi_temperature_get (long deviceID, long featureID, int *error_code, double *buffer, int max_length)

// storage for seabreeze api calls, 
// memory must be allocated for the big arrays, or a segmentation fault will result
struct seabreezeStorage
{
	long stsDeviceID;			// sts device ID for seabreeze api
	long stsSpectrometerID;		// Feature IDs
	long stsShutterID;
	long stsTemperatureID;
	long stsOpticalBenchID;
	long stsRevisionID;
	long stsSpectrumProcessingID;
	long stsNonlinearityCoeffID;
	long stsStrayLightCoeffID;
	long stsIrradCalID;
	long stsContinuousStrobeID;
	long stsCalibrationTableID;
	
	unsigned long minimumIntegrationTime;
	unsigned int integrationTime_us;
	unsigned short int scansToAverage;
	unsigned char boxcarFilterWidth;
	int formattedSpectrumLength;
	int unformattedSpectrumLength;
	int pixelCount;
	
	float stsCollectionArea_cm2;
	// small memory areas allocated separately rather than the complete stucture
	// to avoid segmentation faults on a small raspberryPi, 
	char  stsSerialNumber[MAX_LABEL_SIZE+1];
	float *stsCalibrationArray;	
	double *stsWavelengthArray;	
	double *stsFormattedSpectrumArray;	
	double *stsFormattedDarkSpectrumArray;
	unsigned char *stsRawSpectrumArray;
	unsigned char *stsRawDarkSpectrumArray;
};

struct waveband
{
	struct waveband *previous;
	struct waveband *next;
	int index;
	int wavelengthLow;
	int wavelengthLowIndex;
	int wavelengthHigh;
	int wavelengthHighIndex;
	double value;
};

int OpenSTS(void);
int CloseSTS(void);	
int SetSampleParametersSTS(int, int);
int SetSpectrumProcessingSTS(unsigned char, unsigned short int);
int TakeFormattedSampleSpectrumSTS(void);
int TakeFormattedDarkSpectrumSTS(void);
int TakeRawSampleSpectrumSTS(void);
int TakeRawDarkSpectrumSTS(void);
double GetBandwidth(int);
double GetWavelengthPixelValueSTS(int);
double GetCalibrationPixelValueSTS(int);
double GetFormattedSampleSpectrumPixelValueSTS(int);
double GetFormattedDarkReferenceSpectrumPixelValueSTS(int);
double GetFormattedAbsoluteIrradianceSpectrumPixelValueSTS_uW_cm2_nm(int);
double GetFormattedAbsoluteIrradianceSpectrumPixelValueSTS_umolq_m2_s_nm(int);
double GetRawSampleSpectrumPixelValueSTS(int);
double GetRawDarkReferenceSpectrumPixelValueSTS(int);
double GetRawAbsoluteIrradianceSpectrumPixelValueSTS_uW_cm2_nm(int);
double GetRawAbsoluteIrradianceSpectrumPixelValueSTS_umolq_m2_s_nm(int);
void CalculateRawAbsoluteIrradianceForWaveband_uw_cm2(struct waveband *);
void CalculateRawAbsoluteIrradianceForWaveband_umolq_m2_s(struct waveband *);
void CalculateFormattedAbsoluteIrradianceForWaveband_uw_cm2(struct waveband *);
void CalculateFormattedAbsoluteIrradianceForWaveband_umolq_m2_s(struct waveband *);
const char *GetErrorDescriptionSTS(int);
int AddWaveband(int, int);
int RemoveWavebandWithIndex(int);
struct waveband *GetWavebandWithIndex(int);
struct waveband *GetLastWaveband(void);
int GetLastWavebandIndex(void);
int GetTemperature_STS(double *, int);
const char *GetSerialNumber_STS(void);
int GetOpticalBenchDetails_STS(char *, int);
int GetBoxcarFilterWidth_STS(unsigned char *);
int GetScansToAverage_STS(unsigned short int *);
unsigned int GetIntegrationTime_ms_STS(void);
unsigned int GetIntegrationTime_us_STS(void);



#endif