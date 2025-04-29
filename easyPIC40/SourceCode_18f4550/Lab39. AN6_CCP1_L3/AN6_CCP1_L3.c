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
#include <pic18.h>        //调用头文件，可以去PICC18软件下去查找PIC18F4550.H

__CONFIG(1,HS) ;  //EXT 20MHz,No PLL
__CONFIG(2,WDTDIS) ;      //WDT OFF
__CONFIG(3,PBADDIS) ;     //PORTB<4:0> configured as digital I/O on RESET 
__CONFIG(4,LVPDIS&XINSTDIS) ;     //Low Voltage Programming Disabled ,Disable extended instruction set (Legacy mode) 


//---------------------------------------
//名称: 主函数
//----------------------------------------- 
main(void)
{
	OSCCON=0X00;    		//选择主震荡器
	ADCON1=0X0f;            //所有IO均为数字口，模拟量输入禁止
	TRISA=0B11111111;   	//未用设置为输入
  	TRISC=0B11111011;   	//RC2设置为输出
    PORTC=0B00000000;   	//RC2设置为输出
	PCFG0=1;	            //PCFG3-PCFG0=0111，则AN0-AN7均为AD输入口
	PCFG1=1;	            //PCFG3-PCFG0=0111，则AN0-AN7均为AD输入口
	PCFG2=1;	            //PCFG3-PCFG0=0111，则AN0-AN7均为AD输入口
	PCFG3=0;	            //PCFG3-PCFG0=0111，则AN0-AN7均为AD输入口
	ADFM=0;                 //转换后数据左移，则ADRESH内数据为高8位数据
    ADIE=0;		            //禁止AD中断 
	CCP1CON=0x3c;	        //设置CCP1为PWM模式
	PR2=63;              	//频率15.625KHZ
	T2CKPS1=0;
	T2CKPS0=0;           	//前分频为1：1
	TMR2ON=1;	         	//启动TIMER2	
  	while(1)
  	{    


		ADCON0=0x19;		//选择AN6，
		asm("NOP");         //略作延时
		asm("NOP");
		asm("NOP");
		asm("NOP");
		GODONE = 1;
		while(GODONE)	continue;		//GODONE为0则AD转换结束
    	CCPR1L=ADRESH; 			//写入占空比参数			
  	}
}
