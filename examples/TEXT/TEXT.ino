/*!
	@file TEXT.ino
	@brief Library test file, tests text & fonts.
	@author Gavin Lyons.
	@note See USER OPTIONS 1-2 in SETUP function.
	@test
	-# Test 701 Print out all fonts with writeCharString
	-# Test 702 Print out all fonts with print
	-# Test 703 Misc print class tests(string object, println invert, wrap, base nums etc)
*/


// libraries 
#include "ST7789_LTSM.hpp"
// Included Fonts 
#include <fonts_LTSM/FontArialBold_LTSM.hpp>
#include <fonts_LTSM/FontArialRound_LTSM.hpp>
#include <fonts_LTSM/FontDefault_LTSM.hpp>
#include <fonts_LTSM/FontGll_LTSM.hpp>
#include <fonts_LTSM/FontGroTeskBig_LTSM.hpp>
#include <fonts_LTSM/FontGroTesk_LTSM.hpp>
#include <fonts_LTSM/FontHallfetica_LTSM.hpp>
#include <fonts_LTSM/FontInconsola_LTSM.hpp>
#include <fonts_LTSM/FontMega_LTSM.hpp>
#include <fonts_LTSM/FontMint_LTSM.hpp>
#include <fonts_LTSM/FontOrla_LTSM.hpp>
#include <fonts_LTSM/FontPico_LTSM.hpp>
#include <fonts_LTSM/FontRetro_LTSM.hpp>
#include <fonts_LTSM/FontSevenSeg_LTSM.hpp>
#include <fonts_LTSM/FontSinclairS_LTSM.hpp>
#include <fonts_LTSM/FontSixteenSeg_LTSM.hpp>


/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000	  // mS
#define TEST_DELAY2 2000	  // mS
#define TEST_DELAY5 5000	  // mS
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

ST7789_LTSM myTFT;
bool bhardwareSPI = true; // true for hardware spi, false for software

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

//  MAIN loop
void loop(void)
{
	Test701();
	Test702();
	Test703();
	EndTests();
}
//

// Function Space
void Test701(void) {

	Serial.println("Test 701: Print out all fonts with writeCharString");
	char teststr1[] = "Default ";
	char teststr2[] = "GLL ";
	char teststr3[] = "Pico ";
	char teststr4[] = "Sinclair ";
	char teststr5[] = "Orla ";
	char teststr6[] = "Retro ";
	char teststr7[] = "Mega";
	char teststr8[] = "Arial b";
	char teststr9[] = "Hall ";
	char teststr10[] = "Arial R";
	char teststr11[] = "GroTesk";

	myTFT.fillScreen(myTFT.C_BLACK);

	myTFT.setFont(FontDefault);
	myTFT.writeCharString(5, 45, teststr1);
	myTFT.setFont(FontGll);
	myTFT.writeCharString(5, 55, teststr2);
	myTFT.setFont(FontPico);
	myTFT.writeCharString(5, 70, teststr3);
	myTFT.setFont(FontSinclairS);
	myTFT.writeCharString(5, 130, teststr4);
	myTFT.setFont(FontOrla);
	myTFT.writeCharString(5, 170, teststr5);
	myTFT.setFont(FontRetro);
	myTFT.writeCharString(5, 200, teststr6);
	DisplayReset();

	myTFT.setFont(FontMega);
	myTFT.writeCharString(5, 52, teststr7);
	myTFT.setFont(FontArialBold);
	myTFT.writeCharString(5, 80, teststr8);
	myTFT.setFont(FontHallfetica);
	myTFT.writeCharString(5, 120, teststr9);
	myTFT.setFont(FontArialRound);
	myTFT.writeCharString(5, 200, teststr10);
	DisplayReset();

	myTFT.setFont(FontGroTesk);
	myTFT.writeCharString(5, 55, teststr11);

	char teststr12[] = "16";
	char teststr13[] = "7";
	char teststr14[] = "inco";
	char teststr15[] = "GB";
	char teststr16[] = "Mint";
	myTFT.setFont(FontSixteenSeg);
	myTFT.writeCharString(5,120, teststr12);
	DisplayReset();

	myTFT.setFont(FontSevenSeg);
	myTFT.writeCharString(5, 55, teststr13);
	myTFT.setFont(FontInconsola);
	myTFT.writeCharString(5, 120, teststr14);
	DisplayReset();
	
	myTFT.setFont(FontGroTeskBig);
	myTFT.writeCharString(5, 55, teststr15);
	myTFT.setFont(FontMint);
	myTFT.writeCharString(5, 120, teststr16);
	DisplayReset();
}

