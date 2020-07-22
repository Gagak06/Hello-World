//LCDG_graphics.c

#include "LCD_graphics.h"


@near uint8_t display_buffer[LCDG_BUF_LEN];


void LCDG_displayUpdate(void)
{
	LCD_updateFull_it(display_buffer);
    //LCDG_displayUpdate_part(0, LCDG_PAGE_START, LCDG_PAGE_MAX, LCDG_WIDTH);    
    //LCDG_displayUpdate_part(0, LCDG_PAGE_START, LCDG_PAGE_MAX, LCDG_WIDTH);    
}

void LCDG_displayUpdate_part(uint8_t x, uint8_t page_st, uint8_t page_ed, uint8_t width)
{
    uint8_t *buf_addr;
    buf_addr = display_buffer;
    buf_addr += x; 
    buf_addr += LCDG_WIDTH* page_st; 
    while(page_st <= page_ed)
    {
        LCD_setPageData((LCDG_X_OFFSET + x), page_st, buf_addr, width);
        page_st++;
        buf_addr += LCDG_WIDTH;
    }
		
		
}

void LCDG_init(void)
{
	lcd_st7567_init();
	LCDG_clearAll();
	LCDG_displayUpdate();
}

void LCDG_clearAll(void)
{
    uint16_t index = LCDG_BUF_LEN;
    while(index--)
    {
        display_buffer[index] = 0x00;
    }
}

void LCDG_full(void)
{
    uint16_t index = LCDG_BUF_LEN;
    while(index--)
    {
        display_buffer[index] = 0x31;
    }
}

void LCDG_clear(uint8_t x, uint8_t y, uint8_t width, uint8_t high)
{
    uint8_t h,w;    
    uint8_t page;
    uint8_t bit_shift;
    
    h = 0;
    bit_shift = (y % 8);
    page = (y / 8);
    
    while(high > 0)
    {
        for(w = 0; w < width; w++)
        {
            display_buffer[LCDG_WIDTH*page + x + w] &= (0xFF >> (8 - bit_shift));
            //display_buffer[LCDG_WIDTH*page + x + w] |= (image[w + h*width] << bit_shift);
            
            if(bit_shift)
            {
                display_buffer[LCDG_WIDTH*(page+1) + x + w] &= (0xFF << bit_shift);
                //display_buffer[LCDG_WIDTH*(page+1) + x + w] |= (image[w + h*width] >> (8 - bit_shift));
            }
        }
        h++;
        page++;
        high -= 8;
    }
}

/*
void LCDG_image_mix(uint8_t x, uint8_t y, int16_t image_width, int16_t image_high, uint8_t* image)
{
    uint8_t h,w;    
    uint8_t page;
    uint8_t bit_shift;
    
    h = 0;
    bit_shift = (y % 8);
    page = (y / 8);
    
    while(image_high > 0)
    {
        for(w = 0; w < image_width; w++)
        {
            display_buffer[LCDG_WIDTH*page + x + w] |= (image[w + h*image_width] << bit_shift);
            
            if(bit_shift)
            {
                display_buffer[LCDG_WIDTH*(page+1) + x + w] |= (image[w + h*image_width] >> (8 - bit_shift));
            }
        }
        h++;
        page++;
        image_high -= 8;
    }
}

void LCDG_image(uint8_t x, uint8_t y, int16_t image_width, int16_t image_high, uint8_t* image)
{
    uint8_t h,w;    
    uint8_t page;
    uint8_t bit_shift;
    
    h = 0;
    bit_shift = (y % 8);
    page = (y / 8);
    
    while(image_high > 0)
    {
        for(w = 0; w < image_width; w++)
        {
            display_buffer[LCDG_WIDTH*page + x + w] &= (0xFF >> (8 - bit_shift));
            display_buffer[LCDG_WIDTH*page + x + w] |= (image[w + h*image_width] << bit_shift);
            
            if(bit_shift)
            {
                display_buffer[LCDG_WIDTH*(page+1) + x + w] &= (0xFF << bit_shift);
                display_buffer[LCDG_WIDTH*(page+1) + x + w] |= (image[w + h*image_width] >> (8 - bit_shift));
            }
        }
        h++;
        page++;
        image_high -= 8;
    }
}

void LCDG_image_antiWhite(uint8_t x, uint8_t y, int16_t image_width, int16_t image_high, uint8_t* image)
{
    uint8_t h,w;    
    uint8_t page;
    uint8_t bit_shift;
    
    h = 0;
    bit_shift = (y % 8);
    page = (y / 8);
    
    while(image_high > 0)
    {
        for(w = 0; w < image_width; w++)
        {
            display_buffer[LCDG_WIDTH*page + x + w] &= (0xFF >> (8 - bit_shift));
            display_buffer[LCDG_WIDTH*page + x + w] |= ((~image[w + h*image_width]) << bit_shift);
            
            if(bit_shift)
            {
                display_buffer[LCDG_WIDTH*(page+1) + x + w] &= (0xFF << bit_shift);
                display_buffer[LCDG_WIDTH*(page+1) + x + w] |= ((~image[w + h*image_width]) >> (8 - bit_shift));
            }
        }
        h++;
        page++;
        image_high -= 8;
    }
}
*/

