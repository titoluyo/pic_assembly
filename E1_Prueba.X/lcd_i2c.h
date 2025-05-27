/* #include "lcd_i2c.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 05/12/2019
* Electronica y Circuitos
*
*/

#ifndef LCD_I2C_H
#define	LCD_I2C_H

#define ADDRESS_LCD 0x4E //0x27 

void Lcd_Init(void);
void Lcd_Cmd(unsigned char cmd);
void Lcd_Set_Cursor(char col, char row);
void Lcd_Write_Char(char c);
void Lcd_Write_String(const char *str);
void Lcd_Clear(void);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);
void Lcd_Blink(void);
void Lcd_NoBlink(void);
void Lcd_CGRAM_WriteChar(char n);
void Lcd_CGRAM_CreateChar(char pos, const char* new_char);

#endif