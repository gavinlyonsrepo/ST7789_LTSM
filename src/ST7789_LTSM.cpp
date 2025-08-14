/*!
	@file    ST7789_LTSM.cpp
	@author  Gavin Lyons
	@brief   Library source file for st7789 display for arduino, Arduino library ST7789_LTSM
	@details Depends on arduino library display16_graphics_LTSM  by Liontron systems
*/

#include "ST7789_LTSM.hpp"
 
/*!
	@brief Constructor for class ST7789_LTSM
*/
ST7789_LTSM :: ST7789_LTSM(){}

/*!
	@brief : Init Hardware SPI
*/
void ST7789_LTSM::TFTHWSPIInitialize(void){
	SPI.begin();
}

/*!
	@brief: Call when powering down TFT
	@note  Will switch off SPI 
*/
void ST7789_LTSM ::TFTPowerDown(void)
{
	TFTenableDisplay(false);
	if (_resetPinOn == true) {
		DISPLAY16_RST_SetLow;
	}
	DISPLAY16_DC_SetLow;
	DISPLAY16_CS_SetLow;
	if (_hardwareSPI == true) {
		SPI.end();
	}else{
		DISPLAY16_SCLK_SetLow;
		DISPLAY16_SDATA_SetLow;
	}
}

/*!
	@brief Method for Hardware Reset pin control
*/
void ST7789_LTSM ::TFTResetPIN() {
	if (_resetPinOn == false) return;
	const uint16_t resetDelay = 10; /**< Hardware reset delay in milliseconds  */
	DISPLAY16_RST_SetDigitalOutput;
	DISPLAY16_RST_SetHigh;
	MILLISEC_DELAY(resetDelay);
	DISPLAY16_RST_SetLow;
	MILLISEC_DELAY(resetDelay);
	DISPLAY16_RST_SetHigh;
	MILLISEC_DELAY(resetDelay);
}

/*!
	@brief  sets up TFT GPIO
	@param CommDelay SW SPI GPIO delay uS
	@param rst reset GPIO, optional pass -1 to disable, see note
	@param dc data or command GPIO.
	@param cs chip select GPIO 
	@param sclk Data clock GPIO  
	@param din Data to TFT GPIO 
	@note if -1 is passed for reset pin, software reset is used, if LCD has optional reset pin
		method overload used , method 2 software SPI
*/
void ST7789_LTSM ::setupGPIO_SPI(uint16_t CommDelay, int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din)
{
	_hardwareSPI = false;
	TFTSwSpiGpioDelaySet(CommDelay);
	_display_RST= rst;
	_display_DC = dc;
	_display_CS = cs;
	_display_SDATA = din;
	_display_SCLK = sclk;

	if (_display_RST == -1 ){
		_resetPinOn = false;
	}else{
		_resetPinOn = true;
	}
}

/*!
	@brief sets up TFT GPIO
	@param speed_hz SPI baudrate in hz
	@param rst reset GPIO
	@param dc data or command GPIO.
	@param cs chip select GPIO
	@note method overload used , method 1 hardware SPI
*/
void ST7789_LTSM::setupGPIO_SPI(uint32_t speed_hz, int8_t rst, int8_t dc, int8_t cs)
{
	_speedSPIHz = speed_hz;
	_hardwareSPI = true;
	_display_RST= rst;
	_display_DC = dc;
	_display_CS = cs;
}

/*!
	@brief init routine for ST7789 controller
*/
void ST7789_LTSM::TFTST7789Initialize() 
{
	if (_resetPinOn == true) TFTResetPIN();

	DISPLAY16_DC_SetDigitalOutput;
	DISPLAY16_DC_SetLow;
	DISPLAY16_CS_SetDigitalOutput;
	DISPLAY16_CS_SetHigh;

	if (_hardwareSPI == false)
	{
		DISPLAY16_SCLK_SetDigitalOutput;
		DISPLAY16_SDATA_SetDigitalOutput;
		DISPLAY16_SCLK_SetLow;
		DISPLAY16_SDATA_SetLow;
	}else{
		TFTHWSPIInitialize();
	}
		cmd89();
		AdjustWidthHeight();
		setRotation(Degrees_0);
}


/*!
	@brief Toggle the invert mode
	@param invert true invert off , false invert on
*/
void ST7789_LTSM ::TFTchangeInvertMode(bool invert) {
	if(invert) {
		writeCommand(ST7789_INVOFF);
	} else {
		writeCommand(ST7789_INVON);
	}
}

