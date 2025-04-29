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

void  delay();              //delay function declare
void  init();               //I/O PORT initialize function declare
const unsigned char TABLE[]=
{                          //Table
    0B11000000,            //"0"
    0B11111001,            //"1"
    0B10100100,            //"2"
    0B10110000,            //"3"
    0B10011001,            //"4"
    0B10010010,            //"5"
    
};

void main()                 //main function
   {
   
     while(1)                //dead circle,LED continue light                       
       {                                                                            
         init();             // call initialize function                            
         PORTD=TABLE[0];     //D PORT OUTPUT LED display code of 0                  
         PORTA=0X3E;         // light the first LED                                 
         delay();            //delay some time ,ensure LED lighteness               
                                                                                    
         PORTD=TABLE[1];     //display data 1(the same as data 0)                   
         PORTA=0X3d;                                                                
         delay();                                                                   
                                                                                    
         PORTD=TABLE[2];     //display data 2(the same as data 0)                   
         PORTA=0X3b;                                                                
         delay();                                                                   
                                                                                    
         PORTD=TABLE[3];     //display data 3(the same as data 0)                   
         PORTA=0X37;                                                                
         delay();                                                                   
                                                                                    
         PORTD=TABLE[4];     //display data 4(the same as data 0)                   
         PORTA=0X2f;                                                                
         delay();                                                                   
                                                                                    
         PORTD=TABLE[5];     //display data 5(the same as data 0)                   
         PORTA=0X1f;                                                                
         delay();                                                                   
        }                                                                           
    }                                                                               
                                                                                    
 void init()                 //I/O PORT initialize function                         
   {                                                                                
    TRISA=0X00;             //set A PORT OUTPUT                                     
    TRISD=0X00;             //set D PORT OUTPUT                                     
    ADCON1=0X07;            //set A PORT general I/O PORT                           
    PORTA=0xff;                                                                     
    PORTD=0xff;             //clear all display                                     
   }                                                                                
                                                                                    
 void  delay()              //delay function                                        
    {                                                                               
     int i;                 //                                                      
     for(i=0x100;i--;);     //delay                                                 
    }  
