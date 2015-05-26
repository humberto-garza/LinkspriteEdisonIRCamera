#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "LNK_CAM.h"
#include <termios.h>

int main( int argc, char *argv[] )
{
	int x;
	int la=0;
	unsigned char XH = 0x00;
	unsigned char XL = 0x00; 
	unsigned char MH = 0x00;
	unsigned char ML = 0x00;
	//////////////////////////////////
	printf("Waiting for Camera to reset: /dev/ttyMFD1...\n");
	x = reset_camera();
	printf("Status: %d\n\n",x);
	//////////////////////////////////


	//////////////////////////////////
	printf("Taking Photo: /dev/ttyMFD1...\n");
	x = take_picture();
	printf("Status: %d\n\n",x);

	//////////////////////////////////
	printf("Reading Photo Size: /dev/ttyMFD1...\n");
	x = read_size(&XH, &XL);
	printf("XH=%02x\nXL=%02x\nStatus: %d\n\n", XH, XL,x);

	//////////////////////////////////
	printf("Exporting Photo to buffer...\n");
	unsigned char *photo_buffer = NULL;
	x = export_buf(XH, XL, MH, ML, &photo_buffer);
	printf("Name %d\nStatus: %d\n\n",1, x);
	printf("%02x_%02x_%02x_%02x\n\n",photo_buffer[0],photo_buffer[1],photo_buffer[x-2],photo_buffer[x-1]);


	//////////////////////////////////
	printf("Exporting Buffer to photo...\n");
	buf_to_pic(&photo_buffer, x, 1);

	//////////////////////////////////
	printf("Exporting Photo to File...\n");
	x = export_pic(XH, XL, MH, ML, 2);
	printf("Name %d\nStatus: %d\n\n",2, x);

	//////////////////////////////////
	printf("Stoping Cam: /dev/ttyMFD1...\n");
	x = stop_cam();
	printf("Status: %d\n\n",x);

	//////////////////////////////////

	
	free(photo_buffer);
	return 0;
}

