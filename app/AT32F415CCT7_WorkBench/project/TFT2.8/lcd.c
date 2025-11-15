#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lcd.h"
#include "bsp_lcd_spi.h"
#include "lcd_init.h"
#include "lcd_font.h"

#include "FWS2pro_handle.h"

#include "at32_spiflash.h"

void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t color1[1], t = 1;
    u32 num, num1;
    color1[0] = color;
    num = (xend - xsta) * (yend - ysta);
    LCD_Address_Set(xsta, ysta, xend - 1, yend - 1);
    LCD_CS_Clr();

    while (t)
    {
        if (num > 65534)
        {
            num -= 65534;
            num1 = 65534;
        }
        else
        {
            t = 0;
            num1 = num;
        }

        spi_dma_write_halfwords(color1, num1);
    }

    LCD_CS_Set();
}

void LCD_Clear(uint16_t color)
{
    LCD_Fill(0, 0, LCD_W, LCD_H, color);
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_Address_Set(x, y, x, y);
    LCD_WR_DATA(color);
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1;
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;

    if (delta_x > 0)
    {
        incx = 1;
    }
    else if (delta_x == 0)
    {
        incx = 0;
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
    {
        distance = delta_x;
    }
    else
    {
        distance = delta_y;
    }

    for (t = 0; t < distance + 1; t++)
    {
        LCD_DrawPoint(uRow, uCol, color);
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}

void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int a, b;
    a = 0;
    b = r;

    while (a <= b)
    {
        LCD_DrawPoint(x0 - b, y0 - a, color); // 3
        LCD_DrawPoint(x0 + b, y0 - a, color); // 0
        LCD_DrawPoint(x0 - a, y0 + b, color); // 1
        LCD_DrawPoint(x0 - a, y0 - b, color); // 2
        LCD_DrawPoint(x0 + b, y0 + a, color); // 4
        LCD_DrawPoint(x0 + a, y0 - b, color); // 5
        LCD_DrawPoint(x0 + a, y0 + b, color); // 6
        LCD_DrawPoint(x0 - b, y0 + a, color); // 7
        a++;

        if ((a * a + b * b) > (r * r))
        {
            b--;
        }
    }
}

void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    while (*s != 0)
    {
        if (sizey == 12)
        {
            LCD_ShowChinese12x12(x, y, s, fc, bc, sizey, mode);
        }
        else if (sizey == 16)
        {
            LCD_ShowChinese16x16(x, y, s, fc, bc, sizey, mode);
        }
        else if (sizey == 24)
        {
            LCD_ShowChinese24x24(x, y, s, fc, bc, sizey, mode);
        }
        else if (sizey == 32)
        {
            LCD_ShowChinese32x32(x, y, s, fc, bc, sizey, mode);
        }
        else
        {
            return;
        }

        s += 2;
        x += sizey;
    }
}

void LCD_ShowChinese12x12(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;
    uint16_t TypefaceNum;
    uint16_t x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont12) / sizeof(typFNT_GB12);

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);

            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode)
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j))
                        {
                            LCD_WR_DATA(fc);
                        }
                        else
                        {
                            LCD_WR_DATA(bc);
                        }

                        m++;

                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j))
                        {
                            LCD_DrawPoint(x, y, fc);
                        }

                        x++;

                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }

        continue;
    }
}

void LCD_ShowChinese16x16(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;
    uint16_t TypefaceNum;
    uint16_t x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont16) / sizeof(typFNT_GB16);

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);

            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode)
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j))
                        {
                            LCD_WR_DATA(fc);
                        }
                        else
                        {
                            LCD_WR_DATA(bc);
                        }

                        m++;

                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j))
                        {
                            LCD_DrawPoint(x, y, fc);
                        }

                        x++;

                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }

        continue;
    }
}

void LCD_ShowChinese24x24(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;
    uint16_t TypefaceNum;
    uint16_t x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont24) / sizeof(typFNT_GB24);

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);

            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode)
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))
                        {
                            LCD_WR_DATA(fc);
                        }
                        else
                        {
                            LCD_WR_DATA(bc);
                        }

                        m++;

                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))
                        {
                            LCD_DrawPoint(x, y, fc);
                        }

                        x++;

                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }

        continue;
    }
}

