#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 48000000 // Frecuencia del oscilador
#endif

unsigned int valor; 

void UART_Init(void) {
    TRISCbits.RC6 = 0;           // TX como salida
    TRISCbits.RC7 = 1;           // RX como entrada
    TXSTA = 0x24;                // B 0010 0100 Transmision habilitada, modo asincrono, BRGH=1
    RCSTA = 0x90;                // B 1001 0000 Habilita UART y recepcion
    BAUDCON = 0x00;              // Inicializa Baud Control
    BAUDCONbits.BRG16 = 1;       // Usa BRG de 16 bits para precision

    valor = 1249;                // Calculo SPBRG para 9600 baudios: 48000000Hz / (9600baudios*4) - 1 = 1249;
    SPBRG = valor & 0x00FF;      // Carga los 8 bits menos significativos del valor de baud rate en el registro SPBRG
    SPBRGH = valor >> 8;         // Carga los 8 bits mas significativos en el registro SPBRGH (porque se usa BRG de 16 bits)
}

void Uart_SChar(char txChar) {
    while (TXSTAbits.TRMT == 0); // Espera hasta que el buffer este libre
    TXREG = txChar;              // Envia caracter
}

void Uart_SString(char *txString) {
    while (*txString != '\0') {  // Recorre cada caracter de la cadena
        Uart_SChar(*txString++); // Envia caracter por caracter
    }
}
