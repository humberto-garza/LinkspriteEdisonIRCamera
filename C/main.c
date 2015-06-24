/*
Copyright (c) 2015, Intel Corporation

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
* Neither the name of Intel Corporation nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Created by Jose Humberto Garza Rosado
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "LNK_CAM.h"

int main( int argc, char *argv[] )
{
	int x;
	int size = 0; 
	unsigned char resolution;
	unsigned char *photo_buffer = NULL;
	unsigned char XH = 0x00;
	unsigned char XL = 0x00; 
	unsigned char MH = 0x00;
	unsigned char ML = 0x00;
	unsigned char f640 = 0x00; //640x480
	unsigned char f320 = 0x11; //320x240
	unsigned char f160 = 0x22; //160x120
	
	//////////////////////////////////
	//Make sure that the Device communication BAUD is set to 38400 right after connecting it
	//38400 is the default BAUD
	system("stty -F /dev/ttyMFD1 38400");
	//////////////////////////////////


	//Reset Camera
	printf("Waiting /dev/ttyMFD1 camera to reset: ...\n");
	x = reset_camera();
	printf("Reset Status: %d\n",x);
	printf("=======================================\n\n");

	//////////////////////////////////
	//Enter Power Saving
	printf("Entering to Power Saving Mode\n");
	x = enter_power_saving();
	printf("Enter Power Saving Status:  %d\n", x);
	printf("=======================================\n");

	printf("Sleeping for 5 seconds...\n");
	sleep(5);
	printf("=======================================\n");
	//Exit Power Saving
	printf("Exiting from Power Saving Mode\n");
	x = exit_power_saving();
	printf("Exit Power Saving Status:  %d\n", x);
	printf("=======================================\n");
	//////////////////////////////////


	//////////////////////////////////
	//Change BAUD
	int baud = 9600;
	printf("Changing BAUD to: %d\n",baud);
	x = change_baud(baud);
	printf("Change Baud Status:  %d\n", x);
	printf("=======================================\n");
	//////////////////////////////////

	//Test 640x480 Resolution
	printf("Changing Image Size...\n");
	resolution = f640;
	x = image_size(resolution);
	printf("Resize %02x Status:  %d\n", resolution, x);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Taking Photo...\n");
	x = take_picture();
	printf("Taking Photo Status: %d\n", x);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Reading Photo Size: /dev/ttyMFD1...\n");
	x = read_size(&XH, &XL);
	printf("XHXL = %02x%02x\nReading Size Status: %d\n", XH, XL,x);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Exporting Photo to buffer...\n");
	x = export_buf(XH, XL, MH, ML, &photo_buffer, &size);
	printf("Export Status %d\n", x);
	printf("%02x_%02x_%02x_%02x\n",photo_buffer[5],photo_buffer[6],photo_buffer[size-7],photo_buffer[size-6]);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Exporting Buffer to photo 6401...\n");
	buf_to_pic(&photo_buffer, size, 6401);
	printf("Done\n");
	printf("=======================================\n");
	//////////////////////////////////
	printf("Exporting Photo to File 6402...\n");
	x = export_pic(XH, XL, MH, ML, 6402);
	printf("Export Status %d\n", x);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Stoping Cam: /dev/ttyMFD1...\n");
	x = stop_cam();
	printf("Stop Status: %d\n\n",x);
	printf("=======================================\n");
	//////////////////////////////////

	//Test 320x240
	printf("Changing Image Size...\n");
	resolution = f320;
	x = image_size(resolution);
	printf("Resize %02x Status:  %d\n", resolution, x);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Taking Photo...\n");
	x = take_picture();
	printf("Taking Photo Status: %d\n", x);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Reading Photo Size: /dev/ttyMFD1...\n");
	x = read_size(&XH, &XL);
	printf("XHXL = %02x%02x\nReading Size Status: %d\n", XH, XL,x);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Exporting Photo to buffer...\n");
	x = export_buf(XH, XL, MH, ML, &photo_buffer, &size);
	printf("Export Status %d\n", x);
	printf("%02x_%02x_%02x_%02x\n",photo_buffer[5],photo_buffer[6],photo_buffer[size-7],photo_buffer[size-6]);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Exporting Buffer to photo 3201...\n");
	buf_to_pic(&photo_buffer, size, 3201);
	printf("Done\n");
	printf("=======================================\n");
	//////////////////////////////////
	printf("Exporting Photo to File 3202...\n");
	x = export_pic(XH, XL, MH, ML, 3202);
	printf("Export Status %d\n", x);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Stoping Cam: /dev/ttyMFD1...\n");
	x = stop_cam();
	printf("Stop Status: %d\n\n",x);
	printf("=======================================\n");
	//////////////////////////////////

	//Test 160x120
	printf("Changing Image Size...\n");
	resolution = f160;
	x = image_size(resolution);
	printf("Resize %02x Status:  %d\n", resolution, x);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Taking Photo...\n");
	x = take_picture();
	printf("Taking Photo Status: %d\n", x);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Reading Photo Size: /dev/ttyMFD1...\n");
	x = read_size(&XH, &XL);
	printf("XHXL = %02x%02x\nReading Size Status: %d\n", XH, XL,x);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Exporting Photo to buffer...\n");
	x = export_buf(XH, XL, MH, ML, &photo_buffer, &size);
	printf("Export Status %d\n", x);
	printf("%02x_%02x_%02x_%02x\n",photo_buffer[5],photo_buffer[6],photo_buffer[size-7],photo_buffer[size-6]);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Exporting Buffer to photo 1601...\n");
	buf_to_pic(&photo_buffer, size, 1601);
	printf("Done\n");
	printf("=======================================\n");
	//////////////////////////////////
	printf("Exporting Photo to File 1602...\n");
	x = export_pic(XH, XL, MH, ML, 1602);
	printf("Export Status %d\n", x);
	printf("=======================================\n");
	//////////////////////////////////
	printf("Stoping Cam: /dev/ttyMFD1...\n");
	x = stop_cam();
	printf("Stop Status: %d\n\n",x);
	printf("=======================================\n");
	//////////////////////////////////

	//Is better to reset the camera at the end od the program if you changed its BAUD
	//Reset Camera
	printf("Waiting /dev/ttyMFD1 camera to reset: ...\n");
	x = reset_camera();
	printf("Reset Status: %d\n",x);
	printf("=======================================\n\n");
	//////////////////////////////////

	free(photo_buffer);
	return 0;
}

