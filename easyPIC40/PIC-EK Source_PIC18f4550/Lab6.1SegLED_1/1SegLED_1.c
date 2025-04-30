/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	PIC-EK
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Version             : V1.0
* By                  : JEFF
* DATE				  : 20150320
*
*(c) Copyright 2010-2018, Logifind Tech CO.,LTD
*http://www.logifind.com
*All Rights Reserved
*
*********************************************************************************************************
********************************************************************************************************/

#include <pic18.h>        //head file

__CONFIG(1,HS) ;  //EXT 20MHz,No PLL
__CONFIG(2,WDTDIS) ;      //WDT OFF
__CONFIG(3,PBADDIS) ;     //PORTB<4:0> configured as digital I/O on RESET 
__CONFIG(4,LVPDIS&XINSTDIS) ;     //Low Voltage Programming Disabled ,Disable extended instruction set (Legacy mode)   

#define LED_a RD0    	//Configure RD0 as A
#define LED_b RD1    	//Configure RD1 as B
#define LED_c RD2    	//Configure RD2 as C
#define LED_d RD3    	//Configure RD3 as D
#define LED_e RD4    	//Configure RD4 as E
#define LED_f RD5    	//Configure RD5 as F
#define LED_g RD6    	//Configure RD6 as G
#define LED_dp RD7    	//Configure RD7 as DP


void main(void)
{	
	OSCCON=0X00;    		
	ADCON1=0X0f;            //Configure all pins as digital
  	TRISD=0B00000000;   	//Configure PORTD as OUTPUT
	PORTD=0B00000000;       //LED_a-LED_dp--OFF
	LED_b=1;   				//B--ON
	LED_c=1;   				//C--ON	
	while(1)
  	{ 		
			
  	}
}
