
/*******************************************************
* File:    windows_api_test_.cpp
* Date:    January 2017
* Author:  Ocean Optics, Inc.
*
* This is a test program to exercise some of the
* SeaBreeze functionality.  The project is included as part
* of the Visual Studio 2015 seabreeze solution
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
// define to run the self test. Howver, this can take 2 minutes or more, so by default that test is not run.
//#define RUN_SELF_TEST

/* Includes */
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <chrono>
#include <thread>
#include "api/seabreezeapi/SeaBreezeAPI.h"
#include "api/seabreezeapi/SeaBreezeAPIConstants.h"

#include <vector>


/* Prototypes */
void test_serial_number_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_spectrometer_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_shutter_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_light_source_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_lamp_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_eeprom_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_irradcal_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_tec_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_nonlinearity_coeffs_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_temperature_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_spectrum_processing_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_revision_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_optical_bench_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_stray_light_coeffs_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_continuous_strobe_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_data_buffer_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_fast_buffer_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_networking_features(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_acquisition_delay_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_pixel_binning_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);
void test_miscellaneous_commands(long deviceID, int *unsupportedFeatureCount, int *testFailureCount);

void test_ethernet_features(long deviceID, int *unsupportedFeatureCount, int *testFailureCount, unsigned char networkInterfaceIndex); // tested as part of networking features
void test_dhcp_server_features(long deviceID, int *unsupportedFeatureCount, int *testFailureCount, unsigned char networkInterfaceIndex); // tested as part of networking features
void test_ipv4_features(long deviceID, int *unsupportedFeatureCount, int *testFailureCount, unsigned char networkInterfaceIndex); // tested as part of networking features
void test_multicast_features(long deviceID, int *unsupportedFeatureCount, int *testFailureCount, unsigned char networkInterfaceIndex); // tested as part of networking features
void test_wifi_features(long deviceID, int *unsupportedFeatureCount, int *testFailureCount, unsigned char networkInterfaceIndex); // tested as part of networking features

/* Create a type called "testfunc_t" that is just a pointer to any function that
* has this signature:  void func(long)
* This will be used to invoke all of the test functions by reference.
*/
typedef void(*testfunc_t)(long, int*, int*);

/* Create a list of functions to run on each device that is found and opened */
static testfunc_t __test_functions[] = 
{
	test_serial_number_feature,
	test_revision_feature,
	test_optical_bench_feature,
	test_spectrometer_feature,
	test_shutter_feature,
	test_light_source_feature,
	test_lamp_feature,
	test_eeprom_feature,
	test_irradcal_feature,
	test_tec_feature,
	test_nonlinearity_coeffs_feature,
	test_temperature_feature,
	test_spectrum_processing_feature,
	test_stray_light_coeffs_feature,
	test_continuous_strobe_feature,
	test_data_buffer_feature,
	test_fast_buffer_feature,
	test_acquisition_delay_feature,
	test_pixel_binning_feature,
	test_networking_features, // also includes ethernet, multicast, ipv4, dhcp server and wifi
	test_miscellaneous_commands
};

/* Utilities to count errors and unsupported features */
void tallyErrors(int error, int *testFailureCount)
{
	if (error != 0) // 0 = no error
	{
		(*testFailureCount)++;
		printf("====>Log Failure, Count=%d\n", *testFailureCount);
	}
}

void tallyUnsupportedFeatures(int *unsupportedFeatureCount)
{
	(*unsupportedFeatureCount)++;
	printf("------->Unsupported Feature, Count=%d\n", *unsupportedFeatureCount);
}

/* Get a variable that tracks how many test functions have been specified */
static const int __test_function_count = sizeof(__test_functions) / sizeof(testfunc_t);

int main(void)
{


	int errorCode = NOERROR;

	int number_of_devices = 0;
	int devices_available = 0;
	long *device_ids = NULL;
	int i = 0;
	int test_index = 0;
	int flag = 0;
	int unsupportedFeatureCount = 0;
	int testFailureCount = 0;
	int error = 0;
	char nameBuffer[80];

	printf("Ocean Optics API Test\n"); fflush(stdout);

	/* Give the driver a chance to initialize itself */
	sbapi_initialize();

	printf("Probing for devices...\n"); fflush(stdout);
	devices_available = sbapi_probe_devices();

	printf("Getting device count...\n"); fflush(stdout);
	number_of_devices = sbapi_get_number_of_device_ids();

	printf("Device count is %d\n", number_of_devices);
	if (0 == number_of_devices) 
	{
		return 0;
	}

	printf("Getting device IDs...\n");
	device_ids = (long *)calloc(number_of_devices, sizeof(long));
	number_of_devices = sbapi_get_device_ids(device_ids, number_of_devices);
	printf("Got %d device ID%s.\n", number_of_devices, number_of_devices == 1 ? "" : "s");

	for (i = 0; i < number_of_devices; i++) 
	{
		printf("%d: Device 0x%02lX:\n", i, device_ids[i]);
		printf("\tGetting device type...\n");
		flag = sbapi_get_device_type(device_ids[i], &error, nameBuffer, 79);
		printf("\t\tResult is (%d) [%s]\n", flag, sbapi_get_error_string(error));
		if (flag > 0) 
		{
			printf("\tDevice type: [%s]\n", nameBuffer);
		}

		/* Open the device */
		printf("\tAttempting to open:\n");
		flag = sbapi_open_device(device_ids[i], &error);
		printf("\t\tResult is (%d) [%s]\n", flag, sbapi_get_error_string(error));

		// jump to the next iteration if there was a problem
		if (flag != 0) 
		{
			continue;
		}

		// log deviations
		unsupportedFeatureCount = 0;
		testFailureCount = 0;

		/* Test the device */
		for (test_index = 0; test_index < __test_function_count; test_index++) 
		{
			/* Invoke each of the test functions against this device */
			__test_functions[test_index](device_ids[i], &unsupportedFeatureCount, &testFailureCount);
		}

		/* Close the device */
		printf("\tAttempting to close:\n");
		sbapi_close_device(device_ids[i], &error);
		printf("\t\tResult is (%d) [%s]\n", flag, sbapi_get_error_string(error));
		printf("%d: Device 0x%02lX: \n\tNumber of unsupported features = %d\n\tNumber of test failures = %d\n", i, device_ids[i], unsupportedFeatureCount, testFailureCount);
	}

	free(device_ids);

	printf("Finished testing.\n");

	/* Clean up memory allocated by the driver */
	sbapi_shutdown();
	// Provided in case the user ran the application in a new terminal window,
	// which would otherwise close (destroying the output) upon exit.
	char line[80];
	do 
	{
		printf("\nType \"quit\" to exit...");
		fflush(stdout);

		memset(line, 0, 80);
		fgets(line, sizeof(line), stdin);

	} while (strncmp(line, "quit", 4));
	printf("\ndone.\n");

	return errorCode;
}

//////////////////////////////////////////////////////////////////////
// tests
///////////////////////////////////////////////////////////////////////

void test_serial_number_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount)
{
	int error = 0;
	int number_of_serial_numbers;
	long *serial_number_ids = 0;
	int i;
	unsigned char length;
	char *buffer;

	printf("\n\tTesting serial number features:\n");

	printf("\t\tGetting number of serial numbers:\n");
	number_of_serial_numbers = sbapi_get_number_of_serial_number_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_serial_numbers, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_serial_numbers) 
	{
		printf("\tNo serial number capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);
		return;
	}

	serial_number_ids = (long *)calloc(number_of_serial_numbers, sizeof(long));
	printf("\t\tGetting serial number feature IDs...\n");
	number_of_serial_numbers = sbapi_get_serial_number_features(deviceID, &error,
		serial_number_ids, number_of_serial_numbers);
	printf("\t\t\tResult is %d [%s]\n", number_of_serial_numbers,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_serial_numbers; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, serial number 0x%02lX\n",
			i, deviceID, serial_number_ids[i]);

		printf("\t\t\tAttempting to get the maximum serial number length...\n");
		length = sbapi_get_serial_number_maximum_length(deviceID, serial_number_ids[i], &error);
		printf("\t\t\tResult is %d [%s]\n", length, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		buffer = (PCHAR)calloc(length + 1, sizeof(char));

		printf("\t\t\tAttempting to get serial number...\n");
		//memset(buffer, (int)0, sizeof(buffer));
		sbapi_get_serial_number(deviceID, serial_number_ids[i], &error, buffer, length);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		if (0 == error)
		{
			printf("\t\t\t\tSerial number: [%s]\n", buffer);
		}

		free(buffer);
		printf("\t\t%d: Finished testing device 0x%02lX, serial number 0x%02lX\n",
			i, deviceID, serial_number_ids[i]);
	}

	free(serial_number_ids);

	printf("\tFinished testing serial number capabilities.\n");
}

void test_revision_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount) 
{
	int error = 0;
	int number_of_revision_features;
	long *revision_feature_ids = 0;
	int i;

	printf("\n\tTesting revision features:\n");

	printf("\t\tGetting number of revision features:\n");
	number_of_revision_features = sbapi_get_number_of_revision_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_revision_features, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_revision_features)
	{
		printf("\tNo revision capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	revision_feature_ids =
		(long *)calloc(number_of_revision_features, sizeof(long));
	printf("\t\tGetting revisino feature IDs...\n");
	number_of_revision_features = sbapi_get_revision_features(
		deviceID, &error, revision_feature_ids,
		number_of_revision_features);
	printf("\t\t\tResult is %d [%s]\n", number_of_revision_features,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_revision_features; i++) 
	{
		printf("\t\t%d: Testing device 0x%02lX, revision 0x%02lX\n", i, deviceID, revision_feature_ids[i]);
		printf("\t\t\tAttempting to get revisions...\n");
		printf("\t\t\t\tHardware Revision: 0x%x\n", sbapi_revision_hardware_get(deviceID, revision_feature_ids[i], &error));
		printf("\t\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);
		printf("\t\t\t\tFirmware Revision: 0x%x\n", sbapi_revision_firmware_get(deviceID, revision_feature_ids[i], &error));
		printf("\t\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t%d: Finished testing device 0x%02lX, revision 0x%02lX\n",
			i, deviceID, revision_feature_ids[i]);
	}
	free(revision_feature_ids);

	printf("\tFinished testing revision capabilities.\n");
}

void test_optical_bench_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount) 
{
	int error = 0;
	int number_of_optical_benches;
	long *optical_bench_ids = 0;
	int i;
	unsigned int slit_or_fiber;
	char buffer[80];

	printf("\n\tTesting optical bench features:\n");

	printf("\t\tGetting number of optical benches:\n");
	number_of_optical_benches = sbapi_get_number_of_optical_bench_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_optical_benches, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_optical_benches) 
	{
		printf("\tNo optical bench capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	optical_bench_ids = (long *)calloc(number_of_optical_benches, sizeof(long));
	printf("\t\tGetting optical bench feature IDs...\n");
	number_of_optical_benches = sbapi_get_optical_bench_features(deviceID, &error, optical_bench_ids, number_of_optical_benches);
	printf("\t\t\tResult is %d [%s]\n", number_of_optical_benches, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_optical_benches; i++) 
	{
		printf("\t\t%d: Testing device 0x%02lX, optical bench feature id 0x%02lX\n", i, deviceID, optical_bench_ids[i]);

		printf("\t\t\tAttempting to get optical bench ID...\n");
		memset(buffer, (int)0, sizeof(buffer));
		sbapi_optical_bench_get_id(deviceID, optical_bench_ids[i], &error, buffer, 79);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);
		if (0 == error)
		{
			printf("\t\t\t\tOptical Bench ID: [%s]\n", buffer);
		}

		printf("\t\t\tAttempting to get optical bench serial number...\n");
		memset(buffer, (int)0, sizeof(buffer));
		sbapi_optical_bench_get_serial_number(deviceID, optical_bench_ids[i], &error, buffer, 79);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);
		if (0 == error)
		{
			printf("\t\t\t\tOptical Bench Serial Number: [%s]\n", buffer);
		}

		printf("\t\t\tAttempting to get optical bench coating type...\n");
		memset(buffer, (int)0, sizeof(buffer));
		sbapi_optical_bench_get_coating(deviceID, optical_bench_ids[i], &error, buffer, 79);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);
		if (0 == error)
		{
			printf("\t\t\t\tOptical Bench coating type: [%s]\n", buffer);
		}

		printf("\t\t\tAttempting to get optical bench filter type...\n");
		memset(buffer, (int)0, sizeof(buffer));
		sbapi_optical_bench_get_filter(deviceID, optical_bench_ids[i], &error, buffer, 79);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);
		if (0 == error) 
		{
			printf("\t\t\t\tOptical Bench filter type: [%s]\n", buffer);
		}

		printf("\t\t\tAttempting to get optical bench grating type...\n");
		memset(buffer, (int)0, sizeof(buffer));
		sbapi_optical_bench_get_grating(deviceID, optical_bench_ids[i], &error, buffer, 79);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);
		if (0 == error)
		{
			printf("\t\t\t\tOptical Bench grating type: [%s]\n", buffer);
		}

		printf("\t\t\tAttempting to get optical bench slit width...\n");
		memset(buffer, (int)0, sizeof(buffer));
		slit_or_fiber = sbapi_optical_bench_get_slit_width_microns(deviceID, optical_bench_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);
		if (0 == error) 
		{
			printf("\t\t\t\tOptical Bench slit width (microns): [%d]\n", slit_or_fiber);
		}

		printf("\t\t\tAttempting to get optical bench fiber diameter...\n");
		memset(buffer, (int)0, sizeof(buffer));
		slit_or_fiber = sbapi_optical_bench_get_slit_width_microns(deviceID, optical_bench_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);
		if (0 == error) 
		{
			printf("\t\t\t\tOptical Bench fiber diameter (microns): [%d]\n", slit_or_fiber);
		}

		printf("\t\t%d: Finished testing device 0x%02lX, optical bench feature id 0x%02lX\n", i, deviceID, optical_bench_ids[i]);
	}

	free(optical_bench_ids);

	printf("\tFinished testing optical bench capabilities.\n");
}

