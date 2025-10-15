/*
		File: WeatherDisplay.c
		Group 17
		Andrew Nguyen, Anton Tran, Tommy Troung, Abass Mir
		Functionallity: pplication for the TM4C123 that initializes the 
		ST7735 LCD and Port F, then lets the user toggle sunny/cloudy/rainy 
		screens with simple animations via SW1.
*/ 

// WeatherDisplay.c
// Runs on TM4C123
// This project displays three different weather screens on an ST7735 LCD.
// The user can toggle between sunny, cloudy, and rainy screens
// using the onboard switch SW1 (PF4). Each screen has a unique
// background, text layout, and animation.
//
// Hardware Connections:
// ST7735 LCD: See ST7735.c for detailed pinout (uses SSI0 on PA2, PA3, PA5 and GPIO on PA6, PA7).
// Onboard Switch SW1: Connected to PF4.
// Onboard LEDs (optional for debugging): PF1, PF2, PF3.

#include <stdint.h>
#include <stdlib.h> // For rand()
#include "ST7735.h"
#include "bitmaps.h"
#include "tm4c123gh6pm.h"

// === Added: centered + scaled bitmap drawing ===
#ifndef ST7735_TFTWIDTH
#define ST7735_TFTWIDTH 128
#endif
#ifndef ST7735_TFTHEIGHT
#define ST7735_TFTHEIGHT 160
#endif
#ifndef ICON_SCALE
#define ICON_SCALE 2  // 1 = 20x20 (original), 2 = 40x40
#endif
#ifndef ICON_CENTER_X
#define ICON_CENTER_X (ST7735_TFTWIDTH/2)
#endif
#ifndef ICON_CENTER_Y
#define ICON_CENTER_Y 60  // slightly above middle to avoid bottom text
#endif

static void DrawBitmapScaledCentered(const uint16_t *img, int w, int h,
                                     uint16_t bg, int scale){
  int scaledW = w * scale;
  int scaledH = h * scale;
  int left = ICON_CENTER_X - scaledW/2;
  int top  = ICON_CENTER_Y - scaledH/2;
  if(left < 0) left = 0;
  if(top < 0) top = 0;
  if(left + scaledW > ST7735_TFTWIDTH) left = ST7735_TFTWIDTH - scaledW;
  if(top  + scaledH > ST7735_TFTHEIGHT) top  = ST7735_TFTHEIGHT - scaledH;

  // Clear previous area (guard band avoids outline artifacts)
  // Clearing skipped to reduce flicker
  for(int y = 0; y < h; y++){
    int srcY = (h - 1 - y); // flip vertically: assets are bottom-up
    for(int x = 0; x < w; x++){
      uint16_t c = img[srcY*w + x];
      ST7735_FillRect(left + x*scale, top + y*scale, scale, scale, c);
    }
  }
}
// === End added section ===

// --- Animation timing ---
static uint32_t animationFrame = 0;  // global frame counter for animations
#define ANIMATION_SPEED 10U          // higher = slower blink

// ---- Adapter for migrating from palette icons to raw bitmaps (bitmaps.h) ----
// New icons are 20x20 pixels each. We'll map the old ICON_*_P structs to these arrays.
#ifndef ST7735_TFTWIDTH
#define ST7735_TFTWIDTH 128
#endif

typedef struct {
    const unsigned short *idx;   // pointer to 16-bit bitmap data (bitmaps.h)
    int w;
    int h;
    const unsigned short *pal;   // placeholder to keep old signature; unused
} ICON_P_STUB;

// Unused dummy palette
static const unsigned short __dummy_pal[1] = {0};

// Map old names to new arrays (20x20)
static const ICON_P_STUB ICON_SUNNY_P  = { sunny_day, 20, 20, __dummy_pal };
static const ICON_P_STUB ICON_CLOUDY_P = { cloud_day, 20, 20, __dummy_pal };
static const ICON_P_STUB ICON_RAINY_P  = { rainy,     20, 20, __dummy_pal };


// Shims to keep existing draw calls working
static inline void DrawBitmap4BPP_Centered(const unsigned short *idx, int w, int h, const unsigned short *pal, int dx, int y) {
    (void)pal; // unused
    int x = (ST7735_TFTWIDTH - w)/2 + dx;
    ST7735_DrawBitmap(x, y, idx, w, h);
}
static inline void DrawBitmap4BPP(const unsigned short *idx, int w, int h, const unsigned short *pal, int x, int y) {
    (void)pal; // unused
    ST7735_DrawBitmap(x, y, idx, w, h);
}
// ---- End adapter ----

// Function Prototypes
void PortF_Init(void);
void DelayWait10ms(uint32_t n);
void drawSunnyScreen(void);
void drawCloudyScreen(void);
void drawRainyScreen(void);
void animateSun(void);
void animateClouds(void);
void animateRain(void);
#define NUM_DROPS 50

