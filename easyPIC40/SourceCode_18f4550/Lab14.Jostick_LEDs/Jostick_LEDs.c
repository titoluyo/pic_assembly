/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : CN1.(0~4)---RD(0~4)
                        CN2.(0~4)---RB(0~4)
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


#define DOWN  RB0   	//Configure RB0 as Joystick's DOWN
#define CENTER  RB1   	//Configure RB1 as Joystick's CENTER
#define LEFT  RB2   	//Configure RB2 as Joystick's LEFT
#define UP  RB3   	    //Configure RB3 as Joystick's UP
#define RIGHT  RB4   	//Configure RB4 as Joystick's RIGHT


const unsigned char LED[]=
{
0B00000001,     //L1,ON
0B00000010,     //L2,ON
0B00000100,	    //L3,ON
0B00001000,	    //L4,ON
0B00010000,	    //L5,ON

};


void main(void)
{
	OSCCON=0X00;    		
	ADCON1=0X0f;            //Configure all pins as digital
	TRISD=0B00000000;   	//Configure RD as OUTPUT 
	TRISB=0B11111111;   	//Configure RB as INPUT	 	
  	while(1)
  	{    	
     	if(DOWN==0) PORTD=LED[0];           
		else if(CENTER==0) PORTD=LED[1];       
		else if(LEFT==0) PORTD=LED[2];     
		else if(UP==0) PORTD=LED[3];       
		else if(RIGHT==0) PORTD=LED[4];   

  	}
}
