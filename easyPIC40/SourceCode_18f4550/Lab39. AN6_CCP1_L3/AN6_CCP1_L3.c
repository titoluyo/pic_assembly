/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : Connect the bit RE1 on Jumper SW3
                        CN1.2---RC2
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


//---------------------------------------
//����: ������
//----------------------------------------- 
main(void)
{
	OSCCON=0X00;    		//ѡ��������
	ADCON1=0X0f;            //����IO��Ϊ���ֿڣ�ģ���������ֹ
	TRISA=0B11111111;   	//δ������Ϊ����
  	TRISC=0B11111011;   	//RC2����Ϊ���
    PORTC=0B00000000;   	//RC2����Ϊ���
	PCFG0=1;	            //PCFG3-PCFG0=0111����AN0-AN7��ΪAD�����
	PCFG1=1;	            //PCFG3-PCFG0=0111����AN0-AN7��ΪAD�����
	PCFG2=1;	            //PCFG3-PCFG0=0111����AN0-AN7��ΪAD�����
	PCFG3=0;	            //PCFG3-PCFG0=0111����AN0-AN7��ΪAD�����
	ADFM=0;                 //ת�����������ƣ���ADRESH������Ϊ��8λ����
    ADIE=0;		            //��ֹAD�ж� 
	CCP1CON=0x3c;	        //����CCP1ΪPWMģʽ
	PR2=63;              	//Ƶ��15.625KHZ
	T2CKPS1=0;
	T2CKPS0=0;           	//ǰ��ƵΪ1��1
	TMR2ON=1;	         	//����TIMER2	
  	while(1)
  	{    


		ADCON0=0x19;		//ѡ��AN6��
		asm("NOP");         //������ʱ
		asm("NOP");
		asm("NOP");
		asm("NOP");
		GODONE = 1;
		while(GODONE)	continue;		//GODONEΪ0��ADת������
    	CCPR1L=ADRESH; 			//д��ռ�ձȲ���			
  	}
}
