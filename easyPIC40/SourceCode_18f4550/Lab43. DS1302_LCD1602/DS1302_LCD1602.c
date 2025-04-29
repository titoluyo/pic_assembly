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
#include <pic18.h>        //����ͷ�ļ�������ȥPICC18�����ȥ����PIC18F4550.H

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
//����: ��ʱ����
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
	
    //	delay(25,250);          //��ʱ
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
