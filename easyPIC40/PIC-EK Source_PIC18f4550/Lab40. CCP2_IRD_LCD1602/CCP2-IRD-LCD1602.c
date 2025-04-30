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

unsigned char tmr_times;
unsigned char code_data;
unsigned char code;
unsigned char int_times;
unsigned char buf1;
bit head_ok;  	
bit code_ok;
bit clrbit;


void init_timer0(void)
{
    TMR0IF=0;    
    TMR0IE=1;    
    T0CON=0xC8;  //8bit	
	TMR0L=0x13;  //4.0M,250us
    GIE=1;   	 
}


void wrong(void)  
{
  	int_times=0;
  	tmr_times=0;
  	head_ok=0;
  	code_data=0;  	
  	code_ok=0;
}


unsigned char one_zero(void)
{  	
	//�ο�ң����оƬHT6221��˵���飬0�������������Ϊ1.12ms
	//�����У�ʱ���ж�Ϊ0.25ms,���������ж�ȡ����0.75msС��1.5ms�������Ϊ0����
  	if((tmr_times>3)&(tmr_times<6))
  	{
    	return 0;
  	}
	//�ο�ң����оƬHT6221��˵���飬1�������������Ϊ2.24ms
	//�����У�ʱ���ж�Ϊ0.25ms,���������ж�ȡ����1.75msС��2.75ms�������Ϊ0����
  	else if((tmr_times>7)&(tmr_times<11))
  	{
    	return 1;
  	}
	//����������ڲ���0.75-1.5ms����1.75-2.75ms����ģ�������Ϊ�Ǵ��룬����2
  	return 2;
}


void ccp2_int(void)
{
	CCP2CON=0x04;			//��׽ģʽ����׽ÿ���½���
	T1CON=0x00;				//CCPҪʹ�ò�׽���ԣ�T1���������ڶ�ʱ��������ͬ��������ģʽ��
	TRISC1=1;               //RC1����Ϊ����״̬�������Žӵ��˺������ͷ������������
	CCP2IF=0;               //��׽�жϱ�־λ����
	CCP2IE=1;				//�ò�׽�����ж�
	TMR1ON=1;	            //T1����
}


void interrupt ISR(void)    //�жϴ�����
{	
		
  	if(TMR0IF==1)           //�����T0�жϱ�־λΪ1����250USʱ�䵽�������ж�
  	{
		TMR0L=0x13;  //����4.0M,��ʱ250us
  		TMR0IF=0;           //���TO�жϱ�־λ  		
  		tmr_times++;	    //��¼�жϵĴ��������ݴ����жϽ��ձ����״̬

  	}
	 if(CCP2IF==1)          //�ⲿ�жϱ�־λ���Ϊ1����ΪCCP2��׽�жϣ�ת�봦�����
	 {
		CCP2IF=0;            //���CCP2��׽�жϱ�־λ   
		 
		GIE=0; 				 //ȫ���жϹص����Ա��������̽��������ж�	 
  		int_times++;  		 //��׽�жϼƴ�	
  		if(head_ok==0)      	//�����û���յ�ͷ�룬��ô���ж���������Ƿ�Ϊͷ��
  		{
			//�ο�ң����оƬHT6221��˵���飬��������ͷ����һ��9ms����ʼ���һ��4.5ms�Ľ�������
			//�����У�ʱ���ж�Ϊ0.25ms,���������ж�����12.5msС��14.5ms�������Ϊ13.5ms��ͷ�����
    		if(tmr_times>45&tmr_times<58)		
    		{
      			head_ok=1;  	//��Чͷ���־λ��һ����ʾͷ��������
      			int_times=0;    //�ⲿ�жϼƴμĴ������㣬��׼�����պ����4�鹲32������
      			tmr_times=0;    //��ʱ���жϼƴμĴ������㣬׼����ʼ�Ժ���������ʱ

    		}
    		else
    		{

      			wrong();        //���������Ч��ͷ�룬ת����������
    		}
  		}
  		else                	//����Ѿ����յ�ͷ�룬��ô��ʼ�Ժ����32��������н���                    
  		{    

    		code_data=code_data>>1; //�����ջ���������һλ����׼��������һλ
			buf1=one_zero();    		//�жϱ�����0��1�������Ǵ���
			tmr_times=0;  		    //ʱ���жϼƴμĴ�������			
    		if(buf1==1)			//����Ǳ���1
    		{
      			code_data|=0x80;  	//��1����code_data�����λ
    		}    	
			else if(buf1==0)			//����Ǳ���0
	        {
      			code_data&=0x7f;  	//��0����code_data�����λ
    		}    
			else            		//�����0��1����Ϊ���룬
            {
				wrong();       		//���ô��������
				return;
			}    						
    		if(int_times==8)        //����յ���8���������壬��ζ�ŵ�8λ��ַ���Ѿ�������
    		{
				//��ң�����ĵ�8λ��ַ��Ϊ0X00�������ң�������Ҿ����ģ����Ҫ��������������
        		if(code_data!=0x00)  //�������0x00 ��˵�����������ṩ��ң���������ǲ���ʶ��
        		{
        			wrong();         //���ô��������      
					return;  
        		}    		
    		}
    		else if(int_times==16)        //����յ���16���������壬��ζ�Ÿ�8λ��ַ���Ѿ�������
    		{
				//��ң�����ĵ͸�8λ��ַ��Ϊ0XFF
        		if(code_data!=0xFF)  //�������0xff ��˵�����������ṩ��ң���������ǲ���ʶ��
        		{
            		wrong();         //���ô��������      
					return;
        		}	        			
    		}
    		else if(int_times==24)    //�յ��ĵ�����8λ�ֽڼ�Ϊ8λ������
    		{
      			code=code_data;    //�����8λ�������ݴ���code_data1  			
    		}
    		else if(int_times==32)    //�յ��ĵ�����8λ�ֽڼ�Ϊ8λ������ķ���
    		{
      			int_times=0;      	  //�����жϴ����Ĵ������Ա��´δ�ͷ��ʼ		
      			head_ok=0;            //��ͷ���־λ���㣬�Ա��´��ٽ���ʱ����ͷ�뿪ʼ			 			  
      			if((code+code_data)==0xff)    //�жϵ��������ݺ͵����������Ƿ�Ϊ����  			
      			{          			
            		code_ok=1;            	  //�����Ϊ���룬˵���������ı���Ϊ��ȷ����λ��־λ����֪ͨҺ����ʾ	
            	}      			
    		}
  		}   		
  		  		
  	} 
  	GIE=1;   	//����ȫ���ж�
}


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
	TRISD7=1;	   		   //Ϊ��״̬��׼������RD7��Ϊ����
   	RS=0;			   //ѡ��ָ��Ĵ���
   	RW=1;			   //ѡ���
   	E=1;				   //ʹ���ߵ�ƽ�仯
   	while(RD7==1);         //��æ״̬����æʱ�˳�
   	E=0;				   //�ָ�ʹ���ߵ�ƽ
   	TRISD7=0;			   //��RD7����Ϊ���
}


