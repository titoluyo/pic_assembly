/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : CN1.1---RC1
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
#include <pic18.h>        

__CONFIG(1,HS) ;  //EXT 20MHz,No PLL
__CONFIG(2,WDTDIS) ;      //WDT OFF
__CONFIG(3,PBADDIS) ;     //PORTB<4:0> configured as digital I/O on RESET 
__CONFIG(4,LVPDIS&XINSTDIS) ;     //Low Voltage Programming Disabled ,Disable extended instruction set (Legacy mode)   

unsigned int ycon;
unsigned char pwmbuf;



void main(void)
{	
	OSCCON=0X00;    		
	ADCON1=0X0f;            
  	TRISC=0B00000000;   	
	PORTC=0B00000000;       //V0-V7 off
	CCP2CON=0x3c;	        //CCP2---PWM mode
	PR2=63;              	//15.625KHZ
	T2CKPS1=0;
	T2CKPS0=0;           	//1£º1
	TMR2ON=1;	         	//start TIMER2
	pwmbuf=0;               
	CCPR2L=pwmbuf;          
  	while(1)
  	{ 		   		
   		if(++ycon>1000)    
   		{
   			ycon=0;        	
   			pwmbuf++;	   
			CCPR2L=pwmbuf;	
   		}    	
  	}
}
