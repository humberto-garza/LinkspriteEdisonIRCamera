# LinkspriteEdisonIRCamera
Linksprite Infrared Camera Edison Library

Edison JPEG Linksprite Commands
===================
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
