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
#include <pic18.h>        //����ͷ�ļ�������ȥPICC18�����ȥ����PIC18F4550.H

__CONFIG(1,HS) ;  //EXT 20MHz,No PLL
__CONFIG(2,WDTDIS) ;      //WDT OFF
__CONFIG(3,PBADDIS) ;     //PORTB<4:0> configured as digital I/O on RESET 
__CONFIG(4,LVPDIS&XINSTDIS) ;     //Low Voltage Programming Disabled ,Disable extended instruction set (Legacy mode) 


#define RS  RB5 //Data or Instrument Select
#define RW  RB4 //Write or Read
#define E  RB3 //6800 mode Enable single
volatile bit I2Cerror;    //I2C�����־
unsigned char timebuf[13];


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
	TRISD7=1;	   			//Ϊ��״̬��׼��
   	RS=0;			   //ѡ��ָ��Ĵ���
   	RW=1;			   //ѡ���
   	E=1;				   //ʹ���ߵ�ƽ�仯
   	while(RD7==1);         //��æ״̬����æʱ�˳�
   	E=0;				   //�ָ�ʹ���ߵ�ƽ
   	TRISD7=0;
}

void lcd_write_com(unsigned char combuf)
{  
	RS=0;          	//ѡ��ָ��Ĵ���
   	RW=0;           	//ѡ��д
   	PORTD=combuf;         	//������������RD 	
   	E=1;	            //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�
   	asm("NOP");
   	E=0;	            //�ָ�ʹ���ߵ�ƽ
}

void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();				//����æ��⺯��
   	lcd_write_com(combuf); 	//����д�����
}

void lcd_write_data(unsigned char databuf)
{  
	lcd_wait_busy();	//����æ��⺯��
   	RS=1;          	//ѡ�����ݼĴ���
   	RW=0;           	//ѡ��д
   	PORTD=databuf;      	//������������P2
   	E=1;	            //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�
   	asm("NOP");
   	E=0;	            //�ָ�ʹ���ߵ�ƽ
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
	
void lcd_write_char(unsigned char x,unsigned char y,unsigned char buf)
{  		
	lcd_write_address(x,y); //д���ַ			
   	lcd_write_data(buf);		//д����ʾ����    
} 

void IICint(void)
{   	
	SSPCON1=0X08;               //I2C���ط�ʽ��ʱ��=Fosc/[4*(SSPADD+1)]
  	TRISB0=1;                   //����SDAΪ�����
  	TRISB1=1;                   //����SCLΪ�����  	
	SSPADD=0X09;                //����IICʱ��Ƶ��100KHZ
  	SSPSTAT=0X80;               //��ʼ��SSPSTAT�Ĵ���  	
  	SSPCON2=0X00;               //��ʼ��SSPCON2�Ĵ���
  	SSPEN=1;  	
	I2Cerror=0;  			   	
}
//***********************************
//I2C�ظ�����
//***********************************
void Restart()
{
	RSEN=1;
	while(1)
	{   	
		if(SSPIF==1) break;
	}
	SSPIF=0;
}
//***********************************
//I2C��ʼλ���ͺ���
//***********************************
void Start()
{
	unsigned char i=0;
	SEN=1;            //��ʼλ����
	while(1)
	{
    	i++;
    	if(SSPIF==1) break;
    	if(i==200)      //��ֹ��ѭ��
    	{
      		i=0;
      		I2Cerror=1;   
      		break; 
    	}
	}
	SSPIF=0;
}
//***********************************
//I2Cֹͣλ����
//***********************************
void Stop()
{
	PEN=1;
	while(1)
	{   	
		if(SSPIF==1) break;
	}
	SSPIF=0;
}
//***********************************
//I2C���ݷ��ͺ���
//***********************************
void Senddata(unsigned char data1)
{
	unsigned char i;
	SSPBUF=data1;            //��������
	while(1)
	{
    	if(SSPIF==1) break;
	}
	SSPIF=0;
	for(i=0;i<10;i++);

	if(ACKSTAT)   //���û�з���Ӧ���ź�
	{
    	I2Cerror=1;
	}
	else          //�������Ӧ���ź�
	{
    	I2Cerror=0;
	}
}
//***********************************
//I2C����д�ȴ�
//***********************************
void WriteWait()
{
	while(1)
	{
   		Start();           //������д��ʱ�䣬����ڸ������������ݲ�������Ӧ
   		Senddata(0xa0);
   		Stop();
   		if(I2Cerror==0) break; //�ȴ�����д�룬=0����Ӧ��λ����
	}
}

void write(unsigned char address1,unsigned char byte)
{
    Start();
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	return;         //���ط�ֹ��ѭ��
	}
	Senddata(0xa2);   //����Ѱַ��ַ���Լ�д������ַ��
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	Stop();         //������
    	return;         //���ط�ֹ��ѭ��
	}
	Senddata(address1);   //���͵�ַ
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	Stop();         //������
    	return;         //���ط�ֹ��ѭ��
	}	
	Senddata(byte);     //�������� 
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	Stop();         //������
    	return;         //���ط�ֹ��ѭ��
	}
	Stop();
}