void LCDG_image(uint8_t x, uint8_t y, int16_t image_width, int16_t image_high, uint8_t* image, uint8_t mix_function)
{
    uint8_t h,w;    
    uint8_t page;
    uint8_t bit_shift;
    
    h = 0;
    bit_shift = (y % 8);
    page = (y / 8);
    
    while(image_high > 0)
    {
        for(w = 0; w < image_width; w++)
        {
            switch(mix_function)
            {
                case IMAGE_NO_MIX:
                    display_buffer[LCDG_WIDTH*page + x + w] &= (0xFF >> (8 - bit_shift));
                    display_buffer[LCDG_WIDTH*page + x + w] |= (image[w + h*image_width] << bit_shift);
                    
                    if(bit_shift)
                    {
                        display_buffer[LCDG_WIDTH*(page+1) + x + w] &= (0xFF << bit_shift);
                        display_buffer[LCDG_WIDTH*(page+1) + x + w] |= (image[w + h*image_width] >> (8 - bit_shift));
                    }
                    break;
                    
                case IMAGE_MIX:
                    display_buffer[LCDG_WIDTH*page + x + w] |= (image[w + h*image_width] << bit_shift);
                    
                    if(bit_shift)
                    {
                        display_buffer[LCDG_WIDTH*(page+1) + x + w] |= (image[w + h*image_width] >> (8 - bit_shift));
                    }
                    break;
                    
                case IMAGE_ANTIWHITE:
                    display_buffer[LCDG_WIDTH*page + x + w] &= (0xFF >> (8 - bit_shift));
                    display_buffer[LCDG_WIDTH*page + x + w] |= ((~image[w + h*image_width]) << bit_shift);
                    
                    if(bit_shift)
                    {
                        display_buffer[LCDG_WIDTH*(page+1) + x + w] &= (0xFF << bit_shift);
                        display_buffer[LCDG_WIDTH*(page+1) + x + w] |= ((~image[w + h*image_width]) >> (8 - bit_shift));
                    }
                    break;
            }            
        }
        h++;
        page++;
        image_high -= 8;
    }
}

void LCDG_drawLine(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1)
{
    
}

void LCDG_drawCircle(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1)
{
    
}

void LCDG_drawPixel(uint8_t x, uint8_t y, uint8_t dot)
{
    uint8_t page;
    uint8_t pixel;
    
    pixel = (0x01 << (y % 8));
    page = (y / 8);
    if(dot)
    {
        display_buffer[LCDG_WIDTH*page + x] |= pixel;
    }
    else
    {
        display_buffer[LCDG_WIDTH*page + x] &= (~pixel);
    }
    
}

void LCDG_drawRect(uint8_t x, uint8_t y, uint8_t rect_width, uint8_t rect_high)
{
    uint8_t h,w;    
    uint8_t page;
    uint8_t bit_shift;
    
    
    bit_shift = (y % 8);
    page = (y / 8);
    rect_high -= bit_shift;
    
    //head    
    for(w = 0; w < rect_width; w++)
    {
        if((w == 0) || (w == (rect_width - 1)))
        {
            display_buffer[LCDG_WIDTH*page + x + w] |= (0xFF << bit_shift);
        }
        else
        {
            display_buffer[LCDG_WIDTH*page + x + w] |= (0x01 << bit_shift);
        }        
    }
    
    
    // body
    while(rect_high > 8)
    {
        rect_high -= 8;
        page++;        
        display_buffer[LCDG_WIDTH*page + x] = 0xFF;
        display_buffer[LCDG_WIDTH*page + x + rect_width - 1] = 0xFF;
    }
    
    
    bit_shift = (8 - rect_high);
    page++;
    
    
    //bot
    for(w = 0; w < rect_width; w++)
    {
        if((w == 0) || (w == (rect_width - 1)))
        {
            display_buffer[LCDG_WIDTH*page + x + w] |= (0xFF >> bit_shift);    
        }
        else
        {
            display_buffer[LCDG_WIDTH*page + x + w] |= (0x80 >> bit_shift);    
        }         
    }
}

void LCDG_string8x16(uint8_t x, uint8_t y, uint8_t *str, uint8_t mix)
{
    while(*str)
    {
        LCDG_Char8x16(x, y, *str, mix);
        x += 8;
        str++;
    }
}

void LCDG_Char8x16(uint8_t x, uint8_t y, uint8_t ch, uint8_t mix)
{
    // if(mix)
    // {
        // LCDG_image_mix(x, y, 8, 16, (font8x16 + (16*(ch-' '))));    
    // }
    // else
    // {
        // LCDG_image(x, y, 8, 16, (font8x16 + (16*(ch-' '))));
    // }
    LCDG_image(x, y, 8, 16, (font8x16 + (16*(ch-' '))), mix);
}


void LCDG_string12x24(uint8_t x, uint8_t y, uint8_t *str, uint8_t mix)
{
    while(*str)
    {
        LCDG_Char12x24(x, y, *str, mix);
        x += 12;
        str++;
    }
}

void LCDG_Char12x24(uint8_t x, uint8_t y, uint8_t ch, uint8_t mix)
{
    // if(mix)
    // {
        // LCDG_image_mix(x, y, 12, 24, (font12x24 + (36*(ch-' '))));    
    // }
    // else
    // {
        // LCDG_image(x, y, 12, 24, (font12x24 + (36*(ch-' '))));
    // }
    LCDG_image(x, y, 12, 24, (font12x24 + (36*(ch-' '))), mix);
}


void LCDG_digit32x48(uint8_t x, uint8_t y, uint8_t value, uint8_t mix)
{
    if(value > 9)
    {
        return;
    }
    
    // if(mix)
    // {
        // LCDG_image_mix(x, y, 32, 48, (font32x48 + (192 * value)));
    // }
    // else
    // {
        // LCDG_image(x, y, 32, 48, (font32x48 + (192 * value)));
    // }
    
    LCDG_image(x, y, 32, 48, (font32x48 + (192 * value)), mix);
}











