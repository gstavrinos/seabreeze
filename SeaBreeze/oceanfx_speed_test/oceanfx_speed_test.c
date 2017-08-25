/*******************************************************
 * File:    oceanfx_speed_test.c
 * Date:    August 2017
 * Author:  Ocean Optics, Inc.
 *
 * This is a test program to exercise some of the
 * SeaBreeze functionality.  If using Windows, please
 * #define _WINDOWS (directly or as a project setting)
 * so that this will compile.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2017, Ocean Optics Inc
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

/* Includes */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "api/seabreezeapi/SeaBreezeAPI.h"
#include <arpa/inet.h>
// #include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
// #include "api/seabreezeapi/SeaBreezeAPIConstants.h"


#ifndef _WINDOWS
#include <unistd.h>
#else
#include <Windows.h>
#endif

#define SPECTRA_PER_TRIGGER 50000
#define NUMBER_SPECTRA_TO_RETRIEVE 15 // current max available from the OceanFX
#define FAST_BUFFER_ENABLED 1
#define DISPLAY_PERIOD 5
#define OCEANFX_TRIGGER_MODE 0x00
#define TEST_TIME 60
#define LOOP_DELAY 0
#define BUFFER_CAPACITY 50000
//
// show the usage message
//
void display_usage(char *application_name)
{
    fprintf(stderr, "\nUsage:    %s [<IPv4 dotted-address> | ? | help] \n\n"
            "Runs a spectrum retrieval speed test on an Ocean FX.\n "
            "-  An optional IP address is provided for spectrometers using TCP/IP.\n "
            "- \"?\" and \"help\" invoke this message. \nNote that if more than one spectrometer "
            "is found, only the first is used.\n\n", application_name);
    
}


// 
// get the serial number to identify the spectrometer
//
int get_serial_number(long deviceID, char *serial_number, unsigned char length)
{
	int number_of_serial_number_features = 0;
    long serial_number_feature_id = 0;
    int error = -1;

    number_of_serial_number_features = sbapi_get_number_of_serial_number_features(deviceID, &error);
    if(error == 0)
    {
		if(number_of_serial_number_features == 1) // there is only one serial number feature in the ocean fx
		{
			number_of_serial_number_features = sbapi_get_serial_number_features(deviceID, &error, &serial_number_feature_id, number_of_serial_number_features);
			if(error == 0)
			{
				// retrieve the serial number
				sbapi_get_serial_number(deviceID, serial_number_feature_id, &error, serial_number, length);
				if(error != 0)
				{
					printf("Error getting serial number.\n");
				}
			}
			else
			{
				printf("Error retrieving the serial number feature ID.\n");
			}
		}
		else
		{
    		printf("Serial number feature count incorrect. n=%d\n", number_of_serial_number_features);		
		}   
    }
    else
    {
    	printf("Could not retrieve the number of serial number features.\n");
    }
	return error;  
}


//
// set up the spectrometer feature of the Ocean FX in SeaBreeze
//
int configure_spectrometer_feature(long deviceID, long *spectrometer_feature_id_ptr)
{
	int number_of_spectrometer_features = 0;
    int error = -1;

    number_of_spectrometer_features = sbapi_get_number_of_spectrometer_features(deviceID, &error);
    if(error == 0)
    {
		if(number_of_spectrometer_features == 1) // there is only one spectrometer feature in the ocean fx
		{
			number_of_spectrometer_features = sbapi_get_spectrometer_features(deviceID, &error, spectrometer_feature_id_ptr, number_of_spectrometer_features);
			if(error == 0)
			{
				// set integration time to 10us
				sbapi_spectrometer_set_integration_time_micros(deviceID, *spectrometer_feature_id_ptr, &error, 10);
				if(error != 0)
				{
					printf("Error setting integration time.\n");
				}
			}
			else
			{
				printf("Error retrieving the spectrometer feature ID.\n");
			}
		}
		else
		{
    		printf("Spectrometer feature count incorrect. n=%d\n", number_of_spectrometer_features);		
		}   
    }
    else
    {
    	printf("Could not retrieve the number of spectrometer features.\n");
    }
	return error;  
}

