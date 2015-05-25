#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "LNK_CAM.h"

int main( int argc, char *argv[] )
{
	int x;
	int la=0;
	unsigned char XH = 0x00;
	unsigned char XL = 0x00; 
	unsigned char MH = 0x00;
	unsigned char ML = 0x00;
	//////////////////////////////////
	x = reset_camera();
	printf("Reset: %d\n",x);
	//////////////////////////////////



	//////////////////////////////////
	x = take_picture();
	printf("Take1: %d\n",x);
	//////////////////////////////////
	x = read_size(&XH, &XL);
	printf("XH=%x\nXL=%x\n", XH, XL);
	//////////////////////////////////
	x = export_JPEG(XH, XL, MH, ML, 1);
	printf("JPEG Ready %d\n",x);
	//////////////////////////////////
	x = stop_cam();
	printf("Stop Done %d\n",x);
	//////////////////////////////////
	sleep(1);
	//////////////////////////////////
	x = take_picture();
	printf("Take2: %d\n",x);
	//////////////////////////////////
	x = read_size(&XH, &XL);
	printf("XH=%x\nXL=%x\n", XH, XL);
	//////////////////////////////////
	x = export_JPEG(XH, XL, MH, ML, 2);
	printf("JPEG Ready %d\n",x);
	//////////////////////////////////
	x = stop_cam();
	printf("Stop Done %d\n",x);
	//////////////////////////////////
	sleep(1);
	//////////////////////////////////
	x = take_picture();
	printf("Take3: %d\n",x);
	//////////////////////////////////
	x = read_size(&XH, &XL);
	printf("XH=%x\nXL=%x\n", XH, XL);
	//////////////////////////////////
	x = export_JPEG(XH, XL, MH, ML, 3);
	printf("JPEG Ready %d\n",x);
	//////////////////////////////////
	x = stop_cam();
	printf("Stop Done %d\n",x);
	//////////////////////////////////
	sleep(1);







	return 0;
}

