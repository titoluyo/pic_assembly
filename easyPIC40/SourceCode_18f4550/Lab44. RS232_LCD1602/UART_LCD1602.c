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
#include <pic18.h>        //����ͷ�ļ�������ȥPICC18�����ȥ����PIC18F4550.H

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
	TRISD7=1;	   		   //Ϊ��״̬��׼������RD7��Ϊ����
   	RSPIN=0;			   //ѡ��ָ��Ĵ���
   	RWPIN=1;			   //ѡ���
   	EPIN=1;				   //ʹ���ߵ�ƽ�仯
   	while(RD7==1);         //��æ״̬����æʱ�˳�
   	EPIN=0;				   //�ָ�ʹ���ߵ�ƽ
   	TRISD7=0;			   //��RD7����Ϊ���
}

void lcd_write_com(unsigned char combuf)
{  
	RSPIN=0;          	//ѡ��ָ��Ĵ���
   	RWPIN=0;           	//ѡ��д
   	PORTD=combuf;       //������������RD 	
   	EPIN=1;	            //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�
   	asm("NOP");         //��һ���ղ���������ʱƬ��
   	EPIN=0;	            //�ָ�ʹ���ߵ�ƽ
}

void lcd_write_com_busy(unsigned char combuf)
{  
	lcd_wait_busy();		//����æ��⺯��
   	lcd_write_com(combuf); 	//����д�����
}

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

void lcd_write_address(unsigned char x,unsigned char y)
{  
	x&=0x0f;			//�е�ַ������0-15
   	y&=0x01;			//�е�ַ������0-1
   	if(y==0x00)
      	lcd_write_com_busy(x|0x80);	   //��һ�е��е�ַд��
   	else
      	lcd_write_com_busy((x+0x40)|0x80);	   //�ڶ��е��е�ַд��
}

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
	
void lcd_write_char(unsigned char x,unsigned char y,unsigned char buf)
{  		
	lcd_write_address(x,y); 	//д���ַ			
   	lcd_write_data(buf);		//д����ʾ����    
}  

void sci_Init(void)
{
	BRGH = 1;	// high baud rate 
	//SPBRG =25;  // ���ò�����9600 
	SPBRG =129;  // �ⲿ����20MHZ,���ò�����9600 
	SYNC = 0;	// 1=ͬ�����շ�ʽ 0=�첽���շ�ʽ 
	SPEN = 1;	// ����ʹ��λ 
	CREN = 1;	// 1=������������ 0=��ֹ�������� 
	SREN = 0;	// �첽��ʽ����λδ�� 
	TXIE = 0;	// �رշ��� 
	RCIE = 0;	// �رս����ж� 
	TX9  = 0;	// 1��ѡ��9λ���� 0��ѡ��8λ���� 
	RX9  = 0;	// 1��ѡ��9λ���� 0��ѡ��8λ���� 
	TXEN = 1;	// �������� 
	TRISC6=1;   // ��Ϊ����ʹ�ã�RC6��������Ϊ����
	TRISC7=1;   // ��Ϊ����ʹ�ã�RC7��������Ϊ����	
}

void send232byte(unsigned char bytebuf)
{
	TXREG=bytebuf;
	while(TRMT==1);
		
}

unsigned char WaitComm(void)
{   	
	if(RCIF==1)           //������ձ�־λΪ1������������Ҫ����
    {        		
		recebuf=RCREG;	  //�����������ݽ�������	
	  	if(OERR)	      //������������
		{
			CREN = 0;     //����CRENλ�ɽ���λOERR����
			CREN = 1;		
		}	 
		return 1; 	       //������Ϻ󣬷��ر�־1
	}	
	return 0;              //û�н��գ����ر�־0	
}




