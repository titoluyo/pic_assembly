/********************************************************************************************
* PIC16F877A 
*********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "BoardConfig.h" 

#pragma config FOSC  = XT       // Oscillator Selection bits (XT oscillator)
#pragma config WDTE  = OFF      // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP   = OFF      // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD   = OFF      // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT   = OFF      // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP    = OFF      // Flash Program Memory Code Protection bit (Code protection off)

// Unless specified elsewhere, 4MHz system frequency is assumed
#ifndef _XTAL_FREQ
 #define _XTAL_FREQ 4000000
#endif
      
/****************************************************************************
* main()
****************************************************************************/
void main (void)
{
    Board_init();            
    RC4=1;
    while(1)
    {
        PORTD=0XF7;
        __delay_ms(10);
       
        PORTD=0XFB;
        __delay_ms(10);
        
        PORTD=0XFD;
        __delay_ms(10);
        
        PORTD=0XFE;
        __delay_ms(10);
    }
}