//
// set up the data buffer feature for the ocean fx
//
int configure_data_buffer_feature(long deviceID, long *data_buffer_feature_id_ptr)
{
	int number_of_data_buffer_features = 0;
    int error = -1;

    number_of_data_buffer_features = sbapi_get_number_of_data_buffer_features(deviceID, &error);
    if(error == 0)
    {
		if(number_of_data_buffer_features == 1) // there is only one data buffer feature in the ocean fx
		{
			number_of_data_buffer_features = sbapi_get_data_buffer_features(deviceID, &error, data_buffer_feature_id_ptr, (unsigned int)number_of_data_buffer_features);
			if(error == 0)
			{
				// we know the capacity of the buffer, so set it to SPECTRA_PER_TRIGGER and clear it
			    sbapi_data_buffer_set_buffer_capacity(deviceID, *data_buffer_feature_id_ptr, &error, BUFFER_CAPACITY);
			    if(error == 0)
			    {
			    	// clear the data buffer
			    	sbapi_data_buffer_clear(deviceID, *data_buffer_feature_id_ptr, &error);
			    	if(error != 0)
			    	{
			    		printf("Error clearing the buffer.\n");
			    	}
			    }
			    else
			    {
			    	printf("Error setting the buffer capacity.\n");
			    }
			}
			else
			{
				printf("Error retrieving the data buffer feature ID.\n");
			}
		}
		else
		{
    		printf("Data buffer feature count incorrect. n=%d\n", number_of_data_buffer_features);		
		}   
    }
    else
    {
    	printf("Could not retrieve the number of data buffer features.\n");
    }
	return error;
}

//
// set up the fast buffer feature for the ocean fx
//
int configure_fast_buffer_feature(long deviceID)
{
	int number_of_fast_buffer_features = 0;
    long fast_buffer_feature_id = 0;
    int error = -1;

    number_of_fast_buffer_features = sbapi_get_number_of_fast_buffer_features(deviceID, &error);
    if(error == 0)
    {
		if(number_of_fast_buffer_features == 1) // there is only one fast buffer feature in the ocean fx
		{
			number_of_fast_buffer_features = sbapi_get_fast_buffer_features(deviceID, &error, &fast_buffer_feature_id, (unsigned int)number_of_fast_buffer_features);
			if(error == 0)
			{
				// we know the capacity of the buffer, so set it to SPECTRA_PER_TRIGGER
			    sbapi_fast_buffer_set_consecutive_sample_count(deviceID, fast_buffer_feature_id, &error, SPECTRA_PER_TRIGGER);
			    if(error == 0)
			    {
			    	// enable the fast buffer
			    	sbapi_fast_buffer_set_buffering_enable(deviceID, fast_buffer_feature_id, &error, FAST_BUFFER_ENABLED);
			    	if(error != 0)
			    	{
			    		printf("Error enabling the buffer.\n");
			    	}
			    }
			    else
			    {
			    	printf("Error setting the consecutive sample number.\n");
			    }
			}
			else
			{
				printf("Error retrieving the fast buffer feature ID.\n");
			}
		}
		else
		{
    		printf("Fast buffer feature count incorrect. n=%d\n", number_of_fast_buffer_features);		
		}   
    }
    else
    {
    	printf("Could not retrieve the number of fast buffer features.\n");
    }
	return error;
}

//
// return the difference in time, in seconds
double elapsed_time(struct timespec start, struct timespec end)
{
	struct timespec aTime;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		aTime.tv_sec = end.tv_sec-start.tv_sec-1;
		aTime.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		aTime.tv_sec = end.tv_sec-start.tv_sec;
		aTime.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return aTime.tv_sec + (aTime.tv_nsec/1000000000.0);
}

//
// keep the ocean fx buffer full and retrieve as many spectra as possible per second. The top scan rate for the 
//  FPGA and detector is about 4500 spectra per second


