/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : Connect the LCD1602 display module to the "LCD1602" socket directly
                        Connect the bit RC1 on Jumper SW3
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

#define RSPIN  RA1 //Data or Instrument Select
#define RWPIN  RA2 //Write or Read
#define EPIN  RA3 //6800 mode Enable single

unsigned char  temp1;                        //采集到的温度高8位
unsigned char  temp2;                         //采集到的温度低8位

//转换后的温度值小数点部分查表
const unsigned char tablexiao[16]={0,0,1,2,2,3,4,4,5,6,6,7,8,8,9,9};

//---------------------------------------
//名称: 延时函数
//----------------------------------------- 
void delay(unsigned char x,unsigned char y)
{
  	unsigned char z;
  	do{
      	z=y;
      	do{
      		;
      	}while(--z);           	
    }while(--x);     
}
//---------------------------------------
//名称: 复位DS18B20函数
//-----------------------------------------
unsigned char reset(void)
{
	unsigned char outbit;
	TRISC1=0;					//设置RA4位输出口
	RC1=0;					    //设置RA4=0;
    delay(2,70);                //延时503us
    TRISC1=1;                   //设置RA4为输入口，以释放总线等电阻拉高总线
    delay(2,8);                 //延时70us
    if(RC1==1) outbit=0;        //没有接收到应答信号，继续复位
    else outbit=1;              //接收到应答信号
    delay(2,60);                //延时430us 
    return outbit;  	        //带参数返回，如果接收到应答，返回1，否则返回0
}
//---------------------------------------
//名称: 写字节函数
//-----------------------------------------
void write_byte(unsigned char  val)
{
 	unsigned char i;
 	unsigned char temp;
 	for(i=8;i>0;i--)
 	{
   		temp=val&0x01;              //最低位移出
   		TRISC1=0;					//设置RA4位输出口
		RC1=0;					    //设置RA4=0;
   		NOP();
   		NOP();
   		NOP();
   		NOP();
   		NOP();                      //从高拉至低电平,产生写时间隙
   		if(temp==1)  TRISC1=1;      //如果写1,拉高电平
   		delay(2,7);                 //延时63us
   		TRISC1=1;                   //设置RA4为输入口，以释放总线等电阻拉高总线
   		NOP();
   		NOP();
   		val=val>>1;                //右移一位
  	}
}
//---------------------------------------
//名称: 读字节函数
//-----------------------------------------
unsigned char read_byte(void)
{
 unsigned char i;
 unsigned char value=0;                                //读出温度
 for(i=8;i>0;i--)
 {
   value>>=1;
   TRISC1=0;					//设置RA4位输出口
   RC1=0;					    //设置RA4=0;
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();                      //6us
   TRISC1=1;                   //设置RA4为输入口
   NOP();
   NOP();
   NOP(); 
   NOP();                      //4us
   if(RC1==1) value|=0x80;     //如果接收到数据为1,从最高位往右移
   delay(2,7);                 //63us
  }
  return(value);
}
//---------------------------------------
//名称: 启动读温度函数
//-----------------------------------------
void convert_T(void) 
{ 
    if(reset()==1)            //如果复位成功
	{  
        write_byte(0xcc);     // 跳过多器件识别 
        write_byte(0x44);     // 启动温度转换 
    } 
} 
//---------------------------------------
//名称: 读温度函数
//-----------------------------------------
void read_T(void) 
{ 
    unsigned char Lsb,Msb;            
    if(reset()==1) 
	{  
        write_byte(0xcc);      // 跳过多器件识别 
        write_byte(0xbe);      // 读暂存器 
        Lsb=read_byte();       // 低字节 
        Msb=read_byte();  	   // 高字节 
		temp2=Lsb&0x0f;        //LSB的低4位为小数部分
		temp1=(Lsb>>4)|(Msb<<4);//LSB的高4位和MSB拼成整数部分
    }     
} 
//---------------------------------------
//名称: 延时函数		 
//-----------------------------------------
void delay1(unsigned int t)
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
	delay1(150); 
	lcd_write_com(0x38);
   	delay1(50); 
	lcd_write_com(0x38);
   	delay1(50); 
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
void main(void)
{	
	unsigned char buf;
	OSCCON=0X00;    		//选择主震荡器
	ADCON1=0X0f;            //所有IO均为数字口，模拟量输入禁止
	TRISA=0B00000000;   	//RA设置为输出
	TRISC=0B00000000;   	//RB3-5设置为输出
    TRISD=0B00000000;   	//RD设置为输出 	
  	lcdreset();	
	lcd_write_char(0,0,'-');   //-
	lcd_write_char(1,0,'-');   //-		
	lcd_write_char(2,0,'D');   //D
	lcd_write_char(3,0,'S');   //S
	lcd_write_char(4,0,'1');   //1
	lcd_write_char(5,0,'8');   //8
	lcd_write_char(6,0,'B');   //B
	lcd_write_char(7,0,'2');   //2
	lcd_write_char(8,0,'0');   //0

	lcd_write_char(10,0,'T');  //T
	lcd_write_char(11,0,'E');  //E
	lcd_write_char(12,0,'S');  //S
	lcd_write_char(13,0,'T');  //T
	lcd_write_char(14,0,'-');   //-
	lcd_write_char(15,0,'-');   //-
  	while(1)
  	{
		convert_T();            //启动温度转换
    	delay(25,250);          //延时
			
		read_T();               //读温度数据
		if(temp1>99) temp1=99;	//这里我们只显示2位整数部分，所以限定在99度		 
		if(temp2>15) temp2=0;   //限定范围，以免查表溢出    
			
		
		buf=temp1/10;			  
		buf+=0x30;			
		lcd_write_char(4,1,buf);  //温度整数部分十位
		buf=temp1%10;
		buf+=0x30;			
		lcd_write_char(5,1,buf);  //温度整数部分个位
		lcd_write_char(6,1,'.');  //.
		buf=tablexiao[temp2&0x0f];
		buf+=0x30;
		lcd_write_char(7,1,buf);  //温度小数部分
		lcd_write_char(8,1,0xdf);   //.
		lcd_write_char(9,1,0x43);   //C 	  	
  	}
}
