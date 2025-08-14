/*!
	@file    GRAPHICS.ino
	@author  Gavin Lyons, Liontron systems.
	@brief   Example ino file for st7789 driver, ST7789_LTSM library. Graphics methods testing
	@details See USER OPTIONS 1-2 in SETUP function, dislib16_ADVANCED_GRAPHICS_ENABLE in file 
		       common_dis16_data_LTSM.hpp from display16_graphics_LTSM library.
		       needs to be enabled for all tests 905-910
	@test
		-# Test 901  pixels and lines
		-# Test 902  rectangles
		-# Test 903  Circle
		-# Test 904  Triangles
		-# Test 905 Polygons
		-# Test 906 Dot Grid
		-# Test 907 Quadrilateral
		-# Test 908 Ellipse
		-# Test 909 Draw Arc
		-# Test 910 Draw Line at angle function
*/

// libraries 
#include "ST7789_LTSM.hpp"
// Included Fonts 
#include <fonts_LTSM/FontDefault_LTSM.hpp>
#include <fonts_LTSM/FontGroTesk_LTSM.hpp>
///@cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

//  Test timing related defines
#define TEST_DELAY1 1000	  // mS
#define TEST_DELAY2 2000	  // mS
#define TEST_DELAY5 5000	  // mS

uint16_t TFT_WIDTH = 240;  // Screen width in pixels
uint16_t TFT_HEIGHT = 320; // Screen height in pixels

ST7789_LTSM myTFT;
bool bhardwareSPI = true; // true for hardware spi, false for software

void setup(void) {
  Serial.begin(38400);  // optional
  delay(1000);
  Serial.println("Start");
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
  myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
  // ===

  myTFT.TFTST7789Initialize();
}

// MAIN loop

void loop(void)
{
	Test901();
	Test902();
	Test903();
#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
	Test905();
	Test906();
	Test907();
	Test908();
	Test909();
	Test910(5);
#endif
	EndTests();
}
// End OF MAIN

//  Section ::  Function Space

