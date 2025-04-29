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
		for(j=0;j<10;j++)
		{
			asm("CLRWDT"); 
		}
	}
}


void lcd_wait_busy(void)
{  
	TRISD7=1;	   			
   	RSPIN=0;			   
   	RWPIN=1;			   
   	EPIN=1;				   
   	while(RD7==1);         
   	EPIN=0;				 
   	TRISD7=0;
}

void lcd_write_com(unsigned char combuf)
{  
	RSPIN=0;          	
   	RWPIN=0;           	
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
	x&=0x0f;			
   	y&=0x01;			
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
	unsigned int buf1;
	unsigned char buf2;
	OSCCON=0X00;    		
	ADCON1=0X0f;            	
  	TRISA=0B11000000;   	
	TRISD=0B00000000;   		   
	lcdreset();	 								
	buf2=0;		  	
	while(1)
	{		
		if(++buf1>50000)
    	{
    		buf1=0;
    		if(++buf2>50000) buf2=0;
    		lcd_write_char(0,0,buf2/10000+0x30);   			
			lcd_write_char(1,0,(buf2%10000)/1000+0x30);   	
			lcd_write_char(2,0,(buf2%1000)/100+0x30);   	
			lcd_write_char(3,0,(buf2%100)/10+0x30);   		
			lcd_write_char(4,0,(buf2%10)+0x30);   			
		}		
	}  
}
