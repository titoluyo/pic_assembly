/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : Connect the LCD1602 display module to the "LCD1602" socket directly
                        Connect the bit RC6 and RC7 on Jumper SW1
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

__CONFIG(1,HS&CPUDIV1) ;  //EXT 20MHz,No PLL
__CONFIG(2,WDTDIS) ;      //WDT OFF
__CONFIG(3,PBADDIS) ;     //PORTB<4:0> configured as digital I/O on RESET 
__CONFIG(4,LVPDIS&XINSTDIS) ;     //Low Voltage Programming Disabled ,Disable extended instruction set (Legacy mode) 

#define RSPIN  RA1 //Data or Instrument Select
#define RWPIN  RA2 //Write or Read
#define EPIN  RA3 //6800 mode Enable single

bit clrbit;
unsigned char recebuf;



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
	TRISD7=1;	   		   //为读状态做准备，把RD7设为输入
   	RSPIN=0;			   //选择指令寄存器
   	RWPIN=1;			   //选择读
   	EPIN=1;				   //使能线电平变化
   	while(RD7==1);         //读忙状态，不忙时退出
   	EPIN=0;				   //恢复使能线电平
   	TRISD7=0;			   //把RD7设置为输出
}

void lcd_write_com(unsigned char combuf)
{  
	RSPIN=0;          	//选择指令寄存器
   	RWPIN=0;           	//选择写
   	PORTD=combuf;       //把命令字送入RD 	
   	EPIN=1;	            //使能线电平变化，命令送入1602的8位数据口
   	asm("NOP");         //来一个空操作，以延时片刻
   	EPIN=0;	            //恢复使能线电平
}

void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();		//调用忙检测函数
   	lcd_write_com(combuf); 	//调用写命令函数
}

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
	
void lcd_write_char(unsigned char x,unsigned char y,unsigned char buf)
{  		
	lcd_write_address(x,y); 	//写入地址			
   	lcd_write_data(buf);		//写入显示数据    
}  

void sci_Init(void)
{
	BRGH = 1;	// high baud rate 
	//SPBRG =25;  // 设置波特率9600 
	SPBRG =129;  // 外部晶振20MHZ,设置波特率9600 
	SYNC = 0;	// 1=同步接收方式 0=异步接收方式 
	SPEN = 1;	// 串口使能位 
	CREN = 1;	// 1=允许连续接收 0=禁止连续接收 
	SREN = 0;	// 异步方式：此位未用 
	TXIE = 0;	// 关闭发送 
	RCIE = 0;	// 关闭接收中断 
	TX9  = 0;	// 1：选择9位接收 0：选择8位接收 
	RX9  = 0;	// 1：选择9位接收 0：选择8位接收 
	TXEN = 1;	// 发送允许 
	TRISC6=1;   // 作为串口使用，RC6必须设置为输入
	TRISC7=1;   // 作为串口使用，RC7必须设置为输入	
}

void send232byte(unsigned char bytebuf)
{
	TXREG=bytebuf;
	while(TRMT==1);
		
}

unsigned char WaitComm(void)
{   	
	if(RCIF==1)           //如果接收标志位为1，即有内容需要接收
    {        		
		recebuf=RCREG;	  //将缓冲区内容接收下来	
	  	if(OERR)	      //如果有溢出错误
		{
			CREN = 0;     //清零CREN位可将此位OERR清零
			CREN = 1;		
		}	 
		return 1; 	       //接收完毕后，返回标志1
	}	
	return 0;              //没有接收，返回标志0	
}




