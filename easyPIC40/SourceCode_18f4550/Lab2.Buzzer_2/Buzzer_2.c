/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : CN4.4---RD4
* Experimental Results: the Buzzer will sound intermittently.
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

#define beep RD4   	//Configure RD4 as outpout for speaker

unsigned int  fengcon;
unsigned int  beepcon;
bit beepbit;



void fengpro(void)
{
	if(beepbit==1)
	{
		beep=1;
		if(++fengcon>20000)
		{
			fengcon=0;
			beepbit=0;			
		}		 	
	}
	else
	{
		 fengcon=0;
		 beep=0;
	}
}

void main(void)
{
  	OSCCON=0X00;    		
	ADCON1=0X0f;            //Configure all pins as digital  
  	TRISD=0B00000000;   	 
	CCP1CON=0;
	beep=1;	 			 
  	while(1)
  	{
   	 	fengpro();  
		if(++beepcon>60000)
		{
			beepcon=0;
			beepbit=1;		
		}		  	    	
  	}
}
