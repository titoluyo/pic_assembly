#include "lcd.h"
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void lcd_write_nibble(uint8_t nibble) {
    LCD_D4 = (nibble >> 0) & 0x01;
    LCD_D5 = (nibble >> 1) & 0x01;
    LCD_D6 = (nibble >> 2) & 0x01;
    LCD_D7 = (nibble >> 3) & 0x01;

    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    __delay_us(50);
}

void lcd_send_byte(uint8_t byte, uint8_t isData) {
    LCD_RS = isData;
    LCD_RW = 0;

    lcd_write_nibble(byte >> 4);
    lcd_write_nibble(byte & 0x0F);

    if (!isData)
        __delay_ms(2);
    else
        __delay_us(50);
}

void lcd_cmd(uint8_t cmd) {
    lcd_send_byte(cmd, 0);
}

void lcd_data(uint8_t data) {
    lcd_send_byte(data, 1);
}

void lcd_init(void) {
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;

    __delay_ms(20);

    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;

    LCD_TRISD4 = 0;
    LCD_TRISD5 = 0;
    LCD_TRISD6 = 0;
    LCD_TRISD7 = 0;

    lcd_write_nibble(0x03);
    __delay_ms(5);
    lcd_write_nibble(0x03);
    __delay_us(100);
    lcd_write_nibble(0x03);
    __delay_us(100);
    lcd_write_nibble(0x02);

    lcd_cmd(0x28);
    lcd_cmd(0x08);
    lcd_cmd(0x01);
    __delay_ms(2);
    lcd_cmd(0x06);
    lcd_cmd(0x0C);
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

void lcd_clear(void) {
    lcd_cmd(0x01);
    __delay_ms(2);
}

void lcd_home(void) {
    lcd_cmd(0x02);
    __delay_ms(2);
}

void lcd_putch(char c) {
    lcd_data(c);
}

void lcd_putnum(uint16_t num) {
    char buffer[6];
    sprintf(buffer, "%u", num);
    lcd_write_string(buffer, 0); // Writes on row 0 by default
}

// Simple printf-style interface for static strings
void lcd_printf(const char* format, ...) {
    char buffer[32];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    lcd_write_string(buffer, 0); // Writes on row 0 by default
}

void lcd_shift_right(void) {
    lcd_cmd(0x01); // Clear display
    lcd_cmd(0x0C); // Display ON, cursor OFF
}

void lcd_shift_left(void) {
    lcd_cmd(0x01); // Clear display
    lcd_cmd(0x08); // Display OFF
}

void lcd_blink(void) {
    lcd_cmd(0x0F); // Display ON, cursor ON, blinking ON
}

void lcd_no_blink(void) {
    lcd_cmd(0x0C); // Display ON, cursor OFF, blinking OFF
}