// Define a state machine for the weather screens
typedef enum {
    SUNNY,
    CLOUDY,
    RAINY
} WeatherState;

// Global variables for animations
// Sun animation
int g_sunRayAngle = 0;

// Cloud animation
typedef struct {
    int16_t x, y, w, h, speed;
} Cloud;
Cloud g_clouds[3];

// Rain animation
typedef struct {
    int16_t x, y, speed, len;
} RainDrop;
RainDrop g_rainDrops[50];


int main(void) {
    // Initialization
    ST7735_InitR(INITR_REDTAB);
    PortF_Init();

    WeatherState currentState = SUNNY;
    uint8_t needsRedraw = 1; // Flag to redraw the static screen elements

    // Initialize cloud positions for animation
    g_clouds[0] = (Cloud){10, 50, 40, 20, 1};
    g_clouds[1] = (Cloud){60, 40, 50, 25, -1};
    g_clouds[2] = (Cloud){-30, 60, 45, 20, 1};

    // Initialize rain drop positions for animation
    for (int i = 0; i < 50; i++) {
        g_rainDrops[i].x = rand() % ST7735_TFTWIDTH;
        g_rainDrops[i].y = -(rand() % ST7735_TFTHEIGHT); // Start off-screen
        g_rainDrops[i].speed = (rand() % 3) + 2;
        g_rainDrops[i].len = (rand() % 4) + 2;
    }


    while (1) {
        // --- Input Handling: Check for SW1 press ---
        if ((GPIO_PORTF_DATA_R & 0x10) == 0) { // SW1 is pressed
            DelayWait10ms(2); // Debounce delay
            if ((GPIO_PORTF_DATA_R & 0x10) == 0) {
                // Cycle to the next state
                if (currentState == SUNNY) {
                    currentState = CLOUDY;
                } else if (currentState == CLOUDY) {
                    currentState = RAINY;
                } else {
                    currentState = SUNNY;
                }
                needsRedraw = 1; // Set flag to redraw the screen
                while ((GPIO_PORTF_DATA_R & 0x10) == 0) {}; // Wait for switch release
            }
        }

        // --- Display Logic ---
        switch (currentState) {
            case SUNNY:
                if (needsRedraw) {
                    drawSunnyScreen();
                    needsRedraw = 0;
                }
                animateSun();
                break;
            case CLOUDY:
                if (needsRedraw) {
                    drawCloudyScreen();
                    needsRedraw = 0;
                }
                animateClouds();
                break;
            case RAINY:
                if (needsRedraw) {
                    drawRainyScreen();
                    needsRedraw = 0;
                }
                animateRain();
                break;
        }
        
        DelayWait10ms(1); // Control animation speed
    }
}

// Helper function to draw a string with a specific size and background color
void DrawStringSizedColor(int16_t x, int16_t y, char *pt, int16_t textColor, int16_t bgColor, uint8_t size) {
    while(*pt){
        // Prevent writing past the right edge of the screen
        if (x > ST7735_TFTWIDTH - 6 * size) break;
        ST7735_DrawCharS(x, y, *pt, textColor, bgColor, size);
        pt++;
        x += 6 * size; // 5 pixels wide font + 1 pixel space
    }
}

// --- Screen Drawing Functions ---

// Draws the static text and background for the Sunny screen
void drawSunnyScreen(void) {
uint16_t bgColor = ST7735_CYAN;
    ST7735_FillScreen(bgColor);

    DrawStringSizedColor(4, 10, "Carson, CA", ST7735_YELLOW, bgColor, 2);
    DrawStringSizedColor(5, 100, "Avg:85 Max:92 Min:78", ST7735_GREEN, bgColor, 1);
    DrawStringSizedColor(35,110, "Humidity: 60%",        ST7735_GREEN, bgColor, 1);
    DrawStringSizedColor(19,130, "CLEAR",                ST7735_WHITE, bgColor, 3);

   
}

// Draws the static text and background for the Cloudy screen
void drawCloudyScreen(void) {
uint16_t bgColor = ST7735_LIGHTGREY;
    ST7735_FillScreen(bgColor);

    DrawStringSizedColor(4, 10, "Dallas, TX", ST7735_DARKGREY, bgColor, 2);
    DrawStringSizedColor(5, 100, "Avg:75 Max:81 Min:70", ST7735_BLUE, bgColor, 1);
    DrawStringSizedColor(35,110, "Humidity: 75%",        ST7735_BLUE, bgColor, 1);
    DrawStringSizedColor(10,130, "CLOUDY",               ST7735_WHITE, bgColor, 3);

   
}

