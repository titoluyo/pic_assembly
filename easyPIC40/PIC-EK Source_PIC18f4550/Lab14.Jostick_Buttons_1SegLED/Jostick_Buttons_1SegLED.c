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


#define BUTTON1  RB0   	//Configure RB0 as BUTTON1
#define BUTTON2  RB1   	//Configure RB1 as BUTTON2
#define BUTTON3  RB2   	//Configure RB2 as BUTTON3
#define RIGHT  RB3   	//Configure RB0 as right of Joystick
#define DOWN  RB4   	//Configure RB0 as down of Joystick
#define LEFT  RB5   	//Configure RB0 as left of Joystick
#define UP  RB6   	//Configure RB0 as up of Joystick
#define CENTER  RB7   	//Configure RB0 as center of Joystick

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
	TRISD=0B00000000;   	//Configure RD as OUTPUT 
	TRISB=0B11111111;   	//Configure RB as INPUT	 	
  	while(1)
  	{    	
     	if(BUTTON1==0) PORTD=LED[1];              //If BUTTON1 pressed£¬"1" will be displayed  
		else if(BUTTON2==0) PORTD=LED[2]; //If BUTTON2 pressed£¬"2" will be displayed    
		else if(BUTTON3==0) PORTD=LED[3]; //If BUTTON3 pressed£¬"3" will be displayed 
		else if(RIGHT==0) PORTD=LED[4]; //If right pressed£¬"4" will be displayed 
		else if(DOWN==0) PORTD=LED[5]; //If down pressed£¬"5" will be displayed 
		else if(LEFT==0) PORTD=LED[6]; //If left pressed£¬"6" will be displayed 
		else if(UP==0) PORTD=LED[7]; //If up pressed£¬"7" will be displayed 	
		else if(CENTER==0) PORTD=LED[8]; //If center pressed£¬"8" will be displayed 
		else PORTD=0;  		         //If no key pressed£¬nothing will be displayed  
  	}
}
