#ifndef _XTAL_FREQ
#define _XTAL_FREQ 48000000 // Frecuencia del oscilador
#endif
#include <xc.h>
#include <stdint.h>
#include "i2c.h"
#include "lcd_i2c.h"

// Inicializa el LCD en modo 4 bits, sin cursor, sin parpadeo, con auto desplazamiento
void Lcd_Init(void)
{
    __delay_ms(20);          // Espera para estabilizar
    Lcd_Cmd(0x33);           // Secuencia de inicializacion
    Lcd_Cmd(0x32);           // Establece modo 4 bits
    Lcd_Cmd(0x28);           // LCD 2 lineas, 5x8 puntos
    Lcd_Cmd(0x0C);           // Display encendido, cursor apagado
    Lcd_Cmd(0x06);           // Cursor incrementa a la derecha
    Lcd_Cmd(0x01);           // Limpia pantalla
    __delay_ms(3);           // Espera para completar borrado
}

// Envia un comando al LCD
void Lcd_Cmd(unsigned char cmd)
{
    char data_u, data_l;
    data_u = (cmd & 0xF0);           // Parte alta del comando
    data_l = ((cmd << 4) & 0xF0);    // Parte baja del comando
    I2C_Start();                     // Inicia comunicacion I2C
    I2C_Write(ADDRESS_LCD);          // Envia direccion del LCD
    I2C_Write(data_u | 0x0C);        // Envia nibble alto con habilitacion
    I2C_Write(data_u | 0x08);        // Desactiva enable
    I2C_Write(data_l | 0x0C);        // Envia nibble bajo con habilitacion
    I2C_Write(data_l | 0x08);        // Desactiva enable
    I2C_Stop();                      // Termina comunicacion I2C
}

// Escribe un caracter en el LCD
void Lcd_Write_Char(char c)
{
    char data_u, data_l;
    data_u = (c & 0xF0);            // Parte alta del caracter
    data_l = ((c << 4) & 0xF0);     // Parte baja del caracter
    I2C_Start();                    // Inicia comunicacion I2C
    I2C_Write(ADDRESS_LCD);         // Direccion del LCD
    I2C_Write(data_u | 0x0D);       // Envia nibble alto con RS=1, E=1
    I2C_Write(data_u | 0x09);       // Envia nibble alto con E=0
    I2C_Write(data_l | 0x0D);       // Envia nibble bajo con RS=1, E=1
    I2C_Write(data_l | 0x09);       // Envia nibble bajo con E=0
    I2C_Stop();                     // Termina comunicacion
}

// Posiciona el cursor en la columna y fila indicadas
void Lcd_Set_Cursor(char col, char row)
{
    unsigned char address;
    switch(row)
    {
        case 1: address = 0x00; break;
        case 2: address = 0x40; break;
        case 3: address = 0x14; break;
        case 4: address = 0x54; break;
    }
    address += col - 1;             // Ajusta direccion segun columna
    Lcd_Cmd(0x80 | address);        // Envia comando de posicion
}

// Escribe una cadena de texto en el LCD
void Lcd_Write_String(const char *str)
{
    while(*str != '\0')             // Recorre hasta fin de cadena
    {
        Lcd_Write_Char(*str++);     // Escribe caracter por caracter
    }
}

// Limpia la pantalla del LCD
void Lcd_Clear(void)
{
    Lcd_Cmd(0x01);                  // Comando de limpieza
    __delay_ms(2);                  // Espera de seguridad
}

// Desplaza todo el contenido a la derecha
void Lcd_Shift_Right(void)
{
    Lcd_Cmd(0x1C);
}

// Desplaza todo el contenido a la izquierda
void Lcd_Shift_Left(void)
{
    Lcd_Cmd(0x18);
}

// Activa parpadeo del cursor
void Lcd_Blink(void)
{
    Lcd_Cmd(0x0F);
}

// Desactiva parpadeo del cursor
void Lcd_NoBlink(void)
{
    Lcd_Cmd(0x0C);
}

// Escribe un caracter previamente cargado en CGRAM
void Lcd_CGRAM_WriteChar(char n)
{
    Lcd_Write_Char(n);
}

// Crea un nuevo caracter en CGRAM y lo almacena en posicion indicada (0 a 7)
void Lcd_CGRAM_CreateChar(char pos, const char* new_char)
{
    if(pos < 8)                                // Solo 8 posiciones disponibles en CGRAM
    {
        Lcd_Cmd(0x40 + (pos * 8));             // Establece direccion base en CGRAM
        for(char i = 0; i < 8; i++)            // Escribe 8 bytes del patron
        {
            Lcd_Write_Char(new_char[i]);
        }
    }
}
