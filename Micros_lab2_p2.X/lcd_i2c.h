/* #include "lcd_i2c.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 05/12/2019
* Electronica y Circuitos
*
*/

#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>

typedef struct {
    uint8_t address;
} Lcd_I2C;

void Lcd_Init(Lcd_I2C* lcd);
void Lcd_Cmd(Lcd_I2C* lcd, unsigned char cmd);
void Lcd_Set_Cursor(Lcd_I2C* lcd, char col, char row);
void Lcd_Write_Char(Lcd_I2C* lcd, char c);
void Lcd_Write_String(Lcd_I2C* lcd, const char *str);
void Lcd_Clear(Lcd_I2C* lcd);
void Lcd_Shift_Right(Lcd_I2C* lcd);
void Lcd_Shift_Left(Lcd_I2C* lcd);
void Lcd_Blink(Lcd_I2C* lcd);
void Lcd_NoBlink(Lcd_I2C* lcd);
void Lcd_CGRAM_WriteChar(Lcd_I2C* lcd, char n);
void Lcd_CGRAM_CreateChar(Lcd_I2C* lcd, char pos, const char* new_char);

#endif