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
//名称: 延时函数		 
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
//名称: 1602忙检测函数		 
//----------------------------------------- 
void lcd_wait_busy(void)
{  
	TRISD7=1;	   		   //为读状态做准备，把RD7设为输入
   	RSPIN=0;			   //选择指令寄存器
   	RWPIN=1;			   //选择读
   	EPIN=1;				   //使能线电平变化
   	while(RD7==1);         //读忙状态，不忙时退出
   	EPIN=0;				   //恢复使能线电平
   	TRISD7=0;			   //把RD7设置为输出
}
//---------------------------------------
//名称: 1602写命令函数		 
//-----------------------------------------
void lcd_write_com(unsigned char combuf)
{  
	RSPIN=0;          	//选择指令寄存器
   	RWPIN=0;           	//选择写
   	PORTD=combuf;       //把命令字送入RD 	
   	EPIN=1;	            //使能线电平变化，命令送入1602的8位数据口
   	asm("NOP");         //来一个空操作，以延时片刻
   	EPIN=0;	            //恢复使能线电平
}
//---------------------------------------
//名称: 1602写命令函数（带忙检测）		 
//-----------------------------------------
void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();		//调用忙检测函数
   	lcd_write_com(combuf); 	//调用写命令函数
}
//---------------------------------------
//名称: 1602写数据函数（带忙检测）		 
//-----------------------------------------
void lcd_write_data(unsigned char databuf)
{  
	lcd_wait_busy();	//调用忙检测函数
   	RSPIN=1;          	//选择数据寄存器
   	RWPIN=0;           	//选择写
   	PORTD=databuf;      //把数据字送入RD口
   	EPIN=1;	            //使能线电平变化，命令送入1602的8位数据口
   	asm("NOP");
   	EPIN=0;	            //恢复使能线电平
}
//---------------------------------------
//名称: 1602显示地址写函数		 
//-----------------------------------------
void lcd_write_address(unsigned char x,unsigned char y)
{  
	x&=0x0f;			//列地址限制在0-15
   	y&=0x01;			//行地址限制在0-1
   	if(y==0x00)
      	lcd_write_com_busy(x|0x80);	   //第一行的列地址写入
   	else
      	lcd_write_com_busy((x+0x40)|0x80);	   //第二行的列地址写入
}
//---------------------------------------
//名称: 1602初始化函数		 
//-----------------------------------------
void lcdreset(void)
{  
	delay(150); 
	lcd_write_com(0x38);
   	delay(50); 
	lcd_write_com(0x38);
   	delay(50); 
	lcd_write_com(0x38);
   	lcd_write_com_busy(0x38);       //8位数据，双列，5*7字形
   	lcd_write_com_busy(0x08);		//显示功能关，无光标
   	lcd_write_com_busy(0x01);		//清屏指令
   	lcd_write_com_busy(0x06);		//写入新的数据后，光标右移，显示屏不移动
   	lcd_write_com_busy(0x0c);		//显示功能开，无光标，
}  	
//---------------------------------------
//名称: 指定地址写入函数		 
//-----------------------------------------	
void lcd_write_char(unsigned char x,unsigned char y,unsigned char buf)
{  		
	lcd_write_address(x,y); 	//写入地址			
   	lcd_write_data(buf);		//写入显示数据    
} 
//---------------------------------------
//名称: 主函数
//----------------------------------------- 
main(void)
{
	OSCCON=0X00;    		//选择主震荡器
	ADCON1=0X0f;            //所有IO均为数字口，模拟量输入禁止
	TRISA=0B11110001;   	//设置为RA1-RA3输出入
  	TRISB=0B11000111;   	//RB3-5设置为输出
	TRISD=0B00000000;   	//RD设置为输出  
	TRISE=0B00000111;   	//RE0-2设置为输出入

//	PCFG0=1;	            //PCFG3-PCFG0=0111，则AN0-AN7均为AD输入口
//	PCFG1=1;	            //PCFG3-PCFG0=0111，则AN0-AN7均为AD输入口
//	PCFG2=1;	            //PCFG3-PCFG0=0111，则AN0-AN7均为AD输入口
//	PCFG3=0;	            //PCFG3-PCFG0=0111，则AN0-AN7均为AD输入口
	ADFM=0;                 //转换后数据左移，则ADRESH内数据为高8位数据
    ADIE=0;		            //禁止AD中断  
	lcdreset();             //初始化1602
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
		ADCON0=0x19;		//选择AN6，
		asm("NOP");         //略作延时
		asm("NOP");
		asm("NOP");
		asm("NOP");
		GODONE = 1;
		while(GODONE)	continue;		//GODONE为0则AD转换结束
    	disbuf=ADRESH; 
		lcd_write_char(5,1,'A');   	
		lcd_write_char(6,1,'D');   		
		lcd_write_char(7,1,'=');  
    	lcd_write_char(8,1,(disbuf/100)+0x30);   	//百位
		lcd_write_char(9,1,(disbuf%100)/10+0x30);   //十位
		lcd_write_char(10,1,(disbuf%10)+0x30);       //个位	
  	}
}
