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

#define keyO1  RB0   	
#define keyO2  RB1   	
#define keyO3  RB2   	
#define keyO4  RB3   	
#define keyI1  RB4   	
#define keyI2  RB5   	
#define keyI3  RB6   	
#define keyI4  RB7   	

const unsigned char LED[16]=
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
0x77,	  //A
0x7C,	  //B
0x39,	  //C
0x5E,	  //D
0x79,	  //E
0x71,	  //F
};


unsigned char key(void)
{
	keyO1=0;    		//Configure LINE1 as 0
	keyO2=1;    		//Configure LINE2 as 1
	keyO3=1;    		//Configure LINE3 as 1
	keyO4=1;    		//Configure LINE4 as 1	
	asm("NOP");	    	//DELAY
	asm("NOP");
	asm("NOP");	
	asm("NOP");	
	if(keyI1==0)    	//IF ROW1==0£¬It means K1 was pressed£¬return1
	{		
		return 1;				
	}
	else if(keyI2==0)   //IF ROW2==0£¬It means K2 was pressed£¬return2
	{
		return 2;
	}
	else if(keyI3==0)   //IF ROW3==0£¬It means K3 was pressed£¬return3
	{
		return 3;
	}
	else if(keyI4==0)   //IF ROW4==0£¬It means K4 was pressed£¬return4
	{
		return 4;
	}
	keyO1=1;    		//Configure LINE1 as 1
	keyO2=0;    		//Configure LINE2 as 0
	keyO3=1;    		//Configure LINE3 as 0
	keyO4=1;    		//Configure LINE4 as 0	
	asm("NOP");		   	//delay
	asm("NOP");
	asm("NOP");	
	asm("NOP");	
	if(keyI1==0)    	//IF ROW1==0£¬It means K5 was pressed£¬return5
	{		
		return 5;				
	}
	else if(keyI2==0)      //IF ROW2==0£¬It means K6 was pressed£¬return6
	{
		return 6;
	}
	else if(keyI3==0)      //IF ROW3==0£¬It means K7 was pressed£¬return7
	{
		return 7;
	}	
	else if(keyI4==0)      //IF ROW4==0£¬It means K8 was pressed£¬return8
	{
		return 8;
	}
	keyO1=1;    		//Configure LINE1 as 1
	keyO2=1;    		//Configure LINE2 as 1
	keyO3=0;    		//Configure LINE3 as 0
	keyO4=1;    		//Configure LINE4 as 1	
	asm("NOP");		   	//delay	
	asm("NOP");
	asm("NOP");	
	asm("NOP");	
	if(keyI1==0)        //IF ROW1==0£¬It means K9 was pressed£¬return9
	{		
		return 9;				
	}
	else if(keyI2==0)   //IF ROW2==0£¬It means K10 was pressed£¬return10
	{
		return 10;
	}
	else if(keyI3==0)   //IF ROW3==0£¬It means K11 was pressed£¬return11
	{
		return 11;
	}	
	else if(keyI4==0)   //IF ROW4==0£¬It means K12 was pressed£¬return12
	{
		return 12;
	}
	keyO1=1;    		//Configure LINE1 as 1
	keyO2=1;    		//Configure LINE2 as 1
	keyO3=1;    		//Configure LINE3 as 1
	keyO4=0;    		//Configure LINE4 as 0	
	asm("NOP");		   	//DELAY	
	asm("NOP");
	asm("NOP");	
	asm("NOP");	
	if(keyI1==0)    	//IF ROW1==0£¬It means K13 was pressed£¬return13
	{		
		return 13;				
	}
	else if(keyI2==0)   //IF ROW2==0£¬It means K14 was pressed£¬return14
	{
		return 14;
	}
	else if(keyI3==0)   //IF ROW3==0£¬It means K15 was pressed£¬return15
	{
		return 15;
	}	
	else if(keyI4==0)   //IF ROW4==0£¬It means K16 was pressed£¬return16
	{
		return 16;
	}
	return 0;           //NO KEY PRESSED,return0
}


void main(void)
{
	unsigned char keybuf;
	OSCCON=0X00;    		
	ADCON1=0X0f;            //Configure all pins as digital
	TRISD=0B00000000;   	//Configure RD as OUTPUT  
	TRISB=0B11110000;   	//Configure RB0-3 as OUTPUT,Configure RB4-7 as INPUT		 
  	while(1)
  	{    	
     	keybuf=key();      //READ KEY VALUE
		if(keybuf==1) PORTD=LED[0];        //If k1 pressed,"0"will be display.
		else if(keybuf==2) PORTD=LED[1];   //If k2 pressed,"1"will be display.
		else if(keybuf==3) PORTD=LED[2];   //If k3 pressed,"2"will be display.
		else if(keybuf==4) PORTD=LED[3];   //If k4 pressed,"3"will be display.
		else if(keybuf==5) PORTD=LED[4];   //If k5 pressed,"4"will be display.
		else if(keybuf==6) PORTD=LED[5];   //If k6 pressed,"5"will be display.
		else if(keybuf==7) PORTD=LED[6];   //If k7 pressed,"6"will be display.	
		else if(keybuf==8) PORTD=LED[7];   //If k8 pressed,"7"will be display.
		else if(keybuf==9) PORTD=LED[8];   //If k9 pressed,"8"will be display.
		else if(keybuf==10) PORTD=LED[9];  //If k10 pressed,"9"will be display.
		else if(keybuf==11) PORTD=LED[10]; //If k11 pressed,"A"will be display.
		else if(keybuf==12) PORTD=LED[11]; //If k12 pressed,"B"will be display.
		else if(keybuf==13) PORTD=LED[12]; //If k13 pressed,"C"will be display.
		else if(keybuf==14) PORTD=LED[13]; //If k14 pressed,"D"will be display.	
		else if(keybuf==15) PORTD=LED[14]; //If k15 pressed,"E"will be display.
		else if(keybuf==16) PORTD=LED[15]; //If k16 pressed,"F"will be display.
		else PORTD=0;  		         	      		  	    	
  	}
}
