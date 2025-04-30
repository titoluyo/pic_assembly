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
#define beep RC2   	//Configure RC2 as SPEAKER



void main(void)
{
	OSCCON=0X00;    		
	ADCON1=0X0f;            //Configure all pins as digital
	TRISC=0B11111011;   	//Configure RC as INPUT		
	TRISB=0B11111111;   	//Configure RB as INPUT	 	  			 
  	while(1)
  	{    	
     	if(BUTTON1==0) beep=1;              //If BUTTON1 pressed£¬SPEAKER  will be activated
		else if(BUTTON2==0) beep=1; //If BUTTON2 pressed£¬SPEAKER  will be activated  
		else if(BUTTON3==0) beep=1; //If BUTTON3 pressed£¬SPEAKER  will be activated
		else if(RIGHT==0) beep=1; //If right pressed£¬SPEAKER  will be activated
		else if(DOWN==0) beep=1; //If down pressed£¬SPEAKER  will be activated
		else if(LEFT==0) beep=1; //If left pressed£¬SPEAKER  will be activated
		else if(UP==0) beep=1; //If up pressed£¬SPEAKER  will be activated	
		else if(CENTER==0) beep=1; //If center pressed£¬SPEAKER  will be activated
		else beep=0;  		   //If no key pressed£¬SPEAKER will be OFF 
  	}
}
