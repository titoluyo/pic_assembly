
#include <xc.h>
#include "config.h"

#define _XTAL_FREQ 8000000

void main(void) {
    ADCON1bits.PCFG = 0x0F;       // Todos los pines como digitales
    TRISCbits.TRISC2 = 0;         // RC2 como salida (PWM CCP1)

    PR2 = 124;                    // Para 1 kHz con prescaler 16
    TMR2 = 0;                     // Reiniciar Timer2
    CCPR1L = 31;                  // ~50% duty cycle

    CCP1CONbits.DC1B1 = 0;        // Bits menos significativos del duty
    CCP1CONbits.DC1B0 = 0;

    CCP1CONbits.CCP1M3 = 1;       // PWM mode: 1100
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M1 = 0;
    CCP1CONbits.CCP1M0 = 0;

    T2CONbits.T2CKPS1 = 1;        // Prescaler = 16
    T2CONbits.T2CKPS0 = 1;

    T2CONbits.TMR2ON = 1;         // ENCENDER TIMER2

    while (1){
    }
    return;
}
