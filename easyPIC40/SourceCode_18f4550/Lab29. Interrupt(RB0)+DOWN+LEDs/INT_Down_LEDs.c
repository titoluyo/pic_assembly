/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : CN2.0---RB0
                        CN1---PORT-D
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

unsigned int a;
unsigned char b;
//7  6  5  4  3  2  1  0
//v7 v6 v5 v4 v3 v2 v1 v0  
const unsigned char liushui[64]=
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
0B11111110,
0B11111100,
0B11111000,
0B11110000,
0B11100000,
0B11000000,   //20
0B10000000,
0B10000000,
0B01000000,
0B00100000,
0B00010000,
0B00001000,
0B00000100,
0B00000010,
0B00000001,
0B00000010,  //30
0B00000100,
0B00001000,
0B00010000,
0B00100000,
0B01000000,
0B10000000,
0B01000000,
0B00100000,
0B00010000,
0B00001000,  //40
0B00000100,
0B00000010,
0B00000001,
0B00000011,
0B00000110,
0B00001100,
0B00011000,
0B00110000,
0B01100000,
0B11000000,  //50
0B00000000,
0B00000001,
0B00000101,
0B00010101,
0B01010101,  
0B01010111,
0B01011111,
0B01111111,
0B11111111,
0B00000000,  //60
0B10101010,
0B01010101,
0B00000000,  //63
};


void interrupt init1(void)
{
	if(INT0IF==1)        
	{				
		b=0x3f;            
	}	
	INT0IF=0;             
}


void main(void)
{	
	OSCCON=0X00;    		
	ADCON1=0X0f;            //Configure all pins as digital	
	TRISB=0B11111111;   	//
  	TRISD=0B00000000;   	//
	PORTD=0B00000000;       //V0-V7 OFF
	RBIE=0;     	   	    //
	INTEDG0=0;               //	 
  	INT0IE=1;  	           
  	GIE=1;                  
  	while(1)
  	{ 		
   		if(++a>10000)    
   		{
   			a=0;        
			b++;        
    		b&=0x3f;    
			PORTD=liushui[b]; 	
   		}    
  	}
}