void LCD_ShowChinese32x32(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;
    uint16_t TypefaceNum;
    uint16_t x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont32) / sizeof(typFNT_GB32);

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);

            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode)
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))
                        {
                            LCD_WR_DATA(fc);
                        }
                        else
                        {
                            LCD_WR_DATA(bc);
                        }

                        m++;

                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))
                        {
                            LCD_DrawPoint(x, y, fc);
                        }

                        x++;

                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }

        continue;
    }
}

void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t temp, sizex, t, m = 0;
    uint16_t i, TypefaceNum;
    uint16_t x0 = x;
    sizex = sizey / 2;
    TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    num = num - ' ';
    LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1);

    for (i = 0; i < TypefaceNum; i++)
    {
        if (sizey == 12)
        {
            temp = ascii_1206[num][i];
        }
        else if (sizey == 16)
        {
            temp = ascii_1608[num][i];
        }
        else if (sizey == 24)
        {
            temp = ascii_2412[num][i];
        }
        else if (sizey == 32)
        {
            temp = ascii_3216[num][i];
        }
        else if (sizey == 56)
        {
            temp = ascii_5628[num][i];
        }
        else if (sizey == 64)
        {
            temp = ascii_6432[num][i];
        }
        else if (sizey == 120)
        {
            temp = ascii_12060[num][i];
        }
#if 0
        else if (sizey == 128)
        {
            temp = ascii_12864[num][i]; 
        }
#endif
        else
        {
            return;
        }

        for (t = 0; t < 8; t++)
        {
            if (!mode)
            {
                if (temp & (0x01 << t))
                {
                    LCD_WR_DATA(fc);
                }
                else
                {
                    LCD_WR_DATA(bc);
                }

                m++;

                if (m % sizex == 0)
                {
                    m = 0;
                    break;
                }
            }
            else
            {
                if (temp & (0x01 << t))
                {
                    LCD_DrawPoint(x, y, fc);
                }

                x++;

                if ((x - x0) == sizex)
                {
                    x = x0;
                    y++;
                    break;
                }
            }
        }
    }
}

void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    while (*p != '\0')
    {
        LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
        x += sizey / 2;
        p++;
    }
}

uint8_t Calc_IntNum(uint64_t calc_num)
{
    uint8_t Counter = 0;
    uint16_t num2 = calc_num;
    int var = 0;
    float num_temp = (float)num2 / 10;

    for (var = 0; var < 1000; ++var)
    {
        if (num_temp * 10 >= 1)
        {
            num_temp = num_temp / 10;
            Counter++;
        }
        else
        {
            break;
        }
    }

    return Counter;
}

u32 mypow(uint8_t m, uint8_t n)
{
    u32 result = 1;

    while (n--)
    {
        result *= m;
    }

    return result;
}

void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t sizex = sizey / 2;

    for (t = 0; t < len; t++)
    {
        temp = (num / mypow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
//                LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
				LCD_ShowChar(x + t * sizex, y, '0' , fc, bc, sizey, 0);
                continue; 
            }
            else
            {
                enshow = 1;
            }
        }

        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

void LCD_SHOW_BLACK(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
	uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t sizex = sizey / 2;

    for (t = 0; t < len; t++)
    {
        temp = (num / mypow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
                continue; 
            }
            else
            {
                enshow = 1;
            }
        }

        LCD_ShowChar(x + t * sizex, y,' ', fc, bc, sizey, 0);
    }
	
	
    for (t = 0; t < len; t++)
    {	
	   switch(t)
		{
			case 1:
				LCD_ShowChar(x + t * sizex, y, ' ' , fc, bc, sizey, 0);
				break;
			case 2:
				LCD_ShowChar(x + t * sizex, y, ' ' , fc, bc, sizey, 0);
				break;
			case 3:
				LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
				break;
			case 4:
				LCD_ShowChar(x + t * sizex, y, ' ' , fc, bc, sizey, 0);
				break;
		}
		
		
    }
}

