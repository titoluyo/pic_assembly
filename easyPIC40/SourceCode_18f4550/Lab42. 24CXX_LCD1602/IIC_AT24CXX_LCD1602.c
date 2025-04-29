/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : Connect the LCD1602 display module to the "LCD1602" socket directly
                        Disconnect the bit RC3 and RC4 on Jumper SW4,J5.24CXX-SDA----RB0,J5.24CXX-SCL----RB1,
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
#include <pic18.h>        //����ͷ�ļ�������ȥPICC18�����ȥ����PIC18F4550.H

__CONFIG(1,HS) ;  //EXT 20MHz,No PLL
__CONFIG(2,WDTDIS) ;      //WDT OFF
__CONFIG(3,PBADDIS) ;     //PORTB<4:0> configured as digital I/O on RESET 
__CONFIG(4,LVPDIS&XINSTDIS) ;     //Low Voltage Programming Disabled ,Disable extended instruction set (Legacy mode) 

#define RSPIN  RA1 //Data or Instrument Select
#define RWPIN  RA2 //Write or Read
#define EPIN  RA3 //6800 mode Enable single

#define sda   RB0
#define scl   RB1


bit eepromdi;
bit eepromdo;
unsigned char addressbuf;


void delay1(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<10;j++);
	}
}


void lcd_wait_busy(void)
{  
	TRISD7=1;	   			//Ϊ��״̬��׼��
   	RSPIN=0;			   //ѡ��ָ��Ĵ���
   	RWPIN=1;			   //ѡ���
   	EPIN=1;				   //ʹ���ߵ�ƽ�仯
   	while(RD7==1);         //��æ״̬����æʱ�˳�
   	EPIN=0;				   //�ָ�ʹ���ߵ�ƽ
   	TRISD7=0;
}

void lcd_write_com(unsigned char combuf)
{  
	RSPIN=0;          	//ѡ��ָ��Ĵ���
   	RWPIN=0;           	//ѡ��д
   	PORTD=combuf;         	//������������RD 	
   	EPIN=1;	            //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�
   	asm("NOP");
   	EPIN=0;	            //�ָ�ʹ���ߵ�ƽ
}

void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();				//����æ��⺯��
   	lcd_write_com(combuf); 	//����д�����
}

void lcd_write_data(unsigned char databuf)
{  
	lcd_wait_busy();	//����æ��⺯��
   	RSPIN=1;          	//ѡ�����ݼĴ���
   	RWPIN=0;           	//ѡ��д
   	PORTD=databuf;      	//������������P2
   	EPIN=1;	            //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�
   	asm("NOP");
   	EPIN=0;	            //�ָ�ʹ���ߵ�ƽ
}

void lcd_write_address(unsigned char x,unsigned char y)
{  
	x&=0x0f;			//�е�ַ������0-15
   	y&=0x01;			//�е�ַ������0-1
   	if(y==0x00)
      	lcd_write_com_busy(x|0x80);	   //��һ�е��е�ַд��
   	else
      	lcd_write_com_busy((x+0x40)|0x80);	   //�ڶ��е��е�ַд��
}

