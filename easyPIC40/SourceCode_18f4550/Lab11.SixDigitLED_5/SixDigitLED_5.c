/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : CN5.(0~7)---RD(0~7)
                        CN6.(0~5)---RA(0~5)
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

const unsigned char LED[]=
{                          //Table
    0B11000000,            //"0"
    0B11111001,            //"1"
    0B10100100,            //"2"
    0B10110000,            //"3"
    0B10011001,            //"4"
    0B10010010,            //"5"
    
};



void main(void)            
{



    TRISA=0X00;             //set A PORT OUTPUT                                     
    TRISD=0X00;             //set D PORT OUTPUT                                     
    ADCON1=0X07;            //set A PORT general I/O PORT                           
    PORTA=0xff;                                                                     
    PORTD=0xff;             //clear all display 

    //****Display "5"****
    PORTA=0X1F;         
    PORTD=LED[5];          
    

    while(1)               
    {

    }
}