void Test702(void)
{
	Serial.println("Test 702: Print class methods");

	// Test Fonts default +  + pico+ sinclair + retro
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);

	myTFT.setCursor(5,55);
	myTFT.setFont(FontDefault);
	myTFT.print("Default ");
	myTFT.print(-43);

	myTFT.setCursor(5,75);
	myTFT.setFont(FontGll);
	myTFT.print("GLL ");
	myTFT.print(123.284,1); // print 123.3

	myTFT.setCursor(5,125);
	myTFT.setFont(FontPico);
	myTFT.print("pico ");
	myTFT.print(747);

	myTFT.setCursor(5,170);
	myTFT.setFont(FontSinclairS);
	myTFT.print("sinclair ");
	myTFT.print(456);

	myTFT.setCursor(5,200);
	myTFT.setFont(FontRetro);
	myTFT.print("retro  ");
	myTFT.print(-3.14);
	DisplayReset();

	// Test font  mega Arial bold and Hallf.
	myTFT.setCursor(5,55);
	myTFT.setFont(FontMega);
	myTFT.println("mega ");
	myTFT.print(61);

	myTFT.setCursor(5,100);
	myTFT.setFont(FontArialBold);
	myTFT.println("A bold");
	myTFT.print(12.08);

	myTFT.setCursor(5,160);
	myTFT.setFont(FontHallfetica);
	myTFT.println("hall f");
	myTFT.print(1.48);
	DisplayReset();

	// Test Font orla + arial round

	myTFT.setFont(FontOrla);
	myTFT.setCursor(5,55);
	myTFT.println(-7.16);
	myTFT.print("Orla");

	myTFT.setCursor(5,120);
	myTFT.setFont(FontArialRound);
	myTFT.println(-8.16);
	myTFT.print("a rnd");
	DisplayReset();

	// Test font grotesk + sixteen segment
	myTFT.setCursor(5,75);
	myTFT.setFont(FontGroTesk);
	myTFT.println("GROTESK");
	myTFT.print(1.78);

	myTFT.setCursor(0,180);
	myTFT.setFont(FontSixteenSeg);
	myTFT.print(1245);
	DisplayReset();
	
	// Test font seven segment 
	myTFT.setCursor(5,55);
	myTFT.setFont(FontSevenSeg);
	myTFT.println(12);
	DisplayReset();

	myTFT.setFont(FontMint);
	myTFT.println("MINT");;
	myTFT.print("9`C");
	DisplayReset();
}

void Test703(void)
{
	Serial.println("Test 703: Misc print class");
	//Inverted print fonts 1-6
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);
	myTFT.setFont(FontDefault);

	myTFT.setFont(FontSinclairS);
	myTFT.setCursor(5,55);
	myTFT.print(-49);

	myTFT.setFont(FontMega);
	myTFT.setCursor(5,90);
	myTFT.print(112.09);
	myTFT.setCursor(5,120);
	myTFT.print("ABCD");

	myTFT.setFont(FontGll);
	myTFT.setCursor(5,200);
	myTFT.print("ER");

	DisplayReset();

	// Inverted 
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);

	myTFT.setFont(FontArialBold);
	myTFT.setCursor(5,55);
	myTFT.print("INVERT");
	myTFT.setCursor(5,85);
	myTFT.print(-94.982, 2);

	myTFT.setFont(FontHallfetica);
	myTFT.setCursor(5,120);
	myTFT.print("INVERT");
	myTFT.setCursor(5,200);
	myTFT.print(123456);
	DisplayReset();

	// Test print with DEC BIN OCT HEX
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
	uint8_t numPos = 47;
	myTFT.setFont(FontMega);
	myTFT.setCursor(5,55);
	myTFT.print(numPos , DEC); // 47
	myTFT.setCursor(5,75);
	myTFT.print(numPos , BIN); // 10111
	myTFT.setCursor(5,120);
	myTFT.print(numPos , OCT); // 57
	myTFT.setCursor(5,160);
	myTFT.print(numPos , HEX); // 2F

	DisplayReset();

	//text wrap with print
	myTFT.setCursor(5,70);
	myTFT.print("12345678901234567890ABCDEFGHIJ");
	DisplayReset();
	
}


void DisplayReset(void)
{
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void)
{
	myTFT.setFont(FontGroTesk);
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setCursor(15,50);
	myTFT.print("Tests over");
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTPowerDown();
	Serial.println("Tests Over");
  while(1){};
}

/// @endcond
