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
#include <pic18.h>        //调用头文件，可以去PICC18软件下去查找PIC18F4550.H

__CONFIG(1,HS) ;  //EXT 20MHz,No PLL
__CONFIG(2,WDTDIS) ;      //WDT OFF
__CONFIG(3,PBADDIS) ;     //PORTB<4:0> configured as digital I/O on RESET 
__CONFIG(4,LVPDIS&XINSTDIS) ;     //Low Voltage Programming Disabled ,Disable extended instruction set (Legacy mode) 


#define RS  RB5 //Data or Instrument Select
#define RW  RB4 //Write or Read
#define E  RB3 //6800 mode Enable single
volatile bit I2Cerror;    //I2C错误标志
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
	TRISD7=1;	   			//为读状态做准备
   	RS=0;			   //选择指令寄存器
   	RW=1;			   //选择读
   	E=1;				   //使能线电平变化
   	while(RD7==1);         //读忙状态，不忙时退出
   	E=0;				   //恢复使能线电平
   	TRISD7=0;
}

void lcd_write_com(unsigned char combuf)
{  
	RS=0;          	//选择指令寄存器
   	RW=0;           	//选择写
   	PORTD=combuf;         	//把命令字送入RD 	
   	E=1;	            //使能线电平变化，命令送入1602的8位数据口
   	asm("NOP");
   	E=0;	            //恢复使能线电平
}

void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();				//调用忙检测函数
   	lcd_write_com(combuf); 	//调用写命令函数
}

void lcd_write_data(unsigned char databuf)
{  
	lcd_wait_busy();	//调用忙检测函数
   	RS=1;          	//选择数据寄存器
   	RW=0;           	//选择写
   	PORTD=databuf;      	//把数据字送入P2
   	E=1;	            //使能线电平变化，命令送入1602的8位数据口
   	asm("NOP");
   	E=0;	            //恢复使能线电平
}

void lcd_write_address(unsigned char x,unsigned char y)
{  
	x&=0x0f;			//列地址限制在0-15
   	y&=0x01;			//行地址限制在0-1
   	if(y==0x00)
      	lcd_write_com_busy(x|0x80);	   //第一行的列地址写入
   	else
      	lcd_write_com_busy((x+0x40)|0x80);	   //第二行的列地址写入
}

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
	
void lcd_write_char(unsigned char x,unsigned char y,unsigned char buf)
{  		
	lcd_write_address(x,y); //写入地址			
   	lcd_write_data(buf);		//写入显示数据    
} 

void IICint(void)
{   	
	SSPCON1=0X08;               //I2C主控方式，时钟=Fosc/[4*(SSPADD+1)]
  	TRISB0=1;                   //设置SDA为输入口
  	TRISB1=1;                   //设置SCL为输入口  	
	SSPADD=0X09;                //设置IIC时钟频率100KHZ
  	SSPSTAT=0X80;               //初始化SSPSTAT寄存器  	
  	SSPCON2=0X00;               //初始化SSPCON2寄存器
  	SSPEN=1;  	
	I2Cerror=0;  			   	
}
//***********************************
//I2C重复启动
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
//I2C起始位发送函数
//***********************************
void Start()
{
	unsigned char i=0;
	SEN=1;            //起始位发送
	while(1)
	{
    	i++;
    	if(SSPIF==1) break;
    	if(i==200)      //防止死循环
    	{
      		i=0;
      		I2Cerror=1;   
      		break; 
    	}
	}
	SSPIF=0;
}
//***********************************
//I2C停止位发送
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
//I2C数据发送函数
//***********************************
void Senddata(unsigned char data1)
{
	unsigned char i;
	SSPBUF=data1;            //发送数据
	while(1)
	{
    	if(SSPIF==1) break;
	}
	SSPIF=0;
	for(i=0;i<10;i++);

	if(ACKSTAT)   //如果没有发出应答信号
	{
    	I2Cerror=1;
	}
	else          //如果发出应答信号
	{
    	I2Cerror=0;
	}
}
//***********************************
//I2C数据写等待
//***********************************
void WriteWait()
{
	while(1)
	{
   		Start();           //在数据写入时间，如果在给器件发送数据不会有响应
   		Senddata(0xa0);
   		Stop();
   		if(I2Cerror==0) break; //等待数据写入，=0则有应答位产生
	}
}

