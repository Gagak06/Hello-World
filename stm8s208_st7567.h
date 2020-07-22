//stm8s208_st7567.h
#ifndef _STM8S208_ST7567_H_
#define _STM8S208_ST7567_H_

#include "stm8s_conf.h"

#define LCD_COL_OFFSET 5

#define LCD_PAGE_NUM_MAX    8
#define LCD_WIDTH_MAX       132

#define LCD_PORT        GPIOC
#define LCD_PIN_NCS      GPIO_PIN_2
#define LCD_PIN_RST     GPIO_PIN_3
#define LCD_PIN_A0      GPIO_PIN_4
#define LCD_PIN_SCLK    GPIO_PIN_5
#define LCD_PIN_SDA     GPIO_PIN_6

#define RES_SET     (GPIO_WriteHigh(LCD_PORT, LCD_PIN_RST))
#define RES_RESET   (GPIO_WriteLow(LCD_PORT, LCD_PIN_RST))
#define A0_SET      (GPIO_WriteHigh(LCD_PORT, LCD_PIN_A0))
#define A0_RESET    (GPIO_WriteLow(LCD_PORT, LCD_PIN_A0))
#define NCS_SET      (GPIO_WriteHigh(LCD_PORT, LCD_PIN_NCS))
#define NCS_RESET    (GPIO_WriteLow(LCD_PORT, LCD_PIN_NCS))

void lcd_st7567_init(void);
void lcd_st7567_deinit(void);

void LCD_sendData(uint8_t data);
void LCD_sendCMD(uint8_t cmd);
void LCD_delay_ms(uint16_t time);
void LCD_cmd_display_on(void);
void LCD_cmd_display_off(void);
void LCD_cmd_set_startline(uint8_t line);
void LCD_cmd_set_pageAddress(uint8_t num);
void LCD_cmd_set_columnAddress(uint8_t col);
void LCD_cmd_allPixelOn(uint8_t sw);
void LCD_cmd_inverseDisplay(uint8_t sw);
void LCD_cmd_reset(void);
void LCD_cmd_set_power(uint8_t setting);
void LCD_cmd_powerSave(void);
void LCD_cmd_set_electronicVolumeLevel(uint8_t level);
void LCD_cmd_set_regulationRatio(uint8_t ratio);
void LCD_cmd_set_COMDirection(uint8_t sw);
void LCD_cmd_set_SEGDirection(uint8_t sw);
void LCD_cmd_set_booster(uint8_t sw);

void LCD_clear(void);
void LCD_setPageData(uint8_t x, uint8_t page, uint8_t* pdata, uint8_t data_len);


void LCD_updateFull_it(uint8_t *pdata);
void spi_it(void);


#endif //_STM8S208_ST7567_H_