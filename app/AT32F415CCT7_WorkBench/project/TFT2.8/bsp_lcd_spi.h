#ifndef __BSP_LCD_SPI__
#define __BSP_LCD_SPI__

#include "at32f415.h"

// #define LCD_RES_Clr()   gpio_bits_reset(GPIOA,GPIO_PINS_11);
// #define LCD_RES_Set()   gpio_bits_set(GPIOA,GPIO_PINS_11);

// #define LCD_DC_Clr()    gpio_bits_reset(GPIOA,GPIO_PINS_8);
// #define LCD_DC_Set()    gpio_bits_set(GPIOA,GPIO_PINS_8);

// #define LCD_CS_Clr()    gpio_bits_reset(GPIOB,GPIO_PINS_12);
// #define LCD_CS_Set()    gpio_bits_set(GPIOB,GPIO_PINS_12);

#define LCD_RES_Clr() GPIOA->clr = 0x0800
#define LCD_RES_Set() GPIOA->scr = 0x0800

#define LCD_DC_Clr() GPIOA->clr = 0x0100
#define LCD_DC_Set() GPIOA->scr = 0x0100

#define LCD_CS_Clr() GPIOB->clr = 0x1000
#define LCD_CS_Set() GPIOB->scr = 0x1000

void spi_dma_write_bytes(uint8_t *pbuffer, uint32_t length);
void spi_dma_write_halfwords(uint16_t *pbuffer, uint32_t length);
uint8_t spi_write_read_byte(uint8_t data);

#endif
