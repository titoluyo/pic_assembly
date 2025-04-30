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

//7  6  5  4  3  2  1  0
//v7 v6 v5 v4 v3 v2 v1 v0  
const unsigned char liushui[16]=
{
0B00000001,  //0
0B00000010,
0B00000100,
0B00001000,
0B00010000,
0B00100000,
0B01000000,
0B10000000,
0B11000000,
0B11100000,
0B11110000,   //10
0B11111000,
0B11111100,
0B11111110,
0B11111111,
0B00000000,
};


void main(void)
{	
	unsigned int n;
	unsigned char m=63;	
  	OSCCON=0X00;    		
	ADCON1=0X0f;             //Configure all pins as digital
  	TRISD=0B00000000;   	//Configure PORTD as OUTPUT
	PORTD=0B00000000;       //V0-V7--OFF
  	while(1)
  	{ 		
   		for(n=0;n<10000;n++);   		
		if(++m>15) m=0;    		
		PORTD=liushui[m];  	
  	}
}
