/* #include "lcd_i2c.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 05/12/2019
* Electronica y Circuitos
*
*/

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000  // 8MHz
#endif

#include <xc.h>
#include <stdint.h>
#include "i2c.h"
#include "lcd_i2c.h"

void Lcd_Init(Lcd_I2C* lcd) {
    __delay_ms(20);
    Lcd_Cmd(lcd, 0x33);
    Lcd_Cmd(lcd, 0x32);
    Lcd_Cmd(lcd, 0x28);
    Lcd_Cmd(lcd, 0x0C);
    Lcd_Cmd(lcd, 0x06);
    Lcd_Cmd(lcd, 0x01);
    __delay_ms(3);
}

void Lcd_Cmd(Lcd_I2C* lcd, unsigned char cmd)
{
    char data_u, data_l;
    data_u = (cmd & 0xF0);
    data_l = ((cmd << 4) & 0xF0);
    I2C_Start();
    I2C_Write(lcd->address);
    I2C_Write(data_u | 0x0C);
    I2C_Write(data_u | 0x08);
    I2C_Write(data_l | 0x0C);
    I2C_Write(data_l | 0x08);
    I2C_Stop();
}

void Lcd_Write_Char(Lcd_I2C* lcd, char c)
{
    char data_u, data_l;
    data_u = (c & 0xF0);
    data_l = ((c << 4) & 0xF0);
    I2C_Start();
    I2C_Write(lcd->address);
    I2C_Write(data_u | 0x0D);
    I2C_Write(data_u | 0x09);
    I2C_Write(data_l | 0x0D);
    I2C_Write(data_l | 0x09);
    I2C_Stop();
}

void Lcd_Set_Cursor(Lcd_I2C* lcd, char col, char row)
{
    unsigned char address;
    switch(row)
    {
        case 1:
            address = 0x00;
            break;
        case 2:
            address = 0x40;
            break;
        case 3:
            address = 0x14;
            break;
        case 4:
            address = 0x54;
            break;
        default:
            address = 0x00;
    }
    address += col - 1;
    Lcd_Cmd(lcd, 0x80 | address);
}

void Lcd_Write_String(Lcd_I2C* lcd, const char *str)
{
    while(*str != '\0')
    {
        Lcd_Write_Char(lcd, *str++);
    }
}

void Lcd_Clear(Lcd_I2C* lcd)
{
    Lcd_Cmd(lcd, 0x01);
    __delay_ms(2);
}

void Lcd_Shift_Right(Lcd_I2C* lcd)
{
    Lcd_Cmd(lcd, 0x1C);
}

void Lcd_Shift_Left(Lcd_I2C* lcd)
{
    Lcd_Cmd(lcd, 0x18);
}

void Lcd_Blink(Lcd_I2C* lcd)
{
    Lcd_Cmd(lcd, 0x0F);
}

void Lcd_NoBlink(Lcd_I2C* lcd)
{
    Lcd_Cmd(lcd, 0x0C);
}

void Lcd_CGRAM_WriteChar(Lcd_I2C* lcd, char n)
{
    Lcd_Write_Char(lcd, n);
}

void Lcd_CGRAM_CreateChar(Lcd_I2C* lcd, char pos, const char* new_char)
{
    if(pos < 8)
    {
        Lcd_Cmd(lcd, 0x40 + (pos * 8));
        for(char i = 0; i < 8; i++)
        {
            Lcd_Write_Char(lcd, new_char[i]);
        }
    }
}