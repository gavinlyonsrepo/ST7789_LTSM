/*!
	@file    BITMAP.ino
	@author  Gavin Lyons
	@brief   Example ino file for st7789 bitmap tests + FPS bitmap test.
	@details Arduino library ST7789_LTSM library. See USER OPTIONS 1-2 in SETUP function  
	@test
		-# Test 300 Sprite 
		-# Test 301 icons
		-# Test 302 bi-color small image
		-# Test 303 bi-color full screen image 128x128
		-# Test 304 16 bit color image from a data array
		-# Test 305 16 bit color image data from a data array
		-# Test 306 8 bit color image data from a data array
		-# Test 601 FPS bitmap results to serial port
*/

// libraries
#include "ST7789_LTSM.hpp"
// Font data
#include "fonts_LTSM/FontDefault_LTSM.hpp"
#include "fonts_LTSM/FontMega_LTSM.hpp"
// bitmap test data ~53K
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_16color1.hpp"
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_16color2.hpp"
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_8color.hpp"
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_bicolor.hpp"

/// @cond

//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

ST7789_LTSM myTFT;

bool bhardwareSPI = true;  // true for hardware spi, false for software

void setup(void) {
  Serial.begin(38400);  // optional
  delay(1000);

  // === USER OPTION 1 SPI_SPEED + TYPE ===
  int8_t DC_TFT = 5;
  int8_t RST_TFT = 4;
  int8_t CS_TFT = 15;
  if (bhardwareSPI == true) {          // hw spi
    uint32_t TFT_SCLK_FREQ = 8000000;  // Spi freq in Hertz
    myTFT.setupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT);
  } else {                        // sw spi
    uint16_t SWSPICommDelay = 0;  // optional SW SPI GPIO delay in uS
    int8_t SDIN_TFT = 13;
    int8_t SCLK_TFT = 12;
    myTFT.setupGPIO_SPI(SWSPICommDelay, RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
  }
  // ===
  // ===  USER OPTION 2 Screen Setup ===
  uint16_t OFFSET_COL = 0;    // These offsets can be adjusted for any issues->
  uint16_t OFFSET_ROW = 0;    // with screen manufacture tolerance/defects
  uint16_t TFT_WIDTH = 240;   // Screen width in pixels
  uint16_t TFT_HEIGHT = 320;  // Screen height in pixels
  myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
  // ===
  myTFT.TFTST7789Initialize();
}


void loop(void) {
  myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
  Test300();
  Test301();
  Test302();
  Test303();
  Test304();
  Test305();
  Test306();
  Test603();
  EndTests();
}

//  Function Space
/*!
	@brief  "sprite demo" 32 x 32 with LBLUE background
*/
void Test300(void) {
  // Test 300-A test 16-bit color Sprite
  // Draw as sprite, without background , 32 X 32 .bakcground color = ST7375_LBLUE
  // Green bakcground screen
  myTFT.fillScreen(myTFT.C_GREEN);
  delay(TEST_DELAY2);
  myTFT.drawSpriteData(55, 55, SpriteTest16, 32, 32, myTFT.C_LBLUE, false);
  myTFT.drawSpriteData(100, 40, SpriteTest16, 32, 32, myTFT.C_LBLUE, true);
  delay(TEST_DELAY5);

  // Test 300-B test 16-bit color Sprite
  // Draw as sprite, without background , 32 X 32 .bakcground color = ST7375_LBLUE
  // Bitmap background screen
  myTFT.drawBitmap16Data(50, 50, PosterImage, 80, 48);
  delay(TEST_DELAY2);

  myTFT.drawSpriteData(80, 50, SpriteTest16, 32, 32, myTFT.C_LBLUE, false);
  myTFT.drawSpriteData(60, 80, SpriteTest16, 32, 32, myTFT.C_LBLUE, false);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}


