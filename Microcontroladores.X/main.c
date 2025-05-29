#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lcd.h"

void main(void) {


    lcd_init();

    lcd_write_string("HELLO", 0);
    lcd_write_string("WORLD 16", 1);
    __delay_ms(2000);

    lcd_write_string("Test LCD Func", 0);
    lcd_set_cursor(1, 0);
    lcd_write_string("Cursor Blink", 1);
    __delay_ms(2000);

    // Blink ON
    lcd_blink();
    __delay_ms(2000);

    // Blink OFF
    lcd_no_blink();
    __delay_ms(2000);

    // Shift display RIGHT
    lcd_clear();
    lcd_write_string("Shift RIGHT", 0);
    lcd_shift_right();  // Display ON
    __delay_ms(2000);

    // Shift display LEFT
    lcd_clear();
    lcd_write_string("Shift LEFT", 0);
    lcd_shift_left();   // Display OFF (you won't see anything)
    __delay_ms(2000);

    // Bring back display and write again
    lcd_no_blink();
    lcd_clear();
    lcd_write_string("12345", 0);
    lcd_set_cursor(2, 2);
    lcd_putnum(6789);
    __delay_ms(3000);

    // Display final formatted string
    lcd_clear();
    lcd_printf("Temp: %02dC", 24);
    lcd_set_cursor(2, 2);
    lcd_printf("Volts: %.1f", 3.3);  // if using float support
    while (1);
}