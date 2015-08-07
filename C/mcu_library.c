/*
When you connect the camera to the serial2 you need to deactivate the Linux Console,
you can achieve that using this commands:

systemctl stop serial-getty@ttyMFD2.service
dmesg -n 1

After you did that you can use the serial2 normally
You will also have to connect the GPIO48(mraa33) to the enable and use a 500Ohm resistor 
as a pulldown
*/
#include "mcu_api.h"
#include "mcu_errno.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int x = 0;
unsigned char reset_camera[4]     = {0x56, 0x00, 0x26, 0x00};
unsigned char resolution[9]     = {0x56, 0x00, 0x31, 0x05 , 0x04, 0x01, 0x00, 0x19, 0x11}; //Low Res 0x22, High Res 0x00, Medium Res 0x11
unsigned char take_photo[5]     = {0x56, 0x00, 0x36, 0x01, 0x00};
unsigned char photo_size[5]     = {0x56, 0x00, 0x34, 0x01, 0x00};
unsigned char stop_camera[5]    = {0x56, 0x00, 0x36, 0x01, 0x03};

unsigned char buf[12]       = {0x00};
unsigned char flush[8]        = {0xFF};
unsigned char photo_buffer[15000]   = {0x00};

void flush_camera()
{
  while(!(flush[0]== 0x00 && flush[1]== 0x00 && flush[2]== 0x00 && flush[3]== 0x00 && flush[4]== 0x00 && flush[5]== 0x00 && flush[6]== 0x00 && flush[7]== 0x00))
  {
    uart_read(2,flush+0, 1);
    uart_read(2,flush+1, 1);
    uart_read(2,flush+2, 1);
    uart_read(2,flush+3, 1);
    uart_read(2,flush+4, 1);
    uart_read(2,flush+5, 1);
    uart_read(2,flush+6, 1);
    uart_read(2,flush+7, 1);
    //debug_print(DBG_INFO, "Flush %x_%x_%x_%x_%x_%x_%x_%x\n",flush[0],flush[1],flush[2],flush[3],flush[4],flush[5],flush[6],flush[7]);
  }
}
void mcu_main()
{
  //This pin will be used as EOE (Level Shifter Enable)
  gpio_setup(33, 1); /* set GPIO48 (mraa33) as output */
  gpio_write(33, 0); /* Write 0 to GPIO48 (mraa33) */
  /*
  Execute this commands on the ATOM before "before2" is printed on the terminal

  stty -F /dev/ttyMFD1 38400
  systemctl stop serial-getty@ttyMFD2.service
  dmesg -n 1
  cat /dev/ttymcu1

   */


  debug_print(DBG_INFO, "Before1\n");
  mcu_sleep(3050);
  debug_print(DBG_INFO, "Before2\n");

  gpio_write(33, 1);/* Write 1 to GPIO48 (mraa33) */
  uart_setup(2, 38400);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //RESOLUTION
  buf[0]=0x00;
  while(!(buf[0]==0x76 && buf[2]==0x31))
  {
    debug_print(DBG_INFO, "==================================\n");
    x = uart_write(2, resolution, 9);
    x = uart_read(2,buf+0, 1);
    x = uart_read(2,buf+1, 1);
    x = uart_read(2,buf+2, 1);
    x = uart_read(2,buf+3, 1);
    x = uart_read(2,buf+4, 1);
    debug_print(DBG_INFO, "After RESOLUTION: %x_%x\n", buf[0], buf[2]);
    flush_camera();
    mcu_sleep(50);
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //RESET
  buf[0]=0x00;
  while(!(buf[0]==0x76 && buf[2]==0x26))
  {
    debug_print(DBG_INFO, "==================================\n");
    x = uart_write(2, reset_camera, 4);
    x = uart_read(2,buf+0, 1);
    x = uart_read(2,buf+1, 1);
    x = uart_read(2,buf+2, 1);
    x = uart_read(2,buf+3, 1);
    debug_print(DBG_INFO, "After RESET: %x_%x\n", buf[0], buf[2]);
    flush_camera();
    mcu_sleep(50);
  }
  mcu_sleep(250);


  while(1)
  {
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //TAKE_Photo
    buf[0]=0x00;
    while(!(buf[0]==0x76 && buf[2]==0x36))
    {

      x = uart_write(2, take_photo, 5);//
      x = uart_read(2,buf+0, 1);
      x = uart_read(2,buf+1, 1);
      x = uart_read(2,buf+2, 1);
      x = uart_read(2,buf+3, 1);
      //debug_print(DBG_INFO, "==================================\n");
      //debug_print(DBG_INFO, "After TAKE Photo: %x_%x\n", buf[0], buf[2]);
      flush_camera();
      //mcu_sleep(5);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //READ_Size
    buf[0]=0x00;
    while(!(buf[0]==0x76 && buf[2]==0x34))
    {
      x = uart_write(2, photo_size, 5);
      x = uart_read(2,buf+0, 1);
      x = uart_read(2,buf+1, 1);
      x = uart_read(2,buf+2, 1);
      x = uart_read(2,buf+3, 1);
      x = uart_read(2,buf+4, 1);
      x = uart_read(2,buf+5, 1);
      x = uart_read(2,buf+6, 1);
      x = uart_read(2,buf+7, 1);
      x = uart_read(2,buf+8, 1);
      //debug_print(DBG_INFO, "==================================\n");
      //debug_print(DBG_INFO, "After Read READ Size: %x_%x %x_%x\n", buf[0], buf[2], buf[7], buf[8]);
      flush_camera();
      //mcu_sleep(5);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //READ_Photo
      int fsizeH = (buf[7]<<8) + (buf[8]);
      int total_read = fsizeH;

      //You have to ask for an 8 multiple size value
      while( (total_read%8)!=0)
        total_read+=2;

      int aloc=0;
      unsigned char MH = 0x00;
      unsigned char ML = 0x00;
      unsigned char KH = (unsigned int)(total_read>>8);
      unsigned char KL = (unsigned int)(total_read & 0xff);
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      unsigned char jpeg[16] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00, MH, ML, 0x00, 0x00, KH, KL, 00, 00};
      uart_write(2, jpeg, 16);

      //Sync Bytes are 0x76 0x00 0x32 0x00 0x00
      uart_read(2,buf+0, 1);
      uart_read(2,buf+1, 1);
      uart_read(2,buf+2, 1);
      uart_read(2,buf+3, 1);
      uart_read(2,buf+4, 1);

      //Read the Total_read value but only the fsizeH characters correspond to the foto
      while(aloc<total_read)
      {
        uart_read(2,photo_buffer+aloc, 1);
        aloc++;
      }

      //Sync Bytes are 0x76 0x00 0x32 0x00 0x00
      uart_read(2,buf+5, 1);
      uart_read(2,buf+6, 1);
      uart_read(2,buf+7, 1);
      uart_read(2,buf+8, 1);
      uart_read(2,buf+9, 1);

      //debug_print(DBG_INFO, "==================================\n");
      debug_print(DBG_INFO, "After JPEG Read:%x_%x...%x_%x\n", photo_buffer[0], photo_buffer[1], photo_buffer[fsizeH-2], photo_buffer[fsizeH-1]);
      flush_camera();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //STOP_camera
    buf[0]=0x00;
    while(!(buf[0]==0x76 && buf[2]==0x36))
    {
      x = uart_write(2, stop_camera, 5);//
      x = uart_read(2,buf+0, 1);
      x = uart_read(2,buf+1, 1);
      x = uart_read(2,buf+2, 1);
      x = uart_read(2,buf+3, 1);
      //debug_print(DBG_INFO, "==================================\n");
      //debug_print(DBG_INFO, "After STOP Camera: %x_%x\n", buf[0], buf[2]);
      flush_camera();
      //mcu_sleep(5);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  }

}


