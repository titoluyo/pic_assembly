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

unsigned int a=0;
unsigned char b=9;

const unsigned char LED[10]=
{
0x3f,     //0
0x06,     //1
0x5b,	  //2
0x4f,	  //3
0x66,	  //4
0x6d,	  //5
0x7d,	  //6
0x07,	  //7
0x7f,	  //8
0x6f,	  //9
};


void interrupt init1(void)
{
	if(INT0IF==1)         
	{				
		b=9;            
	}	
	INT0IF=0;             
}


void main(void)
{	
	OSCCON=0X00;    		
	ADCON1=0X0f;            //Configure all pins as digital
	TRISD=0B00000000;   	//RD OUTPUT
	PORTD=0B00000000;       //LED_a-LED_dp  OFF
	RBIE=0;     	   	    
	INTEDG0=0;              	 
  	INT0IE=1;  	            
  	GIE=1;                 
	while(1)
  	{ 		
		if(++a>20000)    	//DELAY
  		{                   		     	
  			a=0;         	
  			if(++b>9) b=0;   //0-9
  			PORTD=LED[b];  				
 		}
  	}
}
