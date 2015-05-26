# LinkspriteEdisonIRCamera
Linksprite Infrared Camera Edison Library

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
 - Between the RX and TX of your camera and the Arduino's connect a 10K resistor
 - Power your Camera with a separate Power Source since the Arduino Breakout Limits the Current
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




