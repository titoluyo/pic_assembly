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

#define V0 RD0    	
    	

unsigned int ycon;

void timer1init(void)
{
	T1CON=0X01;	
	TMR1IF=0;                
	TMR1IE=1;                
	TMR1H=0xFF;           
	TMR1L=0x13;
	PIR1=0;
	PEIE=1;
	GIE=1;
}

void interrupt  T1(void)
{
	if(TMR1IF==1)           //250us
  {
  		TMR1H=0XFF;
		TMR1L=0x13;		
		TMR1IF=0;
		if(++ycon>2999)  //2000��0.5S
		{
			ycon=0;
			V0=!V0;    	   					
		}
	}
}

void main(void)
{	
	OSCCON=0X00;    		
	ADCON1=0X0f;          
  	TRISD=0B00000000;   	   
	PORTD=0B00000000;         
  	timer1init();  	  
  	while(1)
  	{ 		
    	
  	}
}
