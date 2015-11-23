
/*******************************************************
 * File:    heliospectraSTSdaemon_eventLoop.c
 * Date:    January 2015
 * Author:  Heliospectra AB, Kirk C.
 *
 * Provides the EventLoop function for the 
 * heliospectraSTSdaemon
 *
 * c. 2015 Heliospectra AB
 *
 *******************************************************/

// networking
#include <sys/types.h>
#include <sys/socket.h>	// define the socket parameters
#include <netinet/in.h>	// define internet structures
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pwd.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <poll.h>

#include <time.h>		// system clock functions
#include <sys/time.h>	// timer functions
#include <string.h>		// memset()

#include "heliospectraSTSdaemon_support.h"
#include "heliospectraSTSdaemon_eventLoop.h"


/***************************************************************************/
// The Ocean Optics version of Pidora is PREEMPT_RT configured
//  A typical RPi A has a clock drift under 50ppm
//   (http://blog.remibergsma.com/2013/05/12/how-accurately-can-the-raspberry-pi-keep-time/)
// Latency with the PREEMPT_RT patch is acceptable
//  (https://www.mpi-sws.org/~bbb/papers/pdf/ospert13.pdf)
/***************************************************************************/



/***************************************************************************/
// Send utilities
/***************************************************************************/
void SendMessage(int clientFileDescriptor, int *connectedToClient, const char *message)
{
	if (write(clientFileDescriptor, message, strlen(message))==0)
		*connectedToClient=FALSE;
}

void SendOK(int clientFileDescriptor, int *connectedToClient)
{
	SendMessage(clientFileDescriptor, connectedToClient, "OK\n");
}

void SendUnknownCommand(int clientFileDescriptor, int *connectedToClient)
{
	SendMessage(clientFileDescriptor, connectedToClient, "ER - Unrecognized Command\n");
}

void SendBadCommandFormat(int clientFileDescriptor, int *connectedToClient)
{
	SendMessage(clientFileDescriptor, connectedToClient, "ER - Bad Command Format\n");
}

void SendBadArgumentRange(int clientFileDescriptor, int *connectedToClient)
{
	SendMessage(clientFileDescriptor, connectedToClient, "ER - Bad Argument Range\n");
}

void SendMessageErrorSTS(int clientFileDescriptor, int *connectedToClient, int stsError)
{
	SendMessage(clientFileDescriptor, connectedToClient, "ER - ");
	SendMessage(clientFileDescriptor, connectedToClient, GetErrorDescriptionSTS(stsError));
	SendMessage(clientFileDescriptor, connectedToClient, "\n");
}

/***************************************************************************/
// Send a list of commands back to the client
/***************************************************************************/
void SendCommandList(int clientFileDescriptor, int *connectedToClient)
{
	SendOK(clientFileDescriptor, connectedToClient);
	
	char *commandList=
	"? - returns: a list of all the recognized commands, <crlf> OK\n"
	"x - exits the event loop so that the daemon stops, the OS restarts it, returns: OK\n"
	"q - quits the current session, closes the connection, the daemon continues, returns: OK\n"
	"t - take spectrum, when done, returns: <crlf> OK\n"
	"    t 0 for the uncorrected dark reference spectrum\n"
	"    t 1 for the uncorrected sample spectrum\n"
	"    t 2 for the corrected dark reference spectrum\n"
	"    t 3 for the corrected sample spectrum\n"
	"s - send spectrum, returns: a long list of tab separated values <crlf> OK\n"
	"    spectrums return tab separated wavelength and spectrometer count values\n"
	"     corrected spectrums are automatically offset for short term temperature drift\n"
	"    absolute irradiance returns tab separated wavelength and irradiance\n"
	"    s 0 for the uncorrected dark reference spectrum\n"
	"    s 1 for the uncorrected sample spectrum\n"
	"    s 2 for the corrected dark reference spectrum\n"
	"    s 3 for the corrected sample spectrum\n"
	"    s 4 for the uncorrected absolute irradiance flux uW/cm2/nm\n"	
	"    s 5 for the uncorrected absolute irradiance umol q/cm2/s/nm\n"
	"    s 6 for the corrected absolute irradiance flux uW/cm2/nm\n"
	"    s 7 for the corrected absolute irradiance umol q/cm2/s/nm\n"
	"    s 8 for the wavelength table\n"
	"    s 9 for the calibration table\n"	
	"i - integration time setting in milliseconds, returns: OK\n"
	"    i time_ms, ex: 4 seconds would be written i 4000\n"
	"I - integration time setting in microseconds, returns: OK\n"
	"    I time_us, ex: 4 seconds would be written I 4000000\n"
	"b - boxcar filter width and scans to average, returns: OK\n"
	"    b boxcarFilterWidth, scansToAverage ex: b 10 5 (boxcar filter is 0-15, scans to average 1-5000)\n"
	"a - add waveband, returns: OK\n"
	"    a lowWavelength highWavelength, ex: a 400 700\n"
	"r - remove waveband, returns: OK\n"
	"    r wavebandIndex, ex: r 2\n"
	"l - list wavebands, returns: tab separated wavebands with indexes, low and high wavelengths <crlf> OK\n"
	"    for example: 1<tab>400<tab>700<crlf>2<tab>500<tab>600<crlf>, etc. <crlf> OK)\n"
	"d - detector temperature, returns: temperature centigrade <crlf> OK\n"
	"w - waveband irradiance, returns: absolute irradiance for each waveband, tab delimited values, <crlf> OK\n"
	"    ex: wavebandIndex <tab> irradiance <tab> wavebandIndex <space> irradiance<crlf>\n"
	"    w 0 for uncorrected spectral wavebands, uW/cm2\n"
	"    w 1 for uncorrected spectral wavebands, umol/m2/s\n"
	"    w 2 for corrected spectral wavebands, uW/cm2\n"
	"    w 3 for corrected spectral wavebands, umol/m2/s\n"
	"    w 4 for uncorrected spectra wavebands, uW/cm2, umol/m2/s\n"
	"    w 5 for corrected spectra wavebands, uW/cm2, umol/m2/s\n"
	"g - get values from STS, returns: value <crlf> OK\n"
	"    g 0 get serial number\n"
	"    g 1 get optical bench description\n"
	"    g 2 get CPU temperature, in degrees C\n"
	"    g 3 get boxcar filter width\n"
	"    g 4 get the number of scans to average\n"
	"    g 5 get integration time in milliseconds\n"
	"    g 6 get integration time in microseconds\n";

	SendMessage(clientFileDescriptor, connectedToClient, commandList);
}

