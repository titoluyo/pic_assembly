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
#include <pic18.h>       //����ͷ�ļ�������ȥPICC18�����ȥ����PIC18F4520.H

__CONFIG(1,HS) ;  //EXT 20MHz,No PLL
__CONFIG(2,WDTDIS) ;      //WDT OFF
__CONFIG(3,PBADDIS) ;     //PORTB<4:0> configured as digital I/O on RESET 
__CONFIG(4,LVPDIS&XINSTDIS) ;     //Low Voltage Programming Disabled ,Disable extended instruction set (Legacy mode) 



#define Rs  RA0 //COMMNAD/DATA SELECT 
#define RW  RA1 //READ/WRITE SELECT 
#define E   RA2 //ENABLE SIGNAL 
#define Psb RA3 //PARALLEL/SERIAL SELECT��H/L��
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
	Rs=0;                    //���ø��ֽ�����Ϊ�������
	RW=1;                    //���ô˴β���Ϊ��
	E=1;                     //ʹ��
	TRISD=0B11111111;   	 //RD����Ϊ����
	while((PORTD&0X01)&&(ii>0))          //��æ״̬����æʱ�˳�
	{
		ii--;
	}
	E=0;                    //ʧ��
	TRISD=0B00000000;   	//RD����Ϊ���
	Rs=0;                   //���ø��ֽ�����Ϊ��������
	RW=0;                   //���ô˴β���Ϊ��д
	//write_bus(cmdcode);     //ͨ���������ߴ�������������д��������ȥ
    PORTD = cmdcode;                     //data to bus. 
	E=1;                    //ʹ��д
	asm("NOP");             //�Լ��ӳ�
	asm("NOP");
	asm("NOP");
	E=0;                    //ʧ��д
}


void write_data(unsigned char Rsspdata)
{
	unsigned int ii=20000;
	Rs=0;                    //���ø��ֽ�����Ϊ�������
	RW=1;                    //���ô˴β���Ϊ��
	E=1;                     //ʹ��
	TRISD=0B11111111;   	 //RD����Ϊ����
	while((PORTD&0X01)&&(ii>0))          //��æ״̬����æʱ�˳�
	{
		ii--;
	}
	E=0;                    //ʧ��
	TRISD=0B00000000;   	//RD����Ϊ���
	Rs=1;                   //���ø��ֽ�����Ϊ����
	RW=0;                   //���ô˴β���Ϊ��д
	//write_bus(Rsspdata);    //ͨ���������ߴ�������������д��������ȥ
    PORTD = Rsspdata;                     //data to bus. 
	E=1;                    //ʹ��д
	asm("NOP");             //�Լ��ӳ�
	asm("NOP");
	asm("NOP");
	E=0;                    //ʧ��д
}


void write_xy(unsigned char x,unsigned char y)
{
	unsigned char address;
	switch(y){
		case 0:address=0x80+x;break;  //��һ�е��׵�ַ��0x80
		case 1:address=0x90+x;break;  //�ڶ��е��׵�ַ��0x90
		case 2:address=0x88+x;break;  //�����е��׵�ַ��0x88
		case 3:address=0x98+x;break;  //�����е��׵�ַ��0x98
		default:address=0x80+x;
   }
	write_com(address);	
}


void init_lcd(void)
{
	Rst=0;           //��λҺ��
	delay(250);      //��Щ��ʱ
	delay(250);
	Rst=1;           //��λ����
	delay(250);      //��Щ��ʱ
	Psb=1;           //ѡ��8bit����
	
	write_com(0x30);  //����ָ�  
	write_com(0x0c);  //��ʾ��
	write_com(0x06);  //������ƣ�������ʾ���ƶ�
	write_com(0x01);  //�����ʾ(����DDRAM)

	delay(250);
	
}


void write_string(unsigned char x,unsigned char y,const unsigned char *s)
{
	write_xy(x,y);   //��д���ַ����
	while(*s)        //ֻҪ�ַ���ûд��ͼ���д
	{
		write_data(*s); //д��һ���ַ�������
		s++;            //ָ��ת����һ��
		delay(10);
	}
}


void lcd12864_dis_oneline(unsigned char line,const unsigned char *p)
{
  unsigned char c;
  switch(line%4)
  {
    case 0:   
            write_com(0x80);  //��һ�е��׵�ַΪ0x80
            for(c=0;c<16;c++) //�����е�16���ַ�ȫ��д��ȥ������ռ�����ַ�λ
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
            write_com(0x90);  //�ڶ��е��׵�ַΪ0x90
            for(c=0;c<16;c++) //�����е�16���ַ�ȫ��д��ȥ������ռ�����ַ�λ
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
            write_com(0x88);  //�����е��׵�ַΪ0x88
            for(c=0;c<16;c++) //�����е�16���ַ�ȫ��д��ȥ������ռ�����ַ�λ
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
            write_com(0x98);  //�����е��׵�ַΪ0x98
            for(c=0;c<16;c++) //�����е�16���ַ�ȫ��д��ȥ������ռ�����ַ�λ
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
    TRISA=0B00000000;      //��ʼ��RB7-RB0�������������	
	TRISD=0B00000000;   	//RD����Ϊ���
    TRISB=0B00000000;      //��ʼ��RB7-RB0�������������
    PORTA=0B00000000;      //
    PORTB=0B00000000;      //��ʼ��RB7-RB0����ֵ
    PORTD=0B00000000;      //��ʼ��RD7-RD0����ֵ
  	init_lcd();             //��ʼ��12864	
//	lcd12864_dis_oneline(0,"�޷��Ƽ����޹�˾");
    write_string(1,0,"easyPIC-40");
    write_string(1,1,"LogiFind Tech");
	write_string(1,2,"LogiFind.com");
	write_string(0,3,"Skype:love100mhz");
	while(1)
	{		
	
	}  
}
