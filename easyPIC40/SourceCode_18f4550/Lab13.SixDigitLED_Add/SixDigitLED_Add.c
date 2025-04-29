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


unsigned long a1,a2,a3,a4,a5,a6,a7,a8;
const unsigned char LED[10]=
{                          
    0B11000000,            //"0"
    0B11111001,            //"1"
    0B10100100,            //"2"
    0B10110000,            //"3"
    0B10011001,            //"4"
    0B10010010,            //"5"

    0B10000010,            //"6"
    0B11111000,            //"7"
    0B10000000,            //"8"
    0B10010000,            //"9"

};



 void  delay()              //delay function                                        
    {                                                                               
     int i;                 //                                                      
     for(i=50;i--;);     //delay                                                 
    } 


void main(void)            
{

    unsigned char c=0;     
    unsigned char d=0;    
    unsigned char e=0;     
    unsigned long f=0;     //0-999999



    TRISA=0X00;             //set A PORT OUTPUT                                     
    TRISD=0X00;             //set D PORT OUTPUT                                     
    ADCON1=0X07;            //set A PORT general I/O PORT                           
    PORTA=0xFF;                                                                     
    PORTD=0xFF;             //clear all display 





    while(1)               
    {

       for(c=0;c<100;c++);
       if(++e>20)        
       {
           e=0;           
            if(++f>999999) f=0;//0-999999
        }
        PORTD=0xFF;           
        if(++d>5) d=0;     
        if(d==0)           //Digit-6
        {
            PORTA=0x1F;
            PORTD=LED[f/100000];       
//delay(); 
        }
        else if(d==1)      //Digit-5
        {
            PORTA=0x2F;
            PORTD=LED[(f%100000)/10000]; 
//delay(); 
        }
        else if(d==2)      //Digit-4
        {
            PORTA=0x37;
            PORTD=LED[(f%10000)/1000];   
//delay(); 
        }
        else if(d==3)      //Digit-3
        {
            PORTA=0x3B;
            PORTD=LED[(f%1000)/100];         
//delay(); 
        }

        else if(d==4)      //Digit-2
        {
            PORTA=0x3D;
//delay(); 
            PORTD=LED[(f%100)/10];   
        }
        else if(d==5)      //Digit-1
        {
            PORTA=0x3E;
            PORTD=LED[f%10];         
//delay(); 


    }
}
}
