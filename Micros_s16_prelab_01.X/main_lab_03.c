// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (HS oscillator, PLL enabled (HSPLL))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#include "i2c.h"
#include "lcd_i2c.h"
#include <stdio.h>
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (HS oscillator, PLL enabled (HSPLL))
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 48000000
#endif

#define baudios 9600

Lcd_I2C lcd1 = { .address = 0x4C };

uint16_t ADC_Read(unsigned char ch);
void Uart_SChar(char txChar);
void Uart_SString(char *txString);
char buffer[30]; // store string to send through serial port
unsigned int valor;
unsigned int adc;
float voltaje;
float temperatura;

void main(void) {
    // ADC initialize
    ADCON0 = 0x00; // AN0
    ADCON1 = 0x0E;
    ADCON2 = 0x8F; // 1000 1111
    
    // I2C LCD Display
    I2C_Init_Master(I2C_100KHZ);
    Lcd_Init(&lcd1);
    Lcd_Clear(&lcd1);
    Lcd_Set_Cursor(&lcd1,0,1);
    __delay_ms(100);
    
    // Serial initialize
    TRISCbits.RC6 = 0; // Output
    TRISCbits.RC7 = 1; // Input
    TXSTA = 0x24; // B 0010 0100
    RCSTA = 0x90; // B 1001 0000
    BAUDCON = 0x00; // 0000 0000 reset device
    BAUDCONbits.BRG16 = 1;
    valor = 1249;
    SPBRG = valor & 0x00FF;
    SPBRGH = valor >> 8;

    while(1){
        adc = ADC_Read(0);
        voltaje = adc*5.0/1023.0;
        temperatura = voltaje*(150.0+55.0)/5.0-55;
        
        sprintf(buffer," Temperatura: %.0f\r\n", temperatura);
        Uart_SString(buffer);
        
        __delay_ms(100);
        Lcd_Set_Cursor(&lcd1,0,1);
        __delay_ms(100);
        Lcd_Write_String(&lcd1, buffer);
        __delay_ms(200);
    }

    return;
}

uint16_t ADC_Read(unsigned char ch){
    uint16_t result;
    if(ch > 13){
        result = 0;
    } else {
        ADCON0 = 0x00;
        ADCON0 = (unsigned char)(ch << 2);
        ADCON0bits.ADON = 1; // turn on ADC
        ADCON0bits.GO_DONE = 1;
        while(ADCON0bits.GO_DONE == 1); // while it's busy
        //result = ( ( ADRESH << 8 ) + ADRESL );
        result = ADRES;
    }
    return result;
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
