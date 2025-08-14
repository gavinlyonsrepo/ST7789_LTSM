/*!
	@file    DEMOS.ino
	@author  Gavin Lyons
	@brief   Example file for st7789 library.  ST7789_LTSM library. demo
	@details See USER OPTIONS 1-2 in SETUP function
				   If dislib16_ADVANCED_GRAPHICS_ENABLE is not enabled it will not compile,
				   it is in file common_dis16_data_LTSM.hpp from display16_graphics_LTSM library.
	@test
		-# Test 401 gauge demo 
*/

// libraries 
#include "ST7789_LTSM.hpp"
#include "fonts_LTSM/FontDefault_LTSM.hpp" // Depends

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif
#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE 
#pragma message("gll: dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. it is required for this demo")
#endif

ST7789_LTSM myTFT;
bool bhardwareSPI = true; // true for hardware spi, false for software

// Demo 
// Gauge properties
const int GAUGE_WIDTH = 20;
const int GAUGE_HEIGHT = 140;
const int GAUGE_X_START = 20;
const int GAUGE_Y_START = 40;
const int GAUGE_SPACING = 70;
// Store previous values to prevent redundant redraws
float prevVal1 = -1, prevVal2 = -1, prevVal3 = -1;
void updateGauges(float t);
void drawGauge(int x, int y, uint16_t color, float value);
void drawGaugeDemo(uint16_t  countLimit = 50);
uint16_t generateColor(uint8_t value);
int mapValue(int x, int in_min, int in_max, int out_min, int out_max);

void setup(void) {
  Serial.begin(38400);  // optional
  delay(1000);
  Serial.println("Start");
  // === USER OPTION 1 SPI_SPEED + TYPE ===
  int8_t DC_TFT = 5;
  int8_t RST_TFT = 4;
  int8_t CS_TFT = 15;
  if (bhardwareSPI == true) {          // hw spi
    uint32_t TFT_SCLK_FREQ = 12000000;  // Spi freq in Hertz
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
	drawGaugeDemo(50);
	EndTests();
}


// Function Space 
void EndTests(void)
{
	myTFT.TFTPowerDown(); 
	Serial.println("End");
  while(1){};
}

void drawGaugeDemo(uint16_t countLimit)
{
	Serial.print("Demo ends at : ");
  Serial.println(countLimit);
	myTFT.setFont(FontDefault);
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
	myTFT.fillScreen(myTFT.C_BLACK);
	float t = 0;
	while (countLimit-- > 1 ) 
	{
		updateGauges(t);
		t += 0.1;
		//Serial.print(countLimit);
		delay(500);
	}
	myTFT.fillScreen(myTFT.C_BLACK);
	Serial.println("Demo Over");
}

void drawGauge(int x, int y, uint16_t color, float value) {
	// Draw border
	myTFT.drawRectWH(x - 2, y - 2, GAUGE_WIDTH + 4, GAUGE_HEIGHT + 4, myTFT.C_WHITE);
	// Clear inside
	myTFT.fillRectBuffer(x, y, GAUGE_WIDTH, GAUGE_HEIGHT, myTFT.C_BLACK);
	// Draw fill
	int fillHeight = static_cast<int>(GAUGE_HEIGHT * value);
	if (fillHeight == 0) fillHeight =1;
	myTFT.fillRect(x, y + (GAUGE_HEIGHT - fillHeight), GAUGE_WIDTH, fillHeight, color);
}

void updateGauges(float t) {
	// Calculate values
	float val1 = (std::sin(t) + 1) / 2;
	float val2 = (std::cos(t) + 1) / 2;
	float val3 = ((std::sin(t) * std::cos(t)) + 1) / 2;
	// Convert float (0 to 1) to integer (1 to 127) range for color mapping
	uint8_t mappedVal1 = map(static_cast<int>(val1 * 100), 0, 100, 1, 127);
	uint8_t mappedVal2 = map(static_cast<int>(val2 * 100), 0, 100, 1, 127);
	uint8_t mappedVal3 = map(static_cast<int>(val3 * 100), 0, 100, 1, 127);
	// Generate dynamic colors
	uint16_t color1 = generateColor(mappedVal1);
	uint16_t color2 = generateColor(mappedVal2);
	uint16_t color3 = generateColor(mappedVal3);

	// Draw gauges with dynamic colors
	drawGauge(GAUGE_X_START, GAUGE_Y_START, color1, val1);
	drawGauge(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START, color2, val2);
	drawGauge(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START, color3, val3);

	char buffer[6]; // To store formatted text
	// Draw gauges only if values changed
	if (val1 != prevVal1) {
		drawGauge(GAUGE_X_START, GAUGE_Y_START, color1, val1);
		sprintf(buffer, "%.2f", val1);
		myTFT.fillRectBuffer(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.C_BLACK); // Clear previous text
		myTFT.setCursor(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		prevVal1 = val1;
	}
	if (val2 != prevVal2) {
		drawGauge(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START, color2, val2);
		sprintf(buffer, "%.2f", val2);
		myTFT.fillRectBuffer(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.C_BLACK);
		myTFT.setCursor(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		prevVal2 = val2;
	}
	if (val3 != prevVal3) {
		drawGauge(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START, color3, val3);
		sprintf(buffer, "%.2f", val3);
		myTFT.fillRectBuffer(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.C_BLACK);
		myTFT.setCursor(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		prevVal3 = val3;
	}
}
/*!
	@brief This function is designed to return a color in the 16-bit 
		RGB format based on the input value, creating a smooth transition 
		between colors in different stages. 
	@details The gradient is calculated by 
		cycling through different color transitions as val increases, 
		allowing for smooth visual effects such as changing hues 
		or creating rainbow-like effects on an LCD. 1-127 will
		This will create a gradient color between blue and red based on the value.
*/
uint16_t generateColor(uint8_t value)
{
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
	uint8_t segment = value / 32;
	switch(segment) 
	{
		case 0: red = 0; green = 2 * (value % 32); blue = 31; break;
		case 1: red = 0; green = 63; blue = 31 - (value % 32); break;
		case 2: red = value % 32; green = 63; blue = 0; break;
		case 3: red = 31; green = 63 - 2 * (value % 32); blue = 0; break;
	}
	return (red << 11) + (green << 5) + blue;
}

///@endcond
