#include <xc.h>
#include <pic18f4550.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

void Led7segInit(void) {
    ADCON1bits.PCFG = 0xF;
    TRISD = 0;
    LATD = 0; // all D off
    
    TRISBbits.TRISB0 = 1; // btn ASC
    TRISBbits.TRISB1 = 1; // DESC
    
}

//    LATD = 0b01111110;
//    LATD = 0b01100000;

const unsigned char LED[10]=
{
0x3f,     //0
0x06,     //1
0x5b,	  //2
0x4f,	  //3
0x66,	  //4
0x6d,	  //5
0x7d,	  //6
0x07,	  //7
0x7f,	  //8
0x6f,	  //9
};

void Led7segLoop(void) {
    if (PORTBbits.RB0 == 1) {
        LATD = 0B11000000;
        __delay_ms(1000);
        LATD = 0;
    }
//    if (PORTBbits.RB0 == 1) {
//        for(int i=0; i<10; i++) {
//            LATD = LED[i];
//            __delay_ms(1000);
//        }
//        LATD = 0;
//    }
    if (PORTBbits.RB1 == 1) {
        for(int i=9; i>=0; i--) {
            LATD = LED[i];
            __delay_ms(1000);
        }
        LATD = 0;
    }
}