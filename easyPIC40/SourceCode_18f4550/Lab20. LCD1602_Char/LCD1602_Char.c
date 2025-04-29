/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : Connect the LCD1602 display module to the "LCD1602" socket directly
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

#define RSPIN  RA1 //Data or Instrument Select
#define RWPIN  RA2 //Write or Read
#define EPIN  RA3 //6800 mode Enable single



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
   	RSPIN=0;			   
   	RWPIN=1;			   //SELECT READ
   	EPIN=1;				   
   	while(RD7==1);         
   	EPIN=0;				   
   	TRISD7=0;
}


void lcd_write_com(unsigned char combuf)
{  
	RSPIN=0;          	
   	RWPIN=0;           	//SELECT WRITE
   	PORTD=combuf;         		
   	EPIN=1;	            
   	asm("NOP");
   	EPIN=0;	            
}


void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();				
   	lcd_write_com(combuf); 	
}


void lcd_write_data(unsigned char databuf)
{  
	lcd_wait_busy();	
   	RSPIN=1;          	
   	RWPIN=0;           	
   	PORTD=databuf;      	
   	EPIN=1;	            
   	asm("NOP");
   	EPIN=0;	          
}


void lcd_write_address(unsigned char x,unsigned char y)
{  
	x&=0x0f;			//0-15
   	y&=0x01;			//0-1
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



void main(void)
{	  
	OSCCON=0X00;    		
	ADCON1=0X0f;            	
  	TRISA=0B00000000;   	
	TRISD=0B00000000;   		   
	lcdreset();	 													
   	lcd_write_char(0,0,0x41);   //A
   	lcd_write_char(1,0,0x42);   //B
   	lcd_write_char(2,0,0x43);   //C
   	lcd_write_char(3,0,0x44);   //D
   	lcd_write_char(4,0,0x45);   //E
   	lcd_write_char(5,0,0x46);   //F
   	lcd_write_char(6,0,0x47);   //G
   	lcd_write_char(7,0,0x48);   //H
   	lcd_write_char(8,0,0x49);   //I
   	lcd_write_char(9,0,0x4A);   //J
   	lcd_write_char(10,0,0x4B);   //K
   	lcd_write_char(11,0,0x4C);   //L
   	lcd_write_char(12,0,0x4D);   //M
   	lcd_write_char(13,0,0x4E);   //N
   	lcd_write_char(14,0,0x4F);   //O
   	lcd_write_char(15,0,0x50);   //P
   	
   	lcd_write_char(0,1,0x61);   //a
   	lcd_write_char(1,1,0x62);   //b
   	lcd_write_char(2,1,0x63);   //c
   	lcd_write_char(3,1,0x64);   //d
   	lcd_write_char(4,1,0x65);   //e
   	lcd_write_char(5,1,0x66);   //f
   	lcd_write_char(6,1,0x67);   //g
   	lcd_write_char(7,1,0x68);   //h
   	lcd_write_char(8,1,0x69);   //i
   	lcd_write_char(9,1,0x6A);   //j
   	lcd_write_char(10,1,0x6B);   //k
   	lcd_write_char(11,1,0x6C);   //l
   	lcd_write_char(12,1,0x6D);   //m
   	lcd_write_char(13,1,0x6E);   //n
   	lcd_write_char(14,1,0x6F);   //o
   	lcd_write_char(15,1,0x70);   //p
	while(1)
	{		
			
	}  
}