void LCD_Show_SLEEP_TIME(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t sizex = sizey / 2;
	uint8_t decimal_data = 0;
	uint8_t bai = 0,shi = 0,ge = 0;

	decimal_data = (uint16_t)(num * 10) % 10;
	bai = (uint16_t)num / 100;
	shi = (uint16_t)num / 10 % 10;
	ge = (uint16_t)num % 10;
    for (t = 0; t < len; t++)
    {
       switch(t)
		{
//			case 1:
//				if(bai == 0)
//					LCD_ShowChar(x + t * sizex, y, '0', fc, bc, sizey, 0);
//				else
//					LCD_ShowChar(x + t * sizex, y, bai+48, fc, bc, sizey, 0);
//				break;
			case 1:
				if(shi == 0)
					LCD_ShowChar(x + t * sizex, y, '0', fc, bc, sizey, 0);
				else
					LCD_ShowChar(x + t * sizex, y, shi+48, fc, bc, sizey, 0);
				break;
			case 2:
				if(ge == 0)
					LCD_ShowChar(x + t * sizex, y, '0', fc, bc, sizey, 0);
				else
					LCD_ShowChar(x + t * sizex, y, ge+48, fc, bc, sizey, 0);
				break;
			case 3:
				LCD_ShowChar(x + t * sizex, y, '.', fc, bc, sizey, 0);
				break;
			case 4:
				if(decimal_data == 0)
					LCD_ShowChar(x + t * sizex, y, '0', fc, bc, sizey, 0);
				else 	
					LCD_ShowChar(x + t * sizex, y, decimal_data+48, fc, bc, sizey, 0);
				break;
		}
    }
}

void LCD_VISION(uint16_t x, uint16_t y,  uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
	uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t sizex = sizey / 2;

    for (t = 0; t < len; t++)
    {
		switch(t)
		{
			case 1:
				LCD_ShowChar(x + t * sizex, y, 'V', fc, bc, sizey, 0);
				break;
			case 2:
				LCD_ShowChar(x + t * sizex, y, '1', fc, bc, sizey, 0);
				break;
			case 3:
				LCD_ShowChar(x + t * sizex, y, '.', fc, bc, sizey, 0);
				break;
			case 4:
				LCD_ShowChar(x + t * sizex, y, '1', fc, bc, sizey, 0);
				break;
			case 5:
				LCD_ShowChar(x + t * sizex, y, '.', fc, bc, sizey, 0);
				break;
			case 6:
				LCD_ShowChar(x + t * sizex, y, '0', fc, bc, sizey, 0);
				break;
		}
        
    }
}
void LCD_CH(uint16_t x, uint16_t y,  uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey,uint8_t in_ch)
{
	uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t sizex = sizey / 2;

    for (t = 0; t < len; t++)
    {
		switch(t)
		{
			case 1:
				LCD_ShowChar(x + t * sizex, y, 'C', fc, bc, sizey, 0);
				break;
			case 2:
				LCD_ShowChar(x + t * sizex, y, 'H', fc, bc, sizey, 0);
				break;
			case 3:
				if(in_ch == 1)
					LCD_ShowChar(x + t * sizex, y, '1', fc, bc, sizey, 0);
				else if(in_ch == 2)
					LCD_ShowChar(x + t * sizex, y, '2', fc, bc, sizey, 0);
				else if(in_ch == 3)
					LCD_ShowChar(x + t * sizex, y, '3', fc, bc, sizey, 0);
				break;
		}    
    }
}

void LCD_MIN(uint16_t x, uint16_t y,  uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
	uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t sizex = sizey / 2;

    for (t = 0; t < len; t++)
    {
		switch(t)
		{
			case 1:
				LCD_ShowChar(x + t * sizex, y, 'M', fc, bc, sizey, 0);
				break;
			case 2:
				LCD_ShowChar(x + t * sizex, y, 'i', fc, bc, sizey, 0);
				break;
			case 3:
				LCD_ShowChar(x + t * sizex, y, 'n', fc, bc, sizey, 0);
				break;
		}    
    }
}


