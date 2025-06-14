//#include <stdio.h>
//#include <stdlib.h>

#include <xc.h>
#include <stdint.h>

// CONFIG1L
#pragma config PLLDIV = 2       // (8 MHz crystal -> 4 MHz input)
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 1
// CONFIG1H
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (HS oscillator, PLL enabled (HSPLL))
#pragma config FCMEN = OFF
#pragma config IESO = OFF
// CONFIG2L
#pragma config PWRT = ON
#pragma config BOR = ON
#pragma config BORV = 3
#pragma config VREGEN = ON
// CONFIG2H
#pragma config WDT = OFF
#pragma config WDTPS = 32768
// CONFIG3H
#pragma config CCP2MX = ON
#pragma config PBADEN = OFF     // PORTB<4:0> as digital I/O
#pragma config LPT1OSC = OFF
#pragma config MCLRE = ON
// CONFIG4L
#pragma config STVREN = ON
#pragma config LVP = OFF
#pragma config ICPRT = OFF
#pragma config XINST = OFF
// CONFIG5L-7H: code protection -> OFF

#define _XTAL_FREQ 48000000UL

void setupTimer0(void);
void __interrupt() isr(void);

void main(void) {
    TRISD0 = 0;     // RD0 como salida
    LATD0 = 0;      // LED apagado

    setupTimer0();

    while (1) {
        // El trabajo ocurre en la interrupción
    }
}

void setupTimer0(void) {
    // Configurar Timer0 como temporizador de 16 bits con prescaler 1:256
    T0CONbits.TMR0ON = 0;
    T0CONbits.T08BIT = 0;
    T0CONbits.T0CS = 0;         // Reloj interno
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 0b111;     // Prescaler 1:256

    TMR0H = 0xA4;               // 500 ms ? 0xA473
    TMR0L = 0x73;

    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    T0CONbits.TMR0ON = 1;
}

// ISR de interrupción global
void __interrupt() isr(void) {
    if (INTCONbits.TMR0IF) {
        LATD0 ^= 1;             // Toggle LED

        TMR0H = 0xA4;           // Recargar para 500 ms
        TMR0L = 0x73;

        INTCONbits.TMR0IF = 0;
    }
}

