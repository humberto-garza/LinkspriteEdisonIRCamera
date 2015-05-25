/*

*/
#ifndef LNK_CAM
#define LNK_CAM

#include "LNK_CAM.c"
int reset_camera();
int take_picture();
int read_size(unsigned char *XH,unsigned char *XL);
int export_JPEG(unsigned char KH, unsigned char KL, unsigned char MH, unsigned char ML, int i);
int stop_cam();
#endif