void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    volatile uint8_t t, temp, sizex;
    uint16_t num1;
    sizex = sizey / 2;
    num1 = num * 1000;

    for (t = 0; t < len; t++)
    {
        temp = (num1 / mypow(10, len - t - 1)) % 10;

        if (t == (len - 2))
        {
            LCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
            t++;
            len += 1;
        }

        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

void LCD_ShowFloatNum(u16 x, u16 y, float num, u8 float_n, uint16_t fc, uint16_t bc, u8 size)
{
    uint32_t mi_temp = 1;
    uint32_t zhengshu = 0;
    uint32_t xiaoshu = 0;
    uint8_t zhengshu_counter = 0;
    // uint8_t xiaoshu_counter = 0;
    int var = 0;

    for (var = 0; var < float_n; ++var)
    {
        mi_temp = 10 * mi_temp;
    }

    zhengshu = (uint32_t)num;
    xiaoshu = (uint32_t)((num - zhengshu) * mi_temp);
    zhengshu_counter = Calc_IntNum(zhengshu);
    // xiaoshu_counter = Calc_IntNum(xiaoshu);
    LCD_ShowIntNum(x, y, zhengshu, zhengshu_counter, fc, bc,
                   size);
    LCD_ShowChar((x + size * zhengshu_counter / 2), y, '.', fc, bc, size,
                 0);
    LCD_ShowIntNum((x + size * zhengshu_counter / 2 + size / 2), y, xiaoshu, float_n, fc, bc,
                   size);
}

void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[])
{
    uint8_t t = 1;
    u32 num = length * width * 2, num1;
    LCD_Address_Set(x, y, x + length - 1, y + width - 1);
    LCD_CS_Clr();

    while (t)
    {
        if (num > 65534)
        {
            num -= 65534;
            num1 = 65534;
        }
        else
        {
            t = 0;
            num1 = num;
        }

        spi_dma_write_bytes((uint8_t *)pic, num1);
        pic += 65534;
    }

    LCD_CS_Set();
}

char string[20];
void insertChar(char *str, char ch, int pos)
{
    int len = strlen(str);

    for (int i = len; i >= pos; i--)
    {
        str[i + 1] = str[i];
    }

    str[pos] = ch;
}

