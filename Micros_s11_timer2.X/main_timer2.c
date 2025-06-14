#include <xc.h>
#include "config.h"

/*
HSPLL_HS - FrecuenciaIn x 6 => 8MHz x 6 = 48Mhz

f =  1Mhz => T = 1 us
f =  8Mhz => T = 0.125 us
f = 48Mhz => T = 20.83 ns

OJO
si XT = 4Mhz => Fosc = XT / 4 = 1Mhz

Entonces config
FOSC HSPLL_HC
WDT OFF


Timer1 16bits => 65536
TMR1 = 65536 - (Tiempo_solicitado * Fosc)/(4 * Pre_escalador)

si quiero 20ms = 20x10^-3
si config timer1 0xF8 
TMR1 = 65536 - ( 20x10^(-3) * 48x10^(6) ) / ( 4 * 8 ) 
 */


#define _XTAL_FREQ 8000000

int c = 0;
int c2 = 0;

void __interrupt() isr() {
    if (TMR1IF == 1) {  // si la bandera esta levantada
        c++;
        if (c >= 25) {
            c = 0;
            PORTDbits.RD0 = ~PORTDbits.RD0; // hacer parpadear led y nuevamente contar
        }
        TMR1 = 35536;
        TMR1IF = 0;
    }
    if (TMR0IF == 1) {  // si la bandera esta levantada
//        c2++;
//        if (c2 >= 25) {
//            c2 = 0;
            PORTDbits.RD1 = ~PORTDbits.RD1; // hacer parpadear led y nuevamente contar
//        }
        TMR0 = 57724;
        TMR0IF = 0;
    }
}

void main(void) {
    ADCON1 = 0x0F; // All digital pins

    INTCONbits.GIE = 0;
    
    TRISDbits.TRISD0 = 0;  // RD0 como salida
    T1CON = 0xF8; // configuro mi timer1 inicialmente con prescalador de 1:8
    PIE1bits.TMR1IE = 1; // enable interrupt
    PIR1bits.TMR1IF = 0; // disable flag
    INTCONbits.PEIE = 1; // enable device interrupts
    // INTCONbits.GIE = 1;  // enable global interrupts
    TMR1 = 35536;        // load count value for interrupt
    LATDbits.LATD1 = 1; // 
    T1CONbits.TMR1ON = 1; // enable timer

    TRISDbits.TRISD1 = 0;  // RD1 como salida
    T0CONbits.T0CS = 0; // internal Clock source
    T0CONbits.PSA = 0; // enables pre-scaler
    T0CONbits.T0PS = 0b111; // 256 pre-scaler
    T0CONbits.T08BIT = 0;   // 16 bits de cuenta
    T0CONbits.TMR0ON = 1;   // Tmr0 starts
    INTCONbits.TMR0IF = 0;  // Timer 0 flag off
    INTCONbits.TMR0IE = 1;  // Allow 
    INTCONbits.GIE = 1;  // enable global interrupts
    TMR0 = 57724;
    
    TRISDbits.TRISD2 = 0;  // RD2 como salida
    
    while(1){
        PORTDbits.RD2 = ~PORTDbits.RD2;
        __delay_ms(200);
    }
}
