#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 48000000
#endif

unsigned int valor;

void UART_Init(void) {
    // Serial initialize
    TRISCbits.RC6 = 0; // Output
    TRISCbits.RC7 = 1; // Input
    TXSTA = 0x24; // B 0010 0100
    RCSTA = 0x90; // B 1001 0000
    BAUDCON = 0x00; // 0000 0000 reset device
    BAUDCONbits.BRG16 = 1;
    valor = 1249; // (unsigned int)((float)_XTAL_FREQ/(float)(baudios*4))-1; // 1249;
    SPBRG = valor & 0x00FF;
    SPBRGH = valor >> 8;
}

void Uart_SChar(char txChar) {
    while (TXSTAbits.TRMT == 0);
    TXREG = txChar;
}

void Uart_SString(char *txString) {
    while (*txString != '\0') {
        Uart_SChar(*txString++);
    }
}
