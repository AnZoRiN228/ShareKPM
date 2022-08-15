#ifndef __SSD1306
#define __SSD1306

#include "stm32f10x.h"                  // Device header
#include "initDevice.h"
#include "fonts.h"

// I2c address
#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR        0x78
#endif // SSD1306_I2C_ADDR

// SSD1306 width in pixels
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH           128
#endif // SSD1306_WIDTH

// SSD1306 LCD height in pixels
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT          64
#endif // SSD1306_HEIGHT

//
//  Enumeration for screen colors
//
typedef enum {
    Black = 0x00,   // Black color, no pixel
    White = 0x01,   // Pixel is set. Color depends on LCD
} SSD1306_COLOR;
//
//  Struct to store transformations
//
typedef struct {
	uint8_t CurrentX;
	uint8_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;
//
//  Function definitions
//
uint8_t ssd1306_Init(void);
void ssd1306_SendCommand(unsigned char command);
void ssd1306_send(uint8_t control_byte, uint8_t data);
void ssd1306_SendData(uint8_t *data, uint8_t count);
void ssd1306_UpdateScreen(uint8_t updateFrom, uint8_t updateTo);
void ssd1306_UpdateAllScreen(void);
void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color);
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void ssd1306_InvertColors(void);
void ssd1306_ConvertVar(char *arr, unsigned int size, const char *arrStr, int avarage);

#endif /*__SSD1306 */