void main(void)	
{
	unsigned char buf;
	OSCCON=0X00;    		//选择主震荡器
	ADCON1=0X0f;            //所有IO均为数字口，模拟量输入禁止
  	TRISA=0B11000000;   	//RB3-5设置为输出
    TRISD=0B00000000;   	//RD设置为输出   		   
	lcdreset();  
  	sci_Init(); 
  	lcd_write_char(0,0,'R');   	//在坐标(0,0)处写入R
	lcd_write_char(1,0,'E');   	//在坐标(1,0)处写入e		
	lcd_write_char(2,0,'C');   	//在坐标(2,0)处写入c
	lcd_write_char(3,0,'E');   	//在坐标(3,0)处写入e	
	lcd_write_char(4,0,'I');   	//在坐标(4,0)处写入i		
	lcd_write_char(5,0,'V');   	//在坐标(5,0)处写入v
	lcd_write_char(6,0,'E');   	//在坐标(6,0)处写入e

	lcd_write_char(8,0,'A');   	//在坐标(8,0)处写入a

	lcd_write_char(10,0,'B');  	//在坐标(10,0)处写入b
	lcd_write_char(11,0,'Y');  	//在坐标(11,0)处写入y
	lcd_write_char(12,0,'T');  	//在坐标(12,0)处写入t
	lcd_write_char(13,0,'E');  	//在坐标(13,0)处写入e 
	

	lcd_write_char(0,1,'B');   	//在坐标(0,1)处写入B
	lcd_write_char(1,1,'A');   	//在坐标(1,1)处写入a		
	lcd_write_char(2,1,'U');   	//在坐标(2,1)处写入u
	lcd_write_char(3,1,'D');   	//在坐标(3,1)处写入d	

	lcd_write_char(5,1,'R');   	//在坐标(5,1)处写入r
	lcd_write_char(6,1,'A');   	//在坐标(6,1)处写入a
	lcd_write_char(7,1,'T');   	//在坐标(7,1)处写入t
	lcd_write_char(8,1,'E');   	//在坐标(8,1)处写入e
	lcd_write_char(9,1,':');   	//在坐标(9,1)处写入:
	
	lcd_write_char(10,1,'9');   //在坐标(10,1)处写入9
	lcd_write_char(11,1,'6');   //在坐标(11,1)处写入6
	lcd_write_char(12,1,'0');   //在坐标(12,1)处写入0
	lcd_write_char(13,1,'0');   //在坐标(13,1)处写入0
	clrbit=1;			        //置清屏标志位,以便在第一个
  	while(1)
  	{
 	
    	if(WaitComm()==1)				//如果有收到上位机数据
		{  						
			if(clrbit)		        	//如果为第一次收到数据启动清屏操作
			{
		 		clrbit=0;			 		
				lcdreset();         	//清屏幕 
			}							  
			lcd_write_char(0,0,'R');   	//在坐标(0,0)处写入R
			lcd_write_char(1,0,'E');   	//在坐标(1,0)处写入e		
			lcd_write_char(2,0,'C');   	//在坐标(2,0)处写入c
			lcd_write_char(3,0,'E');   	//在坐标(3,0)处写入e
			lcd_write_char(4,0,':');   	//在坐标(4,0)处写入:
			lcd_write_char(6,0,'0');   	//在坐标(6,0)处写入0
			lcd_write_char(7,0,'X');   	//在坐标(7,0)处写入x
			buf=recebuf>>4;            	//收到的数据高4位放入BUF
			if(buf<10)                 	//因为1602需要送入ASCII码，需要如下转换
				buf+=0X30;          	//如果0-9，直接加上'0'0X30
			else
				buf+=0x37;             	//如果A-F，直接加上0X37
			lcd_write_char(8,0,buf);   	//在坐标(8,0)处写入该数据
			buf=recebuf&0x0f;          	//收到的数据低4位放入BUF
			if(buf<10)                 	//因为1602需要送入ASCII码，需要如下转换
				buf+=0X30;          	//如果0-9，直接加上'0'0X30
			else
				buf+=0x37;             	//如果A-F，直接加上0X37
			lcd_write_char(9,0,buf);   	//在坐标(9,0)处写入该数据	

			recebuf+=1;                 //做一次加一动作
			send232byte(recebuf);		//回复数据为接收数据
		
			lcd_write_char(0,1,'S');   	//在坐标(0,1)处写入S
			lcd_write_char(1,1,'E');   	//在坐标(1,1)处写入e		
			lcd_write_char(2,1,'N');   	//在坐标(2,1)处写入n
			lcd_write_char(3,1,'D');   	//在坐标(3,1)处写入d
			lcd_write_char(4,1,':');   	//在坐标(4,1)处写入:
			lcd_write_char(6,1,'0');   	//在坐标(6,1)处写入0
			lcd_write_char(7,1,'X');   	//在坐标(7,1)处写入x
			
			buf=recebuf>>4;        		//发送的数据高4位放入BUF
			if(buf<10)                 	//因为1602需要送入ASCII码，需要如下转换
				buf+=0X30;          	//如果0-9，直接加上'0'0X30
			else
				buf+=0x37;             	//如果A-F，直接加上0X37
			lcd_write_char(8,1,buf);   	//在坐标(8,1)处写入该数据
			buf=recebuf&0x0f;      		//发送的数据低4位放入BUF
			if(buf<10)                 	//因为1602需要送入ASCII码，需要如下转换
				buf+=0X30;          	//如果0-9，直接加上'0'0X30
			else
				buf+=0x37;             	//如果A-F，直接加上0X37
			lcd_write_char(9,1,buf);   	//在坐标(9,1)处写入该数据				
		}	           	
  	}
}
