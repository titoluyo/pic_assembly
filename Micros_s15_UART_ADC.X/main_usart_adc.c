/*
 * File:   main_usart_adc.c
 * Author: tito.luyo
 *
 * Created on July 3, 2025, 6:42 PM
 */
// LAB: LM35 ver datasheet
// tempC = ((5.0 * tempC * 100.0)/1024.00)-50

#include <xc.h>
#include <stdio.h>
//#include "config.h"
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (HS oscillator, PLL enabled (HSPLL))
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

#define _XTAL_FREQ 48000000  
#define baudios 9600

unsigned int ADC_Read(unsigned char ch);
void Uart_SChar(char txChar);
void Uart_SString(char *txString);
char buffer[30]; // store string to send through serial port
unsigned int valor;
unsigned int adc;

void main(void) {
    // ADC initialize
    ADCON0 = 0x00;
    ADCON1 = 0x0E;
    ADCON2 = 0x8F;
    
    // Serial initialize
    TRISCbits.RC6 = 0; // Output
    TRISCbits.RC7 = 1; // Input
    TXSTA = 0x24; // B 0010 0100
    RCSTA = 0x90; // B 1001 0000
    BAUDCON = 0x00; // 0000 0000 reset device
    BAUDCONbits.BRG16 = 1;
    valor = 1249; //(_XTAL_FREQ/(baudios*4))-1;
    SPBRG = valor & 0x00FF;
    SPBRGH = valor >> 8;

    while(1){
        adc = ADC_Read(0);
        sprintf(buffer,"ADC0 valor: %d\r\n", adc);
        Uart_SString(buffer);
        __delay_ms(200);
    }

    return;
}

unsigned int ADC_Read(unsigned char ch){
    if(ch > 13){
        return 0;
    } else {
        ADCON0 = 0x00;
        ADCON0 = (unsigned char)(ch << 2);
        ADCON0bits.ADON = 1; // turn on ADC
        ADCON0bits.GO_DONE = 1;
        while(ADCON0bits.GO_DONE == 1); // while it's busy
        return ADRES;
    }
    
}
void Uart_SChar(char txChar){
    while (TXSTAbits.TRMT == 0);
    TXREG = txChar;    
}
void Uart_SString(char *txString){
    while (*txString != '\0'){
        Uart_SChar(*txString++);
    }
}