void write(unsigned char address1,unsigned char byte)
{
    Start();
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	return;         //返回防止死循环
	}
	Senddata(0xa2);   //发送寻址地址，以及写操作地址！
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	Stop();         //错误处理
    	return;         //返回防止死循环
	}
	Senddata(address1);   //发送地址
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	Stop();         //错误处理
    	return;         //返回防止死循环
	}	
	Senddata(byte);     //发送数据 
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	Stop();         //错误处理
    	return;         //返回防止死循环
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
    	return 0xff;         //返回防止死循环
	}
	Senddata(0xa2);   //发送寻址地址，以及写操作地址！
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	Stop();         //错误处理
    	return 0xff;         //返回防止死循环
	}
	Senddata(address);   //发送地址
	if(I2Cerror==1)
	{
    	I2Cerror=0;
    	Stop();         //错误处理
    	return 0xff;         //返回防止死循环
	}
	Restart();         //发送重新起始位
	Senddata(0xa3);    //发送读命令
	if(I2Cerror==1)
	{
   		I2Cerror=0;
   		Stop();         //错误处理
   		return 0xff;         //返回防止死循环
	}
	RCEN=1;
	while(1)
	{   	
		if(SSPIF==1) break;
	}
	SSPIF=0;
	bytebuf=SSPBUF;      //读数
	ACKDT=1;          //发送非应答位
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
	OSCCON=0X00;    		//选择主震荡器
	ADCON1=0X0f;            //所有IO均为数字口，模拟量输入禁止
  	TRISB=0B11000111;   	//RB3-5设置为输出
	TRISD=0B00000000;   	//RD设置为输出  	
  	IICint();               //调用IIC初始化函数    	
	write(0x00,0x00);
	write(0x01,0x00);
  	write(0x02,0x00);				//设定秒
  	write(0x03,0x55);				//设定分
  	write(0x04,0x20);				//设定时
  	write(0x05,0x01);				//设定日
  	write(0x06,0x00);				//设定星期
  	write(0x07,0x02);				//设定月
  	write(0x08,0x11);				//设定年  	
  	lcdreset();	 						//复位1602	   	
  	while(1)
  	{    	
		delay(150);
		buf=read(0x08);        		 //读年寄存器
		lcd_write_char(0,0,'2');   //2 
		lcd_write_char(1,0,'0');   //0		 	
		lcd_write_char(2,0,((buf>>4)&0x0f)+'0'); 
		lcd_write_char(3,0,(buf&0x0f)+'0');
		lcd_write_char(4,0,'.');   //.	
		buf=read(0x07);        		 //读月寄存器
		lcd_write_char(5,0,((buf>>4)&0x01)+'0'); 
		lcd_write_char(6,0,(buf&0x0f)+'0');
		lcd_write_char(7,0,'.');   //.
		buf=read(0x05);        		 //读日寄存器
		lcd_write_char(8,0,((buf>>4)&0x03)+'0'); 
		lcd_write_char(9,0,(buf&0x0f)+'0');
		buf=read(0x04);        		 //读时寄存器
	  	lcd_write_char(0,1,((buf>>4)&0x03)+'0'); 
		lcd_write_char(1,1,(buf&0x0f)+'0');
		lcd_write_char(2,1,':');   //:
		buf=read(0x03);        		 //读分寄存器
    	lcd_write_char(3,1,((buf>>4)&0x0f)+'0'); 
		lcd_write_char(4,1,(buf&0x0f)+'0');
		lcd_write_char(5,1,':');   //:
		buf=read(0x02);        		 //读秒寄存器
    	lcd_write_char(6,1,((buf>>4)&0x07)+'0'); 
		lcd_write_char(7,1,(buf&0x0f)+'0');			  	    	
  	}
}
