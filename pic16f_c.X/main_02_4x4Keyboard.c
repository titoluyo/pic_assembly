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

uchar LED_DIS[5] = {0x92,0xbf,0xc0,0xc0};  

void LED_DISPLAY(void);    
void delay(uchar a,uchar i);
    

/********************************************************************************************
* void main (void)
*********************************************************************************************/
void main (void)
{
    uchar key;
    Board_init();      
    TRISB       = 0x0F;      
    PORTB       = 0x0F;    
    OPTION_REG &= 0x7F;     
    ADCON1      = 0x00;
  
    while(1)
    {
        key = PORTB & 0x0F;            
        if(key != 0x0F)   
        {
            switch(key)
            {
                case  0x0E:            
                    LED_DIS[2] = 0xF9;    
                    LED_DIS[3] = 0xF8;      
                    break;
                case  0x0D:           
                    LED_DIS[2] = 0xF9; 
                    LED_DIS[3] = 0x80;
                    break;
                case  0x0B:          
                    LED_DIS[2] = 0xF9; 
                    LED_DIS[3] = 0x90;
                    break;
                case  0x07:           
                    LED_DIS[2] = 0xA4; 
                    LED_DIS[3] = 0xC0;
                    break;
            }
        }
        LED_DISPLAY();       
    }
}

/********************************************************************************************
* void LED_DISPLAY(void)
*********************************************************************************************/
void LED_DISPLAY(void)
{
    uchar i;
    uchar TEMP;
    RA3   = 0;         
    TEMP  = 0x80;
    for(i=0; i<4; i++)
    {
        RC4   = 1;                  
        PORTD = TEMP;               
        RC4   = 0;                
        RC3   = 1;
        PORTD = LED_DIS[i];
        RC3   = 0;
        TEMP >>= 1;      
        PORTD = 0x00;              
        RC4   = 0;                  
        delay(3,10);
    } 
}

/********************************************************************************************
* void delay(uchar a,uchar i)
*********************************************************************************************/
void delay(uchar a,uchar i)
{	
    while(--a)
    while(--i);
}