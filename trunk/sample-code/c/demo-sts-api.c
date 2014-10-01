/**
* @file     demo-sts-api.c
* @author   Mark Zieg <mark.zieg@oceanoptics.com>
* @date     2012-11-08
*
* Demonstrate how to communicate via the STS / OBP API.
* Compare to better-structured analogue in serial-sts-demo.
*
* TODO:
*   - support real checksums
*   - support data block (between header and checksum/footer)
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
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

#include "api/SeaBreezeWrapper.h"

// global for convenience
const int spec_index = 0;
int error = 0;

struct OBPHeader
{
    unsigned char  start_bytes[2];          // 0-1: 0xC1, 0xC0
    unsigned short protocol_version;        // 2-3: 0x1000
    unsigned short flags;                   // 4-5
    unsigned short errno;                   // 6-7
    unsigned int   message_type;            // 8-11
    unsigned int   regarding;               // 12-15
    unsigned char  reserved[6];             // 16-21
    unsigned char  checksum_type;           // 22
    unsigned char  immediate_data_length;   // 23
    unsigned char  immediate_data[16];      // 24-39
    unsigned int   bytes_remaining;         // 40-43
};                                          // 44 bytes total

const char* getOBPError(unsigned errno)
{
    switch (errno)
    {
        case 1: return "Invalid/unsupported protocol";
        case 2: return "Unknown message type";
        case 3: return "Bad checksum";
        case 4: return "Message too large";
        case 5: return "Payload length does not match message type";
        case 6: return "Payload data invalid";
        case 7: return "Device not ready for given message";
        case 8: return "Unknown checksum type";
        case 9: return "Device reset unexpectedly";
    }
    return "UNKNOWN STS ERROR CODE";
}

// returns 0 on success
int sendOBPMessage(
        unsigned int message_type,                // (input)  message type to send
        unsigned int immediate_data_length,       // (input)  size of data to send with request (max 16 bytes)
        unsigned char *immediate_data,            // (input)  data to send with request
        unsigned int *output_len,                 // (output) how much data came back in response
        unsigned char *output_data,               // (in/out) pre-allocated buffer to receive response data
        unsigned int max_output_len,              // (input)  max size of response data (output_data allocated size)
        unsigned int expected_extra_response_len) // (input)  expected size of non-immediate response data (above 64 byte total)
{                                                 //          (zero if no response expected)
    const unsigned char STS_REQUEST_ENDPOINT  = 0x01;
    const unsigned char STS_RESPONSE_ENDPOINT = 0x81;

    if (output_len)
        *output_len = 0;

    if (immediate_data_length > 16)
    {
        fprintf(stderr, "ERROR: sendOBPMessage only supports immediate data <= 16 bytes\n");
        error = 1;
        return 1;
    }

    if (sizeof(struct OBPHeader) != 44)
    {
        fprintf(stderr, "ERROR: header size is %u bytes (expected 44)\n", (unsigned) sizeof(struct OBPHeader));
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////
    // define outbound message (header, checksum, footer)
    ////////////////////////////////////////////////////////////////////////////

    // header
    struct OBPHeader header;
    memset(&header, 0, sizeof(header));
    header.start_bytes[0] = 0xc1;
    header.start_bytes[1] = 0xc0;
    header.protocol_version = 0x1000;
    header.flags = 0x0000;  // no ACK requested
    header.message_type = message_type;
    header.immediate_data_length = immediate_data_length;
    for (unsigned i = 0; i < immediate_data_length; i++)
        header.immediate_data[i] = immediate_data[i];
    header.bytes_remaining = 16 + 4; // i.e. 0x14, 16-byte checksum + 4-byte footer

    // checksum
    unsigned char checksum[16];
    memset(checksum, 0, sizeof(checksum));

    // footer
    unsigned char footer[] = { 0xc5, 0xc4, 0xc3, 0xc2 };

    ////////////////////////////////////////////////////////////////////////////
    // assemble into one message
    ////////////////////////////////////////////////////////////////////////////

    // TODO: merge message with header, checksum, and footer, to avoid this
    //       extra storage and copy
    unsigned char message[64];
    memset(message, 0, sizeof(message));

    memcpy((void*) message + 0,       (void*)&header,  sizeof(header));
    memcpy((void*) message + 44,      (void*) checksum, sizeof(checksum));
    memcpy((void*) message + 44 + 16, (void*) footer,   sizeof(footer));

    ////////////////////////////////////////////////////////////////////////////
    // send message
    ////////////////////////////////////////////////////////////////////////////

    seabreeze_write_usb(spec_index, &error, STS_REQUEST_ENDPOINT, message, sizeof(message));
    check_error(spec_index, &error, "seabreeze_write_usb");

    ////////////////////////////////////////////////////////////////////////////
    // read response, if one is expected
    ////////////////////////////////////////////////////////////////////////////

    if (max_output_len > 0 && output_len && output_data)
    {
        unsigned expected_response_size = sizeof(message) + expected_extra_response_len;
        unsigned char *response = (unsigned char*) malloc(expected_response_size);

        int bytes_read = seabreeze_read_usb(spec_index, &error, STS_RESPONSE_ENDPOINT, response, expected_response_size);
        check_error(spec_index, &error, "seabreeze_read_usb(response)");
        if (bytes_read != expected_response_size)
            fprintf(stderr, "WARNING: only read %d of expected %u byte response\n", bytes_read, expected_response_size);

        struct OBPHeader *response_header = (struct OBPHeader*) response;
        if (0 != response_header->errno)
        {
            printf("ERROR: STS response contained error: %s\n", getOBPError(response_header->errno));
            return -1;
        }

        unsigned bytes_copied = 0;

        // extract immediate_data from response
        if (response_header->immediate_data_length > 0)
        {
            unsigned bytes_to_copy = response_header->immediate_data_length;
            if (bytes_to_copy + bytes_copied > max_output_len)
            {
                fprintf(stderr, "WARNING: dropped %d immediate_data bytes (not enough room in output buffer)\n",
                    max_output_len - (bytes_to_copy + bytes_copied));
                bytes_to_copy = max_output_len - (bytes_to_copy + bytes_copied);
            }
            if (bytes_to_copy > 0)
            {
                memcpy(output_data, response_header->immediate_data, bytes_to_copy);
                bytes_copied += bytes_to_copy;
            }
        }

        // extract any remaining_data
        if (bytes_read > sizeof(message) && response_header->bytes_remaining > sizeof(checksum) + sizeof(footer))
        {
            // extract remaining_data from response
            unsigned bytes_to_copy = bytes_read - sizeof(message);
            if (bytes_to_copy + bytes_copied > max_output_len)
            {
                fprintf(stderr, "WARNING: dropped %d remaining_data bytes (not enough room in output buffer)\n",
                    max_output_len - (bytes_to_copy + bytes_copied));
                bytes_to_copy = max_output_len - bytes_copied;
            }

            if (bytes_to_copy > 0)
                memcpy(output_data + bytes_copied, response + sizeof(header), bytes_to_copy);
            bytes_copied += bytes_to_copy;
        }

        if (output_len)
            *output_len = bytes_copied;

        // TODO: validate checksum
        // TODO: validate footer

        // cleanup
        free(response);
    }

    return error;
}

int main(int argc, char **argv)
{
    ////////////////////////////////////////////////////////////////////////////
    // startup
    ////////////////////////////////////////////////////////////////////////////

    for (unsigned i = 1; i < argc; i++)
        if (!strcmp(argv[i], "--verbose"))
            seabreeze_set_verbose(1);

    char ver[256];
    seabreeze_get_api_version_string(ver, sizeof(ver));
    printf("SeaBreeze API Version: %s\n", ver);

    printf("Testing STS API.\n\n");

    seabreeze_open_spectrometer(spec_index, &error);
    if (check_error(spec_index, &error, "seabreeze_open_spectrometer"))
        return 1;

    char spec_type[16];
    seabreeze_get_model(spec_index, &error, spec_type, sizeof(spec_type));
    if (check_error(spec_index, &error, "seabreeze_get_model"))
        return 1;

    if (strncmp(spec_type, "STS", 3))
    {
        printf("WARNING: This program is only designed to test Ocean Optics STS spectrometers!\n");
        printf("         Proceed at your own risk...\n\n");
    }

    unsigned int result_size = 0;

    ////////////////////////////////////////////////////////////////////////////
    // get hardware revision
    ////////////////////////////////////////////////////////////////////////////

    unsigned char hardware_rev_result[1];
    if (!sendOBPMessage(0x00000080L, 0, NULL, &result_size, hardware_rev_result, sizeof(hardware_rev_result), 0))
    {
        if (1 == result_size)
        {
            unsigned char hardware_revision = hardware_rev_result[0];
            printf("Hardware Revision: %04x (%u)\n", hardware_revision, hardware_revision);
        }
        else
        {
            printf("ERROR: expected %tu bytes back from get_hardware_revision, received %u\n", sizeof(hardware_rev_result), result_size);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // get firmware revision
    ////////////////////////////////////////////////////////////////////////////

    unsigned char firmware_rev_result[2];
    if (!sendOBPMessage(0x00000090L, 0, NULL, &result_size, firmware_rev_result, sizeof(firmware_rev_result), 0))
    {
        if (2 == result_size)
        {
            unsigned short firmware_revision = 0;
            firmware_revision = firmware_rev_result[0] | (firmware_rev_result[1] << 8);
            printf("Firmware Revision: %04x (%u)\n", firmware_revision, firmware_revision);
        }
        else
        {
            printf("ERROR: expected %tu bytes back from get_firmware_revision, received %u\n", sizeof(firmware_rev_result), result_size);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // set integration time
    ////////////////////////////////////////////////////////////////////////////

    // STS will not take acquisition unless you first set integration time
    unsigned char integration_time[] = { 0xa0, 0x86, 0x01, 0x00 }; // 100ms -> 100,000us -> hex -> little-endian
    if (sendOBPMessage(0x00110010, sizeof(integration_time), integration_time, NULL, NULL, 0, 0))
        printf("ERROR: could not set integration time to 100ms\n");

    ////////////////////////////////////////////////////////////////////////////
    // get corrected spectrum
    ////////////////////////////////////////////////////////////////////////////

    // this works, just commenting it out...
    //
    // unsigned char spectra_result[2048];
    // if (!sendOBPMessage(0x00101000, 0, NULL, &result_size, spectra_result, sizeof(spectra_result), sizeof(spectra_result)))
    // {
    //     if (2048 == result_size)
    //         printf("Retrieved spectra of %tu pixels\n", sizeof(spectra_result) / 2);
    //     else
    //         printf("ERROR: expected %tu bytes back from get_spectrum, received %u\n", sizeof(spectra_result), result_size);
    // }

    ////////////////////////////////////////////////////////////////////////////
    // get and send partial corrected spectrum
    ////////////////////////////////////////////////////////////////////////////

    unsigned char subspectrum_specification[] = {
        0x02, 0x00, // mode 2 (LSB first)
        0x64, 0x00, // start at pixel 100
        0x01, 0x00, // increment 1 pixel at a time
        0x0A, 0x00  // return 10 pixels
    };

    // send subspectrum specification
    if (!sendOBPMessage(0x00102010L, sizeof(subspectrum_specification), subspectrum_specification, NULL, NULL, 0, 0))
    {
        // retreive a processed subspectrum
        unsigned char subspectrum[20];
        if (!sendOBPMessage(0x00102080L, 0, NULL, &result_size, subspectrum, sizeof(subspectrum), sizeof(subspectrum)))
        {
            if (sizeof(subspectrum) == result_size)
            {
                printf("\nPixels 100-109:\n");
                for (unsigned i = 0; i < 10; i++)
                {
                    unsigned pixel = 100 + i;
                    unsigned short value = subspectrum[i * 2] | (subspectrum[i * 2 + 1] << 8);
                    printf("  %u (0x%04x): %5u (0x%04x)\n", pixel, pixel, value, value);
                }
            }
            else
            {
                printf("ERROR: expected %tu bytes back from get_and_send_partial_corrected_spectrum, received %u\n", sizeof(subspectrum), result_size);
            }
        }
        else
        {
            printf("ERROR: unable to retrieve subspectrum\n");
        }
    }
    else
    {
        printf("ERROR: unable to set subspectrum specification\n");
    }

    ////////////////////////////////////////////////////////////////////////////
    // cleanup
    ////////////////////////////////////////////////////////////////////////////

    seabreeze_set_verbose(0);

    seabreeze_close_spectrometer(spec_index, &error);
    if (check_error(spec_index, &error, "seabreeze_close_spectrometer"))
        return 1;

    return 0;
}