void TranferPicturetoTFT_LCD(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint8_t Pic_Num)
{
    uint32_t uiDataLength = 0;
    uint32_t uiPic_Addr = NULL;
    uint16_t usPic_Data;
    static u8 Read_data[BLOCK_SIZE] = {0};
    uiDataLength = (width) * (height) * 2;
	if(sFWS2_t.general_parameter.language_state == CHINESE)
	{
		switch (Pic_Num)
		{
		case 1:
			uiPic_Addr = PIC_ADDRESS_1;
			break;
		case 2:
			uiPic_Addr = PIC_ADDRESS_2;
			break;
		case 3:
			uiPic_Addr = PIC_ADDRESS_3;
			break;
		case 4:
			uiPic_Addr = PIC_ADDRESS_4;
			break;
		case 6:
			uiPic_Addr = PIC_ADDRESS_6;
			break;
		case 8:
			uiPic_Addr = PIC_ADDRESS_8;
			break;
		case 10:
			uiPic_Addr = PIC_ADDRESS_10;
			break;
		case 12:
			uiPic_Addr = PIC_ADDRESS_12;
			break;
		case 14:
			uiPic_Addr = PIC_ADDRESS_14;
			break;
		case 16:
			uiPic_Addr = PIC_ADDRESS_16;
			break;
		case 18:
			uiPic_Addr = PIC_ADDRESS_18;
			break;
		case 20:
			uiPic_Addr = PIC_ADDRESS_20;
			break;
		case 22:
			uiPic_Addr = PIC_ADDRESS_22;
			break;
		case 24:
			uiPic_Addr = PIC_ADDRESS_24;
			break;
		case 26:
			uiPic_Addr = PIC_ADDRESS_26;
			break;
		case 27:
			uiPic_Addr = PIC_ADDRESS_27;
			break;
		case 29:
			uiPic_Addr = PIC_ADDRESS_29;
			break;
		case 31:
			uiPic_Addr = PIC_ADDRESS_31;
			break;
		case 33:
			uiPic_Addr = PIC_ADDRESS_33;
			break;
		case 35:
			uiPic_Addr = PIC_ADDRESS_35;
			break;
		case 37:
			uiPic_Addr = PIC_ADDRESS_37;
			break;
		case 39:
			uiPic_Addr = PIC_ADDRESS_39;
			break;
		case 41:
			uiPic_Addr = PIC_ADDRESS_41;
			break;
		case 42:
			uiPic_Addr = PIC_ADDRESS_42;
			break;
		case 44:
			uiPic_Addr = PIC_ADDRESS_44;
			break;
		case 46:
			uiPic_Addr = PIC_ADDRESS_46;
			break;
		case 48:
			uiPic_Addr = PIC_ADDRESS_48;
			break;
		case 50:
			uiPic_Addr = PIC_ADDRESS_50;
			break;
		case 51:
			uiPic_Addr = PIC_ADDRESS_51;
			break;
		case 52:
			uiPic_Addr = PIC_ADDRESS_52;
			break;
		case 53:
			uiPic_Addr = PIC_ADDRESS_53;
			break;
		case 54:
			uiPic_Addr = PIC_ADDRESS_54;
			break;
		case 55:
			uiPic_Addr = PIC_ADDRESS_55;
			break;
		case 56:
			uiPic_Addr = PIC_ADDRESS_56;
			break;
		case 57:
			uiPic_Addr = PIC_ADDRESS_57;
			break;
		case 58:
			uiPic_Addr = PIC_ADDRESS_58;
			break;
		case 59:
			uiPic_Addr = PIC_ADDRESS_59;
			break;
		case 60:
			uiPic_Addr = PIC_ADDRESS_60;
			break;
		case 61:
			uiPic_Addr = PIC_ADDRESS_61;
			break;
		case 62:
			uiPic_Addr = PIC_ADDRESS_62;
			break;
		case 63:
			uiPic_Addr = PIC_ADDRESS_63;
			break;
		case 64:
			uiPic_Addr = PIC_ADDRESS_64;
			break;
		case 66:
			uiPic_Addr = PIC_ADDRESS_66;
			break;
		case 68:
			uiPic_Addr = PIC_ADDRESS_68;
			break;
		case 70:
			uiPic_Addr = PIC_ADDRESS_70;
			break;
		case 72:
			uiPic_Addr = PIC_ADDRESS_72;
			break;
		case 74:
			uiPic_Addr = PIC_ADDRESS_74;
			break;
		case 75:
			uiPic_Addr = PIC_ADDRESS_75;
			break;
		case 78:
			uiPic_Addr = PIC_ADDRESS_78;
			break;
		case 79:
			uiPic_Addr = PIC_ADDRESS_79;
			break;
		case 82:
			uiPic_Addr = PIC_ADDRESS_82;
			break;
		case 83:
			uiPic_Addr = PIC_ADDRESS_83;
			break;
		case 86:
			uiPic_Addr = PIC_ADDRESS_86;
			break;
		case 87:
			uiPic_Addr = PIC_ADDRESS_87;
			break;
		}
	}
	else if(sFWS2_t.general_parameter.language_state == ENGLISH)
	{
		switch (Pic_Num)
		{
		case 1:
			uiPic_Addr = PIC_ADDRESS_1;
			break;
		case 2:
			uiPic_Addr = PIC_ADDRESS_2;
			break;
		case 3:
			uiPic_Addr = PIC_ADDRESS_3;
			break;
		case 4:
			uiPic_Addr = PIC_ADDRESS_5;
			break;
		case 6:
			uiPic_Addr = PIC_ADDRESS_7;
			break;
		case 8:
			uiPic_Addr = PIC_ADDRESS_9;
			break;
		case 10:
			uiPic_Addr = PIC_ADDRESS_11;
			break;
		case 12:
			uiPic_Addr = PIC_ADDRESS_13;
			break;
		case 14:
			uiPic_Addr = PIC_ADDRESS_15;
			break;
		case 16:
			uiPic_Addr = PIC_ADDRESS_17;
			break;
		case 18:
			uiPic_Addr = PIC_ADDRESS_19;
			break;
		case 20:
			uiPic_Addr = PIC_ADDRESS_21;
			break;
		case 22:
			uiPic_Addr = PIC_ADDRESS_23;
			break;
		case 24:
			uiPic_Addr = PIC_ADDRESS_25;
			break;
		case 26:
			uiPic_Addr = PIC_ADDRESS_26;
			break;
		case 27:
			uiPic_Addr = PIC_ADDRESS_28;
			break;
		case 29:
			uiPic_Addr = PIC_ADDRESS_30;
			break;
		case 31:
			uiPic_Addr = PIC_ADDRESS_32;
			break;
		case 33:
			uiPic_Addr = PIC_ADDRESS_34;
			break;
		case 35:
			uiPic_Addr = PIC_ADDRESS_36;
			break;
		case 37:
			uiPic_Addr = PIC_ADDRESS_38;
			break;
		case 40:
			uiPic_Addr = PIC_ADDRESS_40;
			break;
		case 41:
			uiPic_Addr = PIC_ADDRESS_41;
			break;
		case 42:
			uiPic_Addr = PIC_ADDRESS_43;
			break;
		case 44:
			uiPic_Addr = PIC_ADDRESS_45;
			break;
		case 46:
			uiPic_Addr = PIC_ADDRESS_47;
			break;
		case 48:
			uiPic_Addr = PIC_ADDRESS_49;
			break;
		case 50:
			uiPic_Addr = PIC_ADDRESS_50;
			break;
		case 51:
			uiPic_Addr = PIC_ADDRESS_51;
			break;
		case 52:
			uiPic_Addr = PIC_ADDRESS_52;
			break;
		case 53:
			uiPic_Addr = PIC_ADDRESS_53;
			break;
		case 54:
			uiPic_Addr = PIC_ADDRESS_54;
			break;
		case 55:
			uiPic_Addr = PIC_ADDRESS_55;
			break;
		case 56:
			uiPic_Addr = PIC_ADDRESS_56;
			break;
		case 57:
			uiPic_Addr = PIC_ADDRESS_57;
			break;
		case 58:
			uiPic_Addr = PIC_ADDRESS_58;
			break;
		case 59:
			uiPic_Addr = PIC_ADDRESS_59;
			break;	
		case 60:
			uiPic_Addr = PIC_ADDRESS_60;
			break;
		case 61:
			uiPic_Addr = PIC_ADDRESS_61;
			break;
		case 62:
			uiPic_Addr = PIC_ADDRESS_62;
			break;
		case 63:
			uiPic_Addr = PIC_ADDRESS_63;
			break;
		case 64:
			uiPic_Addr = PIC_ADDRESS_65;
			break;
		case 66:
			uiPic_Addr = PIC_ADDRESS_67;
			break;
		case 68:
			uiPic_Addr = PIC_ADDRESS_69;
			break;
		case 70:
			uiPic_Addr = PIC_ADDRESS_71;
			break;
		case 72:
			uiPic_Addr = PIC_ADDRESS_73;
			break;
		case 74:
			uiPic_Addr = PIC_ADDRESS_76;
			break;
		case 75:
			uiPic_Addr = PIC_ADDRESS_77;
			break;
		case 78:
			uiPic_Addr = PIC_ADDRESS_80;
			break;
		case 79:
			uiPic_Addr = PIC_ADDRESS_81;
			break;
		case 82:
			uiPic_Addr = PIC_ADDRESS_84;
			break;
		case 83:
			uiPic_Addr = PIC_ADDRESS_85;
			break;
		case 86:
			uiPic_Addr = PIC_ADDRESS_86;
			break;
		case 87:
			uiPic_Addr = PIC_ADDRESS_87;
			break;
		}
	}
    width = width + x1;
    height = height + y1;
    LCD_Address_Set(x1, y1, width - 1, height - 1);

    while (uiDataLength)
    {
        if (BLOCK_SIZE > uiDataLength)
        {
            spiflash_read(Read_data, uiPic_Addr, (uiDataLength));

            for (u32 i = 0; i < (uiDataLength); i += 2)
            {
                usPic_Data = Read_data[i];
                usPic_Data <<= 8;
                usPic_Data = usPic_Data + Read_data[i + 1];
                LCD_WR_DATA(usPic_Data);
            }

            uiPic_Addr += uiDataLength;
            uiDataLength -= uiDataLength;
        }
        else
        {
            spiflash_read(Read_data, uiPic_Addr, BLOCK_SIZE);

            for (u32 i = 0; i < BLOCK_SIZE; i += 2)
            {
                usPic_Data = Read_data[i];
                usPic_Data <<= 8;
                usPic_Data = usPic_Data + Read_data[i + 1];
                LCD_WR_DATA(usPic_Data);
            }

            uiPic_Addr += BLOCK_SIZE;
            uiDataLength -= BLOCK_SIZE;
        }
    }
}

