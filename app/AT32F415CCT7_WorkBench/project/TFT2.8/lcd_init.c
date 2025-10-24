#include "wk_system.h"
#include "bsp_lcd_spi.h"
#include "lcd_init.h"

/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Write_Bus(uint8_t dat)
{
    LCD_CS_Clr();
    spi_write_read_byte(dat);
    LCD_CS_Set();
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
    LCD_Write_Bus(dat);
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
    LCD_DC_Set(); // д����
    // LCD_CS_Clr();
    LCD_Write_Bus(dat >> 8);
    LCD_Write_Bus(dat);
    // LCD_CS_Set();
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
    LCD_DC_Clr(); // д����
    // LCD_CS_Clr();
    LCD_Write_Bus(dat);
    // LCD_CS_Set();
    LCD_DC_Set(); // д����
}

/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (USE_HORIZONTAL == 0)
    {
        LCD_WR_REG(0x2a); // �е�ַ����
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b); // �е�ַ����
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c); // ������д
    }
    else if (USE_HORIZONTAL == 1)
    {
        LCD_WR_REG(0x2a); // �е�ַ����
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b); // �е�ַ����
        LCD_WR_DATA(y1 + 80);
        LCD_WR_DATA(y2 + 80);
        LCD_WR_REG(0x2c); // ������д
    }
    else if (USE_HORIZONTAL == 2)
    {
        LCD_WR_REG(0x2a); // �е�ַ����
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b); // �е�ַ����
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c); // ������д
    }
    else
    {
        LCD_WR_REG(0x2a); // �е�ַ����
        LCD_WR_DATA(x1 + 80);
        LCD_WR_DATA(x2 + 80);
        LCD_WR_REG(0x2b); // �е�ַ����
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c); // ������д
    }
}

void LCD_Init(void)
{

    LCD_RES_Clr();
    wk_delay_ms(200);
    LCD_RES_Set();
    wk_delay_ms(120);
    LCD_WR_REG(0x11);
    wk_delay_ms(120); // Delay 120ms
    LCD_WR_REG(0x36);
    LCD_WR_REG(0x36);

    //    if (0 == USE_HORIZONTAL)
    //    {
    //        LCD_WR_DATA8(0x00);
    //    }
    //    else if (1 == USE_HORIZONTAL)
    //    {
    //        LCD_WR_DATA8(0x60);
    //    }
    //    else if (2 == USE_HORIZONTAL)
    //    {
    //        LCD_WR_DATA8(0xA0);
    //    }
    //    else if (3 == USE_HORIZONTAL)
    //        ;

    if (USE_HORIZONTAL == 0)
    {
        LCD_WR_DATA8(0x00);
    }
    else if (USE_HORIZONTAL == 1)
    {
        LCD_WR_DATA8(0xC0);
    }
    else if (USE_HORIZONTAL == 2)
    {
        LCD_WR_DATA8(0x70);
    }
    else
    {
        LCD_WR_DATA8(0xA0);
    }

    LCD_WR_REG(0x3a);
    LCD_WR_DATA8(0x05);
    LCD_WR_REG(0xb2);
    LCD_WR_DATA8(0x0c);
    LCD_WR_DATA8(0x0c);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x33);
    LCD_WR_REG(0xb7);
    LCD_WR_DATA8(0x35);
    LCD_WR_REG(0xbb);
    LCD_WR_DATA8(0x19);
    LCD_WR_REG(0xc0);
    LCD_WR_DATA8(0x2c);
    LCD_WR_REG(0xc2);
    LCD_WR_DATA8(0x01);
    LCD_WR_REG(0xc3);
    LCD_WR_DATA8(0x0d); // GVDD=4.2V
    LCD_WR_REG(0xc4);
    LCD_WR_DATA8(0x20);
    LCD_WR_REG(0xc6);
    LCD_WR_DATA8(0x0f); // Frame=60Hz
    LCD_WR_REG(0xd0);
    LCD_WR_DATA8(0xa4);
    LCD_WR_DATA8(0xa1);
    LCD_WR_REG(0xe0);
    LCD_WR_DATA8(0xd0);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x0f);
    LCD_WR_DATA8(0x10);
    LCD_WR_DATA8(0x28);
    LCD_WR_DATA8(0x34);
    LCD_WR_DATA8(0x50);
    LCD_WR_DATA8(0x44);
    LCD_WR_DATA8(0x3a);
    LCD_WR_DATA8(0x0b);
    LCD_WR_DATA8(0x06);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x20);
    LCD_WR_REG(0xe1);
    LCD_WR_DATA8(0xd0);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x0a);
    LCD_WR_DATA8(0x0b);
    LCD_WR_DATA8(0x16);
    LCD_WR_DATA8(0x32);
    LCD_WR_DATA8(0x40);
    LCD_WR_DATA8(0x4a);
    LCD_WR_DATA8(0x2b);
    LCD_WR_DATA8(0x1b);
    LCD_WR_DATA8(0x1c);
    LCD_WR_DATA8(0x22);
    LCD_WR_DATA8(0x1f);
//    LCD_WR_REG(0x21);
    LCD_WR_REG(0x29);

    
}
