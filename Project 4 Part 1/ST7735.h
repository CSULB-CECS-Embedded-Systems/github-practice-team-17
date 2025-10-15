/*
		File: ST7735.h
		Group 17
		Andrew Nguyen, Anton Tran, Tommy Troung, Abass Mir
		Functionallity: Header with color constants, dimensions, 
		init flags, and function prototypes for the ST7735 graphics/printing API.
*/ 


// ST7735.h
// Runs on LM4F120/TM4C123
// Prototypes for the low level drivers for the ST7735 160x128 LCD.
// Daniel Valvano
// March 30, 2015

#ifndef __ST7735_H__
#define __ST7735_H__
#include <stdint.h>

// ST7735 color definitions
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF
#define ST7735_LIGHTGREY 0xC618
#define ST7735_DARKGREY 0x7BEF
#define ST7735_DARKBLUE  0x0010


// screen dimensions
#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160

enum initRFlags {
  INITR_GREENTAB = 0x0,
  INITR_REDTAB   = 0x1,
  INITR_BLACKTAB = 0x2
};

// Initialization for ST7735B screens
void ST7735_InitB(void);

// Initialization for ST7735R screens (green or red tabs)
void ST7735_InitR(enum initRFlags option);

// Draw a pixel at the given coordinates with the given color
void ST7735_DrawPixel(int16_t x, int16_t y, uint16_t color);

// Draw a vertical line at the given coordinates with the given height and color
void ST7735_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

// Draw a horizontal line at the given coordinates with the given width and color
void ST7735_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

// Draw a line with the given coordinates and color
void ST7735_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

// Fill the screen with the given color
void ST7735_FillScreen(uint16_t color);

// Draw a filled rectangle at the given coordinates with the given width, height, and color
void ST7735_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

// Draw a circle with the given radius and color
void ST7735_DrawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint16_t color);

// Fill a circle with the given radius and color
void ST7735_FillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint16_t color);


// Pass 8-bit (each) R,G,B and get back 16-bit packed color
uint16_t ST7735_Color565(uint8_t r, uint8_t g, uint8_t b);

// Swaps the red and blue values of the given 16-bit packed color
uint16_t ST7735_SwapColor(uint16_t x);

// Displays a 16-bit color BMP image
void ST7735_DrawBitmap(int16_t x, int16_t y, const uint16_t *image, int16_t w, int16_t h);

// Simple character draw function
void ST7735_DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size);

// Advanced character draw function
void ST7735_DrawChar(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size);

// String draw function
uint32_t ST7735_DrawString(uint16_t x, uint16_t y, char *pt, int16_t textColor);

// Move the cursor to the desired X- and Y-position
void ST7735_SetCursor(uint32_t newX, uint32_t newY);

// Output a 32-bit number in unsigned decimal format
void ST7735_OutUDec(uint32_t n);

// Change the image rotation
void ST7735_SetRotation(uint8_t m);

// Send the command to invert all of the colors
void ST7735_InvertDisplay(int i);

// Standard device driver initialization function for printf
void Output_Init(void);

#endif // __ST7735_H__