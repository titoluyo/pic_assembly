/********************************************************************************************************
*********************************************************************************************************
*
* Hardware Environment:	easyPIC-40
* Build Environment   : MPLAB IDE or MPLAB X IDE + PICC18_v8.35
* Microcontroller     : PIC18F4550
* Hardware Connection : Connect the LCD1602 display module to the "LCD1602" socket directly
                        Connect the bit RB0,RB4 and RB5 on Jumper SW4
* Version             : V1.0
* By                  : JEFF
* DATE				  : 20190320
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

#define i_o   RB4                      //1302I_O           
#define sclk  RB0                      //1302 clock        
#define rst   RB5                      //1302 enable bit 

#define TRIS_i_o   TRISB4                      //1302I_O           
#define TRIS_sclk  TRISB0                      //1302 clock        
#define TRIS_rst   TRISB5                      //1302 enable bit 

// unsigned char time_rx;
  unsigned char time_rx @ 0x30;        //define receive reg.
  static volatile bit time_rx7   @ (unsigned)&time_rx*8+7;   //receive reg highest.
//static volatile bit temp0     @ (unsigned)&temp*8+0;

void port_init();                      //port initilize subroutine.
void ds1302_init();                    //DS1302 initilize subroutine.
void set_time();                       //set time subroutine.
void get_time();                       //get time subroutine.
void display();                        //display subroutine.
void time_write_1(unsigned char time_tx);    //write one byte subroutine.
unsigned char  time_read_1();          //read one byte subroutine.
void delay();                          //delay subroutine.
//define the time: sec,min,hour,day,month,week,year,control word.
const char table[]={0x00,0x58,0x12,0x8,0x3,0x06,0x06,0x00};
//define the read time and date save table.
char table1[7];
//define the display code for display 0-9
const char table2[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};


//---------------------------------------
//名称: 延时函数
//----------------------------------------- 
/*void delay(unsigned char x,unsigned char y)
{
  	unsigned char z;
  	do{
      	z=y;
      	do{
      		;
      	}while(--z);           	
    }while(--x);     
}*/

