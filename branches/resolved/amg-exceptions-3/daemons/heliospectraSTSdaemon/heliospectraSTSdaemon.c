
/*******************************************************
 * File:    heliospectraSTSdaemon.c
 * Date:    January 2015
 * Author:  Heliospectra AB, Kirk C.
 *
 * This daemon provides an interface for the Ocean Optics STS spectrometer
 * to a Raspberry Pi. Interface is provided by a single TCP socket on default port 54545
 *
 * The deamon only supports one STS spectrometer connected to the Raspberry Pi. 
 * 
 * For command line arguments, see showUsage().
 *
 * c. 2015 Heliospectra AB
 *
 *******************************************************/

#include <stdio.h>		// printf() etc
#include <stdlib.h>		// for atoi(), free(), exit() etc.
#include <stdarg.h>		// for strcpy()
#include <unistd.h>  	// for getopt()
#include <string.h>		// for strcpy()
#include <errno.h>		// use proper error codes
#include <sys/select.h> // for select()

#include <ctype.h>
#include <sys/time.h>
#include <fcntl.h>
#include <math.h>		// lround()

#include <sys/types.h>
#include <sys/socket.h>	// define the socket parameters
#include <netinet/in.h>	// define internet structures
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>
#include <pwd.h>
#include <arpa/nameser.h>
#include <resolv.h>

#include <signal.h>
#include "api/seabreezeapi/SeaBreezeAPI.h"

#include "heliospectraSTSdaemon_eventLoop.h"
#include "heliospectraSTSdaemon_support.h"

/****************************************************************************************/
// Usage statement for bad command line arguments
/****************************************************************************************/

void showUsage(void)
{
    puts("Usage: heliospectraSTSdaemon [-x variable 1] [-y variable 2] ... [-z variable n]\n"
         "heliospectraSTSdaemon \n"
         "\t-? (help)\n"
         "\t-p port for tcp socket (54545 default)\n"
         );
}


/****************************************************************************************/
//	prints an error message to standard error. errorNumber should come from errno.h 
//	defaultErrorMessage identifies the source of the error. 
/****************************************************************************************/
void exitError(short errorNumber, const char *errorMessage)
{
	int returnValue=noError;
	
	const char* defaultErrorMessage = "stsInterfaceDaemon Stopped. ERROR - ";
    const size_t errorMessageLength     = strlen(errorMessage);
    const size_t defaultErrorMessageLength = strlen(defaultErrorMessage);
    const size_t totalLength = errorMessageLength + defaultErrorMessageLength;

    char *const strBuf = malloc(totalLength + 1);
    if (strBuf != NULL) 
    {
		strcpy(strBuf, defaultErrorMessage);
    	strcpy(strBuf + defaultErrorMessageLength, errorMessage);

		errno=errorNumber;
		perror(strBuf);
		free(strBuf);
		
    	returnValue=errorNumber; 
    }
    else
    {
        perror("heliospectraSTSdaemon ERROR - errorExit function failed due to a bad malloc.");
        returnValue=ENOMEM;
    }
  exit(returnValue);
}

/****************************************************************************************/
// sigaction handler to be able to catch system signals and shut down properly
/****************************************************************************************/
static volatile int  exit_flag = 0;						// a pointer is passed to the loop

static void signalActionHandler (int systemSignal)
{ 	
		printf("\nheliospectraSTSdaemon - Terminate signal received: %d\n", systemSignal);
		fflush(stdout);
		exit_flag = 1;
}

// signals that the daemon will recognize as an order to shutdown
int InstallSignalActions (void)
{
	int returnValue=-1;  // Bad return value if the signal action installations fail
	
	// structure for the os signal actions the process will recognize
	struct sigaction osSignalAction;    
	
	// clear the structure and set up a handler for system signals. This allows the
	//  daemon to recognize kills, stops, sighups etc.
	memset (&osSignalAction, 0, sizeof(osSignalAction));
	osSignalAction.sa_handler = &signalActionHandler;
	
	// all of these signals should terminate the process
	if(sigaction(SIGHUP, &osSignalAction, NULL) >= noError)
		if(sigaction(SIGINT, &osSignalAction, NULL) >= noError)
			if(sigaction(SIGTERM, &osSignalAction, NULL) >= noError)
				returnValue=success;
	return(returnValue);
}

void CloseSocketProclaimExit(int tcpFileDescriptor)
{
	close(tcpFileDescriptor);
	printf("Exit Heliospectra STS Daemon.\n\n");
	fflush(stdout);
}

