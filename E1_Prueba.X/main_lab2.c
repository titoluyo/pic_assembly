#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic18f4550.h>
#include "config.h"
#include "i2c.h"
#include "lcd_i2c.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

Lcd_I2C lcd1 = { .address = 0x4E };
Lcd_I2C lcd2 = { .address = 0x4C };

void main(void) {
    ADCON1bits.PCFG = 0x0F; // config all pins as digital
    I2C_Init_Master(I2C_100KHZ);

    Lcd_Init(&lcd1);
    __delay_ms(100);
    Lcd_Clear(&lcd1);
    __delay_ms(100);
    
    Lcd_Init(&lcd2);
    __delay_ms(100);
    Lcd_Clear(&lcd2);
    __delay_ms(100);

    Lcd_Write_String(&lcd1, "Hola UTP I2C");

    Lcd_Write_String(&lcd2, "29-05-25");
    
    while(1) {
        
    }
    return;
}
