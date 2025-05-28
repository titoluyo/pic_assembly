#ifndef LCD_H
#define LCD_H

#include <xc.h> // include processor files - each processor file is guarded.  

// CONFIGURATION BITS (adjust as needed for your project)
#pragma config FOSC = HS        // Oscillator Selection bits
#pragma config WDT = OFF      // Watchdog Timer Enable bits
#pragma config LVP = OFF        // Low-voltage programming disable

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000  // 8 MHz crystal frequency
#endif

#define LCD_RS LATBbits.LATB5
#define LCD_RW LATBbits.LATB4
#define LCD_EN LATBbits.LATB3

// Data lines D4-D7
#define LCD_D4 LATDbits.LATD4
#define LCD_D5 LATDbits.LATD5
#define LCD_D6 LATDbits.LATD6
#define LCD_D7 LATDbits.LATD7

#define LCD_TRISD4 TRISDbits.TRISD4
#define LCD_TRISD5 TRISDbits.TRISD5
#define LCD_TRISD6 TRISDbits.TRISD6
#define LCD_TRISD7 TRISDbits.TRISD7

void lcd_init(void);
void lcd_cmd(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_write_string(const char* str, uint8_t row);

#endif