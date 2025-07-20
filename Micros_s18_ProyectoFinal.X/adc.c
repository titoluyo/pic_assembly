//*
#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 48000000
#endif

void ADC_Init(void) {
    // ADC initialize
    ADCON0 = 0x00; // AN0
    ADCON1 = 0x0E;
    ADCON2 = 0x8F; // 1000 1111
    //ADCON2 = 0xA9; // 1010 1001
}

uint16_t ADC_Read(unsigned char ch) {
    uint16_t result = 0;
    if (ch <= 13) {
        ADCON0 = (ch << 2); // Selecciona canal
        ADCON0bits.ADON = 1; // Enciende ADC
        __delay_us(20); // Tiempo de adquisición
        ADCON0bits.GO_DONE = 1;
        while (ADCON0bits.GO_DONE); // Espera a que termine
        result = ((uint16_t) ADRESH << 8) | ADRESL;
    }
    return result;
}
//*/