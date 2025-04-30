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


#define RS  RB5 //Data or Instrument Select
#define RW  RB4 //Write or Read
#define E  RB3 //6800 mode Enable single

unsigned int a;
unsigned int b;


void delay(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<10;j++);
	}
}



void lcd_wait_busy(void)
{  
	TRISD7=1;	   		   
   	RS=0;			   
   	RW=1;			   
   	E=1;				   
   	while(RD7==1);         
   	E=0;				   
   	TRISD7=0;			   
}


void lcd_write_com(unsigned char combuf)
{  
	RS=0;          	
   	RW=0;           	
   	PORTD=combuf;       	
   	E=1;	            
   	asm("NOP");         
   	E=0;	           
}


void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();		
   	lcd_write_com(combuf); 	
}


void lcd_write_data(unsigned char databuf)
{  
	lcd_wait_busy();	
   	RS=1;          	
   	RW=0;           	
   	PORTD=databuf;      
   	E=1;	            
   	asm("NOP");
   	E=0;	            
}


void lcd_write_address(unsigned char x,unsigned char y)
{  
	x&=0x0f;			//ROW 0-15
   	y&=0x01;			//LINE 0-1
   	if(y==0x00)
      	lcd_write_com_busy(x|0x80);	   
   	else
      	lcd_write_com_busy((x+0x40)|0x80);	  
}


void lcdreset(void)
{  
	delay(150); 
	lcd_write_com(0x38);
   	delay(50); 
	lcd_write_com(0x38);
   	delay(50); 
	lcd_write_com(0x38);
   	lcd_write_com_busy(0x38);       
   	lcd_write_com_busy(0x08);		
   	lcd_write_com_busy(0x01);		
   	lcd_write_com_busy(0x06);		
   	lcd_write_com_busy(0x0c);		
}  	

	
void lcd_write_char(unsigned char x,unsigned char y,unsigned char buf)
{  		
	lcd_write_address(x,y); 			
   	lcd_write_data(buf);		 
} 




void interrupt init1(void)
{
	if(INT0IF==1)         
	{		
		a=10000;
		b=9999;        
	}	
	INT0IF=0;             
}


void main(void)
{	
	OSCCON=0X00;    		
	ADCON1=0X0f;            
	TRISB=0B11000111;   	
	TRISD=0B00000000;   	
	RBIE=0;     	   	  
	INTEDG0=0;                
  	INT0IE=1;  	           
  	GIE=1;                 
	lcdreset();	 			
	while(1)
  	{ 		
		if(++a>50000)
		{
			a=0;			
			if(++b>9999) b=0;
			lcd_write_char(0,0,0x30+b/1000); 			
			lcd_write_char(1,0,0x30+(b%1000)/100);  	
			lcd_write_char(2,0,0x30+(b%100)/10);  		
			lcd_write_char(3,0,0x30+b%10);  			
		}  			      	
  	}
}
