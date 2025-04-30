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
#include <pic18.h>        //调用头文件，可以去PICC18软件下去查找PIC18F4550.H

__CONFIG(1,HS) ;  //EXT 20MHz,No PLL
__CONFIG(2,WDTDIS) ;      //WDT OFF
__CONFIG(3,PBADDIS) ;     //PORTB<4:0> configured as digital I/O on RESET 
__CONFIG(4,LVPDIS&XINSTDIS) ;     //Low Voltage Programming Disabled ,Disable extended instruction set (Legacy mode) 

#define beep   RD2 //定义蜂鸣器接口

unsigned char tmr_times;
unsigned char code_data;
unsigned char code;
unsigned char int_times;
unsigned char buf1;
unsigned int  fengcon;
bit head_ok;  	
bit beepbit;



void init_timer0(void)
{
	TMR0IF=0;    //T0中断标志位清零
    TMR0IE=1;    //T0中断允许
    T0CON=0xC8;  //8bit定时方式，预分频关闭，	
	TMR0L=0x13;  //晶振4.0M,定时250us
    GIE=1;   	 //开总中断
}


void wrong(void)  //如果有错误，将相应寄存器清零，准备下一次接收
{
  	int_times=0;
  	tmr_times=0;
  	head_ok=0;
  	code_data=0;  	
}


unsigned char one_zero(void)
{  	
	//参看遥控器芯片HT6221的说明书，0编码的脉冲周期为1.12ms
	//本例中，时间中断为0.25ms,所以我们判定取大于0.75ms小于1.5ms的脉冲均为0编码
  	if((tmr_times>3)&(tmr_times<6))
  	{
    	return 0;
  	}
	//参看遥控器芯片HT6221的说明书，1编码的脉冲周期为2.24ms
	//本例中，时间中断为0.25ms,所以我们判定取大于1.75ms小于2.75ms的脉冲均为0编码
  	else if((tmr_times>7)&(tmr_times<11))
  	{
    	return 1;
  	}
	//如果脉冲周期不在0.75-1.5ms或者1.75-2.75ms区间的，我们认为是错码，返回2
  	return 2;
}


void ccp2_int(void)
{
	CCP2CON=0x04;			//捕捉模式，捕捉每个下降沿
	T1CON=0x00;				//CCP要使用捕捉特性，T1必须运行在定时工作或者同步计数器模式下
	TRISC1=1;               //RC1设置为输入状态，该引脚接到了红外接收头的数据引脚上
	CCP2IF=0;               //捕捉中断标志位清零
	CCP2IE=1;				//该捕捉允许中断
	TMR1ON=1;	            //T1启动
}


void interrupt ISR(void)    //中断处理函数
{	
		
  	if(TMR0IF==1)           //如果是T0中断标志位为1，即250US时间到并引发中断
  	{
		TMR0L=0x13;  //晶振4.0M,定时250us
  		TMR0IF=0;           //清除TO中断标志位  		
  		tmr_times++;	    //记录中断的次数，依据此来判断接收编码的状态
  	}
	 if(CCP2IF==1)          //外部中断标志位如果为1，即为CCP2捕捉中断，转入处理程序
	 {
		CCP2IF=0;            //清除CCP2捕捉中断标志位      
		GIE=0; 				 //全局中断关掉，以避免解码过程进入其他中断	 
  		int_times++;  		 //捕捉中断计次	
  		if(head_ok==0)      	//如果还没接收到头码，那么先判断脉冲编码是否为头码
  		{
			//参看遥控器芯片HT6221的说明书，发射代码的头码由一个9ms的起始码和一个4.5ms的结果码组成
			//本例中，时间中断为0.25ms,所以我们判定大于12.5ms小于14.5ms的脉冲均为13.5ms的头码编码
    		if(tmr_times>45&tmr_times<58)		
    		{
      			head_ok=1;  	//有效头码标志位置一，以示头码接收完成
      			int_times=0;    //外部中断计次寄存器清零，以准备接收后面的4组共32个脉冲
      			tmr_times=0;    //定时器中断计次寄存器清零，准备开始对后面的脉冲计时	       
    		}
    		else
    		{
      			wrong();        //如果不是有效的头码，转入错误处理程序
    		}
  		}
  		else                	//如果已经接收到头码，那么开始对后面的32个脉冲进行解码                    
  		{    
    		code_data=code_data>>1; //将接收缓冲区右移一位，以准备接收下一位
			buf1=one_zero();    		//判断编码是0或1，或者是错码
			tmr_times=0;  		    //时间中断计次寄存器清零			
    		if(buf1==1)			//如果是编码1
    		{
      			code_data|=0x80;  	//将1移入code_data的最高位
    		}    	
			else if(buf1==0)			//如果是编码0
	        {
      			code_data&=0x7f;  	//将0移入code_data的最高位
    		}    
			else            		//编码非0非1，即为错码，
            {
				wrong();       		//调用错误处理程序
				return;
			}    						
    		if(int_times==8)        //如果收到了8个编码脉冲，意味着低8位地址码已经接收完
    		{
				//本遥控器的低8位地址码为0X00，这个是遥控器厂家决定的，如果要订做，可以随便改
        		if(code_data!=0x00)  //如果不是0x00 ，说明不是我们提供的遥控器，我们不予识别
        		{
        			wrong();         //调用错误处理程序      
					return;  
        		}    		
    		}
    		else if(int_times==16)        //如果收到了16个编码脉冲，意味着高8位地址码已经接收完
    		{
				//本遥控器的低高8位地址码为0XFF
        		if(code_data!=0xFF)  //如果不是0xff ，说明不是我们提供的遥控器，我们不予识别
        		{
            		wrong();         //调用错误处理程序      
					return;
        		}	        			
    		}
    		else if(int_times==24)    //收到的第三组8位字节即为8位数据码
    		{
      			code=code_data;    //将这个8位数据码暂存至code_data1  			
    		}
    		else if(int_times==32)    //收到的第四组8位字节即为8位数据码的反码
    		{
      			int_times=0;      	  //清零中断次数寄存器，以便下次从头开始		
      			head_ok=0;            //将头码标志位清零，以便下次再接收时，从头码开始			 			  
      			if((code+code_data)==0xff)    //判断第三组数据和第四组数据是否互为反码  			
      			{          			
            		beepbit=1;            	  //如果互为反码，说明接收来的编码为正确，启动蜂鸣器	
            	}      			
    		}
  		}   		
  		  		
  	} 
  	GIE=1;   	//开放全局中断
}


void fengpro(void)
{
	if(beepbit==1)
	{		
		beep=1;
		if(++fengcon>1000)
		{
			fengcon=0;
			beepbit=0;			
		}		 	
	}	
	else
	{
		beep=0;
	}
}


main(void)
{
  	unsigned char buf;
	OSCCON=0X00;    		//选择主震荡器
	ADCON1=0X0f;            //所有IO均为数字口，模拟量输入禁止
	TRISC=0B11111111;   	//Rc设置为输出入
    TRISD=0B11111011;   	//RD2设置为输出
  	init_timer0();          //TO初始化函数
  	ccp2_int();	            //CCP2初始化函数  
	beepbit=1;	
  	while(1)
  	{ 
    	fengpro();	
  	}
}
