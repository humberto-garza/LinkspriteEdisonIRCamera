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
	unsigned char buf[10];
	unsigned char size[5] = {0x56, 0x00, 0x34, 0x01, 0x00}; 
	FILE *camera = fopen("/dev/ttyMFD1", "a+");

	fprintf(camera,"%c%c%c%c%c",size[0],size[1],size[2],size[3],size[4]);

	fgets(buf,10,camera);
	//printf("%02x_%02x_%02x_%02x_%02x_%02x_%02x_%02x_%02x_%02x\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9]);
	if(buf[0]==0x76 && buf[2]==0x34)
		result =0;

	*XH = buf[7];
	*XL = buf[8];
	fclose(camera);

	return result;
}

int export_buf(unsigned char KH, unsigned char KL, unsigned char MH, unsigned char ML, unsigned char **photo_buffer)
{
	//Time to send file delimiters
	unsigned char XH = 0x00;
	unsigned char XL = 0x0A;

	//Open Camera File
	unsigned char jpeg[16] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00, MH, ML, 0x00, 0x00, KH, KL, XH, XL}; 
	FILE *camera = fopen("/dev/ttyMFD1", "a+");

	//Send command to return the JPEG File by the camera
	fprintf(camera,"%c%c%c%c%c%c%c%c%c%c%c%c%x%x%x%02x",jpeg[0],jpeg[1],jpeg[2],jpeg[3],jpeg[4],jpeg[5],jpeg[6],jpeg[7],jpeg[8],jpeg[9],jpeg[10],jpeg[11],jpeg[12],jpeg[13],jpeg[14],jpeg[15]);
	
	//Read file delimeter
	unsigned char sync[5] ={0x00};
	fread(sync, 5, 1, camera);
	//If the delimiter is not right, return -1
	if(sync[0]!= 0x76 && sync[1]!= 0x00 && sync[2]!= 0x32 && sync[3]!= 0x00 && sync[4]!= 0x00)
		return -1;

	//Assign the Size of the JPEG (It mus be calculated in these steps in order to make it work)
	int fsizeH = ((long)KH)<<8;
	int fsizeL = ((long)KL);
	fsizeH += fsizeL;

	//Allocate the Proper Space for the buffer
	*photo_buffer = malloc(fsizeH + 1);
	fread(*photo_buffer, fsizeH, 1, camera);

	//Flush the rest of the bytes sent by the camera until the delimiter is found
	while(1)
	{	
		sync[0] = sync[1];
		sync[1] = sync[2];
		sync[2] = sync[3];
		sync[3] = sync[4];
		sync[4] = fgetc(camera);

		if(sync[0]== 0x76 && sync[1]== 0x00 && sync[2]== 0x32 && sync[3]== 0x00 && sync[4]== 0x00)
			break;
	}
	
	fclose(camera);
	return fsizeH;
}


int export_pic(unsigned char KH, unsigned char KL, unsigned char MH, unsigned char ML, int i)
{
	//Time to send file delimiters
	unsigned char XH = 0x00;
	unsigned char XL = 0x0A;

	//Open Camera File
	unsigned char jpeg[16] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00, MH, ML, 0x00, 0x00, KH, KL, XH, XL}; 
	FILE *camera = fopen("/dev/ttyMFD1", "a+");

	//Open JPEG File to Export the Photo
	char buffer[32]={0x00};
	snprintf(buffer, sizeof(buffer),"foto%d.jpg",i);
	FILE *foto = fopen(buffer, "w");

	//Send command to return the JPEG File by the camera
	fprintf(camera,"%c%c%c%c%c%c%c%c%c%c%c%c%x%x%x%02x",jpeg[0],jpeg[1],jpeg[2],jpeg[3],jpeg[4],jpeg[5],jpeg[6],jpeg[7],jpeg[8],jpeg[9],jpeg[10],jpeg[11],jpeg[12],jpeg[13],jpeg[14],jpeg[15]);
	
	//Read file delimeter
	unsigned char sync[5] ={0x00};
	fread(sync, 5, 1, camera);
	//If the delimiter is not right, return -1
	if(sync[0]!= 0x76 && sync[1]!= 0x00 && sync[2]!= 0x32 && sync[3]!= 0x00 && sync[4]!= 0x00)
		return -1;

	//Assign the Size of the JPEG (It mus be calculated in these steps in order to make it work)
	int fsizeH = ((long)KH)<<8;
	int fsizeL = ((long)KL);
	fsizeH += fsizeL;

	//Allocate the Proper Space for the buffer
	unsigned char *photo_buffer = malloc(fsizeH + 1);
	fread(photo_buffer, fsizeH, 1, camera);

	//Flush the rest of the bytes sent by the camera until the delimiter is found
	while(1)
	{	
		sync[0] = sync[1];
		sync[1] = sync[2];
		sync[2] = sync[3];
		sync[3] = sync[4];
		sync[4] = fgetc(camera);

		if(sync[0]== 0x76 && sync[1]== 0x00 && sync[2]== 0x32 && sync[3]== 0x00 && sync[4]== 0x00)
			break;
	}
	int x;
	for(x=0; x<fsizeH; x++)
	{
		fputc(photo_buffer[x],foto);

	}
	fclose(foto);
	fclose(camera);
	return fsizeH;
}

void buf_to_pic(unsigned char **photo_buffer,int size, int num)
{
	//Open JPEG File to Export the Photo
	char buffer[32]={0x00};
	snprintf(buffer, sizeof(buffer),"foto%d.jpg", num);
	FILE *foto = fopen(buffer, "w");

	int x;
	for(x=0; x<size; x++)
		fputc((*photo_buffer)[x],foto);

	fclose(foto);
}

int stop_cam()
{
	int result =-1;
	char buf[4];
	unsigned char stop[5] = {0x56, 0x00, 0x36, 0x01, 0x03}; 
	FILE *camera = fopen("/dev/ttyMFD1", "a+");

	fprintf(camera,"%c%c%c%c%c",stop[0],stop[1],stop[2],stop[3],stop[4]);
	
	fgets(buf,4,camera);
	//printf("%02x_%02x_%02x_%02x\n",buf[0],buf[1],buf[2],buf[3]);
	if(buf[0]==0x76 && buf[2]==0x36)
		result =0;

	fclose(camera);
	return result;
}

int export_JPEG2(unsigned char KH, unsigned char KL, unsigned char MH, unsigned char ML, int i)
{
	//Reading margin
	int a = 0x1FF;

	KH = KH + a/0x100;
    KL = KL + a%0x100;

	unsigned char XH = 0x00;
	unsigned char XL = 0x0A;
	unsigned char jpeg[16] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00, MH, ML, 0x00, 0x00, KH, KL, XH, XL}; 
	FILE *camera = fopen("/dev/ttyMFD1", "a+");

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
