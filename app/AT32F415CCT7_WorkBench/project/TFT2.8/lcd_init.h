#ifndef __LCD_INIT_H__
#define __LCD_INIT_H__

#include "stdint.h"

#define USE_HORIZONTAL 2 // ���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����

#define LCD_W 320
#define LCD_H 240
#define LCD_SPI SPI2

void LCD_Init(void);
void LCD_Write_Bus(uint8_t dat);
void LCD_WR_DATA8(uint8_t dat);
void LCD_WR_DATA(uint16_t dat);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

#endif /* __BSP_LCD_H__ */
