#ifndef LCD8_H
#define	LCD8_H

#include <xc.h> // include processor files - each processor file is guarded.  

// CONFIGURATION BITS (adjust as needed for your project)
#pragma config FOSC = HS        // Oscillator Selection bits
#pragma config WDT = OFF      // Watchdog Timer Enable bits
#pragma config LVP = OFF        // Low-voltage programming disable

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000  // 8 MHz crystal frequency
#endif

// LCD control pins (adjust as needed)
#define RS LATBbits.LB5
#define RW LATBbits.LB4
#define EN LATBbits.LB3
#define LCD_DATA LATD          // Assuming LCD data pins on PORTD

void lcd_pulse_enable(void);
void lcd_cmd(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_init(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_write_string(const char* str, uint8_t row);

#endif	/* LCD8_H */

