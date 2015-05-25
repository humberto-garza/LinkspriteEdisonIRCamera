#include "LNK_CAM.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int reset_camera()
{
	int result =-1;
	char buf[4];
	unsigned char reset[4] = {0x56, 0x00, 0x26, 0x00}; 
	FILE *camera = fopen("/dev/ttyMFD1", "a+");

	printf("Waiting for Camera to reset: /dev/ttyMFD1...\n");
	fprintf(camera,"%c%c%c%c",reset[0],reset[1],reset[2],reset[3]);

	fgets(buf,4,camera);
	//printf("%x_%x_%x_%x\n",buf[0],buf[1],buf[2],buf[3]);
	if(buf[0]==0x76 && buf[2]==0x26)
		result =0;

	fclose(camera);
	sleep(3);
	return result;
}

int take_picture()
{
	int result =-1;
	char buf[4];
	unsigned char take[5] = {0x56, 0x00, 0x36, 0x01, 0x00}; 
	FILE *camera = fopen("/dev/ttyMFD1", "a+");

	printf("Taking Photo: /dev/ttyMFD1...\n");
	fprintf(camera,"%c%c%c%c%c",take[0],take[1],take[2],take[3],take[4]);
	
	fgets(buf,4,camera);
	//printf("%x_%x_%x_%x\n",buf[0],buf[1],buf[2],buf[3]);
	if(buf[0]==0x76 && buf[2]==0x36)
		result =0;

	fclose(camera);
	return result;
}

int read_size(unsigned char *XH,unsigned char *XL)
{
	int result =-1;
	char buf[9];
	unsigned char size[5] = {0x56, 0x00, 0x34, 0x01, 0x00}; 
	FILE *camera = fopen("/dev/ttyMFD1", "a+");

	printf("Reading Photo Size: /dev/ttyMFD1...\n");
	fprintf(camera,"%c%c%c%c%c",size[0],size[1],size[2],size[3],size[4]);
	
	fgets(buf,9,camera);
	//printf("%x_%x_%x_%x_%x_%x_%x_%x_%x\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8]);
	if(buf[0]==0x76 && buf[2]==0x34)
		result =0;

	*XH = buf[7];
	*XL = buf[8];
	fclose(camera);

	return result;
}

int export_JPEG(unsigned char KH, unsigned char KL, unsigned char MH, unsigned char ML, int i)
{
	KH = KH + 0x20/0x100;
    KL = KL + 0x20%0x100;
	unsigned char XH = 0x00;
	unsigned char XL = 0x0A;
	unsigned char jpeg[16] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00, MH, ML, 0x00, 0x00, KH, KL, XH, XL}; 
	FILE *camera = fopen("/dev/ttyMFD1", "a+");

	printf("Exporting Photo as jpeg...\n");
	fprintf(camera,"%c%c%c%c%c%c%c%c%c%c%c%c%x%x%x%02x",jpeg[0],jpeg[1],jpeg[2],jpeg[3],jpeg[4],jpeg[5],jpeg[6],jpeg[7],jpeg[8],jpeg[9],jpeg[10],jpeg[11],jpeg[12],jpeg[13],jpeg[14],jpeg[15]);
	
	unsigned int last = 0x00;
	unsigned int c0 = 0x00;
	unsigned int c1 = 0x00;
	unsigned int c2 = 0x00;
	unsigned int c3 = 0x00;
	unsigned int c4 = 0x00;

	int x= 0;
	int val =-1;

	for(x=0;x<10;x++)
	{
		last = fgetc(camera);
		c4 = c3;
		c3 = c2;
		c2 = c1;
		c1 = c0;
		c0 = last;
	}

	char buffer[32]={0x00};
	snprintf(buffer, sizeof(buffer),"foto%d.jpg",i);
	FILE *foto = fopen(buffer, "w");

	while(1) 
	{
		last = fgetc(camera);

		fputc(c4,foto);

		c4 = c3;
		c3 = c2;
		c2 = c1;
		c1 = c0;
		c0 = last;

		if(c4 ==0xff && c3== 0xd9 && val != 0)
		{
			val = 0;
			fputc(c4,foto);
			fputc(c3,foto);
			fclose(foto);
		}	
		if(c4== 0x76 && c3== 0x00 && c2== 0x32 && c1== 0x00 && c0== 0x00)
			break;
	}

	fclose(camera);
	return val;
}

int stop_cam()
{
	int result =-1;
	char buf[4];
	unsigned char stop[5] = {0x56, 0x00, 0x36, 0x01, 0x03}; 
	FILE *camera = fopen("/dev/ttyMFD1", "a+");

	printf("Stoping Cam: /dev/ttyMFD1...\n");
	fprintf(camera,"%c%c%c%c%c",stop[0],stop[1],stop[2],stop[3],stop[4]);
	
	fgets(buf,4,camera);
	//printf("%02x_%02x_%02x_%02x\n",buf[0],buf[1],buf[2],buf[3]);
	if(buf[0]==0x76 && buf[2]==0x36)
		result =0;

	fclose(camera);
	return result;
}