/***************************************************************************/
// Send the temperature of the detector in degrees C
/***************************************************************************/
void SendDetectorTemperature(int clientFileDescriptor, int *connectedToClient)
{
	char dataBuffer[DATA_BUFFER_SIZE];
	double temperature;
	int error;
	
	if((error=GetTemperature_STS(&temperature, STS_TEMPERATURE_DETECTOR))==noError)
	{
		sprintf(dataBuffer, "%4.2f\nOK\n", temperature);
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}
	else
	{
		sprintf(dataBuffer, "ER (%d)- Failed to get temperature.\n%s\n", error, GetErrorDescriptionSTS(error));
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}

}

/***************************************************************************/
// Send the temperature of the CPU in degrees C
/***************************************************************************/
void SendTemperatureCPU(int clientFileDescriptor, int *connectedToClient)
{
	char dataBuffer[DATA_BUFFER_SIZE];
	double temperature;
	int error;
	
	if((error=GetTemperature_STS(&temperature, STS_TEMPERATURE_CPU))==noError)
	{
		snprintf(dataBuffer, DATA_BUFFER_SIZE, "%4.2f\nOK\n", temperature);
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}
	else
	{
		sprintf(dataBuffer, "ER (%d)- Failed to get temperature.\n%s\n", error, GetErrorDescriptionSTS(error));
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}

}

/***************************************************************************/
// Send the STS serial number
/***************************************************************************/
void SendSerialNumber(int clientFileDescriptor, int *connectedToClient)
{
	char dataBuffer[DATA_BUFFER_SIZE];
	
	snprintf(dataBuffer, DATA_BUFFER_SIZE, "%s\nOK\n", GetSerialNumber_STS());
	SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);

}

/***************************************************************************/
// Send a description of the optical bench
/***************************************************************************/
void SendOpticalBenchInfo(int clientFileDescriptor, int *connectedToClient)
{
	char benchBuffer[DATA_BUFFER_SIZE];
	char sendBuffer[DATA_BUFFER_SIZE];
	int error;

	if ((error=GetOpticalBenchDetails_STS(benchBuffer, DATA_BUFFER_SIZE))==noError)
	{
		snprintf(sendBuffer, DATA_BUFFER_SIZE, "%s\nOK\n", benchBuffer);
		SendMessage(clientFileDescriptor, connectedToClient, sendBuffer);
	}
	else
	{
		snprintf(sendBuffer, DATA_BUFFER_SIZE, "ER (%d)- optical bench details\n%s\n", error, GetErrorDescriptionSTS(error));
		SendMessage(clientFileDescriptor, connectedToClient, sendBuffer);
	}
}

/***************************************************************************/
// Send the boxcar filter width
/***************************************************************************/
void SendBoxcarWidth(int clientFileDescriptor, int *connectedToClient)
{
	char sendBuffer[DATA_BUFFER_SIZE];
	unsigned char boxcarWidth;
	int error;

	if ((error=GetBoxcarFilterWidth_STS(&boxcarWidth))==noError)
	{
		snprintf(sendBuffer, DATA_BUFFER_SIZE, "%d\nOK\n", boxcarWidth);
		SendMessage(clientFileDescriptor, connectedToClient, sendBuffer);
	}
	else
	{
		snprintf(sendBuffer, DATA_BUFFER_SIZE, "ER (%d)- Boxcar filter width\n%s\n", error, GetErrorDescriptionSTS(error));
		SendMessage(clientFileDescriptor, connectedToClient, sendBuffer);
	}
}

