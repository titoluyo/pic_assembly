#include <xc.h>
#include "config.h"
#include "lcd.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

void main(void) {
    ADCON1bits.PCFG = 0x0F; // all digital pins
    TRISDbits.TRISD0 = 0;   // pin led as output
    TRISDbits.TRISD1 = 0;   // pin motor as output
    LATDbits.LATD0 = 0;     // iniital LED state: OFF
    LATDbits.LATD1 = 0;     // iniital MOTOR state: OFF

    // LCD
    __delay_ms(100);
    L1Lcd_Init();
    __delay_ms(100);
    L1Lcd_Write_String("Hello UTP");
    
    INTCONbits.GIE = 1; // enable all global interrupts
    // INTCONbits.GIEH = 1; // enable all global interrupts
    // INTCONbits.GIEL = 1; // enable all global interrupts
    // INTCONbits.PEIE = 1; // enable peripherals (only required priorities))
    // RCONbits.IPEN = 1;   // enable priorities
    
    // int0
    // INTCONbits.INT0E = 1; // enable int0
    INTCONbits.INT0IE = 1; // enable int0
    INTCONbits.INT0IF = 0;
    INTCON2bits.INTEDG0 = 0; // detect falling edge

    // int1
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT1IF = 0;
    INTCON2bits.INTEDG1 = 0; // detect falling edge
//    INTCON3bits.INT1E = 1; // enable int0
    //INTCON2bits.INTEDG1 = 0; // detect falling edge
    // INTCON3bits.INT1IP = 1; // set int0 low priority ()
    
    while(1) {
        __delay_ms(100);
    }
    return;
}

void __interrupt() ISR_INT(void) {
    if (INTCONbits.INT0F == 1) {
//        LATDbits.LATD0 = ~LATDbits.LATD0; // toggles LED state
//        LATDbits.LATD1 = ~LATDbits.LATD1; // toggles MOTOR state
        LATDbits.LATD0 = 1; // fires alarm
        LATDbits.LATD1 = 1; // fires alarm
        L1Lcd_Clear();
        __delay_ms(2);
        L1Lcd_Write_String("Motor ON");
        INTCONbits.INT0F = 0;
    }
    if (INTCON3bits.INT1F == 1) {
        LATDbits.LATD0 = 0; // shutdown alarm
        LATDbits.LATD1 = 0; // shutdown alarm
        L1Lcd_Clear();
        __delay_ms(2);
        L1Lcd_Write_String("Motor OFF");
        INTCON3bits.INT1F = 0;
    }
}
/*
void __interrupt(high_priority) ISR_INT0(void) {
    if (INTCONbits.INT0F == 1) {
        LATDbits.LATD0 = 1; // fires alarm
        INTCONbits.INT0F = 0;
    }
}

void __interrupt(low_priority) ISR_INT1(void) {
    if (INTCON3bits.INT1F == 1) {
        LATDbits.LATD0 = 0; // shutdown alarm
        INTCON3bits.INT1F = 0;
    }
}
*/