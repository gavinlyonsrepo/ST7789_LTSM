# ST7789 Readme

[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

[![Image TFT](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789.jpg)

## Table of contents

  * [Overview](#overview)
  * [Installation](#installation)
  * [Dependency](#dependency)
  * [Software](#software)
      * [SPI](#spi)
      * [Bitmap](#bitmap)
  * [Hardware](#hardware)
  * [Tested](#tested)
  * [Output](#output)
  * [Notes](#notes)
     * [Display offsets](#display-offsets)

## Overview

* Name: ST7789_LTSM
* Description:

C++ Library for a ST7789 TFT SPI LCD for the Arduino Eco-system.

1. Arduino eco-system library.
2. Invert, Scroll, rotate and sleep control.
3. 16 ASCII fonts included, fonts can easily be removed or added.
4. Advanced graphics class included.
5. Advanced frame buffer mode included.
6. Bitmaps supported: 1, 8 and 16 bit.
7. Hardware & software SPI options
8. Code commented for 'doxygen' API generation tool.

## Software

## Installation

The library is included in the official Arduino library manger and the optimum way to install it is using the library manager which can be opened by the *manage libraries* option in Arduino IDE. 

## Dependency

This library requires the Arduino library 'display16_LTSM' as a dependency. display16_LTSM library contains
the graphics, bitmaps, and font methods as well as font data and bitmap test data. Its also 
where the user sets options(debug, advanced graphics and frame buffer mode). 'display16_LTSM' is also written by Author of this library. 
When you install 'ST7789_LTSM' with Arduino IDE. It should install 'display16_LTSM' as well after 
a prompt, if it does not you have to install it same way as 'ST7789_LTSM'.
The 'display16_LTSM' project and readme is at [URL github link](https://github.com/gavinlyonsrepo/display16_LTSM)


### SPI

In the example ino files. There are sections in "setup()" function where user can make adjustments to select for SPI type used, and screen size.

1. USER OPTION 1 GPIO, SPI_SPEED + TYPE
2. USER OPTION 2 SCREEN SECTION

*USER OPTION 1 GPIO SPI SPEED*

Two different constructors which one is called depends on 'bhardwareSPI' in example files, 
true for hardware spi, false for software SPI.

Hardware SPI:

Here the user can pass the SPI Bus freq in Hertz, Currently set to 8 Mhz,
and the Reset, chip select and data or command line. Any GPIO can be used for these.
The MOSI and CLk are tied to default MCU SPI interface GPIO.

Software SPI:

The optional GPIO software uS delay,which by default is zero.
Setting this higher can be used to slow down Software SPI
which may be beneficial on Fast MCU's.
The 5 GPIO pins used. Any GPIO can be used for these.

*USER OPTION 2 Screen size + Offsets*

User can adjust screen pixel height, screen pixel width and x & y screen offsets.
These offsets can be used in the event of screen damage or manufacturing errors around edge
such as cropped data or defective pixels. The function TFTInitScreenSize sets them.

## Hardware

Connections as setup in HELLO_WORLD.ino  test file.

| TFT PinNum | Pindesc | Hardware SPI | Software SPI |
| --- | --- | --- | --- |
| 1 | LED | VCC | VCC |
| 2 | SCLK | MCU SPI CLK | GPIO12 |
| 3 | SDA | MCU MOSI CLK | GPIO13 |
| 4 | A0/DC | GPIO5 | GPIO5 |
| 5 | RESET | GPIO4 | GPIO4 |
| 6 | SS/CS | GPIO15 | GPIO15 |
| 7 | GND | GND | GND |
| 8 | VCC | VCC | VCC |

1. NOTE: Connect LED backlight pin 1 thru a 150R/220R ohm resistor to 3.3/5V VCC.
2. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
3. You can connect VCC to 5V if there is a 3.3 volt regulator on back of TFT module.
4. Pick any GPIO you like but on HW SPI mode SCLK and SDA will be tied to SPIO interface of MCU.
5. Backlight on/off control is left to user.
6. If no reset pin on display or hat, pass -1 as argument to disable.s

## Output

[![output pic](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789output.jpg)

## Notes

### Display offsets

The display initialisation requires an offset calculation which differs for different size and resolution displays.
This is in the code(Function AdjustWidthHeight()) but the many different size displays are not available for testing or dealt with.
If using a display other than 240x320(the default and size of ST7789 video ram , VRAM) and if user finds they cannot address all screen
or their data is offset. Try Setting the pixel width and height of your screen to 240X320 and do not write as 
much as possible to the part of the VRAM you cannot see.
For example  if you have a 240X280 display in 0 degree rotation, the VRAM is same for ALL displays sizes.
1. Set pixel Width = 240 and pixel height = 320
2. Do not write to the missing 40 pixels in the Y-axis, you still can but it is inefficient.

[![pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/offset_st7789.png)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/offset_st7789.png)

Also if you have a 240x135 size display and you see the issue described in 
[github issue 10 ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/issues/10) 
You can alternately apply the fix there to function AdjustWidthHeight().
