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

#define U5A RB0    	//Configure RB0 as U5A
#define U5B RB1    	//Configure RB1 as U5B
#define U5C RB2    	//Configure RB2 as U5C
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


void main(void)
{	
	OSCCON=0X00;    		
	ADCON1=0X0f;            //Configure all pins as digital
	TRISB=0B11111000;   	//Configure RB0-3 as OUTPUT
	TRISD=0B00000000;   	//Configure RD as OUTPUT
	PORTD=0B00000000;       //LED_a-LED_dp--OFF
    U5A=1;			        //U5A=1
	U5B=0;			        //U5B=0
	U5C=0;			        //U5C=0
	PORTD=LED[1];  			//SEND "1" TO PORTD
	while(1)
  	{ 				
			
  	}
}
