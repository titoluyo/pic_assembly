#include "lcdp.h"

void Lcd_Cmd(LcdHandle* lcd, char a) {
    lcd->set_rs(0);
    lcd->set_data(a >> 4);
    lcd->set_en(1); __delay_ms(2); lcd->set_en(0);
    lcd->set_data(a & 0x0F);
    lcd->set_en(1); __delay_ms(2); lcd->set_en(0);
}

void Lcd_Clear(LcdHandle* lcd) {
    Lcd_Cmd(lcd, 0x01);
    __delay_ms(2);
}

void Lcd_Set_Cursor(LcdHandle* lcd, char row, char col) {
    char pos;
    switch(row) {
        case 1: pos = 0x80 + col - 1; break;
        case 2: pos = 0xC0 + col - 1; break;
        default: pos = 0x80;
    }
    Lcd_Cmd(lcd, pos);
}

void Lcd_Init(LcdHandle* lcd) {
    lcd->set_rw(0);
    __delay_ms(20);
    Lcd_Cmd(lcd, 0x03); __delay_ms(5);
    Lcd_Cmd(lcd, 0x03); __delay_ms(11);
    Lcd_Cmd(lcd, 0x03);
    Lcd_Cmd(lcd, 0x02);
    Lcd_Cmd(lcd, 0x28);
    Lcd_Cmd(lcd, 0x0C);
    Lcd_Cmd(lcd, 0x06);
    Lcd_Clear(lcd);
}

void Lcd_Write_Char(LcdHandle* lcd, char a) {
    lcd->set_rs(1);
    lcd->set_data(a >> 4);
    lcd->set_en(1); __delay_us(40); lcd->set_en(0);
    lcd->set_data(a & 0x0F);
    lcd->set_en(1); __delay_us(40); lcd->set_en(0);
}

void Lcd_Write_String(LcdHandle* lcd, const char *a) {
    while(*a) {
        Lcd_Write_Char(lcd, *a++);
    }
}

void Lcd_Shift_Right(LcdHandle* lcd) {
    Lcd_Cmd(lcd, 0x1C);
}

void Lcd_Shift_Left(LcdHandle* lcd) {
    Lcd_Cmd(lcd, 0x18);
}

void Lcd_Blink(LcdHandle* lcd) {
    Lcd_Cmd(lcd, 0x0F);
}

void Lcd_NoBlink(LcdHandle* lcd) {
    Lcd_Cmd(lcd, 0x0C);
}