void ocean_fx_standard_speed_test(long deviceID, long data_buffer_feature_id, long spectrometer_feature_id)
{   
	int error = -1;
    static struct ocean_fx_raw_spectrum_with_meta_data
    {
    	uint8_t meta_data[64];
    	uint16_t pixel_values[2136];
    	uint32_t checksum;
    } spectral_data[NUMBER_SPECTRA_TO_RETRIEVE];

    int bytesReturned = 0;
    int spectra_retrieved = 0;
	struct timespec start_time, timing_mark;
    int loopCount = 0;

	// clear the spectrum buffer
	sbapi_data_buffer_clear(deviceID, data_buffer_feature_id, &error);
    sbapi_spectrometer_set_trigger_mode(deviceID, spectrometer_feature_id, &error, OCEANFX_TRIGGER_MODE);

    printf("\n\n\n[Standard speed test]\nStarting the clock. Collecting spectra for about %d seconds.\n\n", TEST_TIME);
	if(clock_gettime(CLOCK_REALTIME, &start_time) == 0)
	{
		if(clock_gettime(CLOCK_REALTIME, &timing_mark) == 0)
		{
			int nextDisplay = DISPLAY_PERIOD;

            while (elapsed_time(start_time, timing_mark) < TEST_TIME)
			{
                if(LOOP_DELAY>0)
                    usleep(LOOP_DELAY);

				if (clock_gettime(CLOCK_REALTIME, &timing_mark) != 0)
				{
					printf("clock_gettime() error\n");
					break;
				}

                if(elapsed_time(start_time, timing_mark) > nextDisplay)
                {
                    printf("\nElapsed time = %f seconds\nAverage loop time = %f seconds\nCurrent spectra per second = %f\nSpectra retrieved = %d\nAverage retrieved per loop = %f\n\n",
                           elapsed_time(start_time, timing_mark),
                           elapsed_time(start_time, timing_mark)/loopCount,
                           spectra_retrieved / elapsed_time(start_time, timing_mark), spectra_retrieved, (double)spectra_retrieved/loopCount);
					nextDisplay += DISPLAY_PERIOD;
                }

				bytesReturned = sbapi_spectrometer_get_fast_buffer_spectrum(deviceID, spectrometer_feature_id, &error, (unsigned char *)spectral_data, sizeof(spectral_data), NUMBER_SPECTRA_TO_RETRIEVE);
				spectra_retrieved += (bytesReturned / (sizeof(spectral_data[0])));
                loopCount++;
			}

			if (clock_gettime(CLOCK_REALTIME, &timing_mark) == 0)
			{
				printf("Stopping the clock.\n");
				printf("Elapsed time = %f seconds\n", elapsed_time(start_time, timing_mark));
                printf("Average loop time = %f seconds\n", elapsed_time(start_time, timing_mark)/loopCount);
				printf("Spectra retrieved = %d\n", spectra_retrieved);
				printf("Spectra per second = %f\n", spectra_retrieved/elapsed_time(start_time, timing_mark));
                printf("Average retrieved per loop = %f\n\n", (double)spectra_retrieved/loopCount);
			}
			else
			{
				printf("clock_gettime() error\n");
			}

            // new sbapi_spectrometer_abort_spectral_acquisition() command should go here...
            sbapi_data_buffer_clear(deviceID, data_buffer_feature_id, &error);
            //sbapi_spectrometer_set_trigger_mode(deviceID, spectrometer_feature_id, &error, 0x00); // software trigger mode
		}
	}
	else
	{
		printf("clock_gettime() error\n");
	}

}