void lcd_write_com(unsigned char combuf)
{  
	RS=0;          	//ѡ��ָ��Ĵ���
   	RW=0;           	//ѡ��д
   	PORTD=combuf;       //������������RD 	
   	E=1;	            //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�
   	asm("NOP");         //��һ���ղ���������ʱƬ��
   	E=0;	            //�ָ�ʹ���ߵ�ƽ
}


void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();		//����æ��⺯��
   	lcd_write_com(combuf); 	//����д�����
}


void lcd_write_data(unsigned char databuf)
{  
	lcd_wait_busy();	//����æ��⺯��
   	RS=1;          	//ѡ�����ݼĴ���
   	RW=0;           	//ѡ��д
   	PORTD=databuf;      //������������RD��
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
	lcd_write_address(x,y); 	//д���ַ			
   	lcd_write_data(buf);		//д����ʾ����    
} 




main(void)
{
  	unsigned char buf;
	OSCCON=0X00;    		//ѡ��������
	ADCON1=0X0f;            //����IO��Ϊ���ֿڣ�ģ���������ֹ
	TRISB=0B11000111;   	//RB3-5����Ϊ���
	TRISC=0B01111111;   	//RD����Ϊ���
    TRISD=0B00000000;   	//RD����Ϊ���
  	init_timer0();          //TO��ʼ������
  	ccp2_int();	            //CCP2��ʼ������
  	lcdreset();	 				//��λ1602	  
    lcd_write_char(0,0,'P');   //P
	lcd_write_char(1,0,'r');   //r
	lcd_write_char(2,0,'e');   //e
	lcd_write_char(3,0,'s');   //s
	lcd_write_char(4,0,'s');   //s

	lcd_write_char(6,0,'R');   //R
	lcd_write_char(7,0,'e');   //e
	lcd_write_char(8,0,'m');   //m
	lcd_write_char(9,0,'o');   //o
	lcd_write_char(10,0,'t');  //t
	lcd_write_char(11,0,'e');  //e
	
	lcd_write_char(13,0,'c');   //c
	lcd_write_char(14,0,'o');   //o
	lcd_write_char(15,0,'n');   //n
	lcd_write_char(0,1,'t');    //t
	lcd_write_char(1,1,'r');    //r
	lcd_write_char(2,1,'o');    //o
	lcd_write_char(3,1,'l');    //l	 
	clrbit=1;	                 //��λ������־λ���Ա��ڵ�һ����ʾ��������ʱ������������ʾ�ַ������
  	while(1)
  	{ 
    	if(code_ok==1)         //������յ�����Ч�ص�ң����������
    	{
    		code_ok=0;         //����ñ�־λ
    		if(clrbit)		   //���Ϊ��һ����ʾң������������
			{
				clrbit=0;    
				lcdreset();    //��һ����Ļ
			}
    		lcd_write_char(0,0,'C');  //C
			lcd_write_char(1,0,'O');  //O
			lcd_write_char(2,0,'D');  //D
			lcd_write_char(3,0,'E');  //E
			lcd_write_char(4,0,':');  //:

			lcd_write_char(6,0,'0');  //0
			lcd_write_char(7,0,'X');  //X
			buf=code>>4;               //��ʾ���������4λ
			if(buf<10)                 //���С��10
			{
				buf+=0x30;			   //ֱ�Ӽ�0x30,1602������ʾ������
			}
			else                       //�����Լ�����10,
			{
            	buf+=0x37;			   //ֱ�Ӽ�0x37,1602������ʾ������
			}
			lcd_write_char(8,0,buf);  //��ʾ�������4λ
			buf=code&0x0f;
			if(buf<10)                 //���С��10
			{
				buf+=0x30;			   //ֱ�Ӽ�0x30,1602������ʾ������
			}
			else                       //�����Լ�����10,
			{
            	buf+=0x37;			   //ֱ�Ӽ�0x37,1602������ʾ������
			}
			lcd_write_char(9,0,buf);  //��ʾ�������4λ					
      	}    			  	        	
  	}
}