void test_spectrometer_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount) {
	int error = 0;
	int length;
	long integration_time;
	double max_intensity;
	int number_of_spectrometers;
	long *spectrometer_ids = 0;
	int i;
	double *doubleBuffer = 0;
	unsigned char *byteBuffer = 0;
	int *edarkBuffer = 0;
	int j;

	printf("\n\tTesting spectrometer features:\n");

	printf("\t\tGetting number of spectrometers:\n");
	number_of_spectrometers = sbapi_get_number_of_spectrometer_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_spectrometers,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_spectrometers)
	{
		printf("\tNo spectrometer capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	spectrometer_ids = (long *)calloc(number_of_spectrometers, sizeof(long));
	printf("\t\tGetting spectrometer feature IDs...\n");
	number_of_spectrometers = sbapi_get_spectrometer_features(deviceID, &error,
		spectrometer_ids, number_of_spectrometers);
	printf("\t\t\tResult is %d [%s]\n", number_of_spectrometers,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	// this really only works for Jaz...
	for (i = 0; i < number_of_spectrometers; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, spectrometer 0x%02lX\n",
			i, deviceID, spectrometer_ids[i]);

		printf("\t\t\tAttempting to set trigger mode to 0\n");
		sbapi_spectrometer_set_trigger_mode(deviceID, spectrometer_ids[i], &error, 0);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tGetting maximum intensity\n");
		max_intensity = sbapi_spectrometer_get_maximum_intensity(deviceID, spectrometer_ids[i], &error);
		printf("\t\t\t\tResult is %1.2f [%s]\n", max_intensity,
			sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tGetting minimum integration time\n");
		integration_time = sbapi_spectrometer_get_minimum_integration_time_micros(deviceID, spectrometer_ids[i], &error);
		printf("\t\t\t\tResult is %ld [%s]\n", integration_time,
			sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tSetting integration time to minimum:\n");
		sbapi_spectrometer_set_integration_time_micros(deviceID, spectrometer_ids[i], &error, integration_time);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tGetting spectrum length\n");
		length = sbapi_spectrometer_get_formatted_spectrum_length(deviceID,
			spectrometer_ids[i], &error);
		printf("\t\t\t\tResult is %d [%s]\n", length,
			sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		doubleBuffer = (double *)calloc(length, sizeof(double));
		printf("\t\t\tGetting spectrum...\n");
		length = sbapi_spectrometer_get_formatted_spectrum(deviceID,
			spectrometer_ids[i], &error, doubleBuffer, length);
		printf("\t\t\t\tResult is %d [%s]\n", length,
			sbapi_get_error_string(error));
		printf("\t\t\t\tPixel indices 19 and 20 are: %1.1f, %1.1f\n",
			doubleBuffer[19], doubleBuffer[20]);
		free(doubleBuffer);
		tallyErrors(error, testFailureCount);

		printf("\t\t\tGetting unformatted spectrum length\n");
		length = sbapi_spectrometer_get_unformatted_spectrum_length(deviceID,
			spectrometer_ids[i], &error);
		printf("\t\t\t\tResult is %d [%s]\n", length,
			sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		byteBuffer = (unsigned char *)calloc(length, sizeof(unsigned char));
		printf("\t\t\tGetting unformatted spectrum...\n");
		length = sbapi_spectrometer_get_unformatted_spectrum(deviceID,
			spectrometer_ids[i], &error, byteBuffer, length);
		printf("\t\t\t\tResult is %d [%s]\n", length,
			sbapi_get_error_string(error));
		printf("\t\t\t\tByte indices 19 and 20 are: 0x%02X, 0x%02X\n",
			byteBuffer[19], byteBuffer[20]);
		free(byteBuffer);
		tallyErrors(error, testFailureCount);

		doubleBuffer = (double *)calloc(length, sizeof(double));
		printf("\t\t\tGetting wavelengths...\n");
		length = sbapi_spectrometer_get_wavelengths(deviceID,
			spectrometer_ids[i], &error, doubleBuffer, length);
		if (length == 0)
			printf("\t\t\t\tThere were no wavelength calibration coefficients returned by the spectrometer.\n");
		printf("\t\t\t\tResult is %d [%s]\n", length,
			sbapi_get_error_string(error));
		printf("\t\t\t\tPixel indices 19 and 20 are: %1.2f, %1.2f\n",
			doubleBuffer[19], doubleBuffer[20]);
		free(doubleBuffer);
		tallyErrors(error, testFailureCount);

		printf("\t\t\tGetting electric dark pixel count...\n");
		length = sbapi_spectrometer_get_electric_dark_pixel_count(
			deviceID, spectrometer_ids[i], &error);
		printf("\t\t\t\tResult is %d [%s]\n", length,
			sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		if (length > 0)
		{
			edarkBuffer = (int *)calloc(length, sizeof(int));
			printf("\t\t\tGetting electric dark pixels...\n");
			length = sbapi_spectrometer_get_electric_dark_pixel_indices(deviceID,
				spectrometer_ids[i], &error, edarkBuffer, length);
			printf("\t\t\t\tResult is %d [%s]\n", length,
				sbapi_get_error_string(error));
			printf("\t\t\tIndices: ");
			tallyErrors(error, testFailureCount);

			for (j = 0; j < length; j++)
			{
				printf("%d ", edarkBuffer[j]);
			}
			printf("\n");
			free(edarkBuffer);
		}
		else 
		{
			printf("\t\t\t\tSpectrometer has no electric dark pixels.\n");
		}


		printf("\t\t%d: Finished testing device 0x%02lX, spectrometer 0x%02lX\n",
			i, deviceID, spectrometer_ids[i]);
	}
	printf("\tFinished testing spectrometer capabilities.\n");

	free(spectrometer_ids);
}


// ToDo Although this tests the shutter firmware, it does not test the shutter hardware
//   This test should be rewritten to look for a change in light to indicate 
//   actual shutter activity instead of a software indicator
void test_shutter_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount) {
	int error = 0;
	int number_of_shutters;
	long *shutter_ids = 0;
	int i;

	printf("\n\tTesting shutter features:\n");

	printf("\t\tGetting number of shutters:\n");
	number_of_shutters = sbapi_get_number_of_shutter_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_shutters,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_shutters)
	{
		printf("\tNo shutter capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	shutter_ids = (long *)calloc(number_of_shutters, sizeof(long));
	printf("\t\tGetting shutter feature IDs...\n");
	number_of_shutters = sbapi_get_shutter_features(deviceID, &error,
		shutter_ids, number_of_shutters);
	printf("\t\t\tResult is %d [%s]\n", number_of_shutters,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_shutters; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, shutter 0x%02lX\n",
			i, deviceID, shutter_ids[i]);
		printf("\t\t\tNOTE: Some shutter features are synchronized to other\n"
			"\t\t\tNOTE: events, such as spectrometer acquisitions.\n"
			"\t\t\tNOTE: This test only sends the shutter command and this\n"
			"\t\t\tNOTE: may be inadequate to actually toggle the shutter.\n"
			"\t\t\tNOTE: This test is only intended to see if the command\n"
			"\t\t\tNOTE: can be sucessfully created and sent.\n");
		printf("\t\t\tAttempting to close shutter.\n");
		sbapi_shutter_set_shutter_open(deviceID,
			shutter_ids[i], &error, 0);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to open shutter.\n");
		sbapi_shutter_set_shutter_open(deviceID,
			shutter_ids[i], &error, 1);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t%d: Finished testing device 0x%02lX, shutter 0x%02lX\n",
			i, deviceID, shutter_ids[i]);
	}
	free(shutter_ids);

	printf("\tFinished testing shutter capabilities.\n");
}

void test_spectrum_processing_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount) {
	int error = 0;
	int number_of_sp;
	long *sp_ids = 0;
	int i;
	unsigned char boxcarWidthW, boxcarWidthR, boxcarWidthR2;
	unsigned short int scansToAverageW, scansToAverageR, scansToAverageR2;

	printf("\n\tTesting spectrum processing features:\n");

	printf("\t\tGetting number of spectrum processing features:\n");
	number_of_sp = sbapi_get_number_of_spectrum_processing_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_sp,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_sp) 
	{
		printf("\tNo spectrum processing capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	sp_ids = (long *)calloc(number_of_sp, sizeof(long));
	printf("\t\tGetting spectrum processing feature IDs...\n");
	number_of_sp = sbapi_get_spectrum_processing_features(deviceID, &error,
		sp_ids, number_of_sp);
	printf("\t\t\tResult is %d [%s]\n", number_of_sp, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_sp; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, spectrum processing 0x%02lX\n",
			i, deviceID, sp_ids[i]);

		printf("\t\t\tAttempting to read boxcar filter width.\n");
		boxcarWidthR = sbapi_spectrum_processing_boxcar_width_get(deviceID, sp_ids[i], &error);
		printf("\t\t\t\tResult is [%s], boxcar filter width= %d\n", sbapi_get_error_string(error), boxcarWidthR);
		tallyErrors(error, testFailureCount);

		if (boxcarWidthR<15)
			boxcarWidthW = boxcarWidthR + 1;
		else
			boxcarWidthW = 0;

		printf("\t\t\tAttempting to write a boxcar filter width of %d.\n", boxcarWidthW);
		sbapi_spectrum_processing_boxcar_width_set(deviceID, sp_ids[i], &error, boxcarWidthW);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to read boxcar filter width again.\n");
		boxcarWidthR2 = sbapi_spectrum_processing_boxcar_width_get(deviceID, sp_ids[i], &error);
		printf("\t\t\t\tResult is [%s], boxcar filter width = %d\n", sbapi_get_error_string(error), boxcarWidthR2);
		tallyErrors(error, testFailureCount);

		if ((boxcarWidthR2 == boxcarWidthW) && (boxcarWidthR2 != boxcarWidthR))
			printf("\t\t\t\t...passed...\n");
		else
		{
			printf("\t\t\t\t...failed...\n");
			(*testFailureCount)++;
		}


		printf("\t\t\tAttempting to read scans to average.\n");
		scansToAverageR = sbapi_spectrum_processing_scans_to_average_get(deviceID, sp_ids[i], &error);
		printf("\t\t\t\tResult is [%s], scans to average = %d\n", sbapi_get_error_string(error), scansToAverageR);
		tallyErrors(error, testFailureCount);

		if ((scansToAverageR >= 1) && (scansToAverageR<15))
			scansToAverageW = scansToAverageR + 1;
		else
			scansToAverageW = 1;

		printf("\t\t\tAttempting to write a scans to average number of %d.\n", scansToAverageW);
		sbapi_spectrum_processing_scans_to_average_set(deviceID, sp_ids[i], &error, scansToAverageW);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to read scans to average again.\n");
		scansToAverageR2 = sbapi_spectrum_processing_scans_to_average_get(deviceID, sp_ids[i], &error);
		printf("\t\t\t\tResult is [%s], scans to average = %d\n", sbapi_get_error_string(error), scansToAverageR2);
		tallyErrors(error, testFailureCount);

		if ((scansToAverageR2 == scansToAverageW) && (scansToAverageR2 != scansToAverageR))
			printf("\t\t\t\t...passed...\n");
		else
		{
			printf("\t\t\t\t...failed...\n");
			(*testFailureCount)++;
		}

		printf("\t\t%d: Finished testing device 0x%02lX, spectrum processing feature 0x%02lX\n",
			i, deviceID, sp_ids[i]);
	}
	free(sp_ids);

	printf("\tFinished testing spectrum processing capabilities.\n");
}

void test_light_source_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount) {
	int error = 0;
	int number_of_light_sources;
	long *light_source_ids = 0;
	int source_count;
	int i;
	int source;
	unsigned char flag;
	double intensity = 0;

	printf("\n\tTesting light source features:\n");

	printf("\t\tGetting number of light sources:\n");
	number_of_light_sources = sbapi_get_number_of_light_source_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_light_sources,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_light_sources) 
	{
		printf("\tNo light source capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	light_source_ids = (long *)calloc(number_of_light_sources, sizeof(long));
	printf("\t\tGetting light source feature IDs...\n");
	number_of_light_sources = sbapi_get_light_source_features(deviceID, &error,
		light_source_ids, number_of_light_sources);
	printf("\t\t\tResult is %d [%s]\n", number_of_light_sources,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_light_sources; i++) 
	{
		printf("\t\t%d: Testing device 0x%02lX, light source 0x%02lX\n",
			i, deviceID, light_source_ids[i]);

		printf("\t\t\tAttempting to get light source count for feature.\n");
		source_count = sbapi_light_source_get_count(deviceID,
			light_source_ids[i], &error);
		printf("\t\t\t\tResult is %d [%s]\n", source_count,
			sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		for (source = 0; source < source_count; source++) 
		{
			printf("\t\t\tExamining light source %d\n", source);

			printf("\t\t\t\tAttempting to query whether intensity control is available\n");
			flag = sbapi_light_source_has_variable_intensity(deviceID, light_source_ids[i],
				&error, source);
			printf("\t\t\t\tResult is %s [%s]\n", 0 == flag ? "false" : "true",
				sbapi_get_error_string(error));
			tallyErrors(error, testFailureCount);

			if (0 != flag)
			{
				printf("\t\t\t\tAttempting to get light source %d percent intensity\n", source);
				intensity = sbapi_light_source_get_intensity(deviceID, light_source_ids[i], &error, source);
				printf("\t\t\t\t\tResult is %1.1f%% [%s]\n", intensity * 100.0, sbapi_get_error_string(error));
				tallyErrors(error, testFailureCount);

				printf("\t\t\t\tAttempting to set intensity to half of maximum\n");
				sbapi_light_source_set_intensity(deviceID, light_source_ids[i],
					&error, source, 0.5);
				printf("\t\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
				tallyErrors(error, testFailureCount);

				/* Re-read the intensity to see if it was applied */
				printf("\t\t\t\tAttempting to get light source %d percent intensity\n", source);
				intensity = sbapi_light_source_get_intensity(deviceID, light_source_ids[i], &error, source);
				printf("\t\t\t\t\tResult is %1.1f%% [%s]\n", intensity * 100.0, sbapi_get_error_string(error));
				tallyErrors(error, testFailureCount);
			}

			printf("\t\t\t\tAttempting to query whether enable is available\n");
			flag = sbapi_light_source_has_enable(deviceID, light_source_ids[i],
				&error, source);
			printf("\t\t\t\tResult is %s [%s]\n", 0 == flag ? "false" : "true",
				sbapi_get_error_string(error));
			tallyErrors(error, testFailureCount);

			if (0 != flag)
			{
				printf("\t\t\t\tAttempting to get light source %d enable status\n", source);
				flag = sbapi_light_source_is_enabled(deviceID, light_source_ids[i],
					&error, source);
				printf("\t\t\t\t\tResult is %s [%s]\n", 0 == flag ? "false" : "true",
					sbapi_get_error_string(error));
				tallyErrors(error, testFailureCount);

				printf("\t\t\t\tAttempting to enable light source %d\n", source);
				sbapi_light_source_set_enable(deviceID, light_source_ids[i],
					&error, source, 1);
				printf("\t\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
				tallyErrors(error, testFailureCount);

				printf("\t\t\t\tAttempting to get light source %d enable status\n", source);
				flag = sbapi_light_source_is_enabled(deviceID, light_source_ids[i],
					&error, source);
				printf("\t\t\t\t\tResult is %s [%s]\n", 0 == flag ? "false" : "true",
					sbapi_get_error_string(error));
				tallyErrors(error, testFailureCount);

				printf("\t\t\t\tAttempting to disable light source %d\n", source);
				sbapi_light_source_set_enable(deviceID, light_source_ids[i],
					&error, source, 0);
				printf("\t\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
				tallyErrors(error, testFailureCount);

				printf("\t\t\t\tAttempting to get light source %d enable status\n", source);
				flag = sbapi_light_source_is_enabled(deviceID, light_source_ids[i],
					&error, source);
				printf("\t\t\t\t\tResult is %s [%s]\n", 0 == flag ? "false" : "true",
					sbapi_get_error_string(error));
				tallyErrors(error, testFailureCount);

			}
		}
	}

	free(light_source_ids);
	printf("\tFinished testing light source capabilities.\n");
}

void test_lamp_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount) {
	int error = 0;
	int number_of_lamps;
	long *lamp_ids = 0;
	int i;

	printf("\n\tTesting lamp features:\n");

	printf("\t\tGetting number of lamps:\n");
	number_of_lamps = sbapi_get_number_of_lamp_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_lamps,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_lamps) 
	{
		printf("\tNo lamp capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	lamp_ids = (long *)calloc(number_of_lamps, sizeof(long));
	printf("\t\tGetting lamp feature IDs...\n");
	number_of_lamps = sbapi_get_lamp_features(deviceID, &error,
		lamp_ids, number_of_lamps);
	printf("\t\t\tResult is %d [%s]\n", number_of_lamps,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_lamps; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, lamp 0x%02lX\n",
			i, deviceID, lamp_ids[i]);
		printf("\t\t\tNOTE: Some lamp features are synchronized to other\n"
			"\t\t\tNOTE: events, such as spectrometer acquisitions.\n"
			"\t\t\tNOTE: This test only sends the lamp command and this\n"
			"\t\t\tNOTE: may be inadequate to actually toggle the lamp.\n"
			"\t\t\tNOTE: This test is only intended to see if the command\n"
			"\t\t\tNOTE: can be sucessfully created and sent.\n");
		printf("\t\t\tAttempting to enable lamp.\n");
		sbapi_lamp_set_lamp_enable(deviceID,
			lamp_ids[i], &error, 1);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to disable lamp.\n");
		sbapi_lamp_set_lamp_enable(deviceID,
			lamp_ids[i], &error, 0);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));

		printf("\t\t%d: Finished testing device 0x%02lX, lamp 0x%02lX\n",
			i, deviceID, lamp_ids[i]);
		tallyErrors(error, testFailureCount);
	}
	free(lamp_ids);
	printf("\tFinished testing lamp capabilities.\n");
}

void test_eeprom_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount)
{
	int error = 0;
	int number_of_eeproms;
	long *eeprom_ids = 0;
	unsigned char buffer[80];
	int i;

	printf("\n\tTesting EEPROM features:\n");

	printf("\t\tGetting number of EEPROMs:\n");
	number_of_eeproms = sbapi_get_number_of_eeprom_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_eeproms,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_eeproms) 
	{
		printf("\tNo EEPROM capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	eeprom_ids = (long *)calloc(number_of_eeproms, sizeof(long));
	printf("\t\tGetting EEPROM feature IDs...\n");
	number_of_eeproms = sbapi_get_eeprom_features(deviceID, &error,
		eeprom_ids, number_of_eeproms);
	printf("\t\t\tResult is %d [%s]\n", number_of_eeproms,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_eeproms; i++) 
	{
		printf("\t\t%d: Testing device 0x%02lX, eeprom 0x%02lX\n",
			i, deviceID, eeprom_ids[i]);

		printf("\t\t\tAttempting to get EEPROM slot 0...\n");
		memset(buffer, (int)0, sizeof(buffer));
		sbapi_eeprom_read_slot(deviceID, eeprom_ids[i], &error, 0, buffer, 79);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		if (0 == error) 
		{
			printf("\t\t\t\tEEPROM slot 0: [%s]\n", buffer);
		}

		printf("\t\t%d: Finished testing device 0x%02lX, eeprom 0x%02lX\n",
			i, deviceID, eeprom_ids[i]);
	}
	free(eeprom_ids);
	printf("\tFinished testing EEPROM capabilities.\n");
}


void test_irradcal_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount) {
	int error = 0;
	int number_of_irradcals;
	long *irradcal_ids = 0;
	float buffer[20];
	int i;
	int length;
	int flag;
	float area;

	printf("\n\tTesting irradiance calibration features:\n");

	printf("\t\tGetting number of irradiance calibration features:\n");
	number_of_irradcals = sbapi_get_number_of_irrad_cal_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_irradcals,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_irradcals) 
	{
		printf("\tNo irradiance calibration capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	irradcal_ids = (long *)calloc(number_of_irradcals, sizeof(long));
	printf("\t\tGetting irradiance calibration feature IDs...\n");
	number_of_irradcals = sbapi_get_irrad_cal_features(deviceID, &error,
		irradcal_ids, number_of_irradcals);
	printf("\t\t\tResult is %d [%s]\n", number_of_irradcals,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_irradcals; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, irradcal 0x%02lX\n",
			i, deviceID, irradcal_ids[i]);

		// Note: this test can fail on OBP devices (STS) if no irradiance calibration
		//       has yet been written to the device (per STS Data Sheet, p26)
		printf("\t\t\tAttempting to get partial irradiance calibration...\n");
		memset(buffer, (int)0, sizeof(buffer));
		length = sbapi_irrad_calibration_read(deviceID, irradcal_ids[i], &error,
			buffer, 20);
		printf("\t\t\t\tRead %d values [%s]\n", length, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		if (0 == error && length > 0) 
		{
			printf("\t\t\t\tFirst calibration term: %1.2e\n", buffer[0]);
		}

		printf("\t\t\tAttempting to check for collection area...\n");
		flag = sbapi_irrad_calibration_has_collection_area(deviceID,
			irradcal_ids[i], &error);
		printf("\t\t\t\tResult is %d [%s]\n", flag, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		if (0 != flag)
		{
			printf("\t\t\tAttempting to read collection area...\n");
			area = sbapi_irrad_calibration_read_collection_area(deviceID,
				irradcal_ids[i], &error);
			printf("\t\t\t\tResult is %1.2e [%s]\n", area, sbapi_get_error_string(error));
			tallyErrors(error, testFailureCount);
		}
		else 
		{
			printf("\t\t\tNo collection area available.\n");
		}

		printf("\t\t%d: Finished testing device 0x%02lX, irradcal 0x%02lX\n",
			i, deviceID, irradcal_ids[i]);
	}
	free(irradcal_ids);

	printf("\tFinished testing irradiance calibration capabilities.\n");
}

void test_tec_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount) {
	int error = 0;
	int number_of_tec;
	long *tec_ids = 0;
	int i;
	float temperature;

	printf("\n\tTesting TEC features:\n");

	printf("\t\tGetting number of TECs:\n");
	number_of_tec = sbapi_get_number_of_thermo_electric_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_tec,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_tec) 
	{
		printf("\tNo TEC capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	tec_ids = (long *)calloc(number_of_tec, sizeof(long));
	printf("\t\tGetting TEC feature IDs...\n");
	number_of_tec = sbapi_get_thermo_electric_features(deviceID, &error,
		tec_ids, number_of_tec);
	printf("\t\t\tResult is %d [%s]\n", number_of_tec,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_tec; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, tec 0x%02lX\n",
			i, deviceID, tec_ids[i]);

		printf("\t\t\tAttempting to read TEC temperature...\n");
		temperature = (float)sbapi_tec_read_temperature_degrees_C(deviceID,
			tec_ids[i], &error);
		printf("\t\t\t\tResult is %1.2f [%s]\n", temperature,
			sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to enable TEC...\n");
		sbapi_tec_set_enable(deviceID, tec_ids[i], &error, 1);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to set TEC temperature to -5C...\n");
		sbapi_tec_set_temperature_setpoint_degrees_C(deviceID,
			tec_ids[i], &error, -5.0);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t%d: Finished testing device 0x%02lX, tec 0x%02lX\n",
			i, deviceID, tec_ids[i]);
	}

	free(tec_ids);
	printf("\tFinished testing TEC capabilities.\n");
}

void test_nonlinearity_coeffs_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount)
{
	int error = 0;
	int number_of_nonlinearity_coeff_features;
	long *nonlinearity_coeff_feature_ids = 0;
	double buffer[10];
	int i;
	int length;

	printf("\n\tTesting nonlinearity coefficient features:\n");

	printf("\t\tGetting number of nonlinearity coefficient features:\n");
	number_of_nonlinearity_coeff_features =
		sbapi_get_number_of_nonlinearity_coeffs_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_nonlinearity_coeff_features,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_nonlinearity_coeff_features)
	{
		printf("\tNo nonlinearity coefficient capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	nonlinearity_coeff_feature_ids =
		(long *)calloc(number_of_nonlinearity_coeff_features, sizeof(long));
	printf("\t\tGetting nonlinearity coefficient feature IDs...\n");
	number_of_nonlinearity_coeff_features = sbapi_get_nonlinearity_coeffs_features(
		deviceID, &error, nonlinearity_coeff_feature_ids,
		number_of_nonlinearity_coeff_features);
	printf("\t\t\tResult is %d [%s]\n", number_of_nonlinearity_coeff_features,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_nonlinearity_coeff_features; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, nonlinearity coeffs 0x%02lX\n",
			i, deviceID, nonlinearity_coeff_feature_ids[i]);

		printf("\t\t\tAttempting to get nonlinearity coefficients...\n");
		memset(buffer, (int)0, sizeof(buffer));
		length = sbapi_nonlinearity_coeffs_get(deviceID,
			nonlinearity_coeff_feature_ids[i], &error, buffer, 10);
		printf("\t\t\t\tResult is %d [%s]\n", length, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		if (0 == error && length > 0)
		{
			printf("\t\t\t\tFirst calibration term: %1.2e\n", buffer[0]);
		}

		printf("\t\t%d: Finished testing device 0x%02lX, nonlinearity coeffs 0x%02lX\n",
			i, deviceID, nonlinearity_coeff_feature_ids[i]);
	}
	free(nonlinearity_coeff_feature_ids);

	printf("\tFinished testing nonlinearity coefficient capabilities.\n");
}


void test_temperature_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount)
{
	int error = 0;
	int number_of_temperature_features;
	long *temperature_feature_ids = 0;
	double buffer[10];  // how many temperatures could there be on _any_ given spectrometer
	int i;
	int t_index;
	int length;
	double myTemp;

	printf("\n\tTesting temperature features:\n");

	printf("\t\tGetting number of temperature features:\n");
	number_of_temperature_features =
		sbapi_get_number_of_temperature_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_temperature_features,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_temperature_features) 
	{
		printf("\tNo temperature capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	temperature_feature_ids =
		(long *)calloc(number_of_temperature_features, sizeof(long));
	printf("\t\tGetting temperature feature IDs...\n");
	number_of_temperature_features = sbapi_get_temperature_features(
		deviceID, &error, temperature_feature_ids,
		number_of_temperature_features);
	printf("\t\t\tResult is %d [%s]\n", number_of_temperature_features,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_temperature_features; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, temperatures 0x%02lX\n",
			i, deviceID, temperature_feature_ids[i]);

		printf("\t\t\tAttempting to get all temperatures...\n");
		memset(buffer, (int)0, sizeof(buffer));
		length = sbapi_temperature_get_all(deviceID,
			temperature_feature_ids[i], &error, buffer, 10);
		printf("\t\t\t\tResult is %d [%s]\n", length, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		for (t_index = 0; t_index<length; t_index++) 
		{
			if (0 == error && length > 0)
			{
				printf("\t\t\t\tTemperature(%d): %2.2f\n", t_index, buffer[t_index]);
			}
		}

		printf("\t\t\tAttempting to get the number of specific temperatures...\n");

		length = sbapi_temperature_count_get(deviceID,
			temperature_feature_ids[i], &error);
		printf("\t\t\t\tResult is %d [%s]\n", length, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		for (t_index = 0; t_index<length; t_index++) 
		{
			if (0 == error && length > 0)
			{
				myTemp = sbapi_temperature_get(deviceID, temperature_feature_ids[i], &error, t_index);
				printf("\t\t\t\tTemperature(%d): %2.2f\n", t_index, myTemp);
				tallyErrors(error, testFailureCount);
			}
		}

		printf("\t\t%d: Finished testing device 0x%02lX, temperatures 0x%02lX\n",
			i, deviceID, temperature_feature_ids[i]);
	}
	free(temperature_feature_ids);

	printf("\tFinished testing temperature capabilities.\n");
}


void test_stray_light_coeffs_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount)
{
	int error = 0;
	int number_of_stray_light_coeff_features;
	long *stray_light_coeff_feature_ids = 0;
	double buffer[5];
	int i;
	int length;

	printf("\n\tTesting stray light coefficient features:\n");

	printf("\t\tGetting number of stray light coefficient features:\n");
	number_of_stray_light_coeff_features = sbapi_get_number_of_stray_light_coeffs_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_stray_light_coeff_features,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_stray_light_coeff_features) 
	{
		printf("\tNo stray light coefficient capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	stray_light_coeff_feature_ids =
		(long *)calloc(number_of_stray_light_coeff_features, sizeof(long));
	printf("\t\tGetting stray light coefficient feature IDs...\n");
	number_of_stray_light_coeff_features = sbapi_get_stray_light_coeffs_features(
		deviceID, &error, stray_light_coeff_feature_ids,
		number_of_stray_light_coeff_features);
	printf("\t\t\tResult is %d [%s]\n", number_of_stray_light_coeff_features,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_stray_light_coeff_features; i++) 
	{
		printf("\t\t%d: Testing device 0x%02lX, stray light coeffs 0x%02lX\n",
			i, deviceID, stray_light_coeff_feature_ids[i]);

		printf("\t\t\tAttempting to get stray light coefficients...\n");
		memset(buffer, (int)0, sizeof(buffer));
		length = sbapi_stray_light_coeffs_get(deviceID,
			stray_light_coeff_feature_ids[i], &error, buffer, 5);
		printf("\t\t\t\tResult is %d [%s]\n", length, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		if (0 == error && length > 0)
		{
			printf("\t\t\t\tFirst calibration term: %1.2e\n", buffer[0]);
		}

		printf("\t\t%d: Finished testing device 0x%02lX, stray light coeffs 0x%02lX\n",
			i, deviceID, stray_light_coeff_feature_ids[i]);
	}
	free(stray_light_coeff_feature_ids);

	printf("\tFinished testing stray light coefficient capabilities.\n");
}


void test_continuous_strobe_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount)
{
	int error = 0;
	int number_of_cont_strobes;
	long *cont_strobe_ids = 0;
	int i;

	printf("\n\tTesting continuous strobe features:\n");

	printf("\t\tGetting number of continuous strobes:\n");
	number_of_cont_strobes = sbapi_get_number_of_continuous_strobe_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_cont_strobes,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_cont_strobes) 
	{
		printf("\tNo continuous strobe capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	cont_strobe_ids = (long *)calloc(number_of_cont_strobes, sizeof(long));
	printf("\t\tGetting continuous strobe feature IDs...\n");
	number_of_cont_strobes = sbapi_get_continuous_strobe_features(deviceID, &error,
		cont_strobe_ids, number_of_cont_strobes);
	printf("\t\t\tResult is %d [%s]\n", number_of_cont_strobes,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_cont_strobes; i++) 
	{
		printf("\t\t%d: Testing device 0x%02lX, continuous strobe 0x%02lX\n",
			i, deviceID, cont_strobe_ids[i]);
		printf("\t\t\tAttempting to enable continous strobe.\n");
		sbapi_continuous_strobe_set_continuous_strobe_enable(deviceID,
			cont_strobe_ids[i], &error, 1);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to set period to 20ms.\n");
		sbapi_continuous_strobe_set_continuous_strobe_period_micros(deviceID,
			cont_strobe_ids[i], &error, 20000);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tDelaying to allow verification.\n");


		std::this_thread::sleep_for(std::chrono::seconds(2));

		printf("\t\t\tAttempting to set period to 50ms.\n");
		sbapi_continuous_strobe_set_continuous_strobe_period_micros(deviceID,
			cont_strobe_ids[i], &error, 50000);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tDelaying to allow verification.\n");

		std::this_thread::sleep_for(std::chrono::seconds(2));

		printf("\t\t\tAttempting to disable continous strobe.\n");
		sbapi_continuous_strobe_set_continuous_strobe_enable(deviceID,
			cont_strobe_ids[i], &error, 0);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t%d: Finished testing device 0x%02lX, continuous strobe 0x%02lX\n",
			i, deviceID, cont_strobe_ids[i]);
	}
	free(cont_strobe_ids);

	printf("\tFinished testing continuous strobe capabilities.\n");
}


void test_data_buffer_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount)
{
	int error = 0;
	int number_of_data_buffer_features = 0;
	long *data_buffer_ids = 0;
	int i;
	unsigned long capacity = 0;
	unsigned long minCapacity = 0;
	unsigned long maxCapacity = 0;
	unsigned long oldCapacity = 0;
	unsigned long targetCapacity = 0;
	unsigned long count = 0;
	unsigned long newCount = 0;

	printf("\n\tTesting data buffer features:\n");

	printf("\t\tGetting number of data buffer features:\n");
	number_of_data_buffer_features = sbapi_get_number_of_data_buffer_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_data_buffer_features, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_data_buffer_features) 
	{
		printf("\tNo data buffer capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	data_buffer_ids = (long *)calloc(number_of_data_buffer_features, sizeof(long));
	printf("\t\tGetting data buffer feature IDs...\n");
	number_of_data_buffer_features = sbapi_get_data_buffer_features(deviceID, &error,
		data_buffer_ids, number_of_data_buffer_features);
	printf("\t\t\tResult is %d [%s]\n", number_of_data_buffer_features,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_data_buffer_features; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, data buffer 0x%02lX\n",
			i, deviceID, data_buffer_ids[i]);

		printf("\t\t\tAttempting to get minimum buffer size...\n");
		minCapacity = sbapi_data_buffer_get_buffer_capacity_minimum(deviceID, data_buffer_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error) 
		{
			printf("\t\t\t\tMinimum buffer size: %ld\n", minCapacity);
		}
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to get maximum buffer size...\n");
		maxCapacity = sbapi_data_buffer_get_buffer_capacity_maximum(deviceID, data_buffer_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error)
		{
			printf("\t\t\t\tMaximum buffer size: %ld\n", maxCapacity);
		}
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to get current buffer size...\n");
		oldCapacity = sbapi_data_buffer_get_buffer_capacity(deviceID, data_buffer_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error) {
			printf("\t\t\t\tCurrent buffer size: %ld\n", oldCapacity);
		}
		tallyErrors(error, testFailureCount);

		if (oldCapacity != maxCapacity)
		{
			targetCapacity = maxCapacity;
		}
		else if (maxCapacity > minCapacity) 
		{
			/* The current capacity setting is the maximum, so try to back off by one.
			* This test confirms that there is at least some room between min and max.
			*/
			targetCapacity = maxCapacity - 1;
		}
		else
		{
			/* Not possible to change the capacity to anything new */
			targetCapacity = oldCapacity;
		}

		if (targetCapacity != oldCapacity) 
		{
			printf("\t\t\tAttempting to set buffer size to %ld...\n", targetCapacity);
			sbapi_data_buffer_set_buffer_capacity(deviceID, data_buffer_ids[i], &error, targetCapacity);
			printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
			tallyErrors(error, testFailureCount);

			printf("\t\t\tAttempting to get current buffer size...\n");
			capacity = sbapi_data_buffer_get_buffer_capacity(deviceID, data_buffer_ids[i], &error);
			printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
			if (0 == error) 
			{
				printf("\t\t\t\tCurrent buffer size: %ld\n", capacity);
			}
			tallyErrors(error, testFailureCount);
			if (capacity != targetCapacity)
			{
				printf("\t\t\t\tERROR: did not get back the expected value.\n");
				(*testFailureCount)++;
			}
		}

		printf("\t\t\tAttempting to get number of items in the buffer...\n");
		count = sbapi_data_buffer_get_number_of_elements(deviceID, data_buffer_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error)
		{
			printf("\t\t\t\tNumber of items ready for retrieval: %ld\n", count);
		}
		tallyErrors(error, testFailureCount);

		printf("\t\t\tWaiting to allow more data to be buffered...\n");
		/* This assumes that the spectrometer test occurred before this, so the
		* integration time and trigger mode were set in a way that this will
		* keep acquiring a few spectra.
		*/

		std::this_thread::sleep_for(std::chrono::seconds(2));

		printf("\t\t\tAttempting to get number of items in the buffer...\n");
		count = sbapi_data_buffer_get_number_of_elements(deviceID, data_buffer_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error) 
		{
			printf("\t\t\t\tNumber of items ready for retrieval: %ld\n", count);
		}
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to clear buffer...\n");
		sbapi_data_buffer_clear(deviceID, data_buffer_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to get number of items in the buffer...\n");
		newCount = sbapi_data_buffer_get_number_of_elements(deviceID, data_buffer_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error) 
		{
			printf("\t\t\t\tNumber of items ready for retrieval: %ld\n", newCount);
		}
		tallyErrors(error, testFailureCount);

		if (newCount >= count && newCount > 1) 
		{
			/* It is expected that the count would drop to zero, but there is a chance that
			* another scan could be stored between the clear and the query.  This should
			* allow at most one scan in that interval without throwing the error.
			*/
			printf("\t\t\t\tERROR: count did not seem to drop when the buffer was cleared.\n");
			(*testFailureCount)++;
		}

		if (targetCapacity != oldCapacity) 
		{
			printf("\t\t\tAttempting to set buffer size back to %ld...\n", oldCapacity);
			sbapi_data_buffer_set_buffer_capacity(deviceID, data_buffer_ids[i], &error, oldCapacity);
			printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
			tallyErrors(error, testFailureCount);

			printf("\t\t\tAttempting to get current buffer size...\n");
			capacity = sbapi_data_buffer_get_buffer_capacity(deviceID, data_buffer_ids[i], &error);
			printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
			if (0 == error)
			{
				printf("\t\t\t\tCurrent buffer size: %ld\n", capacity);
			}
			if (capacity != oldCapacity)
			{
				printf("\t\t\t\tERROR: did not get back the expected value.\n");
				(*testFailureCount)++;
			}
			tallyErrors(error, testFailureCount);
		}

		printf("\t\t%d: Finished testing device 0x%02lX, data buffer 0x%02lX\n",
			i, deviceID, data_buffer_ids[i]);
	}
	free(data_buffer_ids);

	printf("\tFinished testing data buffer capabilities.\n");
}

void test_fast_buffer_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount)
{
	// assumes one spectrometer id must be obtained, but there is only one data buffer id=0

	int number_of_fast_buffer_features = 0;
	int error = 0;
	long fast_buffer_feature_ids = 0;
	long data_buffer_feature_ids = 0;
	long spectrometerID = 0;
	unsigned char fastBufferEnableState = 0;
	unsigned int numberOfSamplesToTake = 1000;

	printf("\n\tTesting fast buffer features:\n");

	printf("\t\tGetting number of fast buffer features:\n");
	number_of_fast_buffer_features = sbapi_get_number_of_fast_buffer_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_fast_buffer_features, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_fast_buffer_features) 
	{
		printf("\tNo fast buffer capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	printf("\t\t%d: Testing device 0x%02lX, fast buffer 0x%02lX\n", 0, deviceID, 0);

	// this is just to test the command. There is only one fast buffer feature at this point.

	printf("\t\tGetting fast buffer feature IDs...\n");
	number_of_fast_buffer_features = sbapi_get_fast_buffer_features(deviceID, &error, &fast_buffer_feature_ids, 1);
	printf("\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	// id's needed from other features
	if (error == 0)
	{
		int number_of_spectrometers = sbapi_get_spectrometer_features(deviceID, &error, &spectrometerID, 1);
	}

	if (error == 0)
	{
		// there is only one databuffer feature
		int number_of_data_buffers = sbapi_get_data_buffer_features(deviceID, &error, &data_buffer_feature_ids, 1);
	}


	// set up for taking a fast buffered spectrum
	if (error == 0)
	{
		printf("\t\t\tDisable all triggers\n");
		sbapi_spectrometer_set_trigger_mode(deviceID, spectrometerID, &error, 0xFF); 
	}
	
	if (error == 0)
	{
		printf("\t\t\tFast buffering, set buffer to maximum capacity ...\n");

		int maxCapacity = sbapi_data_buffer_get_buffer_capacity_maximum(deviceID, data_buffer_feature_ids, &error);
		if (0 == error)
		{
			sbapi_data_buffer_set_buffer_capacity(deviceID, data_buffer_feature_ids, &error, maxCapacity);
			if (0 == error)
			{
				printf("\t\t\t\tFast buffer capacity was set to: %d\n", maxCapacity);
			}
		}
	}

	if (error == 0)
	{
		printf("\t\t\tSetting integration time to 1ms\n");
		sbapi_spectrometer_set_integration_time_micros(deviceID, spectrometerID, &error, 1000000/numberOfSamplesToTake);
	}

	if (error == 0)
	{
		printf("\t\t\tSetting consecutive sample count to 1000:\n");
		numberOfSamplesToTake = 1000;
		sbapi_fast_buffer_set_consecutive_sample_count(deviceID, fast_buffer_feature_ids, &error, numberOfSamplesToTake);
		if (error == 0)
		{
			unsigned int sampleCount = sbapi_fast_buffer_get_consecutive_sample_count(deviceID, fast_buffer_feature_ids, &error);
			if (error == 0)
			{
				if (sampleCount != numberOfSamplesToTake)
					error = ERROR_VALUE_NOT_EXPECTED;
			}
		}
	}

	if (0 == error)
	{
		printf("\t\t\t\tEnable fast buffer.\n");
		sbapi_fast_buffer_set_buffering_enable(deviceID, fast_buffer_feature_ids, &error, 1);
		if (error == 0)
		{
			unsigned char bufferEnabled = sbapi_fast_buffer_get_buffering_enable(deviceID, fast_buffer_feature_ids, &error);
			if (error == 0)
			{
				if (bufferEnabled != 1)
					error = ERROR_VALUE_NOT_EXPECTED;
			}
		}
	}

	if (error == 0)
	{
		printf("\t\t\tClear the spectrum buffer\n");
		sbapi_data_buffer_clear(deviceID, data_buffer_feature_ids, &error);
	}

	if (error == 0)
	{
		unsigned int numberOfSamplesToRetrieve = 15;

		// Flame Fx is the only spectrometer using this right now, so the pixel size is a short. However, in the future 
		// some way for the user to know what size integer is used for a pixel. That is probably another awkward length call for
		//  a fast spectrum that returns the total number of bytes.
		int pixelCount = sbapi_spectrometer_get_formatted_spectrum_length(deviceID, spectrometerID, &error); // this is really just the pixel count
		int dataMaxLength = (((pixelCount*sizeof(unsigned short)) + 32 ) * numberOfSamplesToRetrieve); // 32 is the metadata.
		std::vector<byte> *dataBuffer = new std::vector<byte>(dataMaxLength);

		unsigned int checksum = 0;
		if (error == 0)
		{
			printf("\t\t\tTake three spectral consecutive sample scans \n");


			for (int i = 0; i < 3; i++)
			{
				printf("\t\t\t\tScan %d\n", i);

				if (0 == error)
				{
					printf("\t\t\tClear the spectrum buffer\n");
					sbapi_data_buffer_clear(deviceID, data_buffer_feature_ids, &error);
				}

				if (error == 0)
				{

					printf("\t\t\tSet trigger to free running to allow a trigger\n");
					sbapi_spectrometer_set_trigger_mode(deviceID, spectrometerID, &error, 0x00); // trigger id = 0
				}

				if (error == 0)
				{
					printf("\t\t\tSend get spectrum to start sampling\n");
					// send a get fast buffered spectrum request only for a trigger, buffer should be clear
					// when that is true, no spectra are returned, but a new sample is triggered
					int bytesReturned = sbapi_spectrometer_get_fast_buffer_spectrum(deviceID, spectrometerID, &error, dataBuffer->data(), dataMaxLength, 0);
				}

				// disable all triggers  
				if (error == 0)
				{
					printf("\t\t\tSet trigger to disabled to prevent triggering when fetching samples\n");
					sbapi_spectrometer_set_trigger_mode(deviceID, spectrometerID, &error, 0xFF); // trigger id = 0
				}

				printf("\t\t\tWaiting to allow data to be buffered...\n");
				/* This assumes that the spectrometer test occurred before this, so the
				* integration time and trigger mode were set in a way that this will
				* keep acquiring a few spectra.
				*/

				std::this_thread::sleep_for(std::chrono::seconds(2));

				// call get fast buffered spectrum until the buffer is empty
				while (error == 0)
				{
					unsigned int retrieveCount = numberOfSamplesToRetrieve;
					unsigned int spectraInBuffer = sbapi_data_buffer_get_number_of_elements(deviceID, data_buffer_feature_ids, &error);
					if (spectraInBuffer < numberOfSamplesToRetrieve)
						retrieveCount = spectraInBuffer;

					int bytesReturned = sbapi_spectrometer_get_fast_buffer_spectrum(deviceID, spectrometerID, &error, dataBuffer->data(), dataMaxLength, retrieveCount);
					printf("\t\t\tSpectra in Buffer: %d\tSpectrum Count: %d\tTimestamp: %d\n", spectraInBuffer, *reinterpret_cast<unsigned int*>(&(*dataBuffer)[24]), *reinterpret_cast<unsigned long*>(&(*dataBuffer)[8]));
					if (spectraInBuffer == 0)
						break;
				}

				if (error != 0)
				{
					break;
				}
			}
		}
		delete dataBuffer;
	}

	if (0 == error)
	{
		printf("\t\t\t\tDisable fast buffer.\n");
		sbapi_fast_buffer_set_buffering_enable(deviceID, fast_buffer_feature_ids, &error, 0);
	}

	if(0==error)
	{
		printf("\t\t\tClear the spectrum buffer\n");
		sbapi_data_buffer_clear(deviceID, data_buffer_feature_ids, &error);
	}

	if (error == 0)
	{
		printf("\t\t\tSet trigger to free running\n");
		sbapi_spectrometer_set_trigger_mode(deviceID, spectrometerID, &error, 0x00); // trigger id = 0
	}

	printf("\t\t\t\tResult code is [%s]\n", sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);


	printf("\t\t%d: Finished testing device 0x%02lX, fast buffer 0x%02lX\n", 0, deviceID, 0);
	printf("\tFinished testing fast buffer capabilities.\n");
}


void test_acquisition_delay_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount)
{
	int error = 0;
	int number_of_acq_delays = 0;
	long *acq_delay_ids = 0;
	int i;
	unsigned long delay_min;
	unsigned long delay_max;
	unsigned long delay_inc;
	unsigned long old_delay;
	unsigned long new_delay;
	unsigned long temp;

	printf("\n\tTesting acquisition delay features:\n");

	printf("\t\tGetting number of acquisition delay features:\n");
	number_of_acq_delays = sbapi_get_number_of_acquisition_delay_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_acq_delays,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_acq_delays) 
	{
		printf("\tNo acquisition delay capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	acq_delay_ids = (long *)calloc(number_of_acq_delays, sizeof(long));
	printf("\t\tGetting acquisition delay feature IDs...\n");
	number_of_acq_delays = sbapi_get_acquisition_delay_features(deviceID, &error,
		acq_delay_ids, number_of_acq_delays);
	printf("\t\t\tResult is %d [%s]\n", number_of_acq_delays,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_acq_delays; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, acquisition delay 0x%02lX\n",
			i, deviceID, acq_delay_ids[i]);

		printf("\t\t\tAttempting to get minimum delay...\n");
		delay_min = sbapi_acquisition_delay_get_delay_minimum_microseconds(deviceID, acq_delay_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error)
		{
			printf("\t\t\t\tMinimum acquisition delay: %ld usec\n", delay_min);
		}
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to get maximum delay...\n");
		delay_max = sbapi_acquisition_delay_get_delay_maximum_microseconds(deviceID, acq_delay_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error)
		{
			printf("\t\t\t\tMaximum acquisition delay: %ld usec\n", delay_max);
		}
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to get delay increment...\n");
		delay_inc = sbapi_acquisition_delay_get_delay_increment_microseconds(deviceID, acq_delay_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error)
		{
			printf("\t\t\t\tAcquisition delay increment: %ld usec\n", delay_inc);
		}
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to get current acquisition delay...\n");
		old_delay = sbapi_acquisition_delay_get_delay_microseconds(deviceID, acq_delay_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error)
		{
			printf("\t\t\t\tAcquisition delay: %ld usec\n", old_delay);
		}
		else 
		{
			printf("\t\t\t\tAcquisition delay read may cause an error if never previously written; moving on\n");
		}

		if (old_delay != delay_max) 
		{
			new_delay = delay_max;
		}
		else if (delay_min < delay_max)
		{
			new_delay = delay_max - delay_inc;
		}
		else
		{
			new_delay = old_delay;
		}

		printf("\t\t\tAttempting to set acquisition delay to %ld usec...\n", new_delay);
		sbapi_acquisition_delay_set_delay_microseconds(deviceID, acq_delay_ids[i], &error, new_delay);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to get current acquisition delay...\n");
		temp = sbapi_acquisition_delay_get_delay_microseconds(deviceID, acq_delay_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error) 
		{
			printf("\t\t\t\tAcquisition delay: %ld usec\n", temp);
		}
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to set acquisition delay back to %ld usec...\n", old_delay);
		sbapi_acquisition_delay_set_delay_microseconds(deviceID, acq_delay_ids[i], &error, old_delay);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to get current acquisition delay...\n");
		temp = sbapi_acquisition_delay_get_delay_microseconds(deviceID, acq_delay_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error)
		{
			printf("\t\t\t\tAcquisition delay: %ld usec\n", temp);
		}
		tallyErrors(error, testFailureCount);
	}
	free(acq_delay_ids);

	printf("\tFinished testing acquisition delay capabilities\n");
}

void test_pixel_binning_feature(long deviceID, int *unsupportedFeatureCount, int *testFailureCount)
{

	int error = 0;
	int number_of_pixel_binning;
	long *pixel_binning_ids;
	int i;
	unsigned char max_binning;
	unsigned char factory_default_pixel_binning;
	unsigned char factor;
	unsigned char f;
	unsigned char check_default;

	printf("\n\tTesting pixel binning features:\n");

	printf("\t\tGetting number of pixel binning features:\n");
	number_of_pixel_binning = sbapi_get_number_of_pixel_binning_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_pixel_binning,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_pixel_binning)
	{
		printf("\tNo pixel binning capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);

		return;
	}

	pixel_binning_ids = (long *)calloc(number_of_pixel_binning, sizeof(long));
	printf("\t\tGetting acquisition delay feature IDs...\n");
	number_of_pixel_binning = sbapi_get_pixel_binning_features(deviceID, &error,
		pixel_binning_ids, number_of_pixel_binning);
	printf("\t\t\tResult is %d [%s]\n", number_of_pixel_binning,
		sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_pixel_binning; i++)
	{
		printf("\t\t\tAttempting to get maximum pixel binning factor...\n");
		max_binning = sbapi_binning_get_max_pixel_binning_factor(deviceID, pixel_binning_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error)
		{
			printf("\t\t\t\tMaximum pixel binning: %d\n", max_binning);
		}
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to set pixel binning factor too high...\n");
		sbapi_binning_set_pixel_binning_factor(deviceID, pixel_binning_ids[i], &error, max_binning + 1);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error)
		{
			printf("\t\t\t\tERROR: set the pixel binning too high.\n");
			++testFailureCount;
		}
		error = 0;
		tallyErrors(error, testFailureCount);

		/* Remember the default pixel binning for the end of the test. We are
		* assuming that the spectrometer has the factory default pixel binning at this point.
		*/
		factory_default_pixel_binning = sbapi_binning_get_default_pixel_binning_factor(deviceID, pixel_binning_ids[i], &error);

		for (factor = max_binning; factor <= max_binning && factor >= 0; --factor)
		{
			printf("\t\t\tAttempting to set pixel binning factor...\n");
			sbapi_binning_set_pixel_binning_factor(deviceID, pixel_binning_ids[i], &error, factor);
			printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
			if (0 == error)
			{
				printf("\t\t\t\tPixel binning set to: %d\n", factor);
			}
			tallyErrors(error, testFailureCount);

			// TODO check that the number of wavelengths and intensities returned are adjusted

			printf("\t\t\tAttempting to get pixel binning factor...\n");
			f = sbapi_binning_get_pixel_binning_factor(deviceID, pixel_binning_ids[i], &error);
			printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
			if (0 == error)
			{
				printf("\t\t\t\tPixel binning returned: %d\n", f);
			}
			if (f != factor)
			{
				printf("\t\t\t\tERROR: pixel binning factor returned does not match the set value.\n");
				++testFailureCount;
			}
			tallyErrors(error, testFailureCount);

			printf("\t\t\tAttempting to set the default pixel binning factor...\n");
			sbapi_binning_set_default_pixel_binning_factor(deviceID, pixel_binning_ids[i], &error, factor);
			printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
			if (0 == error)
			{
				printf("\t\t\t\tDefault pixel binning set to: %d\n", factor);
			}
			tallyErrors(error, testFailureCount);

			printf("\t\t\tAttempting to get the default pixel binning factor...\n");
			f = sbapi_binning_get_default_pixel_binning_factor(deviceID, pixel_binning_ids[i], &error);
			printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
			if (0 == error)
			{
				printf("\t\t\t\tDefault pixel binning returned: %d\n", f);
			}
			if (f != factor) 
			{
				printf("\t\t\t\tERROR: default pixel binning factor returned does not match the set value.\n");
				++testFailureCount;
			}
			tallyErrors(error, testFailureCount);
		}
		printf("\t\t\tAttempting to reset the factory default pixel binning factor...\n");
		sbapi_binning_reset_default_pixel_binning_factor(deviceID, pixel_binning_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error)
		{
			printf("\t\t\t\tReset the default pixel binning factor\n\n");
		}
		tallyErrors(error, testFailureCount);

		printf("\t\t\tChecking the factory default pixel binning factor...\n");
		check_default = sbapi_binning_get_default_pixel_binning_factor(deviceID, pixel_binning_ids[i], &error);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (0 == error) 
		{
			printf("\t\t\t\tGet the default pixel binning factor: %d\n\n", check_default);
		}
		tallyErrors(error, testFailureCount);
		if (check_default != factory_default_pixel_binning) 
		{
			printf("\t\t\t\tERROR: default pixel binning factor returned does not match the factory value.\n");
			++testFailureCount;
		}
		tallyErrors(error, testFailureCount);
	}
	free(pixel_binning_ids);

	printf("\tFinished testing pixel binning capabilities\n");
}

void test_networking_features(long deviceID, int *unsupportedFeatureCount, int *testFailureCount)
{
	int error = 0;
	int number_of_network_configurations;
	long *network_configuration_ids = 0;
	int i;
	unsigned char numberOfNetworkInterfaces;

	printf("\n\tTesting networking features:\n");

	printf("\t\tExercising network configuration commands:\n");
	printf("\t\tRetrieve number of network configuration features:\n");
	number_of_network_configurations = sbapi_get_number_of_network_configuration_features(deviceID, &error);
	printf("\t\tResult is %d  [%s]\n", number_of_network_configurations, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_network_configurations)
	{
		printf("\tNo network capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);
		return;
	}

	network_configuration_ids = (long *)calloc(number_of_network_configurations, sizeof(long));
	printf("\t\tGetting network configuration feature IDs...\n");
	number_of_network_configurations = sbapi_get_network_configuration_features(deviceID, &error, network_configuration_ids, number_of_network_configurations);
	printf("\t\t\tResult is %d [%s]\n", number_of_network_configurations, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_network_configurations; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, network feature id 0x%02lX\n", i, deviceID, network_configuration_ids[i]);

		printf("\t\t\tAttempting to get the number of network interfaces...\n");
		numberOfNetworkInterfaces = sbapi_network_configuration_get_interface_count(deviceID, network_configuration_ids[i], &error);
		printf("\t\t\tResult is %d [%s]\n", numberOfNetworkInterfaces, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		for (unsigned char networkInterfaceIndex = 0; networkInterfaceIndex < numberOfNetworkInterfaces; networkInterfaceIndex++)
		{
			printf("\t\t\tInterface Index = %d\n", networkInterfaceIndex);
			// get interface type
			printf("\t\t\tAttempting to retrieve the interface type...\n");
			unsigned char interfaceType = sbapi_network_configuration_get_interface_connection_type(deviceID, network_configuration_ids[i], &error, networkInterfaceIndex);
			std::string typeName = "";
			
			switch (interfaceType)
			{
				case 0:
					typeName = "Loopback";
					break;
				case 1:
					typeName = "Wired Ethernet";
					break;
				case 2:
					typeName = "WiFi";
					break;
				case 3:
					typeName = "CDC Ethernet (USB)";
					break;
				default:
					typeName = "Undefined";
					break;
			}			
			printf("\n>>>\t\t\tResult is %d [%s]: %s\n\n", interfaceType, sbapi_get_error_string(error), typeName.data());
			tallyErrors(error, testFailureCount);
			
			// switch interface enable state
			printf("\t\t\tAttempting to retrieve the interface enable state...\n");
			unsigned char interfaceEnableState1 = sbapi_network_configuration_get_interface_enable_status(deviceID, network_configuration_ids[i], &error, networkInterfaceIndex);
			printf("\t\t\tResult is %d [%s]\n", interfaceEnableState1, sbapi_get_error_string(error));
			tallyErrors(error, testFailureCount);

			if (error == 0)
			{
				unsigned char interfaceEnableState2 = interfaceEnableState1;
				printf("\t\t\tAttempting to switch the interface enable state. (This can take 15 seconds or so.)\n");
				if (interfaceEnableState2 == 0)
				{
					interfaceEnableState2 = 1;
					printf("\t\t\t\tInterface was disabled.\n");
				}
				else
				{
					interfaceEnableState2 = 0;
					printf("\t\t\t\tInterface was enabled.\n");
				}

				sbapi_network_configuration_set_interface_enable_status(deviceID, network_configuration_ids[i], &error, networkInterfaceIndex, interfaceEnableState2);
				Sleep(4000);
				tallyErrors(error, testFailureCount);
				if (error == 0)
				{
					printf("\t\t\tThe interface enable state was successfully modified.\n");
				}
				else
				{
					printf("\t\t\tThe interface enable state did not change. sbapi error = %s\n", sbapi_get_error_string(error));
				}
				
				
				
				// set interface enable state to true
				printf("\t\t\tAttempting to set the interface enable state to true. (This can take 15 seconds or so.)\n");
				sbapi_network_configuration_set_interface_enable_status(deviceID, network_configuration_ids[i], &error, networkInterfaceIndex, 1);
				Sleep(4000);
				printf("\t\t\tResult is[%s]\n", sbapi_get_error_string(error));
				tallyErrors(error, testFailureCount);

				if (error == 0)
				{
					switch(interfaceType)
					{
					case 0: // loopback
						test_ethernet_features(deviceID, unsupportedFeatureCount, testFailureCount, networkInterfaceIndex);
						test_ipv4_features(deviceID, unsupportedFeatureCount, testFailureCount, networkInterfaceIndex);
						test_multicast_features(deviceID, unsupportedFeatureCount, testFailureCount, networkInterfaceIndex);
						break;
					case 1: // wired ethernet
						// test gigabit ethernet configuration commands
						test_ethernet_features(deviceID, unsupportedFeatureCount, testFailureCount, networkInterfaceIndex);
						test_ipv4_features(deviceID, unsupportedFeatureCount, testFailureCount, networkInterfaceIndex);
						test_multicast_features(deviceID, unsupportedFeatureCount, testFailureCount, networkInterfaceIndex);
						break;
					case 2: // wifi
						test_wifi_features(deviceID, unsupportedFeatureCount, testFailureCount, networkInterfaceIndex);
						test_dhcp_server_features(deviceID, unsupportedFeatureCount, testFailureCount, networkInterfaceIndex);
						test_ipv4_features(deviceID, unsupportedFeatureCount, testFailureCount, networkInterfaceIndex);
						test_multicast_features(deviceID, unsupportedFeatureCount, testFailureCount, networkInterfaceIndex);
						break;
					case 3: // CDC ethernet (USB)

						break;
					default:

						break;
					}

#ifdef RUN_SELF_TEST

					// run self test
					printf("\t\t\tAttempting to run a network interface selftest. This can take up to two minutes to execute. Be patient.\n");
					unsigned char myResult = sbapi_network_configuration_run_interface_self_test(deviceID, network_configuration_ids[i], &error, networkInterfaceIndex);
					std::string selftestResult = "Failed";
					if (myResult == 1)
						selftestResult = "Passed";
					printf("\t\t\tSelf test result: %s [%s]\n", selftestResult.data(), sbapi_get_error_string(error));
					tallyErrors(error, testFailureCount);
#endif

				}



				// save interface settings
				printf("\t\t\tAttempting to save the interface settings\n");
				sbapi_network_configuration_save_interface_settings(deviceID, network_configuration_ids[i], &error, networkInterfaceIndex);
				printf("\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
				tallyErrors(error, testFailureCount);
			}


		}
		printf("\t\t%d: Finished testing device 0x%02lX, network configuration feature 0x%02lX\n", i, deviceID, network_configuration_ids[i]);
	}

	free(network_configuration_ids);

	printf("\tFinished testing network feature capabilities.\n");
}

void test_miscellaneous_commands(long deviceID, int *unsupportedFeatureCount, int *testFailureCount)
{
	printf("\n\tTesting miscellaneous commands:\n");
	char ipAddress[] = "192.168.1.1";
	int port = 54321;

	int result = sbapi_add_TCPIPv4_device_location("FLAMEX", ipAddress, port);
	printf("\tFinished testing miscellaneous commands. \n");
}

void test_ethernet_features(long deviceID, int *unsupportedFeatureCount, int *testFailureCount, unsigned char networkInterfaceIndex)
{
	int error = 0;
	int number_of_ethernet_configurations;
	long *ethernet_configuration_ids = 0;
	int i;

	printf("\n\tTesting ethernet configuration features for network interface index = %d\n", networkInterfaceIndex);

	printf("\t\tExercising ethernet configuration commands:\n");
	number_of_ethernet_configurations = sbapi_get_number_of_ethernet_configuration_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_ethernet_configurations, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_ethernet_configurations)
	{
		printf("\tNo ethernet capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);
		return;
	}

	ethernet_configuration_ids = (long *)calloc(number_of_ethernet_configurations, sizeof(long));
	printf("\t\tGetting ethernet configuration feature IDs...\n");
	number_of_ethernet_configurations = sbapi_get_ethernet_configuration_features(deviceID, &error, ethernet_configuration_ids, number_of_ethernet_configurations);
	printf("\t\t\tResult is %d [%s]\n", number_of_ethernet_configurations, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_ethernet_configurations; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, ethernet feature id 0x%02lX\n", i, deviceID, ethernet_configuration_ids[i]);
		printf("\t\t\tInterface Index = %d\n", networkInterfaceIndex);

		// switch GbE enable state
		printf("\t\t\tAttempting to retrieve the GbE enable state...\n");
		unsigned char ethernetEnableState1 = sbapi_ethernet_configuration_get_gbe_enable_status(deviceID, ethernet_configuration_ids[i], &error, networkInterfaceIndex);
		printf("\t\t\tResult is %d [%s]\n", ethernetEnableState1, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		unsigned char ethernetEnableState2 = ethernetEnableState1;
		printf("\t\t\tAttempting to switch the GbE enable state...\n");
		if (ethernetEnableState2 == 0)
		{
			ethernetEnableState2 = 1;
			printf("\t\t\t\tGbE was disabled.\n");
		}
		else
		{
			ethernetEnableState2 = 0;
			printf("\t\t\t\tGbE was enabled.\n");
		}

		sbapi_ethernet_configuration_set_gbe_enable_status(deviceID, ethernet_configuration_ids[i], &error, networkInterfaceIndex, ethernetEnableState2);
		tallyErrors(error, testFailureCount);
		if (error == 0)
		{
			printf("\t\t\tThe GbE enable state was successfully modified.\n");
		}
		else
		{
			printf("\t\t\tThe GTbE enable state did not change. sbapi error = %s\n", sbapi_get_error_string(error));
		}


		// set GbE enable state to true
		printf("\t\t\tAttempting to set the GbE enable state to true\n");
		sbapi_ethernet_configuration_set_gbe_enable_status(deviceID, ethernet_configuration_ids[i], &error, networkInterfaceIndex, 1);
		printf("\t\t\tResult is[%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		unsigned char aMacAddress[6];

		// get the mac address
		printf("\t\t\tAttempting to retrieve the MAC address...\n");
		sbapi_ethernet_configuration_get_mac_address(deviceID, ethernet_configuration_ids[i], &error, networkInterfaceIndex, &aMacAddress);
			
		printf("\t\t\tResult is %x:%x:%x:%x:%x:%x [%s]\n", aMacAddress[0], aMacAddress[1], aMacAddress[2], 
			aMacAddress[3], aMacAddress[4], aMacAddress[5], sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		// set the same mac address and just check the error code
		printf("\t\t\tAttempting to set the MAC address to the same value...\n");
		sbapi_ethernet_configuration_get_mac_address(deviceID, ethernet_configuration_ids[i], &error, networkInterfaceIndex, &aMacAddress);

		printf("\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);


		printf("\t\t%d: Finished testing device 0x%02lX, ethernet configuration feature 0x%02lX\n", i, deviceID, ethernet_configuration_ids[i]);
	}

	free(ethernet_configuration_ids);

	printf("\tFinished testing ethernet feature capabilities.\n");
}

void test_dhcp_server_features(long deviceID, int *unsupportedFeatureCount, int *testFailureCount, unsigned char networkInterfaceIndex)
{
	int error = 0;
	int number_of_dhcp_server_features;
	long *dhcp_server_ids = 0;
	int i;

	printf("\n\tTesting dhcp server configuration features for network interface index = %d\n", networkInterfaceIndex);

	printf("\t\tExercising dhcp server commands:\n");
	number_of_dhcp_server_features = sbapi_get_number_of_dhcp_server_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_dhcp_server_features, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_dhcp_server_features)
	{
		printf("\tNo dhcp server capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);
		return;
	}

	dhcp_server_ids = (long *)calloc(number_of_dhcp_server_features, sizeof(long));
	printf("\t\tGetting dhcp server feature IDs...\n");
	number_of_dhcp_server_features = sbapi_get_dhcp_server_features(deviceID, &error, dhcp_server_ids, number_of_dhcp_server_features);
	printf("\t\t\tResult is %d [%s]\n", number_of_dhcp_server_features, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_dhcp_server_features; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, dhcp server feature id 0x%02lX\n", i, deviceID, dhcp_server_ids[i]);
		printf("\t\t\tInterface Index = %d\n", networkInterfaceIndex);

		// switch dhcp server enable state
		printf("\t\t\tAttempting to retrieve the dhcp server enable state...\n");
		unsigned char dhcpServerEnableState1 = sbapi_dhcp_server_get_enable_state(deviceID, dhcp_server_ids[i], &error, networkInterfaceIndex);
		printf("\t\t\tResult is %d [%s]\n", dhcpServerEnableState1, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		unsigned char dhcpServerEnableState2 = dhcpServerEnableState1;
		printf("\t\t\tAttempting to switch the dhcp server enable state...\n");
		if (dhcpServerEnableState2 == 0)
		{
			dhcpServerEnableState2 = 1;
			printf("\t\t\t\tThe DHCP server was disabled.\n");
		}
		else
		{
			dhcpServerEnableState2 = 0;
			printf("\t\t\t\tThe DHCP server was enabled.\n");
		}

		sbapi_dhcp_server_set_enable_state(deviceID, dhcp_server_ids[i], &error, networkInterfaceIndex, dhcpServerEnableState2);
		tallyErrors(error, testFailureCount);
		if (error == 0)
		{
			printf("\t\t\tThe dhcp server enable state was successfully modified.\n");
		}
		else
		{
			printf("\t\t\tThe dhcp server enable state did not change. sbapi error = %s\n", sbapi_get_error_string(error));
		}


		// set dhcp server enable state to false
		printf("\t\t\tAttempting to set the dhcp server enable state to false\n");
		sbapi_dhcp_server_set_enable_state(deviceID, dhcp_server_ids[i], &error, networkInterfaceIndex, 0);
		printf("\t\t\tResult is[%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		unsigned char dhcpServerAddress[4];
		unsigned char netMask = 0;

		// get the dhcp server address
		printf("\t\t\tAttempting to retrieve the dhcp server address...\n");
		sbapi_dhcp_server_get_address(deviceID, dhcp_server_ids[i], &error, networkInterfaceIndex, &dhcpServerAddress, &netMask);

		printf("\t\t\tResult is %d.%d.%d.%d/%d [%s]\n", dhcpServerAddress[0], dhcpServerAddress[1], dhcpServerAddress[2],
			dhcpServerAddress[3], netMask, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		// set the same dhcp server address and just check the error code
		printf("\t\t\tAttempting to set the dhcp server address to the same value...\n");
		sbapi_dhcp_server_get_address(deviceID, dhcp_server_ids[i], &error, networkInterfaceIndex, &dhcpServerAddress, &netMask);

		printf("\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t%d: Finished testing device 0x%02lX, dhcp server feature 0x%02lX\n", i, deviceID, dhcp_server_ids[i]);
	}

	free(dhcp_server_ids);

	printf("\tFinished testing dhcp server feature capabilities.\n");
}

void test_ipv4_features(long deviceID, int *unsupportedFeatureCount, int *testFailureCount, unsigned char networkInterfaceIndex)
{
	int error = 0;
	int number_of_ipv4_features;
	long *ipv4_feature_ids = 0;
	int i;

	printf("\n\tTesting ipv4 features for network interface index = %d\n", networkInterfaceIndex);

	printf("\t\tExercising ipv4 commands:\n");
	number_of_ipv4_features = sbapi_get_number_of_ipv4_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_ipv4_features, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_ipv4_features)
	{
		printf("\tNo ipv4 capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);
		return;
	}

	ipv4_feature_ids = (long *)calloc(number_of_ipv4_features, sizeof(long));
	printf("\t\tGetting ipv4 server feature IDs...\n");
	number_of_ipv4_features = sbapi_get_ipv4_features(deviceID, &error, ipv4_feature_ids, number_of_ipv4_features);
	printf("\t\t\tResult is %d [%s]\n", number_of_ipv4_features, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_ipv4_features; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, ipv4 feature id 0x%02lX\n", i, deviceID, ipv4_feature_ids[i]);
		printf("\t\t\tInterface Index = %d\n", networkInterfaceIndex);

		// switch dhcp client enable state
		printf("\t\t\tAttempting to retrieve the dhcp client enable state...\n");
		unsigned char dhcpEnableState1 = sbapi_ipv4_get_dhcp_enable_state(deviceID, ipv4_feature_ids[i], &error, networkInterfaceIndex);
		printf("\t\t\tResult is %d [%s]\n", dhcpEnableState1, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		unsigned char dhcpEnableState2 = dhcpEnableState1;
		printf("\t\t\tAttempting to switch the dhcp client enable state...\n");
		if (dhcpEnableState2 == 0)
		{
			dhcpEnableState2 = 1;
			printf("\t\t\t\tThe dhcp client was disabled.\n");
		}
		else
		{
			dhcpEnableState2 = 0;
			printf("\t\t\t\tThe dhcp client was enabled.\n");
		}

		sbapi_ipv4_set_dhcp_enable_state(deviceID, ipv4_feature_ids[i], &error, networkInterfaceIndex, dhcpEnableState2);
		tallyErrors(error, testFailureCount);
		if (error == 0)
		{
			printf("\t\t\tThe dhcp client enable state was successfully modified.\n");
		}
		else
		{
			printf("\t\t\tThe dhcp client enable state did not change. sbapi error = %s\n", sbapi_get_error_string(error));
		}


		// set dhcp server enable state to false
		printf("\t\t\tAttempting to set the dhcp client enable state to true\n");
		sbapi_ipv4_set_dhcp_enable_state(deviceID, ipv4_feature_ids[i], &error, networkInterfaceIndex, 1);
		printf("\t\t\tResult is[%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		unsigned char ipv4GatewayAddress[4];

		// get the dhcp gateway address
		printf("\t\t\tAttempting to retrieve the default gateway address...\n");
		sbapi_ipv4_get_default_gateway_address(deviceID, ipv4_feature_ids[i], &error, networkInterfaceIndex, &ipv4GatewayAddress);

		printf("\t\t\tResult is %d.%d.%d.%d[%s]\n", ipv4GatewayAddress[0], ipv4GatewayAddress[1], ipv4GatewayAddress[2],
			ipv4GatewayAddress[3], sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		// set the same default gateway address and just check the error code
		printf("\t\t\tAttempting to set the default gateway address to the same value...\n");
		sbapi_ipv4_set_default_gateway_address(deviceID, ipv4_feature_ids[i], &error, networkInterfaceIndex, ipv4GatewayAddress);

		printf("\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);


		unsigned char ipv4NumberOfAddresses = 0;

		// get the number of ipv4 addresses
		printf("\t\t\tAttempting to retrieve the number of ipv4 addresses...\n");
		ipv4NumberOfAddresses = sbapi_ipv4_get_number_of_addresses(deviceID, ipv4_feature_ids[i], &error, networkInterfaceIndex);

		printf("\t\t\tResult is %d [%s]\n", ipv4NumberOfAddresses, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to list the ipv4 addresses...\n");
		if (error == 0)
		{
			for (unsigned char addressIndex = 0; addressIndex < ipv4NumberOfAddresses; addressIndex++)
			{
				unsigned char ipv4Address[4];
				unsigned char netMask = 0;

				// get the dhcp server address
				printf("\t\t\t\tAttempting to retrieve the dhcp server address...\n");
				sbapi_ipv4_get_address(deviceID, ipv4_feature_ids[i], &error, networkInterfaceIndex, addressIndex, (&ipv4Address), &netMask);

				printf("\t\t\t\tResult is addressIndex = %d, ipv4 address =  %d.%d.%d.%d/%d [%s]\n", addressIndex, ipv4Address[0], ipv4Address[1], 
					ipv4Address[2], ipv4Address[3], netMask, sbapi_get_error_string(error));
				tallyErrors(error, testFailureCount);

				if (error != 0)
					break;
			}
		}

		unsigned char ipv4Address[4] = { 192, 168, 1, 69 };
		unsigned char netMask = 24;

		// add and delete an ipv4 address
		printf("\t\t\tAttempting to add an ipv4 address...\n");
		printf("\t\t\t\tNumber of addresses = %d...\n", ipv4NumberOfAddresses);

		sbapi_ipv4_add_address(deviceID, ipv4_feature_ids[i], &error, networkInterfaceIndex, ipv4Address, netMask);
		printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		if (error == 0)
		{
			ipv4NumberOfAddresses = sbapi_ipv4_get_number_of_addresses(deviceID, ipv4_feature_ids[i], &error, networkInterfaceIndex);
			printf("\t\t\t\tNumber of addresses = %d...\n", ipv4NumberOfAddresses);

			sbapi_ipv4_get_address(deviceID, ipv4_feature_ids[i], &error, networkInterfaceIndex, ipv4NumberOfAddresses-1, &ipv4Address, &netMask);
			printf("\t\t\t\tAdded ipv4 address %d.%d.%d.%d/%d [%s]\n", ipv4Address[0], ipv4Address[1], ipv4Address[2], ipv4Address[3], netMask, sbapi_get_error_string(error));

			printf("\t\t\tAttempting to delete the recently added ipv4 address...\n");
			sbapi_ipv4_delete_address(deviceID, ipv4_feature_ids[i], &error, networkInterfaceIndex, ipv4NumberOfAddresses-1);
			printf("\t\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
			
			ipv4NumberOfAddresses = sbapi_ipv4_get_number_of_addresses(deviceID, ipv4_feature_ids[i], &error, networkInterfaceIndex);
			printf("\t\t\t\tNumber of addresses = %d...\n", ipv4NumberOfAddresses);
		}

		printf("\t\t%d: Finished testing device 0x%02lX, ipv4 feature 0x%02lX\n", i, deviceID, ipv4_feature_ids[i]);
	}

	free(ipv4_feature_ids);

	printf("\tFinished testing ipv4 feature capabilities.\n");
}

void test_multicast_features(long deviceID, int *unsupportedFeatureCount, int *testFailureCount, unsigned char networkInterfaceIndex)
{
	int error = 0;
	int number_of_multicast_features;
	long *multicast_feature_ids = 0;
	int i;

	printf("\n\tTesting multicast features for network interface index = %d\n", networkInterfaceIndex);

	printf("\t\tExercising multicast commands:\n");
	number_of_multicast_features = sbapi_get_number_of_multicast_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_multicast_features, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_multicast_features)
	{
		printf("\tNo multicast capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);
		return;
	}

	multicast_feature_ids = (long *)calloc(number_of_multicast_features, sizeof(long));
	printf("\t\tGetting multicast feature IDs...\n");
	number_of_multicast_features = sbapi_get_multicast_features(deviceID, &error, multicast_feature_ids, number_of_multicast_features);
	printf("\t\t\tResult is %d [%s]\n", number_of_multicast_features, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_multicast_features; i++)
	{
		printf("\t\t%d: Testing device 0x%02lX, multicast feature id 0x%02lX\n", i, deviceID, multicast_feature_ids[i]);
		printf("\t\t\tInterface Index = %d\n", networkInterfaceIndex);

		unsigned char groupAddress[4];
		unsigned short port;

		// these functions are for convenience. They do not contact the spectrometer
		sbapi_multicast_get_group_address(deviceID, multicast_feature_ids[i], &error, networkInterfaceIndex, &groupAddress);
		port = sbapi_multicast_get_group_port(deviceID, multicast_feature_ids[i], &error, networkInterfaceIndex);
		printf("\t\t\tMulticast Address = %d.%d.%d.%d, Port = %d\n", groupAddress[0], groupAddress[1], groupAddress[2], groupAddress[3], port);

		// switch multicast enable state
		printf("\t\t\tAttempting to retrieve the multicast enable state...\n");
		unsigned char multicastEnableState1 = sbapi_multicast_get_enable_state(deviceID, multicast_feature_ids[i], &error, networkInterfaceIndex);
		printf("\t\t\tResult is %d [%s]\n", multicastEnableState1, sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		unsigned char multicastEnableState2= multicastEnableState1;
		printf("\t\t\tAttempting to switch multicast enable state...\n");
		if (multicastEnableState2 == 0)
		{
			multicastEnableState2 = 1;
			printf("\t\t\t\tMulticast was disabled.\n");
		}
		else
		{
			multicastEnableState2 = 0;
			printf("\t\t\t\tMulticast was enabled.\n");
		}

		sbapi_multicast_set_enable_state(deviceID, multicast_feature_ids[i], &error, networkInterfaceIndex, multicastEnableState2);
		tallyErrors(error, testFailureCount);
		if (error == 0)
		{
			printf("\t\t\tThe multicast enable state was successfully modified.\n");
		}
		else
		{
			printf("\t\t\tThe multicast enable state did not change. sbapi error = %s\n", sbapi_get_error_string(error));
		}

		// set dhcp server enable state to false
		printf("\t\t\tAttempting to set the multicast enable state to false\n");
		sbapi_multicast_set_enable_state(deviceID, multicast_feature_ids[i], &error, networkInterfaceIndex, 0);
		printf("\t\t\tResult is[%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t%d: Finished testing device 0x%02lX, dhcp server feature 0x%02lX\n", i, deviceID, multicast_feature_ids[i]);
	}

	free(multicast_feature_ids);

	printf("\tFinished testing multicast feature capabilities.\n");
}

void test_wifi_features(long deviceID, int *unsupportedFeatureCount, int *testFailureCount, unsigned char networkInterfaceIndex)
{
	int error = 0;
	int number_of_wifi_features;
	long *wifi_feature_ids = 0;
	int i;

	printf("\n\tTesting wifi features for network interface index = %d\n", networkInterfaceIndex);

	printf("\t\tExercising wifi commands:\n");
	number_of_wifi_features = sbapi_get_number_of_wifi_configuration_features(deviceID, &error);
	printf("\t\t\tResult is %d [%s]\n", number_of_wifi_features, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	if (0 == number_of_wifi_features)
	{
		printf("\tNo wifi configuration capabilities found.\n");
		tallyUnsupportedFeatures(unsupportedFeatureCount);
		return;
	}

	wifi_feature_ids = (long *)calloc(number_of_wifi_features, sizeof(long));
	printf("\t\tGetting wifi configuration feature IDs...\n");
	number_of_wifi_features = sbapi_get_wifi_configuration_features(deviceID, &error, wifi_feature_ids, number_of_wifi_features);
	printf("\t\t\tResult is %d [%s]\n", number_of_wifi_features, sbapi_get_error_string(error));
	tallyErrors(error, testFailureCount);

	for (i = 0; i < number_of_wifi_features; i++)
	{
		unsigned char ssid[32];
		unsigned char characterCount = 0;
		std::string myString;

		printf("\t\t%d: Testing device 0x%02lX, wifi configuration feature id 0x%02lX\n", i, deviceID, wifi_feature_ids[i]);
		printf("\t\t\tInterface Index = %d\n", networkInterfaceIndex);

		// wifi commands are tested, but settings are not changed, with the exception of the pass phrase which is changed to PassPhras3!
		
		printf("\t\t\tAttempting to retrieve the wifi mode...\n");
		unsigned char wifiMode = sbapi_wifi_configuration_get_mode(deviceID, wifi_feature_ids[i], &error, networkInterfaceIndex);
		if (wifiMode == 0)
		{
			myString.replace(myString.begin(), myString.end(), "Client");
		}
		else
		{
			myString.replace(myString.begin(), myString.end(), "Access Point");
		}		
		printf("\t\t\tResult is %s [%s]\n", myString.c_str(), sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to set the wifi mode with the same value...\n");
		sbapi_wifi_configuration_set_mode(deviceID, wifi_feature_ids[i], &error, networkInterfaceIndex, wifiMode);
		printf("\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);


		printf("\t\t\tAttempting to retrieve the wifi security type...\n");
		unsigned char securityType = sbapi_wifi_configuration_get_security_type(deviceID, wifi_feature_ids[i], &error, networkInterfaceIndex);
		if (securityType == 0)
		{
			myString.replace(myString.begin(), myString.end(), "Open");
		}
		else
		{
			myString.replace(myString.begin(), myString.end(), "WPA2");
		}
		printf("\t\t\tResult is %s [%s]\n", myString.c_str(), sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to set the wifi security type with the same value...\n");
		sbapi_wifi_configuration_set_security_type(deviceID, wifi_feature_ids[i], &error, networkInterfaceIndex, securityType);
		printf("\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to retrieve the wifi ssid...\n");
		characterCount = sbapi_wifi_configuration_get_ssid(deviceID, wifi_feature_ids[i], &error, networkInterfaceIndex, &ssid);
		if (characterCount > 0)
		{
			myString.replace(myString.begin(), myString.end(), &(ssid[0]), &(ssid[characterCount]));
		}
		printf("\t\t\tResult is %s [%s]\n", myString.c_str(), sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to set the wifi ssid with the same value...\n");
		sbapi_wifi_configuration_set_ssid(deviceID, wifi_feature_ids[i], &error, networkInterfaceIndex, ssid, characterCount);
		printf("\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t\tAttempting to set the wifi pass phrase with: PassPhras3.\n");
		myString.replace(myString.begin(), myString.end(), "PassPhras3");
		sbapi_wifi_configuration_set_pass_phrase(deviceID, wifi_feature_ids[i], &error, networkInterfaceIndex, (unsigned char *)(myString.c_str()), myString.size());
		printf("\t\t\tResult is [%s]\n", sbapi_get_error_string(error));
		tallyErrors(error, testFailureCount);

		printf("\t\t%d: Finished testing device 0x%02lX, wifi configuration feature 0x%02lX\n", i, deviceID, wifi_feature_ids[i]);
	}

	free(wifi_feature_ids);

	printf("\tFinished testing multicast feature capabilities.\n");
}