void  delay()              //
    {
     int i;                 //define variable
     for(i=200;i--;);     //delay
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



//---------------------------------------------
//DS1302 initilize.
void ds1302_init()
  {
   
   rst=0;/* RST*/
   TRIS_rst=0;  
   TRIS_i_o=1;/* IO*/
   sclk=0;/* SCLK*/
   TRIS_sclk=0; 

   sclk=0;                            //pull low clock
   rst =0;                            //reset DS1302
   rst=1;                             //enable DS1302

   time_write_1(0x8e);                //send control command
   time_write_1(0);                   //enable write DS1302
   rst=0;                             //reset
  }

//---------------------------------------------
//set time.
void set_time()
  {
   int i;                             //define the loop counter.
   rst=1;                             //enable DS1302
   time_write_1(0xbe);                //
   for(i=0;i<8;i++)                   //continue to write 8 bytes.
     {
       time_write_1(table[i]);        //write one byte
     }
   rst=0;                             //reset
   }

//---------------------------------------------
//get time.
void get_time()
 {
   int i;                             //set loop counter.
   rst=1;                             //enable DS1302
   time_write_1(0xbf);                //
   for(i=0;i<7;i++)                   //continue to read 7 bytes.
     {
        table1[i]=time_read_1();      //
     }
    rst=0;                            //reset DS1302
 }

//--------------------------------------------
//write one byte
void time_write_1(unsigned char time_tx)
 {
    int j;                            //set the loop counter.
    TRIS_i_o=0;/* IO*/

    for(j=0;j<8;j++)                  //continue to write 8bit
      {
        i_o=0;                        //
        sclk=0;                       //pull low clk
        if(time_tx&0x01)              //judge the send bit is 0 or 1.
          {
            i_o=1;                    //is 1
          }
        time_tx=time_tx>>1;           //rotate right 1 bit.
        sclk=1;                       //pull high clk
       }
      sclk=0;                         //finished 1 byte,pull low clk
  }

//---------------------------------------------
//read one byte.
unsigned char time_read_1()
 {
   int j;                            //set the loop counter. 
   TRIS_i_o=1;/* IO*/ 
   TRISB4=1;                         //continue to write 8bit 
   for(j=0;j<8;j++)                  
      {
        sclk=0;                       //pull low clk                   
        time_rx=time_rx>>1;           //judge the send bit is 0 or 1.  
        time_rx7=i_o;                //put the received bit into the reg's highest.
       sclk=1;                       //pull high clk                 
      }                                                              
    TRISB4=0;                        //finished 1 byte,pull low clk  
    sclk=0;                          
    return(time_rx);                 
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
    TRISB=0X02;                     //rb1 input, others output
	
  	lcdreset();	
    ds1302_init();                   //DS1302 initilize.
    set_time();                      //set time
	lcd_write_char(0,0,'-');   //-
	lcd_write_char(1,0,'-');   //-		
	lcd_write_char(2,0,'D');   //D
	lcd_write_char(3,0,'S');   //S
	lcd_write_char(4,0,'1');   //1
	lcd_write_char(5,0,'3');   //3
	lcd_write_char(6,0,'0');   //0
	lcd_write_char(7,0,'2');   //2
	lcd_write_char(8,0,' ');   //0

	lcd_write_char(10,0,'T');  //T
	lcd_write_char(11,0,'E');  //E
	lcd_write_char(12,0,'S');  //S
	lcd_write_char(13,0,'T');  //T
	lcd_write_char(14,0,'-');   //-
	lcd_write_char(15,0,'-');   //-
  	while(1)
  	{
	
    //	delay(25,250);          //延时
      int i;
      if(RB1==0)                     //judge rb1.
       {
          table1[0]=table1[3];     
          table1[1]=table1[4];
          table1[2]=table1[6];
       }
	
		get_time();

		/*i=table1[0]&0x0f;             //sec's low.
		lcd_write_char(11,1,2);  //
       // delay();

        i=table1[0]&0xf0;             //sec's high
        i=i>>4;
		lcd_write_char(10,1,i);  //
        //delay();

	    lcd_write_char(9,1,'-');   //-
        //delay();
  
        i=table1[1]&0x0f;             //min's low. 
        lcd_write_char(8,1,i);
        //delay();

        i=table1[1]&0xf0;             //min's high                 
        i=i>>4;
        lcd_write_char(7,1,i);
        //delay();

	    lcd_write_char(6,1,'-');   //-
        //delay();

        i=table1[2]&0x0f;             //hour's low.                 
        lcd_write_char(5,1,i);
        //delay();                      //delay    
      
        i=table1[2]&0xf0;             //hour's high                 
        i=i>>4;                       //rotate right for 4 bits.   
        lcd_write_char(4,1,i);
        //delay();*/

		i=table1[0]&0x0f;             //sec's low.
        i|=0b00110000;
		lcd_write_char(11,1,i);  //
       // delay();

        i=table1[0]&0xf0;             //sec's high
        i=i>>4;
        i|=0b00110000;
		lcd_write_char(10,1,i);  //
        //delay();

	    lcd_write_char(9,1,'-');   //-
        //delay();
  
        i=table1[1]&0x0f;             //min's low. 
        i|=0b00110000;
        lcd_write_char(8,1,i);
        //delay();

        i=table1[1]&0xf0;             //min's high                 
        i=i>>4;
        i|=0b00110000;
        lcd_write_char(7,1,i);
        //delay();

	    lcd_write_char(6,1,'-');   //-
        //delay();

        i=table1[2]&0x0f;             //hour's low.  
        i|=0b00110000;               
        lcd_write_char(5,1,i);
        //delay();                      //delay    
      
        i=table1[2]&0xf0;             //hour's high                 
        i=i>>4;                       //rotate right for 4 bits. 
        i|=0b00110000;  
        lcd_write_char(4,1,i);
	  	
  	}
}
