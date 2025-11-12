#ifndef __LCD_H__
#define __LCD_H__
#include "stdint.h"
#include "at32f415.h"

void LCD_Clear(uint16_t color);
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);

void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey,
                     uint8_t mode);
void LCD_ShowChinese12x12(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey,
                          uint8_t mode);
void LCD_ShowChinese16x16(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey,
                          uint8_t mode);
void LCD_ShowChinese24x24(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey,
                          uint8_t mode);
void LCD_ShowChinese32x32(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey,
                          uint8_t mode);

void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey,
                  uint8_t mode);
void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey,
                    uint8_t mode);
uint32_t mypow(uint8_t m, uint8_t n);
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);
void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc,
                       uint8_t sizey);
void LCD_ShowFloatNum(u16 x, u16 y, float num, u8 float_n, uint16_t fc, uint16_t bc, u8 size);
void LCD_VISION(uint16_t x, uint16_t y,  uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);
void LCD_CH(uint16_t x, uint16_t y,  uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey,uint8_t in_ch);
void LCD_MIN(uint16_t x, uint16_t y,  uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);
void LCD_Show_SLEEP_TIME(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey);
void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[]);
void LCD_Test(void);
void LCD_Display(void);
void insertChar(char *str, char ch, int pos);
void DrawProgressBar(u16 x1, u16 y1, u16 x2, u16 y2, u8 scale, u16 bar_color, u16 bc_color, uint8_t Pic_Num);
void TranferPicturetoTFT_LCD(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint8_t Pic_Num);

void LCD_Show_Grid(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t line_count, uint16_t color);
void LCD_DrawLineSegment_Y(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t line_space, uint16_t color);
void LCD_DrawLineSegment_X(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t line_space, uint16_t color);
void LCD_Show_Curve(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, float s1_temp, float s1_power);
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define CRIMSON 0XD8A7
#define TROPICAL_ORANGE 0XFC06
#define Scarlet 0XF920
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430
#define DARKBLUE 0X01CF
#define LIGHTBLUE 0X7D7C
#define GRAYBLUE 0X5458
#define LIGHTGREEN 0X841F
#define LGRAY 0XC618
#define LGRAYBLUE 0XA651
#define LBBLUE 0X2B12

#define BLOCK_SIZE 4096

#define PIC_ADDRESS_1  0x00000000
#define PIC_ADDRESS_2  0x00025800
#define PIC_ADDRESS_3  0x00046F00
#define PIC_ADDRESS_4  0x00068600
#define PIC_ADDRESS_5  0x00089D00
#define PIC_ADDRESS_6  0x000AB400
#define PIC_ADDRESS_7  0x000CCB00
#define PIC_ADDRESS_8  0x000EE200
#define PIC_ADDRESS_9  0x0010F900
#define PIC_ADDRESS_10 0x00131000
#define PIC_ADDRESS_11 0x00152700
#define PIC_ADDRESS_12 0x00173E00
#define PIC_ADDRESS_13 0x00195500
#define PIC_ADDRESS_14 0x001B6C00
#define PIC_ADDRESS_15 0x001D8300
#define PIC_ADDRESS_16 0x001F9A00
#define PIC_ADDRESS_17 0x0021B100
#define PIC_ADDRESS_18 0x0023C800
#define PIC_ADDRESS_19 0x0025DF00
#define PIC_ADDRESS_20 0x0027F600
#define PIC_ADDRESS_21 0x002A0D00
#define PIC_ADDRESS_22 0x002C2400
#define PIC_ADDRESS_23 0x002E3B00
#define PIC_ADDRESS_24 0x00305200
#define PIC_ADDRESS_25 0x00326900
#define PIC_ADDRESS_26 0x00348000
#define PIC_ADDRESS_27 0x00369700
#define PIC_ADDRESS_28 0x0038AE00
#define PIC_ADDRESS_29 0x003AC500
#define PIC_ADDRESS_30 0x003CDC00
#define PIC_ADDRESS_31 0x003EF300
#define PIC_ADDRESS_32 0x00410A00
#define PIC_ADDRESS_33 0x00432100
#define PIC_ADDRESS_34 0x00453800
#define PIC_ADDRESS_35 0x00474F00
#define PIC_ADDRESS_36 0x00496600
#define PIC_ADDRESS_37 0x004B7D00
#define PIC_ADDRESS_38 0x004D9400
#define PIC_ADDRESS_39 0x004FAB00
#define PIC_ADDRESS_40 0x0051C200
#define PIC_ADDRESS_41 0x0053D900
#define PIC_ADDRESS_42 0x0055F000
#define PIC_ADDRESS_43 0x00580700
#define PIC_ADDRESS_44 0x005A1E00
#define PIC_ADDRESS_45 0x005C3500
#define PIC_ADDRESS_46 0x005E4C00
#define PIC_ADDRESS_47 0x00606300
#define PIC_ADDRESS_48 0x00627A00
#define PIC_ADDRESS_49 0x00649100
#define PIC_ADDRESS_50 0x0066A800
#define PIC_ADDRESS_51 0x0066AA88
#define PIC_ADDRESS_52 0x0066AD10
#define PIC_ADDRESS_53 0x0066B054
#define PIC_ADDRESS_54 0x0066B398
#define PIC_ADDRESS_55 0x0066C798
#define PIC_ADDRESS_56 0x0066DB98
#define PIC_ADDRESS_57 0x0066EF98
#define PIC_ADDRESS_58 0x00670398
#define PIC_ADDRESS_59 0x00671798
#define PIC_ADDRESS_60 0x00672B98
#define PIC_ADDRESS_61 0x00672EB6
#define PIC_ADDRESS_62 0x006731D4
#define PIC_ADDRESS_63 0x006734A6
#define PIC_ADDRESS_64 0x00673778
#define PIC_ADDRESS_65 0x0067BD10
#define PIC_ADDRESS_66 0x006842A8
#define PIC_ADDRESS_67 0x0068C2CC
#define PIC_ADDRESS_68 0x006942F0
#define PIC_ADDRESS_69 0x0069C314
#define PIC_ADDRESS_70 0x006A4338
#define PIC_ADDRESS_71 0x006AC35C
#define PIC_ADDRESS_72 0x006B4380
#define PIC_ADDRESS_73 0x006BC3A4
#define PIC_ADDRESS_74 0x006C43C8
#define PIC_ADDRESS_75 0x006C55D4
#define PIC_ADDRESS_76 0x006C67E0
#define PIC_ADDRESS_77 0x006C79EC
#define PIC_ADDRESS_78 0x006C8BF8
#define PIC_ADDRESS_79 0x006C9918
#define PIC_ADDRESS_80 0x006CA638
#define PIC_ADDRESS_81 0x006CB358
#define PIC_ADDRESS_82 0x006CC078
#define PIC_ADDRESS_83 0x006CD7D8
#define PIC_ADDRESS_84 0x006CEF38
#define PIC_ADDRESS_85 0x006D0698
#define PIC_ADDRESS_86 0x006D1DF8
#define PIC_ADDRESS_87 0x006D1F00
//#define PIC_ADDRESS_88 
//#define PIC_ADDRESS_89 
//#define PIC_ADDRESS_90 
//#define PIC_ADDRESS_91 




















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#endif
