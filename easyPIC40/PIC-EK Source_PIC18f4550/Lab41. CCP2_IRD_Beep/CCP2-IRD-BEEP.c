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

#define beep   RD2 //����������ӿ�

unsigned char tmr_times;
unsigned char code_data;
unsigned char code;
unsigned char int_times;
unsigned char buf1;
unsigned int  fengcon;
bit head_ok;  	
bit beepbit;



void init_timer0(void)
{
	TMR0IF=0;    //T0�жϱ�־λ����
    TMR0IE=1;    //T0�ж�����
    T0CON=0xC8;  //8bit��ʱ��ʽ��Ԥ��Ƶ�رգ�	
	TMR0L=0x13;  //����4.0M,��ʱ250us
    GIE=1;   	 //�����ж�
}


void wrong(void)  //����д��󣬽���Ӧ�Ĵ������㣬׼����һ�ν���
{
  	int_times=0;
  	tmr_times=0;
  	head_ok=0;
  	code_data=0;  	
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
            		beepbit=1;            	  //�����Ϊ���룬˵���������ı���Ϊ��ȷ������������	
            	}      			
    		}
  		}   		
  		  		
  	} 
  	GIE=1;   	//����ȫ���ж�
}


void fengpro(void)
{
	if(beepbit==1)
	{		
		beep=1;
		if(++fengcon>1000)
		{
			fengcon=0;
			beepbit=0;			
		}		 	
	}	
	else
	{
		beep=0;
	}
}


main(void)
{
  	unsigned char buf;
	OSCCON=0X00;    		//ѡ��������
	ADCON1=0X0f;            //����IO��Ϊ���ֿڣ�ģ���������ֹ
	TRISC=0B11111111;   	//Rc����Ϊ�����
    TRISD=0B11111011;   	//RD2����Ϊ���
  	init_timer0();          //TO��ʼ������
  	ccp2_int();	            //CCP2��ʼ������  
	beepbit=1;	
  	while(1)
  	{ 
    	fengpro();	
  	}
}
