/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : CN1.0---RD0
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

#define L1 RD0    	//Configure RD0 as L1


unsigned int ycon;



void timer0init(void)
{
	T0CS=0;                //TMR0-timer mode
	T0PS3=1;                 
	TMR0IF=0;                //clear flag
	TMR0IE=1;                
	T0CON=0x88;  //16bit	
	TMR0=0xff13;  //4.0M,250us
	GIE=1;
}

void interrupt  ISR(void)
{
	if(TMR0IF==1)           //250us
  	{
		TMR0=0xff13;  //4.0M,250us
		TMR0IF=0;
		if(++ycon>2999)  //2000£¬0.5S
		{
			ycon=0;
			L1=!L1;    	   					
		}
	}
}


void main(void)
{	
	OSCCON=0X00;    		
	ADCON1=0X0f;            
  	TRISD=0B00000000;   	    
	PORTD=0B00000000;      
  	timer0init();  	    	  	
  	while(1)
  	{ 		
    		
  	}
}
