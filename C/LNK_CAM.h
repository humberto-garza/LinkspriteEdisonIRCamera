/*

*/
#ifndef LNK_CAM
#define LNK_CAM

#include "LNK_CAM.c"
int reset_camera();
int take_picture();
int read_size(unsigned char *XH,unsigned char *XL);

int export_buf(unsigned char KH, unsigned char KL, unsigned char MH, unsigned char ML, unsigned char **photo_buffer);
int export_pic(unsigned char KH, unsigned char KL, unsigned char MH, unsigned char ML, int i);

void buf_to_pic(unsigned char **photo_buffer,int size, int num);

int export_JPEG2(unsigned char KH, unsigned char KL, unsigned char MH, unsigned char ML, int i);
int stop_cam();
#endif
