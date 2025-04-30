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

//---------------------------------------
//定义一个数据表格
const unsigned char TAB[8]=
{                          //定义表格一定要使用const，这样会做到程序存储区中
    0x04,                  //表格第1步数据0B00000100
    0x0C,                  //表格第2步数据0B00001100
    0x08,                  //表格第3步数据0B00001000
    0x18,                  //表格第4步数据0B00011000
    0x10,                  //表格第5步数据0B00010000
    0x30,                  //表格第6步数据0B00110000
    0x20,                  //表格第7步数据0B00100000
    0x24,                  //表格第8步数据0B00100100
};


void main(void)            //主函数,单片机开机后就是从这个函数开始运行
{

    unsigned char x=0;      //定义一个int型变量，做延时用
    unsigned char y=7;     //定义一个char型变量，查表索引用

    TRISD=0B11000011;      //初始化RD7-RD0的输入输出方向
    PORTD=0B00000000;      //初始化RD7-RD0的数值

    while(1)               //死循环,单片机初始化后,将一直运行这个死循环
    {
        for(x=0;x<200;x++);//延时若干时间，具体时长暂不理会
        if(++y>7) y=0;     //y做加1操作，并限制在0-7以内
        PORTD=TAB[y];      //将TAB[y]的值送入RD口输出

    }
}

