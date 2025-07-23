#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 48000000 // Frecuencia del oscilador
#endif

void ADC_Init(void) {
    ADCON0 = 0x00;     // Selecciona AN0 como canal inicial
    ADCON1 = 0x0D;     // AN0 y AN1 como entrada analogica, el resto digitales
    ADCON2 = 0x8F;     // 1000 1111 Justificacion derecha, Fosc/32, tiempo de adquisicion
}

uint16_t ADC_Read(unsigned char ch) {
    uint16_t result = 0;                        // Declara e inicializa resultado
    if (ch <= 13) {                             // Verifica canal valido
        ADCON0 = (unsigned char)(ch << 2);      // Selecciona el canal deseado
        ADCON0bits.ADON = 1;                    // Habilita el ADC
        __delay_us(20);                         // Espera para adquirir muestra
        ADCON0bits.GO_DONE = 1;                 // Inicia conversion
        while (ADCON0bits.GO_DONE);             // Espera a que termine
        result = ((uint16_t) ADRESH << 8) | ADRESL; // Combina resultado 10 bits
    }
    return result;
}
