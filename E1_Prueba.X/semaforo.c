#include <xc.h>
#include <pic18f4550.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

void SemaforoInit (void) {
    ADCON1bits.PCFG = 0xF;
    TRISCbits.TRISC0 = 0; // RA0 salida ROJO
    TRISCbits.TRISC1 = 0; // RA1 salida AMBAR
    TRISCbits.TRISC2 = 0; // RA2 salida VERDE
}

void SemaforoLoop (void) {
    LATCbits.LATC0 = 1; // encendido ROJO
    LATCbits.LATC1 = 0;
    LATCbits.LATC2 = 0;
    __delay_ms(2000); // wait 3 sec
    LATCbits.LATC0 = 0;
    LATCbits.LATC1 = 0;
    LATCbits.LATC2 = 1; // encendido VERDE
    __delay_ms(1000); // wait 2 sec
    LATCbits.LATC0 = 0;
    LATCbits.LATC1 = 1; // encendido AMBAR
    LATCbits.LATC2 = 0;        
    __delay_ms(500); // wait 2 sec
}

void InterruptorInit(void) {
    TRISBbits.TRISB0 = 1; // boton entrada
    LATCbits.LATC0 = 0;
    LATCbits.LATC1 = 0;
    LATCbits.LATC2 = 0;
}
void InterruptorLoop(void) {
    if (PORTBbits.RB0 == 0) {
        __delay_ms(500);
        LATC = ~LATC;
    }
}