unsigned char read(unsigned char address)
{
	unsigned char bytebuf;
	Start();
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	return 0xff;         //���ط�ֹ��ѭ��
	}
	Senddata(0xa2);   //����Ѱַ��ַ���Լ�д������ַ��
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	Stop();         //������
    	return 0xff;         //���ط�ֹ��ѭ��
	}
	Senddata(address);   //���͵�ַ
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	Stop();         //������
    	return 0xff;         //���ط�ֹ��ѭ��
	}
	Restart();         //����������ʼλ
	Senddata(0xa3);    //���Ͷ�����
	if(I2Cerror==1)
	{
   		I2Cerror=0;
   		Stop();         //������
   		return 0xff;         //���ط�ֹ��ѭ��
	}
	RCEN=1;
	while(1)
	{   	
		if(SSPIF==1) break;
	}
	SSPIF=0;
	bytebuf=SSPBUF;      //����
	ACKDT=1;          //���ͷ�Ӧ��λ
	ACKEN=1;
	while(1)
	{   	
		if(SSPIF==1) break;
	}
	SSPIF=0;
	Stop();
    
    return bytebuf;
}

void main(void)
{
	unsigned char buf;
	OSCCON=0X00;    		//ѡ��������
	ADCON1=0X0f;            //����IO��Ϊ���ֿڣ�ģ���������ֹ
  	TRISB=0B11000111;   	//RB3-5����Ϊ���
	TRISD=0B00000000;   	//RD����Ϊ���  	
  	IICint();               //����IIC��ʼ������    	
	write(0x00,0x00);
	write(0x01,0x00);
  	write(0x02,0x00);				//�趨��
  	write(0x03,0x55);				//�趨��
  	write(0x04,0x20);				//�趨ʱ
  	write(0x05,0x01);				//�趨��
  	write(0x06,0x00);				//�趨����
  	write(0x07,0x02);				//�趨��
  	write(0x08,0x11);				//�趨��  	
  	lcdreset();	 						//��λ1602	   	
  	while(1)
  	{    	
		delay(150);
		buf=read(0x08);        		 //����Ĵ���
		lcd_write_char(0,0,'2');   //2 
		lcd_write_char(1,0,'0');   //0		 	
		lcd_write_char(2,0,((buf>>4)&0x0f)+'0'); 
		lcd_write_char(3,0,(buf&0x0f)+'0');
		lcd_write_char(4,0,'.');   //.	
		buf=read(0x07);        		 //���¼Ĵ���
		lcd_write_char(5,0,((buf>>4)&0x01)+'0'); 
		lcd_write_char(6,0,(buf&0x0f)+'0');
		lcd_write_char(7,0,'.');   //.
		buf=read(0x05);        		 //���ռĴ���
		lcd_write_char(8,0,((buf>>4)&0x03)+'0'); 
		lcd_write_char(9,0,(buf&0x0f)+'0');
		buf=read(0x04);        		 //��ʱ�Ĵ���
	  	lcd_write_char(0,1,((buf>>4)&0x03)+'0'); 
		lcd_write_char(1,1,(buf&0x0f)+'0');
		lcd_write_char(2,1,':');   //:
		buf=read(0x03);        		 //���ּĴ���
    	lcd_write_char(3,1,((buf>>4)&0x0f)+'0'); 
		lcd_write_char(4,1,(buf&0x0f)+'0');
		lcd_write_char(5,1,':');   //:
		buf=read(0x02);        		 //����Ĵ���
    	lcd_write_char(6,1,((buf>>4)&0x07)+'0'); 
		lcd_write_char(7,1,(buf&0x0f)+'0');			  	    	
  	}
}
