/********************************************************************************************
* PIC16F877A 
* Output via J5, where RC2 sinks current using the ULN2003 chip   
* Pressing and holding red button K18 will decrease LED10 brightness
* Pressing and holding red button K19 will increase LED10 brightness
* N.B. There is a low and high limit in LED10 brightness, 
*      no matter how long the button is pressed
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


#define u8	unsigned char
u8 a;

void PWM_Init() ;
void repeat();

/********************************************************************************************
* void main() 
*********************************************************************************************/
void main() 
{  
    Board_init();       
    PWM_Init();	     
    T2CON=0X04;	    
    do 
    { 	
        repeat(); 
    }while(1);        
}

/********************************************************************************************
* void PWM_Init() 
*********************************************************************************************/			
void PWM_Init() 
{  
    PORTB   = 0xff;         // Only used for Buttons K18 and K19
    TRISB   = 0xff;         // Only used for Buttons K18 and K19
    nRBPU   = 0;            // Only used for Buttons K18 and K19

    a       = 0XD0;         
    TMR1    = a;            // Set Duty counter to mid value brightness (think logs)
    CCPR1L  = a;            // For RC2
    CCP1CON = 0X3C;	 
    
//  CCPR2L  = a;            // For RC1
//  CCP2CON = 0X3C;	   

    INTCON  = 0X00;	      

// To control the PWM duty cycle over a range of 0 to 100 % 
// the CCPRxL register must settable to values from zero to PR2+1. 
// If PR2 is set to 0xFF then there is no value that the CCPRxL register 
// can be set that will yield a 100% duty cycle.
// If you need to reach 100% for the duty cycle the maximum value 
// you can use for PR2 is 0xFE.
// This results in a "best case" duty cycle resolution of 1 part in 1020. 
// This is just 4 counts short of 10-bits so Microchip just calls it good.
    PR2     = 0XFE;  	      
    TRISC   = 0XF8;	     
}

/********************************************************************************************
* void repeat()
*********************************************************************************************/	
void repeat()
{
    TRISB=0xff;
    if(RB1==0)
    {
        __delay_ms(5);
        if(RB1 == 0)
        {
            if( (a+1)< 0xff)
            {
                a += 1;
                CCPR1L = a;
//                CCPR2L =  a;
            }else{
                a      = 0xff;
                CCPR1L = a;
            }
        }
//        while(RB1 == 0);
    }		
    if(RB2==0)
    {
        __delay_ms(5);
        if(RB2==0)
        {
            if( (a-1)> 0x00)
            {
                a      -= 1;
                CCPR1L =  a;
//                CCPR2L =  a;
            }else{
                a      = 0x00;
                CCPR1L = a;
            }
        }
//        while(RB2==0);
    }		
}