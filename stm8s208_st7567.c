//stm8s208_st7567.c
#include "stm8s208_st7567.h"

typedef enum{
    LCD_SPI_IT_DATA,
    LCD_SPI_IT_PAGE,
    LCD_SPI_IT_COL0,
    LCD_SPI_IT_COL1    
}LCD_SPI_STATE;


void lcd_st7567_init_spi(void);
void lcd_st7567_init_gpio(void);
void lcd_st7567_init_lcd(void);
void lcd_st7567_delay(uint16_t time);

void lcd_st7567_spi_sendByte(uint8_t byte);
void lcd_st7567_sendData(uint8_t data);
void lcd_st7567_sendCMD(uint8_t cmd);



uint8_t *spi_pdata;
uint16_t spi_data_len;
uint8_t spi_it_page;
uint8_t spi_it_state;



void lcd_st7567_init(void)
{
    lcd_st7567_init_gpio();
    lcd_st7567_init_spi();
    lcd_st7567_init_lcd();
}

void lcd_st7567_init_gpio(void)
{
    GPIO_Init(LCD_PORT, LCD_PIN_NCS
                       |LCD_PIN_RST
                       |LCD_PIN_A0
                       |LCD_PIN_SCLK
                       |LCD_PIN_SDA     , GPIO_MODE_OUT_PP_LOW_SLOW);
    NCS_RESET;                   
    A0_RESET;
    RES_RESET;
}

void lcd_st7567_init_spi(void)
{
    SPI_Init(SPI_FIRSTBIT_MSB,
             SPI_BAUDRATEPRESCALER_2, 
             SPI_MODE_MASTER,
             SPI_CLOCKPOLARITY_LOW, 
             SPI_CLOCKPHASE_1EDGE, 
             SPI_DATADIRECTION_1LINE_TX,
             SPI_NSS_SOFT,
             0x07); //0x07 disable CRC
    SPI_Cmd(ENABLE);    //Enable SPI module    
}

void lcd_st7567_init_lcd(void)
{
    //LCD_delay_ms(3000); // 延时1ms , Datasheet 要求至少大于1us
    RES_SET;     //LCD 复位无效(H)
    
    lcd_st7567_delay(3000); //硬件复位
    LCD_cmd_reset();    
    lcd_st7567_delay(3000);
    
    LCD_cmd_set_power(0x07);
    LCD_cmd_display_on();
    
    LCD_cmd_set_regulationRatio(0x05);
    LCD_cmd_set_electronicVolumeLevel(0x15);
    
    LCD_cmd_set_SEGDirection(1);    //reverse SEG direction
    // LCD_cmd_set_SEGDirection(0);    //normal SEG direction
    LCD_cmd_set_COMDirection(0);    //normal COM direction
    
    LCD_cmd_inverseDisplay(0);      // no inverse
    LCD_cmd_allPixelOn(0);
    
    LCD_cmd_set_booster(0);
    
    LCD_cmd_set_startline(0);
		
    LCD_clear();
}

void lcd_st7567_deinit(void)
{
    LCD_cmd_display_off();
    LCD_cmd_allPixelOn(1);
    lcd_st7567_delay(3000);
    
}


void lcd_st7567_delay(uint16_t time)
{
    while(time != 0)
    {
        time--;
        _asm("NOP");
    }
}

void lcd_st7567_spi_sendByte(uint8_t byte)
{
    NCS_RESET;
    // _asm("NOP");
    
    SPI->DR = byte;
	while(SPI_GetFlagStatus(SPI_FLAG_BSY)){};
    
    NCS_SET;
}

void lcd_st7567_spi_sendLoop(uint8_t byte, uint8_t times)
{
    NCS_RESET;
    
    while(times--)
    {
        while(SPI_GetFlagStatus(SPI_FLAG_TXE) == 0){};
        SPI->DR = byte;
    }
    
    while(SPI_GetFlagStatus(SPI_FLAG_BSY)){};
    NCS_SET;
}

void lcd_st7567_spi_sendArray(uint8_t *pArray, uint8_t len)
{
    NCS_RESET;
    
    while(len--)
    {
        while(SPI_GetFlagStatus(SPI_FLAG_TXE) == 0){};
        SPI->DR = *pArray;
        pArray++;
    }
    
    while(SPI_GetFlagStatus(SPI_FLAG_BSY)){};
    NCS_SET;
}

void lcd_st7567_sendData(uint8_t data)
{
    A0_SET;
    lcd_st7567_spi_sendByte(data);
}

void lcd_st7567_sendCMD(uint8_t cmd)
{
    A0_RESET;		
    lcd_st7567_spi_sendByte(cmd);
}


// command
void LCD_cmd_display_on(void)
{
    lcd_st7567_sendCMD(0xAF);
}

void LCD_cmd_display_off(void)
{
    lcd_st7567_sendCMD(0xAE);
}

