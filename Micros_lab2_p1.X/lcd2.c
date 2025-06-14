#include "lcd2.h"

void L2Lcd_Port(char a)
{
    (a & 1) ? (L2D4 = 1) : (L2D4 = 0);
    (a & 2) ? (L2D5 = 1) : (L2D5 = 0);
    (a & 4) ? (L2D6 = 1) : (L2D6 = 0);
    (a & 8) ? (L2D7 = 1) : (L2D7 = 0);
}

void L2Lcd_Cmd(char a)
{
    L2RS = 0;
    L2Lcd_Port(a);
    L2EN = 1;
    __delay_ms(4);
    L2EN = 0;
}

void L2Lcd_Clear(void)
{
    L2Lcd_Cmd(0);
    L2Lcd_Cmd(1);
}

void L2Lcd_Set_Cursor(char a, char b)
{
    char temp,z,y;
    if(a == 1)
    {
        temp = 0x80 + b - 1;
        z = temp>>4;
        y = temp & 0x0F;
        L2Lcd_Cmd(z);
        L2Lcd_Cmd(y);
    }
    else if(a == 2)
    {
        temp = 0xC0 + b - 1;
        z = temp>>4;
        y = temp & 0x0F;
        L2Lcd_Cmd(z);
        L2Lcd_Cmd(y);
    }
    else if(a == 3)
    {
        temp = 0x94 + b - 1;
        z = temp>>4;
        y = temp & 0x0F;
        L2Lcd_Cmd(z);
        L2Lcd_Cmd(y);
    }
    else if(a == 4)
    {
        temp = 0xD4 + b - 1;
        z = temp>>4;
        y = temp & 0x0F;
        L2Lcd_Cmd(z);
        L2Lcd_Cmd(y);
    }
}

void L2Lcd_Init(void)
{
	L2RS_DIR = 0;
    L2RW_DIR = 0;
	L2EN_DIR = 0;
	L2D4_DIR = 0;
	L2D5_DIR = 0;
	L2D6_DIR = 0;
	L2D7_DIR = 0;
    
    L2RW = 0;
    
    L2Lcd_Port(0x00);
    __delay_ms(20);
    L2Lcd_Cmd(0x03);
    __delay_ms(5);
    L2Lcd_Cmd(0x03);
    __delay_ms(11);
    L2Lcd_Cmd(0x03);
    L2Lcd_Cmd(0x02);
    L2Lcd_Cmd(0x02);
    L2Lcd_Cmd(0x08);
    L2Lcd_Cmd(0x00);
    L2Lcd_Cmd(0x0C);
    L2Lcd_Cmd(0x00);
    L2Lcd_Cmd(0x06);
    L2Lcd_Clear();
}

void L2Lcd_Write_Char(char a)
{
    char temp,y;
    temp = a&0x0F;
    y = a&0xF0;
    L2RS = 1;
    L2Lcd_Port(y>>4);
    L2EN = 1;
    __delay_us(40);
    L2EN = 0;
    L2Lcd_Port(temp);
    L2EN = 1;
    __delay_us(40);
    L2EN = 0;
}

void L2Lcd_Write_String(const char *a)
{
    int i;
    for(i=0;a[i]!='\0';i++)
        L2Lcd_Write_Char(a[i]);
}

void L2Lcd_Shift_Right(void)
{
    L2Lcd_Cmd(0x01);
    L2Lcd_Cmd(0x0C);
}

void L2Lcd_Shift_Left(void)
{
    L2Lcd_Cmd(0x01);
    L2Lcd_Cmd(0x08);
}

void L2Lcd_Blink(void)
{
    L2Lcd_Cmd(0x00);
    L2Lcd_Cmd(0x0F);
}

void L2Lcd_NoBlink(void)
{
    L2Lcd_Cmd(0x00);
    L2Lcd_Cmd(0x0C);
}

#ifdef USE_CGRAM_LCD
void L2Lcd_CGRAM_CreateChar(char add, const char* chardata)
{
    switch(add)
    {
        case 0:
            for(char i=0; i<=7; i++)
                L2Lcd_Write_Char(chardata[i]);
            break;
        case 1:
            for(char i=8; i<=15; i++)
                L2Lcd_Write_Char(chardata[i-8]);
            break;
        case 2:
            for(char i=16; i<=23; i++)
                L2Lcd_Write_Char(chardata[i-16]);
            break;
        case 3:
            for(char i=24; i<=31; i++)
                L2Lcd_Write_Char(chardata[i-24]);
            break;
        case 4:
            for(char i=32; i<=39; i++)
                L2Lcd_Write_Char(chardata[i-32]);
            break;
        case 5:
            for(char i=40; i<=47; i++)
                L2Lcd_Write_Char(chardata[i-40]);
            break;
        case 6:
            for(char i=48; i<=55; i++)
                L2Lcd_Write_Char(chardata[i-48]);
            break;
        case 7:
            for(char i=56; i<=63; i++)
                L2Lcd_Write_Char(chardata[i-56]);
            break;
    }
}

void L2Lcd_CGRAM_Init(void)
{
    L2Lcd_Cmd(0x04);
    L2Lcd_Cmd(0x00);
}

void L2Lcd_CGRAM_Close(void)
{
    L2Lcd_Cmd(0x00);
    L2Lcd_Cmd(0x02);
}
#endif