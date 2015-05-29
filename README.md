# LinkspriteEdisonIRCamera
Infrared JPEG Color Camera Serial UART TTL Level - Edison Library

UPDATES!
-------------
 - Now you can Change the compression Ratio 
  - I tried to change the ratio in the specified by the manufacturer but the camera refuses to do so; anyways the option to change it is there altough the camera will not make the change.
 - Now you can get the 3 available resolutions 640x480, 320x240 and 160x120
 - Now you can export the content to a buffer or directly to a jpeg file
 - You can print your jpeg buffer
 - I will soon explain the Returns of the functions; in simple words, if you get a 0 return, everything's fine. I will add this information soon.

----------

### There will be some extra modifications and documentation

 - Function to Change Baud
 - Function to Change Ratio
 - Function to Change Resolution
 - Function to Activate Power Saving Mode
 - Explanation of the function Returns
 - Getting Started with the Arduino Expansion Board
 - Getting Started with the Mini Breakout Board
 
----------

Resources
-------------
 - Infrared JPEG Color Camera Serial UART TTL Level [Web Page](http://store.linksprite.com/infrared-jpeg-color-camera-serial-uart-ttl-level/)
 - Infrared JPEG Color Camera Serial UART TTL Level [User Manual ](http://www.linksprite.com/upload/file/1291522825.pdf)


----------

Setting up your Arduino Board
-------------
In order to make this code work with your Arduino Breakout Board, you will need to consider several steps:
 - Even though you will use your Arduino Board, you will program it using the Linux Terminal
- The easy way to activate the Serial Port to communicate with the camera is running this Arduino Sketch:
```sh
void setup() 
{  
       Serial1.begin(38400); 
}  
void loop() 
{  
       delay(1000);      
}  
```
 - Add the **C Folder** files to a directory of your preference within the Edison (Yocto)
 - Between the RX and TX of your camera and the Arduino's connect a 10K resistor
 - Power your Camera with a separate Power Source since the Arduino Breakout Limits the Current
 - You can power it with your computer using a normal USB cable
 - Cut the USB Cable and you will see 4 cables (you will only use the Red and Black)
  - Be carefull not to connect the other 2 cables
 - Connect the RedUSB cable to the 5V pin of the camera
 - Connect the BlackUSB cable to the Ground pin of the camera and to the Arduino Ground
 - Compile the code:
```sh
 gcc main.c -o main.out
```
 - Run the code:
```sh
 chmod +x main.out
 ./main.out
```
 - You will see that this sample main will output 2 fotos, the first one is created by printing the array that contains the photo, and the second one by printing directly to the file with no user-created array


 
----------

Reset Camera
-------------
echo -n -e \\x56\\x00\\x26\\x00 > /dev/ttyMFD1

Return: 76 00 26 00

----------
Send Take Picture
-------------
echo -n -e \\x56\\x00\\x36\\x01\\x00 > /dev/ttyMFD1

Return: 76 00 36 00 00 

----------
Read JPEG file size
-------------
echo -n -e \\x56\\x00\\x34\\x01\\x00 > /dev/ttyMFD1

Return: 76 00 34 00 04 00 00 XH XL

----------
Read JPEG file content
-------------
56 00 32 0C 00 0A 00 00 MH ML 00 00 KH KL XX XX 

00 00 MH ML = Starting Address

00 00 KH KL = Length of JPEG file 

XX XX = 00 0A	Recommended

echo -n -e \\x56\\x00\\x32\\x0c\\x00\\x0a\\x00\\x00\\xMH\\xML\\x00\\x00\\xKH\\xKL\\xXX\\xXX > /dev/ttyMFD1

Return: JPEG File

----------
Stop Taking Pictures
-------------
echo -n -e \\x56\\x00\\x34\\x01\\x03 > /dev/ttyMFD1

Return: 76 00 36 00 00 

----------