/****************************************************************************************/
// c main, but the event loop is in another file
/****************************************************************************************/
int main(int argc, char **argv) {
    
    //
    // getopt() strings
    //
    char *tcpPortNumberString = NULL;		// variable for the -p argument  
    char *tcpPortNumber = "54545";  // getaddrinfo() uses a string for service
    
    //
    // file descriptors and structures for UDP
    //
	int fileDescriptorForTCP;		
	struct addrinfo networkHints, *tcpSocketInfoPointer, *addrInfoPtr; 
	
	// generic integer for status returned by function calls
	int returnValue; 				

  	/***************************************************************************/
	// set up a handler for catching signals from the operating system
	/***************************************************************************/
	if (InstallSignalActions() == noError)
	{
		/***************************************************************************/
		// process the command line arguments
		/***************************************************************************/
		int getoptResult = 0;
		
		opterr = 0;   			// defined in unistd.h for getopt
		
		// an argument followed by :: is not required, but read
		while ((getoptResult = getopt(argc, argv, "?p:")) != -1)  
		{
			switch (getoptResult) 
			{
				case '?':
					showUsage();
					exit(0);
					break;
				case 'p':
					tcpPortNumberString = optarg; 
					printf("port= %s, %s\n", tcpPortNumberString, optarg);
					break;
				default : 
					showUsage();
					exit(EINVAL);
			}
		}
	
		if (opterr==noError)
		{
			// override default values if user specified
			if (tcpPortNumberString!=NULL)
				tcpPortNumber=tcpPortNumberString;

			
			/***************************************************************************/
			// If the command line arguments were valid, set up a single TCP socket 
			/***************************************************************************/
			
			// status for the command line
			printf("\nStart Heliospectra STS Daemon:\n Port Number = %s\n", tcpPortNumber);
		
			// clear out an address info structure
			memset(&networkHints, 0, sizeof(networkHints));
			// set up the address info structure for IPV4 and TCP
			networkHints.ai_family = AF_INET;	// IPV4
			networkHints.ai_socktype = SOCK_STREAM;  // TCP
			networkHints.ai_flags = AI_PASSIVE|AI_ADDRCONFIG;     // fill in my IP for me
			networkHints.ai_protocol=IPPROTO_TCP;
			
			// Get a linked list of address info structures
			if ((returnValue = getaddrinfo(NULL, tcpPortNumber, &networkHints, &tcpSocketInfoPointer)) == noError)
			{
				// loop through the possible available addresses. Take the first available
				for(addrInfoPtr = tcpSocketInfoPointer; addrInfoPtr!= NULL;
						 addrInfoPtr=addrInfoPtr->ai_next)
				{
					// create a TCP socket
					if ((fileDescriptorForTCP = socket(addrInfoPtr->ai_family,
							 addrInfoPtr->ai_socktype, addrInfoPtr->ai_protocol)) >= 0)
						break;
					else
					{
						// Don't exit, keep looking, but do indicate the lookup error
						perror("heliospectraSTSdaemon socket");  // perror also gives a description from errno
					}
				}
			
				// if the address information pointer is still NULL, there was no socket with which to bind
				if (addrInfoPtr != NULL)
				{
					int opt=1;      // option is to be 1 for TRUE or 0 for FALSE 

					// Be able to reuse the socket in case the application 
					//  closes poorly or there is a crash
  					if (setsockopt(fileDescriptorForTCP, SOL_SOCKET, SO_REUSEADDR, 
  								(char *)&opt,sizeof(opt))==noError)
  					{
  						// bind to the socket
  						if (bind(fileDescriptorForTCP, addrInfoPtr->ai_addr, 
  							addrInfoPtr->ai_addrlen)==noError)
  						{
  							freeaddrinfo(tcpSocketInfoPointer);
							// listen for a connection, take only one
							if (listen(fileDescriptorForTCP, 3) == noError) 
							{
				/***************************************************************************/
				// Connect to a spectrometer and start the TCP server
				/***************************************************************************/	
								printf("Listening...\n");
								
								if (OpenSTS()==noError)
								{
									double myTemperature;
									char buffer[DATA_BUFFER_SIZE];
									
									// give a bit of data
									printf("\tSTS SerialNumber = %s\n", GetSerialNumber_STS());
									
									if (GetOpticalBenchDetails_STS(buffer, DATA_BUFFER_SIZE-1)==noError)
										printf("\tSTS Optical Bench Details - %s\n", buffer);
									else
										printf("\tSTS Optical Bench Details could not be retrieved.\n");
										
									if (GetTemperature_STS(&myTemperature, STS_TEMPERATURE_DETECTOR)==noError)
										printf("\tDetector temperature=%4.1f\n", myTemperature);
									else
										printf("Failed to get detector temperature\n");
		
									if (GetTemperature_STS(&myTemperature, STS_TEMPERATURE_CPU)==noError)
										printf("\tCPU temperature=%4.1f\n", myTemperature);
									else
										printf("Failed to get CPU temperature\n");
									
									// the default waveband is for the full sts bandwidth
									AddWaveband(ceil(GetWavelengthPixelValueSTS(0)), 
										floor(GetWavelengthPixelValueSTS(STS_PIXEL_COUNT-1)));
									
									// set a default integration time
									SetSampleParametersSTS(0, 1000);																	
									              
									// run the event loop
									returnValue=EventLoop(&exit_flag, fileDescriptorForTCP); 
									
									// close the spectrometer and clean up
									CloseSTS(); // ignore return value...
									
								 	// free memory and close the UDP port
									CloseSocketProclaimExit(fileDescriptorForTCP);
									return(returnValue);
								}
								else
								{
									CloseSocketProclaimExit(fileDescriptorForTCP);
									exitError(ENXIO, "Could not open the STS spectrometer");
								}
							}
							else
							{
								CloseSocketProclaimExit(fileDescriptorForTCP);
								exitError(ENETUNREACH, "Could not listen to the socket");
							}
						}
						else
						{
							freeaddrinfo(tcpSocketInfoPointer);
							CloseSocketProclaimExit(fileDescriptorForTCP);		
							exitError(ENETUNREACH, "Could not bind to the socket");
						}
					}
					else
					{
						freeaddrinfo(tcpSocketInfoPointer);
						exitError(ENETUNREACH, "Could not set the TCP socket for reuse");
					}
				}
				else
				{
					freeaddrinfo(tcpSocketInfoPointer);
					exitError(ENETUNREACH, "Could not open a TCP socket");
				}
			}
			else
			{
				char strBuffer[100]; // all of the messages from gai_strerror() are less than 50 characters.
				// get a description of the failure
				sprintf(strBuffer, "Failed getaddrinfo() call: %s\n", gai_strerror(returnValue)); 
				exitError(ENETUNREACH, strBuffer);
			}
		}
		else
		{
			showUsage();
			exit(EINVAL);
		}  
	}
	else
		exitError(-1, "Bad signal action"); // not sure of an error code for this
}
