/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : Connect the LCD12864 display module to the "LCD12864" socket directly
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
#include <pic18.h>       //调用头文件，可以去PICC18软件下去查找PIC18F4520.H

__CONFIG(1,HS) ;  //EXT 20MHz,No PLL
__CONFIG(2,WDTDIS) ;      //WDT OFF
__CONFIG(3,PBADDIS) ;     //PORTB<4:0> configured as digital I/O on RESET 
__CONFIG(4,LVPDIS&XINSTDIS) ;     //Low Voltage Programming Disabled ,Disable extended instruction set (Legacy mode) 



#define Rs  RA0 //COMMNAD/DATA SELECT 
#define RW  RA1 //READ/WRITE SELECT 
#define E   RA2 //ENABLE SIGNAL 
#define Psb RA3 //PARALLEL/SERIAL SELECT（H/L）
#define Rst RA4 //RESET SIGNAL



void delay(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<10;j++)
		{
			asm("NOP");
		}
	}
}

void write_com(unsigned char cmdcode)
{
	unsigned int ii=20000;
	Rs=0;                    //设置该字节数据为命令代码
	RW=1;                    //设置此次操作为读
	E=1;                     //使能
	TRISD=0B11111111;   	 //RD设置为输入
	while((PORTD&0X01)&&(ii>0))          //读忙状态，不忙时退出
	{
		ii--;
	}
	E=0;                    //失能
	TRISD=0B00000000;   	//RD设置为输出
	Rs=0;                   //设置该字节数据为控制命令
	RW=0;                   //设置此次操作为读写
	//write_bus(cmdcode);     //通过数据总线处理函数将命令字写进总线中去
    PORTD = cmdcode;                     //data to bus. 
	E=1;                    //使能写
	asm("NOP");             //略加延迟
	asm("NOP");
	asm("NOP");
	E=0;                    //失能写
}


void write_data(unsigned char Rsspdata)
{
	unsigned int ii=20000;
	Rs=0;                    //设置该字节数据为命令代码
	RW=1;                    //设置此次操作为读
	E=1;                     //使能
	TRISD=0B11111111;   	 //RD设置为输入
	while((PORTD&0X01)&&(ii>0))          //读忙状态，不忙时退出
	{
		ii--;
	}
	E=0;                    //失能
	TRISD=0B00000000;   	//RD设置为输出
	Rs=1;                   //设置该字节数据为数据
	RW=0;                   //设置此次操作为读写
	//write_bus(Rsspdata);    //通过数据总线处理函数将命令字写进总线中去
    PORTD = Rsspdata;                     //data to bus. 
	E=1;                    //使能写
	asm("NOP");             //略加延迟
	asm("NOP");
	asm("NOP");
	E=0;                    //失能写
}


void write_xy(unsigned char x,unsigned char y)
{
	unsigned char address;
	switch(y){
		case 0:address=0x80+x;break;  //第一行的首地址是0x80
		case 1:address=0x90+x;break;  //第二行的首地址是0x90
		case 2:address=0x88+x;break;  //第三行的首地址是0x88
		case 3:address=0x98+x;break;  //第四行的首地址是0x98
		default:address=0x80+x;
   }
	write_com(address);	
}


void init_lcd(void)
{
	Rst=0;           //复位液晶
	delay(250);      //加些延时
	delay(250);
	Rst=1;           //复位结束
	delay(250);      //加些延时
	Psb=1;           //选择8bit并口
	
	write_com(0x30);  //基本指令集  
	write_com(0x0c);  //显示开
	write_com(0x06);  //光标右移，整体显示不移动
	write_com(0x01);  //清除显示(清零DDRAM)

	delay(250);
	
}


void write_string(unsigned char x,unsigned char y,const unsigned char *s)
{
	write_xy(x,y);   //先写入地址参数
	while(*s)        //只要字符串没写完就继续写
	{
		write_data(*s); //写入一个字符串数据
		s++;            //指针转到下一个
		delay(10);
	}
}


void lcd12864_dis_oneline(unsigned char line,const unsigned char *p)
{
  unsigned char c;
  switch(line%4)
  {
    case 0:   
            write_com(0x80);  //第一行的首地址为0x80
            for(c=0;c<16;c++) //将本行的16个字符全部写进去，汉字占两个字符位
            {
              write_data(*p);
              p++;
              if(*p==0)
              {
                break;
              }
            }    
            break;
    case 1:
            write_com(0x90);  //第二行的首地址为0x90
            for(c=0;c<16;c++) //将本行的16个字符全部写进去，汉字占两个字符位
            {
              write_data(*p);
            p++;
              if(*p==0)
              {
                break;
              }
            }    
            break;
    case 2:
            write_com(0x88);  //第三行的首地址为0x88
            for(c=0;c<16;c++) //将本行的16个字符全部写进去，汉字占两个字符位
            {
              write_data(*p);
              p++;
              if(*p==0)
              {
                break;
              }
            }    
            break;
    case 3:
            write_com(0x98);  //第四行的首地址为0x98
            for(c=0;c<16;c++) //将本行的16个字符全部写进去，汉字占两个字符位
            {
              write_data(*p);
              p++;
              if(*p==0)
              {
                break;
              }
            }    
            break;
    default : break; 
  }
}


 
void main(void)
{	

    ADCON1=0X0F;               //a port as ordinary i/o.
    TRISA=0B00000000;      //初始化RB7-RB0的输入输出方向	
	TRISD=0B00000000;   	//RD设置为输出
    TRISB=0B00000000;      //初始化RB7-RB0的输入输出方向
    PORTA=0B00000000;      //
    PORTB=0B00000000;      //初始化RB7-RB0的数值
    PORTD=0B00000000;      //初始化RD7-RD0的数值
  	init_lcd();             //初始化12864	
//	lcd12864_dis_oneline(0,"罗法科技有限公司");
    write_string(1,0,"easyPIC-40");
    write_string(1,1,"LogiFind Tech");
	write_string(1,2,"LogiFind.com");
	write_string(0,3,"Skype:love100mhz");
	while(1)
	{		
	
	}  
}
