#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "lcd.h"

#define _XTAL_FREQ 48000000

/*
 * Tosc = 1/48Mhz = 20.83ns 
 * According datasheet for 48Mhz -> 64 * Tosc = 64 * 20.83ns = 1.33us
 * Calculating min req acquisition Time : 2.4 us
 * needed more time 1.33us * 2 = 2.66
 * => TAD = 2
 * 
 * For 8Mhz : 11.43Mhz => 8 * Tosc
 * 8 * (1 / 8mhz) = 1us
 * but needed 2.4 us
 * needed more time 1us * 4 = 4us
 */

//int valorAN0;
//void ADC_init();
//unsigned int ADC_Read(unsigned char ch);
int valoradc;
float valorvolt;
char buffer[10];

int main(int argc, char** argv) {
    Lcd_Init();
    Lcd_Clear();
    TRISB = 0; // All port B as output
    TRISDbits.RD0 = 0; // RD0 output
    TRISDbits.RD1 = 0; // RD1 output
    LATB = 0; // Initialize OFF
    LATDbits.LATD0 = 0; 
    LATDbits.LATD1 = 0;
    
    ADCON0 = 0x00; // datasheet pg:261 Channel0(AN0)
    ADCON1 = 0x0E; // datasheet pg:262 AN0 as Analog
    ADCON2 = 0x8F; // 1000 1111 pg:263 bit7:right justified, tad:2, Frc
    ADCON0bits.ADON = 1; // Turn on the ADC
    
    
//    void ADC_init();
//    TRISB = 0; // port B as Output
//    ADCON0bits.ADON = 1; // A/D converter enabled

    while(1) {
        Lcd_Set_Cursor(1, 1);
        ADCON0bits.GO_DONE = 1; // starts to convert
        while(ADCON0bits.GO_DONE == 1);
        LATB = ADRESL;
        LATD = ADRESH;
        __delay_ms(50);
        
        valoradc = (int)ADRES;
        valorvolt = 0.004887585f * (float)valoradc;
        
//        int intPart = (int)valorvolt;
//        int fracPart = (int)((valorvolt - intPart) * 100); // 2 decimals
//        sprintf(buffer, "ADC: %d.%02d", intPart, abs(fracPart));

//        floatToStr(valorvolt, buffer);
        sprintf(buffer, "   ADC: %.2f", valorvolt);
        __delay_ms(100);
        Lcd_Write_String(buffer);
        
//        valorAN0 = ADC_Read();
//        valorAN0 = valorAN0 / 4; // 10bits -> 8bits
//        LATB = valorAN0;
//        __delay_ms(100);
    }
    
    return (EXIT_SUCCESS);
}

//void ADC_Init() {
//    ADCON1bits.PCFG = 0x0E; // AN0 as Analog
//    ADCON1bits.VCFG = 0;
//    ADCON1bits.VCFG1 = 0;
//    ADCON0 = 0x00;
//    ADCON0 = 0x8F;
//}
//
//unsigned int ADC_Read(unsigned char ch){
//    ADCON0 = 0x00;
//    ADCON0 = (ch<<2);
//    ADCON0bits.ADON = 1;
//    ADCON0bits.GO_DONE = 1;
//    while(ADCON0bits.GO_DONE == 1);
//    return ADRES;
//}