void main(void)	
{
	unsigned char buf;
	OSCCON=0X00;    		//ѡ��������
	ADCON1=0X0f;            //����IO��Ϊ���ֿڣ�ģ���������ֹ
  	TRISA=0B11000000;   	//RB3-5����Ϊ���
    TRISD=0B00000000;   	//RD����Ϊ���   		   
	lcdreset();  
  	sci_Init(); 
  	lcd_write_char(0,0,'R');   	//������(0,0)��д��R
	lcd_write_char(1,0,'E');   	//������(1,0)��д��e		
	lcd_write_char(2,0,'C');   	//������(2,0)��д��c
	lcd_write_char(3,0,'E');   	//������(3,0)��д��e	
	lcd_write_char(4,0,'I');   	//������(4,0)��д��i		
	lcd_write_char(5,0,'V');   	//������(5,0)��д��v
	lcd_write_char(6,0,'E');   	//������(6,0)��д��e

	lcd_write_char(8,0,'A');   	//������(8,0)��д��a

	lcd_write_char(10,0,'B');  	//������(10,0)��д��b
	lcd_write_char(11,0,'Y');  	//������(11,0)��д��y
	lcd_write_char(12,0,'T');  	//������(12,0)��д��t
	lcd_write_char(13,0,'E');  	//������(13,0)��д��e 
	

	lcd_write_char(0,1,'B');   	//������(0,1)��д��B
	lcd_write_char(1,1,'A');   	//������(1,1)��д��a		
	lcd_write_char(2,1,'U');   	//������(2,1)��д��u
	lcd_write_char(3,1,'D');   	//������(3,1)��д��d	

	lcd_write_char(5,1,'R');   	//������(5,1)��д��r
	lcd_write_char(6,1,'A');   	//������(6,1)��д��a
	lcd_write_char(7,1,'T');   	//������(7,1)��д��t
	lcd_write_char(8,1,'E');   	//������(8,1)��д��e
	lcd_write_char(9,1,':');   	//������(9,1)��д��:
	
	lcd_write_char(10,1,'9');   //������(10,1)��д��9
	lcd_write_char(11,1,'6');   //������(11,1)��д��6
	lcd_write_char(12,1,'0');   //������(12,1)��д��0
	lcd_write_char(13,1,'0');   //������(13,1)��д��0
	clrbit=1;			        //��������־λ,�Ա��ڵ�һ��
  	while(1)
  	{
 	
    	if(WaitComm()==1)				//������յ���λ������
		{  						
			if(clrbit)		        	//���Ϊ��һ���յ�����������������
			{
		 		clrbit=0;			 		
				lcdreset();         	//����Ļ 
			}							  
			lcd_write_char(0,0,'R');   	//������(0,0)��д��R
			lcd_write_char(1,0,'E');   	//������(1,0)��д��e		
			lcd_write_char(2,0,'C');   	//������(2,0)��д��c
			lcd_write_char(3,0,'E');   	//������(3,0)��д��e
			lcd_write_char(4,0,':');   	//������(4,0)��д��:
			lcd_write_char(6,0,'0');   	//������(6,0)��д��0
			lcd_write_char(7,0,'X');   	//������(7,0)��д��x
			buf=recebuf>>4;            	//�յ������ݸ�4λ����BUF
			if(buf<10)                 	//��Ϊ1602��Ҫ����ASCII�룬��Ҫ����ת��
				buf+=0X30;          	//���0-9��ֱ�Ӽ���'0'0X30
			else
				buf+=0x37;             	//���A-F��ֱ�Ӽ���0X37
			lcd_write_char(8,0,buf);   	//������(8,0)��д�������
			buf=recebuf&0x0f;          	//�յ������ݵ�4λ����BUF
			if(buf<10)                 	//��Ϊ1602��Ҫ����ASCII�룬��Ҫ����ת��
				buf+=0X30;          	//���0-9��ֱ�Ӽ���'0'0X30
			else
				buf+=0x37;             	//���A-F��ֱ�Ӽ���0X37
			lcd_write_char(9,0,buf);   	//������(9,0)��д�������	

			recebuf+=1;                 //��һ�μ�һ����
			send232byte(recebuf);		//�ظ�����Ϊ��������
		
			lcd_write_char(0,1,'S');   	//������(0,1)��д��S
			lcd_write_char(1,1,'E');   	//������(1,1)��д��e		
			lcd_write_char(2,1,'N');   	//������(2,1)��д��n
			lcd_write_char(3,1,'D');   	//������(3,1)��д��d
			lcd_write_char(4,1,':');   	//������(4,1)��д��:
			lcd_write_char(6,1,'0');   	//������(6,1)��д��0
			lcd_write_char(7,1,'X');   	//������(7,1)��д��x
			
			buf=recebuf>>4;        		//���͵����ݸ�4λ����BUF
			if(buf<10)                 	//��Ϊ1602��Ҫ����ASCII�룬��Ҫ����ת��
				buf+=0X30;          	//���0-9��ֱ�Ӽ���'0'0X30
			else
				buf+=0x37;             	//���A-F��ֱ�Ӽ���0X37
			lcd_write_char(8,1,buf);   	//������(8,1)��д�������
			buf=recebuf&0x0f;      		//���͵����ݵ�4λ����BUF
			if(buf<10)                 	//��Ϊ1602��Ҫ����ASCII�룬��Ҫ����ת��
				buf+=0X30;          	//���0-9��ֱ�Ӽ���'0'0X30
			else
				buf+=0x37;             	//���A-F��ֱ�Ӽ���0X37
			lcd_write_char(9,1,buf);   	//������(9,1)��д�������				
		}	           	
  	}
}
