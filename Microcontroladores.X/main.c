#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// CONFIGURATION BITS (adjust as needed for your project)
#pragma config FOSC = HS        // Oscillator Selection bits
#pragma config WDT = OFF      // Watchdog Timer Enable bits
#pragma config LVP = OFF        // Low-voltage programming disable

#define _XTAL_FREQ 8000000  // 8 MHz crystal frequency

// LCD control pins (adjust as needed)
#define RS LATBbits.LB5
#define RW LATBbits.LB4
#define EN LATBbits.LB3
#define LCD_DATA LATD          // Assuming LCD data pins on PORTD

void lcd_pulse_enable(void) {
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_us(100);
}

void lcd_cmd(uint8_t cmd) {
    RS = 0;
    RW = 0; // Force write
    LCD_DATA = cmd;
    lcd_pulse_enable();
    __delay_ms(2); // Some commands require longer delays
}

void lcd_data(uint8_t data) {
    RS = 1;
    RW = 0; // Force write
    LCD_DATA = data;
    lcd_pulse_enable();
    __delay_us(100);
}

void lcd_init(void) {
    __delay_ms(20);         // Wait after power on
    RW = 0;
    __delay_ms(20);
    lcd_cmd(0x38);          // Function set: 8-bit, 2 lines, 5x8 dots
    lcd_cmd(0x08);          // Display off
    lcd_cmd(0x01);          // Display clear
    __delay_ms(2);
    lcd_cmd(0x06);          // Entry mode: increment, no shift
    lcd_cmd(0x0C);          // Display on, cursor off
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? col : (0x40 + col);
    lcd_cmd(0x80 | address);
}

void lcd_write_string(const char* str, uint8_t row) {
    lcd_set_cursor(row, 0);
    while (*str) {
        lcd_data(*str++);
    }
}

void main(void) {
    // Setup ports (assume PORTB for control, PORTD for data)
    TRISB = 0x00;
    LATB = 0x00;
    TRISD = 0x00;
    LATD = 0x00;

    lcd_init();

    lcd_write_string("HELLO", 0);
    lcd_write_string("WORLD5", 1);

    while (1);
}