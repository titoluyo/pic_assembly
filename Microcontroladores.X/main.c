#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lcd.h"

void main(void) {


    lcd_init();

    lcd_write_string("HELLO", 0);
    lcd_write_string("WORLD12", 1);

    while (1);
}