// Draws the static text and background for the Rainy screen
void drawRainyScreen(void) {
uint16_t bgColor = ST7735_DARKBLUE;
    ST7735_FillScreen(bgColor); 

    DrawStringSizedColor(4, 10,  "AUSTIN, TX",            ST7735_LIGHTGREY, bgColor, 2);
    DrawStringSizedColor(5, 100, "Avg:68 Max:72 Min:65",  ST7735_YELLOW,    bgColor, 1);
    DrawStringSizedColor(35,110, "Humidity: 88%",         ST7735_YELLOW,    bgColor, 1);
    DrawStringSizedColor(19,130, "RAINY",                 ST7735_CYAN,      bgColor, 3);

   
}

// --- Animation Functions ---
void UpdateSunnyAnimation(void);
void UpdateRainyAnimation(void);
void UpdateCloudyAnimation(void);
void animateSun(void){ UpdateSunnyAnimation(); }

void animateClouds(void){ UpdateCloudyAnimation(); }

// Animates a rotating sun for the sunny screen
void UpdateSunnyAnimation(void) {
  static uint32_t prevBlink = 0xFFFFFFFF; // sentinel
  animationFrame++;
  uint32_t showBlink = (animationFrame / ANIMATION_SPEED) % 2;
  if(showBlink != prevBlink) {
    const uint16_t *icon = showBlink ? sunny_day_blink : sunny_day;
    DrawBitmapScaledCentered(icon, 20, 20, ST7735_CYAN, ICON_SCALE);
    prevBlink = showBlink;
  }
  DelayWait10ms(1);
}

// Helper function to draw a cloud
void UpdateCloudyAnimation(void) {
  static uint32_t prevBlink = 0xFFFFFFFF; // sentinel
  animationFrame++;
  uint32_t showBlink = (animationFrame / ANIMATION_SPEED) % 2;
  if(showBlink != prevBlink) {
    const uint16_t *icon = showBlink ? cloud_day_blink : cloud_day;
    DrawBitmapScaledCentered(icon, 20, 20, ST7735_LIGHTGREY, ICON_SCALE);
    prevBlink = showBlink;
  }
  DelayWait10ms(1);
}

// Animates drifting clouds
void UpdateRainyAnimation(void) {
  static uint32_t prevBlink = 0xFFFFFFFF; // sentinel
  animationFrame++;
  uint32_t showBlink = (animationFrame / ANIMATION_SPEED) % 2;
  if(showBlink != prevBlink) {
    const uint16_t *icon = showBlink ? rainy_blink : rainy;
    DrawBitmapScaledCentered(icon, 20, 20, ST7735_DARKBLUE, ICON_SCALE);
    prevBlink = showBlink;
  }
  DelayWait10ms(1);
}

// Draws only the text overlay for the Rainy screen (no screen clear)
static void drawRainyOverlayText(void) {
    uint16_t bgColor = ST7735_DARKBLUE;
    DrawStringSizedColor(4, 10,  "AUSTIN, TX",            ST7735_LIGHTGREY, bgColor, 2);
    DrawStringSizedColor(5, 100, "Avg:68 Max:72 Min:65",  ST7735_YELLOW,    bgColor, 1);
    DrawStringSizedColor(35,110, "Humidity: 88%",         ST7735_YELLOW,    bgColor, 1);
    DrawStringSizedColor(19,130, "RAINY",                 ST7735_CYAN,      bgColor, 3);
}
// Animates falling rain
void animateRain(void) {
  static uint32_t prevBlink = 0xFFFFFFFF; // sentinel
  animationFrame++;
  uint32_t showBlink = (animationFrame / ANIMATION_SPEED) % 2;
  if(showBlink != prevBlink) {
    const uint16_t *icon = showBlink ? rainy_blink : rainy;
    DrawBitmapScaledCentered(icon, 20, 20, ST7735_DARKBLUE, ICON_SCALE);
    prevBlink = showBlink;
  }
  DelayWait10ms(1);
}



// --- Hardware Initialization and Utilities ---

// Initializes Port F for SW1 input
void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20; // 1) activate clock for Port F
    while ((SYSCTL_PRGPIO_R & 0x20) == 0) {}; // allow time for clock to start
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // 2) unlock GPIO Port F
    GPIO_PORTF_CR_R = 0x1F;     // allow changes to PF4-0
    GPIO_PORTF_AMSEL_R = 0x00;  // 3) disable analog on PF
    GPIO_PORTF_PCTL_R = 0x00000000; // 4) PCTL GPIO on PF4-0
    GPIO_PORTF_DIR_R = 0x0E;    // 5) PF4,PF0 in, PF3-1 out
    GPIO_PORTF_AFSEL_R = 0x00;  // 6) disable alt funct on PF7-0
    GPIO_PORTF_PUR_R = 0x11;    // enable pull-up on PF0 and PF4
    GPIO_PORTF_DEN_R = 0x1F;    // 7) enable digital I/O on PF4-0
}

// Simple delay function
void DelayWait10ms(uint32_t n) {
    uint32_t volatile time;
    while (n) {
        time = 727240 / 91; // 10msec
        while (time) {
            time--;
        }
        n--;
    }
}
