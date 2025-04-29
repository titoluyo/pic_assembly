/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : CN4.0---RA0
                        CN4.1---RA1
                        CN4.2---RA2
                        CN4.3---RA3
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

#define bujin_io1   RA0 //
#define bujin_io2   RA1 //
#define bujin_io3   RA2 //
#define bujin_io4   RA3 //

unsigned int  bujincon1;
unsigned char bujincon2;
const unsigned char bujin_table[]={
0B00000001,
0B00000011,
0B00000010,
0B00000110,
0B00000100,
0B00001100,
0B00001000,
0B00001001,
};


void main(void)
{	
	unsigned char buf;
	OSCCON=0X00;    		
	ADCON1=0X0f;            //Configure all pins as digital
    TRISA=0B11000000;   	    //Configure RA as OUTPUT 
	PORTA=0B00000000;           
  	while(1)
  	{
    	if(++bujincon1>200)
    	{
    		 bujincon1=0;
    		 bujincon2++;
    		 bujincon2&=0x07;	
    		 buf=bujin_table[bujincon2];
    		 if(buf&0x01) bujin_io1=1;
    		 else bujin_io1=0;
    		 if(buf&0x02) bujin_io2=1;
    		 else bujin_io2=0;
    		 if(buf&0x04) bujin_io3=1;
    		 else bujin_io3=0;
    		 if(buf&0x08) bujin_io4=1;
    		 else bujin_io4=0;		
    	}
  	}
}
