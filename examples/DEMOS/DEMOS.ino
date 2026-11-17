/*!
	@file    DEMOS.ino
	@author  Gavin Lyons
	@brief   Example file for st7789 library.  ST7789_LTSM library. demos
	@details See USER OPTIONS 1-2 in SETUP function
	@test
		-# Test 401 Vertical gauge demo 
		-# Test 402 Round gauge demo
*/

// libraries
#include "ST7789_LTSM.hpp"
#include "fonts_LTSM/FontDefault_LTSM.hpp"  // Depends
#include <fonts_LTSM/FontMega_LTSM.hpp>

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

ST7789_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software

// Demo 401
// Gauge properties
const int GAUGE_WIDTH = 20;
const int GAUGE_HEIGHT = 140;
const int GAUGE_X_START = 20;
const int GAUGE_Y_START = 40;
const int GAUGE_SPACING = 70;
const uint8_t GAUGE_COUNT = 3;
// What is currently on screen for each gauge, so only changes are redrawn
struct GaugeState {
	int fillHeight;  // fill height in pixels last drawn, -1 = nothing drawn yet
	uint16_t color;  // fill color last drawn
	int textValue;   // value * 100 last printed, -1 = nothing printed yet
};
GaugeState gauges[GAUGE_COUNT];



void setup(void) {
	Serial.begin(38400);  // optional
	delay(1000);
	Serial.println("Start");
	// === USER OPTION 1 SPI_SPEED + TYPE ===
	int8_t DC_TFT = 5;
	int8_t RST_TFT = 4;
	int8_t CS_TFT = 15;
	if (bhardwareSPI == true) {           // hw spi
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
void loop(void) {
	drawGaugeDemo401(50);
	drawGaugeDemo402(100);
	EndTests();
}


// Function Space
void EndTests(void) {
	myTFT.TFTPowerDown();
	Serial.println("End");
	while (1) {};
}

// demo 401
void drawGaugeDemo401(uint16_t countLimit) {
	Serial.print("Demo ends at : ");
	Serial.println(countLimit);
	myTFT.setFont(FontDefault);
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
	myTFT.fillScreen(myTFT.C_BLACK);
	// Reset state and draw the static borders once, not every frame
	for (uint8_t i = 0; i < GAUGE_COUNT; i++) {
		gauges[i] = {-1, 0, -1};
		drawGaugeFrame(GAUGE_X_START + i * GAUGE_SPACING, GAUGE_Y_START);
	}
	float t = 0;
	while (countLimit-- > 1) {
		updateGauges(t);
		t += 0.1;
		//Serial.print(countLimit);
		delay(500);
	}
	myTFT.fillScreen(myTFT.C_BLACK);
	Serial.println("Demo Over");
}

// Static white border, drawn once per gauge
void drawGaugeFrame(int x, int y) {
	myTFT.drawRectWH(x - 2, y - 2, GAUGE_WIDTH + 4, GAUGE_HEIGHT + 4, myTFT.C_WHITE);
}

// Redraws only what changed: no full clear, no black flash.
void drawGauge(uint8_t index, int x, int y, uint16_t color, float value) {
	GaugeState &g = gauges[index];
	int fillHeight = static_cast<int>(GAUGE_HEIGHT * value);
	if (fillHeight < 1) fillHeight = 1;

	if (fillHeight != g.fillHeight || color != g.color) {
		int top = y + (GAUGE_HEIGHT - fillHeight);
		if (g.fillHeight < 0 || color != g.color) {
			// First draw or color changed: repaint the fill in the new color directly over the old
			myTFT.fillRect(x, top, GAUGE_WIDTH, fillHeight, color);
		} else if (fillHeight > g.fillHeight) {
			// Same color, bar grew: draw only the new strip on top
			myTFT.fillRect(x, top, GAUGE_WIDTH, fillHeight - g.fillHeight, color);
		}
		if (g.fillHeight > fillHeight) {
			// Bar shrank: black out only the strip that was uncovered
			myTFT.fillRectBuffer(x, y + (GAUGE_HEIGHT - g.fillHeight), GAUGE_WIDTH, g.fillHeight - fillHeight, myTFT.C_BLACK);
		}
		g.fillHeight = fillHeight;
		g.color = color;
	}

	// Text: only reprint if the displayed 2 decimal value changed.
	// Text is always 4 chars so the background colour from setTextColor overwrites the old text.
	int textValue = static_cast<int>(value * 100 + 0.5f);
	if (textValue != g.textValue) {
		char buffer[16];
		snprintf(buffer, sizeof(buffer), "%d.%02d", textValue / 100, textValue % 100);
		myTFT.setCursor(x, y + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		g.textValue = textValue;
	}
}

void updateGauges(float t) {
	// Calculate values
	float val[GAUGE_COUNT];
	val[0] = (std::sin(t) + 1) / 2;
	val[1] = (std::cos(t) + 1) / 2;
	val[2] = ((std::sin(t) * std::cos(t)) + 1) / 2;
	for (uint8_t i = 0; i < GAUGE_COUNT; i++) {
		// Convert float (0 to 1) to integer (1 to 127) range for color mapping
		uint8_t mappedVal = map(static_cast<int>(val[i] * 100), 0, 100, 1, 127);
		uint16_t color = generateColor(mappedVal);
		drawGauge(i, GAUGE_X_START + i * GAUGE_SPACING, GAUGE_Y_START, color, val[i]);
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
uint16_t generateColor(uint8_t value) {
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
	uint8_t segment = value / 32;
	switch (segment) {
		case 0:
			red = 0;
			green = 2 * (value % 32);
			blue = 31;
			break;
		case 1:
			red = 0;
			green = 63;
			blue = 31 - (value % 32);
			break;
		case 2:
			red = value % 32;
			green = 63;
			blue = 0;
			break;
		case 3:
			red = 31;
			green = 63 - 2 * (value % 32);
			blue = 0;
			break;
	}
	return (red << 11) + (green << 5) + blue;
}

// demo 402
void drawGaugeDemo402(uint16_t countLimit) {
	Serial.print("Gauge Demo ends at : ");
	Serial.println(countLimit);
	int16_t currentValue = 100;
	int16_t oldValue = 100;
	myTFT.setFont(FontMega);
	char buffer[12];
	// Draw Gauge
	uint16_t count = 1;
	uint8_t x = 100;
	uint8_t y = 100;
	const int16_t minValue = 1;
	const int16_t maxValue = 255;
	uint8_t radius = 63;
	myTFT.drawCircle(x, y, radius, myTFT.C_RED);
	drawGaugeMarkers(x, y, radius, 150, 390, 1.3);
	drawGaugeMarkers(x, y, radius, 165, 375, 1.1);
	// Random number setup
	// Initialize random generator , optional
	// randomSeed(analogRead(A0));

	while (count++ < countLimit) {
		int step = random(-10, 11);  // Arduino built-in RNG
    currentValue += step;
    currentValue = constrain(currentValue, minValue, maxValue);
		myTFT.setCursor(0, 180);
		if (oldValue != currentValue) {
			drawPointer(currentValue, oldValue, x, y, radius, myTFT.C_GREEN, myTFT.C_BLACK);
			oldValue = currentValue;
		}
		myTFT.print(" Count :: ");
		myTFT.println(count);
		myTFT.print(" Value :: ");
		sprintf(buffer, "%03d", currentValue);
		myTFT.print(buffer);
	}
	myTFT.fillScreen(myTFT.C_BLACK);
	Serial.println("Gauge Demo Over ");
}

void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor) {
	int angle;
	// Loop through the specified angle range, drawing ticks every 30 degrees
	for (angle = startAngle; angle <= endAngle; angle += 30) {
		float angleRad, innerX, innerY, outerX, outerY;
		// Convert degrees to radians
		angleRad = angle * (std::numbers::pi / 180);
		// inner marker position
		innerX = cos(angleRad) * (radius / scaleFactor);
		innerY = sin(angleRad) * (radius / scaleFactor);
		// outer marker position
		outerX = cos(angleRad) * radius;
		outerY = sin(angleRad) * radius;
		// Draw marker line from inner to outer radius
		myTFT.drawLine(1 + centerX + innerX, 1 + centerY + innerY,
		               1 + centerX + outerX, 1 + centerY + outerY,
		               myTFT.C_WHITE);
	}
}

void drawPointer(const int16_t &currentValue, const int16_t &oldValue, uint8_t x, uint8_t y, uint8_t r, uint16_t colour, uint16_t bcolour) {
	uint16_t i;
	// If the current value is increasing
	if (currentValue > oldValue) {
		// Incrementally move the pointer from oldValue to currentValue
		for (i = oldValue; i <= currentValue; i++) {
			// Erase the previous pointer position
			drawPointerHelper(i - 1, x, y, r, bcolour);
			// Draw the pointer at the new position
			drawPointerHelper(i, x, y, r, colour);
			// Introduce a small delay for smooth animation
			if ((currentValue - oldValue) < 128) {
				delay(20);
			} else {
				delay(30);  // Slower update for small changes
			}
		}
	} else  // If the current value is decreasing
	{
		// Decrementally move the pointer from oldValue to currentValue
		for (i = oldValue; i >= currentValue; i--) {
			// Erase the previous pointer position
			drawPointerHelper(i + 1, x, y, r, bcolour);
			// Draw the pointer at the new position
			drawPointerHelper(i, x, y, r, colour);
			// Adjust delay based on the difference to control animation speed
			if ((oldValue - currentValue) >= 128) {
				delay(20);
			} else {
				delay(30);  // Slower update for small changes
			}
		}
	}
}

void drawPointerHelper(int16_t value, uint8_t centerX, uint8_t centerY, uint8_t radius, uint16_t pointerColor) {
	float angleRad, pointerX, pointerY;
	int16_t minValue = 0;
	int16_t maxValue = 255;
	int startAngle = 150;  // Gauge starting angle in degrees
	int endAngle = 240;    // Gauge ending angle in degrees
	// Ensure the value stays within the valid range
	if (value > maxValue) value = maxValue;
	if (value < minValue) value = minValue;
	// Map value to an angle in radians based on the gauge range
	angleRad = (((float)(uint16_t)(value - minValue) / (float)(uint16_t)(maxValue - minValue) * endAngle)
	            + startAngle)
	           * (std::numbers::pi / 180);
	// Calculate pointer tip coordinates
	pointerX = cos(angleRad) * (radius / 1.35);
	pointerY = sin(angleRad) * (radius / 1.35);
	// Draw the pointer line from the center to the calculated tip position
	myTFT.drawLine(centerX, centerY, 1 + centerX + pointerX, 1 + centerY + pointerY, pointerColor);
	// Draw a small circle to represent the pointer base
	myTFT.fillCircle(centerX, centerY, 2, pointerColor);
}
///@endcond
