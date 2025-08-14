/*!
	@file   FUNCTIONS_FPS.ino
	@author Gavin Lyons
	@brief  Example cpp file for st7789 driver.
			Function testing, rotate , modes, scroll etc.
			Also FPS frame rate per second test.
	@note  See USER OPTIONS 1-2 in SETUP function
	@test
		-# Test500 Scroll test 
		-# Test501 Color
		-# Test503 Rotate
		-# Test504 change modes test -> Invert, display on/off and Sleep.
		-# Test602 FPS frame rate per second test for text
		-# Test603 FPS frame rate per second test for graphics
*/

// libraries
#include "ST7789_LTSM.hpp"
// Font data
#include <fonts_LTSM/FontDefault_LTSM.hpp>
#include <fonts_LTSM/FontGroTesk_LTSM.hpp>
#include <fonts_LTSM/FontArialRound_LTSM.hpp>

/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000  // mS
#define TEST_DELAY2 2000  // mS
#define TEST_DELAY5 5000  // mS
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
  myTFT.fillScreen(myTFT.C_BLACK);
  Test500();
  Test501();
  Test503();
  Test504();
  Test602();
  Test603();
  EndTests();
}

void Test500(void) {
  Serial.println("Test 500: Scroll");
  myTFT.setFont(FontDefault);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  const uint8_t LINES = 10, LINE_SIZE = 10, LINE_OFFSET = 3, TOP_FIXED = 0, BOTTOM_FIXED = 0;
  char teststr1[] = "Scroll test";

  for (uint8_t i = 0; i < LINES; i++) {
    myTFT.writeCharString(5, LINE_OFFSET + i * LINE_SIZE, teststr1);
  }
  myTFT.setScrollDefinition(TOP_FIXED, BOTTOM_FIXED, 1);  // bottom-to-top
  uint8_t pos = LINE_OFFSET;
  for (uint8_t i = 0; i < LINES; i++) {
    for (uint8_t j = 0; j < LINE_SIZE; j++) {
      myTFT.TFTVerticalScroll(pos + TOP_FIXED);
      pos++;
      // check pos if necessary: must be < tftTFT_HEIGHT - TOP_FIXED - BOTTOM_FIXED
    }
    delay(TEST_DELAY1);
  }
  myTFT.TFTNormalMode();
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test501(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  Serial.println("Test 501: Color Test:: Red,green,blue,yellow,white, black background");
  myTFT.fillRoundRect(8, 60, 24, 60, 8, myTFT.C_RED);
  myTFT.fillRoundRect(32, 60, 24, 60, 8, myTFT.C_GREEN);
  myTFT.fillRoundRect(56, 60, 24, 60, 8, myTFT.C_BLUE);
  myTFT.fillRoundRect(80, 60, 24, 60, 8, myTFT.C_YELLOW);
  myTFT.fillRoundRect(104, 60, 24, 60, 8, myTFT.C_WHITE);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Rotate
*/
void Test503() {
  char teststr0[] = "Rotate 0";    // normal
  char teststr1[] = "Rotate 90";   // 90
  char teststr2[] = "Rotate 180";  // 180
  char teststr3[] = "Rotate 270";  // 270

  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  myTFT.setRotation(myTFT.Degrees_0);
  myTFT.writeCharString(40, 40, teststr0);
  delay(TEST_DELAY2);

  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setRotation(myTFT.Degrees_90);
  myTFT.writeCharString(40, 40, teststr1);
  delay(TEST_DELAY2);

  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setRotation(myTFT.Degrees_180);
  myTFT.writeCharString(40, 40, teststr2);
  delay(TEST_DELAY2);

  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setRotation(myTFT.Degrees_270);
  myTFT.writeCharString(40, 40, teststr3);
  delay(TEST_DELAY2);

  myTFT.setRotation(myTFT.Degrees_0);
  myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  change modes test -> Invert, display on/off and Sleep.
*/
void Test504() {
  Serial.println("Test 504: Mode Tests");
  char teststr1[] = "Modes Test";
  myTFT.fillRoundRect(32, 60, 24, 60, 8, myTFT.C_RED);
  myTFT.fillRoundRect(56, 60, 24, 60, 8, myTFT.C_GREEN);
  myTFT.fillRoundRect(80, 60, 24, 60, 8, myTFT.C_BLUE);
  myTFT.fillRoundRect(104, 60, 24, 60, 8, myTFT.C_YELLOW);
  myTFT.fillRoundRect(124, 60, 24, 60, 8, myTFT.C_WHITE);
  myTFT.writeCharString(40, 140, teststr1);
  delay(TEST_DELAY2);

  // Invert on and off
  myTFT.TFTchangeInvertMode(true);
  Serial.println("Test 504-2: Mode Invert");
  delay(TEST_DELAY5);
  myTFT.TFTchangeInvertMode(false);
  delay(TEST_DELAY5);

  // Display on and off
  myTFT.TFTenableDisplay(false);
  Serial.println("Test 504-3: Mode Display off");
  delay(TEST_DELAY5);
  myTFT.TFTenableDisplay(true);
  delay(TEST_DELAY5);

  // Sleep on and off
  myTFT.TFTsleepDisplay(true);
  Serial.println("Test 504-4: Mode Sleep on");
  delay(TEST_DELAY5);
  myTFT.TFTsleepDisplay(false);
  delay(TEST_DELAY2);
  myTFT.fillScreen(myTFT.C_BLACK);
}


/*!
	@brief   Frame rate per second test, FPS test. text
*/
void Test602(void) {
  myTFT.setFont(FontArialRound);
  // Values to count frame rate per second
  long previousMillis = 0;
  long lastFramerate = 0;
  long currentFramerate = 0;
  uint16_t count = 0;
  uint16_t seconds = 0;
  uint16_t fps = 0;
  while (1) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000)  // every second
    {
      fps = currentFramerate - lastFramerate;
      lastFramerate = currentFramerate;
      previousMillis = currentMillis;
      seconds++;
      if (count >= 500) {
        Serial.print("FPS : ");
        Serial.println(fps);
        Serial.print("Seconds  : ");
        Serial.println(seconds);
        return;  // end if count gets to this
      }
    }
    currentFramerate++;
    count++;
    //  ** Code to test **
    // print seconds
    myTFT.setCursor(5, 55);
    myTFT.print("Secs :");
    myTFT.setCursor(120, 55);
    myTFT.print(seconds);
    // print fps
    myTFT.setCursor(5, 85);
    myTFT.print("FPS :");
    myTFT.setCursor(120, 85);
    myTFT.print(fps);
    // print count
    myTFT.setCursor(5, 125);
    myTFT.print("Count:");
    myTFT.setCursor(120, 125);
    myTFT.print(count);

    myTFT.setCursor(5, 165);
    myTFT.print("Testing!");
    //  ****
  }
}

/*!
	@brief   Frame rate per second test, FPS test. graphics
*/
void Test603(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setFont(FontArialRound);
  // Values to count frame rate per second
  long previousMillis = 0;
  long lastFramerate = 0;
  long currentFramerate = 0;
  uint16_t count = 0;
  uint16_t seconds = 0;
  uint16_t fps = 0;
  uint16_t shapeColor = 0x2222;
  while (1) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 1000)  // every second
    {
      fps = currentFramerate - lastFramerate;
      lastFramerate = currentFramerate;
      previousMillis = currentMillis;
      seconds++;
      shapeColor = rand() % 60000;
      if (count >= 1000) {
        Serial.print("FPS : ");
        Serial.println(fps);
        Serial.print("Seconds  : ");
        Serial.println(seconds);
        return;  // end if count gets to this
      }
    }
    currentFramerate++;
    count++;

    //  ** Code to test **
    // print fps
    myTFT.setCursor(5, 45);
    myTFT.print(fps);
    // print some graphics
    myTFT.drawCircle(90, 210, 10, shapeColor);
    myTFT.drawRectWH(40, 200, 20, 20, shapeColor << 1);
    myTFT.drawRoundRect(5, 200, 20, 20, 2, shapeColor >> 1);
    myTFT.fillCircle(90, 210, 10, shapeColor);
    myTFT.fillRect(40, 200, 20, 20, shapeColor << 1);
    myTFT.fillRoundRect(5, 200, 20, 20, 2, shapeColor >> 1);
    myTFT.fillTriangle(55, 120, 100, 90, 127, 120, shapeColor - 100);
    myTFT.fillTriangle(55, 120, 100, 160, 160, 160, shapeColor);
    //  ****
  }
}

/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void) {
  myTFT.setFont(FontGroTesk);
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  myTFT.setCursor(15, 50);
  myTFT.print("Tests over");
  delay(TEST_DELAY5);
  myTFT.TFTPowerDown();
  Serial.println("Tests Over");
  while(1){};
}

// *************** EOF ****************

/// @endcond