void ocean_fx_split_speed_test(long deviceID, long data_buffer_feature_id, long spectrometer_feature_id)
{
    int error = -1;
    static struct ocean_fx_raw_spectrum_with_meta_data
    {
        uint8_t meta_data[64];
        uint16_t pixel_values[2136];
        uint32_t checksum;
    } spectral_data[NUMBER_SPECTRA_TO_RETRIEVE];

    int bytesReturned = 0;
    int spectra_retrieved = 0;
    struct timespec start_time, timing_mark;
    int loopCount = 0;

    // clear the spectrum buffer
    sbapi_data_buffer_clear(deviceID, data_buffer_feature_id, &error);
    sbapi_spectrometer_set_trigger_mode(deviceID, spectrometer_feature_id, &error, OCEANFX_TRIGGER_MODE);

    printf("\n\n\n[Overlapping Request Speed Test]\nStarting the clock. Collecting spectra for about %d seconds.\n\n", TEST_TIME);
    if(clock_gettime(CLOCK_REALTIME, &start_time) == 0)
    {
        if(clock_gettime(CLOCK_REALTIME, &timing_mark) == 0)
        {
            int nextDisplay = DISPLAY_PERIOD;

            // start the buffered acquisition. The request must be balanced by a response
            sbapi_spectrometer_fast_buffer_spectrum_request(deviceID, spectrometer_feature_id, &error, NUMBER_SPECTRA_TO_RETRIEVE);
            while (elapsed_time(start_time, timing_mark) < TEST_TIME)
            {
                if(LOOP_DELAY>0)
                    usleep(LOOP_DELAY);

                sbapi_spectrometer_fast_buffer_spectrum_request(deviceID, spectrometer_feature_id, &error, NUMBER_SPECTRA_TO_RETRIEVE);

                if (clock_gettime(CLOCK_REALTIME, &timing_mark) != 0)
                {
                    printf("clock_gettime() error\n");
                    break;
                }

                if(elapsed_time(start_time, timing_mark) > nextDisplay)
                {
                    printf("\nElapsed time = %f seconds\nAverage loop time = %f seconds\nCurrent spectra per second = %f\nSpectra retrieved = %d\nAverage retrieved per loop = %f\n\n",
                           elapsed_time(start_time, timing_mark),
                           elapsed_time(start_time, timing_mark)/loopCount,
                           spectra_retrieved / elapsed_time(start_time, timing_mark), spectra_retrieved, (double)spectra_retrieved/loopCount);
                    nextDisplay += DISPLAY_PERIOD;
                }

                //bytesReturned = sbapi_spectrometer_get_fast_buffer_spectrum(deviceID, spectrometer_feature_id, &error, (unsigned char *)spectral_data, sizeof(spectral_data), NUMBER_SPECTRA_TO_RETRIEVE);
                bytesReturned = sbapi_spectrometer_fast_buffer_spectrum_response(deviceID, spectrometer_feature_id, &error, (unsigned char *)spectral_data, sizeof(spectral_data), NUMBER_SPECTRA_TO_RETRIEVE);

                spectra_retrieved += (bytesReturned / (sizeof(spectral_data[0])));
                loopCount++;
            }
            bytesReturned = sbapi_spectrometer_fast_buffer_spectrum_response(deviceID, spectrometer_feature_id, &error, (unsigned char *)spectral_data, sizeof(spectral_data), NUMBER_SPECTRA_TO_RETRIEVE);

            if (clock_gettime(CLOCK_REALTIME, &timing_mark) == 0)
            {
                printf("Stopping the clock.\n");
                printf("Elapsed time = %f seconds\n", elapsed_time(start_time, timing_mark));
                printf("Average loop time = %f seconds\n", elapsed_time(start_time, timing_mark)/loopCount);
                printf("Spectra retrieved = %d\n", spectra_retrieved);
                printf("Spectra per second = %f\n", spectra_retrieved/elapsed_time(start_time, timing_mark));
                printf("Average retrieved per loop = %f\n\n", (double)spectra_retrieved/loopCount);
            }
            else
            {
                printf("clock_gettime() error\n");
            }

            // new sbapi_spectrometer_abort_spectral_acquisition() command should go here...
            sbapi_data_buffer_clear(deviceID, data_buffer_feature_id, &error);
            //sbapi_spectrometer_set_trigger_mode(deviceID, spectrometer_feature_id, &error, 0x00);
        }
    }
    else
    {
        printf("clock_gettime() error\n");
    }

}