void lcdreset(void)
{  
	delay1(150); 
	lcd_write_com(0x38);
   	delay1(50); 
	lcd_write_com(0x38);
   	delay1(50); 
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
	lcd_write_address(x,y); //д���ַ			
   	lcd_write_data(buf);		//д����ʾ����    
} 
//---------------------------------------
//����: ��ʱ����	
//----------------------------------------- 
void delay(void)
{
	unsigned char k;
	for(k=0;k<180;k++);
}
//---------------------------------------
//����: ����I2C���ߺ���	
//----------------------------------------- 
void i2cstart(void)
{  	
  	scl=1;
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	sda=1;
  	delay();
  	sda=0;
  	delay();
  	scl=0;
  	delay();
}
//---------------------------------------
//����: ��ֹI2C���ߺ���	
//----------------------------------------- 
void i2cstop(void)
{
  	sda=0;
  	sda=0;  
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");	
  	scl=1;
  	delay();
  	sda=1;
  	delay();
  	scl=0;
  	delay();
}
//---------------------------------------
//����: ��I2C���߶���һλ���ݺ���	
//-----------------------------------------
void bitin(void)
{
  	eepromdi=1;  	
  	TRISB0=1;
  	scl=1;
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	eepromdi=sda;
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	scl=0;
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	TRISB0=0;
}
//---------------------------------------
//����: ��I2C����д��һλ���ݺ���	
//-----------------------------------------
void bitout(void)
{
  	sda=eepromdo;
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	scl=1;
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	scl=0;
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
  	asm("NOP");
}
//---------------------------------------
//����: ��I2C����д��һ�ֽں���	
//-----------------------------------------
void i2cwrite(unsigned char sedata)
{
  	unsigned char k;
  	for(k=0;k<8;k++)
  	{
    	if(sedata&0x80)
    	{
      		eepromdo=1;
    	}
    	else
    	{
      		eepromdo=0;
    	}
    	sedata=sedata<<1;
    	bitout();
  	}
  	bitin();
}
//---------------------------------------
//����: ��I2C���߶���һ�ֽں���	
//-----------------------------------------
unsigned char i2cread(void)
{
  	unsigned char redata;
  	unsigned char m;
  	for(m=0;m<8;m++)
  	{
    	redata=redata<<1;
    	bitin();
    	if(eepromdi==1)
    	{
      		redata|=0x01;
    	}
    	else
    	{
      		redata&=0xfe;
    	}
    	asm("NOP");
  	}
  	eepromdo=1;
  	bitout();
  	return redata;
}
//---------------------------------------
//����: ��AT24C02ָ����ַ�϶���1�ֽ����ݺ���	
//-----------------------------------------
unsigned char read(unsigned char address)
{
	unsigned char eebuf3;
  	i2cstart();
  	i2cwrite(0xa0);
  	i2cwrite(address);
  	i2cstart();
  	i2cwrite(0xa1);
  	eebuf3=i2cread();
  	i2cstop();
  	return eebuf3;
}
//---------------------------------------
//����: ��AT24C02ָ����ַ��д��1�ֽ����ݺ���	
//-----------------------------------------
void write(unsigned char address1,unsigned char byte)
{
  	i2cstart();
  	i2cwrite(0xa0);
  	i2cwrite(address1);
  	i2cwrite(byte);
  	i2cstop();
}
//---------------------------------------
//����: ������		 
//----------------------------------------- 
void main(void)
{
	unsigned char buf;
	OSCCON=0X00;    		//ѡ��������
	ADCON1=0X0f;            //����IO��Ϊ���ֿڣ�ģ���������ֹ
	TRISA=0B11000000;   	
   	TRISB=0B11000100;   	//RB3-5����Ϊ���
	TRISD=0B00000000;   	//RD����Ϊ���
	buf=read(0x18);        	//��AT24C02�е�ַ0X18��ȡ����
  	buf++;                 	//��������+1
  	write(0x18,buf);       	//��+1������ݱ��浽0X18��
	   
	lcdreset();	 								//��λ1602	    	 
  	lcd_write_char(0,0,'-');   //-
	lcd_write_char(1,0,'-');   //-
	lcd_write_char(2,0,'A');   //A
	lcd_write_char(3,0,'T');   //T
	lcd_write_char(4,0,'2');   //2
	lcd_write_char(5,0,'4');   //4   
	lcd_write_char(6,0,'C');   //C  	
	lcd_write_char(7,0,'0');   //0
	lcd_write_char(8,0,'2');   //2

	lcd_write_char(10,0,'T');  //T
	lcd_write_char(11,0,'E');  //E
	lcd_write_char(12,0,'S');  //S
	lcd_write_char(13,0,'T');  //T
	lcd_write_char(14,0,'-');   //-
	lcd_write_char(15,0,'-');   //-

	lcd_write_char(0,1,'B');   //B
	lcd_write_char(1,1,'o');   //o
	lcd_write_char(2,1,'o');   //o
	lcd_write_char(3,1,'t');   //t
   
	lcd_write_char(5,1,'t');   //t  	
	lcd_write_char(6,1,'i');   //i
	lcd_write_char(7,1,'m');   //m
	lcd_write_char(8,1,'e');   //e
	lcd_write_char(9,1,'s');   //s
	lcd_write_char(10,1,':');   //:
  
	lcd_write_char(12,1,buf/100+0x30);	    //��ʾ��λ
	lcd_write_char(13,1,(buf%100)/10+0x30); //��ʾʮλ
	lcd_write_char(14,1,buf%10+0x30);	    //��ʾ��λ
  	while(1)
  	{    	  		  	    	
  	}
}
