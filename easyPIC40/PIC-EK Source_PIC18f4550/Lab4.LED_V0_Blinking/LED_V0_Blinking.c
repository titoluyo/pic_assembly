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


#define V0 RD0    	//Configure RD0 as V0


unsigned int ycon;



void main(void)
{	
	OSCCON=0X00;    		
	ADCON1=0X0f;            //Configure all pins as digital
  	TRISD=0B00000000;   	//Configure PORTD as OUTPUT
	PORTD=0B00000000;       //V0-V7---OFF
  	while(1)
  	{ 		   		
   		if(++ycon>5000)     //DELAY
   		{
   			ycon=0;        	//CLEAR
   			V0=!V0;    	   			
   		}    	
  	}
}
