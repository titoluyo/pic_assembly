#ifndef LCDP_H
#define LCDP_H

#include <xc.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

typedef struct {
    void (*set_rs)(char val);
    void (*set_rw)(char val);
    void (*set_en)(char val);
    void (*set_data)(char nibble);
} LcdHandle;

void Lcd_Init(LcdHandle* lcd);
void Lcd_Cmd(LcdHandle* lcd, char a);
void Lcd_Clear(LcdHandle* lcd);
void Lcd_Set_Cursor(LcdHandle* lcd, char a, char b);
void Lcd_Write_Char(LcdHandle* lcd, char a);
void Lcd_Write_String(LcdHandle* lcd, const char *a);
void Lcd_Shift_Right(LcdHandle* lcd);
void Lcd_Shift_Left(LcdHandle* lcd);
void Lcd_Blink(LcdHandle* lcd);
void Lcd_NoBlink(LcdHandle* lcd);

#endif