#if 1
void DrawProgressBar(u16 x1, u16 y1, u16 x2, u16 y2, u8 scale, u16 bar_color, u16 bc_color, uint8_t Pic_Num)
{
    if (scale >= 98)
    {
        scale = 98;
    }

    if (scale <= 5)
    {
        scale = 5;
    }

    u16 Bar_Length = y2 - y1;
    u16 len = Bar_Length * (100 - scale) / 100;
    LCD_Fill(x1, y1 - 4, x2+2 , y1 + len - (11 / 2), bc_color);
	TranferPicturetoTFT_LCD(x1, (y1 + len) - (11 / 2), 12, 11, Pic_Num);
    LCD_Fill(x1, y1 + len + (11 / 2)+1, x2+1, y2, bar_color);
}

void LCD_DrawLineSegment_X(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t line_space, uint16_t color)
{
    uint16_t line_number = (x2 - x1) / line_space;

    for (int i = 0; i < line_number; i++)
    {
        if (i % 2)
        {
            LCD_DrawLine(x1 + i * line_space, y1, x1 + i * line_space + line_space, y2, color);
        }
    }
}

void LCD_DrawLineSegment_Y(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t line_space, uint16_t color)
{
    uint16_t line_number = (y2 - y1) / line_space;

    for (int i = 0; i < line_number; i++)
    {
        if (i % 2)
        {
            LCD_DrawLine(x1, y1 + i * line_space, x1, y1 + i * line_space + line_space, color);
        }
    }
}

