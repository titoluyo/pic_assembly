#ifndef LCD_I2C_H
#define	LCD_I2C_H

#define ADDRESS_LCD 0x4E // Direccion I2C del modulo LCD con backpack PCF8574

void Lcd_Init(void);                     // Inicializa el LCD
void Lcd_Cmd(unsigned char cmd);         // Envia un comando al LCD (ej. limpiar, cursor, etc)
void Lcd_Set_Cursor(char col, char row); // Posiciona el cursor en la columna y fila especificadas
void Lcd_Write_Char(char c);             // Escribe un caracter en la posicion actual del cursor
void Lcd_Write_String(const char *str);  // Escribe una cadena de texto desde un puntero
void Lcd_Clear(void);                    // Limpia la pantalla
void Lcd_Shift_Right(void);              // Desplaza el texto hacia la derecha
void Lcd_Shift_Left(void);               // Desplaza el texto hacia la izquierda
void Lcd_Blink(void);                    // Activa el parpadeo del cursor
void Lcd_NoBlink(void);                  // Desactiva el parpadeo del cursor
void Lcd_CGRAM_WriteChar(char n);        // Escribe un caracter personalizado desde CGRAM
void Lcd_CGRAM_CreateChar(char pos, const char* new_char); // Crea un caracter personalizado en la posicion dada de CGRAM

#endif