/*! @brief  small bitmap,*/
void Test301(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  char teststr1[] = "Test 301";
  myTFT.writeCharString(90, 90, teststr1);
  // TOP small bitmap box
  myTFT.drawBitmap(40, 40, 16, 8, myTFT.C_BLACK, myTFT.C_WHITE, SignalIcon);
  myTFT.drawBitmap(60, 40, 16, 8, myTFT.C_BLACK, myTFT.C_WHITE, MsgIcon);
  myTFT.drawBitmap(80, 40, 8, 8, myTFT.C_BLACK, myTFT.C_WHITE, AlarmIcon);
  myTFT.drawBitmap(110, 40, 16, 8, myTFT.C_BLACK, myTFT.C_WHITE, BatIcon);
  // second box
  myTFT.drawBitmap(35, 70, 16, 8, myTFT.C_GREEN, myTFT.C_BLACK, PowerIcon);
  myTFT.drawBitmap(55, 70, 16, 8, myTFT.C_RED, myTFT.C_YELLOW, SpeedIcon);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}  // end of Test 301

/*!
	@brief  Test 302 bi-color small image 20x24
*/
void Test302(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  char teststr1[] = "Test 302";
  myTFT.writeCharString(50, 50, teststr1);
  myTFT.drawBitmap(80, 60, 40, 16, myTFT.C_CYAN, myTFT.C_BLACK, SunTextImage);
  myTFT.drawBitmap(20, 100, 40, 16, myTFT.C_RED, myTFT.C_BLACK, SunTextImage);
  myTFT.drawBitmap(30, 140, 40, 16, myTFT.C_YELLOW, myTFT.C_RED, SunTextImage);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test303 bi-color full screen image 128x128
*/
void Test303(void) {
  char teststr1[] = "Test 303";
  myTFT.writeCharString(50, 50, teststr1);
  delay(TEST_DELAY2);
  myTFT.drawBitmap(50, 65, 128, 128, myTFT.C_WHITE, myTFT.C_GREEN, ArrowImage);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test304 16 bit color image from a data array
*/
void Test304(void) {
  char teststr1[] = "Test 304";
  myTFT.writeCharString(50, 50, teststr1);
  delay(TEST_DELAY2);
  myTFT.drawBitmap16Data(65, 65, MotorImage, 128, 128);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test305 16 bit color image data from a data array
*/
void Test305(void) {
  char teststr1[] = "Test 305";
  myTFT.writeCharString(50, 50, teststr1);
  delay(TEST_DELAY2);
  myTFT.drawBitmap16Data(65, 65, PosterImage, 80, 48);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test306 8 bit color image data from a data array
*/
void Test306(void) {
  char teststr1[] = "Test 306";
  myTFT.writeCharString(50, 50, teststr1);
  delay(TEST_DELAY2);
  myTFT.drawBitmap8Data(65, 65, ColorTestImage, 96, 96);
  delay(TEST_DELAY5);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test603 frame rate per second FPS ,results to serial port
*/
void Test603(void) {
  // Values to count frame rate per second
  long previousMillis = 0;
  long lastFramerate = 0;
  long currentFramerate = 0;
  myTFT.setFont(FontMega);
  uint16_t count = 0;
  uint16_t seconds = 0;
  uint16_t fps = 0;
  char teststr1[] = "Test 601 FPS, Output Results to USB port";
  myTFT.writeCharString(20, 50, teststr1);
  delay(TEST_DELAY5);
  myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);

  while (1) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 1000)  // every second
    {
      fps = currentFramerate - lastFramerate;
      lastFramerate = currentFramerate;
      previousMillis = currentMillis;
      seconds++;
      if (seconds == 20)
        break;
    }
    currentFramerate++;
    count++;

    //  ** Code to test **
    myTFT.drawBitmap16Data(60, 120, MotorImage, 128, 128);  //myTFT.fillScreen(randomColor );
    myTFT.setCursor(60, 90);
    myTFT.print(fps);
    //   **
  }  // end of while

  // Report results to usb
  printf("Seconds :: %u \n", seconds);
  printf("Count :: %u \n", count);
  printf("FPS :: %u \n", fps);
  // Print to screen
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setCursor(0, 50);
  myTFT.print("Seconds : ");
  myTFT.println(seconds);
  myTFT.print("Count : ");
  myTFT.println(count);
  myTFT.print("FPS : ");
  myTFT.print(fps);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void) {
  char teststr1[] = "Tests over";
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.writeCharString(50, 50, teststr1);
  delay(TEST_DELAY5);
  myTFT.TFTPowerDown();
  Serial.println("Tests Over");
  while (1) {};
}
// *************** EOF ****************

/// @endcond