// 通用电压到Y坐标映射函数
int voltage_to_y(float voltage, float v_min, float v_max, int y_top, int y_bottom)
{
    // 计算显示区域高度
    int display_height = y_bottom - y_top;

    // 确保电压在范围内
    if (voltage < v_min)
    {
        voltage = v_min;
    }

    if (voltage > v_max)
    {
        voltage = v_max;
    }

    // 计算电压在范围内的比例位置
    float position = (voltage - v_min) / (v_max - v_min);
    // 反转比例（因为屏幕Y轴向下增加）
    position = 1.0 - position;
    // 计算Y坐标（四舍五入）
    int y = y_top + (int)(position * display_height + 0.5);
    return y;
}

#if 1
void LCD_Show_Grid(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t line_count, uint16_t color)
{
    uint8_t i, j;
    float x_line_space = 0;
    float y_line_space = 0;
    uint8_t buff[10];
    x_line_space = (x2 - x1) / line_count;
    y_line_space = (y2 - y1) / line_count;

    for (i = 0; i < 6; i++)
    {
        if (i == 0)
        {
            LCD_DrawLine(x1, y1, x2, y1, color);
            LCD_DrawLine(x1, y1, x1, y2, color);
            LCD_DrawLine(x1, y2, x2, y2, color);
            LCD_DrawLine(x2, y1, x2, y2, color);
        }
        else
        {
            LCD_DrawLine(x1 + i * x_line_space, y1, x1 + i * x_line_space, y2, color);
            LCD_DrawLine(x1, y1 + i * y_line_space, x2, y1 + i * y_line_space, color);
        }
    }

//    if (sFWS2_t.general_parameter.temp_unit == CELSIUS)
//    {
//        LCD_ShowString(x1 + 8, y1 + 2, (uint8_t *)"C", color, BLACK, 12, 0);
//        LCD_ShowString(x1 + 2, y1 + 12, (uint8_t *)"500", color, BLACK, 12, 0);
//        LCD_ShowString(x1 + 2, y1 + 12 * 3, (uint8_t *)"400", color, BLACK, 12, 0);
//        LCD_ShowString(x1 + 2, y1 + 12 * 5, (uint8_t *)"300", color, BLACK, 12, 0);
//        LCD_ShowString(x1 + 2, y1 + 12 * 7, (uint8_t *)"200", color, BLACK, 12, 0);
//        LCD_ShowString(x1 + 2, y1 + 12 * 9, (uint8_t *)"100", color, BLACK, 12, 0);
//    }
//    else
//    {
//        LCD_ShowString(x1 + 8, y1 + 2, (uint8_t *)"F", color, BLACK, 12, 0);
//        LCD_ShowString(x1 + 2, y1 + 12, (uint8_t *)"932", color, BLACK, 12, 0);
//        LCD_ShowString(x1 + 2, y1 + 12 * 3, (uint8_t *)"752", color, BLACK, 12, 0);
//        LCD_ShowString(x1 + 2, y1 + 12 * 5, (uint8_t *)"572", color, BLACK, 12, 0);
//        LCD_ShowString(x1 + 2, y1 + 12 * 7, (uint8_t *)"392", color, BLACK, 12, 0);
//        LCD_ShowString(x1 + 2, y1 + 12 * 9, (uint8_t *)"212", color, BLACK, 12, 0);
//    }
    LCD_ShowString(x1 + 318, y1 + 2, (uint8_t *)"%", color, BLACK, 12, 0);
    LCD_ShowString(x1 + 310, y1 + 2 + 20, (uint8_t *)"100", color, BLACK, 12, 0);
    LCD_ShowString(x1 + 310, y1 + 2 + 20 + 40, (uint8_t *)"75", color, BLACK, 12, 0);
    LCD_ShowString(x1 + 310, y1 + 2 + 20 + 40 + 40, (uint8_t *)"50", color, BLACK, 12, 0);
    LCD_ShowString(x1 + 310 + 8, y1 + 2 + 20 + 40 + 40 + 40, (uint8_t *)"25", color, BLACK, 12, 0);
    LCD_ShowString(x1 + 310 + 8 + 8, y1 + 2 + 20 + 40 + 40 + 40 + 40, (uint8_t *)"0", color, BLACK, 12, 0);
}

