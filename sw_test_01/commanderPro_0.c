// downloaded from https://github.com/audiohacked/OpenCorsairLink/issues/70 
//
// Tiny demo of Corsair Commander Pro Communication with hidapi-libusb.
//
// The program requests the firmware version, then prints out the response
// as string of hex values.
//
// Then the first fan is set to 100% speed for 5 seconds before slowing down
// to 30%. The program then exists.
//
// The code is very quick and dirty so it will most likely segfault if
// no Corsair Commander PRO is connected.
//
// Compile with gcc -I/usr/include/hidapi -lhidapi-libusb (filename.c)
// Run with ./a.out
//
//
// To complie: gcc -c -o commanderPro_0 commanderPro_0.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hidapi.h"
//#include "/home/stephen/Downloads/OpenCorsairLink/hidapi/hidapi/hidapi.h"

#define MAX_STR 255

int main(int argc, char* argv[])
{
	int res;
	unsigned char outbuf[63];
    unsigned char inbuf[16];

	wchar_t wstr[MAX_STR];
	hid_device *handle;

	// Open the device using the VID, PID, and optionally the Serial number.
	handle = hid_open(0x1b1c, 0x0c10, NULL);

	// Read the Manufacturer String
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	printf("Manufacturer String: %ls\n", wstr);

	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	printf("Product String: %ls\n", wstr);

	// Read the Serial Number String
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	printf("Serial Number String: %ls", wstr);
	printf("\n");

    // Clear the outbuf array
    int i = 0;
    for (; i < 63; ++i)
        outbuf[i] = 0x00;

    // Construct a command
    outbuf[0] = 0x02;   // Get firmware version
    
    // Write 63 bytes, then read 16 bytes.
    res = hid_write(handle, outbuf, 63);
    res = hid_read(handle, inbuf, 16);

    // Print result
    for (i = 0; i < 16; ++i)
        printf("%x ", inbuf[i]);
    printf("\n");

    outbuf[0] = 0x23;   // Set fan to fixed %
    outbuf[1] = 0x00;   // Select fan number 1
    outbuf[2] = 0x64;   // Set speed to 100%
    res = hid_write(handle, outbuf, 63);
    res = hid_read(handle, inbuf, 16);

    sleep(5);

    outbuf[0] = 0x23;   // Set fan to fixed %
    outbuf[1] = 0x00;   // Select fan number 1
    outbuf[2] = 0x1E;   // Set speed to 30%
    res = hid_write(handle, outbuf, 63);
    res = hid_read(handle, inbuf, 16);

    return 0;
}
