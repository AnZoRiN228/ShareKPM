#include "ssd1306.h"

#define SSD1306_DATA_MODE 0x40
#define SSD1306_COMMAND_MODE 0x80

// Screenbuffer
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

// Screen object
static SSD1306_t SSD1306;

uint8_t ssd1306_Init(void){
	
	delay_ms(100);

// Init LCD

	ssd1306_SendCommand(0xAE);   // Display off
	ssd1306_SendCommand(0x20);   // Set Memory Addressing Mode
	ssd1306_SendCommand(0x10);   // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	ssd1306_SendCommand(0xB0);   // Set Page Start Address for Page Addressing Mode,0-7
	ssd1306_SendCommand(0xC8);   // Set COM Output Scan Direction
	ssd1306_SendCommand(0x00);   // Set low column address
	ssd1306_SendCommand(0x10);   // Set high column address
	ssd1306_SendCommand(0x40);   // Set start line address
	ssd1306_SendCommand(0x81);   // set contrast control register
	ssd1306_SendCommand(0xFF);
	ssd1306_SendCommand(0xA1);   // Set segment re-map 0 to 127
	ssd1306_SendCommand(0xA6);   // Set normal display

	ssd1306_SendCommand(0xA8);   // Set multiplex ratio(1 to 64)
	ssd1306_SendCommand(SSD1306_HEIGHT - 1);

	ssd1306_SendCommand(0xA4);   // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	ssd1306_SendCommand(0xD3);   // Set display offset
	ssd1306_SendCommand(0x00);   // No offset
	ssd1306_SendCommand(0xD5);   // Set display clock divide ratio/oscillator frequency
	ssd1306_SendCommand(0xF0);   // Set divide ratio
	ssd1306_SendCommand(0xD9);   // Set pre-charge period
	ssd1306_SendCommand(0x22);

	ssd1306_SendCommand(0xDA);   // Set com pins hardware configuration
#ifdef SSD1306_COM_LR_REMAP
	ssd1306_SendCommand(hi2c, 0x32);   // Enable COM left/right remap
#else
	ssd1306_SendCommand(0x12);   // Do not use COM left/right remap
#endif // SSD1306_COM_LR_REMAP

	ssd1306_SendCommand(0xDB);   // Set vcomh
	ssd1306_SendCommand(0x20);   // 0x20,0.77xVcc
	ssd1306_SendCommand(0x8D);   // Set DC-DC enable
	ssd1306_SendCommand(0x14);   //
	ssd1306_SendCommand(0xAF);   // Turn on SSD1306 panel	
	
// Clear screen
	ssd1306_Fill(Black);

	// Flush buffer to screen
	ssd1306_UpdateAllScreen();

	// Set default values for screen object
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;

	SSD1306.Initialized = 1;

	return 0;
}
void ssd1306_SendCommand(unsigned char command){
	ssd1306_send(SSD1306_COMMAND_MODE, command);
}
void 	ssd1306_send(uint8_t control_byte, uint8_t data){
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB)){}
	(void) I2C1->SR1;
	I2C1->DR = SSD1306_I2C_ADDR;
	while (!(I2C1->SR1 & I2C_SR1_ADDR)){}
	(void) I2C1->SR1;
	(void) I2C1->SR2;
	I2C1->DR=control_byte;
	while (!(I2C1->SR1 & I2C_SR1_TXE)){}
	I2C1->DR=data;
	while (!(I2C1->SR1 & I2C_SR1_BTF)){}
	I2C1->CR1 |= I2C_CR1_STOP;
	if (!(I2C1->SR2 & I2C_SR2_BUSY)){I2C_BusyError();}
}
void ssd1306_SendData(uint8_t *data, uint8_t count){ 
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB)){}
	(void) I2C1->SR1;
	I2C1->DR = SSD1306_I2C_ADDR;
	while (!(I2C1->SR1 & I2C_SR1_ADDR)){}
	(void) I2C1->SR1;
	(void) I2C1->SR2;
	I2C1->DR=SSD1306_DATA_MODE;
	while (!(I2C1->SR1 & I2C_SR1_TXE)){}
	for(uint8_t index=0;index<=count-1;index++)
	{
		I2C1->DR=*data++ ;
		while (!(I2C1->SR1 & I2C_SR1_TXE)){}
	}
	while (!(I2C1->SR1 & I2C_SR1_BTF)){}
	I2C1->CR1 |= I2C_CR1_STOP;
	if (!(I2C1->SR2 & I2C_SR2_BUSY)){I2C_BusyError();}
}
void ssd1306_Fill(SSD1306_COLOR color){
	// Fill screenbuffer with a constant value (color)
	uint32_t i;
	for(i = 0; i < sizeof(SSD1306_Buffer); i++)
	{
			SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
	}
}
void ssd1306_UpdateScreen(uint8_t updateFrom, uint8_t updateTo){
	uint8_t i;
	for (i = updateFrom-1; i <=updateTo-1; i++) 
	{
		ssd1306_SendCommand(0xB0 + i);
		ssd1306_SendCommand(0x00);
		ssd1306_SendCommand(0x10);
		ssd1306_SendData(&SSD1306_Buffer[SSD1306_WIDTH * i], SSD1306_WIDTH);
	}
}
void ssd1306_UpdateAllScreen(void){
	uint8_t i;
	for (i = 0; i < 8; i++) 
	{
		ssd1306_SendCommand(0xB0 + i);
		ssd1306_SendCommand(0x00);
		ssd1306_SendCommand(0x10);
		ssd1306_SendData(&SSD1306_Buffer[SSD1306_WIDTH * i], SSD1306_WIDTH);
	}
}
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color){
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
	{
		// Don't write outside the buffer
		return;
	}
	// Check if pixel should be inverted
	if (SSD1306.Inverted)
	{
		color = (SSD1306_COLOR)!color;
	}
	// Draw in the correct color
	if (color == White)
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	}
	else
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color){
	uint32_t b;

	// Check remaining space on current line
	if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight))
	{
		// Not enough space on current line
		return 0;
	}
	// Translate font to screenbuffer
	for (uint32_t i = 0; i < Font.FontHeight; i++)
	{
		b = Font.data[(ch - 32) * Font.FontHeight + i];
		for (uint32_t j = 0; j < Font.FontWidth; j++)
		{
			if ((b << j) & 0x8000)
			{
				ssd1306_DrawPixel(SSD1306.CurrentX + (uint8_t)j, (SSD1306.CurrentY + (uint8_t)i), (SSD1306_COLOR) color);
			}
			else
			{
				ssd1306_DrawPixel(SSD1306.CurrentX + (uint8_t)j, (SSD1306.CurrentY + (uint8_t)i), (SSD1306_COLOR)!color);
			}
		}
	}
	// The current space is now taken
	SSD1306.CurrentX += Font.FontWidth;
	// Return written char for validation
	return ch;
}
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color){
	// Write until null-byte
	while (*str)
	{
		if (ssd1306_WriteChar(*str, Font, color) != *str)
		{
			// Char could not be written
			return *str;
		}
		// Next char
		str++;
	}
	// Everything ok
	return *str;
}
void ssd1306_InvertColors(void){
	SSD1306.Inverted = !SSD1306.Inverted;
}
void ssd1306_SetCursor(uint8_t x, uint8_t y){
  SSD1306.CurrentX = x;
  SSD1306.CurrentY = y;
}
void ssd1306_ConvertVar(char *arr, unsigned int size, const char *arrStr, int avarage){
	snprintf(arr, size, arrStr, avarage);
}