/***************************************************************************/
// Send the Scans to Average filter width
/***************************************************************************/
void SendScansToAverage(int clientFileDescriptor, int *connectedToClient)
{
	char sendBuffer[DATA_BUFFER_SIZE];
	unsigned short int scansToAverage;
	int error;

	if ((error=GetScansToAverage_STS(&scansToAverage))==noError)
	{
		snprintf(sendBuffer, DATA_BUFFER_SIZE, "%d\nOK\n", scansToAverage);
		SendMessage(clientFileDescriptor, connectedToClient, sendBuffer);
	}
	else
	{
		snprintf(sendBuffer, DATA_BUFFER_SIZE, "ER (%d)- scans to average\n%s\n", error, GetErrorDescriptionSTS(error));
		SendMessage(clientFileDescriptor, connectedToClient, sendBuffer);
	}
}

/***************************************************************************/
// Send the integration time in microseconds
/***************************************************************************/
void SendIntegrationTime_us(int clientFileDescriptor, int *connectedToClient)
{
	char sendBuffer[DATA_BUFFER_SIZE];

	snprintf(sendBuffer, DATA_BUFFER_SIZE, "%d\nOK\n", GetIntegrationTime_us_STS());
	SendMessage(clientFileDescriptor, connectedToClient, sendBuffer);

}

/***************************************************************************/
// Send the integration time in milleseconds
/***************************************************************************/
void SendIntegrationTime_ms(int clientFileDescriptor, int *connectedToClient)
{
	char sendBuffer[DATA_BUFFER_SIZE];

	snprintf(sendBuffer, DATA_BUFFER_SIZE, "%d\nOK\n", GetIntegrationTime_ms_STS());
	SendMessage(clientFileDescriptor, connectedToClient, sendBuffer);

}