//
// parse command arguments, look for an ocean fx spectrometer and set up the spectrometer for the speed test
//
int main(int argc, char*argv[]) 
{
#define MAX_IP_ADDRESS_LENGTH 15
	long data_buffer_feature_id = 0;
	long spectrometer_feature_id = 0;

    int number_of_devices;
    long *device_ids;
    int flag;
    int error = 0;
    char nameBuffer[80];
	bool use_tcp = false;
	
	char ipAddress[] = "192.168.254.254";  // default ocean fx ip address
	unsigned int port = 57357;
    
    // parse command line arguments
    if(argc == 2)
	{
		struct in_addr addr;
		
		if((strcmp(argv[1], "?") == 0) || (strcmp(argv[1], "help") == 0))
		{
			display_usage(argv[0]);
			return EXIT_SUCCESS;
		}
		else if(inet_aton(argv[1], &addr) != 0) // 0 if not valid
		{
			use_tcp = true;
			strncpy(ipAddress, argv[1], MAX_IP_ADDRESS_LENGTH);
		}
		else
		{
			display_usage(argv[0]);
			return EXIT_FAILURE;
		}	
    }
    else if(argc != 1)
	{
		display_usage(argv[0]);
		return EXIT_FAILURE;
	}
    
    printf("\n\n\n=====> STARTING OCEAN FX SPEED TEST <=====\n\n"); fflush(stdout);

    printf("Test time = %d\n", TEST_TIME);
    printf("Spectra per trigger = %d\n", SPECTRA_PER_TRIGGER);
    printf("Spectra retrieved per query = %d\n", NUMBER_SPECTRA_TO_RETRIEVE);
    printf("Fast Buffer Enable = %d\n", FAST_BUFFER_ENABLED);
    printf("Display period = %d\n", DISPLAY_PERIOD);
    printf("Trigger Mode = %d\n", OCEANFX_TRIGGER_MODE);
    printf("Loop delay = %d us\n\n", LOOP_DELAY);

    /* Initialize the SeaBreeze driver */
    sbapi_initialize();

    printf("Probing for USB devices...\n"); fflush(stdout);
    sbapi_probe_devices();



	if(use_tcp)
	{
		printf("Adding a TCP/IPv4 device.\n");
		int result = sbapi_add_TCPIPv4_device_location("FlameX", ipAddress, port);
		if(result == 0)
		{
			printf("IP Address %s was added.", ipAddress);
		}
		else
		{
			printf("Could not add IP Address %s. Quitting...", ipAddress);
			return(EXIT_FAILURE);
		}	
	}
   
    number_of_devices = sbapi_get_number_of_device_ids();

    if(number_of_devices > 0)
    {
		device_ids = (long *)calloc((size_t)number_of_devices, sizeof(long));
		number_of_devices = sbapi_get_device_ids(device_ids, (unsigned int)number_of_devices);

		if(number_of_devices > 0)
		{
			printf("Device ID = 0x%02lX:\n", device_ids[0]);
			flag = sbapi_get_device_type(device_ids[0], &error, nameBuffer, 79);
			if(flag > 0) 
			{
				printf("Device type = [%s]\n", nameBuffer);
			}

			/* Open the device */
			if(strcmp(nameBuffer, "FLAMEX") == 0)
			{
				if(sbapi_open_device(device_ids[0], &error) == 0)
				{
					printf("Connected to the spectrometer\n");
					char serial_number[255];
					
					// normally, much more would be needed to set up a spectrometer, but in this case, since the test is
					//  only for Ocean FX spectrometers, and only for testing speed, just the trigger mode and integration time are set. 
					if((error = get_serial_number(device_ids[0], serial_number, sizeof(serial_number))) == 0)
					{
						printf("Serial Number = %s\n", serial_number);
					}
					else
					{
						printf("Could not retrieve a serial number.\n");
					}
					
					// configure spectrometer
					if((error = configure_spectrometer_feature(device_ids[0], &spectrometer_feature_id)) == 0)
					{
						// configure data buffer
						if((error = configure_data_buffer_feature(device_ids[0], &data_buffer_feature_id)) == 0)
						{
							// configure fast buffer
							if((error = configure_fast_buffer_feature(device_ids[0])) == 0)
							{							
								ocean_fx_standard_speed_test(device_ids[0], data_buffer_feature_id, spectrometer_feature_id);
                                //if(FAST_BUFFER_ENABLED == 1)
                                //{
                                    ocean_fx_split_speed_test(device_ids[0], data_buffer_feature_id, spectrometer_feature_id);
                                //}
                                //else
                                //{
                                    //printf("Overlapped test disabled, since fast buffering is disabled.\n");
                                //}
							}
							else
							{
								printf("Could not configure the data buffer feature. Error=%d\n", error);
							}
						}
						else
						{
							printf("Could not configure the data buffer feature. Error=%d\n", error);
						}
					}
					else
					{
						printf("Could not configure the spectrometer feature. Error=%x\n", error);
					} 			
				}
				else
				{
					printf("Could not connect to  the spectrometer: %s]\n", sbapi_get_error_string(error));			
				}			
			}
			else
			{
				printf("The spectrometer found (%s), was not an Ocean FX.\n", nameBuffer);
			}

		
			/* Close the device */
			printf("Disconnecting from the spectrometer.\n");
			sbapi_close_device(device_ids[0], &error);

		}
		free(device_ids);    
    }
    else
    {
    	printf("No Ocean FX spectrometers were found.\n");
        return EXIT_FAILURE;
    }

    /* Clean up memory allocated by the driver */
    sbapi_shutdown();
    printf("\n\n=====> FINISHED OCEAN FX SPEED TEST <=====\n\n\n"); fflush(stdout);
    return 0;
}
