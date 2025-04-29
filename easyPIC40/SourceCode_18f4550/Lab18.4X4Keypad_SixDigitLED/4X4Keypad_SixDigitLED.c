/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : CN5.(0~7)---RD(0~7)
                        CN6.(0~5)---RA(0~5)
                        CN3.(0~7)---RB(0~7)
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

//---------------------------------------
//数码管字形表，供显示时查询
const unsigned char TABLE[16]=
{                          //定义表格一定要使用const，这样会做到程序存储区中
    0B00111111,            //"0"的字形表，0x3F
    0B00000110,            //"1"的字形表，0x06
    0B01011011,            //"2"的字形表，0x5B
    0B01001111,            //"3"的字形表，0x4F
    0B01100110,            //"4"的字形表，0x66
    0B01101101,            //"5"的字形表，0x6D
    0B01111101,            //"6"的字形表，0x7D
    0B00000111,            //"7"的字形表，0x07
    0B01111111,            //"8"的字形表，0x7F
    0B01101111,            //"9"的字形表，0x6F
    0B01110111,            //"A"的字形表，0x77
    0B01111100,            //"B"的字形表，0x7C
    0B00111001,            //"C"的字形表，0x39
    0B01011110,            //"D"的字形表，0x5E
    0B01111001,            //"E"的字形表，0x79
    0B01110001,            //"F"的字形表，0x71
};
int result=0x00,preres=0x00;
void delay();              	
void init();               	
void scan();               
void display();       	


void main()
{
	init();   

             	
	while (1)                  
	{

		scan();               
		display(result);     
	}
}


void init()
{
	ADCON1 = 0X07;             
	TRISB = 0X0F;  				 
	TRISD = 0;  			
	TRISA = 0;  				
	PORTA = 0xFF;				
	PORTD = 0x40;
     RBPU=0;
}


void scan()
{
	PORTB = 0X7f;                
	asm("nop");                
	result = PORTB;               
	result = result & 0x0f;       
	if (result != 0x0f)            
	{
		result = result | 0x70;    
	}
	else                        
	{
		PORTB = 0Xbf;              
		asm("nop");             
		result = PORTB;             
		result = result & 0x0f;       
		if (result != 0xf)           
		{
			result = result | 0xb0;    
		}
		else                     
		{
			PORTB = 0Xdf;          
			asm("nop");         
			result = PORTB;        
			result = result & 0x0f;  
			if (result != 0x0f)      
			{
				result = result | 0xd0; 
			}
			else                  
			{
				PORTB = 0Xef;       
				asm("nop");        
				result = PORTB;     
				result = result & 0x0f; 
				if (result != 0x0f)   
				{
					result = result | 0xe0; 
				}
				else               
				{
					result = 0xff;   
				}
			}
		}
	}
	if(result==0xff)
		result=preres;
	else
		preres=result;


}


void display()
{
	switch (result)
	{
	case 0xe7:
		PORTD = ~TABLE[3];PORTA = 0X00;delay();break;   	//K3
	case 0xeb:
		PORTD = ~TABLE[2];PORTA = 0X00;delay();break;   	//K2
	case 0xed:
		PORTD = ~TABLE[1];PORTA = 0X00;delay();break;   	//K1
	case 0xee:
		PORTD = ~TABLE[0];PORTA = 0X00;delay();break;   	//K0
	case 0xd7:
		PORTD = ~TABLE[7];PORTA = 0X00;delay();break;   	//K7
	case 0xdb:
		PORTD = ~TABLE[6];PORTA = 0X00;delay();break;   	//K6
	case 0xdd:
		PORTD = ~TABLE[5];PORTA = 0X00;delay();break;   	//K5
	case 0xde:
		PORTD = ~TABLE[4];PORTA = 0X00;delay();break;   	//K4
	case 0xb7:
		PORTD = ~TABLE[11];PORTA = 0X00;delay();break;   //KB
	case 0xbb:
		PORTD = ~TABLE[10];PORTA = 0X00;delay();break;   //KA
	case 0xbd:
		PORTD = ~TABLE[9];PORTA = 0X00;delay();break;   	//K9
	case 0xbe:
		PORTD = ~TABLE[8];PORTA = 0X00;delay();break;   	//K8
	case 0x77:
		PORTD = ~TABLE[15];PORTA = 0X00;delay();break;   //KF
	case 0x7b:
		PORTD = ~TABLE[14];PORTA = 0X00;delay();break;   //KE
	case 0x7d:
		PORTD = ~TABLE[13];PORTA = 0X00;delay();break;   //KD
	case 0x7e:
		PORTD = ~TABLE[12];PORTA = 0X00;delay();break;   //KC
	case 0x00:
		PORTD = 0x40; PORTA = 0x00; delay();     
	}
}


void delay()
{
	int i;                 		
	for (i = 0x100;i--;);     
}