/*!
	@brief Toggle the partial display mode
	@param partialDisplay true  on false  off
*/
void ST7789_LTSM ::TFTpartialDisplay(bool partialDisplay){
	if(partialDisplay) {
		writeCommand(ST7789_PTLON);
	} else {
		writeCommand(ST7789_NORON);
	}
}

/*!
	@brief enable /disable display mode
	@param enableDisplay true enable on false disable
*/
void ST7789_LTSM ::TFTenableDisplay(bool enableDisplay){
	if(enableDisplay) {
		writeCommand(ST7789_DISPON);
	} else {
		writeCommand(ST7789_DISPOFF);
	}
}

/*!
	@brief Toggle the sleep mode
	@param sleepMode true sleep on false sleep off
*/
void ST7789_LTSM ::TFTsleepDisplay(bool sleepMode){
	if(sleepMode) {
		writeCommand(ST7789_SLPIN);
		MILLISEC_DELAY(5);
	} else {
		writeCommand(ST7789_SLPOUT);
		MILLISEC_DELAY(120);
	}
}

/*!
	@brief Toggle the idle display mode
	@param idleMode true enable on false disable
*/
void ST7789_LTSM ::TFTidleDisplay(bool idleMode){
	if( idleMode) {
		writeCommand(ST7789_IDLE_ON);
	} else {
		writeCommand(ST7789_IDLE_OFF);
	}
}

/*!
	@brief return Display to normal mode
	@note used after scroll set for example
*/
void ST7789_LTSM::TFTNormalMode(void){writeCommand(ST7789_NORON);}

/*!
	@brief: change rotation of display.
	@param mode display_rotate_e enum
	0 = Normal
	1=  90 rotate
	2 = 180 rotate
	3 =  270 rotate
*/
void ST7789_LTSM ::setRotation(display_rotate_e mode) {
	uint8_t madctl = 0;
	switch (mode) {
		case Degrees_0 :
			madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB;
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			_XStart = _colstart;
			_YStart = _rowstart;
			break;
		case Degrees_90:
			madctl = ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
			_YStart = _colstart;
			_XStart = _rowstart;
			_width  =_heightStartTFT;
			_height = _widthStartTFT;
			break;
		case Degrees_180:
			madctl = ST7789_MADCTL_RGB;
			_XStart = _colstart;
			_YStart = _rowstart;
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			break;
		case Degrees_270:
			madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
			_YStart = _colstart;
			_XStart = _rowstart;
			_width =_heightStartTFT;
			_height = _widthStartTFT;
			break;
	}
	writeCommand(ST7789_MADCTL);
	writeData(madctl);
}

/*!
	@brief initialise the variables that define the size of the screen
	@param colOffset Column offset
	@param rowOffset row offset
	@param width_TFT width in pixels
	@param height_TFT height in pixels
	@note  The offsets can be adjusted for any issues with manufacture tolerance/defects
*/
void ST7789_LTSM  :: TFTInitScreenSize(uint16_t colOffset, uint16_t rowOffset, uint16_t width_TFT, uint16_t height_TFT)
{
	_colstart = colOffset; 
	_rowstart = rowOffset;
	_XStart = colOffset; 
	_YStart = rowOffset;
	
	_width = width_TFT;
	_height = height_TFT;
	_widthStartTFT = width_TFT;
	_heightStartTFT = height_TFT;
}



