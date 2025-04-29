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
#include <pic18.h>        //����ͷ�ļ�������ȥPICC18�����ȥ����PIC18F4550.H

__CONFIG(1,HS) ;  //EXT 20MHz,No PLL
__CONFIG(2,WDTDIS) ;      //WDT OFF
__CONFIG(3,PBADDIS) ;     //PORTB<4:0> configured as digital I/O on RESET 
__CONFIG(4,LVPDIS&XINSTDIS) ;     //Low Voltage Programming Disabled ,Disable extended instruction set (Legacy mode) 

#define RSPIN  RA1 //Data or Instrument Select
#define RWPIN  RA2 //Write or Read
#define EPIN  RA3 //6800 mode Enable single

unsigned char  temp1;                        //�ɼ������¶ȸ�8λ
unsigned char  temp2;                         //�ɼ������¶ȵ�8λ

//ת������¶�ֵС���㲿�ֲ��
const unsigned char tablexiao[16]={0,0,1,2,2,3,4,4,5,6,6,7,8,8,9,9};

//---------------------------------------
//����: ��ʱ����
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
//����: ��λDS18B20����
//-----------------------------------------
unsigned char reset(void)
{
	unsigned char outbit;
	TRISC1=0;					//����RA4λ�����
	RC1=0;					    //����RA4=0;
    delay(2,70);                //��ʱ503us
    TRISC1=1;                   //����RA4Ϊ����ڣ����ͷ����ߵȵ�����������
    delay(2,8);                 //��ʱ70us
    if(RC1==1) outbit=0;        //û�н��յ�Ӧ���źţ�������λ
    else outbit=1;              //���յ�Ӧ���ź�
    delay(2,60);                //��ʱ430us 
    return outbit;  	        //���������أ�������յ�Ӧ�𣬷���1�����򷵻�0
}
//---------------------------------------
//����: д�ֽں���
//-----------------------------------------
void write_byte(unsigned char  val)
{
 	unsigned char i;
 	unsigned char temp;
 	for(i=8;i>0;i--)
 	{
   		temp=val&0x01;              //���λ�Ƴ�
   		TRISC1=0;					//����RA4λ�����
		RC1=0;					    //����RA4=0;
   		NOP();
   		NOP();
   		NOP();
   		NOP();
   		NOP();                      //�Ӹ������͵�ƽ,����дʱ��϶
   		if(temp==1)  TRISC1=1;      //���д1,���ߵ�ƽ
   		delay(2,7);                 //��ʱ63us
   		TRISC1=1;                   //����RA4Ϊ����ڣ����ͷ����ߵȵ�����������
   		NOP();
   		NOP();
   		val=val>>1;                //����һλ
  	}
}
//---------------------------------------
//����: ���ֽں���
//-----------------------------------------
unsigned char read_byte(void)
{
 unsigned char i;
 unsigned char value=0;                                //�����¶�
 for(i=8;i>0;i--)
 {
   value>>=1;
   TRISC1=0;					//����RA4λ�����
   RC1=0;					    //����RA4=0;
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();                      //6us
   TRISC1=1;                   //����RA4Ϊ�����
   NOP();
   NOP();
   NOP(); 
   NOP();                      //4us
   if(RC1==1) value|=0x80;     //������յ�����Ϊ1,�����λ������
   delay(2,7);                 //63us
  }
  return(value);
}
//---------------------------------------
//����: �������¶Ⱥ���
//-----------------------------------------
void convert_T(void) 
{ 
    if(reset()==1)            //�����λ�ɹ�
	{  
        write_byte(0xcc);     // ����������ʶ�� 
        write_byte(0x44);     // �����¶�ת�� 
    } 
} 
//---------------------------------------
//����: ���¶Ⱥ���
//-----------------------------------------
void read_T(void) 
{ 
    unsigned char Lsb,Msb;            
    if(reset()==1) 
	{  
        write_byte(0xcc);      // ����������ʶ�� 
        write_byte(0xbe);      // ���ݴ��� 
        Lsb=read_byte();       // ���ֽ� 
        Msb=read_byte();  	   // ���ֽ� 
		temp2=Lsb&0x0f;        //LSB�ĵ�4λΪС������
		temp1=(Lsb>>4)|(Msb<<4);//LSB�ĸ�4λ��MSBƴ����������
    }     
} 
//---------------------------------------
//����: ��ʱ����		 
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
//����: 1602æ��⺯��		 
//----------------------------------------- 
void lcd_wait_busy(void)
{  
	TRISD7=1;	   		   //Ϊ��״̬��׼������RD7��Ϊ����
   	RSPIN=0;			   //ѡ��ָ��Ĵ���
   	RWPIN=1;			   //ѡ���
   	EPIN=1;				   //ʹ���ߵ�ƽ�仯
   	while(RD7==1);         //��æ״̬����æʱ�˳�
   	EPIN=0;				   //�ָ�ʹ���ߵ�ƽ
   	TRISD7=0;			   //��RD7����Ϊ���
}
//---------------------------------------
//����: 1602д�����		 
//-----------------------------------------
void lcd_write_com(unsigned char combuf)
{  
	RSPIN=0;          	//ѡ��ָ��Ĵ���
   	RWPIN=0;           	//ѡ��д
   	PORTD=combuf;       //������������RD 	
   	EPIN=1;	            //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�
   	asm("NOP");         //��һ���ղ���������ʱƬ��
   	EPIN=0;	            //�ָ�ʹ���ߵ�ƽ
}
//---------------------------------------
//����: 1602д���������æ��⣩		 
//-----------------------------------------
void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();		//����æ��⺯��
   	lcd_write_com(combuf); 	//����д�����
}
//---------------------------------------
//����: 1602д���ݺ�������æ��⣩		 
//-----------------------------------------
void lcd_write_data(unsigned char databuf)
{  
	lcd_wait_busy();	//����æ��⺯��
   	RSPIN=1;          	//ѡ�����ݼĴ���
   	RWPIN=0;           	//ѡ��д
   	PORTD=databuf;      //������������RD��
   	EPIN=1;	            //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�
   	asm("NOP");
   	EPIN=0;	            //�ָ�ʹ���ߵ�ƽ
}
//---------------------------------------
//����: 1602��ʾ��ַд����		 
//-----------------------------------------
void lcd_write_address(unsigned char x,unsigned char y)
{  
	x&=0x0f;			//�е�ַ������0-15
   	y&=0x01;			//�е�ַ������0-1
   	if(y==0x00)
      	lcd_write_com_busy(x|0x80);	   //��һ�е��е�ַд��
   	else
      	lcd_write_com_busy((x+0x40)|0x80);	   //�ڶ��е��е�ַд��
}
//---------------------------------------
//����: 1602��ʼ������		 
//-----------------------------------------
void lcdreset(void)
{  
	delay1(150); 
	lcd_write_com(0x38);
   	delay1(50); 
	lcd_write_com(0x38);
   	delay1(50); 
	lcd_write_com(0x38);
   	lcd_write_com_busy(0x38);       //8λ���ݣ�˫�У�5*7����
   	lcd_write_com_busy(0x08);		//��ʾ���ܹأ��޹��
   	lcd_write_com_busy(0x01);		//����ָ��
   	lcd_write_com_busy(0x06);		//д���µ����ݺ󣬹�����ƣ���ʾ�����ƶ�
   	lcd_write_com_busy(0x0c);		//��ʾ���ܿ����޹�꣬
}  	
//---------------------------------------
//����: ָ����ַд�뺯��		 
//-----------------------------------------	
void lcd_write_char(unsigned char x,unsigned char y,unsigned char buf)
{  		
	lcd_write_address(x,y); 	//д���ַ			
   	lcd_write_data(buf);		//д����ʾ����    
}  
//---------------------------------------
//����: ������		 
//----------------------------------------- 
void main(void)
{	
	unsigned char buf;
	OSCCON=0X00;    		//ѡ��������
	ADCON1=0X0f;            //����IO��Ϊ���ֿڣ�ģ���������ֹ
	TRISA=0B00000000;   	//RA����Ϊ���
	TRISC=0B00000000;   	//RB3-5����Ϊ���
    TRISD=0B00000000;   	//RD����Ϊ��� 	
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
		convert_T();            //�����¶�ת��
    	delay(25,250);          //��ʱ
			
		read_T();               //���¶�����
		if(temp1>99) temp1=99;	//��������ֻ��ʾ2λ�������֣������޶���99��		 
		if(temp2>15) temp2=0;   //�޶���Χ�����������    
			
		
		buf=temp1/10;			  
		buf+=0x30;			
		lcd_write_char(4,1,buf);  //�¶���������ʮλ
		buf=temp1%10;
		buf+=0x30;			
		lcd_write_char(5,1,buf);  //�¶��������ָ�λ
		lcd_write_char(6,1,'.');  //.
		buf=tablexiao[temp2&0x0f];
		buf+=0x30;
		lcd_write_char(7,1,buf);  //�¶�С������
		lcd_write_char(8,1,0xdf);   //.
		lcd_write_char(9,1,0x43);   //C 	  	
  	}
}