/*!
	@brief   pixels and lines
*/
void Test901(void)
{
	delay(TEST_DELAY1);
	myTFT.fillScreen(myTFT.C_BLACK);

	myTFT.drawPixel(85, 55, myTFT.C_WHITE);
	myTFT.drawPixel(87, 59, myTFT.C_WHITE);
	myTFT.drawPixel(110, 79, myTFT.C_WHITE);
	myTFT.drawLine(80, 80, 40, 40, myTFT.C_RED);
	myTFT.drawFastVLine(100, 100, 100, myTFT.C_GREEN);
	myTFT.drawFastHLine(160, 60, 70, myTFT.C_YELLOW);

	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  rectangles
*/
void Test902(void)
{
	myTFT.drawRectWH(65, 65, 20, 20, myTFT.C_RED);
	if(myTFT.fillRectBuffer(105, 75, 20, 20, myTFT.C_YELLOW) != 0) //uses spiwrite
	{
		printf("Error Test902 1: Error in the fillRectangle function\r\n");
	}
	myTFT.fillRect(160, 55, 20, 20, myTFT.C_GREEN);
	myTFT.drawRoundRect(15, 160, 50, 50, 5, myTFT.C_CYAN);
	myTFT.fillRoundRect(70, 160, 50, 50, 10, myTFT.C_WHITE);

	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  circles + triangles
*/
void Test903(void)
{
	myTFT.drawCircle(90, 160, 15, myTFT.C_GREEN);
	myTFT.fillCircle(140, 80, 15, myTFT.C_YELLOW);
	myTFT.drawTriangle(35, 80, 85, 40, 115, 80, myTFT.C_CYAN);
	myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.C_RED);

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
	delay(TEST_DELAY5);
	myTFT.TFTPowerDown();
	Serial.println("Tests Over");
  while(1){};
}

#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
void Test905(void)
{
	Serial.println("Test 905 : polygons");
	myTFT.fillScreen(myTFT.C_BLACK);
	//drawPolygon( x,  y, sides,  diameter,  rotation, color);
	// decagon
	myTFT.drawPolygon(50, 100, 10, 50, 0, false, myTFT.C_RED);
	// octagon
	myTFT.drawPolygon(150, 100, 8, 50, 0, false, myTFT.C_GREEN);
	// hexagon rotated
	myTFT.drawPolygon(50, 200, 6, 50, 45, false, myTFT.C_WHITE);
	// pentagon
	myTFT.drawPolygon(150, 200, 5, 50, 0, false, myTFT.C_TAN);
	delay(TEST_DELAY2);
	// decagon
	myTFT.drawPolygon(50, 100, 10, 50, 0, true, myTFT.C_RED);
	// octagon
	myTFT.drawPolygon(150, 100, 8, 50, 0, true, myTFT.C_GREEN);
	// hexagon rotated
	myTFT.drawPolygon(50, 200, 6, 50, 45, true, myTFT.C_WHITE);
	// pentagon
	myTFT.drawPolygon(150, 200, 5, 50, 0, true, myTFT.C_TAN);
	delay(TEST_DELAY2);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test906(void) {
	Serial.println("Test 906 : dot grid");
	myTFT.fillScreen(myTFT.C_BLACK);
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myTFT.drawDotGrid(0, 0, TFT_WIDTH, TFT_HEIGHT, 5, myTFT.C_GREEN);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test907() {
	Serial.println("Test 907 : quadrilateral");
	// Define the  quadrilateral vertices for different shapes
	// (quadrilateral rectangle )
	int16_t x0 = 20, y0 = 20; // First vertex
	int16_t x1 = 50, y1 = 20;
	int16_t x2 = 50, y2 = 50;
	int16_t x3 = 20, y3 = 50;
	//  (Parallelogram )
	int16_t x4 = 180, y4 = 240;  // First vertex
	int16_t x5 = 230, y5 = 250;
	int16_t x6 = 220, y6 = 290;
	int16_t x7 = 170, y7 = 280;
	//  (Trapezoid 1)
	int16_t x8 = 106, y8 = 107;  // First vertex
	int16_t x9 = 112, y9 = 128;
	int16_t x10 = 134, y10 = 128;
	int16_t x11 = 153, y11 = 107;
	//  (Trapezoid 2)
	int16_t x12 = 16, y12 = 207;  // First vertex
	int16_t x13 = 29, y13 = 242;
	int16_t x14 = 85, y14 = 242;
	int16_t x15 = 46, y15 = 207;

	myTFT.drawQuadrilateral(x0, y0, x1, y1, x2, y2, x3, y3, myTFT.C_RED);
	myTFT.drawQuadrilateral(x4, y4, x5, y5, x6, y6, x7, y7, myTFT.C_GREEN);
	myTFT.drawQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myTFT.C_BLUE);
	myTFT.drawQuadrilateral(x12, y12, x13, y13, x14, y14, x15, y15, myTFT.C_YELLOW);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.fillQuadrilateral(x0, y0, x1, y1, x2, y2, x3, y3, myTFT.C_RED);
	myTFT.fillQuadrilateral(x4, y4, x5, y5, x6, y6, x7, y7, myTFT.C_GREEN);
	myTFT.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myTFT.C_BLUE);
	myTFT.fillQuadrilateral(x12, y12, x13, y13, x14, y14, x15, y15, myTFT.C_YELLOW);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test908(void) {
	Serial.println("Test 908 : Ellipse");
	myTFT.fillScreen(myTFT.C_BLACK);
	// Define ellipses at different positions
	// Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
	myTFT.drawEllipse(TFT_WIDTH / 4,TFT_HEIGHT / 4, 20, 40, false, myTFT.C_WHITE);  // Top-left
	myTFT.drawEllipse(3 * TFT_WIDTH / 4,TFT_HEIGHT / 4, 25, 50, false, myTFT.C_RED); // Top-right
	myTFT.drawEllipse(3 * TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 50, 35, false, myTFT.C_YELLOW); // Bottom-right
	delay(TEST_DELAY5);
	myTFT.drawEllipse(TFT_WIDTH / 4,TFT_HEIGHT / 4, 20, 40, true, myTFT.C_WHITE);
	myTFT.drawEllipse(3 * TFT_WIDTH / 4,TFT_HEIGHT / 4, 25, 50, true, myTFT.C_RED);
	myTFT.drawEllipse(3 * TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 50, 35, true, myTFT.C_YELLOW);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test909(void) {
	Serial.println("Test 909 : Drawing Arc: drawArc");
	myTFT.fillScreen(myTFT.C_BLACK);
	int16_t centerX = 120;  // X-coordinate of the circle center
	int16_t centerY = 160;  // Y-coordinate of the circle center
	int16_t radius = 50;    // Radius of the circle
	//drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
	// _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
	// XXX 270 XXX
	// 180 XXX 000
	// XXX 090 XXX
	// Draw the full lower half (0° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 10, 0.0f, 180.0f, myTFT.C_GREEN);
	// Draw the full upper half (180° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.C_BLUE);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
	// Draw the first quarter (0° to 90°)
	myTFT.drawArc(centerX, centerY, radius, 5, 0.0f, 90.0f, myTFT.C_RED);
	// Draw the second quarter (90° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 5, 90.0f, 180.0f, myTFT.C_YELLOW);
	// Draw the third quarter (180° to 270°)
	myTFT.drawArc(centerX, centerY, radius, 5, 180.0f, 270.0f, myTFT.C_CYAN);
	// Draw the fourth quarter (270° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 5, 270.0f, 360.0f, myTFT.C_MAGENTA);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test910(uint8_t revolutions)
{
	Serial.println("Test 910 : Draw line at angle function");
	myTFT.drawCircle(84, 84, 60, myTFT.C_GREEN);
	myTFT.drawCircle(84, 84, 59, myTFT.C_GREEN);
	for (uint8_t j =0 ; j< revolutions ; j++)
	{
		for (int i = 0; i <= 361; i++)
		{
			//drawLineAngle(x, y, angle, start,length, offset,  color);
			myTFT.drawLineAngle(84, 84, i-1, 0 , 58, -90, myTFT.C_BLACK);//erase last needle
			myTFT.drawLineAngle(84, 84, i, 0, 58, -90, myTFT.C_GREEN);//draw needle
			delay(15);
		}
	}
	myTFT.fillScreen(myTFT.C_BLACK);
}

#endif

///@endcond