/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t ST7789_LTSM::TFTSwSpiGpioDelayGet(void){return _SWSPIGPIODelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  ST7789_LTSM::TFTSwSpiGpioDelaySet(uint16_t CommDelay){_SWSPIGPIODelay = CommDelay;}


/*!
	@brief Command Initialization sequence for ST7789 display
*/
void ST7789_LTSM::cmd89(void)
{
	uint8_t CASETsequence[] {0x00, 0x00, 0,  240};
	uint8_t RASETsequence[] {0x00, 0x00, 320 >> 8 , 320 & 0xFF};
	
	writeCommand(ST7789_SWRESET);
	MILLISEC_DELAY (150);
	writeCommand(ST7789_SLPOUT);
	MILLISEC_DELAY (500);
	writeCommand(ST7789_COLMOD); //Set color mode
	writeData(0x55); // 16 bit color
	MILLISEC_DELAY (10);

	writeCommand(ST7789_MADCTL); // Mem access ctrl (directions)
	writeData(0x08); // Row/col address, top-bottom refresh

	writeCommand(ST7789_CASET);  //Column address set
	spiWriteDataBuffer(CASETsequence, sizeof(CASETsequence));
	writeCommand(ST7789_RASET);  //Row address set
	spiWriteDataBuffer(RASETsequence, sizeof(RASETsequence));

	writeCommand(ST7789_INVON);
	MILLISEC_DELAY (10);
	writeCommand(ST7789_NORON);
	MILLISEC_DELAY (10);
	writeCommand(ST7789_DISPON);
	MILLISEC_DELAY (10);
}

/*!
	@brief Initialization  width and height code common to all ST7789 displays
	@note ST7789 display require an offset calculation which differs for different size displays
*/
void ST7789_LTSM::AdjustWidthHeight() {
	if (_width == 240 && _height == 240) {
	// 1.3", 1.54" displays (right justified)
		_rowstart = (320 - _height);
		_rowstart2 = 0;
		_colstart = _colstart2 = (240 - _width);
	} else if (_width == 135 && _height == 240) {
	// 1.14" display (centered, with odd size)
		_rowstart = _rowstart2 = (int)((320 - _height) / 2);
		_colstart = (int)((240 - _width + 1) / 2);
		_colstart2 = (int)((240 - _width) / 2);
	} else {
	// 1.47", 1.69, 1.9", 2.0" displays (centered)
		_rowstart = _rowstart2 = (int)((320 - _height) / 2);
		_colstart = _colstart2 = (int)((240 - _width) / 2);
	}
}

/*!
  @brief SPI displays set an address window rectangle for blitting pixels
  @param  x0 Top left corner x coordinate
  @param  y0  Top left corner y coordinate
  @param  x1  Bottom-right x coordinate
  @param  y1  Bottom-right y coordinate
  @note https://en.wikipedia.org/wiki/Bit_blit
 */
void ST7789_LTSM::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint16_t x0_ = x0 + _XStart;
	uint16_t x1_ = x1 + _XStart;
	uint16_t y0_ = y0 + _YStart;
	uint16_t y1_ = y1 + _YStart;
	uint8_t seqCASET[] = {
		(uint8_t)(x0_ >> 8), (uint8_t)(x0_ & 0xFF),
		(uint8_t)(x1_ >> 8), (uint8_t)(x1_ & 0xFF)
	};
	uint8_t seqRASET[] = {
		(uint8_t)(y0_ >> 8), (uint8_t)(y0_ & 0xFF),
		(uint8_t)(y1_ >> 8), (uint8_t)(y1_ & 0xFF)
	};

	writeCommand(ST7789_CASET); // Column address set
	spiWriteDataBuffer(seqCASET, sizeof(seqCASET));
	writeCommand(ST7789_RASET); // Row address set
	spiWriteDataBuffer(seqRASET, sizeof(seqRASET));
	writeCommand(ST7789_RAMWR); // Write to RAM
}

/*!
	@brief This method defines the Vertical Scrolling Area of the display where:
	@param top_fix_heightTFT describes the Top Fixed Area,
	@param bottom_fix_heightTFT describes the Bottom Fixed Area and
	@param _scroll_direction is scroll direction (0 for top to bottom and 1 for bottom to top).
*/
void ST7789_LTSM::setScrollDefinition(uint16_t top_fix_heightTFT, uint16_t bottom_fix_heightTFT, bool _scroll_direction) {
	uint16_t scroll_heightTFT;
	scroll_heightTFT = 320- top_fix_heightTFT - bottom_fix_heightTFT; // ST7789 320x240 VRAM
	writeCommand(ST7789_VSCRDEF);
	writeData(top_fix_heightTFT >> 8);
	writeData(top_fix_heightTFT & 0xFF);
	writeData(scroll_heightTFT >> 8);
	writeData(scroll_heightTFT & 0xFF);
	writeData(bottom_fix_heightTFT >> 8);
	writeData(bottom_fix_heightTFT & 0xFF);
	//writeCommand(ST7789_MADCTL);

	if (_scroll_direction) {
		writeData(ST7789_SRLBTT); //bottom to top
	} else {
		writeData(ST7789_SRLTTB); //top to bottom
	}
}

/*!
	@brief This method is used together with the setScrollDefinition.
	@param vsp scrolling mode
*/
void ST7789_LTSM ::TFTVerticalScroll(uint16_t vsp) {
	writeCommand(ST7789_VSCRSADD);
	writeData(vsp >> 8);
	writeData(vsp & 0xFF);
}

/*!
	@brief Software reset command
*/
void ST7789_LTSM::TFTresetSWDisplay(void) 
{
  writeCommand(ST7789_SWRESET);
  MILLISEC_DELAY(5);
}
//**************** EOF *****************