/***************************************************************************/
// Send Wavebandlist
/***************************************************************************/
void SendWavebandList(int clientFileDescriptor, int *connectedToClient)
{
	int theLastIndex, i;
	int notOK=FALSE;
	struct waveband *aWaveband;
	char dataBuffer[DATA_BUFFER_SIZE];
	
	theLastIndex=GetLastWavebandIndex();
	if (theLastIndex>=0)
	{		
		for(i=0; i<=theLastIndex;i++)
		{
			aWaveband=GetWavebandWithIndex(i);
			if(aWaveband!=NULL)
			{
				sprintf(dataBuffer, "%d\t%d\t%d\n", aWaveband->index,	
						aWaveband->wavelengthLow, aWaveband->wavelengthHigh);
				SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
			}
			else
			{
				SendBadCommandFormat(clientFileDescriptor, connectedToClient);
				notOK=TRUE;
				break;
			}
		}
	}
	else
	{
		SendMessage(clientFileDescriptor, connectedToClient, "ER - There were no wavebands.\n");
		notOK=TRUE;
	}
	
	if(!notOK)
		SendOK(clientFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Send a list of the STS wavelengths, pixel \t wavelength \n
/***************************************************************************/
void SendWavelengthList(int clientFileDescriptor, int *connectedToClient)
{
	int i;
	
	char dataBuffer[DATA_BUFFER_SIZE];
	
	for(i=0; i<=STS_PIXEL_COUNT-1; i++)
	{
		sprintf(dataBuffer, "%d\t%f\n", i, GetWavelengthPixelValueSTS(i));
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}
	SendOK(clientFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Send a list of the calibration points, pixel number \t calibration \n
/***************************************************************************/
void SendCalibrationTable(int clientFileDescriptor, int *connectedToClient)
{
	int i;
	
	char dataBuffer[DATA_BUFFER_SIZE];
	
	for(i=0; i<=STS_PIXEL_COUNT-1; i++)
	{
		sprintf(dataBuffer, "%d\t%f\tuJ/count\n", i, GetCalibrationPixelValueSTS(i));
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}
	SendOK(clientFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Send a list of the raw dark reference spectrum points pixel \t value \n
/***************************************************************************/
void SendRawDarkReferenceSpectrum(int clientFileDescriptor, int *connectedToClient)
{
	int i;
	
	char dataBuffer[DATA_BUFFER_SIZE];
	
	for(i=0; i<=STS_PIXEL_COUNT-1; i++)
	{
		sprintf(dataBuffer, "%d\t%5.0f\n", i, GetRawDarkReferenceSpectrumPixelValueSTS(i));
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}
	SendOK(clientFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Send a list of the formatted dark reference spectrum points pixel \t value \n
/***************************************************************************/
void SendFormattedDarkReferenceSpectrum(int clientFileDescriptor, int *connectedToClient)
{
	int i;
	
	char dataBuffer[DATA_BUFFER_SIZE];
	
	for(i=0; i<=STS_PIXEL_COUNT-1; i++)
	{
		sprintf(dataBuffer, "%d\t%5.0f\n", i, GetFormattedDarkReferenceSpectrumPixelValueSTS(i));
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}
	SendOK(clientFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Send a list of the Raw sample spectrum points, pixel number \t value \n
/***************************************************************************/
void SendRawSampleSpectrum(int clientFileDescriptor, int *connectedToClient)
{
	int i;
	
	char dataBuffer[DATA_BUFFER_SIZE];
	
	for(i=0; i<=STS_PIXEL_COUNT-1; i++)
	{
		sprintf(dataBuffer, "%d\t%5.0f\n", i, GetRawSampleSpectrumPixelValueSTS(i));
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}
	SendOK(clientFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Send a list of the formatted sample spectrum points, pixel number \t value \n
/***************************************************************************/
void SendFormattedSampleSpectrum(int clientFileDescriptor, int *connectedToClient)
{
	int i;
	
	char dataBuffer[DATA_BUFFER_SIZE];
	
	for(i=0; i<=STS_PIXEL_COUNT-1; i++)
	{
		sprintf(dataBuffer, "%d\t%5.0f\n", i, GetFormattedSampleSpectrumPixelValueSTS(i));
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}
	SendOK(clientFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Send a list of the raw absolute irradiance (uw_cm2_nm) spectrum points, 
//   pixel number \t value \n
/***************************************************************************/
void SendRawAbsoluteIrradianceSpectrum_uw_cm2_nm(int clientFileDescriptor, int *connectedToClient)
{
	int i;
	
	char dataBuffer[DATA_BUFFER_SIZE];
	
	for(i=0; i<=STS_PIXEL_COUNT-1; i++)
	{
		sprintf(dataBuffer, "%d\t%e\tuW/cm2/nm\n", i, GetRawAbsoluteIrradianceSpectrumPixelValueSTS_uW_cm2_nm(i));
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}
	SendOK(clientFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Send a list of the formatted absolute irradiance (uw_cm2_nm) spectrum points, 
//    pixel number \t value \n
/***************************************************************************/
void SendFormattedAbsoluteIrradianceSpectrum_uw_cm2_nm(int clientFileDescriptor, int *connectedToClient)
{
	int i;
	
	char dataBuffer[DATA_BUFFER_SIZE];
	
	for(i=0; i<=STS_PIXEL_COUNT-1; i++)
	{
		sprintf(dataBuffer, "%d\t%e\tuW/cm2/nm\n", i, GetFormattedAbsoluteIrradianceSpectrumPixelValueSTS_uW_cm2_nm(i));
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}
	SendOK(clientFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Send a list of the raw absolute irradiance (uw_cm2_nm) spectrum points, 
//   pixel number \t value \n
/***************************************************************************/
void SendRawAbsoluteIrradianceSpectrum_umolq_m2_s_nm(int clientFileDescriptor, int *connectedToClient)
{
	int i;
	
	char dataBuffer[DATA_BUFFER_SIZE];
	
	for(i=0; i<=STS_PIXEL_COUNT-1; i++)
	{
		sprintf(dataBuffer, "%d\t%e\tumol q/m2/nm\n", i, GetRawAbsoluteIrradianceSpectrumPixelValueSTS_umolq_m2_s_nm(i));
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}
	SendOK(clientFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Send a list of the formatted absolute irradiance (uw_cm2_nm) spectrum points, 
//    pixel number \t value \n
/***************************************************************************/
void SendFormattedAbsoluteIrradianceSpectrum_umolq_m2_s_nm(int clientFileDescriptor, int *connectedToClient)
{
	int i;
	
	char dataBuffer[DATA_BUFFER_SIZE];
	
	for(i=0; i<=STS_PIXEL_COUNT-1; i++)
	{
		sprintf(dataBuffer, "%d\t%e\tumol q/m2/nm\n", i, GetFormattedAbsoluteIrradianceSpectrumPixelValueSTS_umolq_m2_s_nm(i));
		SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
	}
	SendOK(clientFileDescriptor, connectedToClient);
}


/***************************************************************************/
// Send a list of the raw absolute waveband irradiance values in uW/cm2 for each waveband
//   the format is wavebandID <tab> lowWavelength <tab> highWavelength <tab> value <crlf> etc.
/***************************************************************************/
void SendRawAbsoluteIrradianceSpectralWaveband_uw_cm2(int clientFileDescriptor, int *connectedToClient)
{

	// get wavebands
	int theLastIndex, i;
	int notOK=FALSE;
	struct waveband *aWaveband;
	char dataBuffer[DATA_BUFFER_SIZE];
	
	theLastIndex=GetLastWavebandIndex();
	if (theLastIndex>=0)
	{		
		for(i=0; i<=theLastIndex;i++)
		{
			aWaveband=GetWavebandWithIndex(i);
			if(aWaveband!=NULL)
			{
				CalculateRawAbsoluteIrradianceForWaveband_uw_cm2(aWaveband);
				sprintf(dataBuffer, "%d\t%d\t%d\t%f\tuW/cm2\n", aWaveband->index, aWaveband->wavelengthLow,
						aWaveband->wavelengthHigh, aWaveband->value);
				SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
			}
			else
			{
				SendBadCommandFormat(clientFileDescriptor, connectedToClient);
				notOK=TRUE;
				break;
			}
		}
	}
	else
	{
		notOK=TRUE;
		SendMessage(clientFileDescriptor, connectedToClient, "ER - There were no wavebands.\n");
	}
	
	if(!notOK)
		SendOK(clientFileDescriptor, connectedToClient);

}

/***************************************************************************/
// Send a list of the raw absolute waveband irradiance values in umolq/m2/s for each waveband
//   the format is wavebandID <tab> lowWavelength <tab> highWavelength <tab> value <crlf> etc.
/***************************************************************************/
void SendRawAbsoluteIrradianceSpectralWaveband_umolq_m2_s(int clientFileDescriptor, int *connectedToClient)
{

	// get wavebands
	int theLastIndex, i;
	int notOK=FALSE;
	struct waveband *aWaveband;
	char dataBuffer[DATA_BUFFER_SIZE];
	
	theLastIndex=GetLastWavebandIndex();
	if (theLastIndex>=0)
	{		
		for(i=0; i<=theLastIndex;i++)
		{
			aWaveband=GetWavebandWithIndex(i);
			if(aWaveband!=NULL)
			{
				CalculateRawAbsoluteIrradianceForWaveband_umolq_m2_s(aWaveband);
				sprintf(dataBuffer, "%d\t%d\t%d\t%f\tumol q/m2/s\n", aWaveband->index, aWaveband->wavelengthLow,
					aWaveband->wavelengthHigh, aWaveband->value);
				SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
			}
			else
			{
				SendBadCommandFormat(clientFileDescriptor, connectedToClient);
				notOK=TRUE;
				break;
			}
		}
	}
	else
	{
		notOK=TRUE;
		SendMessage(clientFileDescriptor, connectedToClient, "ER - There were no wavebands.\n");
	}
	
	if(!notOK)
		SendOK(clientFileDescriptor, connectedToClient);
}


/***************************************************************************/
// Send a list of the formatted absolute waveband irradiance values in uW/cm2 for each waveband
//   the format is wavebandID <tab> lowWavelength <tab> highWavelength <tab> value <crlf> etc.
/***************************************************************************/
void SendFormattedAbsoluteIrradianceSpectralWaveband_uw_cm2(int clientFileDescriptor, int *connectedToClient)
{

	// get wavebands
	int theLastIndex, i;
	int notOK=FALSE;
	struct waveband *aWaveband;
	char dataBuffer[DATA_BUFFER_SIZE];
	
	theLastIndex=GetLastWavebandIndex();
	if (theLastIndex>=0)
	{		
		for(i=0; i<=theLastIndex;i++)
		{
			aWaveband=GetWavebandWithIndex(i);
			if(aWaveband!=NULL)
			{
				CalculateFormattedAbsoluteIrradianceForWaveband_uw_cm2(aWaveband);
				sprintf(dataBuffer, "%d\t%d\t%d\t%f\tuW/cm2\n", aWaveband->index, aWaveband->wavelengthLow,
						aWaveband->wavelengthHigh, aWaveband->value);
				SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
			}
			else
			{
				SendBadCommandFormat(clientFileDescriptor, connectedToClient);
				notOK=TRUE;
				break;
			}
		}
	}
	else
	{
		SendMessage(clientFileDescriptor, connectedToClient, "ER - There were no wavebands.\n");
		notOK=TRUE;
	}
	
	if(!notOK)
		SendOK(clientFileDescriptor, connectedToClient);
}


/***************************************************************************/
// Send a list of the formatted absolute waveband irradiance values in umolq/m2/s for each waveband
//   the format is wavebandID <tab> lowWavelength <tab> highWavelength <tab> value <crlf> etc.
/***************************************************************************/
void SendFormattedAbsoluteIrradianceSpectralWaveband_umolq_m2_s(int clientFileDescriptor, int *connectedToClient)
{

	// get wavebands
	int theLastIndex, i;
	int notOK=FALSE;
	struct waveband *aWaveband;
	char dataBuffer[DATA_BUFFER_SIZE];
	
	theLastIndex=GetLastWavebandIndex();
	if (theLastIndex>=0)
	{		
		for(i=0; i<=theLastIndex;i++)
		{
			aWaveband=GetWavebandWithIndex(i);
			if(aWaveband!=NULL)
			{
				CalculateFormattedAbsoluteIrradianceForWaveband_umolq_m2_s(aWaveband);
				sprintf(dataBuffer, "%d\t%d\t%d\t%f\tumol q/m2/s\n", aWaveband->index, aWaveband->wavelengthLow,
						aWaveband->wavelengthHigh, aWaveband->value);
				SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
			}
			else
			{
				SendBadCommandFormat(clientFileDescriptor, connectedToClient);
				notOK=TRUE;
				break;
			}
		}
	}
	else
	{
		SendMessage(clientFileDescriptor, connectedToClient, "ER - There were no wavebands.\n");
		notOK=TRUE;
	}
	
	if(!notOK)
		SendOK(clientFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Send a list of the raw absolute waveband irradiance values in 
//  uW/cm2 and umolq/m2/s for each waveband    the format is 
//  wavebandID <tab> lowWavelength <tab> highWavelength <tab> value <tab> value <crlf> etc.
/***************************************************************************/
void SendRawAbsoluteIrradianceSpectralWavebands(int clientFileDescriptor, int *connectedToClient)
{

	// get wavebands
	int theLastIndex, i;
	int notOK=FALSE;
	struct waveband *aWaveband;
	char dataBuffer[DATA_BUFFER_SIZE];
	double uw_cm2, umol_m2_s;
	
	theLastIndex=GetLastWavebandIndex();
	if (theLastIndex>=0)
	{		
		for(i=0; i<=theLastIndex;i++)
		{
			aWaveband=GetWavebandWithIndex(i);
			if(aWaveband!=NULL)
			{
				CalculateRawAbsoluteIrradianceForWaveband_uw_cm2(aWaveband);
				uw_cm2=aWaveband->value;
				CalculateRawAbsoluteIrradianceForWaveband_umolq_m2_s(aWaveband);
				umol_m2_s=aWaveband->value;
				
				sprintf(dataBuffer, "%d\t%d\t%d\t%f\tuW/cm2\t%f\tumol q/m2/s\n", aWaveband->index, aWaveband->wavelengthLow,
						aWaveband->wavelengthHigh, uw_cm2, umol_m2_s);
				SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
			}
			else
			{
				SendBadCommandFormat(clientFileDescriptor, connectedToClient);
				notOK=TRUE;
				break;
			}
		}
	}
	else
	{
		SendMessage(clientFileDescriptor, connectedToClient, "ER - There were no wavebands.\n");
		notOK=TRUE;
	}
	
	if(!notOK)
		SendOK(clientFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Send a list of the formatted absolute waveband irradiance values in 
//  uW/cm2 and umolq/m2/s for each waveband    the format is 
//  wavebandID <tab> lowWavelength <tab> highWavelength <tab> value <tab> value <crlf> etc.
/***************************************************************************/
void SendFormattedAbsoluteIrradianceSpectralWavebands(int clientFileDescriptor, int *connectedToClient)
{

	// get wavebands
	int theLastIndex, i;
	int notOK=FALSE;
	struct waveband *aWaveband;
	char dataBuffer[DATA_BUFFER_SIZE];
	double uw_cm2, umol_m2_s;
	
	theLastIndex=GetLastWavebandIndex();
	if (theLastIndex>=0)
	{		
		for(i=0; i<=theLastIndex;i++)
		{
			aWaveband=GetWavebandWithIndex(i);
			if(aWaveband!=NULL)
			{
				CalculateFormattedAbsoluteIrradianceForWaveband_uw_cm2(aWaveband);
				uw_cm2=aWaveband->value;
				CalculateFormattedAbsoluteIrradianceForWaveband_umolq_m2_s(aWaveband);
				umol_m2_s=aWaveband->value;
				
				sprintf(dataBuffer, "%d\t%d\t%d\t%f\tuW/cm2\t%f\tumol q/m2/s\n", aWaveband->index, aWaveband->wavelengthLow,
						aWaveband->wavelengthHigh, uw_cm2, umol_m2_s);
				SendMessage(clientFileDescriptor, connectedToClient, dataBuffer);
			}
			else
			{
				SendBadCommandFormat(clientFileDescriptor, connectedToClient);
				notOK=TRUE;
				break;
			}
		}
	}
	else
	{
		SendMessage(clientFileDescriptor, connectedToClient, "ER - There were no wavebands.\n");
		notOK=TRUE;
	}
	
	if(!notOK)
		SendOK(clientFileDescriptor, connectedToClient);
}


/***************************************************************************/
// Parse the message from the client and act accordingly
/***************************************************************************/

void parseClientMessage(char *command, int tcpFileDescriptor, 
						volatile int *doQuit, int *connectedToClient)
{	
	int stsReturnValue=0, lowNumber=0, highNumber=0;
	char c=0; // commandCharacter
	
	if(command!=NULL)
	{
	// See SendCommandList for the commands implemented
		if(sscanf(command, "%c %d %d", &c, &lowNumber, &highNumber)>0)
		{			
			switch (c)
			{
				case '?':
				case 'h':
					SendCommandList(tcpFileDescriptor, connectedToClient);
					break;
				case 'x':
					SendMessage(tcpFileDescriptor, connectedToClient, "Restarting heliospectraSTSdeamon...\n");
					SendOK(tcpFileDescriptor, connectedToClient);
					*doQuit=1;
					break;
				case 's':
					switch (lowNumber)
					{
						case 0:
							SendRawDarkReferenceSpectrum(tcpFileDescriptor, connectedToClient);
							break;
						case 1:
							SendRawSampleSpectrum(tcpFileDescriptor, connectedToClient);
							break;
						case 2:
							SendFormattedDarkReferenceSpectrum(tcpFileDescriptor, connectedToClient);
							break;
						case 3:
							SendFormattedSampleSpectrum(tcpFileDescriptor, connectedToClient);
							break;
						case 4:
							SendRawAbsoluteIrradianceSpectrum_uw_cm2_nm(tcpFileDescriptor, connectedToClient);
							break;
						case 5:
							SendRawAbsoluteIrradianceSpectrum_umolq_m2_s_nm(tcpFileDescriptor, connectedToClient);
							break;	
						case 6:
							SendFormattedAbsoluteIrradianceSpectrum_uw_cm2_nm(tcpFileDescriptor, connectedToClient);
							break;
						case 7:
							SendFormattedAbsoluteIrradianceSpectrum_umolq_m2_s_nm(tcpFileDescriptor, connectedToClient);
							break;
						case 8:
							SendWavelengthList(tcpFileDescriptor, connectedToClient);
							break;
						case 9:
							SendCalibrationTable(tcpFileDescriptor, connectedToClient);
							break;												
						default:
							SendBadCommandFormat(tcpFileDescriptor, connectedToClient);
							break;
					}
					break;
				case 'i':
					// 0 sets normal trigger
					if ((stsReturnValue=SetSampleParametersSTS(0, lowNumber*1000))==noError)
						SendOK(tcpFileDescriptor, connectedToClient);
					else
						SendMessageErrorSTS(tcpFileDescriptor, connectedToClient, stsReturnValue);
					break;
				case 'I':
					// 0 sets normal trigger
					if ((stsReturnValue=SetSampleParametersSTS(0, lowNumber))==noError)
						SendOK(tcpFileDescriptor, connectedToClient);
					else
						SendMessageErrorSTS(tcpFileDescriptor, connectedToClient, stsReturnValue);
					break;
				case 'b':
					// 0 sets boxcar filter width and scans to average
					if ((stsReturnValue=SetSpectrumProcessingSTS(lowNumber, highNumber))==noError)
						SendOK(tcpFileDescriptor, connectedToClient);
					else
						SendMessageErrorSTS(tcpFileDescriptor, connectedToClient, stsReturnValue);
					break;
				case 'a':
					if ((stsReturnValue=AddWaveband(lowNumber, highNumber))>=noError)
						SendOK(tcpFileDescriptor, connectedToClient);
					else
						SendBadCommandFormat(tcpFileDescriptor, connectedToClient);
					break;
				case 'r':
					if ((stsReturnValue=RemoveWavebandWithIndex(lowNumber))>=noError)
						SendOK(tcpFileDescriptor, connectedToClient);
					else
						SendBadCommandFormat(tcpFileDescriptor, connectedToClient);
					break;
				case 'l':
					SendWavebandList(tcpFileDescriptor, connectedToClient);
					break;
				case 'd':
					SendDetectorTemperature(tcpFileDescriptor, connectedToClient);
					break;
				case 't':
					switch (lowNumber)
					{
						case 0:
							if ((stsReturnValue=TakeRawDarkSpectrumSTS())==noError)
								SendOK(tcpFileDescriptor, connectedToClient);
							else
								SendMessageErrorSTS(tcpFileDescriptor, connectedToClient, stsReturnValue);
							break;
						case 1:
							if ((stsReturnValue=TakeRawSampleSpectrumSTS())==noError)
								SendOK(tcpFileDescriptor, connectedToClient);
							else
								SendMessageErrorSTS(tcpFileDescriptor, connectedToClient, stsReturnValue);
							break;
						case 2:
							if ((stsReturnValue=TakeFormattedDarkSpectrumSTS())==noError)
								SendOK(tcpFileDescriptor, connectedToClient);
							else
								SendMessageErrorSTS(tcpFileDescriptor, connectedToClient, stsReturnValue);
							break;
						case 3:
							if ((stsReturnValue=TakeFormattedSampleSpectrumSTS())==noError)
								SendOK(tcpFileDescriptor, connectedToClient);
							else
								SendMessageErrorSTS(tcpFileDescriptor, connectedToClient, stsReturnValue);
							break;
						default:
							SendBadCommandFormat(tcpFileDescriptor, connectedToClient);
							break;
					}
					break;
				case 'w':
					switch (lowNumber)
					{
						case 0:
							SendRawAbsoluteIrradianceSpectralWaveband_uw_cm2(tcpFileDescriptor, connectedToClient);
							break;
						case 1:
							SendRawAbsoluteIrradianceSpectralWaveband_umolq_m2_s(tcpFileDescriptor, connectedToClient);
							break;
						case 2:
							SendFormattedAbsoluteIrradianceSpectralWaveband_uw_cm2(tcpFileDescriptor, connectedToClient);
							break;
						case 3:
							SendFormattedAbsoluteIrradianceSpectralWaveband_umolq_m2_s(tcpFileDescriptor, connectedToClient);
							break;
						case 4:
							SendRawAbsoluteIrradianceSpectralWavebands(tcpFileDescriptor, connectedToClient);
							break;
						case 5:
							SendFormattedAbsoluteIrradianceSpectralWavebands(tcpFileDescriptor, connectedToClient);
							break;							
						default:
							SendBadCommandFormat(tcpFileDescriptor, connectedToClient);
							break;
					}
					break;
				case 'g':
					switch (lowNumber)
					{
						case 0:
							SendSerialNumber(tcpFileDescriptor, connectedToClient);
							break;
						case 1:
							SendOpticalBenchInfo(tcpFileDescriptor, connectedToClient);
							break;
						case 2:
							SendTemperatureCPU(tcpFileDescriptor, connectedToClient);
							break;
						case 3:
							SendBoxcarWidth(tcpFileDescriptor, connectedToClient);
							break;
						case 4:
							SendScansToAverage(tcpFileDescriptor, connectedToClient);
							break;
						case 5:
							SendIntegrationTime_ms(tcpFileDescriptor, connectedToClient);
							break;
						case 6:
							SendIntegrationTime_us(tcpFileDescriptor, connectedToClient);
							break;
						default:
							SendBadCommandFormat(tcpFileDescriptor, connectedToClient);
							break;
					}
					break;
				case 'q':
					SendMessage(tcpFileDescriptor, connectedToClient, "Close connection...\n");
					*connectedToClient=FALSE;
					SendOK(tcpFileDescriptor, connectedToClient);
					break;
				default:
					SendUnknownCommand(tcpFileDescriptor, connectedToClient);
					break;
			}
		}
	}
	else
		SendUnknownCommand(tcpFileDescriptor, connectedToClient);
}

/***************************************************************************/
// Function for the event loop
/***************************************************************************/

int EventLoop(volatile int *exitFlag, int afileDescriptorForTCP)
{
	int returnValue=0;
	int serverFileDescriptor, pollValue;
	struct pollfd fileDescriptorSet;
	int connectionOpen=TRUE;
	struct sockaddr clientAddress;
	socklen_t clientAddressLength;
	char clientMessage[MAX_MESSAGE_LENGTH];
	
	/***************************************************************************/
    // This is the event loop. It provides a single TCP port for access
    //  to the STS spectrometer via a network
    /***************************************************************************/
	while (!(*exitFlag))
	{
		clientAddressLength=sizeof(clientAddress);
		if ((serverFileDescriptor=accept(afileDescriptorForTCP, &clientAddress,
				&clientAddressLength)) >= 0)
		{
			// receive data from connection
			connectionOpen=TRUE;
			SendMessage(serverFileDescriptor, (int *)exitFlag, "HeliospectraSTSdaemon v1.0\nEnter ? for command list.\n");
			while ((!(*exitFlag)) && connectionOpen)
			{
				fileDescriptorSet.fd=serverFileDescriptor;
				fileDescriptorSet.events=POLLIN;  // poll is triggered by input
				fileDescriptorSet.revents=NOFLAGS;
				
				// wait for data, one file descriptor and no timeout
				pollValue=poll(&fileDescriptorSet, 1, POLL_WAIT_FOREVER);
				if (pollValue>0)
				{
					// if there are no data in the message the connection has been closed
					if(read(serverFileDescriptor, clientMessage,sizeof(clientMessage))>0)
					{
						parseClientMessage(clientMessage, serverFileDescriptor, 
											exitFlag, &connectionOpen);
					}
					else
						connectionOpen=FALSE;
				}
				else
				{
					perror("poll() error");
					*exitFlag=1;
				}
			}
		}
			else
				perror("Could not accept new socket connection");
			
			close(serverFileDescriptor);
	}
	return(returnValue);
}
