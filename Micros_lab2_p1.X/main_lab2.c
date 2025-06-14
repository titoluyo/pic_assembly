#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic18f4550.h>
#include "config.h"
#include "lcd.h"
#include "lcd2.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

void main(void) {
    __delay_ms(100);
    L1Lcd_Init();
    __delay_ms(100);
    L1Lcd_Write_String("Hello UTP");
    
    __delay_ms(100);
    L2Lcd_Init();
    __delay_ms(100);
    L2Lcd_Write_String("29-05-25");
    
    while(1) {
        
    }
    return;
}