void LCD_Show_Curve(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, float s1_temp, float s1_power)
{
    int i;
    volatile static uint16_t temp_input_data = 0;
    volatile static uint16_t power_input_data = 0;

    /* get base temp data*/
    if (s1_temp >= 500)
    {
        s1_temp = 500;
    }

    if (s1_temp <= 100)
    {
        s1_temp = 100;
    }

    /* get base power data*/
    if (s1_power >= 100)
    {
        s1_power = 100;
    }

    if (s1_power <= 0)
    {
        s1_power = 0;
    }

    temp_input_data = voltage_to_y(s1_temp, 100, 500, y1 + 29, y2 - 1);
    power_input_data = voltage_to_y(s1_power, 0, 100, y1 + 29, y2 - 1);

    for (i = 0; i < (CURVE_BUFF_SIZE - 1); i++)
    {

        LCD_DrawLine(x1 + i, sFWS2_t.base.temp_buff[i], x1 + i, sFWS2_t.base.temp_buff[i + 1], BLACK);

        LCD_DrawLine(x1 + i, sFWS2_t.base.power_buff[i], x1 + i, sFWS2_t.base.power_buff[i + 1], BLACK);

        if (i < (CURVE_BUFF_SIZE - 2))
        {
			LCD_DrawLine(x1 + i, sFWS2_t.base.temp_buff[i + 1], x1 + i, sFWS2_t.base.temp_buff[i + 2], 0xe428);
			LCD_DrawLine(x1 + i, sFWS2_t.base.power_buff[i + 1], x1 + i, sFWS2_t.base.power_buff[i + 2], BLUE);
//			LCD_DrawLine(x1 + i, sFWS2_t.base.power_buff[i + 1], x1 + i, sFWS2_t.base.power_buff[i + 2], 0x0b1d);

        }

        sFWS2_t.base.temp_buff[i] = sFWS2_t.base.temp_buff[i + 1];
        sFWS2_t.base.power_buff[i] = sFWS2_t.base.power_buff[i + 1];
    }

    sFWS2_t.base.temp_buff[(CURVE_BUFF_SIZE - 1)] = temp_input_data;
    sFWS2_t.base.power_buff[(CURVE_BUFF_SIZE - 1)] = power_input_data;
    LCD_Show_Grid(x1, y1, x2, y2, 5,0x840f);
}
#endif

#endif
