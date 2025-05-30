/********************************************************************************************
* PIC16F877A 
* LED 2 - Flash a zero through each of the four "7 segment" displays
*********************************************************************************************/
#include <xc.h>   
#include <stdio.h>
#include <stdlib.h>
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

#define uchar  unsigned char
#define uint   unsigned int

uchar DIS_NUM[4];   
uchar LED_DIS[11] = {
    0xc0,   // 1100 0000  0
    0xf9,   // 1111 1001  1
    0xa4,   // 1010 0100  2
    0xb0,   // 1011 0000  3
    0x99,   // 1001 1001  4
    0x92,   // 1001 0010  5
    0x82,   // 1000 0010  6
    0xf8,   // 1111 1000  7
    0x80,   // 1000 0000  8
    0x90,   // 1001 0000  9
    0xff    // 1111 1111  -
};     
   
void LED_DISPLAY(void);
 
/********************************************************************************************
* void main()
*********************************************************************************************/
void main()
{
    uint counter;
     
    Board_init();    

    while(1)
    {
        for(counter=1234;counter<9999;counter++)
        {
            DIS_NUM[0]=(uchar)(counter/1000);         
            DIS_NUM[1]=(uchar)((counter%1000)/100);   
            DIS_NUM[2]=(uchar)((counter%100)/10);   
            DIS_NUM[3]=(uchar)(counter%10);          
            LED_DISPLAY();                           
        }        
    }  
}

/********************************************************************************************
* void LED_DISPLAY()
*********************************************************************************************/
void LED_DISPLAY(void)
{
    uchar i;
    uchar TEMP;
    uchar k;
    
    RA3 = 0;                
    
//    for(k=0; k<50; k++)
//    {
        for(TEMP = 0X80, i = 0; i < 4; i++)
        {
            RC4    = 1;                    
            PORTD  = TEMP;              
            RC4    = 0;                    
            RC3    = 1;
            PORTD  = LED_DIS[DIS_NUM[i]];
            RC3    = 0;
            TEMP  >>= 1;  
            __delay_ms(200);
            RC4    = 1; 
            PORTD  = 0X00;
            RC4    = 0;
            __delay_ms(200);
        }
//    }
}