void LCD_cmd_set_startline(uint8_t line)
{
    lcd_st7567_sendCMD((0x40 | (line&0x3F)));
}

void LCD_cmd_set_pageAddress(uint8_t num)
{
    lcd_st7567_sendCMD((0xB0 | (num&0x0F)));    
}

void LCD_cmd_set_columnAddress(uint8_t col)
{
    lcd_st7567_sendCMD((0x10 | (col>>4)));
    lcd_st7567_sendCMD((col&0x0F));
}

void LCD_cmd_allPixelOn(uint8_t sw)
{
    lcd_st7567_sendCMD((0xA4 | (sw&0x01)));
}

void LCD_cmd_inverseDisplay(uint8_t sw)
{
    lcd_st7567_sendCMD((0xA6 | (sw&0x01)));
}

void LCD_cmd_reset(void)
{
    lcd_st7567_sendCMD(0xE2);
}

void LCD_cmd_set_power(uint8_t setting)
{
    lcd_st7567_sendCMD((0x28 | (setting&0x07)));
}

void LCD_cmd_powerSave(void)
{
    LCD_cmd_display_off();
    LCD_cmd_allPixelOn(1);
}

void LCD_cmd_set_electronicVolumeLevel(uint8_t level)
{
    lcd_st7567_sendCMD(0x81);
    lcd_st7567_sendCMD(level&0x3F);
}

void LCD_cmd_set_regulationRatio(uint8_t ratio)
{
    lcd_st7567_sendCMD(((ratio&0x07) | 0x20));    
}

void LCD_cmd_set_COMDirection(uint8_t sw)
{
    lcd_st7567_sendCMD((0xC0 | (sw&0x08)));
}

void LCD_cmd_set_SEGDirection(uint8_t sw)
{
    lcd_st7567_sendCMD((0xA0 | (sw&0x01)));
}

void LCD_cmd_set_booster(uint8_t sw)
{
    lcd_st7567_sendCMD(0xF8);
    lcd_st7567_sendCMD((sw&0x01));
}



void LCD_clear(void)
{
    uint8_t page, col;
    for(page = 0; page < LCD_PAGE_NUM_MAX; page++)
    {
        LCD_cmd_set_pageAddress(page);
        LCD_cmd_set_columnAddress(0);
        
        for(col = 0; col < LCD_WIDTH_MAX; col++)
        {
            lcd_st7567_sendData(0x00);
        }
    }    
}

void LCD_setPageData(uint8_t x, uint8_t page, uint8_t* pdata, uint8_t data_len)
{
    LCD_cmd_set_pageAddress(page);
    LCD_cmd_set_columnAddress(x);        
		
    A0_SET;
    lcd_st7567_spi_sendArray(pdata, data_len);
}

void LCD_updateFull_it(uint8_t *pdata)
{
    spi_data_len = 1024;
    spi_pdata = pdata;    
    
    SPI_ITConfig(SPI_IT_TXE, ENABLE);  // Enable Transmit buffer empty interrupt    
    while(SPI_GetFlagStatus(SPI_FLAG_BSY)){};
    
    A0_RESET;
    NCS_RESET;
    spi_it_page = 0;
    
    SPI->DR = (0xB0 | (spi_it_page&0x0F));  //start with set page number
    spi_it_page++;
    
    spi_it_state = LCD_SPI_IT_COL0;
   
}





void spi_it(void)
{
    switch(spi_it_state)
    {
        case LCD_SPI_IT_DATA:
            if(spi_data_len)
            {
                spi_data_len--;
                
                SPI->DR = *spi_pdata;
                spi_pdata++;
                
                if(spi_data_len%128 == 0)
                {
                    spi_it_state = LCD_SPI_IT_PAGE;
                }                
            }
            else
            {
                //END
                SPI_ITConfig(SPI_IT_TXE, DISABLE);
                while(SPI_GetFlagStatus(SPI_FLAG_BSY)){};
                NCS_SET;   
            }
            break;
            
        case LCD_SPI_IT_PAGE:
            while(SPI_GetFlagStatus(SPI_FLAG_BSY)){};
            A0_RESET;
            
            SPI->DR = (0xB0 | (spi_it_page&0x0F));
            spi_it_page++;
            spi_it_state = LCD_SPI_IT_COL0;
            break;
            
        case LCD_SPI_IT_COL0:
            //set colum first byte
            //colum always 0
            SPI->DR = 0x10;
            spi_it_state = LCD_SPI_IT_COL1;
            break;
            
        case LCD_SPI_IT_COL1:
            //set colum second byte
            //colum always 0
            SPI->DR = ((LCD_COL_OFFSET-1)&0x0f);
            spi_it_state = LCD_SPI_IT_DATA ;
            
            while(SPI_GetFlagStatus(SPI_FLAG_BSY)){};
            A0_SET;
            break;
    }
}
