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
//����һ�����ݱ��
const unsigned char TAB[8]=
{                          //������һ��Ҫʹ��const����������������洢����
    0x04,                  //����1������0B00000100
    0x0C,                  //����2������0B00001100
    0x08,                  //����3������0B00001000
    0x18,                  //����4������0B00011000
    0x10,                  //����5������0B00010000
    0x30,                  //����6������0B00110000
    0x20,                  //����7������0B00100000
    0x24,                  //����8������0B00100100
};


void main(void)            //������,��Ƭ����������Ǵ����������ʼ����
{

    unsigned char x=0;      //����һ��int�ͱ���������ʱ��
    unsigned char y=7;     //����һ��char�ͱ��������������

    TRISD=0B11000011;      //��ʼ��RD7-RD0�������������
    PORTD=0B00000000;      //��ʼ��RD7-RD0����ֵ

    while(1)               //��ѭ��,��Ƭ����ʼ����,��һֱ���������ѭ��
    {
        for(x=0;x<200;x++);//��ʱ����ʱ�䣬����ʱ���ݲ����
        if(++y>7) y=0;     //y����1��������������0-7����
        PORTD=TAB[y];      //��TAB[y]��ֵ����RD�����

    }
}

