//lcd_graphics.h

#ifndef _LCD_GRAPHICS_H_
#define _LCD_GRAPHICS_H_

//#include <stdint.h>

//#include "lcd_st7567.h"
#include "stm8s208_st7567.h"
#include "font_data.h"

#define LCDG_X_OFFSET    4
#define LCDG_WIDTH       ((uint8_t)128)
#define LCDG_HIGH        ((uint8_t)64)
#define LCDG_PAGE_START  (0)
#define LCDG_PAGE_MAX    (LCDG_HIGH/8)
#define LCDG_BUF_LEN     (LCDG_WIDTH*LCDG_PAGE_MAX)

typedef enum{
    IMAGE_NO_MIX = 0,
    IMAGE_MIX,
    IMAGE_ANTIWHITE    
}IMAGE_MIX_SET;


void LCDG_init(void);

//basic function
void LCDG_displayUpdate(void);
void LCDG_displayUpdate_part(uint8_t x, uint8_t page_st, uint8_t page_ed, uint8_t width);
void LCDG_clearAll(void);
void LCDG_clear(uint8_t x, uint8_t y, uint8_t width, uint8_t high);

// void LCDG_image(uint8_t x, uint8_t y, int16_t image_width, int16_t image_high, uint8_t* image);
// void LCDG_image_mix(uint8_t x, uint8_t y, int16_t image_width, int16_t image_high, uint8_t* image);
void LCDG_image(uint8_t x, uint8_t y, int16_t image_width, int16_t image_high, uint8_t* image, uint8_t mix_function);

//void LCDG_drawLine(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1);
void LCDG_drawRect(uint8_t x, uint8_t y, uint8_t rect_width, uint8_t rect_high);
void LCDG_full(void);   // all black
void LCDG_drawPixel(uint8_t x, uint8_t y, uint8_t dot);


void LCDG_Char8x16(uint8_t x, uint8_t y, uint8_t c, uint8_t mix);
void LCDG_string8x16(uint8_t x, uint8_t y, uint8_t *str, uint8_t mix);

void LCDG_Char12x24(uint8_t x, uint8_t y, uint8_t ch, uint8_t mix);
void LCDG_string12x24(uint8_t x, uint8_t y, uint8_t *str, uint8_t mix);

void LCDG_digit32x48(uint8_t x, uint8_t y, uint8_t value, uint8_t mix);


#endif  //_LCD_GRAPHICS_H_