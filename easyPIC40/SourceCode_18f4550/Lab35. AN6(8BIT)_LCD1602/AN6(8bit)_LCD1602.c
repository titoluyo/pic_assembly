/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : Connect the LCD1602 display module to the "LCD1602" socket directly
                        Connect the bit RE1 on Jumper SW3
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

unsigned char disbuf;

//---------------------------------------
//����: ��ʱ����		 
//-----------------------------------------
void delay(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<10;j++);
	}
}

//---------------------------------------
//����: 1602æ��⺯��		 
//----------------------------------------- 
void lcd_wait_busy(void)
{  
	TRISD7=1;	   		   //Ϊ��״̬��׼������RD7��Ϊ����
   	RSPIN=0;			   //ѡ��ָ��Ĵ���
   	RWPIN=1;			   //ѡ���
   	EPIN=1;				   //ʹ���ߵ�ƽ�仯
   	while(RD7==1);         //��æ״̬����æʱ�˳�
   	EPIN=0;				   //�ָ�ʹ���ߵ�ƽ
   	TRISD7=0;			   //��RD7����Ϊ���
}
//---------------------------------------
//����: 1602д�����		 
//-----------------------------------------
void lcd_write_com(unsigned char combuf)
{  
	RSPIN=0;          	//ѡ��ָ��Ĵ���
   	RWPIN=0;           	//ѡ��д
   	PORTD=combuf;       //������������RD 	
   	EPIN=1;	            //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�
   	asm("NOP");         //��һ���ղ���������ʱƬ��
   	EPIN=0;	            //�ָ�ʹ���ߵ�ƽ
}
//---------------------------------------
//����: 1602д���������æ��⣩		 
//-----------------------------------------
void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();		//����æ��⺯��
   	lcd_write_com(combuf); 	//����д�����
}
//---------------------------------------
//����: 1602д���ݺ�������æ��⣩		 
//-----------------------------------------
void lcd_write_data(unsigned char databuf)
{  
	lcd_wait_busy();	//����æ��⺯��
   	RSPIN=1;          	//ѡ�����ݼĴ���
   	RWPIN=0;           	//ѡ��д
   	PORTD=databuf;      //������������RD��
   	EPIN=1;	            //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�
   	asm("NOP");
   	EPIN=0;	            //�ָ�ʹ���ߵ�ƽ
}
//---------------------------------------
//����: 1602��ʾ��ַд����		 
//-----------------------------------------
void lcd_write_address(unsigned char x,unsigned char y)
{  
	x&=0x0f;			//�е�ַ������0-15
   	y&=0x01;			//�е�ַ������0-1
   	if(y==0x00)
      	lcd_write_com_busy(x|0x80);	   //��һ�е��е�ַд��
   	else
      	lcd_write_com_busy((x+0x40)|0x80);	   //�ڶ��е��е�ַд��
}
//---------------------------------------
//����: 1602��ʼ������		 
//-----------------------------------------
void lcdreset(void)
{  
	delay(150); 
	lcd_write_com(0x38);
   	delay(50); 
	lcd_write_com(0x38);
   	delay(50); 
	lcd_write_com(0x38);
   	lcd_write_com_busy(0x38);       //8λ���ݣ�˫�У�5*7����
   	lcd_write_com_busy(0x08);		//��ʾ���ܹأ��޹��
   	lcd_write_com_busy(0x01);		//����ָ��
   	lcd_write_com_busy(0x06);		//д���µ����ݺ󣬹�����ƣ���ʾ�����ƶ�
   	lcd_write_com_busy(0x0c);		//��ʾ���ܿ����޹�꣬
}  	
//---------------------------------------
//����: ָ����ַд�뺯��		 
//-----------------------------------------	
void lcd_write_char(unsigned char x,unsigned char y,unsigned char buf)
{  		
	lcd_write_address(x,y); 	//д���ַ			
   	lcd_write_data(buf);		//д����ʾ����    
} 
//---------------------------------------
//����: ������
//----------------------------------------- 
main(void)
{
	OSCCON=0X00;    		//ѡ��������
	ADCON1=0X0f;            //����IO��Ϊ���ֿڣ�ģ���������ֹ
	TRISA=0B11110001;   	//����ΪRA1-RA3�����
  	TRISB=0B11000111;   	//RB3-5����Ϊ���
	TRISD=0B00000000;   	//RD����Ϊ���  
	TRISE=0B00000111;   	//RE0-2����Ϊ�����

//	PCFG0=1;	            //PCFG3-PCFG0=0111����AN0-AN7��ΪAD�����
//	PCFG1=1;	            //PCFG3-PCFG0=0111����AN0-AN7��ΪAD�����
//	PCFG2=1;	            //PCFG3-PCFG0=0111����AN0-AN7��ΪAD�����
//	PCFG3=0;	            //PCFG3-PCFG0=0111����AN0-AN7��ΪAD�����
	ADFM=0;                 //ת�����������ƣ���ADRESH������Ϊ��8λ����
    ADIE=0;		            //��ֹAD�ж�  
	lcdreset();             //��ʼ��1602
	lcd_write_char(2,0,'-');   	
	lcd_write_char(3,0,'-');   		
	lcd_write_char(4,0,'A'); 
	lcd_write_char(5,0,'N');   	
	lcd_write_char(6,0,'6'); 
 		
	lcd_write_char(8,0,'T');
	lcd_write_char(9,0,'E');   	
	lcd_write_char(10,0,'S');   		
	lcd_write_char(11,0,'T');
	lcd_write_char(12,0,'-');   	
	lcd_write_char(13,0,'-');
  	while(1)
  	{    	 	
		ADCON0=0x19;		//ѡ��AN6��
		asm("NOP");         //������ʱ
		asm("NOP");
		asm("NOP");
		asm("NOP");
		GODONE = 1;
		while(GODONE)	continue;		//GODONEΪ0��ADת������
    	disbuf=ADRESH; 
		lcd_write_char(5,1,'A');   	
		lcd_write_char(6,1,'D');   		
		lcd_write_char(7,1,'=');  
    	lcd_write_char(8,1,(disbuf/100)+0x30);   	//��λ
		lcd_write_char(9,1,(disbuf%100)/10+0x30);   //ʮλ
		lcd_write_char(10,1,(disbuf%10)+0x30);       //��λ	
  	}
}
