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

unsigned int ycon;
unsigned char a;

 
void timer2init(void)
{
	T2CON=0X04;	
	TMR2IF=0;              
	TMR2IE=1;            
	TMR2=0x13;            	
	PIR1=0;
	PEIE=1;
	GIE=1;
}

void interrupt  T2(void)
{
	if(TMR2IF==1)           //250us
  	{  		
		TMR2=0x13;		
		TMR2IF=0;
		if(++ycon>3999)  //4000��1S
		{
			ycon=0;
			if(++a>9) a=0;   //0-9
  			PORTD=LED[a];  					
		}
	}
}

void main(void)
{	
	ADCON1=0X06;            
  	TRISD=0B00000000;   	    
	PORTD=0B00000000;       
  	timer2init();  	    	  	
  	while(1)
  	{ 		
    		
  	}
}
