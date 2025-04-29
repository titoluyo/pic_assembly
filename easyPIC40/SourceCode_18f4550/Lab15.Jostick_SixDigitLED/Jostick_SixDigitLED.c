/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : CN5.(0~7)---RD(0~7)
                        CN6.(0~5)---RA(0~5)
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

const unsigned char TABLE[] = {0x3f,0x6,0x5b,0x4f,0x66,0x7f};
int result;
void delay();              //delay function declare
void init();                //I/O PORT initialize function declare
void scan();               //key scan function declare
void display(int x);        //display function declare

//main program
void main()
{
	init();   
    PORTD = ~TABLE[5];					//

	while (1)                 //dead circle
	{
		scan();              	//call key scan function
		display(result);     	//call display result function
	}
}

//initialize program
void init()
{
	TRISB = 0X1F;  	//SET RB0-RB4 INPUT
	TRISD = 0;  	//portd OUTPUT
	TRISA = 0;  	//porta OUTPUT
	PORTA = 0x00;	//PORTA OFF
	PORTD = 0x0f;
	RBPU=0;//PORT PULL ON
}

//key scan program
void scan()
{
	result = 0x8;               //initialize key scan result
	if (RB0 == 0)               //judge if B0 press
		result = 0x1;
	if (RB1 == 0)               //judge if B1 press
		result = 0x2;
	if (RB2 == 0)               //judge if B2 press
		result = 0x3;
	if (RB3 == 0)               //judge if B3 press
		result = 0x4;
    if (RB4 == 0)               //judge if B4 press
		result = 0x5;           
}

//display result program
void display(int x)
{
	switch (x)                //different x,different deal
	{
	case 0x1:
		PORTD = ~TABLE[0];
        PORTA = ~0X3F;
        delay();break;  	//B0
	case 0x2:
		PORTD = ~TABLE[1];
        PORTA = ~0X3F;
        delay();break;  	//B1
	case 0x3:
		PORTD = ~TABLE[2];
        PORTA = ~0X3F;
        delay();break;  	//B2
	case 0x4:
		PORTD = ~TABLE[3];
        PORTA = ~0X3F;
        delay();break;  	//B3
    case 0x5:
		PORTD = ~TABLE[4];
        PORTA = ~0X3F;
        delay();break;  	//B4

	case 0x8:
	    //PORTD = ~TABLE[5];
        PORTA = ~0Xff;
        delay();break;  		//NO KEY PRESS
	}
}
//delay program
void delay()              
{
	int i;                   //define integer integer
	for (i = 600;i--;);     //delay
}
